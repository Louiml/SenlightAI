Write a C++ function named `mixPyramidsPattern` that takes a positive integer `n` and prints a symmetric hourglass-like pattern composed of asterisks (`*`) and spaces, exactly as produced by the provided snippet. The pattern must consist of two parts: the first part has rows with decreasing star counts from `n` down to `1` on both sides, separated by an increasing odd number of spaces (1, 3, 5, …); the second part has rows with increasing star counts from `1` up to `n` on both sides, separated by a decreasing odd number of spaces. The function must print each row to standard output, ending each row with a newline. The solution must be standalone (no `main`), include necessary headers, and handle the case where `n` is 1 (printing a single line with one star, one space, one star).
The pattern is constructed row by row. For the first part, for each row index `i` (from 0 to `n-1`), we print `n-i` stars, then `2*i+1` spaces, then `n-i` stars. For the second part, for each row index `i` (from 0 to `n-1`), we print `i+1` stars, then `2*(n-i)-1` spaces, then `i+1` stars. The key is to use nested loops to print the correct counts for each segment. Edge cases: when `n=1`, the first part prints 1 star, 1 space, 1 star; the second part prints 1 star, 1 space, 1 star (since `2*(1-0)-1=1`), so the output is two identical lines. When `n` is large, the loops work naturally. Time complexity is `O(n^2)` because each row prints `O(n)` characters and there are `O(n)` rows. Space complexity is `O(1)` besides the loop counters.
#include <iostream>

// Prints an hourglass-like pattern of stars and spaces for a given positive integer n.
void mixPyramidsPattern(int n) {
    // First part: decreasing stars on both sides, increasing spaces in the middle.
    for (int i = 0; i < n; ++i) {
        // Left stars
        for (int j = 0; j < n - i; ++j) {
            std::cout << '*';
        }
        // Middle spaces
        for (int j = 0; j < 2 * i + 1; ++j) {
            std::cout << ' ';
        }
        // Right stars
        for (int j = 0; j < n - i; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }

    // Second part: increasing stars on both sides, decreasing spaces in the middle.
    for (int i = 0; i < n; ++i) {
        // Left stars
        for (int j = 0; j < i + 1; ++j) {
            std::cout << '*';
        }
        // Middle spaces
        for (int j = 0; j < 2 * (n - i) - 1; ++j) {
            std::cout << ' ';
        }
        // Right stars
        for (int j = 0; j < i + 1; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
}
#include <iostream>
#include <sstream>
#include <cassert>

// Declare the function (already defined in the solution).
void mixPyramidsPattern(int n);

// Helper: capture output of mixPyramidsPattern for a given n as a string.
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    mixPyramidsPattern(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test n = 1: two identical lines "* *" (star, space, star).
    assert(captureOutput(1) == "* *\n* *\n");

    // Test n = 2: expected pattern.
    std::string expected2 = "** *\n*   *\n*   *\n** *\n";
    // Actually, let's compute: For n=2:
    // Part1: i=0: 2 stars, 1 space, 2 stars => "** **"
    //        i=1: 1 star, 3 spaces, 1 star => "*   *"
    // Part2: i=0: 1 star, 3 spaces, 1 star => "*   *"
    //        i=1: 2 stars, 1 space, 2 stars => "** **"
    // So expected: "** **\n*   *\n*   *\n** **\n"
    assert(captureOutput(2) == "** **\n*   *\n*   *\n** **\n");

    // Test n = 3: check first and last lines.
    std::string out3 = captureOutput(3);
    assert(out3.substr(0, 7) == "*** ***"); // first line: 3 stars, 1 space, 3 stars
    assert(out3.substr(out3.size() - 7) == "*** ***\n"); // last line same due to symmetry? Actually last line is part2 i=2: 3 stars, 1 space, 3 stars.

    // Test that the total number of characters for n=3 is correct.
    // Each row has left stars + spaces + right stars + newline.
    // Row counts: i=0: 3+1+3+1=8; i=1: 2+3+2+1=8; i=2: 1+5+1+1=8 (part1) and same for part2 reversed.
    // So each of 6 rows has 8 chars = 48 total.
    assert(out3.size() == 48);

    // Test n=4 first line.
    std::string out4 = captureOutput(4);
    assert(out4.substr(0, 9) == "**** ****"); // 4 stars, 1 space, 4 stars

    std::cout << "All tests passed.\n";
    return 0;
}
