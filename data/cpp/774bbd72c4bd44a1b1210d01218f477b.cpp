// Write a C++ function that takes a positive integer `n` and returns a string containing a right-aligned triangle pattern of asterisks, where the number of asterisks on each row equals the row number (starting from 1), and the total number of rows is `n`. Each row ends with a newline character (`\n`), and there should be no leading spaces. For example, for `n=5`, the output should be `"*\n**\n***\n****\n*****\n"`. If `n` is 0 or negative, return an empty string. The function should be named `makeTriangle` and must be const-correct where appropriate.

The task is straightforward: we need to build a string that represents a triangular pattern. The algorithm is: iterate from row number 1 up to `n`. For each row, append `i` asterisks (`'*'`) to the result, then append a newline character. For `n` ≤ 0, return an empty string immediately because no rows are drawn. Edge cases include `n=1` (single row with one asterisk), `n=0` (empty string), and negative numbers (empty string). The complexity is linear in the total number of characters produced, which is \(O(n^2)\) characters (since row `i` has `i` asterisks plus one newline, summing to \(n(n+1)/2 + n\) characters), and the auxiliary space used is \(O(n^2)\) for the result string itself (which is unavoidable). No extra data structures are needed.

#include <string>

// Build a right-aligned triangle of asterisks with n rows.
// Each row i (1-indexed) contains i asterisks followed by a newline.
// Returns an empty string if n <= 0.
std::string makeTriangle(int n) {
    if (n <= 0) {
        return "";
    }
    
    std::string result;
    // Reserve space to avoid repeated reallocations: sum of 1..n asterisks + n newlines
    result.reserve(static_cast<size_t>(n) * (n + 1) / 2 + n);
    
    for (int row = 1; row <= n; ++row) {
        result.append(row, '*');
        result.push_back('\n');
    }
    return result;
}

#include <cassert>
#include <string>

// Function declaration (or include the solution header here)
std::string makeTriangle(int n);

int main() {
    // Test n = 5 (typical case)
    assert(makeTriangle(5) == "*\n**\n***\n****\n*****\n");
    
    // Test n = 1 (single row)
    assert(makeTriangle(1) == "*\n");
    
    // Test n = 0 (empty)
    assert(makeTriangle(0) == "");
    
    // Test negative n (empty)
    assert(makeTriangle(-3) == "");
    
    // Test n = 3
    assert(makeTriangle(3) == "*\n**\n***\n");
    
    // Test n = 2
    assert(makeTriangle(2) == "*\n**\n");
    
    // Test n = 4
    assert(makeTriangle(4) == "*\n**\n***\n****\n");
    
    return 0;
}
