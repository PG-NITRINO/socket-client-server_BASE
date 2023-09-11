#include "QuerySQL.h"

int AuthenticationSQL(QueryResult& queryResult, sqlite3* db)
{
	if (queryResult.queryType != QueryType::authentication)
	{
		queryResult.error = "Несоотвествует тип (AuthenticationSQL)";
        queryResult.returnCode = ReturnCode::unknowError;
		return -1;
	}

	char* log = &*queryResult.login.begin();

    if (queryResult.password[queryResult.password.size()] == '\r\n')
        queryResult.password.erase(queryResult.password.size());

	char* pass = &*queryResult.password.begin();

    /* Ищем совпадения */
    sqlite3_stmt* stmt;

    char SQL_command[1000] = "SELECT id FROM users WHERE users.login = '";
    strcat_s(SQL_command, log);
	strcat_s(SQL_command, "' AND users.password = '");
	strcat_s(SQL_command, pass);
	strcat_s(SQL_command, "';");

    std::cout << "\nReqest(Ищем совпадения): " << SQL_command << "\n";

    const char* sql_line = SQL_command;

    int rc = sqlite3_prepare_v2(db, sql_line, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {//Что-то не так с запросом
        std::cout << sqlite3_errmsg(db) << "\n";
        queryResult.returnCode = ReturnCode::unknowError;
        return -1;
    }

    bool find = false;  //Такой пользователь не существует
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        std::cout << "\nvremeno2 user find\n";
        find = true;
    }

    if (rc != SQLITE_DONE)
    {//Запрос не завершился
        queryResult.error = sqlite3_errmsg(db);
        queryResult.returnCode = ReturnCode::unknowError;
        return -1;
    }
    sqlite3_finalize(stmt);

    if (find)
    {//Пользователь нашёлся
        queryResult.jwt.jwt = queryResult.login + queryResult.password;

        //Добавляем запись о новом пользователе
        char SQL_command[1000] = "INSERT INTO JWTs(login, jwt) VALUES('";
        strcat_s(SQL_command, queryResult.login.c_str());
        strcat_s(SQL_command, "', '");
        strcat_s(SQL_command, queryResult.jwt.jwt.c_str());
        strcat_s(SQL_command, "');");

        char* err;
        if (sqlite3_exec(db, SQL_command, 0, 0, &err))
        {
            queryResult.error = err;
            sqlite3_free(err);
            queryResult.returnCode = ReturnCode::unknowError;
            return -1;
        }

        std::cout << "\nReqest(Добавляем запись о новом пользователе): " << SQL_command << "\n";
    }
    else
    {
        queryResult.error = "Неверный логин или пароль";
        queryResult.returnCode = ReturnCode::authentificationFalse;
        return -1;
    }
    queryResult.returnCode = ReturnCode::success;
    return 0;
}