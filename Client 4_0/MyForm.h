#pragma once
#include "ExtraFunctions.h"

namespace Client40 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	private: ResponseResult* responseResult;	//��������� ��� �������� ����������� ��������� ������� �������

	public: MyForm(void);

	protected: ~MyForm();

	private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void);
#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e);

	private:
		System::Windows::Forms::Button^ button_Connect;
		System::Windows::Forms::Button^ button_Disconnect;
		System::Windows::Forms::Label^ label_Connect;
		System::Windows::Forms::TextBox^ textBox_Info;

		System::Windows::Forms::Label^ label_host;
		System::Windows::Forms::TextBox^ textBox_host;
		System::Windows::Forms::Label^ label_port;
		System::Windows::Forms::TextBox^ textBox_port;

			//��������� ������ ��������
	private: void generate();

		   System::Windows::Forms::Panel^ panel_Entry;
		   System::Windows::Forms::Button^ button_Registration;			//��� �����������
		   System::Windows::Forms::Button^ button_Authentication;		//��� ����� � �������
		   System::Windows::Forms::Label^ label_password;
		   System::Windows::Forms::Label^ label_login;
		   System::Windows::Forms::TextBox^ textBox_password;
		   System::Windows::Forms::TextBox^ textBox_login;

		   System::Windows::Forms::Label^ label_message;	//��������� � ����������� ����������� � ��������������

			//��������� �����/�����������
	private: void generateEntry();


	private: SOCKET ConnectSocket;


		    //������ ����������� � �������
	private: System::Void button_Connect_Click(System::Object^ sender, System::EventArgs^ e);
		    //������ ���������� �� �������
	private: System::Void button_Disconnect_Click(System::Object^ sender, System::EventArgs^ e);
		    //���� �����(������ �����)
	private: System::Void label_port_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);

		   //������ �����������
	private: System::Void button_Registration_Click(System::Object^ sender, System::EventArgs^ e);
		   //������ �����
	private: System::Void button_Authentication_Click(System::Object^ sender, System::EventArgs^ e);


	public: void SetText_In_TextBoxInfo(System::String^ text);
	public: void SetText_In_TextBoxInfo(const char* text);

	public: void SetText_In_LabelConnect(System::String^ text);

	public: void SetText_In_Labelmessage(System::String^ text);
	public: void SetText_In_Labelmessage(const char* text);

	public: System::String^ Get_Host();

	public: int Get_PortInt();

	public: System::String^ Get_PortString();

	public: System::String^ Get_LoginString();
	public: System::String^ Get_PasswordString();


		  //��������� �������
	private: void EntryON();
		  //������������ �������
	private: void EntryOFF();
	};
}
