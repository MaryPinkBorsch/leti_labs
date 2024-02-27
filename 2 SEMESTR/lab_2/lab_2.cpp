#include "lab_2.h"

using namespace std;

void StrM::print1(std::ofstream &res) 
{
    int i = 0;
    while (massiv[i] != Marker)
    {
        cout << massiv[i];
        res << massiv[i];
        i++;
    }
    cout << endl;
    res << endl;
}

// CHTENIE S MARKEROM 1.1.1
bool StrM::read_StrM(std::ifstream & input, std::ofstream &res)
{
    int i = 0;
    char s;
    
    input >> Marker; // первый маркер для нашего массива
    if (input.eof())
    {
        // cout << " Файл пуст, упс"; ///// !!!! s etim toje nado choto sdelat
        // cout << endl;

        // res << " Файл пуст, упс";
        // res << endl;

        return false;
    }
    // char MMarker = -1; // второй маркер для прерывания чтения с файла
    // input >> MMarker;

    if (input.eof())
    {
        massiv[0] = Marker;
        cout << "stroka pusta v " << endl;
        res << "stroka pusta v " << endl;
    }
    else
    {
        while (1)
        {
            input >> noskipws >> s;
           
            if (s == '\n')
                break;

            if (input.eof())
                break;

            massiv[i] = s;
            i++;
            if (i >= N)
            {
                break;
            }
        }
        massiv[i] = Marker;
    }

    
    if (massiv[0] == Marker )
    {
        cout << " Строка пуста  " << endl;
        res << " Строка пуста  " << endl;
    }
    return true;
}

void Text::print2(std::ofstream &res) 
{
    for(int i = 0; i < num_stroki; i++)
    {
        stroki[i].print1(res);
    }
}


bool Text::read_file(std::string filename, std::ofstream &res)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    num_stroki = 0;
    //TODO proverka eof i return false!!!!!!!!!!!!!!!!!

    // пока в стриме есть что читать и в тексте есть место, считываем строки
    while(!input.eof() && num_stroki < M) 
    {
        if (stroki[num_stroki].read_StrM(input, res) == false)
            break;
        ++num_stroki;
    }
    return true;
}

bool Text::IsRazdelitel(char c) 
{
    return c == ';' || c == '.' || c == '?' || c == '!'; // TODO: "..."
}

bool Text::IsZnak(char c) 
{
    return  c == ',' || c == ':' || c == '-' || c == '\'';
}

void Text::process_znaki(std::ofstream &res)
{
    // идем по всем строкам
    num_predlojenia = 0;
    for (int i=0; i < num_stroki; i++)
    {
        int j = 0;
        // идем по всем символам каждой строки
        while(stroki[i].massiv[j] != stroki[i].Marker)
        {
            // если символ - разделитель предложений            
            if (IsRazdelitel(stroki[i].massiv[j]))
            {
                // и при этом у нас уже было начало предолжения
                if (indexi_predlojenii[num_predlojenia].stroka_idx_start != -1) 
                {
                    // заполним текущее предложение (концы)
                    indexi_predlojenii[num_predlojenia].stroka_idx_end = i;
                    indexi_predlojenii[num_predlojenia].stroka_smeschenie_end = j;
                    indexi_predlojenii[num_predlojenia].num_znaki++;
                    // увеличим счетчик предложений
                    ++num_predlojenia;
                }
            }
            else 
            {
                // у нас обычный символ. Если начала предолжения не было, то это оно
                if (indexi_predlojenii[num_predlojenia].stroka_idx_start == -1) 
                {
                    // заполняем начало предолжения
                    indexi_predlojenii[num_predlojenia].stroka_idx_start = i;
                    indexi_predlojenii[num_predlojenia].stroka_smeschenie_start = j;
                }
                if (IsZnak(stroki[i].massiv[j]))
                    indexi_predlojenii[num_predlojenia].num_znaki++;
            }
            // следующий символ
            ++j;
        }
    }
}

