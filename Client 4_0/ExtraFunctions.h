#pragma once
#include "MyTypes.h"

//�������� ����� �� s � os
void MarshalString(System::String^ s, std::string& os);

//������� ������ �������
void ResponseParse(const char* response, int len, ResponseResult& responseResult);