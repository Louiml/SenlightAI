/*
Write a C++ function `printBinaryPattern` that takes a positive integer `n` and prints, for each row `i` from 1 to `n`, exactly `i` integers separated by spaces in a binary alternating pattern: each row starts with `1` if the row number is odd, and with `0` if the row number is even; subsequent numbers in the same row alternate between `0` and `1`. Each row must be printed on a new line, immediately followed by a newline character (no trailing spaces). The function should print directly to standard output and return nothing (void). The input `n` is guaranteed to be at least 1, but the function should still handle any positive integer correctly. This is a pure output problem; no return value is expected.
*/

#include <iostream>

// Print a binary triangular pattern for the given number of rows.
// Row i (1-based) contains i numbers, starting with 1 for odd rows and 0 for even rows,
// alternating between 0 and 1 thereafter.
void printBinaryPattern(int n) {
    for (int row = 1; row <= n; ++row) {
        int current = (row % 2 == 1) ? 1 : 0;  // starting value for this row
        for (int col = 1; col <= row; ++col) {
            if (col > 1) {
                std::cout << " ";
            }
            std::cout << current;
            current = 1 - current;  // toggle between 0 and 1
        }
        std::cout << "\n";
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Helper to capture output of printBinaryPattern into a string
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printBinaryPattern(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    assert(captureOutput(1) == "1\n");
    assert(captureOutput(2) == "1\n0 1\n");
    assert(captureOutput(3) == "1\n0 1\n1 0 1\n");
    assert(captureOutput(4) == "1\n0 1\n1 0 1\n0 1 0 1\n");
    assert(captureOutput(5) == "1\n0 1\n1 0 1\n0 1 0 1\n1 0 1 0 1\n");
    // Edge case: a slightly larger n to verify pattern consistency
    std::string result6 = captureOutput(6);
    std::string expected6 = "1\n0 1\n1 0 1\n0 1 0 1\n1 0 1 0 1\n0 1 0 1 0 1\n";
    assert(result6 == expected6);
    std::cout << "All tests passed!\n";
    return 0;
}

// The main algorithm is straightforward: iterate over each row from 1 to `n`. For each row, determine the starting value: if the row index `i` is odd, start with 1; if even, start with 0. Then, for the inner loop of length `i`, print the current value followed by a space (except after the last element, where we print a newline). After printing each value, toggle the current value between 0 and 1 using `current = 1 - current`. Edge cases: `n = 1` prints just "1\n". There is no trailing space after the last number in a row—this is ensured by printing each number except the last with a trailing space, and the last number with a newline. Time complexity is O(n^2) because the total number of printed integers is the sum 1+2+...+n = n(n+1)/2. Space complexity is O(1) since only a few integer variables are used. The solution uses a simple loop structure and no additional data structures.
