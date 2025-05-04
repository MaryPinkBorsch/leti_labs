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

    // y_tabl = {};
    // Заполнение вектора структурами
    AC_table table0_0;
    table0_0.Run = 0;
    table0_0.Size = 0;
    table0_0.Len = 14;
    table0_0.Arr_Code = {1, 0, 1, 0};
    y_tabl.push_back(table0_0);

    AC_table table0_1;
    table0_1.Run = 0;
    table0_1.Size = 1;
    table0_1.Len = 12;
    table0_1.Arr_Code = {0, 0};
    y_tabl.push_back(table0_1);

    AC_table table0_2;
    table0_2.Run = 0;
    table0_2.Size = 2;
    table0_2.Len = 12;
    table0_2.Arr_Code = {0, 1};
    y_tabl.push_back(table0_2);

    AC_table table0_3;
    table0_3.Run = 0;
    table0_3.Size = 3;
    table0_3.Len = 13;
    table0_3.Arr_Code = {1, 0, 0};
    y_tabl.push_back(table0_3);

    AC_table table0_4;
    table0_4.Run = 0;
    table0_4.Size = 4;
    table0_4.Len = 14;
    table0_4.Arr_Code = {1, 0, 1, 1};
    y_tabl.push_back(table0_4);

    AC_table table0_5;
    table0_5.Run = 0;
    table0_5.Size = 5;
    table0_5.Len = 15;
    table0_5.Arr_Code = {1, 1, 0, 1, 0};
    y_tabl.push_back(table0_5);

    AC_table table0_6;
    table0_6.Run = 0;
    table0_6.Size = 6;
    table0_6.Len = 17;
    table0_6.Arr_Code = {1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(table0_6);

    AC_table table0_7;
    table0_7.Run = 0;
    table0_7.Size = 7;
    table0_7.Len = 18;
    table0_7.Arr_Code = {1, 1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(table0_7);

    AC_table table0_8;
    table0_8.Run = 0;
    table0_8.Size = 8;
    table0_8.Len = 10;
    table0_8.Arr_Code = {1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(table0_8);

    AC_table table0_9;
    table0_9.Run = 0;
    table0_9.Size = 9;
    table0_9.Len = 16;
    table0_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0};
    y_tabl.push_back(table0_9);

    AC_table table0_A;
    table0_A.Run = 0;
    table0_A.Size = 10;
    table0_A.Len = 16;
    table0_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};
    y_tabl.push_back(table0_A);

    AC_table table1_1;
    table1_1.Run = 1;
    table1_1.Size = 1;
    table1_1.Len = 14;
    table1_1.Arr_Code = {1, 1, 0, 0};
    y_tabl.push_back(table1_1);

    AC_table table1_2;
    table1_2.Run = 1;
    table1_2.Size = 2;
    table1_2.Len = 15;
    table1_2.Arr_Code = {1, 1, 0, 1, 1};
    y_tabl.push_back(table1_2);

    AC_table table1_3;
    table1_3.Run = 1;
    table1_3.Size = 3;
    table1_3.Len = 17;
    table1_3.Arr_Code = {1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(table1_3);

    AC_table table1_4;
    table1_4.Run = 1;
    table1_4.Size = 4;
    table1_4.Len = 19;
    table1_4.Arr_Code = {1, 1, 1, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(table1_4);

    AC_table table1_5;
    table1_5.Run = 1;
    table1_5.Size = 5;
    table1_5.Len = 11;
    table1_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(table1_5);

    AC_table table1_6;
    table1_6.Run = 1;
    table1_6.Size = 6;
    table1_6.Len = 16;
    table1_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0};
    y_tabl.push_back(table1_6);

    AC_table table1_7;
    table1_7.Run = 1;
    table1_7.Size = 7;
    table1_7.Len = 16;
    table1_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1};
    y_tabl.push_back(table1_7);

    AC_table table1_8;
    table1_8.Run = 1;
    table1_8.Size = 8;
    table1_8.Len = 16;
    table1_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0};
    y_tabl.push_back(table1_8);

    AC_table table1_9;
    table1_9.Run = 1;
    table1_9.Size = 9;
    table1_9.Len = 16;
    table1_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1};
    y_tabl.push_back(table1_9);

    AC_table table1_A;
    table1_A.Run = 1;
    table1_A.Size = 10;
    table1_A.Len = 16;
    table1_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0};
    y_tabl.push_back(table1_A);

    AC_table table2_1;
    table2_1.Run = 2;
    table2_1.Size = 1;
    table2_1.Len = 15;
    table2_1.Arr_Code = {1, 1, 1, 0, 0};
    y_tabl.push_back(table2_1);

    AC_table table2_2;
    table2_2.Run = 2;
    table2_2.Size = 2;
    table2_2.Len = 18;
    table2_2.Arr_Code = {1, 1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(table2_2);

    AC_table table2_3;
    table2_3.Run = 2;
    table2_3.Size = 3;
    table2_3.Len = 10;
    table2_3.Arr_Code = {1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(table2_3);

    AC_table table2_4;
    table2_4.Run = 2;
    table2_4.Size = 4;
    table2_4.Len = 12;
    table2_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0};
    y_tabl.push_back(table2_4);

    AC_table table2_5;
    table2_5.Run = 2;
    table2_5.Size = 5;
    table2_5.Len = 16;
    table2_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1};
    y_tabl.push_back(table2_5);

    AC_table table2_6;
    table2_6.Run = 2;
    table2_6.Size = 6;
    table2_6.Len = 16;
    table2_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0};
    y_tabl.push_back(table2_6);

    AC_table table2_7;
    table2_7.Run = 2;
    table2_7.Size = 7;
    table2_7.Len = 16;
    table2_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1};
    y_tabl.push_back(table2_7);

    AC_table table2_8;
    table2_8.Run = 2;
    table2_8.Size = 8;
    table2_8.Len = 16;
    table2_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0};
    y_tabl.push_back(table2_8);

    AC_table table2_9;
    table2_9.Run = 2;
    table2_9.Size = 9;
    table2_9.Len = 16;
    table2_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1};
    y_tabl.push_back(table2_9);

    AC_table table2_A;
    table2_A.Run = 2;
    table2_A.Size = 10;
    table2_A.Len = 16;
    table2_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0};
    y_tabl.push_back(table2_A);

    AC_table table3_1;
    table3_1.Run = 3;
    table3_1.Size = 1;
    table3_1.Len = 16;
    table3_1.Arr_Code = {1, 1, 1, 0, 1, 0};
    y_tabl.push_back(table3_1);

    AC_table table3_2;
    table3_2.Run = 3;
    table3_2.Size = 2;
    table3_2.Len = 19;
    table3_2.Arr_Code = {1, 1, 1, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(table3_2);

    AC_table table3_3;
    table3_3.Run = 3;
    table3_3.Size = 3;
    table3_3.Len = 12;
    table3_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1};
    y_tabl.push_back(table3_3);

    AC_table table3_4;
    table3_4.Run = 3;
    table3_4.Size = 4;
    table3_4.Len = 16;
    table3_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1};
    y_tabl.push_back(table3_4);

    AC_table table3_5;
    table3_5.Run = 3;
    table3_5.Size = 5;
    table3_5.Len = 16;
    table3_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0};
    y_tabl.push_back(table3_5);

    AC_table table3_6;
    table3_6.Run = 3;
    table3_6.Size = 6;
    table3_6.Len = 16;
    table3_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1};
    y_tabl.push_back(table3_6);

    AC_table table3_7;
    table3_7.Run = 3;
    table3_7.Size = 7;
    table3_7.Len = 16;
    table3_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0};
    y_tabl.push_back(table3_7);

    AC_table table3_8;
    table3_8.Run = 3;
    table3_8.Size = 8;
    table3_8.Len = 16;
    table3_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1};
    y_tabl.push_back(table3_8);

    AC_table table3_9;
    table3_9.Run = 3;
    table3_9.Size = 9;
    table3_9.Len = 16;
    table3_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0};
    y_tabl.push_back(table3_9);

    AC_table table3_A;
    table3_A.Run = 3;
    table3_A.Size = 10;
    table3_A.Len = 16;
    table3_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1};
    y_tabl.push_back(table3_A);

    AC_table table4_1;
    table4_1.Run = 4;
    table4_1.Size = 1;
    table4_1.Len = 16;
    table4_1.Arr_Code = {1, 1, 1, 0, 1, 1};
    y_tabl.push_back(table4_1);

    AC_table table4_2;
    table4_2.Run = 4;
    table4_2.Size = 2;
    table4_2.Len = 10;
    table4_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(table4_2);

    AC_table table4_3;
    table4_3.Run = 4;
    table4_3.Size = 3;
    table4_3.Len = 16;
    table4_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0};
    y_tabl.push_back(table4_3);

    AC_table table4_4;
    table4_4.Run = 4;
    table4_4.Size = 4;
    table4_4.Len = 16;
    table4_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1};
    y_tabl.push_back(table4_4);

    AC_table table4_5;
    table4_5.Run = 4;
    table4_5.Size = 5;
    table4_5.Len = 16;
    table4_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0};
    y_tabl.push_back(table4_5);

    AC_table table4_6;
    table4_6.Run = 4;
    table4_6.Size = 6;
    table4_6.Len = 16;
    table4_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1};
    y_tabl.push_back(table4_6);

    AC_table table4_7;
    table4_7.Run = 4;
    table4_7.Size = 7;
    table4_7.Len = 16;
    table4_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0};
    y_tabl.push_back(table4_7);

    AC_table table4_8;
    table4_8.Run = 4;
    table4_8.Size = 8;
    table4_8.Len = 16;
    table4_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1};
    y_tabl.push_back(table4_8);

    AC_table table4_9;
    table4_9.Run = 4;
    table4_9.Size = 9;
    table4_9.Len = 16;
    table4_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0};
    y_tabl.push_back(table4_9);

    AC_table table4_A;
    table4_A.Run = 4;
    table4_A.Size = 10;
    table4_A.Len = 16;
    table4_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1};
    y_tabl.push_back(table4_A);

    AC_table table5_1;
    table5_1.Run = 5;
    table5_1.Size = 1;
    table5_1.Len = 17;
    table5_1.Arr_Code = {1, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(table5_1);

    AC_table table5_2;
    table5_2.Run = 5;
    table5_2.Size = 2;
    table5_2.Len = 11;
    table5_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(table5_2);

    AC_table table5_3;
    table5_3.Run = 5;
    table5_3.Size = 3;
    table5_3.Len = 16;
    table5_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0};
    y_tabl.push_back(table5_3);

    AC_table table5_4;
    table5_4.Run = 5;
    table5_4.Size = 4;
    table5_4.Len = 16;
    table5_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1};
    y_tabl.push_back(table5_4);

    AC_table table5_5;
    table5_5.Run = 5;
    table5_5.Size = 5;
    table5_5.Len = 16;
    table5_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0};
    y_tabl.push_back(table5_5);

    AC_table table5_6;
    table5_6.Run = 5;
    table5_6.Size = 6;
    table5_6.Len = 16;
    table5_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1};
    y_tabl.push_back(table5_6);

    AC_table table5_7;
    table5_7.Run = 5;
    table5_7.Size = 7;
    table5_7.Len = 16;
    table5_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0};
    y_tabl.push_back(table5_7);

    AC_table table5_8;
    table5_8.Run = 5;
    table5_8.Size = 8;
    table5_8.Len = 16;
    table5_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1};
    y_tabl.push_back(table5_8);

    AC_table table5_9;
    table5_9.Run = 5;
    table5_9.Size = 9;
    table5_9.Len = 16;
    table5_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0};
    y_tabl.push_back(table5_9);

    AC_table table5_A;
    table5_A.Run = 5;
    table5_A.Size = 10;
    table5_A.Len = 16;
    table5_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1};
    y_tabl.push_back(table5_A);

    AC_table table6_1;
    table6_1.Run = 6;
    table6_1.Size = 1;
    table6_1.Len = 17;
    table6_1.Arr_Code = {1, 1, 1, 1, 0, 1, 1};
    y_tabl.push_back(table6_1);

    AC_table table6_2;
    table6_2.Run = 6;
    table6_2.Size = 2;
    table6_2.Len = 12;
    table6_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(table6_2);

    AC_table table6_3;
    table6_3.Run = 6;
    table6_3.Size = 3;
    table6_3.Len = 16;
    table6_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0};
    y_tabl.push_back(table6_3);

    AC_table table6_4;
    table6_4.Run = 6;
    table6_4.Size = 4;
    table6_4.Len = 16;
    table6_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1};
    y_tabl.push_back(table6_4);

    AC_table table6_5;
    table6_5.Run = 6;
    table6_5.Size = 5;
    table6_5.Len = 16;
    table6_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0};
    y_tabl.push_back(table6_5);

    AC_table table6_6;
    table6_6.Run = 6;
    table6_6.Size = 6;
    table6_6.Len = 16;
    table6_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1};
    y_tabl.push_back(table6_6);

    AC_table table6_7;
    table6_7.Run = 6;
    table6_7.Size = 7;
    table6_7.Len = 16;
    table6_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0};
    y_tabl.push_back(table6_7);

    AC_table table6_8;
    table6_8.Run = 6;
    table6_8.Size = 8;
    table6_8.Len = 16;
    table6_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1};
    y_tabl.push_back(table6_8);

    AC_table table6_9;
    table6_9.Run = 6;
    table6_9.Size = 9;
    table6_9.Len = 16;
    table6_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0};
    y_tabl.push_back(table6_9);

    AC_table table6_A;
    table6_A.Run = 6;
    table6_A.Size = 10;
    table6_A.Len = 16;
    table6_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1};
    y_tabl.push_back(table6_A);

    AC_table table7_1;
    table7_1.Run = 7;
    table7_1.Size = 1;
    table7_1.Len = 18;
    table7_1.Arr_Code = {1, 1, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(table7_1);

    AC_table table7_2;
    table7_2.Run = 7;
    table7_2.Size = 2;
    table7_2.Len = 12;
    table7_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(table7_2);

    AC_table table7_3;
    table7_3.Run = 7;
    table7_3.Size = 3;
    table7_3.Len = 16;
    table7_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0};
    y_tabl.push_back(table7_3);

    AC_table table7_4;
    table7_4.Run = 7;
    table7_4.Size = 4;
    table7_4.Len = 16;
    table7_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1};
    y_tabl.push_back(table7_4);

    AC_table table7_5;
    table7_5.Run = 7;
    table7_5.Size = 5;
    table7_5.Len = 16;
    table7_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0};
    y_tabl.push_back(table7_5);

    AC_table table7_6;
    table7_6.Run = 7;
    table7_6.Size = 6;
    table7_6.Len = 16;
    table7_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1};
    y_tabl.push_back(table7_6);

    AC_table table7_7;
    table7_7.Run = 7;
    table7_7.Size = 7;
    table7_7.Len = 16;
    table7_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0};
    y_tabl.push_back(table7_7);

    AC_table table7_8;
    table7_8.Run = 7;
    table7_8.Size = 8;
    table7_8.Len = 16;
    table7_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1};
    y_tabl.push_back(table7_8);

    AC_table table7_9;
    table7_9.Run = 7;
    table7_9.Size = 9;
    table7_9.Len = 16;
    table7_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0};
    y_tabl.push_back(table7_9);

    AC_table table7_A;
    table7_A.Run = 7;
    table7_A.Size = 10;
    table7_A.Len = 16;
    table7_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1};
    y_tabl.push_back(table7_A);

    AC_table table8_1;
    table8_1.Run = 8;
    table8_1.Size = 1;
    table8_1.Len = 19;
    table8_1.Arr_Code = {1, 1, 1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(table8_1);

    AC_table table8_2;
    table8_2.Run = 8;
    table8_2.Size = 2;
    table8_2.Len = 15;
    table8_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
    y_tabl.push_back(table8_2);

    AC_table table8_3;
    table8_3.Run = 8;
    table8_3.Size = 3;
    table8_3.Len = 16;
    table8_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(table8_3);

    AC_table table8_4;
    table8_4.Run = 8;
    table8_4.Size = 4;
    table8_4.Len = 16;
    table8_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(table8_4);

    AC_table table8_5;
    table8_5.Run = 8;
    table8_5.Size = 5;
    table8_5.Len = 16;
    table8_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(table8_5);

    AC_table table8_6;
    table8_6.Run = 8;
    table8_6.Size = 6;
    table8_6.Len = 16;
    table8_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(table8_6);

    AC_table table8_7;
    table8_7.Run = 8;
    table8_7.Size = 7;
    table8_7.Len = 16;
    table8_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(table8_7);

    AC_table table8_8;
    table8_8.Run = 8;
    table8_8.Size = 8;
    table8_8.Len = 16;
    table8_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1};
    y_tabl.push_back(table8_8);

    AC_table table8_9;
    table8_9.Run = 8;
    table8_9.Size = 9;
    table8_9.Len = 16;
    table8_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0};
    y_tabl.push_back(table8_9);

    AC_table table8_A;
    table8_A.Run = 8;
    table8_A.Size = 10;
    table8_A.Len = 16;
    table8_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1};
    y_tabl.push_back(table8_A);

    // Таблицы Run = 9
    AC_table table9_1;
    table9_1.Run = 9;
    table9_1.Size = 1;
    table9_1.Len = 19;
    table9_1.Arr_Code = {1, 1, 1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(table9_1);

    AC_table table9_2;
    table9_2.Run = 9;
    table9_2.Size = 2;
    table9_2.Len = 16;
    table9_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0};
    y_tabl.push_back(table9_2);

    AC_table table9_3;
    table9_3.Run = 9;
    table9_3.Size = 3;
    table9_3.Len = 16;
    table9_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1};
    y_tabl.push_back(table9_3);

    AC_table table9_4;
    table9_4.Run = 9;
    table9_4.Size = 4;
    table9_4.Len = 16;
    table9_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
    y_tabl.push_back(table9_4);

    AC_table table9_5;
    table9_5.Run = 9;
    table9_5.Size = 5;
    table9_5.Len = 16;
    table9_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1};
    y_tabl.push_back(table9_5);

    AC_table table9_6;
    table9_6.Run = 9;
    table9_6.Size = 6;
    table9_6.Len = 16;
    table9_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0};
    y_tabl.push_back(table9_6);

    AC_table table9_7;
    table9_7.Run = 9;
    table9_7.Size = 7;
    table9_7.Len = 16;
    table9_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1};
    y_tabl.push_back(table9_7);

    AC_table table9_8;
    table9_8.Run = 9;
    table9_8.Size = 8;
    table9_8.Len = 16;
    table9_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0};
    y_tabl.push_back(table9_8);

    AC_table table9_9;
    table9_9.Run = 9;
    table9_9.Size = 9;
    table9_9.Len = 16;
    table9_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    y_tabl.push_back(table9_9);

    AC_table table9_A;
    table9_A.Run = 9;
    table9_A.Size = 10;
    table9_A.Len = 16;
    table9_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0};
    y_tabl.push_back(table9_A);

    // Таблицы Run = A
    AC_table tableA_1;
    tableA_1.Run = 10; // A == 10
    tableA_1.Size = 1;
    tableA_1.Len = 19;
    tableA_1.Arr_Code = {1, 1, 1, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(tableA_1);

    AC_table tableA_2;
    tableA_2.Run = 10; // A == 10
    tableA_2.Size = 2;
    tableA_2.Len = 16;
    tableA_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1};
    y_tabl.push_back(tableA_2);

    AC_table tableA_3;
    tableA_3.Run = 10; // A == 10
    tableA_3.Size = 3;
    tableA_3.Len = 16;
    tableA_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0};
    y_tabl.push_back(tableA_3);

    AC_table tableA_4;
    tableA_4.Run = 10; // A == 10
    tableA_4.Size = 4;
    tableA_4.Len = 16;
    tableA_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1};
    y_tabl.push_back(tableA_4);

    AC_table tableA_5;
    tableA_5.Run = 10; // A == 10
    tableA_5.Size = 5;
    tableA_5.Len = 16;
    tableA_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0};
    y_tabl.push_back(tableA_5);

    AC_table tableA_6;
    tableA_6.Run = 10; // A == 10
    tableA_6.Size = 6;
    tableA_6.Len = 16;
    tableA_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1};
    y_tabl.push_back(tableA_6);

    AC_table tableA_7;
    tableA_7.Run = 10; // A == 10
    tableA_7.Size = 7;
    tableA_7.Len = 16;
    tableA_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0};
    y_tabl.push_back(tableA_7);

    AC_table tableA_8;
    tableA_8.Run = 10; // A == 10
    tableA_8.Size = 8;
    tableA_8.Len = 16;
    tableA_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1};
    y_tabl.push_back(tableA_8);

    AC_table tableA_9;
    tableA_9.Run = 10; // A == 10
    tableA_9.Size = 9;
    tableA_9.Len = 16;
    tableA_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0};
    y_tabl.push_back(tableA_9);

    AC_table tableA_A;
    tableA_A.Run = 10; // A == 10
    tableA_A.Size = 10;
    tableA_A.Len = 16;
    tableA_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1};
    y_tabl.push_back(tableA_A);

    // Таблицы Run = B
    AC_table tableB_1;
    tableB_1.Run = 11; // B == 11
    tableB_1.Size = 1;
    tableB_1.Len = 10;
    tableB_1.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(tableB_1);

    AC_table tableB_2;
    tableB_2.Run = 11; // B == 11
    tableB_2.Size = 2;
    tableB_2.Len = 16;
    tableB_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0};
    y_tabl.push_back(tableB_2);

    AC_table tableB_3;
    tableB_3.Run = 11; // B == 11
    tableB_3.Size = 3;
    tableB_3.Len = 16;
    tableB_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1};
    y_tabl.push_back(tableB_3);

    AC_table tableB_4;
    tableB_4.Run = 11; // B == 11
    tableB_4.Size = 4;
    tableB_4.Len = 16;
    tableB_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0};
    y_tabl.push_back(tableB_4);

    AC_table tableB_5;
    tableB_5.Run = 11; // B == 11
    tableB_5.Size = 5;
    tableB_5.Len = 16;
    tableB_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1};
    y_tabl.push_back(tableB_5);

    AC_table tableB_6;
    tableB_6.Run = 11; // B == 11
    tableB_6.Size = 6;
    tableB_6.Len = 16;
    tableB_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0};
    y_tabl.push_back(tableB_6);

    AC_table tableB_7;
    tableB_7.Run = 11; // B == 11
    tableB_7.Size = 7;
    tableB_7.Len = 16;
    tableB_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1};
    y_tabl.push_back(tableB_7);

    AC_table tableB_8;
    tableB_8.Run = 11; // B == 11
    tableB_8.Size = 8;
    tableB_8.Len = 16;
    tableB_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0};
    y_tabl.push_back(tableB_8);

    AC_table tableB_9;
    tableB_9.Run = 11; // B == 11
    tableB_9.Size = 9;
    tableB_9.Len = 16;
    tableB_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1};
    y_tabl.push_back(tableB_9);

    AC_table tableB_A;
    tableB_A.Run = 11; // B == 11
    tableB_A.Size = 10;
    tableB_A.Len = 16;
    tableB_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0};
    y_tabl.push_back(tableB_A);

    // Таблицы Run = C
    AC_table tableC_1;
    tableC_1.Run = 12; // C == 12
    tableC_1.Size = 1;
    tableC_1.Len = 10;
    tableC_1.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(tableC_1);

    AC_table tableC_2;
    tableC_2.Run = 12; // C == 12
    tableC_2.Size = 2;
    tableC_2.Len = 16;
    tableC_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1};
    y_tabl.push_back(tableC_2);

    AC_table tableC_3;
    tableC_3.Run = 12; // C == 12
    tableC_3.Size = 3;
    tableC_3.Len = 16;
    tableC_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0};
    y_tabl.push_back(tableC_3);

    AC_table tableC_4;
    tableC_4.Run = 12; // C == 12
    tableC_4.Size = 4;
    tableC_4.Len = 16;
    tableC_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1};
    y_tabl.push_back(tableC_4);

    // Таблицы Run = C
    AC_table tableC_5;
    tableC_5.Run = 12; // C == 12
    tableC_5.Size = 5;
    tableC_5.Len = 16;
    tableC_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0};
    y_tabl.push_back(tableC_5);

    AC_table tableC_6;
    tableC_6.Run = 12; // C == 12
    tableC_6.Size = 6;
    tableC_6.Len = 16;
    tableC_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1};
    y_tabl.push_back(tableC_6);

    AC_table tableC_7;
    tableC_7.Run = 12; // C == 12
    tableC_7.Size = 7;
    tableC_7.Len = 16;
    tableC_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0};
    y_tabl.push_back(tableC_7);

    AC_table tableC_8;
    tableC_8.Run = 12; // C == 12
    tableC_8.Size = 8;
    tableC_8.Len = 16;
    tableC_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};
    y_tabl.push_back(tableC_8);

    AC_table tableC_9;
    tableC_9.Run = 12; // C == 12
    tableC_9.Size = 9;
    tableC_9.Len = 16;
    tableC_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
    y_tabl.push_back(tableC_9);

    AC_table tableC_A;
    tableC_A.Run = 12; // C == 12
    tableC_A.Size = 10;
    tableC_A.Len = 16;
    tableC_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1};
    y_tabl.push_back(tableC_A);

    // Таблицы Run = D
    AC_table tableD_1;
    tableD_1.Run = 13; // D == 13
    tableD_1.Size = 1;
    tableD_1.Len = 11;
    tableD_1.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(tableD_1);

    AC_table tableD_2;
    tableD_2.Run = 13; // D == 13
    tableD_2.Size = 2;
    tableD_2.Len = 16;
    tableD_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0};
    y_tabl.push_back(tableD_2);

    AC_table tableD_3;
    tableD_3.Run = 13; // D == 13
    tableD_3.Size = 3;
    tableD_3.Len = 16;
    tableD_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1};
    y_tabl.push_back(tableD_3);

    AC_table tableD_4;
    tableD_4.Run = 13; // D == 13
    tableD_4.Size = 4;
    tableD_4.Len = 16;
    tableD_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0};
    y_tabl.push_back(tableD_4);

    // Таблицы Run = D
    AC_table tableD_5;
    tableD_5.Run = 13; // D == 13
    tableD_5.Size = 5;
    tableD_5.Len = 16;
    tableD_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1};
    y_tabl.push_back(tableD_5);

    AC_table tableD_6;
    tableD_6.Run = 13; // D == 13
    tableD_6.Size = 6;
    tableD_6.Len = 16;
    tableD_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0};
    y_tabl.push_back(tableD_6);

    AC_table tableD_7;
    tableD_7.Run = 13; // D == 13
    tableD_7.Size = 7;
    tableD_7.Len = 16;
    tableD_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1};
    y_tabl.push_back(tableD_7);

    AC_table tableD_8;
    tableD_8.Run = 13; // D == 13
    tableD_8.Size = 8;
    tableD_8.Len = 16;
    tableD_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0};
    y_tabl.push_back(tableD_8);

    AC_table tableD_9;
    tableD_9.Run = 13; // D == 13
    tableD_9.Size = 9;
    tableD_9.Len = 16;
    tableD_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1};
    y_tabl.push_back(tableD_9);

    AC_table tableD_A;
    tableD_A.Run = 13; // D == 13
    tableD_A.Size = 10;
    tableD_A.Len = 16;
    tableD_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0};
    y_tabl.push_back(tableD_A);

    // Таблицы Run = E
    AC_table tableE_1;
    tableE_1.Run = 14; // E == 14
    tableE_1.Size = 1;
    tableE_1.Len = 16;
    tableE_1.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1};
    y_tabl.push_back(tableE_1);

    AC_table tableE_2;
    tableE_2.Run = 14; // E == 14
    tableE_2.Size = 2;
    tableE_2.Len = 16;
    tableE_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0};
    y_tabl.push_back(tableE_2);

    AC_table tableE_3;
    tableE_3.Run = 14; // E == 14
    tableE_3.Size = 3;
    tableE_3.Len = 16;
    tableE_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1};
    y_tabl.push_back(tableE_3);

    // Таблицы Run = E
    AC_table tableE_4;
    tableE_4.Run = 14; // E == 14
    tableE_4.Size = 4;
    tableE_4.Len = 16;
    tableE_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
    y_tabl.push_back(tableE_4);

    // Таблицы Run = E
    AC_table tableE_5;
    tableE_5.Run = 14; // E == 14
    tableE_5.Size = 5;
    tableE_5.Len = 16;
    tableE_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1};
    y_tabl.push_back(tableE_5);

    AC_table tableE_6;
    tableE_6.Run = 14; // E == 14
    tableE_6.Size = 6;
    tableE_6.Len = 16;
    tableE_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
    y_tabl.push_back(tableE_6);

    AC_table tableE_7;
    tableE_7.Run = 14; // E == 14
    tableE_7.Size = 7;
    tableE_7.Len = 16;
    tableE_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1};
    y_tabl.push_back(tableE_7);

    AC_table tableE_8;
    tableE_8.Run = 14; // E == 14
    tableE_8.Size = 8;
    tableE_8.Len = 16;
    tableE_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0};
    y_tabl.push_back(tableE_8);

    AC_table tableE_9;
    tableE_9.Run = 14; // E == 14
    tableE_9.Size = 9;
    tableE_9.Len = 16;
    tableE_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1};
    y_tabl.push_back(tableE_9);

    AC_table tableE_A;
    tableE_A.Run = 14; // E == 14
    tableE_A.Size = 10;
    tableE_A.Len = 16;
    tableE_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0};
    y_tabl.push_back(tableE_A);

    // Таблицы Run = F
    AC_table tableF_0;
    tableF_0.Run = 15; // F == 15
    tableF_0.Size = 0;
    tableF_0.Len = 11;
    tableF_0.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(tableF_0);

    AC_table tableF_1;
    tableF_1.Run = 15; // F == 15
    tableF_1.Size = 1;
    tableF_1.Len = 16;
    tableF_1.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1};
    y_tabl.push_back(tableF_1);

    AC_table tableF_2;
    tableF_2.Run = 15; // F == 15
    tableF_2.Size = 2;
    tableF_2.Len = 16;
    tableF_2.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
    y_tabl.push_back(tableF_2);

    AC_table tableF_3;
    tableF_3.Run = 15; // F == 15
    tableF_3.Size = 3;
    tableF_3.Len = 16;
    tableF_3.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    y_tabl.push_back(tableF_3);

    AC_table tableF_4;
    tableF_4.Run = 15; // F == 15
    tableF_4.Size = 4;
    tableF_4.Len = 16;
    tableF_4.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0};
    y_tabl.push_back(tableF_4);

    AC_table tableF_5;
    tableF_5.Run = 15; // F == 15
    tableF_5.Size = 5;
    tableF_5.Len = 16;
    tableF_5.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1};
    y_tabl.push_back(tableF_5);

    AC_table tableF_6;
    tableF_6.Run = 15; // F == 15
    tableF_6.Size = 6;
    tableF_6.Len = 16;
    tableF_6.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0};
    y_tabl.push_back(tableF_6);

    AC_table tableF_7;
    tableF_7.Run = 15; // F == 15
    tableF_7.Size = 7;
    tableF_7.Len = 16;
    tableF_7.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1};
    y_tabl.push_back(tableF_7);

    AC_table tableF_8;
    tableF_8.Run = 15; // F == 15
    tableF_8.Size = 8;
    tableF_8.Len = 16;
    tableF_8.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0};
    y_tabl.push_back(tableF_8);

    AC_table tableF_9;
    tableF_9.Run = 15; // F == 15
    tableF_9.Size = 9;
    tableF_9.Len = 16;
    tableF_9.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1};
    y_tabl.push_back(tableF_9);

    AC_table tableF_A;
    tableF_A.Run = 15; // F == 15
    tableF_A.Size = 10;
    tableF_A.Len = 16;
    tableF_A.Arr_Code = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    y_tabl.push_back(tableF_A);

    // там нужна другая такая же огромная таблица, но я пока их скопирую
    croma_tabl = y_tabl;
}

