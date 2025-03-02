#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;
vector<double> prob_estimate(const string& S, int len_code) {
    int total_symbols = S.length();
    if (total_symbols % len_code != 0) {
        cout << "Не подходит для такого деления" << endl;
        return {};
    }

    unordered_map<string, int> symbol_counts;
    for (size_t i = 0; i < S.length(); i += len_code) {
        string symbol = S.substr(i, len_code);
        symbol_counts[symbol]++;
    }

    vector<double> probabilities;
    for (const auto& pair : symbol_counts) {
        probabilities.push_back(static_cast<double>(pair.second) / total_symbols);
    }

    return probabilities;
}

double entropy(const string& S, int code_len) {
    if (S.length() % code_len != 0) {
        cout << "Не подходит для такого деления, len = " << code_len << endl;
        cout << S.length() << endl;
        return 0.0;
    }

    vector<double> P = prob_estimate(S, code_len);
    if (P.empty()) {
        cout << "error" << endl;
        return 0.0;
    }

    // Удаляем нулевые вероятности
    P.erase(remove_if(P.begin(), P.end(), [](double x) { return x == 0; }), P.end());

    double H = 0.0;
    for (double p : P) {
        H -= p * log2(p);
    }

    return H * (S.length() / code_len);
}

int main() {
    string text = "Hello, world! This is a test message to calculate probabilities.";
    int code_len = 4; // Пример длины кода символа

    double entropy_value = entropy(text, code_len);
    cout << "Энтропия: " << entropy_value << endl;

    return 0;
}