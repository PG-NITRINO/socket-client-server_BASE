#include "ProccessResponseResualt.h"

void ProccessResponseResualt(Client40::MyForm^ form, ResponseResult& responseResult)
{
    switch (responseResult.type)
    {
    case(ResponseType::error):
    {
        form->SetText_In_Labelmessage("code = " + responseResult.code + " Error1 ");

        const char* c_errorMes_str = responseResult.massage.c_str();

        form->SetText_In_Labelmessage(c_errorMes_str);
        break;
    }
    case(ResponseType::reg):
    {
        switch (responseResult.code)
        {
            case(0): 
            {
                form->SetText_In_Labelmessage("Успешная регистрация");
                break;
            }
            case(1):
            {
                form->SetText_In_Labelmessage("Такой пользователь уже существует");
                break;
            }
            case(2):
            {
                form->SetText_In_Labelmessage("Сликом короткий пароль (минимум 5 символов)");
                break;
            }
            case(3):
            {
                form->SetText_In_Labelmessage("Неизвестная ошибка на сервере");
                break;
            }
            default:
                form->SetText_In_Labelmessage("code = " + responseResult.code + " Error2");
        }
        break;
    }
    case(ResponseType::aut):
    {
        switch (responseResult.code)
        {
            case(0):
            {
                form->SetText_In_Labelmessage("Успешная аутентификация");
                break;
            }
            case(3):
            {
                form->SetText_In_Labelmessage("Неизвестная ошибка на сервере");
                break;
            }
            case(4):
            {
                form->SetText_In_Labelmessage("Логин или пароль неверный");
                break;
            }
            default:
                form->SetText_In_Labelmessage("code = " + responseResult.code + " Error3");
        }
        break;
    }
    default:
    {
        form->SetText_In_Labelmessage("code = " + responseResult.code + " Error4 ");
        const char* c_errorMes_str = responseResult.massage.c_str();

        form->SetText_In_Labelmessage(c_errorMes_str);
    }
    }
}