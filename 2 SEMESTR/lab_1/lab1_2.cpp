#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
static const int N = 100;
class SlovoIdx
{
    int start = -1; // индекс начала слова
    int end = -1;   // индекс конца слова
    int oldidx = -1;
};
// 1.2.1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class StrL1
{
    char massiv[N + 1];
    int Len;
    char Marker; // это внешний маркер (внутреннего нет, тут лен)

    class SlovoIdx
    {
        public: // norm vashe??
        int start = -1; // индекс начала слова
        int end = -1;   // индекс конца слова
        int oldidx = -1;
    };

    public:
    void SetL(int L)
    {    
        Len = L;  
    }

    int GetL(ifstream &input, std::string filename, ofstream &res)
    {
        
        if (input.eof())
        {
            cout << "Не хватает LEN, стrока пуста в файле " << filename;
        }
        int L;
        input >> L;

        if (L < 0)
        {
            L = 0;
            cout << "Отрицательная длина исправлена в 0 в  " << filename << endl;
            res << "Отрицательная длина исправлена в 0 в  " << filename << endl;
            return false;
        }

        if (L > N)
        {
            L = N;
            cout << "Слишком большая длина исправлена в " << filename << endl;
            res << "Слишком большая длина исправлена в " << filename << endl;
        }
        return L; 
    }

    bool Read1(std::string filename, ofstream &res)
        {
                ifstream input;
        input.open(filename, std::ios_base::in);
        // // TODO: тут проверить что файл открылся gotovo
        if (input.eof())
        {
            cout << " Файл input пуст, упс 1" << endl;
            res << " Файл input пуст, упс 1" << endl;
            return false;
        }

        int i = 0;
        char s;
        int lenght = GetL(input, filename, res);
        SetL(lenght);             

        input >> Marker; // vnesniy маркер для нашего массива

        if (input.eof())
        {
            cout << " МАркера нет " << endl;
            cout << " Файл " << filename << " пуст, упс 2";
            cout << endl;

            res << " МАркера нет " << endl;
            res << " Файл " << filename << " пуст, упс 2";
            res << endl;

            return false;
        }

        if (input.eof())
        {
            cout << " Файл " << filename << " пуст, упс 3";
            cout << endl;

            res << " Файл " << filename << " пуст, упс 3";
            res << endl;

            return false;
        }

        if (input.eof())
        {
            cout << "stroka pusta v " << filename << endl;
            res << "stroka pusta v " << filename << endl;

            massiv[0] = Marker;
        }
        else
        {
            while (1)
            {
                if (i >= N || i >= Len) // i >= ?
                {
                    // stroka.massiv[i + 1] = stroka.Marker; //
                    break;
                }
                input >> noskipws >> s;
                if (s == '\n')
                    break;

                if (input.eof())
                    break;
                if (s == Marker)
                    break;

                massiv[i] = s;
                i++;
                
            }
            if (Len > i)
            {
                cout << " реальная длина меньше заданной " << endl;
                res << " реальная длина меньше заданной " << endl;
                Len = i;
                
            }

            massiv[i] = Marker; //?????????
        }
        if (massiv[0] == Marker)
        {
            cout << "stroka pusta v " << filename << endl;
            res << "stroka pusta v " << filename << endl;
        }

        return true;
    }

    void print2(ofstream &res)
    {
        int i = 0;
        
        while (massiv[i] != Marker || i < Len)
        {
            cout << massiv[i];
            res << massiv[i];
            i++;
        }
        res << endl;
        cout << endl;
    }

    void inplace_swap_slova2(StrL1 &stroka, int start1, int end1, int start2, int end2)
    {
        if (start1 == start2 && end1 == end2)
            return;

        // !!!! слово1 это левое слово то есть end1 < start2
        if (end1 > start2)
        {
            int tmp = start1;
            start1 = start2;
            start2 = tmp;
            tmp = end1;
            end1 = end2;
            end2 = tmp;
        }
        int len1 = end1 - start1 + 1;
        int len2 = end2 - start2 + 1;
        // если слово1 короче слова2 тогда
        if (len1 < len2)
        {
            // меняем буквы пока позволяет длинна слова1
            for (int i = 0; i < len1; i++)
            {
                char tmp = massiv[start1 + i];
                massiv[start1 + i] = massiv[start2 + i];
                massiv[start2 + i] = tmp;
            }
            // после этого по одной переносим оставшиеся буквы слова два влево и сдвигаем остаток вправо
            // количество сдвигаемых симовлов равно start2 - start1
            int sdvig = start2 - start1; // na skolko sdvigat
            int nado_sdvinut = len2 - len1;
            for (int i = 0; i < nado_sdvinut; i++)
            {
                char tmp = massiv[start2 + len1 + i];
                for (int j = 0; j < sdvig; j++)
                {
                    massiv[start2 + len1 + i - j] = massiv[start2 + len1 + i - j - 1]; // прямо как a[i+1] = a[i] pochti СПРАВА НАЛЕВО
                }
                massiv[end1 + 1 + i] = tmp;
            }
        }
        else
        {
            // иначе если слово2 короче слова1 тогда
            // меняем буквы пока позволяет длинна слова2
            for (int i = 0; i < len2; i++)
            {
                char tmp = massiv[start1 + i];
                massiv[start1 + i] = massiv[start2 + i];
                massiv[start2 + i] = tmp;
            }
            // после этого по одной переносим оставшиеся буквы слова1  вправо и сдвигаем остаток влево
            int sdvig = end2 - (start1 + len2);    // na skolko sdvigat (сколько символов переедет за 1 цикл налево на 1 символ)
            int nado_sdvinut = len1 - len2; // кол-во перемещаемых элеменотов
            for (int i = 0; i < nado_sdvinut; i++)
            {
                char tmp = massiv[start1 + len2];
                for (int j = 0; j < sdvig; j++)
                {
                    massiv[start1 + len2 + j] = massiv[start1 + len2 + j + 1]; // прямо как a[i+1] = a[i] pochti
                }
                massiv[end2] = tmp;
            }
        }
    }

    int Compare_slova_idx2(StrL1 &stroka, int start1, int end1, int start2, int end2) // сравниваем слова
    {
        int i = 0;
        while (start1 + i != end1 && start2 + i != end2)
        {
            if (massiv[start1 + i] < massiv[start2 + i])
                return -1;
            if (massiv[start1 + i] > massiv[start2 + i])
                return 1;
            i++;
        }
        if (start1 + i == end1 && start2 + i == end2)
        {
            return 0; // абсолютно одинаковые слова
        }

        if (start1 + i != end1) // так левое слово длиннее правого т.е. больше го
            return 1;
        else
            return -1;
    }

    void Bubble_sort_idx2(StrL1 &stroka, SlovoIdx *SlovaIdx, int num_slova)
    {
        // sorting
        bool swapped = true;
        while (swapped)
        {
            swapped = false;
            for (int i = 0; i < num_slova - 1; i++)
            {
                if (Compare_slova_idx2(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end) == 1)
                {
                    inplace_swap_slova2(stroka, SlovaIdx[i].start, SlovaIdx[i].end, SlovaIdx[i + 1].start, SlovaIdx[i + 1].end);
                    int len1 = SlovaIdx[i].end - SlovaIdx[i].start;
                    int len2 = SlovaIdx[i + 1].end - SlovaIdx[i + 1].start;
                    SlovaIdx[i].end = SlovaIdx[i].start + len2;
                    SlovaIdx[i + 1].start += len2 - len1;
                    SlovaIdx[i + 1].end = SlovaIdx[i + 1].start + len1;
                    swapped = true;
                }
            }
        }
    }

    void process_inPlace2(StrL1 &stroka)
    {
        // заполнить массив индексов начала и конца слов в строке
        // отстортировать массив одновременно меняя местами слова

        int i = 0;
        int num_slova = 0;
        SlovoIdx Slova[N];

        while (i < Len || massiv[i] != Marker) /////!
        {
            if (massiv[i] != ' ')
            {
                if (Slova[num_slova].start == -1)
                {
                    Slova[num_slova].start = i;
                    Slova[num_slova].oldidx = num_slova;
                }
            }
            else if (massiv[i] == ' ') /////// dobavit || stroka.massiv[i] != '\n'???
            {
                if (Slova[num_slova].start != -1)
                {
                    Slova[num_slova].end = i - 1;
                    num_slova++;
                }
            }
            i++;
        }
        // для случая когда после последнего слова нет пробела
        if (Slova[num_slova].end == -1 && Slova[num_slova].start != -1)
        {
            Slova[num_slova].end = i - 1;
            num_slova++;
        }
        Bubble_sort_idx2(stroka, Slova, num_slova);
    }



    int file_process_21(string filename, StrL1 s, ofstream &res)
    {

    
        cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;
        res << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    

        if (s.Read1(filename, res) == false)
        {
            cout << "Обработка файла " << filename << " завершена (2)" << endl;
            res << "Обработка файла " << filename << " завершена (2)" << endl;
            return 1;
            abort;
        }
        else
        {
            s.print2(res);
            // tut sdelat vivod  v res nashalo/itog            
            s.process_inPlace2(s); // norm?
            s.print2(res);

            cout << "Обработка файла " << filename << " завершена (2.1)" << endl;
            res << "Обработка файла " << filename << " завершена (2.1)" << endl;
            return 0;
        }
    }

};

int main(int argc, char *argv[])
{
    string filename2 = "result.txt";
    string filename1 = "input1_2_1m1.txt";
    ofstream res(filename2, ios::out | ios::trunc);
    // cout << "Добро пожаловать в lab 1.1.1 Калюжной Марии 3352 " << endl;

    StrL1 s1;
    s1.file_process_21("in1_2m.txt", s1, res); // norm?????????? pro s1??
    res << endl << endl;
    cout << endl << endl;
    

    return 0;
}