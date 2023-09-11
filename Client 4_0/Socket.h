#pragma once
#include "ProccessResponseResualt.h"

// ����������� � �������
// ��������� ��������� �� ����� ��� ������ � �������
// ���������� ����� �������
// � ������ ������� ���������� INVALID_SOCKET
SOCKET ConnectToServer(Client40::MyForm^ form);

//���������� �� �������
int DisconnectToServer(SOCKET ConnectSocket);

//��������������
//������� ������ �������
int Receiver(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);

//�����������
//ConnectSocket - �����, ��������� � ��������
int RegistrationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);

//��������������
//ConnectSocket - �����, ��������� � ��������
int AuthenticationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult);