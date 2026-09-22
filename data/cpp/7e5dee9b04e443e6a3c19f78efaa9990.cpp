/*
Write a C++ function named `printCenteredTriangle` that takes a single non-negative integer `n` (the height of the triangle) and prints to standard output a right-aligned isosceles triangle made of asterisks (`*`) with exactly `n` rows. Each row `i` (0-indexed) must contain exactly `(n - i - 1)` leading spaces, followed by `(2 * i + 1)` asterisks, and then `(n - i - 1)` trailing spaces, with no extra spaces after the final asterisk of each row (i.e., the trailing spaces are omitted in the output). After each row, a newline is printed. The function must handle `n = 0` gracefully by printing nothing. The function should be reusable across multiple calls and must not read any input or print anything except the triangle itself.
*/

#include <iostream>

// Prints a centered isosceles triangle of height n (non-negative).
// Each row i has (n-i-1) leading spaces, (2*i+1) asterisks, no trailing spaces.
void printCenteredTriangle(int n) {
    for (int i = 0; i < n; ++i) {
        // Print leading spaces
        for (int j = 0; j < n - i - 1; ++j) {
            std::cout << ' ';
        }
        // Print asterisks
        for (int k = 0; k < 2 * i + 1; ++k) {
            std::cout << '*';
        }
        // Newline after each row
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Forward declaration of the function to test
void printCenteredTriangle(int n);

int main() {
    // Helper to capture output
    auto capture = [](int n) {
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        printCenteredTriangle(n);
        std::cout.rdbuf(old);
        return oss.str();
    };

    // Test n=0: empty output
    assert(capture(0) == "");

    // Test n=1: single asterisk
    assert(capture(1) == "*\n");

    // Test n=2: two rows
    assert(capture(2) == " *\n***\n");

    // Test n=3: three rows (classic pattern)
    assert(capture(3) == "  *\n ***\n*****\n");

    // Test n=4: four rows
    assert(capture(4) == "   *\n  ***\n *****\n*******\n");

    // Test n=5: five rows
    assert(capture(5) == "    *\n   ***\n  *****\n *******\n*********\n");

    // Test n=6: six rows
    assert(capture(6) == "     *\n    ***\n   *****\n  *******\n *********\n***********\n");

    // Test multiple calls produce correct output each time
    assert(capture(1) == "*\n");
    assert(capture(3) == "  *\n ***\n*****\n");

    return 0;
}

// The solution directly implements the pattern described: for each row index `i` from 0 to `n-1`, print spaces `n - i - 1` times, then asterisks `2*i + 1` times. The trailing spaces are unnecessary for the visual appearance (since they are invisible) and should be omitted to make the output clean and match typical triangle printing. Edge cases: if `n = 0`, the loop does not execute, so nothing is printed. For `n = 1`, the first (only) row has 0 spaces and 1 asterisk. Time complexity is O(n²) because the total work is sum over rows of (leading spaces + asterisks) = sum of (n-i-1 + 2i+1) = sum of (n + i) = n² + n(n-1)/2, which is O(n²). Space complexity is O(1) besides the loop variables.
