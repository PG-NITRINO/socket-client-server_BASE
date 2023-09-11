#include "QueryParse.h"

//Разбиваем запрос и пытаемся понять, что от нас хотят
void QueryParse(char* buf, int len, QueryResult& queryResult)
{
	QueryState QS = QueryState::start;
	std::string str_buf = "";

	for( int i = 0; i < len; i++)
	{
		*(buf + i) = (char)tolower(*(buf + i));		//Ключевые слова приводим к нижнему регистру

		std::cout << *(buf + i);

		if (*(buf + i) != '/')
		{
			//str_buf += query[i];
			str_buf += *(buf + i);
		}
		else
		{//Дошли до '/' и в str_buf теперь часть запроса
			//Пошли обрабатвать состояния(все дружно вспомнили курсовую по компиляторам)
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
						queryResult.error = "Некорректный запрос (нет api)";
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
						{//Логин и пароль регистрозависимые
							std::cout << *(buf + i);

							if (QS == QueryState::registration_login)
							{//Считываем логин
								if (*(buf + i) != '&')
								{
									str_buf += *(buf + i);
								}
								else
								{
									if (str_buf == "")
									{//Отсутствует логин
										queryResult.queryType = QueryType::error;
										queryResult.error = "Некорректный запрос (нет логина)";
									}
									login = str_buf;
									QS = QueryState::registration_password;
									str_buf.clear();
								}
							}
							else if( QS == QueryState::registration_password)
							{//Считываем пароль
								str_buf += *(buf + i);

								if ( i == len - 1)		//Последний символ запроса
								{
									if (str_buf == "")
									{//Отсутствует пароль
										queryResult.queryType = QueryType::error;
										queryResult.error = "Некорректный запрос (нет пароля)";
										return;
									}
									password = str_buf;
									QS = QueryState::end;
									str_buf.clear();

									//Получили полный и корректный запрос на регистрацию
									queryResult.queryType = QueryType::registration;
									queryResult.login = login;
									queryResult.password = password;

									return;
								}
							}
							else
							{
								queryResult.queryType = QueryType::error;
								queryResult.error = "Некорректный запрос (неизвестное состояние)";
							}
						}

						if (queryResult.queryType = QueryType::unknow)
						{
							//Дошли до конца запроса и не встретили '&' после логина
							queryResult.queryType = QueryType::error;
							queryResult.error = "Некорректный запрос (Ты случайно '&' не забыл?)";
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
						{//Логин и пароль регистрозависимые
							std::cout << *(buf + i);

							if (QS == QueryState::authentication_login)
							{//Считываем логин
								if (*(buf + i) != '&')
								{
									str_buf += *(buf + i);
								}
								else
								{
									if (str_buf == "")
									{//Отсутствует логин
										queryResult.queryType = QueryType::error;
										queryResult.error = "Некорректный запрос (нет логина)";
									}
									login = str_buf;
									QS = QueryState::authentication_password;
									str_buf.clear();
								}
							}
							else if (QS == QueryState::authentication_password)
							{//Считываем пароль
								str_buf += *(buf + i);

								if (i == len - 1)		//Последний символ запроса
								{
									if (str_buf == "")
									{//Отсутствует пароль
										queryResult.queryType = QueryType::error;
										queryResult.error = "Некорректный запрос (нет пароля)";
										return;
									}
									password = str_buf;
									QS = QueryState::end;
									str_buf.clear();

									//Получили полный и корректный запрос на регистрацию
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
								queryResult.error = "Некорректный запрос (неизвестное состояние)";
							}
						}

						if (queryResult.queryType = QueryType::unknow)
						{
							//Дошли до конца запроса и не встретили '&' после логина
							queryResult.queryType = QueryType::error;
							queryResult.error = "Некорректный запрос (Ты случайно '&' не забыл?)";
						}
					}
					else
					{
						queryResult.queryType = QueryType::error;
						queryResult.error = "Некорректный запрос (Ты что от меня хочешь?)";
					}
					break;					
			}
		}
	}

	std::cout << "\nTHE END??????????????????????\n";

	if (queryResult.queryType = QueryType::unknow)
	{
	//Прошли весь запрос и не встретили ни одной '/'
	queryResult.queryType = QueryType::error;
	queryResult.error = "Некорректный запрос (УЖС какой-то), str_buf == " + str_buf;
	}
	
}