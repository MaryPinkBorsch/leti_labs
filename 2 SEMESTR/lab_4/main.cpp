#include "lab_4.h"

using namespace std;

int main(int argc, char *argv[])
{

    std::string filename2 = "result4.txt";
    std::ofstream res(filename2, ios::out | ios::trunc);

    // {
    //     ifstream input;
    //     input.open("in4_1.txt", std::ios_base::in);
    //     FormularV f1;
    //     // ListNodeV * head;
    //     Read_file(input, res, f1);
    // }

    BIG_process(res, "in4_1.txt");
    BIG_process(res, "in4_2.txt");
    BIG_process(res, "in4_3.txt");
    BIG_process(res, "in4_4.txt");
    BIG_process(res, "in4_5.txt");
    BIG_process(res, "in4_6.txt");
    BIG_process(res, "in4_7.txt");

    return 0;
}