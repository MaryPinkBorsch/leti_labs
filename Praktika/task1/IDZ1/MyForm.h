#pragma once
#include <cmath>

namespace IDZ1 {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms; //визуалка
    using namespace System::Drawing; //отрисовка
    using namespace System::Threading; //потоки анимация 

    static const float MY_PI = 3.14159265;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();
            InitializeApp(); //иниц построение траектории
        }

    protected:
        ~MyForm()
        {
            StopAnimation();
            if (components)
                delete components;
            
            
        }

    private:
        System::Windows::Forms::PictureBox^ pictureBox1; //перем интерфейса
        System::Windows::Forms::Button^ buttonStart;
        System::Windows::Forms::Button^ buttonStop;
        System::Windows::Forms::Label^ labelTitle;
        System::Windows::Forms::Label^ labelSpeed;
        System::Windows::Forms::TrackBar^ trackBarSpeed;
        System::Windows::Forms::Button^ buttonDirection;
        System::Windows::Forms::MenuStrip^ menuStrip1;
        System::Windows::Forms::ToolStripMenuItem^ colorsToolStripMenuItem; //подменюшки к заданию вар 4 

        System::Windows::Forms::ToolStripMenuItem^ borderColorToolStripMenuItem;
        System::ComponentModel::Container^ components;

        System::Collections::Generic::List<PointF>^ trajectoryPoints;
        System::Collections::Generic::List<float>^ cumulativeDistances;
        Thread^ animationThread;

        bool isMoving;
        bool isDotted = false;
        bool isRotating = false;
        bool isSizeChanging = false;

        float currentDistance;
        float speed;
        float totalLength;
        float starAngle = 0.0f;
        float starAngle2 = 0.0f;
        float starScale = 0.5f;
        bool clockwise;
        Color StarFillColor;
        Color StarBorderColor;

        //размер фигурки
        System::Windows::Forms::Label^ labelSize;
        System::Windows::Forms::TrackBar^ trackBarSize;
    private: System::Windows::Forms::Button^ button1;
    private: System::Windows::Forms::Button^ button2;
    private: System::Windows::Forms::Button^ button3;
           float StarSize;

        void InitializeApp()
        {
            //списки для точек и длины и всякие стартовые штуки
            trajectoryPoints = gcnew System::Collections::Generic::List<PointF>();
            cumulativeDistances = gcnew System::Collections::Generic::List<float>();
            isMoving = false;
            currentDistance = 0.0f;
            speed = 2.0f;
            clockwise = true;
            StarFillColor = Color::Blue;
            StarBorderColor = Color::DarkBlue;

            //скорость размер и направление
            trackBarSpeed->Minimum = 1;
            trackBarSpeed->Maximum = 10;
            trackBarSpeed->Value = (int)speed;
            trackBarSpeed->TickFrequency = 1;
            labelSpeed->Text = String::Format("Скорость: {0}", speed);
            buttonDirection->Text = L"Против часовой";

            StarSize = 20.0f;
            trackBarSize->Minimum = 10;
            trackBarSize->Maximum = 50;
            trackBarSize->Value = (int)StarSize;
            trackBarSize->TickFrequency = 5;
            labelSize->Text = String::Format("Размер: {0}", StarSize);

            button1->Text = "Solid";
            button2->Text = "NonRotating";
            button3->Text = "Constant Size";


            DrawTrajectory();
        }

        void DrawTrajectory()
        {
            if (pictureBox1->Width == 0 || pictureBox1->Height == 0) return;

            Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
            Graphics^ g = Graphics::FromImage(bmp);
            g->Clear(Color::White);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias; //сглаживание

            const float scale = 30.0f;
            const float step = 0.01f;
            int centerX = pictureBox1->Width / 2;
            int centerY = pictureBox1->Height / 2;

            trajectoryPoints->Clear();
            cumulativeDistances->Clear();

            GenerateTrajectoryPoints(scale, step, centerX, centerY);

            CalculateCumulativeDistances();
            DrawCurrentPosition();
        }

        void GenerateTrajectoryPoints(float scale, float step, int centerX, int centerY) //метод точки траектории
        {
            for (float x = -0.5 * MY_PI; x <= 0.5 * MY_PI; x += step)
            {
                AddTrajectoryPoint(x, scale, centerX, centerY);
            }
        }

        void AddTrajectoryPoint(float x, float scale, int centerX, int centerY)//добавка точек к тому что выше
        {
            float y = Math::Tan(x);
            float plotX = centerX + x * scale;
            float plotY = centerY - y * scale;

            if (!Single::IsNaN(plotY) && plotY >= 0 && plotY < pictureBox1->Height)//корректность
                trajectoryPoints->Add(PointF(plotX, plotY));
        }

        //штука для хорошего перемещения траектории (до этого неравномерно все двигалось так как разное количество точек в разных местах было)
        void CalculateCumulativeDistances()
        {
            cumulativeDistances->Clear();
            if (trajectoryPoints->Count == 0) return;

            cumulativeDistances->Add(0.0f);
            totalLength = 0.0f;

            for (int i = 1; i < trajectoryPoints->Count; i++)
            {
                float dx = trajectoryPoints[i].X - trajectoryPoints[i - 1].X; //ро между точками
                float dy = trajectoryPoints[i].Y - trajectoryPoints[i - 1].Y;
                float distance = (float)Math::Sqrt(dx * dx + dy * dy);
                totalLength += distance;
                cumulativeDistances->Add(totalLength);//в список ее
            }
        }

        void DrawStar(Graphics^ g, PointF center)
        {
            array<PointF>^ points = gcnew array<PointF>(8); //массив 8 точек

            for (int i = 0; i < 8; i++)
            {
                float angle = starAngle + (float)(Math::PI / 4 * i);
                float additionalScale = 1.0;
                if (isSizeChanging)
                {
                    additionalScale = starScale;
                }
                points[i] = PointF(
                    center.X + additionalScale*(i%2 ? StarSize : StarSize/4) * (float)Math::Cos(angle),
                    center.Y + additionalScale*(i % 2 ? StarSize : StarSize / 4) * (float)Math::Sin(angle)
                );
            }

            if (isSizeChanging)
            {
                starScale = 1.5 - Math::Cos(starAngle2);
                starAngle2 += 0.1;
                if (starAngle2 >= 2*MY_PI)
                    starAngle2 = 0.0;
            }

            //g->FillPolygon(gcnew SolidBrush(StarFillColor), points);
            g->DrawPolygon(gcnew Pen(StarBorderColor, 2), points);
            if (isRotating) 
            {
                starAngle += 0.1;
                if (starAngle >= MY_PI)
                    starAngle = 0.0;
            }
        }

        void DrawTrajectoryWithStar(Graphics^ g, PointF StarPosition) //отрисовка всего вместе
        {
            g->Clear(Color::White);
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

            //if (trajectoryPoints->Count > 1)
            //    g->DrawClosedCurve(gcnew Pen(Color::Black, 2), trajectoryPoints->ToArray());
            auto pen = gcnew Pen(Color::Black, 2);
            if (isDotted)
                pen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dash;
            
            if (trajectoryPoints->Count > 1)
                g->DrawCurve(pen, trajectoryPoints->ToArray());

            DrawStar(g, StarPosition);
        }

        void DrawCurrentPosition() //текущая позаиция и отрисовка навого шестиуг
        {
            if (trajectoryPoints->Count == 0 || cumulativeDistances->Count == 0) return;

            int segment = FindCurrentSegment(currentDistance);
            PointF position = CalculateStarPosition(segment, currentDistance);

            Bitmap^ bmp = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
            Graphics^ g = Graphics::FromImage(bmp);
            DrawTrajectoryWithStar(g, position);
            pictureBox1->Image = bmp;
        }

        void AnimateStar()
        {
            while (isMoving)
            {
                this->Invoke(gcnew Action(this, &MyForm::UpdateStarPosition)); //обновление позиции на потоке
                Thread::Sleep(30); //чтобы плавненько
            }
        }

        void UpdateStarPosition()
        {
            if (trajectoryPoints->Count == 0 || cumulativeDistances->Count == 0) return;

            currentDistance += clockwise ? speed : -speed;

            if (currentDistance > totalLength) //цикл модуль
                currentDistance -= totalLength;
            else if (currentDistance < 0)
                currentDistance += totalLength;

            DrawCurrentPosition();
        }

        int FindCurrentSegment(float distance) //интервал на котором будем строить 
        {
            int segment = 0;
            while (segment < cumulativeDistances->Count - 1 &&
                cumulativeDistances[segment + 1] < distance)
            {
                segment++;
            }
            return segment;
        }

        PointF CalculateStarPosition(int segment, float distance) //текущая координата для объ
        {
            float segStart = cumulativeDistances[segment];
            float segEnd = (segment < cumulativeDistances->Count - 1) ?
                cumulativeDistances[segment + 1] : totalLength;
            float t = (distance - segStart) / (segEnd - segStart);

            if (segment < trajectoryPoints->Count - 1) //линейная интерполяция
            {
                return PointF(
                    trajectoryPoints[segment].X + t * (trajectoryPoints[segment + 1].X - trajectoryPoints[segment].X),
                    trajectoryPoints[segment].Y + t * (trajectoryPoints[segment + 1].Y - trajectoryPoints[segment].Y)
                );
            }
            return trajectoryPoints[0];
        }

        void StartAnimation()
        {
            if (!isMoving && trajectoryPoints->Count > 0)
            {
                isMoving = true;
                animationThread = gcnew Thread(gcnew ThreadStart(this, &MyForm::AnimateStar)); //поток
                animationThread->Start();
            }
        }

        void StopAnimation()
        {
            isMoving = false;
            if (animationThread != nullptr)
            {
                animationThread->Join(); //завершение и сброс
                animationThread = nullptr;
            }
        }

        void UpdateStarColors()
        {
            if (!isMoving && trajectoryPoints->Count > 0)
            {
                DrawCurrentPosition();
            }
        }

        void SetFillColor(Color color)
        {
            StarFillColor = color;
            UpdateStarColors();
        }

        void SetBorderColor(Color color)
        {
            StarBorderColor = color;
            UpdateStarColors();
        }

