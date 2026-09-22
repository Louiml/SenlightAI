/*
Write a C++ function named `printNumberDiamond` that takes a positive integer `n` as input and prints a diamond-shaped number pattern to the console. The top half of the diamond has `n` rows: for row `i` (1-indexed from the top), print exactly `n-i` leading spaces followed by the numbers `1` through `i`, each followed by a single space. The bottom half is the mirror image of the top half, omitting the middle row (i.e., it has `n-1` rows, starting from `n-1` down to `1`). For example, when `n = 4`, the output must be exactly:
```
   1    
  1 2   
 1 2 3  
1 2 3 4 
 1 2 3  
  1 2   
   1    
```
Note that after the last number in each row, there is exactly one trailing space, and each row is terminated by a newline. The function must not print any extra leading or trailing spaces beyond those specified. The function should return `void` and must handle edge cases such as `n = 1` (which prints only the single row `"1 "` followed by a newline) and large `n` (e.g., up to 1000) without any performance issues.
*/
#include <iostream>

// Print a diamond-shaped number pattern of size n.
// The top half has rows 1..n; the bottom half mirrors rows n-1..1.
void printNumberDiamond(int n) {
    // Upper half: rows 1 to n
    for (int i = 1; i <= n; ++i) {
        // Print leading spaces: n - i spaces
        for (int j = 0; j < n - i; ++j) {
            std::cout << ' ';
        }
        // Print numbers 1 through i, each followed by a space
        for (int k = 1; k <= i; ++k) {
            std::cout << k << ' ';
        }
        std::cout << '\n';
    }

    // Lower half: rows n-1 down to 1
    for (int i = n - 1; i >= 1; --i) {
        // Print leading spaces: n - i spaces
        for (int j = 0; j < n - i; ++j) {
            std::cout << ' ';
        }
        // Print numbers 1 through i, each followed by a space
        for (int k = 1; k <= i; ++k) {
            std::cout << k << ' ';
        }
        std::cout << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <string>

// Declaration of the function under test
void printNumberDiamond(int n);

// Helper to capture output of the function
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printNumberDiamond(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test n = 1 (single row)
    assert(captureOutput(1) == "1 \n");

    // Test n = 2
    assert(captureOutput(2) == "  1 \n1 2 \n 1 \n");

    // Test n = 3
    assert(captureOutput(3) == "   1 \n  1 2 \n1 2 3 \n  1 2 \n   1 \n");

    // Test n = 4 (as specified in the task)
    assert(captureOutput(4) == "    1 \n   1 2 \n  1 2 3 \n1 2 3 4 \n  1 2 3 \n   1 2 \n    1 \n");

    // Test a larger n = 5 (just ensure no exception and expected row count)
    std::string out = captureOutput(5);
    int newlines = 0;
    for (char c : out) if (c == '\n') ++newlines;
    assert(newlines == 9); // 5 upper + 4 lower rows

    return 0;
}
// The solution builds the diamond in two phases: an upper phase where the row number increases from 1 to `n`, and a lower phase where it decreases from `n-1` down to 1. For each row, first print `n - i` spaces (using a loop from 0 to `n-i-1`), then print numbers `1` through `i` using a nested loop where each number is followed by a single space. This directly matches the required output format without needing any additional spacing logic—just concatenating a space after each number ensures the trailing space is present. The edge case `n = 1` naturally works because the upper loop prints `1 ` and the lower loop runs zero times. The time complexity is proportional to the total number of characters printed: each row has at most `n` numbers plus `n-i` spaces, so the total is `O(n^2)` for the whole diamond. The space complexity is `O(1)` auxiliary, as only loop counters are used, and no dynamic data structures are needed.
