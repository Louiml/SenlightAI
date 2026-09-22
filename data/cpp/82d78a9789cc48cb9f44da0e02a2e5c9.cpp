Write a standalone C++ function named `printDescendingHashPattern` that takes a single integer parameter `rows`. The function must print to standard output a right-aligned triangular pattern of `#` symbols, where the first line contains exactly `rows` `#` characters, and each subsequent line contains one fewer `#`, until the final line contains exactly one `#`. Each line must be terminated by a newline. The function should handle the edge case where `rows` is less than or equal to 0 by printing nothing (no output at all). Assume the input is a non-negative integer within normal int range. The function must not return any value (void), and must use only standard C++ iostream facilities.

The solution uses two nested loops: an outer loop that iterates from `rows` down to 1, and an inner loop that prints exactly the current row’s count of `#` characters. The outer loop decrements the counter by 1 each iteration, so the number of `#` printed decreases by 1 per row. After each inner loop finishes, the function outputs a newline using `std::cout << '\n'`. The edge case for `rows <= 0` is handled by the outer loop condition `for (int i = rows; i >= 1; --i)`, which simply does not execute when `rows` is 0 or negative, resulting in no output. The algorithm runs in \(O(n^2)\) time because the total number of characters printed is the sum of integers from 1 to `rows`, which equals \(rows(rows+1)/2\). The space complexity is \(O(1)\) auxiliary, as only a few integer variables are used, and no additional data structures are required.

#include <iostream>

// Prints a right-aligned descending pattern of '#' characters.
// For rows > 0, prints 'rows' hashes on the first line, then rows-1, ..., 1.
// For rows <= 0, prints nothing.
void printDescendingHashPattern(const int rows) {
    for (int i = rows; i >= 1; --i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << '#';
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function under test.
void printDescendingHashPattern(const int rows);

// Helper to capture output into a string.
std::string captureOutput(int rows) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printDescendingHashPattern(rows);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test normal case.
    assert(captureOutput(3) == "###\n##\n#\n");
    // Test rows = 1.
    assert(captureOutput(1) == "#\n");
    // Test rows = 0 (should output nothing).
    assert(captureOutput(0) == "");
    // Test negative rows (should output nothing).
    assert(captureOutput(-5) == "");
    // Test a larger pattern.
    assert(captureOutput(4) == "####\n###\n##\n#\n");
    // Test rows = 8 (original snippet).
    assert(captureOutput(8) == "########\n#######\n######\n#####\n####\n###\n##\n#\n");
    // Test rows = 2.
    assert(captureOutput(2) == "##\n#\n");
    std::cout << "All tests passed.\n";
    return 0;
}