int Text::MaxZnaki(std::ofstream &res)
{
    int Max= -1;
    for (int i = 0; i < num_predlojenia; i++)
    {
        if(indexi_predlojenii[i].num_znaki > Max)
        {
            Max = indexi_predlojenii[i].num_znaki;
        }
    }
    cout << "max kol-vo znakov = " << Max << endl;
    res << "max kol-vo znakov = " << Max << endl;
    return Max;
} 

void Text::Delete(std::ofstream &res)
{
    int Max = MaxZnaki(res);
    int FromStr = 0;         // строка откуда переносится предложение 
    int FromChar = 0;        // символ в строке откуда переносится предложение
    int ToStr = 0;           // строка куда переносится предложение 
    int ToChar = 0;          //символ в строке куда переносится предложение
    bool NadoPerenesti = false; // flag

    // цикл по предложениям
    for(int i = 0; i < num_predlojenia; i++)
    {
        // если перенос еще не начался
        if(!NadoPerenesti)
        {
            // проверить должен ли начаться перенос
            if(indexi_predlojenii[i].num_znaki == Max)
            {
                NadoPerenesti = true;
            }
            else
                continue; // переход к след. предложению
        }
        // если перенос уже начался
        if(NadoPerenesti)
        {
            int next_predlojenie = i; // индекс предложения из которого будем переносить
            if(indexi_predlojenii[i].num_znaki == Max)
            {
                ToStr = indexi_predlojenii[i].stroka_idx_start;
                ToChar = indexi_predlojenii[i].stroka_smeschenie_start;
                while(indexi_predlojenii[next_predlojenie].num_znaki == Max && next_predlojenie < num_predlojenia)
                    ++next_predlojenie;
                if (next_predlojenie == num_predlojenia)
                    break;
                FromStr = indexi_predlojenii[next_predlojenie].stroka_idx_start;
                FromChar = indexi_predlojenii[next_predlojenie].stroka_smeschenie_start;
            }
            while(FromStr <= indexi_predlojenii[next_predlojenie].stroka_idx_end && (
                FromStr < indexi_predlojenii[next_predlojenie].stroka_smeschenie_end || (
                FromChar <= indexi_predlojenii[next_predlojenie].stroka_smeschenie_end)))
            {
                stroki[ToStr].massiv[ToChar] = stroki[FromStr].massiv[FromChar];
                ++ToChar;
                if (stroki[ToStr].massiv[ToChar] == stroki[ToStr].Marker) 
                {
                    ++ToStr;
                    ToChar = 0;
                }
                ++FromChar;
                if (stroki[FromStr].massiv[FromChar] == stroki[FromStr].Marker) 
                {
                    ++FromStr;
                    FromChar = 0;
                }
            }
        }
    }
    if (ToChar != 0)
        stroki[ToStr].massiv[ToChar] = stroki[ToStr].Marker;
    if (ToStr < num_stroki - 1)
        num_stroki = ToStr + 1;
}


// 17. Предложения могут находится в разных строках текста. 
// Удалить в тексте те предложения, которые: 3) содержат максимальное
// число знаков препинания

int main(int argc, char * argv[]) 
{
    std::string filename2 = "result2.txt";
    std::ofstream res(filename2, ios::out | ios::trunc);

    // Считать файл в объект структуры Text
    Text text;
    text.read_file("in2_1.txt", res);
    text.print2(res);
    text.process_znaki(res);

    Text text1;
    text1.read_file("in2.txt", res);
    text1.print2(res);
    text1.process_znaki(res);
    text1.Delete(res);
    cout<<endl;
    text1.print2(res);

    // Пройти по строкам и выделить предложения
    // Отсортировать предложения по количеству знаков препинания
    // inplace удалить из объекта текст предложения имевшие макс. число знаков препинания
    
    return 0;
}
