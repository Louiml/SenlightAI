// Write a C++ function named `smallestNumberFromPattern` that takes a non-empty string `pattern` consisting only of the characters `'I'` (meaning the next digit must be larger than the current digit) and `'D'` (meaning the next digit must be smaller than the current digit). The function must return a string representing the lexicographically smallest number (containing the digits `1` through `pattern.length() + 1`, each used exactly once, in a single continuous sequence without separators) that satisfies the given pattern. For example, if `pattern = "II"`, the output should be `"123"` because `1 < 2 < 3`. If `pattern = "DD"`, the output should be `"321"` because `3 > 2 > 1`. If `pattern = "ID"`, the output should be `"132"` (since `1 < 3` and `3 > 2`). The function must handle any pattern length from 1 to 8 inclusive, and all pattern characters must be exactly `'I'` or `'D'`. Return an empty string if the input is invalid (e.g., empty, contains other characters, or length greater than 8).

// The core idea is to construct the smallest valid number by processing the pattern sequentially while using a stack to defer output decisions. We iterate from `num = 1` to `num = pattern.length() + 1`, pushing each current `num` onto a stack. The key insight: when we encounter a `'D'` in the pattern, we do not immediately output the last pushed number; instead, we keep it in the stack because the following digit must be smaller, and the smallest possible arrangement is achieved by reversing the order of numbers between consecutive `'I'`s (or the start/end). When we encounter an `'I'` (or reach the end of the pattern), we pop and append all stack elements in LIFO order. This ensures that any run of consecutive `'D'`s produces a decreasing sequence, while a `'I'` forces the next digit to be larger by flushing the stack at that point. The algorithm runs in one pass over the pattern. Edge cases include patterns with no `'I'` (all `'D'`s), which produce the largest-to-smallest order (e.g., `"DDD"` -> `"4321"`), and patterns with no `'D'` (all `'I'`s), which produce the natural increasing order. Time complexity is O(n) where n is the pattern length, because each digit is pushed and popped exactly once. Space complexity is O(n) due to the stack and output string.

#include <string>
#include <stack>

// Returns the lexicographically smallest number string satisfying the given I/D pattern.
// Valid pattern: non-empty, length <= 8, only 'I' or 'D'. Returns empty string if invalid.
std::string smallestNumberFromPattern(const std::string& pattern) {
    // Validate input
    if (pattern.empty() || pattern.size() > 8) return "";
    for (char c : pattern) {
        if (c != 'I' && c != 'D') return "";
    }

    std::string result;
    std::stack<int> pending;

    // Process each position from 1 to pattern.size()+1
    for (int num = 1; num <= static_cast<int>(pattern.size()) + 1; ++num) {
        pending.push(num);

        // If we're at the last number or the pattern says 'I', flush the stack
        if (num == static_cast<int>(pattern.size()) + 1 || pattern[num - 1] == 'I') {
            while (!pending.empty()) {
                result += std::to_string(pending.top());
                pending.pop();
            }
        }
    }

    return result;
}

#include <cassert>
#include <string>
#include <iostream>

// Function under test is declared here (in a real test, include the header)
std::string smallestNumberFromPattern(const std::string& pattern);

int main() {
    // Basic patterns
    assert(smallestNumberFromPattern("I") == "12");
    assert(smallestNumberFromPattern("D") == "21");
    assert(smallestNumberFromPattern("II") == "123");
    assert(smallestNumberFromPattern("DD") == "321");
    assert(smallestNumberFromPattern("ID") == "132");
    assert(smallestNumberFromPattern("DI") == "213");

    // Mixed longer patterns
    assert(smallestNumberFromPattern("III") == "1234");
    assert(smallestNumberFromPattern("DDD") == "4321");
    assert(smallestNumberFromPattern("IID") == "1243");
    assert(smallestNumberFromPattern("DID") == "2143");
    assert(smallestNumberFromPattern("IDID") == "13254");

    // Edge cases: single character, max length, invalid input
    assert(smallestNumberFromPattern("I") == "12");
    assert(smallestNumberFromPattern("DDDDDDDD") == "987654321"); // length 8 pattern -> 9 digits
    assert(smallestNumberFromPattern("") == "");
    assert(smallestNumberFromPattern("IIX") == ""); // invalid character
    assert(smallestNumberFromPattern("IIIIIIIII") == ""); // length 9 pattern (too long)

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
