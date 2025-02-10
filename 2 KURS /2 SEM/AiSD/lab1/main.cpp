#include <iostream>
#include <stdio.h>
#include "rle.h"

using namespace std;
void print(const vector<unsigned char> &tmp)
{
    for (int i = 0; i < tmp.size(); i++) 
    {
        printf("%.2X, ", tmp[i]);
    }
    cout << endl;
}
int main(int argc, char *argv[])
{
    vector<unsigned char> tmp = {0xCF, 0xCF, 0xCF, 0xCF, 0xCF};
    vector<unsigned char> tmp1;
    vector<unsigned char> tmp2;

    rle_compress_2_1(tmp, tmp1);
    cout << "исходная строка: ";
    print(tmp);
    cout << "сжатая строка: ";
    print(tmp1);

    rle_decompress_2_1(tmp1, tmp2);
    
    cout << "разжатая строка: ";
    print(tmp2);

    return 0;
}