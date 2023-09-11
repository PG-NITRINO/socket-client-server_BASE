#include "Socket.h" // Внутри уже есть ProccessResponseResualt.h, а в нём есть MyForm.h

Client40::MyForm::MyForm()
{
	InitializeComponent();
	this->ClientSize = System::Drawing::Size(1000, 500);

	responseResult = new ResponseResult();

	generate();
	generateEntry();
}

Client40::MyForm::~MyForm()
{
	//delete responseResult;
	if (components)
	{
		closesocket(ConnectSocket);	//Очищаем сокет при закрытии формы
		delete components;
	}
}

void Client40::MyForm::InitializeComponent(void)
{
	this->SuspendLayout();
	// 
	// MyForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(278, 244);
	this->Name = L"MyForm";
	this->Text = L"MyForm";
	this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
	this->ResumeLayout(false);

}

System::Void Client40::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
}

void Client40::MyForm::generate()
{
	/* Создаём объекты */
	this->button_Connect = gcnew System::Windows::Forms::Button();
	this->button_Disconnect = gcnew System::Windows::Forms::Button();
	this->label_Connect = gcnew System::Windows::Forms::Label();
	this->textBox_Info = gcnew System::Windows::Forms::TextBox();

	this->label_host = gcnew System::Windows::Forms::Label();
	this->textBox_host = gcnew System::Windows::Forms::TextBox();
	this->label_port = gcnew System::Windows::Forms::Label();
	this->textBox_port = gcnew System::Windows::Forms::TextBox();


	/* Присостанавливаем на время изменений*/
	this->SuspendLayout();
	this->button_Connect->SuspendLayout();
	this->button_Disconnect->SuspendLayout();
	this->label_Connect->SuspendLayout();
	this->textBox_Info->SuspendLayout();

	this->label_host->SuspendLayout();
	this->textBox_host->SuspendLayout();
	this->label_port->SuspendLayout();
	this->textBox_port->SuspendLayout();


	/* button_Connect */
	this->button_Connect->Text = "Connect server";
	this->button_Connect->Location = System::Drawing::Point(50, 50);
	this->button_Connect->Name = "button_Connect";
	this->button_Connect->Size = System::Drawing::Size(100, 50);
	this->button_Connect->TabIndex = 0;
	this->button_Connect->TabStop = false;

	this->button_Connect->Click += gcnew System::EventHandler(this, &MyForm::button_Connect_Click);  // Подключение к серверу


	/* label_Connect */
	this->label_Connect->Font = gcnew System::Drawing::Font("Times New Roman", 12, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_Connect->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_Connect->MaximumSize = System::Drawing::Size((int)(this->button_Connect->Size.Width), (int)(this->button_Connect->Size.Height));
	this->label_Connect->Name = "label_Connect";
	this->label_Connect->Size = System::Drawing::Size((int)(this->button_Connect->Size.Width), (int)(this->button_Connect->Size.Height));
	this->label_Connect->Location = System::Drawing::Point((int)(this->button_Connect->Location.X + this->button_Connect->Size.Width + 20), (int)(this->button_Connect->Location.Y));
	this->label_Connect->TabIndex = 4;
	this->label_Connect->Text = "Not connection";
	this->label_Connect->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_Connect->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* label_host */
	this->label_host->Font = gcnew System::Drawing::Font("Times New Roman", 12, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_host->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_host->Name = "label_host";
	this->label_host->Size = System::Drawing::Size((int)(this->button_Connect->Size.Width / 2), (int)(this->button_Connect->Size.Height / 2));
	this->label_host->Location = System::Drawing::Point((int)(this->button_Connect->Location.X), (int)(this->button_Connect->Location.Y + this->button_Connect->Size.Height + 20));
	this->label_host->TabIndex = 4;
	this->label_host->Text = "Host: ";
	this->label_host->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_host->BorderStyle = System::Windows::Forms::BorderStyle::None;


	/* label_port */
	this->label_port->Font = gcnew System::Drawing::Font("Times New Roman", 11, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_port->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_port->Name = "label_host";
	this->label_port->Size = System::Drawing::Size((int)(this->label_host->Size.Width), (int)(this->label_host->Size.Height));
	this->label_port->Location = System::Drawing::Point((int)(this->label_host->Location.X), (int)(this->label_host->Location.Y + this->label_host->Size.Height + 20));
	this->label_port->TabIndex = 4;
	this->label_port->Text = "Port: ";
	this->label_port->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_port->BorderStyle = System::Windows::Forms::BorderStyle::None;


	/* textBox_host */
	this->textBox_host->ReadOnly = false;
	this->textBox_host->AcceptsReturn = true;
	this->textBox_host->AcceptsTab = true;
	this->textBox_host->Multiline = false;
	this->textBox_host->ScrollBars = System::Windows::Forms::ScrollBars::None;
	this->textBox_host->Location = System::Drawing::Point((int)(this->label_host->Location.X + this->label_host->Size.Width + 20), (int)(this->label_host->Location.Y));
	this->textBox_host->Size = System::Drawing::Size((int)(this->label_host->Size.Width) * 3, (int)(this->label_host->Size.Height));
	this->textBox_host->Text = "localhost";
	this->textBox_host->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* textBox_port */
	this->textBox_port->ReadOnly = false;
	this->textBox_port->AcceptsReturn = true;
	this->textBox_port->AcceptsTab = true;
	this->textBox_port->Multiline = false;
	this->textBox_port->ScrollBars = System::Windows::Forms::ScrollBars::None;
	this->textBox_port->Location = System::Drawing::Point((int)(this->label_port->Location.X + this->label_port->Size.Width + 20), (int)(this->label_port->Location.Y));
	this->textBox_port->Size = System::Drawing::Size((int)(this->label_port->Size.Width) * 3, (int)(this->label_port->Size.Height));
	this->textBox_port->Text = "9034";
	this->textBox_port->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;

	this->textBox_port->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::label_port_KeyPress);  // Подключение к серверу

	/* textBox_Info */
	this->textBox_Info->ReadOnly = true;
	this->textBox_Info->AcceptsReturn = true;
	this->textBox_Info->AcceptsTab = true;
	this->textBox_Info->Multiline = true;
	this->textBox_Info->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
	this->textBox_Info->Location = System::Drawing::Point((int)(this->label_port->Location.X), (int)(this->label_port->Location.Y + this->label_port->Size.Height + 20));
	this->textBox_Info->Size = System::Drawing::Size((int)(this->label_Connect->Location.X + this->label_Connect->Size.Width - this->button_Connect->Location.X) * 1, (int)(this->button_Connect->Size.Height * 4));
	this->textBox_Info->Text = "Information:\n";
	this->textBox_Info->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* button_Disconnect */
	this->button_Disconnect->Text = "Disconnect server";
	this->button_Disconnect->Location = System::Drawing::Point((int)(this->label_Connect->Location.X + this->label_Connect->Size.Width +20), this->label_Connect->Location.Y);
	this->button_Disconnect->Name = "button_Disconnect";
	this->button_Disconnect->Size = System::Drawing::Size(this->button_Connect->Size.Width, this->button_Connect->Size.Height);
	this->button_Disconnect->TabIndex = 0;
	this->button_Disconnect->TabStop = false;

	this->button_Disconnect->Enabled = false;	//Включается при подключении к серверу

	this->button_Disconnect->Click += gcnew System::EventHandler(this, &MyForm::button_Disconnect_Click);  // Подключение к серверу


	/* Добавление в форму */
	this->Controls->Add(this->button_Connect);
	this->Controls->Add(this->button_Disconnect);
	this->Controls->Add(this->label_Connect);
	this->Controls->Add(this->textBox_Info);

	this->Controls->Add(this->label_host);
	this->Controls->Add(this->textBox_host);
	this->Controls->Add(this->label_port);
	this->Controls->Add(this->textBox_port);


	/* Все изменения внесены - убираем остановку */
	this->button_Connect->ResumeLayout(false);
	this->button_Disconnect->ResumeLayout(false);
	this->label_Connect->ResumeLayout(false);
	this->textBox_Info->ResumeLayout(false);

	this->label_host->ResumeLayout(false);
	this->textBox_host->ResumeLayout(false);
	this->label_port->ResumeLayout(false);
	this->textBox_port->ResumeLayout(false);

	this->ResumeLayout(false);
}

void Client40::MyForm::generateEntry()
{
	/* Создаём объекты */
	this->panel_Entry = gcnew System::Windows::Forms::Panel();
	this->button_Registration = gcnew System::Windows::Forms::Button();
	this->button_Authentication = gcnew System::Windows::Forms::Button();
	this->label_password = gcnew System::Windows::Forms::Label();
	this->label_login = gcnew System::Windows::Forms::Label();
	this->label_message = gcnew System::Windows::Forms::Label();
	this->textBox_password = gcnew System::Windows::Forms::TextBox();
	this->textBox_login = gcnew System::Windows::Forms::TextBox();


	/* Присостанавливаем на время изменений*/
	this->SuspendLayout();
	this->panel_Entry->SuspendLayout();
	this->button_Registration->SuspendLayout();
	this->button_Authentication->SuspendLayout();
	this->label_password->SuspendLayout();
	this->label_login->SuspendLayout();
	this->label_message->SuspendLayout();
	this->textBox_password->SuspendLayout();
	this->textBox_login->SuspendLayout();


	/* panel_Entry */
	this->panel_Entry->Dock = System::Windows::Forms::DockStyle::None;
	this->panel_Entry->Location = System::Drawing::Point((int)(this->button_Disconnect->Location.X + this->button_Disconnect->Size.Width) + 20, (int)(this->button_Disconnect->Location.Y));
	this->panel_Entry->Size = System::Drawing::Size(300, 350);
	this->panel_Entry->Name = "panel_Entry";
	this->panel_Entry->TabIndex = 0;
	this->panel_Entry->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;

	this->panel_Entry->Visible = false;    // Изначально отключаем
	this->panel_Entry->Enabled = false;


	/* button_Registration */
	this->button_Registration->Text = "Registration";
	this->button_Registration->Size = System::Drawing::Size(100, 50);
	this->button_Registration->Location = System::Drawing::Point((int)((this->panel_Entry->Size.Width / 2) - (this->button_Registration->Size.Width / 2)), 50);
	this->button_Registration->Name = "button_Registration";
	this->button_Registration->TabIndex = 0;
	this->button_Registration->TabStop = false;

	this->button_Registration->Click += gcnew System::EventHandler(this, &MyForm::button_Registration_Click);  // Попытка регистрации


	/* button_Authentication */
	this->button_Authentication->Text = "Authentication";
	this->button_Authentication->Size = System::Drawing::Size(100, 50);
	this->button_Authentication->Location = System::Drawing::Point((int)((this->panel_Entry->Size.Width / 2) - (this->button_Authentication->Size.Width / 2)), 120);
	this->button_Authentication->Name = "button_Authentication";
	this->button_Authentication->TabIndex = 0;
	this->button_Authentication->TabStop = false;

	this->button_Authentication->Click += gcnew System::EventHandler(this, &MyForm::button_Authentication_Click);  // Попытка аутентификации


	/* label_login */
	this->label_login->Font = gcnew System::Drawing::Font("Times New Roman", 11, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_login->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_login->Name = "label_login";
	this->label_login->Size = System::Drawing::Size((int)(this->button_Authentication->Size.Width) / 2, (int)(this->button_Authentication->Size.Height) / 3);
	this->label_login->Location = System::Drawing::Point(20, (int)(this->button_Authentication->Location.Y + this->button_Authentication->Size.Height + 30));
	this->label_login->TabIndex = 4;
	this->label_login->Text = "Login: ";
	this->label_login->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_login->BorderStyle = System::Windows::Forms::BorderStyle::None;


	/* label_password */
	this->label_password->Font = gcnew System::Drawing::Font("Times New Roman", 11, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_password->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_password->Name = "label_password";
	this->label_password->Size = System::Drawing::Size((int)(this->label_login->Size.Width), (int)(this->label_login->Size.Height));
	this->label_password->Location = System::Drawing::Point(20, (int)(this->label_login->Location.Y + this->label_login->Size.Height + 30));
	this->label_password->TabIndex = 4;
	this->label_password->Text = "Pass: ";
	this->label_password->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_password->BorderStyle = System::Windows::Forms::BorderStyle::None;
	

	/* textBox_login */
	this->textBox_login->ReadOnly = false;
	this->textBox_login->AcceptsReturn = true;
	this->textBox_login->AcceptsTab = true;
	this->textBox_login->Multiline = false;
	this->textBox_login->ScrollBars = System::Windows::Forms::ScrollBars::None;
	this->textBox_login->Location = System::Drawing::Point((int)(this->label_login->Location.X + this->label_login->Size.Width + 20), (int)(this->label_login->Location.Y));
	this->textBox_login->Size = System::Drawing::Size((int)(this->label_login->Size.Width) * 3, (int)(this->label_login->Size.Height));
	this->textBox_login->Text = "";
	this->textBox_login->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* textBox_password */
	this->textBox_password->ReadOnly = false;
	this->textBox_password->AcceptsReturn = true;
	this->textBox_password->AcceptsTab = true;
	this->textBox_password->Multiline = false;
	this->textBox_password->ScrollBars = System::Windows::Forms::ScrollBars::None;
	this->textBox_password->Location = System::Drawing::Point((int)(this->label_password->Location.X + this->label_password->Size.Width + 20), (int)(this->label_password->Location.Y));
	this->textBox_password->Size = System::Drawing::Size((int)(this->label_password->Size.Width) * 3, (int)(this->label_password->Size.Height));
	this->textBox_password->Text = "";
	this->textBox_password->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* label_message */
	this->label_message->Font = gcnew System::Drawing::Font("Times New Roman", 7, ((System::Drawing::FontStyle)((System::Drawing::FontStyle::Regular))));
	this->label_message->Margin = System::Windows::Forms::Padding(0, 0, 0, 0);
	this->label_message->Name = "label_message";
	this->label_message->Size = System::Drawing::Size((int)(this->textBox_password->Location.X + this->textBox_password->Size.Width - this->label_password->Location.X) ,
															(int)(this->panel_Entry->Size.Height - this->label_password->Location.Y - this->label_password->Size.Height) - 20);
	//this->label_message->Location = System::Drawing::Point(0, 0);
	this->label_message->Location = System::Drawing::Point((int)(this->label_password->Location.X), (int)(this->label_password->Location.Y + this->label_password->Size.Height + 10));
	this->label_message->TabIndex = 4;
	this->label_message->Text = "";
	this->label_message->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	this->label_message->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;


	/* Добавление в форму */
	this->panel_Entry->Controls->Add(this->button_Registration);
	this->panel_Entry->Controls->Add(this->button_Authentication);
	this->panel_Entry->Controls->Add(this->label_password);
	this->panel_Entry->Controls->Add(this->label_login);
	this->panel_Entry->Controls->Add(this->textBox_password);
	this->panel_Entry->Controls->Add(this->textBox_login);
	this->panel_Entry->Controls->Add(this->label_message);
	
	this->Controls->Add(this->panel_Entry);


	/* Все изменения внесены - убираем остановку */
	this->panel_Entry->ResumeLayout(false);
	this->button_Registration->ResumeLayout(false);
	this->button_Authentication->ResumeLayout(false);
	this->label_password->ResumeLayout(false);
	this->label_login->ResumeLayout(false);
	this->label_message->ResumeLayout(false);
	this->textBox_password->ResumeLayout(false);
	this->textBox_login->ResumeLayout(false);

	this->ResumeLayout(false);

}

System::Void Client40::MyForm::button_Connect_Click(System::Object^ sender, System::EventArgs^ e)
{
	//this->BackColor = System::Drawing::Color::Green;
	SetText_In_TextBoxInfo("Try connect...");
	ConnectSocket = ConnectToServer(this);
	if (ConnectSocket != INVALID_SOCKET)
	{//То есть мы подключились 
		this->button_Connect->Enabled = false;		//Отключаем кнопку подключения
		this->button_Disconnect->Enabled = true;	//Включаем кнопку отключения
		/* Удалось подключиться к серверу */
		this->label_Connect->Text = "Connect to server";

		EntryON();
	}
}

System::Void Client40::MyForm::button_Disconnect_Click(System::Object^ sender, System::EventArgs^ e)
{
	SetText_In_TextBoxInfo("Disconnection...");
	DisconnectToServer(ConnectSocket);
	this->label_Connect->Text = "Not connection";

	this->button_Connect->Enabled = true;		//Включаем кнопку подключения
	this->button_Disconnect->Enabled = false;	//Отключаем кнопку отключения

	EntryOFF();
}

System::Void Client40::MyForm::label_port_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	char ch = e->KeyChar;

	if ((ch <= (char)(47) || ch >= (char)(58)) && ch != (char)(8))
	{
		e->Handled = true;
	}
}

//Кнопка Регистрации
System::Void Client40::MyForm::button_Registration_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (this->textBox_login->Text != "" &&
		this->textBox_password->Text != "")
	{
		this->label_message->Text = "";
		RegistrationRequest(this, ConnectSocket, *responseResult);
		this->textBox_login->Text = "";
		this->textBox_password->Text = "";
	}
}

//Кнопка Входа
System::Void Client40::MyForm::button_Authentication_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (this->textBox_login->Text != "" &&
		this->textBox_password->Text != "")
	{
		this->label_message->Text = "";
		AuthenticationRequest(this, ConnectSocket, *responseResult);
		this->textBox_login->Text = "";
		this->textBox_password->Text = "";
	}
}

void Client40::MyForm::SetText_In_TextBoxInfo(System::String^ text)
{
	this->textBox_Info->Text = this->textBox_Info->Text + "\r\n(1)" + text;
}

void Client40::MyForm::SetText_In_TextBoxInfo(const char* text)
{
	System::String^ str = gcnew System::String(text);
	this->textBox_Info->Text = this->textBox_Info->Text + "\r\n(2)" + str;
}

void Client40::MyForm::SetText_In_LabelConnect(System::String^ text)
{
	this->label_Connect->Text = text;
}

void Client40::MyForm::SetText_In_Labelmessage(System::String^ text)
{
	this->label_message->Text = this->label_message->Text + text;
}

void Client40::MyForm::SetText_In_Labelmessage(const char* text)
{
	System::String^ str = gcnew System::String(text);
	this->label_message->Text = this->label_message->Text + str;
}

System::String^ Client40::MyForm::Get_Host()
{
	return this->textBox_host->Text;
}

int Client40::MyForm::Get_PortInt()
{
	return System::Int32::Parse(this->textBox_port->Text);
}

System::String^ Client40::MyForm::Get_PortString()
{
	return this->textBox_port->Text;
}

System::String^ Client40::MyForm::Get_LoginString()
{
	return this->textBox_login->Text;
}

System::String^ Client40::MyForm::Get_PasswordString()
{
	return this->textBox_password->Text;
}

//Активация панелей
void Client40::MyForm::EntryON()
{
	this->textBox_login->Text = "";
	this->textBox_password->Text = "";
	this->panel_Entry->Visible = true;
	this->panel_Entry->Enabled = true;
}
		   //Дизактивация панелей
void Client40::MyForm::EntryOFF()
{
	this->panel_Entry->Visible = false;
	this->panel_Entry->Enabled = false;
}