#pragma once
#include <cmath>
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class MyForm : public Form
{
private:
    System::Windows::Forms::PictureBox^ pictureBox1;
    System::Windows::Forms::Button^ button1;
    System::ComponentModel::Container^ components;

public:
    MyForm(void)
    {
        InitializeComponent();
    }

protected:
    ~MyForm()
    {
        if (components)
            delete components;
    }

private:
    void InitializeComponent(void)
    {
        this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
        this->button1 = (gcnew System::Windows::Forms::Button());
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
        this->SuspendLayout();
        // 
        // pictureBox1
        // 
        this->pictureBox1->BackColor = System::Drawing::Color::White;
        this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
        this->pictureBox1->Location = System::Drawing::Point(20, 20);
        this->pictureBox1->Name = L"pictureBox1";
        this->pictureBox1->Size = System::Drawing::Size(400, 400);
        this->pictureBox1->TabIndex = 0;
        this->pictureBox1->TabStop = false;
        // 
        // button1
        // 
        this->button1->Location = System::Drawing::Point(450, 50);
        this->button1->Name = L"button1";
        this->button1->Size = System::Drawing::Size(120, 40);
        this->button1->TabIndex = 1;
        this->button1->Text = L"Start";
        this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
        // 
        // MyForm
        // 
        this->ClientSize = System::Drawing::Size(600, 460);
        this->Controls->Add(this->pictureBox1);
        this->Controls->Add(this->button1);
        this->Name = L"MyForm";
        this->Text = L"Animation";
        this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
        (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
        this->ResumeLayout(false);

    }

private:
    void Paint_Circle(int cX, int cY, int centX, int centY, int radius, int x, int y)
    {
        Graphics^ g = pictureBox1->CreateGraphics();
        g->DrawEllipse(Pens::Black, centX + cX - radius, cY - radius - centY, radius * 2, radius * 2);
        g->DrawLine(Pens::Black, centX + cX, cY - centY, cX + x, cY + y);
    }

    void Paint_Graphic(int cX, int cY, int r2, int x, int y, array<Point>^ p)
    {
        Graphics^ g = pictureBox1->CreateGraphics();
        g->Clear(pictureBox1->BackColor);
        Paint_Circle(cX, cY, 0, 0, r2, x, y);
        if (p->Length > 1)
            g->DrawLines(Pens::Red, p);
    }

private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
{
    double InitT = 0, LastT = 6.3;
    double Step = 0.1, angle = InitT;
    double x, y, x1, y1;
    int cX = 200, cY = 200;      //  
    int R2 = 150;                  //  
    int k = 6;               //     7
    int R1 = R2 / k;         //     

    int pointCount = (int)((LastT - InitT) / Step) + 2;
    array<Point>^ p = gcnew array<Point>(pointCount);
    int i = 0;

    while (angle <= LastT && i < p->Length)
    {
        x = R1 * (k - 1) * (Math::Cos(angle) + Math::Cos((k - 1) * angle) / (k - 1));
        y = R1 * (k - 1) * (Math::Sin(angle) - Math::Sin((k - 1) * angle) / (k - 1));

        if (i < p->Length)
            p[i] = Point(cX + (int)x, cY + (int)y);

        Paint_Graphic(cX, cY, R2, (int)x, (int)y, p);

        x1 = (R2 - R1) * Math::Sin(angle + 1.57);
        y1 = (R2 - R1) * Math::Cos(angle + 1.57);
        Paint_Circle(cX, cY, (int)x1, (int)y1, R1, (int)x, (int)y);

        angle += Step;
        i++;
        System::Threading::Thread::Sleep(20);
    }
}
private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
}
};