
struct Predlojenie_idx
{
    int stroka_idx_start = -1;        // номер строки в котором находится начало предложения
    int stroka_idx_end = -1;          // номер строки в котором находится конец предложения
    int stroka_smeschenie_start = -1; // номер первого символа в предложения строке
    int stroka_smeschenie_end = -1;   // номер последнего символа в предложения строке

    int num_znaki = 0;
};