#pragma once
#include "ProccessResponseResualt.h"

// Подключение к серверу
// Принимает указатель на форму для вызова её методов
// Возвращает сокет сервера
// В случае неудачи возвращает INVALID_SOCKET
SOCKET ConnectToServer(Client40::MyForm^ form);

//Отключение от сервера
int DisconnectToServer(SOCKET ConnectSocket);

//Прослушиватель
//Ожидает ответа сервера
int Receiver(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);

//Регистрация
//ConnectSocket - сокет, связанный с сервером
int RegistrationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);

//Аутентификация
//ConnectSocket - сокет, связанный с сервером
int AuthenticationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);