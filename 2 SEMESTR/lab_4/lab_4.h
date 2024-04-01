#pragma once

#include "FormularV.h"

// это объявление типа указателя на функцию которая работает с элементами вертикальных формуляров
typedef void (*ListNodeVFunction)(ListNodeV& toProcess);

// есть две функции которые работают 
void ToLowerCase(ListNodeV& toProcess);
void ToUpperCase(ListNodeV& toProcess);

bool Read_file(std::ifstream &input, std::ofstream &res, FormularV &formularVert);
void BIG_process(std::ofstream &res, std::string in_filename);
void print2(std::ofstream &res, FormularV &formularVert);
