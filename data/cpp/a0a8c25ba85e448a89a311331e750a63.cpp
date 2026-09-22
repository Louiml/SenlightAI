/*
Write a C++ function `compareStrings(const std::string& a, const std::string& b)` that performs a case-sensitive lexicographical comparison of two strings and returns an integer indicating their relationship: return `-1` if `a` is less than `b`, `0` if they are equal, and `1` if `a` is greater than `b`. The function must not use the relational operators (`<`, `>`, `==`) or the `std::string::compare` method directly on the whole strings; instead, it must implement the comparison character-by-character using a loop over the underlying characters (using `operator[]` or iterators). The function must handle strings of different lengths correctly: if one string is a prefix of the other, the shorter string is considered less than the longer one. The function must be `const`-correct and work with empty strings. Your solution should not modify the input strings.
*/

#include <string>

// Compare two strings lexicographically without using relational operators or std::string::compare.
// Returns -1 if a < b, 0 if a == b, 1 if a > b.
int compareStrings(const std::string& a, const std::string& b) {
    const std::size_t minLen = (a.size() < b.size()) ? a.size() : b.size();

    for (std::size_t i = 0; i < minLen; ++i) {
        if (a[i] < b[i]) {
            return -1;
        } else if (a[i] > b[i]) {
            return 1;
        }
    }

    // All compared characters are equal; compare lengths.
    if (a.size() < b.size()) {
        return -1;
    } else if (a.size() > b.size()) {
        return 1;
    }
    return 0;
}

#include <cassert>
#include <string>

// Declaration of the function under test (assumed to be in the same translation unit or header).
int compareStrings(const std::string& a, const std::string& b);

int main() {
    // Equal strings
    assert(compareStrings("hello", "hello") == 0);
    assert(compareStrings("", "") == 0);

    // Different characters
    assert(compareStrings("abc", "abd") == -1);
    assert(compareStrings("abd", "abc") == 1);
    assert(compareStrings("apple", "banana") == -1);
    assert(compareStrings("Zebra", "apple") == -1); // ASCII: 'Z' (90) < 'a' (97)

    // Prefix cases
    assert(compareStrings("abc", "abcd") == -1);
    assert(compareStrings("abcd", "abc") == 1);

    // One empty and one non-empty
    assert(compareStrings("", "x") == -1);
    assert(compareStrings("x", "") == 1);

    // Case sensitivity
    assert(compareStrings("Hello", "hello") == -1); // 'H' (72) < 'h' (104)

    // Longer strings with early difference
    assert(compareStrings("short", "longerstring") == 1); // 's' > 'l'

    // Identical strings with digits and symbols
    assert(compareStrings("abc123", "abc123") == 0);
    assert(compareStrings("a!b", "a@b") == -1); // '!' (33) < '@' (64)

    return 0;
}

// The core challenge is to replicate the standard lexicographical comparison without relying on library shortcuts. The algorithm compares characters at each index `i` from `0` up to `min(a.size(), b.size()) – 1`. At each step, if the characters differ, the string with the smaller character (by ASCII/UTF-8 byte value) is considered smaller, so we return `-1` or `1` accordingly. If all compared characters are equal, then the shorter string is smaller: if `a.size() < b.size()`, return `-1`; if `a.size() > b.size()`, return `1`; otherwise (both strings equal length and all characters equal), return `0`. Edge cases include empty strings (loop body never executes; return based on sizes), identical strings, and one string being a prefix of another. Time complexity is O(min(n, m)) where n and m are the lengths, since we stop at the first differing character or when the shorter string ends. Space complexity is O(1) — only a few local variables. No additional memory is allocated.
