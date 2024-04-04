#pragma once

#include "FormularV.h"

// это объявление типа указателя на функцию которая работает с элементами вертикальных формуляров
typedef void (*ListNodeVFunction)(ListNodeV& toProcess,std::ofstream &res);

typedef void (*ListNodeVFunction2)(FormularV& toProcess_F,std::ofstream &res);

// есть  функции которые работают (на них можно поставить указатель) 
void ToLowerCase(ListNodeV& toProcess,std::ofstream &res);
void ToUpperCase(ListNodeV& toProcess,std::ofstream &res);
void Delete_Znaki(ListNodeV& toProcess,std::ofstream &res);
void Delete_Znaki2(FormularV& toProcess_F, std::ofstream &res);
void Swap_Znaki(FormularV &toProcess_F, std::ofstream &res);


bool Read_file(std::ifstream &input, std::ofstream &res, FormularV &formularVert);
void BIG_process(std::ofstream &res, std::string in_filename);
void print2(std::ofstream &res, FormularV &formularVert);
