Write a C++ function that takes a positive odd integer `n` and prints a symmetrical hollow diamond (hourglass-like) pattern to standard output. The pattern consists of two halves: an upper triangle where each row has one leading `*`, followed by an inner hollow space, and a closing `*` (except on the first row); and a lower inverted triangle mirrored vertically. The number of rows in each half is `(n+1)/2`, and the total height is `n`. The function must validate that `n` is odd and greater than or equal to 1; if not, it should print nothing. The output must match exactly the structure produced by the given snippet, including spaces before the first asterisk and between the two asterisks when present.

// The pattern is built by controlling spaces and asterisks per row. For the upper half with row index `i` starting at 0, the number of leading spaces is `n/2 - i`, the first `*` is always printed, then the inner gap has `2*i - 1` spaces (only when `i>0`), and a second `*` is printed if `i>0`. For the lower half, row index `i` decreases from `n/2 - 1` down to 0, using the same formulas. This creates a symmetric shape. Edge cases: when `n=1`, the upper half prints one `*` with no inner spaces, and the lower half loop does not run because `n/2-1 = 0` and condition `i>=0` is false when initialized to -1 (using `int i = h-1; i>=0; i--` with `h=0` gives `i=-1` which fails). For invalid input (even or zero), the function should do nothing. Time complexity is `O(n^2)` because for each row, we print leading spaces and inner spaces proportional to `n`, and there are `n` rows. Space complexity is `O(1)` since only a few integer variables are used, and output is streamed directly.

#include <iostream>

// Prints a hollow diamond pattern of height n (n must be odd and >= 1).
// If n is not positive odd, prints nothing.
void printHollowDiamond(int n) {
    if (n <= 0 || n % 2 == 0) {
        return;
    }

    const int half = n / 2;

    // Upper half (includes middle row)
    for (int i = 0; i <= half; ++i) {
        // Leading spaces
        for (int j = 1; j <= half - i; ++j) {
            std::cout << ' ';
        }
        std::cout << '*';
        // Inner spaces (only for rows after the first)
        if (i > 0) {
            for (int j = 1; j <= 2 * i - 1; ++j) {
                std::cout << ' ';
            }
            std::cout << '*';
        }
        std::cout << '\n';
    }

    // Lower half (inverted, excluding the middle row)
    for (int i = half - 1; i >= 0; --i) {
        // Leading spaces
        for (int j = 1; j <= half - i; ++j) {
            std::cout << ' ';
        }
        std::cout << '*';
        // Inner spaces (only for rows after the first in this half)
        if (i > 0) {
            for (int j = 1; j <= 2 * i - 1; ++j) {
                std::cout << ' ';
            }
            std::cout << '*';
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Placeholder declaration to link with the solution function.
void printHollowDiamond(int n);

// Helper to capture output of the function for a given n.
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printHollowDiamond(n);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // n=1: single asterisk.
    assert(captureOutput(1) == "*\n");

    // n=3: two rows upper, one row lower (actually half=1, upper rows 0,1; lower i=0)
    assert(captureOutput(3) == " * \n* *\n * \n");

    // n=5: expected pattern with proper spaces.
    std::string expected5 = "  *  \n * * \n*   *\n * * \n  *  \n";
    assert(captureOutput(5) == expected5);

    // n=7: simple check that output length equals n*(n+1) including newlines? We just verify first line.
    std::string out7 = captureOutput(7);
    assert(out7.substr(0, 5) == "   * "); // three spaces, star, space (then newline)
    assert(out7.size() == 56); // 7 lines each 7 chars + newline = 8 per line, 56 total

    // Invalid inputs print nothing.
    assert(captureOutput(0) == "");
    assert(captureOutput(2) == "");
    assert(captureOutput(-3) == "");
    assert(captureOutput(8) == "");

    // n=7 full correctness can be checked by reconstructing manually.
    std::string expected7 = 
        "   *   \n"
        "  * *  \n"
        " *   * \n"
        "*     *\n"
        " *   * \n"
        "  * *  \n"
        "   *   \n";
    assert(out7 == expected7);

    std::cout << "All tests passed.\n";
    return 0;
}
