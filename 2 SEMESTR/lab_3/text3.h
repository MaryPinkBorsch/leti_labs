#include "ListNode.h"
struct Text
{
  ListNode *head = nullptr;

  int num_stroki;
  

  bool read_file(std::string filename, std::ofstream &res);
  void print2(std::ofstream &res);
  void process_znaki(std::ofstream &res);
  bool IsZnak(char c);
  void BIG_process(std::string filename, std::ofstream &res);
};