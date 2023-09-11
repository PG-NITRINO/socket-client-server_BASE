#include "ExtraFunctions.h"

//Преобразует System::String^ в std::string&
void MarshalString(System::String^ s, std::string& os) {
    using namespace System::Runtime::InteropServices;
    const char* chars =
        (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
    os = chars;
    Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

void ResponseParse(const char* response, int len, ResponseResult& responseResult)
{
    ResponseState RS = ResponseState::start;
    std::string str_buf;
    responseResult.ClearResponseResult();       //Очищаем все поля


    for (int i = 0; i <= len; i++)
    {
        if ((*(response + i) != '/') && (*(response + i) != '\0'))
        {
            str_buf += *(response + i);
        }
        else
        {
            switch (RS)
            {
                case(ResponseState::start):
                {
                    if (str_buf == "registration")
                    {
                        responseResult.massage = "registration";
                        RS = ResponseState::registration;
                    }
                    else if (str_buf == "authentication")
                    {
                        responseResult.massage = "authentication";
                        RS = ResponseState::authentication;
                    }
                    else
                    {
                        responseResult.massage = "error 1";
                        responseResult.type = ResponseType::error;
                    }

                    str_buf.clear();
                    break;
                }

                case(ResponseState::registration):
                {
                    if (str_buf.size() > 1)     //Должен быть код из 1 цифры
                    {
                        responseResult.massage = "error 2";
                        responseResult.type = ResponseType::error;
                        break;
                    }

                    char code = str_buf[0];// *(response + (++i));
                    responseResult.massage += " code = ";
                    responseResult.massage += code;

                    responseResult.type = ResponseType::reg;
                    responseResult.code = (int)code - 48;       // Преобразуем символ в число
                    str_buf.clear();
                    break;
                }

                case(ResponseState::authentication):
                {
                    if (str_buf.size() > 1)     //Должен быть код из 1 цифры
                    {
                        responseResult.massage = "error 3";
                        responseResult.type = ResponseType::error;
                        break;
                    }

                    char code = str_buf[0];// *(response + (++i));
                    responseResult.massage += " code = ";
                    responseResult.massage += code;

                    responseResult.type = ResponseType::aut;
                    responseResult.code = (int)code - 48;       // Преобразуем символ в число

                    str_buf.clear();
                    RS = ResponseState::login;
                    if (responseResult.code == 0)   //Успех, ждём логин и jwt
                    {
                        i++;
                        for (; i < len; i++)
                        {
                            if ((*(response + i) != '&') && (*(response + i) != '\0'))
                            {
                                str_buf += *(response + i);
                            }
                            else
                            {
                                if (RS == ResponseState::login)
                                {
                                    responseResult.login = str_buf;
                                    str_buf.clear();
                                    RS = ResponseState::jwt;
                                }
                                else if (RS == ResponseState::jwt)
                                {
                                    responseResult.jwt = str_buf;
                                }
                            }
                        }
                    }

                    break;
                }
            }

            if (responseResult.type == ResponseType::error)
            {
                break;                  //Выход из обработки с ошибкой
            }
        }
    }
}