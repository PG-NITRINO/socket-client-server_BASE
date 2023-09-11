#pragma once
#include "sqlite3.h"
#include "QueryParse.h"

//Функции для обработки парсинга и определения результатов запроса

//Обработка запроса на регистрацию
int RegistrationSQL(QueryResult& queryResult, sqlite3* db);
//Обработка запроса на аутентификацию
int AuthenticationSQL(QueryResult& queryResult, sqlite3* db);