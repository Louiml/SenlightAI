Write a C++ function named `printRightTriangle` that takes a non-negative integer `n` as input and returns a `std::string` containing a right-angled triangle of asterisks, where the first row has 1 asterisk, the second row has 2 asterisks, and so on up to the `n`-th row which has `n` asterisks. Each row must be terminated by a newline character (`\n`). If `n` is 0, return an empty string. The function must not perform any console output; it must build and return the string so it can be tested. Assume `n` fits within the range of `int`, but handle edge cases such as 0 and very large values gracefully where possible (though for very large `n`, memory constraints may apply; that's acceptable as long as the logic is correct). The function signature should be `std::string printRightTriangle(int n)`.

#include <cassert>
#include <string>

// Declaration of the solution function (assume it's in the same file or included).
std::string printRightTriangle(int n);

int main() {
    // Test n = 0: empty string.
    assert(printRightTriangle(0) == "");

    // Test n = 1: single row with one asterisk.
    assert(printRightTriangle(1) == "*\n");

    // Test n = 2.
    assert(printRightTriangle(2) == "*\n**\n");

    // Test n = 3.
    assert(printRightTriangle(3) == "*\n**\n***\n");

    // Test n = 5, check the pattern length and specific characters.
    std::string pattern5 = printRightTriangle(5);
    assert(pattern5.size() == 15); // 1+2+3+4+5 = 15 asterisks + 5 newlines = 20? Wait: 15+5=20, correction.
    // Actually size: 1+2+3+4+5=15 asterisks + 5 newlines = 20.
    assert(pattern5.size() == 20);
    assert(pattern5 == "*\n**\n***\n****\n*****\n");

    // Test with a larger n (e.g., 10) by verifying total length and row count.
    std::string pattern10 = printRightTriangle(10);
    int newlineCount = 0;
    for (char c : pattern10) {
        if (c == '\n') newlineCount++;
    }
    assert(newlineCount == 10);
    // Total characters: 10*11/2 = 55 asterisks + 10 newlines = 65.
    assert(pattern10.size() == 65);

    return 0;
}

#include <string>

// Builds a right-aligned triangle of asterisks with n rows.
// Returns the pattern as a string, where each row is terminated by '\n'.
// If n is 0, returns an empty string.
std::string printRightTriangle(int n) {
    // Clamp negative values to 0 to produce an empty string.
    if (n <= 0) {
        return "";
    }

    std::string result;
    // Reserve approximate space to reduce reallocations: n(n+1)/2 + n newlines.
    result.reserve(static_cast<size_t>(n) * (n + 1) / 2 + static_cast<size_t>(n));

    for (int row = 1; row <= n; ++row) {
        // Append row asterisks efficiently.
        result.append(static_cast<size_t>(row), '*');
        result.push_back('\n');
    }

    return result;
}

// The task requires constructing a string that represents a right triangle pattern of asterisks. The main algorithm iterates from row `i = 1` to `i = n`, and for each row, appends `i` asterisks followed by a newline character to a result string. This is a straightforward double-loop approach: an outer loop for rows and an inner loop (or use `std::string` constructor with a fill character) to append the required number of `'*'` characters. The edge case `n = 0` simply returns an empty string because no rows are generated. For `n = 1`, the result is just `"*\n"`. The time complexity is `O(n^2)` because the total number of asterisks is `1 + 2 + ... + n = n(n+1)/2`, and each character append takes constant time. The space complexity is `O(n^2)` as well, because the output string itself stores all characters. No special edge cases beyond zero are needed, but we should ensure the function works correctly for negative inputs—since the problem states non-negative integer, we can either treat negative as 0 or rely on the caller; a robust implementation could clamp negative to 0 to avoid infinite loops.
