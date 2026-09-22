/*
Write a C++ function that takes a positive integer `n` as input and returns a `std::vector<std::string>` representing the rows of a right-aligned descending triangle pattern of asterisks. Each row `i` (0-indexed) must contain exactly `n - i` asterisks separated by a single space. However, the pattern must be **right-aligned** within a field of width `n` characters (the maximum possible row length), meaning each row should be padded with leading spaces so that the last asterisk of every row aligns vertically at the same column. For example, for `n = 3`, the output rows (in order) should be: `"* * *"`, `"  * *"`, `"    *"`. The function must handle edge cases where `n = 1` (single row with one asterisk) and ensure no trailing spaces after the last asterisk in each row. Return an empty vector if `n` is 0 or negative.
*/

#include <string>
#include <vector>

// Build a right-aligned descending triangle of asterisks.
// Each row i contains (n - i) asterisks separated by single spaces,
// with leading spaces so the last asterisk of every row aligns vertically.
// Returns an empty vector for n <= 0.
std::vector<std::string> rightAlignedTriangle(int n) {
    std::vector<std::string> result;
    if (n <= 0) {
        return result;
    }
    
    const int maxLen = 2 * n - 1; // length of the longest row (n asterisks with spaces)
    
    for (int row = 0; row < n; ++row) {
        int count = n - row;
        std::string rowStr;
        for (int i = 0; i < count; ++i) {
            if (i > 0) rowStr += ' ';
            rowStr += '*';
        }
        int currentLen = 2 * count - 1;
        int leadingSpaces = maxLen - currentLen;
        // Prepend the required leading spaces.
        std::string padded = std::string(leadingSpaces, ' ') + rowStr;
        result.push_back(padded);
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test n=1
    assert(rightAlignedTriangle(1) == std::vector<std::string>{"*"});
    
    // Test n=2
    assert(rightAlignedTriangle(2) == std::vector<std::string>{"* *", "  *"});
    
    // Test n=3
    assert(rightAlignedTriangle(3) == std::vector<std::string>{"* * *", "  * *", "    *"});
    
    // Test n=4 (first row should be " * * * *"? Actually with n=4, maxLen=7, row0 length=7 -> no leading spaces)
    assert(rightAlignedTriangle(4) == std::vector<std::string>{
        "* * * *",
        "  * * *",
        "    * *",
        "      *"
    });
    
    // Test n=0 (empty)
    assert(rightAlignedTriangle(0).empty());
    
    // Test n=-5 (empty)
    assert(rightAlignedTriangle(-5).empty());
    
    // Additional edge: n=5, check row alignment manually.
    auto res = rightAlignedTriangle(5);
    assert(res.size() == 5);
    assert(res[0] == "* * * * *"); // no leading spaces
    assert(res[4] == "        *"); // 8 spaces then asterisk (maxLen=9, row length=1, spaces=8)
    
    // Check that each row's last character is '*' (no trailing spaces)
    for (const auto& row : res) {
        assert(!row.empty());
        assert(row.back() == '*');
    }
    
    return 0;
}

// The solution builds each row by first constructing the string of `n - row` asterisks separated by spaces (e.g., for `n=3, row=0` => `"* * *"`). Then calculate the required leading padding: since each row has `n - row` asterisks, there are `2*(n - row) - 1` characters in the unstripped row. The maximum row length is `2*n - 1`. The number of leading spaces needed is `(maxLen - currentLen)`. Simply prepend that many spaces to the row string and push it into a vector. This works because each row is independent; no global state is needed. Edge cases: `n <= 0` returns empty vector. `n = 1` yields `maxLen = 1`, currentLen = 1, so no padding. The time complexity is O(n^2) because we build each of n rows, each with up to O(n) characters. Space complexity is O(n^2) for storing the result (the vector itself).
