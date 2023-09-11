#pragma once
#include <string>
#include <iostream>

enum QueryType
{
	unknow,
	error,
	registration,
	authentication
};

enum QueryState
{
	start,
	api,
	registration_login,
	registration_password,
	authentication_login,
	authentication_password,
	end
};

struct JWT
{
	std::string jwt;
};

enum ReturnCode
{
	unknowError,			//Неизвестная ошибка на сервере
	success,				//Успех
	userExists,				//Пользователь уже существует (регистрация)
	shortPassword,			//Короткий пароль (регистрация)
	authentificationFalse	//Неверный логин или пароль
};

struct QueryResult
{
public:
	QueryType queryType = QueryType::unknow;	//Обязательно к заполнению

	//Необязательные поля
	std::string login = "";
	std::string password = "";
	JWT jwt;
	std::string error = "";
	ReturnCode returnCode = ReturnCode::unknowError;
} typedef QueryResult;