Write a C++ function named `generateNumberStarTriangle` that takes a positive integer `n` as input and returns a string containing `n` lines. For each line `i` (1-indexed), the line must contain the digits `1` through `i` consecutively (without spaces), followed by exactly `(n - i)` asterisks (`*`), and end with a newline character (`\n`). The last line should also end with `\n`. If `n` is less than or equal to 0, return an empty string. For example, for `n = 3`, the returned string should be `"1**\n12*\n123\n"`. The function must be self-contained and not rely on global variables.
The solution mirrors the provided snippet but refactors the logic into a reusable function that builds a result string rather than printing directly. The main algorithm iterates `i` from 1 to `n`. For each iteration, it appends digits `1` through `i` to the string, then appends `(n - i)` asterisks, and finally appends a newline. Edge cases include `n = 1` (produces `"1\n"`), `n = 0` or negative (should return empty string), and large `n` (the string size grows quadratically; no overflow issues for reasonable `n`). Time complexity is \(O(n^2)\) because the total number of characters is \(n(n+1)/2 + n\) (digits and stars and newlines). Space complexity is \(O(n^2)\) due to the returned string. The implementation uses `std::string` and `std::to_string` for digit conversion, and applies `const` to the parameter and inner loop variables where appropriate.
#include <string>

// Generate a triangle of numbers followed by stars based on the given size.
// For each line i (1-indexed), write digits 1..i, then (n-i) stars, then newline.
std::string generateNumberStarTriangle(const int n) {
    if (n <= 0) {
        return "";
    }

    std::string result;
    result.reserve(static_cast<size_t>(n) * (n + 1) / 2 + n);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            result += std::to_string(j);
        }
        for (int j = 0; j < (n - i); ++j) {
            result += '*';
        }
        result += '\n';
    }
    return result;
}
#include <cassert>
#include <string>

// Function declaration (included for test compilation)
std::string generateNumberStarTriangle(const int n);

int main() {
    // Test basic cases
    assert(generateNumberStarTriangle(1) == "1\n");
    assert(generateNumberStarTriangle(2) == "1*\n12\n");
    assert(generateNumberStarTriangle(3) == "1**\n12*\n123\n");
    assert(generateNumberStarTriangle(4) == "1***\n12**\n123*\n1234\n");

    // Test non-positive edge cases
    assert(generateNumberStarTriangle(0) == "");
    assert(generateNumberStarTriangle(-3) == "");

    // Test larger n to ensure correct formatting and no leading/trailing spaces
    std::string expected5 = "1****\n12***\n123**\n1234*\n12345\n";
    assert(generateNumberStarTriangle(5) == expected5);

    // Test that newlines are exactly one per line and no extra spaces
    std::string result10 = generateNumberStarTriangle(10);
    size_t newlineCount = 0;
    for (char c : result10) {
        if (c == '\n') ++newlineCount;
    }
    assert(newlineCount == 10);
    // Ensure first line starts with '1' and last line is "12345678910\n"
    assert(result10[0] == '1');
    assert(result10.substr(result10.size() - 13) == "12345678910\n");

    return 0;
}
