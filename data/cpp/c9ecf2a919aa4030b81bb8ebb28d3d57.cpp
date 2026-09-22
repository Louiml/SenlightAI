// Write a standalone C++ function named `printPatterns` that takes a single integer argument `n` (assumed to be positive, 1 ≤ n ≤ 20) and prints exactly five pattern blocks, each separated by a blank line, in this strict order: (1) a solid `n×n` square of `*`, (2) a right-angled triangle of `*` with `n` rows where row `i` has `i` stars, (3) a number triangle with `n` rows where row `i` prints the integers `1` through `i` without spaces, (4) a right-aligned triangle of `*` with `n` rows (each row’s stars are right-justified using spaces), and (5) a symmetric pyramid of `*` with `n` rows (row `i` has `2*i-1` stars, centered with spaces). The function must return `void` and print to `std::cout` directly. The pattern must be exactly as formatted—no leading/trailing spaces on any line except where necessary for right-alignment or centering, and no extra blank lines at the very end of the output (i.e., the last line of the pyramid is followed by a newline but no additional blank line). The function must be const-correct: if any local variable is not modified, declare it `const`. Do not include `main` in the solution; the test code will contain `main`.

The solution uses nested loops for each of the five patterns. For the square, two loops run from 1 to `n` printing `*` without spaces, then a newline. For the triangle, the outer loop runs `i` from 1 to `n`, and the inner loop runs `j` from 1 to `i` printing `*`. For the number triangle, the inner loop prints `j` (the loop variable) instead of a star, using `cout << j` to concatenate numbers without spaces. For the right-aligned triangle, each row first prints `n - i` spaces (using a loop from 1 to `n-i`), then `i` stars. For the pyramid, each row prints `n - i` spaces, then `2*i-1` stars. After each pattern block, print a blank line (i.e., `cout << endl;` after the pattern’s last line) except after the final pyramid, where we must not print an extra blank line. The simplest way is to print the blank line before starting a new pattern (except before the first pattern). Edge cases: when `n` is 1, each pattern has one row, and spaces loops run zero times—that works naturally. Complexity: each pattern’s loops sum to about O(n²) operations, so the entire function runs in O(n²) time and O(1) auxiliary space (only loop counters and the `const int n` parameter). Input constraints ensure `n` fits in `int`.

#include <iostream>

// Print five pattern blocks (square, triangle, number triangle, right-aligned triangle, pyramid)
// of size n, each separated by a blank line. Assumes n >= 1.
void printPatterns(const int n) {
    // Pattern 1: Solid square of stars
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
    std::cout << '\n'; // blank line between blocks

    // Pattern 2: Right-angled triangle of stars
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Pattern 3: Number triangle
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << j; // no space between digits
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Pattern 4: Right-aligned triangle
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i; ++j) {
            std::cout << ' ';
        }
        for (int k = 1; k <= i; ++k) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Pattern 5: Pyramid (last block, no trailing blank line)
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i; ++j) {
            std::cout << ' ';
        }
        for (int k = 1; k <= 2 * i - 1; ++k) {
            std::cout << '*';
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Declare the function to be tested (since it's not provided in this file).
void printPatterns(const int n);

int main() {
    // Helper to capture stdout from a function call.
    auto capture = [](int n) {
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        printPatterns(n);
        std::cout.rdbuf(old);
        return oss.str();
    };

    // n = 1: each pattern is a single row.
    assert(capture(1) == 
        "*\n\n*\n\n1\n\n*\n\n*\n");

    // n = 2: check exact output for all five blocks.
    assert(capture(2) ==
        "**\n**\n\n"
        "*\n**\n\n"
        "1\n12\n\n"
        " *\n**\n\n"
        " *\n***\n");

    // n = 3: verify row counts and alignment.
    std::string out3 = capture(3);
    assert(out3.find("***\n***\n***\n\n") == 0); // square
    assert(out3.find("*\n**\n***\n\n") != std::string::npos); // triangle
    assert(out3.find("1\n12\n123\n\n") != std::string::npos); // number triangle
    assert(out3.find("  *\n **\n***\n\n") != std::string::npos); // right-aligned
    assert(out3.find("  *\n ***\n*****\n") != std::string::npos); // pyramid at end

    // Verify that the total output length for n=3 is correct:
    // square: 3 rows of 3 stars = 9 chars + 3 newlines = 12, plus blank line = 1 -> 13
    // triangle: 1+2+3 stars =6 chars + 3 newlines = 9, plus blank = 1 -> 10
    // number: 1+2+3 digits =6 chars + 3 newlines = 9, plus blank = 1 -> 10
    // right-aligned: spaces+stars: row1:2sp+1*+nl=4, row2:1sp+2*+nl=4, row3:0sp+3*+nl=4 =12, plus blank=1 ->13
    // pyramid: row1:2sp+1*+nl=4, row2:1sp+3*+nl=5, row3:0sp+5*+nl=6 =15, no blank.
    // Total = 13+10+10+13+15 = 61.
    assert(out3.size() == 61);

    // Ensure the last character is a newline (not a blank line after pyramid).
    assert(out3.back() == '\n');
    // Ensure there are exactly 5 blank lines (one between each block, none after last).
    int blankLines = 0;
    for (size_t i = 0; i + 1 < out3.size(); ++i) {
        if (out3[i] == '\n' && out3[i+1] == '\n') blankLines++;
    }
    assert(blankLines == 4); // between block1-2, 2-3, 3-4, 4-5

    return 0;
}
