/*
Write a C++ function `int repeatedStringMatch(const std::string& a, const std::string& b)` that takes two non-empty strings `a` and `b`. The function should return the minimum number of times `a` must be repeated (concatenated with itself) such that `b` becomes a substring of the resulting repeated string. If `b` can never become a substring regardless of how many times `a` is repeated, return `-1`. The repetition count must be a positive integer (at least 1). For example, if `a = "ab"` and `b = "abab"`, repeating `a` twice yields `"abab"`, which contains `b`, so return `2`. If `a = "abc"` and `b = "cabcabc"`, repeating `a` three times yields `"abcabcabc"`, which contains `b`, so return `3`. If `a = "aa"` and `b = "aaa"`, repeating twice yields `"aaaa"` (contains `aaa`), return `2`.
*/

#include <string>

// Returns the minimum number of repetitions of 'a' needed for 'b' to be a substring.
// If impossible, returns -1.
int repeatedStringMatch(const std::string& a, const std::string& b) {
    int repeatCount = 1;
    std::string current = a;

    // Ensure current string length is at least b's length.
    while (current.length() < b.length()) {
        current += a;
        ++repeatCount;
    }

    // Check if b is already present.
    if (current.find(b) != std::string::npos) {
        return repeatCount;
    }

    // Add one more repetition to handle matches starting near the end.
    current += a;
    ++repeatCount;
    if (current.find(b) != std::string::npos) {
        return repeatCount;
    }

    return -1;
}

#include <cassert>
#include <string>

// Declare the function from the solution.
int repeatedStringMatch(const std::string& a, const std::string& b);

int main() {
    // Basic cases from the problem statement.
    assert(repeatedStringMatch("abcd", "cdabcdab") == 3);
    assert(repeatedStringMatch("ab", "abab") == 2);
    assert(repeatedStringMatch("abc", "cabcabc") == 3);
    assert(repeatedStringMatch("aa", "aaa") == 2);

    // Edge cases.
    assert(repeatedStringMatch("a", "a") == 1);
    assert(repeatedStringMatch("a", "aa") == 2);
    assert(repeatedStringMatch("abc", "abcabcabc") == 3);
    assert(repeatedStringMatch("ab", "ba") == 2);  // "abab" contains "ba"
    assert(repeatedStringMatch("abc", "def") == -1); // impossible
    assert(repeatedStringMatch("x", "xxy") == -1);   // impossible

    return 0;
}

// The key observation is that to contain `b` as a substring, the repeated string must be at least as long as `b`, but because the match can start at any position within a copy of `a`, we may need one extra repetition beyond the minimum length. The algorithm starts with one copy of `a` and a repetition count of 1. If the current string is shorter than `b`, we append another copy of `a` and increment the count. After this loop, the current string has length >= `b`. We check if `b` is a substring; if yes, we return the current count. Otherwise, we append one more copy of `a` (incrementing the count) because `b` might start near the end of the current string and spill into the next repetition. We check again; if found, return the new count; otherwise, return `-1` because adding more repetitions would only extend the string further but could never create a match that wasn't possible in the first `len(a)+len(b)` region — after that, any match would have started earlier. The time complexity is `O(n * (m + n))` in the worst case for substring search, where `n` is the length of `a` and `m` is the length of `b` (with typical `find` implementations being linear in the product). The space complexity is `O(n + m)` for constructing the repeated string.
