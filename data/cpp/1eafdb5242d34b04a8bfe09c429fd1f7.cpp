Write a C++ function named `isSubsequence` that takes two strings, `s` (short, length ≤ 100) and `t` (potentially very long, length up to ~500,000), both containing only lowercase English letters, and returns `true` if `s` is a subsequence of `t`, and `false` otherwise. A subsequence can be obtained by deleting some (or none) characters from `t` without changing the order of the remaining characters. For example, `"ace"` is a subsequence of `"abcde"`, but `"aec"` is not. The function must handle edge cases such as an empty `s` (which is always a subsequence), an empty `t` (which can only match an empty `s`), and cases where `s` is longer than `t` (which must return `false`). The solution should be efficient enough for the very long `t` (expected linear time in the length of `t`).
The simplest and most efficient approach is a two‑pointer greedy scan. We maintain an index `i` over `s` and an index `j` over `t`. For each character in `t`, if it matches the current character of `s`, move `i` forward. Continue until either all characters of `s` are matched (return `true`) or we reach the end of `t` (return `false`). This works because to check subsequence, we only need to match characters greedily from left to right; skipping any earlier match cannot help. Edge cases: empty `s` returns `true` immediately (even if `t` is empty); if `s` is longer than `t`, return `false` early. Time complexity is `O(|t|)` because we scan `t` once; space complexity is `O(1)` beyond the input strings. This is far better than the recursive backtracking in the snippet, which could be exponential in the worst case.
#include <string>

// Returns true if s is a subsequence of t, false otherwise.
bool isSubsequence(const std::string& s, const std::string& t) {
    // Empty s is always a subsequence.
    if (s.empty()) return true;
    // If s is longer than t, it cannot be a subsequence.
    if (s.size() > t.size()) return false;

    size_t i = 0; // index over s
    for (size_t j = 0; j < t.size() && i < s.size(); ++j) {
        if (s[i] == t[j]) {
            ++i;
        }
    }
    return i == s.size();
}
#include <cassert>
#include <string>

int main() {
    // Example 1 from problem statement
    assert(isSubsequence("abc", "ahbgdc") == true);
    // Example 2 from problem statement
    assert(isSubsequence("axc", "ahbgdc") == false);
    // Empty s is always a subsequence
    assert(isSubsequence("", "anylongstring") == true);
    // Empty t only matches empty s
    assert(isSubsequence("a", "") == false);
    assert(isSubsequence("", "") == true);
    // s longer than t
    assert(isSubsequence("hello", "hi") == false);
    // Exact match
    assert(isSubsequence("same", "same") == true);
    // All characters match but out of order
    assert(isSubsequence("abc", "cba") == false);
    // Duplicate characters in t
    assert(isSubsequence("aaa", "aaabaaa") == true);
    // Single character match at the end
    assert(isSubsequence("z", "abcdefghijklmnopqrstuvwxyz") == true);
    // Single character not present
    assert(isSubsequence("z", "abc") == false);
    return 0;
}
