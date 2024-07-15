#ifndef EXFUNCS_H
#define EXFUNCS_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

using llint = long long int;

// Clear the screen
void clearScreen();

// Find the length of an number rounded to 4 decimal places
int findLength(double num);

/*
    Print a real number with a specific format.
    width = Length of this number
    decimals = Number of decimal places, default is 4
*/
void printRealNumber(double num, int width, int decimals = 4);

/*
    Print an integer number with a specific format
    width = Length of this number
*/
void printIntNumber(llint num, int width);

// The combination of printRealNumber and printIntNumber
void printNumber(double num, int width, int decimals = 4);

// Print char multiple times
void printChars(char x, int length);

// Check if a "double-typed" number is an integer
bool isIntNumber(double num);

// Round a number to 4 decimal places
double round_4(double num);

// Find decimal places of a number
int findDecimals(double num);

#endif