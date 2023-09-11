#pragma once
#include "MyTypes.h"

//Копирует текст из s в os
void MarshalString(System::String^ s, std::string& os);

//Парсинг ответа сервера
void ResponseParse(const char* response, int len, ResponseResult& responseResult);