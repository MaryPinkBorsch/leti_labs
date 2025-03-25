#include "bwt.h"
using namespace std;

#include <algorithm>
#include <string>
#include <iterator>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// используется маркерный символ $
const char MARKER = '$';

// функция для циклического сдвига
string cyclic_shift(std::vector<char> &input, int distance)
{
	string source;
	source.insert(source.begin(), input.begin(), input.end());
	return source.substr(distance, string::npos) + source.substr(0, distance);
}

void BWT_compress(std::vector<char> &input, std::vector<char> &output)
{
	// надо добавить маркер в инпут
	input.push_back(MARKER);

	vector<string> shifts;

	for (unsigned int i = 0; i < input.size(); i++)
	{
		string shift = cyclic_shift(input, i);

		shifts.push_back(shift);
	}

	// сортируем сдвиги
	sort(shifts.begin(), shifts.end());

	// добавляем последние буквы каждого сдвига в утпут (последний столбец матрицы)

	for (vector<string>::iterator it = shifts.begin(); it != shifts.end(); it++)
	{
		output.push_back(it->at(it->length() - 1));
	}
}

bool comp_tuples(pair<char, int> a, pair<char, int> b)
{
	return (a.first < b.first);
}

void BWT_decompress(std::vector<char> &input, std::vector<char> &output)
{
	if (input.empty())
	{
		return;
	}

	int j;

	// создаем вектор пар
	vector<pair<char, int>> coding_tuples;

	for (unsigned int i = 0; i < input.size(); i++)
	{
		char coded_char = input.at(i);

		// ищем позицию маркера
		if (coded_char == MARKER)
		{
			j = i;
		}

		coding_tuples.push_back(pair<char, int>(input.at(i), i));
	}

	// сортируем
	stable_sort(coding_tuples.begin(), coding_tuples.end(), comp_tuples);

	do
	{
		output.push_back(coding_tuples.at(j).first);
		j = coding_tuples.at(j).second;
	} while (coding_tuples.at(j).first != MARKER);
}
