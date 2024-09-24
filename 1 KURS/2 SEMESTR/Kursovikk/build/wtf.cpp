#include <iostream>

int main (int argc, char * argv[]) 
{
    std::string read_buf;
    std::cin >> std::noskipws >> read_buf;
    std::cout << read_buf << std::endl;
    return 0;
}
