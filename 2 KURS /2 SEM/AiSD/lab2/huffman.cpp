#include "huffman.h"
#include <iostream>
#include <vector>
using namespace std;

void table(std::vector<DC_table> &Y_table, std::vector<DC_table> &Cromatic_table, std::vector<AC_table> &y_tabl, std::vector<AC_table> &croma_tabl)
{

    Y_table.resize(12);
    // Заполнение вектора структурами
    DC_table table0;
    table0.Category = 0;
    table0.Len = 2;
    table0.Arr_Code = {0, 0}; // Добавим дополнительный элемент, чтобы длина массива Arr_Code соответствовала Len
    Y_table.push_back(table0);

    DC_table table1;
    table1.Category = 1;
    table1.Len = 3;
    table1.Arr_Code = {0, 1, 0};
    Y_table.push_back(table1);

    DC_table table2;
    table2.Category = 2;
    table2.Len = 3;
    table2.Arr_Code = {0, 1, 1};
    Y_table.push_back(table2);

    DC_table table3;
    table3.Category = 3;
    table3.Len = 3;
    table3.Arr_Code = {1, 0, 0};
    Y_table.push_back(table3);

    DC_table table4;
    table4.Category = 4;
    table4.Len = 3;
    table4.Arr_Code = {1, 0, 1};
    Y_table.push_back(table4);

    DC_table table5;
    table5.Category = 5;
    table5.Len = 3;
    table5.Arr_Code = {1, 1, 0};
    Y_table.push_back(table5);

    DC_table table6;
    table6.Category = 6;
    table6.Len = 4;
    table6.Arr_Code = {1, 1, 1, 0};
    Y_table.push_back(table6);

    DC_table table7;
    table7.Category = 7;
    table7.Len = 5;
    table7.Arr_Code = {1, 1, 1, 1, 0};
    Y_table.push_back(table7);

    DC_table table8;
    table8.Category = 8;
    table8.Len = 6;
    table8.Arr_Code = {1, 1, 1, 1, 1, 0};
    Y_table.push_back(table8);

    DC_table table9;
    table9.Category = 9;
    table9.Len = 7;
    table9.Arr_Code = {1, 1, 1, 1, 1, 1, 0};
    Y_table.push_back(table9);

    DC_table table10;
    table10.Category = 10;
    table10.Len = 8;
    table10.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0};
    Y_table.push_back(table10);

    DC_table table11;
    table11.Category = 11;
    table11.Len = 9;
    table11.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0};
    Y_table.push_back(table11);

    // Вывод для проверки (необязательно)
    // for (const auto &table : Y_table)
    // {
    //     std::cout << "Category: " << table.Category << ", Len: " << table.Len << ", Code: ";
    //     for (int code : table.Arr_Code)
    //     {
    //         std::cout << code;
    //     }
    //     std::cout << std::endl;
    // }

    DC_table table0;
    table0.Category = 0;
    table0.Len = 2;
    table0.Arr_Code = {0, 0};
    Cromatic_table.push_back(table0);

    DC_table table1;
    table1.Category = 1;
    table1.Len = 2;
    table1.Arr_Code = {0, 1};
    Cromatic_table.push_back(table1);

    DC_table table2;
    table2.Category = 2;
    table2.Len = 2;
    table2.Arr_Code = {1, 0};
    Cromatic_table.push_back(table2);

    DC_table table3;
    table3.Category = 3;
    table3.Len = 3;
    table3.Arr_Code = {1, 1, 0};
    Cromatic_table.push_back(table3);

    DC_table table4;
    table4.Category = 4;
    table4.Len = 4;
    table4.Arr_Code = {1, 1, 1, 0};
    Cromatic_table.push_back(table4);

    DC_table table5;
    table5.Category = 5;
    table5.Len = 5;
    table5.Arr_Code = {1, 1, 1, 1, 0};
    Cromatic_table.push_back(table5);

    DC_table table6;
    table6.Category = 6;
    table6.Len = 6;
    table6.Arr_Code = {1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table6);

    DC_table table7;
    table7.Category = 7;
    table7.Len = 7;
    table7.Arr_Code = {1, 1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table7);

    DC_table table8;
    table8.Category = 8;
    table8.Len = 8;
    table8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table8);

    DC_table table9;
    table9.Category = 9;
    table9.Len = 9;
    table9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table9);

    DC_table table10;
    table10.Category = 10;
    table10.Len = 10;
    table10.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table10);

    DC_table table11;
    table11.Category = 11;
    table11.Len = 11;
    table11.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    Cromatic_table.push_back(table11);

    // теперь заполнениe АС таблиц, там будет ж*па
}
