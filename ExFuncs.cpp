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
    if (isIntNumber(num)) {
        return std::to_string(llint(std::round(num))).length();
    } // If num is an integer, return its length
    std::string dec_part = std::to_string(round_4(num) - std::floor(num)); // Convert the decimal part of num to a string
    while (dec_part.length() > 6) {
        dec_part.pop_back();
    } // Keep the first 4 decimal places
    while(dec_part.back() == '0') {
	    dec_part.pop_back();
	} // Remove trailing zeros
    // Return the length of num if num is a real number(Include the decimal point)
    return dec_part.length() - 1 + findLength(std::floor(num)); 
}

void printRealNumber(double num, int width, int decimals) {
    // Print the number with a fixed number of some decimal places
    std::cout << std::fixed << std::setprecision(decimals); 
    std::cout << std::setw(width) << num;
}

void printIntNumber(llint num, int width) {
    // Print the number with a fixed number of 0 decimal places
    std::cout << std::fixed << std::setprecision(0);
    std::cout << std::setw(width) << std::round(num);
}

void printNumber(double num, int width, int decimals) {
    if (isIntNumber(num)) {
        printIntNumber(llint(std::round(num)), width);
        return;
    }
    printRealNumber(num, width, decimals);
}

void printChars(char x, int length) {
    std::string repeatedChars(length, x);
    std::cout << repeatedChars;
}

bool isIntNumber(double num) {
    return (std::min(num - std::floor(num), std::ceil(num) - num) < 1e-5);
}

double round_4(double num) {
	return std::round(num * 10000.0) / 10000.0;
}

int findDecimals(double num) {
    if(isIntNumber(num)) {
		return 0;
	}
    return findLength(num) - findLength(std::floor(num)) - 1;
}