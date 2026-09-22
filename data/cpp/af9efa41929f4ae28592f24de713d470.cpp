/*
Write a C++ function that takes two strings `s` and `t` as input, where `s` may contain the wildcard character `'?'`, and `t` consists only of lowercase English letters. The function must replace each `'?'` in `s` with a lowercase English letter such that after replacement, the resulting string contains `t` as a subsequence as many times as possible (specifically, the maximum integer `k`, where `k` is a positive integer, such that `t` appears as a subsequence in the modified `s` at least `k` times). If multiple replacements achieve the same maximum `k`, any valid replacement is acceptable. The function should return the modified string after all `'?'` are replaced. For the replacement, follow this strategy: first determine the maximum `k` such that the letter counts in `s` (excluding `'?'`) can be supplemented by the `'?'` characters to satisfy the counts required for `k` copies of `t`. Then replace `'?'` greedily, first supplying the letters needed to complete `k` copies of `t` (in alphabetical order), and any remaining `'?'` with `'a'`. The input strings will be non-empty, and `t` will contain only lowercase letters. You may assume that the given `s` can always be modified to contain at least one copy of `t` as a subsequence (i.e., the total length of `s` is at least the length of `t`). The output string must have the same length as `s` and contain no `'?'`.
*/

#include <string>
#include <array>
#include <algorithm>

// Replace '?' in s to maximize the number of times t appears as a subsequence.
// Returns the modified string.
std::string maximizeSubsequence(std::string s, const std::string& t) {
    // Count letters in t.
    std::array<int, 26> statT = {};
    for (char c : t) {
        statT[c - 'a']++;
    }

    // Count letters in s excluding '?' and count the number of '?'.
    std::array<int, 26> statS = {};
    int empty = 0;
    for (char c : s) {
        if (c == '?') {
            empty++;
        } else {
            statS[c - 'a']++;
        }
    }

    // Find the maximum k such that deficits can be filled by '?'.
    int k = 0;
    for (int candidate = 1; ; ++candidate) {
        int needed = 0;
        for (int i = 0; i < 26; ++i) {
            needed += std::max(statT[i] * candidate - statS[i], 0);
        }
        if (needed > empty) {
            break;
        }
        k = candidate;
    }

    // Build the string of letters that must replace some '?' to satisfy k copies.
    std::string res;
    for (int i = 0; i < 26; ++i) {
        int deficit = std::max(statT[i] * k - statS[i], 0);
        res.append(deficit, static_cast<char>('a' + i));
    }

    // Replace '?' sequentially.
    size_t pos = 0;
    for (char& c : s) {
        if (c == '?') {
            if (pos < res.size()) {
                c = res[pos];
                ++pos;
            } else {
                c = 'a';
            }
        }
    }

    return s;
}

#include <cassert>
#include <string>

// Declaration of the solution function (provided separately).
std::string maximizeSubsequence(std::string s, const std::string& t);

int main() {
    // Basic case: one '?' fills the missing letter.
    assert(maximizeSubsequence("a?b", "ab") == "aab");

    // No '?' needed if s already contains t enough times.
    assert(maximizeSubsequence("abab", "ab") == "abab");

    // Multiple '?' allow more than one copy.
    assert(maximizeSubsequence("a??", "ab") == "aab");

    // '?' at the end, no deficit, replaced with 'a'.
    assert(maximizeSubsequence("abc?", "abc") == "abca");

    // Larger example: need to fill multiple letters in alphabetical order.
    assert(maximizeSubsequence("???", "abc") == "abc");

    // All '?' and t has repeated letters.
    assert(maximizeSubsequence("????", "aa") == "aaaa");

    // Existing letters already satisfy one copy, extra '?' become 'a'.
    assert(maximizeSubsequence("abc??", "abc") == "abcaa");

    // '?' only in the middle, and t requires more than one copy.
    assert(maximizeSubsequence("a??b", "ab") == "aabb");

    // s already contains 2 copies, no '?'.
    assert(maximizeSubsequence("aabb", "ab") == "aabb");

    // Mixed case: t has multiple distinct letters.
    assert(maximizeSubsequence("a?c?e", "ace") == "aacce");
}

// The problem requires finding the maximum repetition count `k` of `t` that can be embedded as a subsequence into `s` after replacing `'?'`. The approach is to count the frequency of each of the 26 letters in `t` and in the non-`?` part of `s`. For a candidate `k`, the total required count of each letter is `statT[i] * k`. If the existing count `statS[i]` is insufficient, we need `max(statT[i] * k - statS[i], 0)` additional copies of that letter, which must come from the `'?'` positions. The sum of these deficits across all letters must not exceed the total number of `'?'` (denoted `empty`). To find the maximum `k`, we can start from `k = 1` and increment, checking the condition, until it fails; the largest `k` that satisfies the condition is the answer. Since the number of `'?'` is bounded by the length of `s` (at most a few thousand in typical constraints), this linear search is efficient; alternatively, binary search could be used, but linear is simpler given small lengths. After determining `k`, we compute the deficit for each letter and build a string `res` containing those needed letters in alphabetical order (to ensure deterministic output). Then we replace each `'?'` in `s` sequentially: if there are remaining letters in `res`, take the next one; otherwise, replace with `'a'` (since no more specific requirement exists, and placing `'a'` does not harm the count of `k` copies because the required deficits are already fulfilled). Edge cases: if `t` contains characters not in `s` and no `'?'` remains, `k` would be 0, but the problem guarantees feasibility; if `empty` is 0, then `k` is determined solely by the existing counts; if multiple letters have deficits, alphabetical order helps with reproducibility. Time complexity is O(|s| + |t| + 26 * k_max) for counting and searching, where k_max is at most |s|/|t| (so effectively O(|s| + |t| + 26 * |s|)), and using a linear loop is fine. Space complexity is O(26) for arrays plus O(|s|) for the result string.