#pragma region Windows Form Designer generated code метод авт генерации создает настраивает подключает
        void InitializeComponent(void)
        {
            this->labelSize = (gcnew System::Windows::Forms::Label());
            this->trackBarSize = (gcnew System::Windows::Forms::TrackBar());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->buttonStart = (gcnew System::Windows::Forms::Button());
            this->buttonStop = (gcnew System::Windows::Forms::Button());
            this->labelTitle = (gcnew System::Windows::Forms::Label());
            this->labelSpeed = (gcnew System::Windows::Forms::Label());
            this->trackBarSpeed = (gcnew System::Windows::Forms::TrackBar());
            this->buttonDirection = (gcnew System::Windows::Forms::Button());
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->colorsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->borderColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->button3 = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSize))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpeed))->BeginInit();
            this->menuStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // labelSize
            // 
            this->labelSize->AutoSize = true;
            this->labelSize->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->labelSize->Location = System::Drawing::Point(852, 388);
            this->labelSize->Name = L"labelSize";
            this->labelSize->Size = System::Drawing::Size(101, 22);
            this->labelSize->TabIndex = 8;
            this->labelSize->Text = L"Размер: 20";
            // 
            // trackBarSize
            // 
            this->trackBarSize->BackColor = System::Drawing::SystemColors::ButtonFace;
            this->trackBarSize->Location = System::Drawing::Point(832, 425);
            this->trackBarSize->Maximum = 50;
            this->trackBarSize->Minimum = 10;
            this->trackBarSize->Name = L"trackBarSize";
            this->trackBarSize->Size = System::Drawing::Size(135, 69);
            this->trackBarSize->TabIndex = 9;
            this->trackBarSize->Value = 20;
            this->trackBarSize->ValueChanged += gcnew System::EventHandler(this, &MyForm::trackBarSize_ValueChanged);
            // 
            // pictureBox1
            // 
            this->pictureBox1->BackColor = System::Drawing::Color::White;
            this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBox1->Location = System::Drawing::Point(21, 65);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(791, 581);
            this->pictureBox1->TabIndex = 0;
            this->pictureBox1->TabStop = false;
            // 
            // buttonStart
            // 
            this->buttonStart->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->buttonStart->Location = System::Drawing::Point(832, 65);
            this->buttonStart->Name = L"buttonStart";
            this->buttonStart->Size = System::Drawing::Size(135, 50);
            this->buttonStart->TabIndex = 1;
            this->buttonStart->Text = L"Начать";
            this->buttonStart->UseVisualStyleBackColor = true;
            this->buttonStart->Click += gcnew System::EventHandler(this, &MyForm::buttonStart_Click);
            // 
            // buttonStop
            // 
            this->buttonStop->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->buttonStop->Location = System::Drawing::Point(832, 121);
            this->buttonStop->Name = L"buttonStop";
            this->buttonStop->Size = System::Drawing::Size(135, 50);
            this->buttonStop->TabIndex = 2;
            this->buttonStop->Text = L"Остановить";
            this->buttonStop->UseVisualStyleBackColor = true;
            this->buttonStop->Click += gcnew System::EventHandler(this, &MyForm::buttonStop_Click);
            // 
            // labelTitle
            // 
            this->labelTitle->AutoSize = true;
            this->labelTitle->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->labelTitle->Location = System::Drawing::Point(154, 36);
            this->labelTitle->Name = L"labelTitle";
            this->labelTitle->Size = System::Drawing::Size(473, 26);
            this->labelTitle->TabIndex = 3;
            this->labelTitle->Text = L"Движение шестиугольника по траектории";
            // 
            // labelSpeed
            // 
            this->labelSpeed->AutoSize = true;
            this->labelSpeed->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->labelSpeed->Location = System::Drawing::Point(845, 506);
            this->labelSpeed->Name = L"labelSpeed";
            this->labelSpeed->Size = System::Drawing::Size(110, 22);
            this->labelSpeed->TabIndex = 5;
            this->labelSpeed->Text = L"Скорость: 2";
            // 
            // trackBarSpeed
            // 
            this->trackBarSpeed->BackColor = System::Drawing::SystemColors::ButtonFace;
            this->trackBarSpeed->Location = System::Drawing::Point(832, 545);
            this->trackBarSpeed->Name = L"trackBarSpeed";
            this->trackBarSpeed->Size = System::Drawing::Size(135, 69);
            this->trackBarSpeed->TabIndex = 6;
            this->trackBarSpeed->ValueChanged += gcnew System::EventHandler(this, &MyForm::trackBarSpeed_ValueChanged);
            // 
            // buttonDirection
            // 
            this->buttonDirection->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->buttonDirection->Location = System::Drawing::Point(832, 177);
            this->buttonDirection->Name = L"buttonDirection";
            this->buttonDirection->Size = System::Drawing::Size(135, 50);
            this->buttonDirection->TabIndex = 7;
            this->buttonDirection->Text = L"Против часовой";
            this->buttonDirection->UseVisualStyleBackColor = true;
            this->buttonDirection->Click += gcnew System::EventHandler(this, &MyForm::buttonDirection_Click);
            // 
            // menuStrip1
            // 
            this->menuStrip1->GripMargin = System::Windows::Forms::Padding(2, 2, 0, 2);
            this->menuStrip1->ImageScalingSize = System::Drawing::Size(24, 24);
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->colorsToolStripMenuItem });
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(1012, 33);
            this->menuStrip1->TabIndex = 4;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // colorsToolStripMenuItem
            // 
            this->colorsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->borderColorToolStripMenuItem });
            this->colorsToolStripMenuItem->Name = L"colorsToolStripMenuItem";
            this->colorsToolStripMenuItem->Size = System::Drawing::Size(76, 29);
            this->colorsToolStripMenuItem->Text = L"Цвета";
            // 
            // borderColorToolStripMenuItem
            // 
            this->borderColorToolStripMenuItem->Name = L"borderColorToolStripMenuItem";
            this->borderColorToolStripMenuItem->Size = System::Drawing::Size(224, 34);
            this->borderColorToolStripMenuItem->Text = L"Цвет контура";
            this->borderColorToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::borderColorToolStripMenuItem_Click);
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(41, 678);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(144, 44);
            this->button1->TabIndex = 10;
            this->button1->Text = L"button1";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(248, 678);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(155, 43);
            this->button2->TabIndex = 11;
            this->button2->Text = L"button2";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
            // 
            // button3
            // 
            this->button3->Location = System::Drawing::Point(457, 678);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(148, 43);
            this->button3->TabIndex = 12;
            this->button3->Text = L"button3";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
            this->ClientSize = System::Drawing::Size(1012, 750);
            this->Controls->Add(this->button3);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->trackBarSize);
            this->Controls->Add(this->labelSize);
            this->Controls->Add(this->buttonDirection);
            this->Controls->Add(this->trackBarSpeed);
            this->Controls->Add(this->labelSpeed);
            this->Controls->Add(this->labelTitle);
            this->Controls->Add(this->buttonStop);
            this->Controls->Add(this->buttonStart);
            this->Controls->Add(this->pictureBox1);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"MyForm";
            this->Text = L"Motion";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSize))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBarSpeed))->EndInit();
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion обработчки событий

    private:
        System::Void buttonStart_Click(System::Object^ sender, System::EventArgs^ e) {
            StartAnimation();
        }

        System::Void buttonStop_Click(System::Object^ sender, System::EventArgs^ e) {
            StopAnimation();
        }

        System::Void trackBarSpeed_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
            speed = trackBarSpeed->Value;
            labelSpeed->Text = String::Format("Скорость: {0}", speed);
        }

        System::Void buttonDirection_Click(System::Object^ sender, System::EventArgs^ e) {
            clockwise = !clockwise;
            buttonDirection->Text = clockwise ? L"Против часовой" : L"По часовой";
            DrawCurrentPosition();
        }

        System::Void fillColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
            ColorDialog^ colorDialog = gcnew ColorDialog();
            colorDialog->Color = StarFillColor;

            if (colorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                SetFillColor(colorDialog->Color);
        }

        System::Void borderColorToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
            ColorDialog^ colorDialog = gcnew ColorDialog();
            colorDialog->Color = StarBorderColor;

            if (colorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                SetBorderColor(colorDialog->Color);
        }

        System::Void trackBarSize_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
            StarSize = trackBarSize->Value;
            labelSize->Text = String::Format("Размер: {0}", StarSize);
            DrawCurrentPosition();
        }

        private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
            isDotted = !isDotted;
            button1->Text = isDotted ? "Dotted" : "Solid";
            DrawTrajectory();
        }

//        bool isRotating = false;
        //bool isSizeChanging = false;


private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
    isRotating = !isRotating;
    button2->Text = isRotating ? "Rotating" : "NonRotating";
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
    isSizeChanging = !isSizeChanging;
    button3->Text = isSizeChanging ? "Pulsating" : "Constant Size";
}

};
}