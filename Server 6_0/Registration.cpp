#include "QuerySQL.h"

//� ������ ������ ���������� -1 � ��������� ���� queryResult.error
int RegistrationSQL(QueryResult& queryResult, sqlite3* db)
{
	if (queryResult.queryType != QueryType::registration)
	{
		queryResult.error = "�������������� ��� (RegistrationSQL)";
		queryResult.returnCode = ReturnCode::unknowError;
		return -1;
	}
	if (queryResult.password.length() < 5)
	{
		queryResult.error = "������� �������� ������ (������� 5 ��������)";
		queryResult.returnCode = ReturnCode::shortPassword;
		return -1;
	}

	char* log = &*queryResult.login.begin();

	if (queryResult.password[queryResult.password.size()] == '\r\n')
		queryResult.password.erase(queryResult.password.size());

	char* pass = &*queryResult.password.begin();

	//��������  �� ���������� ������
	bool unique = true;		//����� �������� ����������
	sqlite3_stmt* stmt;
	char sql_line[1000] = "SELECT login FROM users WHERE users.login = '";
	strcat_s(sql_line, log);
	strcat_s(sql_line, "';");
		
	std::cout << "\nReqest(��������  �� ���������� ������): " << sql_line << "\n";

	int rc = sqlite3_prepare_v2(db, sql_line, -1, &stmt, NULL);

	if (rc != SQLITE_OK)
	{//���-�� �� ��� � ��������
		queryResult.error = sqlite3_errmsg(db);
		queryResult.returnCode = ReturnCode::unknowError;
		return -1;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		unique = false;
	}

	if (rc != SQLITE_DONE)
	{//������ �� ����������
		queryResult.error = sqlite3_errmsg(db);
		queryResult.returnCode = ReturnCode::unknowError;
		return -1;
	}
	sqlite3_finalize(stmt);

	if (!unique)
	{
		queryResult.error = "����� ������������ ��� ����������";
		queryResult.returnCode = ReturnCode::userExists;
		return -1;
	}

	//������� ������ ������������
    char SQL_command[1000] = "INSERT INTO users(login, password) VALUES('";
	strcat_s(SQL_command, log);
	strcat_s(SQL_command, "', '");
	strcat_s(SQL_command, pass);
	strcat_s(SQL_command, "');");

	std::cout << "\nReqest(������� ������ ������������): " << SQL_command << "\n";

	char* err;
	if (sqlite3_exec(db, SQL_command, 0, 0, &err))
	{
		queryResult.error = err;
		sqlite3_free(err);
		queryResult.returnCode = ReturnCode::unknowError;
		return -1;
	}

	queryResult.returnCode = ReturnCode::success;
	return 0;
}