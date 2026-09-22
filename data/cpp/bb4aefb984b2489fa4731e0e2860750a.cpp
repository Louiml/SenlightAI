// Write a C++ function named `generateNumberTriangle` that takes a positive integer `n` and returns a `std::string` containing a right-aligned triangular pattern of numbers. For each row `i` (starting from 1 up to `n`), the row must contain the number `i` repeated `i` times, with each repetition separated by a single space, and rows separated by a newline character (`\n`). For example, if `n = 4`, the output string should be:
// ```
// 1
// 2 2
// 3 3 3
// 4 4 4 4
// ```
// There must be no trailing spaces at the end of any row, and the final row should be followed by a newline character. The function should handle the edge case `n = 1` gracefully and return `"1\n"`. Assume input `n` is always a positive integer (no need to validate). The function must be `const`-correct and should not use any global variables.

The solution involves nested loops to build each row. For each row index `i` (from 1 to `n`), we generate a string that contains `i` occurrences of the integer `i`, separated by spaces. We can accomplish this by using a loop from `j = 1` to `j = i`, appending `std::to_string(i)` to an accumulating row string, and adding a space before each occurrence except the first. After completing the row, we append a newline character (`\n`) to the final result. Important edge cases: `n = 1` produces just `"1\n"`. Since `n` is guaranteed positive, no zero or negative handling is needed. Time complexity is `O(n^2)` because the total number of printed numbers is the sum `1+2+...+n = n(n+1)/2`. Space complexity is `O(n^2)` because we build and return a string that actually contains all those characters. The function is `const`-correct because it takes `n` by value and does not modify any external state.

#include <string>

// Generate a right-aligned triangular pattern of numbers.
// For row i (1-based), print i repeated i times, separated by spaces.
// Rows are separated by '\n'. Returns the full string.
std::string generateNumberTriangle(int n) {
    std::string result;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            if (j > 1) {
                result += ' ';
            }
            result += std::to_string(i);
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

// Prototype of the solution function
std::string generateNumberTriangle(int n);

int main() {
    // Test n=1
    assert(generateNumberTriangle(1) == "1\n");

    // Test n=2
    assert(generateNumberTriangle(2) == "1\n2 2\n");

    // Test n=3
    assert(generateNumberTriangle(3) == "1\n2 2\n3 3 3\n");

    // Test n=4
    assert(generateNumberTriangle(4) == "1\n2 2\n3 3 3\n4 4 4 4\n");

    // Test n=5 (check last line and newline)
    std::string expected5 = "1\n2 2\n3 3 3\n4 4 4 4\n5 5 5 5 5\n";
    assert(generateNumberTriangle(5) == expected5);

    // Test a larger n=10 to ensure no trailing spaces
    std::string result10 = generateNumberTriangle(10);
    assert(result10.size() > 0);
    assert(result10.back() == '\n');
    // Check row 10 contains "10 10 ... 10" (10 times) and no trailing spaces
    std::string row10 = result10.substr(result10.find("10 ") > 0 ? result10.rfind("10 ") : 0);
    // A simpler check: count occurrences of "10 " in the string
    int count = 0;
    for (std::string::size_type pos = 0; (pos = result10.find("10 ", pos)) != std::string::npos; ++pos) {
        count++;
    }
    assert(count == 9); // 9 spaces after "10" mean 10 occurrences total (last one has newline after)

    return 0;
}
