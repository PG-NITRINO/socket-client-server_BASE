#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//Для winsock
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//Доплнительно
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//Для SQLite
//МОЁ
#include "QuerySQL.h" //Внутри sqlite3.h и QueryParse.h
#include "Responses.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define PORT "9034"
// этот порт мы слушаем

// получить sockaddr, IPv4 или IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//Функция обратного вызова
//Обработчик закрытия консоли
BOOL WINAPI MyHandleRoutine(DWORD dwCtrlTpe);

//Глобально, чтобы он был доступен обработчику
sqlite3* db = 0; //хэндл объекта соединения с БД

int main(void)
{
    setlocale(LC_ALL, "Rus");

    /* Таблица с пользователями */
    const char* SQL_command_create_users_table = 
        "CREATE TABLE IF NOT EXISTS users( "
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "login NVARCHAR(50) NOT NULL,"
        "password NVARCHAR(50) NOT NULL"
        "); ";
    /* Таблица логинов и токенов активных соединений, должна быть пустой при старте сервера */
    const char* SQL_command_create_JWTs_table =
        "CREATE TABLE IF NOT EXISTS JWTs( "
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "login NVARCHAR(50) NOT NULL,"
        "jwt NVARCHAR(50) NOT NULL"
        "); "
        "DELETE FROM JWTs;";

    //Соединяемся с SQLite
    char* err = 0;
    //Открываем соединение
    if (sqlite3_open("Messenger.db", &db))
    {
        std::cout << "Ошибка открытия/создания БД: " << sqlite3_errmsg(db);
    }

    //После успешного соединения с БД
    //Устанавливаем обработчик управления на случай закрытия консоли
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)MyHandleRoutine, TRUE))
    {
        std::cout << "Обработчик MyHandleRoutine установлен.\n\n";
    }
    else std::cout << "Обработчик MyHandleRoutine НЕ установлен!\n\n";

    //Выполняем SQL(Добавляем таблицу с пользователями, если её ещё нет)
    if (sqlite3_exec(db, SQL_command_create_users_table, 0, 0, &err))
    {
        std::cout << "Ошибка SQL: " << err;
        sqlite3_free(err);          //Освобаждает память
    }
    //Соединения закрывается в конце программы

    //Выводим списвок пользователей
    sqlite3_stmt* stmt;
    const char* sql_line = "SELECT login, password FROM users;";

    int rc = sqlite3_prepare_v2(db, sql_line, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {//Что-то не так с запросом
        //queryResult.error = sqlite3_errmsg(db);
        std::cout << sqlite3_errmsg(db) << "\n";
        return -1;
    }
    std::cout << "Список пользователей: \n";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::cout << sqlite3_column_text(stmt, 0) << " "
            << sqlite3_column_text(stmt, 1) << "\n";
    }
    if (rc != SQLITE_DONE)
    {//Запрос не завершился
        std::cout << sqlite3_errmsg(db) << "\n";
        return -1;
    }
    sqlite3_finalize(stmt);

    //Выполняем SQL(Добавляем таблицу с аутентифицированных пользователей, если её ещё нет, и сразу очищаем её)
    if (sqlite3_exec(db, SQL_command_create_JWTs_table, 0, 0, &err))
    {
        std::cout << "Ошибка SQL: " << err;
        sqlite3_free(err);          //Освобаждает память
    }


    //Для парсинга запросов
    QueryResult queryResult;

    WSADATA wsaData;    //Структура WSADATA содержит информацию о реализации сокетов Windows.
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);     //Функция WSAStartup инициирует использование Winsock DLL процессом.
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    fd_set master;
    // главный список файловых дескрипторов
    fd_set read_fds;
    // временный список файловых дескрипторов для select()
    int fdmax;
    // максимальный номер файлового дескриптора

    int listener;
    // дескриптор слушаемого сокета
    int newfd;
    // новопринятый дескриптор сокета
    struct sockaddr_storage remoteaddr; // адрес клиента
    socklen_t addrlen;

    char buf[256];
    // буфер для данных клиента
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes = 1;
    // для setsockopt() SO_REUSEADDR, ниже
    int i, j, rv;

    struct addrinfo hints, * ai, * p;

    FD_ZERO(&master);
    // очистка главного и временного массивов
    FD_ZERO(&read_fds);

    // получить сокет и связать
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // убрать мерзкое сообщение "address already in use"
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*) & yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            closesocket(listener);
            continue;
        }
        break;
    }

    // если мы здесь, значит не связались
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai);
    // с этим закончили

    // слушаем
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // добавить слушателя в главный массив
    FD_SET(listener, &master);

    // сохранить наибольший файловый дескриптор
    fdmax = listener;
    // вот он

    //Для временного хранения ответа от сервера
    std::string response;

    // главный цикл
    for (;;)
    {
        read_fds = master;
        // копируем
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // ищем готовые для чтения данные в существующих подключениях
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                // Есть!!
                if (i == listener)
                {
                    // обрабатываем новые подключения
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        // добавить в главный массив
                        if (newfd > fdmax) {
                            // отслеживаем максимальный номер
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family,
                            get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
                    }
                }
                else
                {
                    // обработка данных от клиента
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        // ошибка или соединение закрыто клиентом
                        if (nbytes == 0) {
                            // соединение закрыто
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else {
                            perror("recv");
                        }
                        closesocket(i);
                        // Пока!
                        FD_CLR(i, &master);
                        // удалить из главного массива
                    }
                    else {
                        // от клиента что-то получили

                        QueryParse(buf, nbytes, queryResult);

                        if (queryResult.queryType == QueryType::registration)                            //Запрос на регистрацию
                        {
                            if (RegistrationSQL(queryResult, db) == -1)
                            {
                                std::cout << queryResult.error << "  << RegistrationSQL  Error\n";
                            }
                        }
                        else if ((queryResult.queryType == QueryType::error) || (queryResult.queryType == QueryType::unknow))       //Некорректный запрос
                        {
                            std::cout << queryResult.error << "  << QueryType::error  \n";
                        }
                        else if (queryResult.queryType == QueryType::authentication)
                        {
                            if (AuthenticationSQL(queryResult, db) == -1)
                            {
                                std::cout << queryResult.error << "  << AuthenticationSQL  Error\n";
                            }
                            else                                                                            //Надо отправить jwt и login
                            {
                                std::cout << "\nУспешный успех?! jwt = " << queryResult.jwt.jwt << "\n";
                            }
                        }

                        //Формирование ответа сервера и отправка его клиенту
                        response = FormationResponse(queryResult);
                        const char* c_response = response.c_str();
                        if (send(i, c_response, strlen(c_response), 0) == -1) {
                            perror("send");
                        }

                        //Массовая рассылка
                        /*for (j = 0; j <= fdmax; j++) {
                            // посылаем всем!
                            if (FD_ISSET(j, &master)) {
                                // кроме слушателя и себя
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }*/
                    }
                } // END обработка данных от клиента
            } // END есть новое входящее подключение
        } // END цикл по файловым дескрипторам
    } // END for(;;)—и вы думаете, что это не закончится!

    //Закрываем соединение с БД
    sqlite3_close(db);
    return 0;
}


//Функция обратного вызова
//Обработчик закрытия консоли
BOOL WINAPI MyHandleRoutine(DWORD dwCtrlTpe)
{
    switch (dwCtrlTpe)
    {
    case CTRL_CLOSE_EVENT:
    {
        std::cout << "CTRL_CLOSE_EVENT";

        //Удаляем все данные из таблицы с подключёнными пользователями
        const char* SQL_command_delete_JWTs = "DELETE FROM JWTs;";
        sqlite3_exec(db, SQL_command_delete_JWTs, 0, 0, NULL);

        //Закрываем соединение с БД
        sqlite3_close(db);

        Beep(300, 200);
        return TRUE;
    }
    default: return FALSE;
    }
}