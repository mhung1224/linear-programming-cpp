#include "ExFuncs.h"
#include <algorithm>


void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        std::cout << "\033[2J\033[1;1H";
    #endif
}

int findLength(double num) {
    if (isIntNum(num)) {
        return std::to_string(llint(std::round(num))).length();
    }

    double temp = round_4(num) - std::floor(num);
    std::string result = std::to_string(temp);
    while (result.length() > 6) {
        result.pop_back();
    }
    while(result.back() == '0') {
		result.pop_back();
	}
    return result.length() - 1 + findLength(std::floor(num));
}

void printFormattedRealNum(double num, int width, int decimals) {
    std::cout << std::fixed << std::setprecision(decimals);
    std::cout << std::setw(width) << num;
}

void printFormattedIntNum(llint num, int width) {
    std::cout << std::fixed << std::setprecision(0);
    std::cout << std::setw(width) << std::round(num);
}

void printNum(double num, int width, int decimals) {
    if (isIntNum(num)) {
        printFormattedIntNum(llint(std::round(num)), width);
        return;
    }
    printFormattedRealNum(num, width, decimals);
}

void printChars(char x, int length) {
    std::string repeatedChars(length, x);
    std::cout << repeatedChars;
}

bool isIntNum(double num) {
    double test = std::min(num - std::floor(num), std::ceil(num) - num);
    return (test < 1e-4);
}

double round_4(double num) {
	return std::round(num * 10000.0) / 10000.0;
}

int findDecimals(double num) {
    if(isIntNum(num)) {
		return 0;
	}

    return findLength(num) - findLength(std::floor(num)) - 1;
}