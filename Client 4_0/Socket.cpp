#include "Socket.h" // Внутри уже есть ProccessResponseResualt.h, а в нём есть MyForm.h

#define DEFAULT_BUFLEN 512

SOCKET ConnectToServer(Client40::MyForm^ form)
{
    //WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, *ptr = NULL, hints;
    const char* sendbuf = "this is a test string\r\n";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //Преобразуем System::String^ в std::string и дальше в const char*
    std::string port_str;
    MarshalString(form->Get_PortString(), port_str);
    const char* c_port_str = port_str.c_str();
    std::string host_str;
    MarshalString(form->Get_Host(), host_str);
    const char* c_host_str = host_str.c_str();

    // Resolve the server address and port
    //iResult = getaddrinfo("localhost", "9034", &hints, &result);
    iResult = getaddrinfo(c_host_str, c_port_str, &hints, &result);
    if (iResult != 0) {
        form->SetText_In_TextBoxInfo("getaddrinfo failed with error: " + iResult + "\r\n");

        form->SetText_In_TextBoxInfo("host: ");
        form->SetText_In_TextBoxInfo(c_host_str);
        form->SetText_In_TextBoxInfo("port: ");
        form->SetText_In_TextBoxInfo(c_port_str);

        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            form->SetText_In_TextBoxInfo("socket failed with error: " + WSAGetLastError() + "\r\n");
            //printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        form->SetText_In_TextBoxInfo("Unable to connect to server!\r\n");
        //printf("Unable to connect to server!\n");
        WSACleanup();
        return INVALID_SOCKET;
    }

    return ConnectSocket;
    //Обработка сообщений
        /*
        bool flag = true;
        char ch;
        // Receive until the peer closes the connection
        do {
            // Send an initial buffer
            iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                form->SetText_In_TextBoxInfo("send failed with error: " + WSAGetLastError() + "\n\r");
                //printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            form->SetText_In_TextBoxInfo("Bytes Sent: " + iResult + "\n\r");
            //printf("Bytes Sent: %ld\n", iResult);

            form->SetText_In_TextBoxInfo("Client beetwen send and recv\n\r");
            //std::cout << "Client beetwen send and recv\n";

            iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0)
                form->SetText_In_TextBoxInfo("Bytes received: " + iResult + "\n\r");
                //printf("Bytes received: %d\n", iResult);
            else if (iResult == 0)
                form->SetText_In_TextBoxInfo("Connection closed\n\r");
                //printf("Connection closed\n");
            else
                form->SetText_In_TextBoxInfo("recv failed with error: " + WSAGetLastError() + "\n\r");
                //printf("recv failed with error: %d\n", WSAGetLastError());

            //std::cout << "Repeate recv? (y/n): ";
            //std::cin >> ch;
            //if (ch != 'y') flag = false;

        } while (flag); // while (iResult > 0);

        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();


        return 0;*/
}

int DisconnectToServer(SOCKET ConnectSocket)
{
    closesocket(ConnectSocket);
    return 0;
}

int Receiver(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult)
{
    int iResult;
    const int recvbuflen = 2048;
    char recvbuf[recvbuflen] = {0};         //Обнуляем массив
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
    {
        form->SetText_In_TextBoxInfo("Message received: ");             // Отсылаем в информационное окно 
        form->SetText_In_TextBoxInfo(recvbuf);
        ResponseParse(recvbuf, strlen(recvbuf), responseResult);        // Парсим то, что получили
        ProccessResponseResualt(form, responseResult);
        return 0;
    }
    else if (iResult == 0)
    {
        form->SetText_In_TextBoxInfo("Connection closed\r\n");
        return 1;
    }
    else
    {
        form->SetText_In_TextBoxInfo("recv failed with error: " + WSAGetLastError() + "\r\n");
        return 1;
    }
}

int RegistrationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult)
{
    //Преобразуем System::String^ в std::string и дальше в const char*
    std::string login_str;
    MarshalString(form->Get_LoginString(), login_str);
    std::string password_str;
    MarshalString(form->Get_PasswordString(), password_str);

    std::string request = "api/registration/" + login_str + '&' + password_str;

    const char* c_request_str = request.c_str();

    int iResult;
    iResult = send(ConnectSocket, c_request_str, strlen(c_request_str), 0);
    if (iResult == SOCKET_ERROR) {
        return 1;
    }

    return Receiver(form, ConnectSocket, responseResult);
}

int AuthenticationRequest(Client40::MyForm^ form, SOCKET ConnectSocket, ResponseResult& responseResult)
{
    //Преобразуем System::String^ в std::string и дальше в const char*
    std::string login_str;
    MarshalString(form->Get_LoginString(), login_str);
    std::string password_str;
    MarshalString(form->Get_PasswordString(), password_str);

    std::string request = "api/authentication/" + login_str + '&' + password_str;

    const char* c_request_str = request.c_str();

    int iResult;
    iResult = send(ConnectSocket, c_request_str, strlen(c_request_str), 0);
    if (iResult == SOCKET_ERROR) {
        return 1;
    }

    return Receiver(form, ConnectSocket, responseResult);
}