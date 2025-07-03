#include "MyForm.h"

#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Задание2::MyForm form;
    Application::Run(% form);
    return 0;
}