#pragma once


#ifndef WinSockOn
#define WinSockOn 

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif

#include <string>

//Состояния обработки ответа сервера
enum ResponseState
{
	start,
	registration,
	authentication,
	login,
	jwt,
	end,
};

//Тип ответа сервера
enum ResponseType
{
	unknow,
	error,
	reg,
	aut,
};

//Структура для хранения результатов обработки ответов сервера
struct ResponseResult
{
	ResponseType type = ResponseType::unknow;
	int code = -1;
	std::string login = "";
	std::string jwt = "";
	std::string massage = "";

	void ClearResponseResult()
	{
		this->code = -1;
		this->jwt = "";
		this->login = "";
		this->massage = "";
		this->type = ResponseType::unknow;
	}
};
