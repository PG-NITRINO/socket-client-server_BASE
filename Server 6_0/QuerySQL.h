#pragma once
#include "sqlite3.h"
#include "QueryParse.h"

//������� ��� ��������� �������� � ����������� ����������� �������

//��������� ������� �� �����������
int RegistrationSQL(QueryResult& queryResult, sqlite3* db);
//��������� ������� �� ��������������
int AuthenticationSQL(QueryResult& queryResult, sqlite3* db);