#pragma once

namespace Task63 {

	using namespace System;
	using namespace System::Windows::Forms;

	public ref class MyForm : public Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			selectedShape = -1;
		}

	private:
		System::Windows::Forms::ComboBox^ comboBox1;
		System::Windows::Forms::Label^ label1;
		System::Windows::Forms::PictureBox^ pictureBox1;

		int selectedShape;

		void InitializeComponent(void)
		{
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// comboBox1
			// 
			this->comboBox1->Location = System::Drawing::Point(180, 20);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(150, 28);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->label1->Location = System::Drawing::Point(20, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(150, 30);
			this->label1->TabIndex = 0;
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Location = System::Drawing::Point(30, 70);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(250, 250);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);
			// 
			// MyForm
			// 
			this->ClientSize = System::Drawing::Size(400, 382);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MyForm";
			this->Text = L"Paint a figure";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		this->label1->Text = L"Choose figure:";
		this->comboBox1->Text = L"figures";
		array<String^>^ фигуры = gcnew array<String^> { L"Rectangle", L"Ellipse", L"Circle" };
		this->comboBox1->Items->AddRange(фигуры);
	}

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		selectedShape = comboBox1->SelectedIndex;
		pictureBox1->Invalidate();
	}

	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		e->Graphics->Clear(pictureBox1->BackColor);

		if (selectedShape == -1)
			return;

		System::Drawing::Brush^ fill = gcnew System::Drawing::SolidBrush(System::Drawing::Color::DeepPink);

		switch (selectedShape)
		{
		case 0: // Прямоугольник
			e->Graphics->FillRectangle(fill, 60, 60, 120, 180);
			break;
		case 1: // Эллипс
			e->Graphics->FillEllipse(fill, 60, 60, 120, 180);
			break;
		case 2: // Окружность
			e->Graphics->FillEllipse(fill, 60, 60, 120, 120);
			break;
		}
	}
	};
}
