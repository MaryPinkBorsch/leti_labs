#include "text3.h"

using namespace std;


int main(int argc, char *argv[])
{
    std::string filename2 = "result2.txt";
    std::ofstream res(filename2, ios::out | ios::trunc);

    // Считать файл в объект структуры Text
    Text text;
    text.BIG_process("in3_1.txt", res);

    // Text text1;
    // text1.BIG_process("in2.txt", res);

    // Text text2;
    // text2.BIG_process("in2_2.txt", res);

    // Text text3;
    // text3.BIG_process("in2_3.txt", res);

    // Text text4;
    // text4.BIG_process("in2_4.txt", res);

    // Text text5;
    // text5.BIG_process("in2_5.txt", res);

    // Text text6;
    // text6.BIG_process("in2_6.txt", res);

    // Text text7;
    // text7.BIG_process("in2_7.txt", res);

    // Пройти по строкам и выделить предложения
    // Отсортировать предложения по количеству знаков препинания
    // inplace удалить из объекта текст предложения имевшие макс. число знаков препинания

    return 0;
}
