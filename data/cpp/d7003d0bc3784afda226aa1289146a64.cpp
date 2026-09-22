/*
Write a C++ function that takes a non-negative integer `n` and prints a triangular pattern of uppercase letters to the console. The pattern starts with the first `n` letters of the alphabet (A, B, C, ...) on the first line, and each subsequent line omits the last letter, until the final line contains only the letter 'A'. For example, if `n = 4`, the output should be exactly:
```
A B C D
A B C
A B
A
```
The function should handle `n = 0` by printing nothing (no lines), and `n = 1` by printing only "A". Each letter on a line should be separated by a single space, and each line should end with a newline character. The function must not print any extra trailing spaces or blank lines. The function signature should be `void printAlphabetTriangle(int n)` and it should use a loop-based approach (no recursion). The function should be `const`-correct for any internal variables that do not change.
*/

#include <iostream>

// Prints an uppercase letter triangle with n rows.
// Row i (0-indexed) contains n - i letters starting from 'A'.
void printAlphabetTriangle(const int n) {
    const char start = 'A';
    for (int row = 0; row < n; ++row) {
        const int lettersThisRow = n - row;
        for (int col = 0; col < lettersThisRow; ++col) {
            if (col > 0) {
                std::cout << ' ';
            }
            std::cout << static_cast<char>(start + col);
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to a stringstream to capture output
void testPrintAlphabetTriangle(int n, const std::string& expected) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printAlphabetTriangle(n);
    std::cout.rdbuf(oldCout);
    assert(buffer.str() == expected);
}

int main() {
    testPrintAlphabetTriangle(0, "");
    testPrintAlphabetTriangle(1, "A\n");
    testPrintAlphabetTriangle(2, "A B\nA\n");
    testPrintAlphabetTriangle(3, "A B C\nA B\nA\n");
    testPrintAlphabetTriangle(4, "A B C D\nA B C\nA B\nA\n");
    testPrintAlphabetTriangle(5, "A B C D E\nA B C D\nA B C\nA B\nA\n");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution uses two nested loops. The outer loop iterates over the number of lines, from `0` to `n-1` inclusive. For each line `i`, the number of letters printed is `n - i`. The inner loop uses a `char` variable `ch` starting at `'A'` and increments while `ch < 'A' + (n - i)`. Each letter is printed followed by a space, but to avoid trailing spaces, either print a space before each letter except the first, or print the first letter then print spaces before subsequent letters. A common clean approach is to print the first letter without a leading space, then for each subsequent letter print a space before it. However, the original snippet simply prints a space after each letter, which results in a trailing space on each line—this is acceptable in many contexts but for exactness the task specifies no trailing spaces, so the reference solution avoids trailing spaces by printing a space before each letter except the first on a line. Edge cases: `n = 0` outputs nothing; `n = 1` outputs "A\n". Time complexity is O(n^2) because the total number of letters printed is n + (n-1) + ... + 1 = n(n+1)/2. Space complexity is O(1) aside from the standard output. The function uses a constant character `start = 'A'` and an integer loop counter, both appropriately marked `const` where possible.
