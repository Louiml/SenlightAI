// Write a C++ function named `printPyramidRows` that takes a single positive integer `n` and prints to standard output a centered pyramid of asterisks with `n` rows (for example, `n = 3` produces rows with 1, 3, and 5 asterisks, each centered with leading spaces). The function must not print any trailing spaces after the asterisks on any row. It must handle the case `n = 0` by printing nothing. The output must match exactly the format produced by the original snippet for a single input `n` (i.e., no extra blank lines, no leading/trailing whitespace beyond the required spaces). The function should be reusable and not depend on any global state.
#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output of printPyramidRows into a string
std::string capturePyramid(int n) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    printPyramidRows(n);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    // n = 1: single asterisk
    assert(capturePyramid(1) == "*\n");
    // n = 2: two rows, first with 1 space, second with 0 spaces
    assert(capturePyramid(2) == " *\n***\n");
    // n = 3: classic pyramid
    assert(capturePyramid(3) == "  *\n ***\n*****\n");
    // n = 0: empty output
    assert(capturePyramid(0) == "");
    // n = 4: verify no trailing spaces (check last row has no spaces after asterisks)
    assert(capturePyramid(4) == "   *\n  ***\n *****\n*******\n");
    // n = 5: additional check for one more size
    assert(capturePyramid(5) == "    *\n   ***\n  *****\n *******\n*********\n");
    // Check that output has no trailing spaces on any line (split by '\n' and verify last char is '*')
    std::string result = capturePyramid(5);
    std::string line;
    std::istringstream iss(result);
    while (std::getline(iss, line)) {
        assert(!line.empty() && line.back() == '*');
    }
    return 0;
}
#include <iostream>

// Print a centered pyramid of asterisks with n rows.
// Each row i (0-based) has (n - i - 1) leading spaces and (2*i + 1) asterisks.
// Prints nothing for n = 0. No trailing spaces after asterisks.
void printPyramidRows(int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            std::cout << ' ';
        }
        for (int j = 0; j < 2 * i + 1; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
}
// The solution reproduces the exact pattern from the original code: for each row index `i` ranging from 0 to `n-1`, the number of leading spaces is `n - i - 1`, and the number of asterisks is `2*i + 1`. The original snippet had a third loop that printed nothing (an empty string), which can be omitted. The key is to output spaces for the leading part, then asterisks, then a newline—without any trailing spaces. Edge cases: when `n = 0`, the outer loop does not execute, so nothing is printed; when `n = 1`, one row with no spaces and one asterisk is printed. The time complexity is \(O(n^2)\) because the total number of characters printed is approximately \(n^2\) (sum of leading spaces and asterisks across rows). The space complexity is \(O(1)\) besides the standard output stream buffer.
