// Write a C++ function `countAInRepeatedString` that takes a non-empty string `s` consisting of lowercase English letters and a positive long integer `n`, and returns the total number of occurrences of the character `'a'` in the first `n` characters of the infinite string formed by repeating `s` indefinitely. For example, if `s = "aba"` and `n = 10`, the infinite string is `"abaabaabaa"`, and the first 10 characters contain exactly 7 occurrences of `'a'`. The function must handle very large `n` (up to 10^12) efficiently without constructing the repeated string explicitly. Assume the input string is non-empty and may contain characters other than `'a'`. The function should be pure (no side effects like printing) and return a `long` value.
// The solution avoids building the repeated string, which would be impossible for large `n`. Instead, count the number of `'a'` in the original string `s` first; call this `countPerBlock`. Let `len = s.size()`. The full repetitions of `s` that fit entirely within the first `n` characters are `fullBlocks = n / len`. These contribute `countPerBlock * fullBlocks` occurrences. The remaining `remainder = n % len` characters are a prefix of `s`, so we count `'a'` in the prefix `s.substr(0, remainder)` and add that to the total. Edge cases include: when `n` is a multiple of `len` (remainder zero), when `s` contains no `'a'` (result is zero), and when `s` length is 1 (works fine, the logic reduces to multiplying by `n`). The time complexity is O(len) because we scan the string twice (once for full count, once for prefix) — O(len) overall, and O(1) auxiliary space. The use of `long` is essential to avoid overflow for large `n` (e.g., `n` up to 10^12, and `len` up to 100, multiplication fits in 64-bit).
#include <string>

// Count occurrences of 'a' in the first n characters of the infinite repetition of s.
long countAInRepeatedString(const std::string& s, long n) {
    const std::string::size_type len = s.size();
    long countPerBlock = 0;
    for (const char ch : s) {
        if (ch == 'a') {
            ++countPerBlock;
        }
    }

    const long fullBlocks = n / len;
    const long remainder = n % len;
    long total = countPerBlock * fullBlocks;

    for (std::string::size_type i = 0; i < static_cast<std::string::size_type>(remainder); ++i) {
        if (s[i] == 'a') {
            ++total;
        }
    }
    return total;
}
#include <cassert>
#include <string>

// Assume countAInRepeatedString is defined above.
long countAInRepeatedString(const std::string& s, long n);

int main() {
    // Basic examples
    assert(countAInRepeatedString("aba", 10) == 7);
    assert(countAInRepeatedString("a", 1000000000000L) == 1000000000000L);
    assert(countAInRepeatedString("b", 10) == 0);

    // Edge: n smaller than string length
    assert(countAInRepeatedString("abca", 2) == 1);  // "ab"
    assert(countAInRepeatedString("abc", 3) == 1);   // "abc"
    assert(countAInRepeatedString("abc", 4) == 2);   // "abca" -> two 'a's

    // Edge: n exactly multiple of string length
    assert(countAInRepeatedString("aa", 6) == 6);
    assert(countAInRepeatedString("ab", 4) == 2);

    // Edge: string with no 'a' but remainder still zero
    assert(countAInRepeatedString("xyz", 9) == 0);

    // Edge: large n and mixed content
    assert(countAInRepeatedString("abcabc", 10) == 4); // "abcabcabca" -> 4 'a's

    // Edge: single character 'a' repeated, n=1
    assert(countAInRepeatedString("a", 1) == 1);

    // Edge: string of length 1 with non-'a'
    assert(countAInRepeatedString("z", 100) == 0);

    // Edge: string length 2, n=5
    assert(countAInRepeatedString("aa", 5) == 5); // "aaaaa"
    assert(countAInRepeatedString("ba", 5) == 2); // "babab" -> only positions 2 and 4 are 'a'
}
