#include "ListNode.h"

struct Text
{
  ListNode *head = nullptr;
  int num_stroki;

  bool read_file(std::string filename, std::ofstream &res);
  void print(std::ofstream &res);
  void process(std::string filename, std::ofstream &res);
  void clear(std::ofstream &res);
  // функция вставляет элементы после to
  void insert_after(ListNode *to, StrM & toInsert, int Kolichestvo);

};