#include "MyForm.h"



using namespace System;
using namespace System::Windows::Forms;

static void OnForm1Closed(Object^ sender, FormClosedEventArgs^ e)
{
    exit(0);
}

[STAThreadAttribute]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    IDZ1::MyForm form;
    form.FormClosed += gcnew FormClosedEventHandler(&OnForm1Closed);
    Application::Run(% form);
    return 0;
}