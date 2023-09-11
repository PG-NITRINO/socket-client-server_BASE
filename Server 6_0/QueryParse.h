#pragma once

#include "MyTypes.h"

//Парсит запрос клиента и заполняет структуру
void QueryParse(char* buf, int len, QueryResult& queryResult);