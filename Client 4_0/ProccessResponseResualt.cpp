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
                form->SetText_In_Labelmessage("�������� �����������");
                break;
            }
            case(1):
            {
                form->SetText_In_Labelmessage("����� ������������ ��� ����������");
                break;
            }
            case(2):
            {
                form->SetText_In_Labelmessage("������ �������� ������ (������� 5 ��������)");
                break;
            }
            case(3):
            {
                form->SetText_In_Labelmessage("����������� ������ �� �������");
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
                form->SetText_In_Labelmessage("�������� ��������������");
                break;
            }
            case(3):
            {
                form->SetText_In_Labelmessage("����������� ������ �� �������");
                break;
            }
            case(4):
            {
                form->SetText_In_Labelmessage("����� ��� ������ ��������");
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