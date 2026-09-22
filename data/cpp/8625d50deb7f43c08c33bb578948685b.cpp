/*
Write a C++ function `std::vector<std::string> generateRightAlignedTriangle(int n)` that, given a positive integer `n`, returns a vector of strings representing a right-aligned triangle of asterisks. The triangle has `n` rows. The first row contains 1 asterisk at the far right (i.e., preceded by `n-1` spaces), the second row contains 2 asterisks preceded by `n-2` spaces, and so on, with the `i`-th row (1-indexed) containing `i` asterisks preceded by `n-i` spaces. The last row (row `n`) contains `n` asterisks with no leading spaces. Each row is a string of length exactly `n`, consisting of spaces followed by asterisks. The function must handle edge cases, including `n = 1` (returns a single row with one asterisk) and `n` being zero or negative (in which case the function should return an empty vector). The output should contain no trailing spaces after the asterisks. The function must be `const`-correct where applicable and use descriptive variable names.
*/
#include <string>
#include <vector>

// Generate a right-aligned triangle of asterisks with n rows.
// Each row i (1-indexed) has i asterisks preceded by (n-i) spaces.
// Returns an empty vector if n <= 0.
std::vector<std::string> generateRightAlignedTriangle(const int n) {
    std::vector<std::string> result;
    if (n <= 0) {
        return result;
    }
    result.reserve(static_cast<size_t>(n));
    for (int i = 1; i <= n; ++i) {
        const int spaces = n - i;
        std::string row;
        row.append(spaces, ' ');
        row.append(i, '*');
        result.push_back(row);
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// Include the solution function definition here (e.g., from above).

int main() {
    // n = 1
    std::vector<std::string> r1 = generateRightAlignedTriangle(1);
    assert(r1.size() == 1);
    assert(r1[0] == "*");

    // n = 2
    std::vector<std::string> r2 = generateRightAlignedTriangle(2);
    assert(r2.size() == 2);
    assert(r2[0] == " *");
    assert(r2[1] == "**");

    // n = 3
    std::vector<std::string> r3 = generateRightAlignedTriangle(3);
    assert(r3.size() == 3);
    assert(r3[0] == "  *");
    assert(r3[1] == " **");
    assert(r3[2] == "***");

    // n = 0
    std::vector<std::string> r0 = generateRightAlignedTriangle(0);
    assert(r0.empty());

    // n = -5
    std::vector<std::string> rneg = generateRightAlignedTriangle(-5);
    assert(rneg.empty());

    // n = 4, check row lengths are exactly n
    std::vector<std::string> r4 = generateRightAlignedTriangle(4);
    assert(r4.size() == 4);
    for (size_t idx = 0; idx < r4.size(); ++idx) {
        assert(r4[idx].size() == 4);
    }
    assert(r4[0] == "   *");
    assert(r4[1] == "  **");
    assert(r4[2] == " ***");
    assert(r4[3] == "****");
}
// The solution builds each row from bottom to top logic: for a given row index `i` starting at 1 and going up to `n`, the number of leading spaces is `n - i`, and the number of asterisks is `i`. Construct each row as a string of that many spaces followed by that many asterisks, then append to a vector. The total length of each row string is exactly `n` (spaces + asterisks). We iterate for `i = 1` to `n`, so time complexity is `O(n^2)` because each row has length `n` and there are `n` rows, so total characters output is `n * (n+1)/2` plus spaces, but asymptotically `O(n^2)`. Space complexity is `O(n^2)` as we store all rows in the vector. Edge cases: if `n <= 0`, return empty vector. If `n == 1`, row is just `"*"`. No other special cases needed because the loop handles all `n` correctly; ensure integer arithmetic works for `n - i` when `i == n` giving zero spaces.
