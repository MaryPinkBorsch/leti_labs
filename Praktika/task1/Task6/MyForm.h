#pragma once

namespace Task6 {

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
			m_p = gcnew array<int>(4); // коорд
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
		System::Windows::Forms::TextBox^ textBox1;
		System::Windows::Forms::TextBox^ textBox2;
		System::Windows::Forms::TextBox^ textBox3;
		System::Windows::Forms::TextBox^ textBox4;
		System::Windows::Forms::Button^ button1;
		System::Windows::Forms::PictureBox^ pictureBox1;

		array<int>^ m_p; // коорд

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();

			// textBox1 

			this->textBox1->Location = System::Drawing::Point(746, 169);
			this->textBox1->Size = System::Drawing::Size(100, 26);
			this->textBox1->TabIndex = 0;

			// textBox2

			this->textBox2->Location = System::Drawing::Point(746, 90);
			this->textBox2->Size = System::Drawing::Size(100, 26);
			this->textBox2->TabIndex = 1;

			// textBox3

			this->textBox3->Location = System::Drawing::Point(600, 169);
			this->textBox3->Size = System::Drawing::Size(100, 26);
			this->textBox3->TabIndex = 2;

			// textBox4

			this->textBox4->Location = System::Drawing::Point(600, 90);
			this->textBox4->Size = System::Drawing::Size(100, 26);
			this->textBox4->TabIndex = 3;

			// button1

			this->button1->Location = System::Drawing::Point(651, 278);
			this->button1->Size = System::Drawing::Size(136, 62);
			this->button1->Text = L"Рисовать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);

			// pictureBox1

			this->pictureBox1->Location = System::Drawing::Point(32, 29);
			this->pictureBox1->Size = System::Drawing::Size(503, 311);
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->Paint += gcnew PaintEventHandler(this, &MyForm::pictureBox1_Paint);

			// MyForm

			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->ClientSize = System::Drawing::Size(891, 441);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Text = L"draw";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		this->Text = "draw";
		button1->Text = "draw";
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		m_p[0] = Convert::ToInt32(textBox1->Text);
		m_p[1] = Convert::ToInt32(textBox2->Text);
		m_p[2] = Convert::ToInt32(textBox3->Text);
		m_p[3] = Convert::ToInt32(textBox4->Text);
		pictureBox1->Refresh();

	}

	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		e->Graphics->DrawLine(Pens::Green, m_p[0], m_p[1], m_p[2], m_p[3]);
	}
	};
}