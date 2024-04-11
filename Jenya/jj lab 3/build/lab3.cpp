#include "text3.h"

using namespace std;

int main(int argc, char *argv[])
{
    std::string filename2 = "result333.txt";
    std::ofstream res(filename2, ios::out | ios::trunc);

    // примеры: в начало, в середину, в конец, пустой файл изначально, в позицию больше чем список

    // Считать файл в объект структуры Text
    Text text;
    text.process("in33_1.txt", res);

    // Text text1;
    // text1.process("in33.txt", res); 

    Text text2;
    text2.process("in33_2.txt", res);

    Text text3;
    text3.process("in33_3.txt", res); 

    Text text4;
    text4.process("in33_4.txt", res);

    // Text text5;
    // text5.process("in33_5.txt", res);

    // Text text6;
    // text6.process("in33_6.txt", res);

    // Text text7;
    // text7.process("in33_7.txt", res);

    // Text text8;
    // text8.process("in33_8.txt", res);

    // Text text9;
    // text9.process("in33_9.txt", res);



    return 0;
}