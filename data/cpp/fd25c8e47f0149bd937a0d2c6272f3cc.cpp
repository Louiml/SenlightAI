Write a C++ function `bool isSubsequence(const std::string& text, const std::string& pattern)` that determines whether the characters of `pattern` appear in `text` in the same order (not necessarily consecutively). The function should return `true` if `pattern` is a subsequence of `text`, and `false` otherwise. Your solution must use recursion (not iteration) to solve the problem, and handle the empty string cases correctly: an empty `pattern` is always a subsequence of any `text` (including empty), and a non-empty `pattern` is never a subsequence of an empty `text`. The function must be `const`-correct and should not modify the input strings.

// The core idea is a recursive comparison from the ends of both strings. At each recursive step, we compare the last characters of the remaining suffixes. If they match, we consume both characters and recurse on `n-1` and `m-1`. If they don't match, we discard the last character of `text` and recurse on `n-1` while keeping `m` unchanged. The base cases are: if `pattern` length (`m`) is 0, all pattern characters have been matched, so return `true`; if `text` length (`n`) is 0 but `pattern` is non-empty, the pattern cannot be matched, so return `false`. Important edge cases: both strings empty → return `true`; pattern empty and text non-empty → return `true`; pattern non-empty and text empty → return `false`; duplicate characters and identical strings — handled naturally by the recursion. The recursion depth is at most `n + m` (in the worst case where we discard many text characters one at a time), giving `O(n + m)` time and `O(n + m)` space on the call stack. This is acceptable for typical input sizes in a learning exercise.

#include <string>

// Determine if pattern is a subsequence of text using recursion.
bool isSubsequence(const std::string& text, const std::string& pattern) {
    // Helper recursive function that operates on suffixes.
    // Returns true if pattern[0..m-1] is a subsequence of text[0..n-1].
    auto recur = [&](auto&& self, int n, int m) -> bool {
        // If all pattern characters are matched, success.
        if (m == 0) return true;
        // If text is exhausted but pattern remains, failure.
        if (n == 0) return false;
        // If the last characters match, consume both.
        if (text[n - 1] == pattern[m - 1]) {
            return self(self, n - 1, m - 1);
        }
        // Otherwise, discard the last text character and try again.
        return self(self, n - 1, m);
    };
    
    return recur(recur, text.length(), pattern.length());
}

#include <cassert>
#include <string>

// Declare the function (assumed to be provided as above).
bool isSubsequence(const std::string& text, const std::string& pattern);

int main() {
    // Basic positive cases.
    assert(isSubsequence("abcde", "ace") == true);
    assert(isSubsequence("abcde", "a") == true);
    assert(isSubsequence("abcde", "e") == true);
    assert(isSubsequence("abcde", "abcde") == true);
    assert(isSubsequence("hello world", "hwr") == true);

    // Basic negative cases.
    assert(isSubsequence("abcde", "aec") == false);
    assert(isSubsequence("abcde", "edcba") == false);
    assert(isSubsequence("abc", "abcd") == false);
    assert(isSubsequence("abc", "z") == false);

    // Empty string cases.
    assert(isSubsequence("", "") == true);
    assert(isSubsequence("abc", "") == true);
    assert(isSubsequence("", "a") == false);

    // Duplicate characters and identical patterns.
    assert(isSubsequence("aaa", "aa") == true);
    assert(isSubsequence("aaa", "aaa") == true);
    assert(isSubsequence("aaa", "aaaa") == false);
    assert(isSubsequence("banana", "banana") == true);

    // Case sensitivity.
    assert(isSubsequence("AbC", "AC") == true);
    assert(isSubsequence("AbC", "ac") == false);

    return 0;
}
