Write a C++ function that takes a positive integer `n` and returns a `std::string` representing a right-angled triangular number pattern. The triangle has `n` rows, where row `i` (1-indexed) contains exactly `i` consecutive integers starting from 1 and increasing by 1 across all rows, each printed with a field width of 4 (e.g., `printf("%4d", value)` format, meaning values are right-aligned in a 4-character field, with spaces for padding). The rows must be separated by newline characters, and the last row must end with a newline. For example, for `n=3`, the output string should be:
```
   1
   2   3
   4   5   6
```
(where each number occupies 4 characters, so there are spaces before single-digit numbers). The function must be named `buildNumberTriangle` and take `int n` as input. You may assume `n >= 1`.
The core algorithm is straightforward: iterate over rows from 1 to `n`, and within each row iterate over columns from 1 to the row number, printing the next counter value (starting at 1 and incrementing after each printed number) using a fixed width of 4. Use `std::ostringstream` to build the output string, formatting each integer with `std::setw(4)` (which right-aligns the number within 4 characters, padding with spaces). After finishing each row, append a newline character. Edge cases: `n=1` should produce a single number followed by a newline; numbers grow beyond 4 digits for large `n` (e.g., for `n=100`, the last number is 5050, which is still 4 digits, but for `n=1000`, the last number is 500500 (6 digits) and `setw(4)` will simply print the full number without truncation, so no special handling needed). Time complexity is \(O(n^2)\) because the total number of printed integers is \(1+2+\dots+n = n(n+1)/2\). Auxiliary space is \(O(n^2)\) for the output string, and \(O(1)\) extra for the counter and loop variables.
#include <iomanip>
#include <sstream>
#include <string>

// Builds a right-angled triangle of consecutive integers starting from 1.
// Row i contains i integers, each right-aligned in a width-4 field.
// Returns the entire triangle as a string with newlines between rows.
std::string buildNumberTriangle(int n) {
    std::ostringstream output;
    int current = 1;
    for (int row = 1; row <= n; ++row) {
        for (int col = 1; col <= row; ++col) {
            output << std::setw(4) << current++;
        }
        output << '\n';
    }
    return output.str();
}
#include <cassert>
#include <string>

// The solution function is declared above (assume it is included here).

int main() {
    // Simple case n=1
    assert(buildNumberTriangle(1) == "   1\n");
    
    // Case n=2
    assert(buildNumberTriangle(2) == "   1\n   2   3\n");
    
    // Case n=3 (example from prompt)
    std::string expected3 = "   1\n   2   3\n   4   5   6\n";
    assert(buildNumberTriangle(3) == expected3);
    
    // Check that numbers are right-aligned with width 4
    // n=4 gives 10 numbers, last is 10, which is two digits
    std::string expected4 = "   1\n   2   3\n   4   5   6\n   7   8   9  10\n";
    assert(buildNumberTriangle(4) == expected4);
    
    // Larger n=5 to verify completely filled width for two-digit numbers
    std::string expected5 = "   1\n   2   3\n   4   5   6\n   7   8   9  10\n  11  12  13  14  15\n";
    assert(buildNumberTriangle(5) == expected5);
    
    // n=10 ensures last number is 55, a two-digit value
    // We only check the size and last few characters to keep test simple
    std::string res10 = buildNumberTriangle(10);
    assert(res10.back() == '\n');
    assert(res10.find("  55\n") != std::string::npos); // last line ends with 55
    
    // n=1000 (total numbers = 500500) – verify that the last number is correct
    std::string res1000 = buildNumberTriangle(1000);
    // Compute expected last number: n*(n+1)/2 = 1000*1001/2 = 500500
    // We check that the last non-newline portion contains "500500"
    // Since width may be larger than 4 for 6-digit numbers, setw(4) just prints number as-is
    // Find last occurrence of "500500" before final newline
    size_t pos = res1000.rfind("500500");
    assert(pos != std::string::npos);
    // Ensure it's right before the final newline
    assert(pos + 6 == res1000.size() - 1);
    
    return 0;
}
