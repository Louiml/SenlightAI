Write a C++ function `std::vector<std::string> generateTrianglePattern(int rows)` that, given a positive integer `rows`, returns a vector of strings where each string represents a line of a right-angled triangle made of asterisks (`*`). The first line has 1 asterisk, the second has 2, and so on until the last line has `rows` asterisks. For example, for `rows = 5`, the function should return `{"*", "**", "***", "****", "*****"}`. If `rows` is 0 or negative, return an empty vector. The function should use only standard library facilities and must not print anything to the console.
#include <cassert>
#include <string>
#include <vector>

// Function declaration (from solution)
std::vector<std::string> generateTrianglePattern(int rows);

int main() {
    // Basic 5-row pattern
    std::vector<std::string> expected5 = {"*", "**", "***", "****", "*****"};
    assert(generateTrianglePattern(5) == expected5);

    // Single row
    std::vector<std::string> expected1 = {"*"};
    assert(generateTrianglePattern(1) == expected1);

    // Zero rows
    assert(generateTrianglePattern(0).empty());

    // Negative rows
    assert(generateTrianglePattern(-3).empty());

    // Three rows
    std::vector<std::string> expected3 = {"*", "**", "***"};
    assert(generateTrianglePattern(3) == expected3);

    // Larger row count (10) to verify sizes
    auto result10 = generateTrianglePattern(10);
    assert(result10.size() == 10);
    for (int i = 0; i < 10; ++i) {
        assert(result10[i].size() == static_cast<size_t>(i + 1));
        assert(result10[i] == std::string(i + 1, '*'));
    }
}
#include <string>
#include <vector>

// Generate a right-angled triangle pattern of asterisks with `rows` lines.
// Returns a vector of strings, each line containing i asterisks (1 <= i <= rows).
// Returns an empty vector for non-positive `rows`.
std::vector<std::string> generateTrianglePattern(int rows) {
    std::vector<std::string> result;
    if (rows <= 0) {
        return result;
    }
    result.reserve(rows);
    for (int i = 1; i <= rows; ++i) {
        result.push_back(std::string(i, '*'));
    }
    return result;
}
// The solution is straightforward: use an outer loop from 1 to `rows` (inclusive). For each iteration `i`, construct a string containing exactly `i` asterisks. This can be done using `std::string(i, '*')`, which creates a string of length `i` filled with the character `'*'`. Push this string into a vector. Edge cases: if `rows` is less than or equal to 0, there are no lines to produce, so return an empty vector. The time complexity is O(rows²) because constructing each string of length `i` takes O(i) time, and the sum 1+2+...+rows = rows(rows+1)/2. The auxiliary space is O(rows²) for storing the output vector (dominated by the total number of asterisks), but the extra temporary space per line is O(rows).
