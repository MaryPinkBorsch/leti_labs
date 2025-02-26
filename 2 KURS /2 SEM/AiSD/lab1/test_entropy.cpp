#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>

using namespace std;
// # второй варик
// def prob_estimate2(S):
//     total_symbols = len(S)
//     symbol_counts = {}
//     for symbol in S:
//         if symbol in symbol_counts:
//             symbol_counts[symbol] += 1
//         else:
//             symbol_counts[symbol] = 1
//     probabilities = numpy.array(list(symbol_counts.values())) / total_symbols
//     return probabilities

// предполагается что пробс пустые
void prob_estimate(string &s, unordered_map<char, double> &probs)
{
    if (!probs.empty())
        probs.clear();
    int len = s.size();
    if (len == 0)
        return;
    unordered_map<char, double> map;
    for (int i = 0; i < len; i++)
    {
        map[s[i]]++;
    }
    for (auto &it : map)
    {
        double tmp = it.second / len;
        probs[it.first] = tmp;
    }
}

// # код с учетом длины
// def entropy(S,code_len):
//     P= prob_estimate2(S)
//     P=numpy.array(list(filter(lambda x: x!=0,P)))
//     H=-sum(numpy.log2(P)* P*code_len)
//     return H

double entropy(string &s, int code_len)
{
    unordered_map<char, double> probs;
    prob_estimate(s, probs);
    if (probs.empty())
    {
        cout << "ПУстые вероятности!" << endl;
        return 0;
    }
    double H;
    for (auto &it : probs)
    {
        H += (code_len * it.second * log2(it.second));
    }
    H *= -1;
    return H;
}

int main(int argc, char *argv[])
{
    {
        string tmp = "aaabbb";
        unordered_map<char, double> probs;
        prob_estimate(tmp, probs);

        for (auto &it : probs)
        {
            cout << "Symbol " << it.first << " probability =  " << it.second << endl;
        }

        auto h = entropy(tmp, 1);
        cout << "ENTROPY = " << h<< endl;
    }
    cout << endl
         << endl;
    {
        string tmp = "aaaaaaaaaaaaaaaaaaaaab";
        unordered_map<char, double> probs;
        prob_estimate(tmp, probs);

        for (auto &it : probs)
        {
            cout << "Symbol " << it.first << " probability =  " << it.second << endl;
        }

        auto h = entropy(tmp, 1);
        cout << "ENTROPY = " << h<< endl;

        h = entropy(tmp, 2);
        cout << "ENTROPY (len 2) = " << h<< endl;
    }
}