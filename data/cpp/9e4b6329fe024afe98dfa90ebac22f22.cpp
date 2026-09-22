// Write a C++ function named `isSubsequence` that takes two strings, `s` and `t`, and returns a boolean indicating whether `s` is a subsequence of `t`. A string `s` is a subsequence of `t` if all characters of `s` appear in `t` in the same order, without requiring them to be contiguous. The function must handle empty strings, case-sensitive matching, and cases where `s` is longer than `t`. The function should be efficient, processing each string only once, and must be declared with proper `const` correctness for parameters.

#include <cassert>
#include <string>

// Declaration of the function under test
bool isSubsequence(const std::string& s, const std::string& t);

int main() {
    // Basic true cases
    assert(isSubsequence("abc", "ahbgdc") == true);
    assert(isSubsequence("", "anything") == true);
    assert(isSubsequence("a", "a") == true);

    // Basic false cases
    assert(isSubsequence("axc", "ahbgdc") == false);
    assert(isSubsequence("abc", "") == false);
    assert(isSubsequence("abc", "ab") == false); // s longer than t

    // Order matters
    assert(isSubsequence("acb", "ahbgdc") == false); // wrong order

    // Case sensitivity
    assert(isSubsequence("A", "a") == false);

    // Duplicate characters and non-contiguous matches
    assert(isSubsequence("aaa", "aaaa") == true);
    assert(isSubsequence("bb", "abc") == false);
    assert(isSubsequence("ace", "abcde") == true);

    return 0;
}

#include <string>

// Returns true if `s` is a subsequence of `t` (characters in order, not necessarily contiguous).
// Handles empty strings and case-sensitive comparison.
bool isSubsequence(const std::string& s, const std::string& t) {
    size_t si = 0;
    size_t ti = 0;

    while (ti < t.length() && si < s.length()) {
        if (s[si] == t[ti]) {
            ++si;
        }
        ++ti;
    }

    return si == s.length();
}

// The solution uses a two-pointer technique: one pointer `si` for string `s` and one pointer `ti` for string `t`. Initialize both to 0. Iterate while both pointers are within their respective string lengths. If the characters at the current positions match, advance both pointers; otherwise, advance only the `ti` pointer to search for the match later in `t`. After the loop, if `si` equals the length of `s`, then all characters of `s` were found in order, so return `true`; otherwise, return `false`. Edge cases include: an empty `s` (always true, because the loop condition fails immediately and `si == 0 == s.length()` is true), an empty `t` with non-empty `s` (returns false because the loop never runs and `si` remains 0, which is not equal to `s.length()` unless `s` is empty), and `s` longer than `t` (will inevitably fail because `ti` runs out first). The time complexity is O(n + m) where n and m are the lengths of `t` and `s` respectively, since each pointer advances at most its string length times. Space complexity is O(1) auxiliary, not counting the input strings.
