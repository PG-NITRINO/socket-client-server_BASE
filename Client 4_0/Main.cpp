#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    /* Включаем библиотеки */
    WSADATA wsaData;
    int iResult;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        //form->SetText_In_TextBoxInfo("WSAStartup failed with error: " + iResult + "\n\r");
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

	/* Form Run */
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	Client40::MyForm form;
	Application::Run(% form);

    //Отключаем библиотеки
    WSACleanup();
    return 0;
}