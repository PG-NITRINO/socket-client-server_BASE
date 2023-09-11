#include "QueryParse.h"

//��������� ������ � �������� ������, ��� �� ��� �����
void QueryParse(char* buf, int len, QueryResult& queryResult)
{
	QueryState QS = QueryState::start;
	std::string str_buf = "";

	for( int i = 0; i < len; i++)
	{
		*(buf + i) = (char)tolower(*(buf + i));		//�������� ����� �������� � ������� ��������

		std::cout << *(buf + i);

		if (*(buf + i) != '/')
		{
			//str_buf += query[i];
			str_buf += *(buf + i);
		}
		else
		{//����� �� '/' � � str_buf ������ ����� �������
			//����� ����������� ���������(��� ������ ��������� �������� �� ������������)
			switch (QS)
			{
				case(QueryState::start) : 
					if (str_buf == "api")
					{
						QS = QueryState::api;
						str_buf.clear();
					}
					else
					{
						queryResult.queryType = QueryType::error;
						queryResult.error = "������������ ������ (��� api)";
					}
					break;

				case(QueryState::api) :
					if (str_buf == "registration")
					{
						QS = QueryState::registration_login;
						str_buf.clear();

						std::string login;
						std::string password;
						i++;

						for(; i < len; i++)
						{//����� � ������ �����������������
							std::cout << *(buf + i);

							if (QS == QueryState::registration_login)
							{//��������� �����
								if (*(buf + i) != '&')
								{
									str_buf += *(buf + i);
								}
								else
								{
									if (str_buf == "")
									{//����������� �����
										queryResult.queryType = QueryType::error;
										queryResult.error = "������������ ������ (��� ������)";
									}
									login = str_buf;
									QS = QueryState::registration_password;
									str_buf.clear();
								}
							}
							else if( QS == QueryState::registration_password)
							{//��������� ������
								str_buf += *(buf + i);

								if ( i == len - 1)		//��������� ������ �������
								{
									if (str_buf == "")
									{//����������� ������
										queryResult.queryType = QueryType::error;
										queryResult.error = "������������ ������ (��� ������)";
										return;
									}
									password = str_buf;
									QS = QueryState::end;
									str_buf.clear();

									//�������� ������ � ���������� ������ �� �����������
									queryResult.queryType = QueryType::registration;
									queryResult.login = login;
									queryResult.password = password;

									return;
								}
							}
							else
							{
								queryResult.queryType = QueryType::error;
								queryResult.error = "������������ ������ (����������� ���������)";
							}
						}

						if (queryResult.queryType = QueryType::unknow)
						{
							//����� �� ����� ������� � �� ��������� '&' ����� ������
							queryResult.queryType = QueryType::error;
							queryResult.error = "������������ ������ (�� �������� '&' �� �����?)";
						}
						
					}
					else if (str_buf == "authentication")
					{
						QS = QueryState::authentication_login;
						str_buf.clear();

						std::string login;
						std::string password;
						i++;

						for (; i < len; i++)
						{//����� � ������ �����������������
							std::cout << *(buf + i);

							if (QS == QueryState::authentication_login)
							{//��������� �����
								if (*(buf + i) != '&')
								{
									str_buf += *(buf + i);
								}
								else
								{
									if (str_buf == "")
									{//����������� �����
										queryResult.queryType = QueryType::error;
										queryResult.error = "������������ ������ (��� ������)";
									}
									login = str_buf;
									QS = QueryState::authentication_password;
									str_buf.clear();
								}
							}
							else if (QS == QueryState::authentication_password)
							{//��������� ������
								str_buf += *(buf + i);

								if (i == len - 1)		//��������� ������ �������
								{
									if (str_buf == "")
									{//����������� ������
										queryResult.queryType = QueryType::error;
										queryResult.error = "������������ ������ (��� ������)";
										return;
									}
									password = str_buf;
									QS = QueryState::end;
									str_buf.clear();

									//�������� ������ � ���������� ������ �� �����������
									queryResult.queryType = QueryType::authentication;
									queryResult.login = login;
									queryResult.password = password;

									std::cout << "\nVremenno!!!!!!!!!!! password = " << password << "\n";

									return;
								}
							}
							else
							{
								queryResult.queryType = QueryType::error;
								queryResult.error = "������������ ������ (����������� ���������)";
							}
						}

						if (queryResult.queryType = QueryType::unknow)
						{
							//����� �� ����� ������� � �� ��������� '&' ����� ������
							queryResult.queryType = QueryType::error;
							queryResult.error = "������������ ������ (�� �������� '&' �� �����?)";
						}
					}
					else
					{
						queryResult.queryType = QueryType::error;
						queryResult.error = "������������ ������ (�� ��� �� ���� ������?)";
					}
					break;					
			}
		}
	}

	std::cout << "\nTHE END??????????????????????\n";

	if (queryResult.queryType = QueryType::unknow)
	{
	//������ ���� ������ � �� ��������� �� ����� '/'
	queryResult.queryType = QueryType::error;
	queryResult.error = "������������ ������ (��� �����-��), str_buf == " + str_buf;
	}
	
}