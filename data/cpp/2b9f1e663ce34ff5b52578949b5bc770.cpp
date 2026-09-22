// Write a C++ function that reads exactly 81 integers (representing a 9×9 grid, row by row) from standard input using `std::cin`, and returns a `std::pair<int,int>` where the first element is the maximum value found, and the second element is its 1-based index encoded as `(column * 10 + row)` (i.e., a two-digit number where the tens digit is the 1-based column and the units digit is the 1-based row). If multiple occurrences of the maximum exist, return the index of the first one encountered (in row‑major order, i.e., reading row by row, left to right). The function must not print anything; the caller will handle output. Use `const` correctness where applicable.
The algorithm is a simple linear scan of all 81 inputs. Initialize a maximum value variable to the smallest possible `int` (e.g., `std::numeric_limits<int>::min()`) and an index variable to `0`. For each position, read an integer, and if it is strictly greater than the current maximum, update the maximum and compute the 1‑based index as `(col+1)*10 + (row+1)` (since we loop over `col` from 0 to 8 and `row` from 0 to 8). Using strict `>` ensures that on ties the first encountered value is kept. Edge cases: all values are negative or equal; the scan handles them because the initial maximum is the smallest possible `int`. Time complexity is O(81) = O(1) with constant input size, and space complexity is O(1) beyond the input stream.
#include <limits>
#include <utility>

// Reads 81 integers from standard input and returns {maxValue, encodedIndex}
// where encodedIndex = (1-based column)*10 + (1-based row). On ties, the
// first occurrence (row-major order) is kept.
std::pair<int, int> findMaxAndPosition() {
    int maxValue = std::numeric_limits<int>::min();
    int encodedIndex = 0;

    for (int col = 0; col < 9; ++col) {
        for (int row = 0; row < 9; ++row) {
            int value;
            std::cin >> value;
            if (value > maxValue) {
                maxValue = value;
                encodedIndex = (col + 1) * 10 + (row + 1);
            }
        }
    }

    return {maxValue, encodedIndex};
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declaration of the function under test (in practice it would be in a header)
std::pair<int, int> findMaxAndPosition();

// Helper to feed a fixed input string to std::cin
void feedInput(const std::string& s) {
    static std::istringstream iss;
    iss.str(s);
    iss.clear();
    std::cin.rdbuf(iss.rdbuf());
}

int main() {
    // Test 1: Simple distinct numbers
    std::string input1;
    for (int i = 1; i <= 81; ++i) {
        input1 += std::to_string(i);
        if (i < 81) input1 += " ";
    }
    feedInput(input1);
    auto r1 = findMaxAndPosition();
    assert(r1.first == 81);
    assert(r1.second == 99); // column 9, row 9

    // Test 2: All negative numbers, maximum is -1 at first position (col1, row1)
    std::string input2;
    for (int i = 0; i < 81; ++i) {
        input2 += "-1";
        if (i < 81) input2 += " ";
    }
    feedInput(input2);
    auto r2 = findMaxAndPosition();
    assert(r2.first == -1);
    assert(r2.second == 11); // column 1, row 1

    // Test 3: Maximum appears multiple times, first occurrence is at (col2, row3) → 23
    std::string input3;
    for (int pos = 0; pos < 81; ++pos) {
        if (pos == 12) input3 += "100";      // 0-based index 12 → col=1 (2nd), row=3 (3rd)
        else if (pos == 50) input3 += "100"; // later duplicate
        else input3 += "1";
        if (pos < 80) input3 += " ";
    }
    feedInput(input3);
    auto r3 = findMaxAndPosition();
    assert(r3.first == 100);
    assert(r3.second == 23);

    // Test 4: All zeros
    std::string input4;
    for (int i = 0; i < 81; ++i) {
        input4 += "0 ";
    }
    feedInput(input4);
    auto r4 = findMaxAndPosition();
    assert(r4.first == 0);
    assert(r4.second == 11);

    // Test 5: Single large value in the middle (col5, row6) → 56
    std::string input5;
    for (int pos = 0; pos < 81; ++pos) {
        if (pos == 39) input5 += "999"; // 0-based index 39 → col=4 (5th), row=5 (6th)
        else input5 += "1";
        if (pos < 80) input5 += " ";
    }
    feedInput(input5);
    auto r5 = findMaxAndPosition();
    assert(r5.first == 999);
    assert(r5.second == 56);

    // Test 6: Negative and positive mix, max at (col1, row9) → 19
    std::string input6;
    for (int i = 0; i < 81; ++i) {
        if (i == 8) input6 += "77";       // row 9 of column 1
        else input6 += "-5";
        if (i < 80) input6 += " ";
    }
    feedInput(input6);
    auto r6 = findMaxAndPosition();
    assert(r6.first == 77);
    assert(r6.second == 19);

    // Test 7: Maximum at last row of last column (col9, row9) with large value
    std::string input7;
    for (int i = 0; i < 81; ++i) {
        if (i == 80) input7 += "1000";
        else input7 += "1";
        if (i < 80) input7 += " ";
    }
    feedInput(input7);
    auto r7 = findMaxAndPosition();
    assert(r7.first == 1000);
    assert(r7.second == 99);

    // Test 8: Two equal maxima, first at (col3, row2) → 32
    std::string input8;
    for (int i = 0; i < 81; ++i) {
        if (i == 17) input8 += "50";      // col3 (0-based 2), row2 (0-based 1) → 17
        else if (i == 50) input8 += "50"; // later duplicate
        else input8 += "3";
        if (i < 80) input8 += " ";
    }
    feedInput(input8);
    auto r8 = findMaxAndPosition();
    assert(r8.first == 50);
    assert(r8.second == 32);

    std::cout << "All tests passed.\n";
    return 0;
}
