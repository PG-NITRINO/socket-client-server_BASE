#undef UNICODE

#define WIN32_LEAN_AND_MEAN

//��� winsock
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

//������������
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

//��� SQLite
//�Ψ
#include "QuerySQL.h" //������ sqlite3.h � QueryParse.h
#include "Responses.h"


// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define PORT "9034"
// ���� ���� �� �������

// �������� sockaddr, IPv4 ��� IPv6:
void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//������� ��������� ������
//���������� �������� �������
BOOL WINAPI MyHandleRoutine(DWORD dwCtrlTpe);

//���������, ����� �� ��� �������� �����������
sqlite3* db = 0; //����� ������� ���������� � ��

int main(void)
{
    setlocale(LC_ALL, "Rus");

    /* ������� � �������������� */
    const char* SQL_command_create_users_table = 
        "CREATE TABLE IF NOT EXISTS users( "
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "login NVARCHAR(50) NOT NULL,"
        "password NVARCHAR(50) NOT NULL"
        "); ";
    /* ������� ������� � ������� �������� ����������, ������ ���� ������ ��� ������ ������� */
    const char* SQL_command_create_JWTs_table =
        "CREATE TABLE IF NOT EXISTS JWTs( "
        "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "login NVARCHAR(50) NOT NULL,"
        "jwt NVARCHAR(50) NOT NULL"
        "); "
        "DELETE FROM JWTs;";

    //����������� � SQLite
    char* err = 0;
    //��������� ����������
    if (sqlite3_open("Messenger.db", &db))
    {
        std::cout << "������ ��������/�������� ��: " << sqlite3_errmsg(db);
    }

    //����� ��������� ���������� � ��
    //������������� ���������� ���������� �� ������ �������� �������
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)MyHandleRoutine, TRUE))
    {
        std::cout << "���������� MyHandleRoutine ����������.\n\n";
    }
    else std::cout << "���������� MyHandleRoutine �� ����������!\n\n";

    //��������� SQL(��������� ������� � ��������������, ���� � ��� ���)
    if (sqlite3_exec(db, SQL_command_create_users_table, 0, 0, &err))
    {
        std::cout << "������ SQL: " << err;
        sqlite3_free(err);          //����������� ������
    }
    //���������� ����������� � ����� ���������

    //������� ������� �������������
    sqlite3_stmt* stmt;
    const char* sql_line = "SELECT login, password FROM users;";

    int rc = sqlite3_prepare_v2(db, sql_line, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {//���-�� �� ��� � ��������
        //queryResult.error = sqlite3_errmsg(db);
        std::cout << sqlite3_errmsg(db) << "\n";
        return -1;
    }
    std::cout << "������ �������������: \n";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::cout << sqlite3_column_text(stmt, 0) << " "
            << sqlite3_column_text(stmt, 1) << "\n";
    }
    if (rc != SQLITE_DONE)
    {//������ �� ����������
        std::cout << sqlite3_errmsg(db) << "\n";
        return -1;
    }
    sqlite3_finalize(stmt);

    //��������� SQL(��������� ������� � ������������������� �������������, ���� � ��� ���, � ����� ������� �)
    if (sqlite3_exec(db, SQL_command_create_JWTs_table, 0, 0, &err))
    {
        std::cout << "������ SQL: " << err;
        sqlite3_free(err);          //����������� ������
    }


    //��� �������� ��������
    QueryResult queryResult;

    WSADATA wsaData;    //��������� WSADATA �������� ���������� � ���������� ������� Windows.
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);     //������� WSAStartup ���������� ������������� Winsock DLL ���������.
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    fd_set master;
    // ������� ������ �������� ������������
    fd_set read_fds;
    // ��������� ������ �������� ������������ ��� select()
    int fdmax;
    // ������������ ����� ��������� �����������

    int listener;
    // ���������� ���������� ������
    int newfd;
    // ������������ ���������� ������
    struct sockaddr_storage remoteaddr; // ����� �������
    socklen_t addrlen;

    char buf[256];
    // ����� ��� ������ �������
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes = 1;
    // ��� setsockopt() SO_REUSEADDR, ����
    int i, j, rv;

    struct addrinfo hints, * ai, * p;

    FD_ZERO(&master);
    // ������� �������� � ���������� ��������
    FD_ZERO(&read_fds);

    // �������� ����� � �������
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

        // ������ ������� ��������� "address already in use"
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (char*) & yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            closesocket(listener);
            continue;
        }
        break;
    }

    // ���� �� �����, ������ �� ���������
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai);
    // � ���� ���������

    // �������
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // �������� ��������� � ������� ������
    FD_SET(listener, &master);

    // ��������� ���������� �������� ����������
    fdmax = listener;
    // ��� ��

    //��� ���������� �������� ������ �� �������
    std::string response;

    // ������� ����
    for (;;)
    {
        read_fds = master;
        // ��������
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
        }

        // ���� ������� ��� ������ ������ � ������������ ������������
        for (i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                // ����!!
                if (i == listener)
                {
                    // ������������ ����� �����������
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    }
                    else
                    {
                        FD_SET(newfd, &master);
                        // �������� � ������� ������
                        if (newfd > fdmax) {
                            // ����������� ������������ �����
                            fdmax = newfd;
                        }
                        printf("selectserver: new connection from %s on socket %d\n", inet_ntop(remoteaddr.ss_family,
                            get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN), newfd);
                    }
                }
                else
                {
                    // ��������� ������ �� �������
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        // ������ ��� ���������� ������� ��������
                        if (nbytes == 0) {
                            // ���������� �������
                            printf("selectserver: socket %d hung up\n", i);
                        }
                        else {
                            perror("recv");
                        }
                        closesocket(i);
                        // ����!
                        FD_CLR(i, &master);
                        // ������� �� �������� �������
                    }
                    else {
                        // �� ������� ���-�� ��������

                        QueryParse(buf, nbytes, queryResult);

                        if (queryResult.queryType == QueryType::registration)                            //������ �� �����������
                        {
                            if (RegistrationSQL(queryResult, db) == -1)
                            {
                                std::cout << queryResult.error << "  << RegistrationSQL  Error\n";
                            }
                        }
                        else if ((queryResult.queryType == QueryType::error) || (queryResult.queryType == QueryType::unknow))       //������������ ������
                        {
                            std::cout << queryResult.error << "  << QueryType::error  \n";
                        }
                        else if (queryResult.queryType == QueryType::authentication)
                        {
                            if (AuthenticationSQL(queryResult, db) == -1)
                            {
                                std::cout << queryResult.error << "  << AuthenticationSQL  Error\n";
                            }
                            else                                                                            //���� ��������� jwt � login
                            {
                                std::cout << "\n�������� �����?! jwt = " << queryResult.jwt.jwt << "\n";
                            }
                        }

                        //������������ ������ ������� � �������� ��� �������
                        response = FormationResponse(queryResult);
                        const char* c_response = response.c_str();
                        if (send(i, c_response, strlen(c_response), 0) == -1) {
                            perror("send");
                        }

                        //�������� ��������
                        /*for (j = 0; j <= fdmax; j++) {
                            // �������� ����!
                            if (FD_ISSET(j, &master)) {
                                // ����� ��������� � ����
                                if (j != listener && j != i) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        }*/
                    }
                } // END ��������� ������ �� �������
            } // END ���� ����� �������� �����������
        } // END ���� �� �������� ������������
    } // END for(;;)�� �� �������, ��� ��� �� ����������!

    //��������� ���������� � ��
    sqlite3_close(db);
    return 0;
}


//������� ��������� ������
//���������� �������� �������
BOOL WINAPI MyHandleRoutine(DWORD dwCtrlTpe)
{
    switch (dwCtrlTpe)
    {
    case CTRL_CLOSE_EVENT:
    {
        std::cout << "CTRL_CLOSE_EVENT";

        //������� ��� ������ �� ������� � ������������� ��������������
        const char* SQL_command_delete_JWTs = "DELETE FROM JWTs;";
        sqlite3_exec(db, SQL_command_delete_JWTs, 0, 0, NULL);

        //��������� ���������� � ��
        sqlite3_close(db);

        Beep(300, 200);
        return TRUE;
    }
    default: return FALSE;
    }
}