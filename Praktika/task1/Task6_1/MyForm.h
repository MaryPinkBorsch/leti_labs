#pragma once

namespace Task61 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			m_p = gcnew array<int>(6);
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		array<int>^ m_p;
		System::ComponentModel::Container^ components;

		System::Windows::Forms::TextBox^ textBox1;
		System::Windows::Forms::TextBox^ textBox2;
		System::Windows::Forms::TextBox^ textBox3;
		System::Windows::Forms::TextBox^ textBox4;
		System::Windows::Forms::TextBox^ textBox5;
		System::Windows::Forms::TextBox^ textBox6;
		System::Windows::Forms::Button^ button1;
		System::Windows::Forms::PictureBox^ pictureBox1;
		System::Windows::Forms::Label^ label1;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();

			this->textBox1->Location = System::Drawing::Point(600, 90);
			this->textBox1->Size = System::Drawing::Size(80, 26);

			this->textBox2->Location = System::Drawing::Point(700, 90);
			this->textBox2->Size = System::Drawing::Size(80, 26);

			this->textBox3->Location = System::Drawing::Point(600, 130);
			this->textBox3->Size = System::Drawing::Size(80, 26);

			this->textBox4->Location = System::Drawing::Point(700, 130);
			this->textBox4->Size = System::Drawing::Size(80, 26);

			this->textBox5->Location = System::Drawing::Point(600, 170);
			this->textBox5->Size = System::Drawing::Size(80, 26);

			this->textBox6->Location = System::Drawing::Point(700, 170);
			this->textBox6->Size = System::Drawing::Size(80, 26);

			// button1
			this->button1->Location = System::Drawing::Point(640, 220);
			this->button1->Size = System::Drawing::Size(100, 40);
			this->button1->Text = L"Рисовать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);

			// pictureBox1
			this->pictureBox1->Location = System::Drawing::Point(30, 30);
			this->pictureBox1->Size = System::Drawing::Size(500, 350);
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);

			// label1
			this->label1->Text = L"insert coordinates:";
			this->label1->Location = System::Drawing::Point(30, 5);
			this->label1->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold);
			this->label1->AutoSize = true;

			// MyForm
			this->ClientSize = System::Drawing::Size(800, 420);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->textBox6);
			this->Text = L"draw triangle";
			this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);

			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			m_p[0] = Convert::ToInt32(textBox1->Text); // x1
			m_p[1] = Convert::ToInt32(textBox2->Text); // y1
			m_p[2] = Convert::ToInt32(textBox3->Text); // x2
			m_p[3] = Convert::ToInt32(textBox4->Text); // y2
			m_p[4] = Convert::ToInt32(textBox5->Text); // x3
			m_p[5] = Convert::ToInt32(textBox6->Text); // y3

			pictureBox1->Refresh();
		}
		catch (Exception^ ex) {
			MessageBox::Show("Error: " + ex->Message);
		}
	}

	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		e->Graphics->DrawLine(Pens::Black, m_p[0], m_p[1], m_p[2], m_p[3]);
		e->Graphics->DrawLine(Pens::Red, m_p[2], m_p[3], m_p[4], m_p[5]);
		e->Graphics->DrawLine(Pens::Blue, m_p[4], m_p[5], m_p[0], m_p[1]);
	}
	};
}
