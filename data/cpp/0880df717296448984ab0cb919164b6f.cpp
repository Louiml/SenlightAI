// Write a C++ function that takes a positive integer `rows` and prints Pascal's triangle up to that many rows to standard output, using the iterative binomial coefficient formula `coef = coef * (i - j + 1) / j` where `i` is the current row index (0-based) and `j` is the column index (0-based). The function must print each row with numbers separated by a single space and a newline after each row. Assume the input is always a valid positive integer; no error handling is required. The function should be named `printPascalTriangle` and must use `const int` for the parameter to enforce immutability.
The core algorithm iterates row by row from 0 to `rows - 1`. For each row `i`, it iterates column `j` from 0 to `i`. The coefficient is updated in-place: at the start of each row, set `coef = 1`. For each subsequent `j`, the else branch applies `coef = coef * (i - j + 1) / j`. This computes the binomial coefficient C(i, j) iteratively without overflow for reasonable row counts. The edge case `j == 0 || i == 0` resets `coef` to 1, which naturally handles the first column of every row and the entire first row. Each value is printed with a space after it, and after completing columns for a row, a newline is printed. Time complexity is O(rows^2) because the total number of printed numbers is the sum of the first `rows` integers. Space complexity is O(1) because only a few integer variables are used.
#include <iostream>

// Print Pascal's triangle with the given number of rows.
void printPascalTriangle(const int rows) {
    for (int i = 0; i < rows; ++i) {
        int coef = 1;  // First column of each row is always 1.
        for (int j = 0; j <= i; ++j) {
            if (j == 0 || i == 0) {
                coef = 1;  // Reset for first column and first row.
            } else {
                coef = coef * (i - j + 1) / j;  // Binomial recurrence.
            }
            std::cout << coef << ' ';
        }
        std::cout << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <string>

// Test helper: capture output of printPascalTriangle and return as string.
std::string captureOutput(const int rows) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printPascalTriangle(rows);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    assert(captureOutput(1) == "1 \n");
    assert(captureOutput(2) == "1 \n1 1 \n");
    assert(captureOutput(3) == "1 \n1 1 \n1 2 1 \n");
    assert(captureOutput(4) == "1 \n1 1 \n1 2 1 \n1 3 3 1 \n");
    assert(captureOutput(5) == "1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n");
    assert(captureOutput(6) == "1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n1 5 10 10 5 1 \n");
    assert(captureOutput(7) == "1 \n1 1 \n1 2 1 \n1 3 3 1 \n1 4 6 4 1 \n1 5 10 10 5 1 \n1 6 15 20 15 6 1 \n");
    return 0;
}
