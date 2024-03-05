#include "text.h"

using namespace std;

void Text::print2(std::ofstream &res)
{
    for (int i = 0; i < num_stroki; i++)
    {
        stroki[i].print1(res);
    }
}

bool Text::read_file(std::string filename, std::ofstream &res)
{
    ifstream input;
    input.open(filename, std::ios_base::in);
    num_stroki = 0;

    // !! TODO proverka eof i return false !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // пока в стриме есть что читать и в тексте есть место, считываем строки
    while (!input.eof() && num_stroki < M)
    {
        if (stroki[num_stroki].read_StrM(input, res) == false)
        {
            if (input.eof())
                break;
        }
        else
            ++num_stroki;
    }

    if(!input.eof() && num_stroki == M)
    {
        cout << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!" << endl;
        res << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много строк!"<< endl;
        exit(1);
    }
    return true;
}

bool Text::IsRazdelitel(char c)
{
    return c == ';' || c == '.' || c == '?' || c == '!';
}

bool Text::IsZnak(char c)
{
    return c == ',' || c == ':' || c == '-' || c == '\'';
}

void Text::process_znaki(std::ofstream &res)
{
    // идем по всем строкам
    num_predlojenia = 0;
    for (int i = 0; i < num_stroki; i++)
    {
        int j = 0;
        // идем по всем символам каждой строки
        while (stroki[i].massiv[j] != stroki[i].Marker)
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
                    if(num_predlojenia == M)
                    {
                        cout << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много предложений!" << endl;
                        res << "ОБРАБОТКА НЕВОЗМОЖНА, слишком много предложений!"<< endl;
                        exit(1);
                    }
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
    int Max = -1;
    for (int i = 0; i < num_predlojenia; i++)
    {
        if (indexi_predlojenii[i].num_znaki > Max)
        {
            Max = indexi_predlojenii[i].num_znaki;
        }
    }
    cout << endl
         << "max kol-vo znakov = " << Max << endl;
    res << endl
        << "max kol-vo znakov = " << Max << endl;
    return Max;
}

void Text::Delete(std::ofstream &res)
{
    int Max = MaxZnaki(res);
    int FromStr = 0;  // строка откуда переносится предложение
    int FromChar = 0; // символ в строке откуда переносится предложение
    int ToStr = 0;    // строка куда переносится предложение
    int ToChar = 0;   // символ в строке куда переносится предложение
    int next_predlojenie = 0;

    // пока не прошли весь текст
    while ((next_predlojenie < num_predlojenia) &&
           ((FromStr < indexi_predlojenii[num_predlojenia - 1].stroka_idx_end) ||
            (FromStr == indexi_predlojenii[num_predlojenia - 1].stroka_idx_end &&
             (FromChar <= indexi_predlojenii[num_predlojenia - 1].stroka_smeschenie_end))))
    {
        // пропускаем предложения с максимальным количеством знаком
        while (indexi_predlojenii[next_predlojenie].num_znaki == Max)
        {
            ++next_predlojenie;
            if (next_predlojenie == num_predlojenia)
                break;
            // выставляем то откуда копировать в начало следующего предложения
            FromStr = indexi_predlojenii[next_predlojenie].stroka_idx_start;
            FromChar = indexi_predlojenii[next_predlojenie].stroka_smeschenie_start;
        }
        if (next_predlojenie == num_predlojenia)
            break;
        // начинаем перетаскивать предложение налево\вверх
        while (FromStr < indexi_predlojenii[next_predlojenie].stroka_idx_end || (FromStr == indexi_predlojenii[next_predlojenie].stroka_idx_end && (FromChar <= indexi_predlojenii[next_predlojenie].stroka_smeschenie_end)))
        {
            StrM & strTo = stroki[ToStr];
            StrM & strFrom = stroki[FromStr];
            // копируем только если надо (смещение ненулевое) т е  если FromStr!=ToStr
            // и FromChar != ToChar
            if (!((FromStr == ToStr) && (FromChar == ToChar)))
            {
                
                strTo.massiv[ToChar] = strFrom.massiv[FromChar];
                //stroki[ToStr].massiv[ToChar] = stroki[FromStr].massiv[FromChar];
            }  
            /////!!!!//! "ЭТО НЕ 2МЕРНЫЙ МАССИВ!!!! это структура в стркутуре" вынести 2мерность в 1дномерность!!!!!! добавить еще функций
            ++ToChar;
            // переезжаем в начало следующей строчки куда копировать если надо
            if (strTo.massiv[ToChar] == stroki[ToStr].Marker)
            {
                ++ToStr;
                ToChar = 0;
            }
            ++FromChar;
            // переезжаем в начало следующей строчки откуда копировать если надо
            if (strFrom.massiv[FromChar] == stroki[FromStr].Marker)
            {
                ++FromStr;
                FromChar = 0;
            }
        }
        // следующее предложение
        ++next_predlojenie;
    }
    StrM & strTo = stroki[ToStr];
    // ставим новый маркер после всееех сдвигов
    //if (ToChar != 0)
    strTo.massiv[ToChar] = stroki[ToStr].Marker;
    // отбрасываем лишние строчки после сдвигов если таковые остались
    if (ToStr < num_stroki - 1)
        num_stroki = ToStr + (ToChar != 0 ? 1 : 0);
}

// 17. Предложения могут находится в разных строках текста.
// Удалить в тексте те предложения, которые: 3) содержат максимальное
// число знаков препинания

void Text::BIG_process(string filename, ofstream &res)
{
    cout << endl
         << " Началась обработка файла " << filename << endl
         << endl;
    res << endl
        << " Началась обработка файла " << filename << endl
        << endl;
    if (!read_file(filename, res))
    {
        res << "Oshibka reading" << endl;
        cout << "Oshibka reading" << endl;
    }
    else
    {

        cout << endl
             << " Исходный текст: " << endl;
        res << endl
            << " Исходный текст: " << endl;
        print2(res);
        process_znaki(res);
        Delete(res);

        cout << endl
             << endl;
        cout << "RESULT: " << endl
             << endl;

        res << endl
            << endl;
        res << "RESULT: " << endl
            << endl;

        print2(res);
        cout << endl;
        res << endl;

        cout << " Кончилась обработка файла " << filename << endl
             << endl
             << endl
             << endl;
        res << " Кончилась обработка файла " << filename << endl
            << endl
            << endl
            << endl;
    }
}