// функции для обработки одной пары
void huff_code(var_pair &input, huff_pair &output, bool flag, bool y_flag)
{
    std::vector<DC_table> Y_table = {};
    std::vector<DC_table> Cromatic_table = {};
    std::vector<AC_table> y_tabl = {};
    std::vector<AC_table> croma_tabl = {};

    table(Y_table, Cromatic_table, y_tabl, croma_tabl);

    // flag = 0 ---> обработка ДС
    // flag = 1 ---> обработка АС

    // y_flag = 0 ---> обработка Сб или Ср
    // y_flag = 1 ---> обработка У

    // обработка ДС
    if (!flag)
    {

        // обработка ДС для У
        if (y_flag)
        {
            for (auto &it : Y_table)
            {
                // ищем категорию равную длине бин формы числа
                if (it.Category == input.num_bits)
                {
                    output.bit_arr = input.arr;
                    output.h_code = it.Arr_Code;
                    output.positive = input.positive;
                    break;
                }
            }
        }
        // обработка DC для Сб или Ср
        else
        {
            for (auto &it : Cromatic_table)
            {
                // ищем категорию равную длине бин формы числа
                if (it.Category == input.num_bits)
                {
                    output.bit_arr = input.arr;
                    output.h_code = it.Arr_Code;
                    output.positive = input.positive;
                    break;
                }
            }
        }
    }
    // обработка АС
    else
    {
        // обработка AС для У
        if (y_flag)
        {
            for (auto &it : y_tabl)
            {
                // ищем SIZE равную длине бин формы числа  и пробег равный числу повторений РЛЕ
                if (it.Size == input.num_bits && it.Run == input.AC_num)
                {
                    output.bit_arr = input.arr;
                    output.h_code = it.Arr_Code;
                    output.positive = input.positive;
                    output.AC_num = input.AC_num;
                    break;
                }
            }
        }
        // обработка AC Сб или Ср
        else
        {
            for (auto &it : croma_tabl)
            {
                // ищем SIZE равную длине бин формы числа  и пробег равный числу повторений РЛЕ
                if (it.Size == input.num_bits && it.Run == input.AC_num)
                {
                    output.bit_arr = input.arr;
                    output.h_code = it.Arr_Code;
                    output.positive = input.positive;
                    output.AC_num = input.AC_num;
                    break;
                }
            }
        }
    }
}
void huff_decode(huff_pair &input, var_pair &output, bool flag, bool y_flag)
{
    std::vector<DC_table> Y_table = {};
    std::vector<DC_table> Cromatic_table = {};
    std::vector<AC_table> y_tabl = {};
    std::vector<AC_table> croma_tabl = {};

    table(Y_table, Cromatic_table, y_tabl, croma_tabl);
    // flag = 0 ---> обработка ДС
    // flag = 1 ---> обработка АС

    // y_flag = 0 ---> обработка Сб или Ср
    // y_flag = 1 ---> обработка У

    // обработка ДС
    if (!flag)
    {

        // обработка ДС для У
        if (y_flag)
        {
            for (auto &it : Y_table)
            {
                // ищем по коду Хаффмана
                if (it.Arr_Code == input.h_code)
                {
                    output.arr = input.bit_arr;
                    output.num_bits = output.arr.size();
                    output.positive = input.positive;

                    break;
                }
            }
        }
        // обработка DC для Сб или Ср
        else
        {
            for (auto &it : Cromatic_table)
            {
                // ищем по коду Хаффмана
                if (it.Arr_Code == input.h_code)
                {
                    output.arr = input.bit_arr;
                    output.num_bits = output.arr.size();
                    output.positive = input.positive;

                    break;
                }
            }
        }
    }
    // обработка АС
    else
    {
        // обработка AС для У
        if (y_flag)
        {
            for (auto &it : y_tabl)
            {
                // ищем по коду Хаффмана  и пробег равный числу повторений РЛЕ
                if (it.Arr_Code == input.h_code && it.Run == input.AC_num)
                {
                    // output.bit_arr = input.arr;
                    // output.h_code = it.Arr_Code;
                    // output.positive = input.positive;
                    // output.AC_num = input.AC_num;
                    output.arr = input.bit_arr;
                    output.num_bits = output.arr.size();
                    output.AC_num = input.AC_num;
                    output.positive = input.positive;
                    break;
                }
            }
        }
        // обработка AC Сб или Ср
        else
        {
            for (auto &it : croma_tabl)
            {
                // ищем по коду Хаффмана  и пробег равный числу повторений РЛЕ
                if (it.Arr_Code == input.h_code && it.Run == input.AC_num)
                {
                    // output.bit_arr = input.arr;
                    // output.h_code = it.Arr_Code;
                    // output.positive = input.positive;
                    // output.AC_num = input.AC_num;
                    output.arr = input.bit_arr;
                    output.num_bits = output.arr.size();
                    output.AC_num = input.AC_num;
                    output.positive = input.positive;
                    break;
                }
            }
        }
    }
}