// Write a C++ function `std::vector<std::string> printStarPatterns(int t, const std::vector<int>& linesPerCase)` that, given a number of test cases `t` and a vector of positive integers (one per test case, each representing the number of lines to print), returns a vector of strings. For each test case, the corresponding string must be the right-angled triangle star pattern with `n` lines, where the `i`-th line (0-indexed) contains exactly `i+1` asterisks separated by spaces, and consecutive lines are separated by newline characters. The function must handle `t == 0` gracefully (return an empty vector). It must also reject invalid inputs by returning an empty vector if `linesPerCase` has a different size than `t`, or if any line count is not a positive integer. The pattern should have no trailing space at the end of any line, and no extra newline at the very end of the string for each test case.
#include <cassert>
#include <string>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Single test case with 3 lines
    {
        std::vector<int> input = {3};
        auto patterns = printStarPatterns(1, input);
        assert(patterns.size() == 1);
        assert(patterns[0] == "*\n* *\n* * *");
    }
    // Multiple test cases
    {
        std::vector<int> input = {1, 2, 4};
        auto patterns = printStarPatterns(3, input);
        assert(patterns.size() == 3);
        assert(patterns[0] == "*");
        assert(patterns[1] == "*\n* *");
        assert(patterns[2] == "*\n* *\n* * *\n* * * *");
    }
    // Zero test cases
    {
        std::vector<int> input;
        auto patterns = printStarPatterns(0, input);
        assert(patterns.empty());
    }
    // Mismatched size
    {
        std::vector<int> input = {1, 2};
        auto patterns = printStarPatterns(1, input);
        assert(patterns.empty());
    }
    // Invalid line count (zero or negative)
    {
        std::vector<int> input = {0};
        auto patterns = printStarPatterns(1, input);
        assert(patterns.empty());
    }
    {
        std::vector<int> input = {-2};
        auto patterns = printStarPatterns(1, input);
        assert(patterns.empty());
    }
    // Single line per test case
    {
        std::vector<int> input = {1, 1};
        auto patterns = printStarPatterns(2, input);
        assert(patterns.size() == 2);
        assert(patterns[0] == "*");
        assert(patterns[1] == "*");
    }
    // Larger pattern to verify no trailing space and no extra newline
    {
        std::vector<int> input = {5};
        auto patterns = printStarPatterns(1, input);
        assert(patterns.size() == 1);
        assert(patterns[0] == "*\n* *\n* * *\n* * * *\n* * * * *");
        // Verify last character is not newline
        assert(patterns[0].back() == '*');
    }
    return 0;
}
#include <string>
#include <vector>

// Build a right-angled triangle star pattern with n lines.
// Each line i (0-indexed) has i+1 stars separated by spaces.
// Lines are separated by newline, no trailing space or extra newline at end.
std::string buildTriangle(int n) {
    std::string pattern;
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col <= row; ++col) {
            pattern += "*";
            if (col < row) {
                pattern += " ";
            }
        }
        if (row < n - 1) {
            pattern += "\n";
        }
    }
    return pattern;
}

// Returns a vector of star patterns for each test case.
// If inputs are invalid (t != linesPerCase.size() or any n <= 0), returns empty vector.
std::vector<std::string> printStarPatterns(int t, const std::vector<int>& linesPerCase) {
    std::vector<std::string> result;
    if (t < 0 || linesPerCase.size() != static_cast<size_t>(t)) {
        return result;
    }
    for (int i = 0; i < t; ++i) {
        int n = linesPerCase[i];
        if (n <= 0) {
            return std::vector<std::string>();
        }
        result.push_back(buildTriangle(n));
    }
    return result;
}
// The core idea is to iterate over each test case, and for each, build a string by constructing `n` lines. For each row index `row` (from 0 to `n-1`), we append `row+1` asterisks separated by a single space, then append a newline if it's not the last row. A nested loop generates the stars: for each column from 0 to `row` inclusive, output "* " but for the last column omit the trailing space (or simply build each line as a separate string and then join them). Important edge cases: `t` can be 0 (return empty vector). The size of `linesPerCase` must match `t`, and every `n` in it must be > 0; otherwise, return an empty vector. Since each test case produces an independent pattern, we can process sequentially. Time complexity: For a given `n`, the number of stars printed is `1+2+...+n = n(n+1)/2`, so total across all test cases is `O(total n^2)`. Space complexity: `O(n^2)` for the longest output string, plus the vector of strings itself.
