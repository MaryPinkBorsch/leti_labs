#include "ListNode.h"
struct Text
{
    // строки из которых состоит текст. предложение может быть в нескольих строках подряд
    StrM stroki[M];

    ListNode * head = nullptr;
    //Predlojenie_idx indexi_predlojenii[M];

    int num_stroki;
   // int num_predlojenia;

    bool read_file(std::string filename, std::ofstream &res);
    void print2(std::ofstream &res);
    void process_znaki(std::ofstream &res);
  //  bool IsRazdelitel(char c);
    bool IsZnak(char c);
    //int MaxZnaki(std::ofstream &res);
    void Delete(std::ofstream &res);

    void BIG_process(std::string filename, std::ofstream &res);
};