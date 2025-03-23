#include "bwt.h"
using namespace std;

#include <algorithm>
#include <string>
#include <iterator>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// This implementation assumes the $ as the special EOT character that does not appear in the source text.
const char MARKER = '$';

// Do a cyclic shift on the source text
// For s[0..n-1] return the concatenation of s[i+1..n-1] and s[0..i] where i is the distance
string cyclic_shift(std::vector<char> &input, int distance)
{
	string source;
	source.insert(source.begin(), input.begin(), input.end());
	return source.substr(distance, string::npos) + source.substr(0, distance);
}

void BWT_compress(std::vector<char> &input, std::vector<char> &output)
{
	// Append special EOT character
	input.push_back(MARKER);

	// Generate the n cyclic shifts and put them in a list
	vector<string> shifts;

	for (unsigned int i = 0; i < input.size(); i++)
	{
		string shift = cyclic_shift(input, i);

		shifts.push_back(shift);
	}

	// Sort the list lexicographically
	sort(shifts.begin(), shifts.end());

	// Return a string comprised of the last letter of each word in the sorted list

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

	// Put the encoded characters in tuples (c[i], i)
	vector<pair<char, int>> coding_tuples;

	for (unsigned int i = 0; i < input.size(); i++)
	{
		char coded_char = input.at(i);

		// Find the index of the special end of text character
		if (coded_char == MARKER)
		{
			j = i;
		}

		coding_tuples.push_back(pair<char, int>(input.at(i), i));
	}

	// Sort the tuples by their character preserving order (stable)
	/*
	 * This is because the order in which we encountered the same character provides us
	 * with a different index leading to a different cyclic shift and thus a different following character
	 */
	stable_sort(coding_tuples.begin(), coding_tuples.end(), comp_tuples);

	// Follow the indicies to retrieve the letters in the source text

	do
	{
		output.push_back(coding_tuples.at(j).first);
		j = coding_tuples.at(j).second;
	} while (coding_tuples.at(j).first != MARKER);
}
