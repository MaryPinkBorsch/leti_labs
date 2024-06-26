#include "common.h"

#include <filesystem>

#include "Birga.h"

using namespace std;

// нужны структуры:
// 1) Еmployer, поля - name, work_field, adress, phone
// 2) Worker, поля - name, familia, otchestvo, wanted_profession, work_field, worked_time, salary
// 3) Vacancia , поля - profession, grafik, salary, education_lvl
// 4) birga_vacanciy - offers, resumes, closed_vacancis

// 5. Обеспечить эффективное хранение данных и быстрый поиск информации в бюро по трудоустройству.
// Должна быть представлена информация о:
// - работодателях (название, сфера деятельности, адрес, телефон и т.п.),
// предлагаемых вакансиях:
// (наименование должности, график работы, оклад, требования по образованию, предложения и заявки работодателей и т.д.),
// - соискателях (фамилия, имя, отчество, возможные должности, сфера деятельности, стаж работы,
// ожидаемый оклад и т.д.).
// Соискатель должен получать список подходящих по его желаниям предложений от работодателей,
// работодатель должен получать список всех подходящих под его требования работников.
// Должны добавляться новые соискатели, работодатели, вакансии. В случае совпадения интересов
// работодателя и соискателя вакансия должна быть занята и перемещена в список удовлетворенных заявок.

int main(int argc, char *argv[])
{

    Birga birga;

    std::ofstream log("log.txt", ios::out);
    std::ofstream res("result.txt", ios::out|ios::trunc);

    std::string filename = "baza_dannih.txt";

    // перед началом работы надо считать базу данных если она есть (в самом начале ее нет,
    // позже будет введена ручками, а потом считываться с маркерами)
    if (std::filesystem::exists(filename))
    {
        birga.Read(filename, log);
    }

    birga.BigProcess(log,res);

    // после окончания работы надо записать базу данных
    birga.Write(filename, log);
    return 0;
}