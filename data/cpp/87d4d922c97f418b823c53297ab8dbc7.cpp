Write a C++ function `printPattern(int n)` that takes a positive integer `n` and returns a `std::string` containing a pattern of `n` rows, each row having exactly `n` characters. The first row consists of `n` occurrences of the character `'A'`, the second row consists of `n` occurrences of `'B'`, and so on, increasing the letter by one each row (i.e., row `i` uses the `i`-th letter of the alphabet, starting from `'A'` for row 1). Rows are separated by a newline character `'\n'` (do not add a trailing newline after the last row). For example, if `n = 3`, the returned string must be `"AAA\nBBB\nCCC"`. The function should handle arbitrary positive `n` values, including potentially large ones, but you may assume the alphabet wraps after `'Z'` (i.e., after `'Z'`, the next letter is `'A'` again, so for `n = 27`, row 27 would be all `'A'`s). However, for simplicity and to avoid overflow, you may assume `n` is small enough that all letters fit in a `char` (i.e., `n <= 26` or allow wrapping, but ensure correctness). The function must be self-contained, taking only the integer parameter and returning the constructed string.

// The problem is straightforward: generate a string with `n` rows, each row having `n` repeated characters. The character for row `i` (1-indexed) is `'A' + (i - 1)`, but to handle `n > 26` gracefully, we can use modular arithmetic: `char('A' + (i - 1) % 26)`. The main algorithm: for each row from 1 to `n`, create a substring of length `n` containing the same character, append it to the result string, and add a newline if it is not the last row. Edge cases: `n` could be 1 (only one row, no trailing newline), and `n` could be large (memory usage is `O(n^2)` due to the output size, so time complexity is `O(n^2)`). The space complexity is also `O(n^2)` because the returned string holds all characters. We can optimize by building each row using `std::string(n, ch)` for efficiency. The solution uses no extra space beyond the output string.

#include <string>

// Return a pattern of n rows, each row containing n copies of a letter,
// starting from 'A' and incrementing each row (wrapping after 'Z').
std::string printPattern(int n) {
    std::string result;
    // Reserve space to avoid reallocations: n rows of n chars plus newlines.
    result.reserve(static_cast<size_t>(n) * (n + 1));

    for (int row = 1; row <= n; ++row) {
        const char ch = static_cast<char>('A' + (row - 1) % 26);
        if (row > 1) {
            result.push_back('\n');
        }
        result.append(n, ch);
    }
    return result;
}

#include <cassert>
#include <string>

// Assume the solution's printPattern is declared above or included.
int main() {
    // Basic case: 3 rows
    assert(printPattern(1) == "A");
    assert(printPattern(2) == "AA\nBB");
    assert(printPattern(3) == "AAA\nBBB\nCCC");
    // Check wrapping after 'Z'
    assert(printPattern(27) == "AAAAAAAAAAAAAAAAAAAAAAAAAAA\nBBBBBBBBBBBBBBBBBBBBBBBBBBB\nCCCCCCCCCCCCCCCCCCCCCCCCCCC");
    // Direct comparison for a 5-row pattern
    std::string expected = "AAAAA\nBBBBB\nCCCCC\nDDDDD\nEEEEE";
    assert(printPattern(5) == expected);
    // Ensure no trailing newline
    std::string result = printPattern(4);
    assert(result.back() != '\n');
    // Check length: n rows * n chars + (n-1) newlines
    assert(printPattern(6).size() == 6 * 6 + 5);
}
