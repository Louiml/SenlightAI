// Write a C++ function `bool canBreakWord(const std::string& s, const std::vector<std::string>& wordDict)` that returns `true` if the string `s` can be segmented into a space-separated sequence of one or more dictionary words, where each word must appear exactly as in `wordDict` (case-sensitive) and words can be reused. The function must handle empty strings, empty dictionaries, duplicate dictionary entries, and very long strings efficiently. You may not use the STL `regex` or `std::regex`, and the solution must be self-contained in a single function (no global helper functions or static variables). Output `true`/`false` based on whether a valid segmentation exists.

#include <cassert>
#include <vector>
#include <string>
#include <functional>

// Include the solution function here (or link it).
// For self-contained test, copy the function above into this file.

int main() {
    using std::vector;
    using std::string;

    // Basic cases
    assert(canBreakWord("leetcode", {"leet", "code"}) == true);
    assert(canBreakWord("applepenapple", {"apple", "pen"}) == true);
    assert(canBreakWord("catsandog", {"cats", "dog", "sand", "and", "cat"}) == false);

    // Empty string always true (base case)
    assert(canBreakWord("", {"a"}) == true);

    // Empty dictionary: non-empty string cannot break
    assert(canBreakWord("a", {}) == false);

    // Single character with matching dict
    assert(canBreakWord("a", {"a"}) == true);
    assert(canBreakWord("b", {"a"}) == false);

    // Duplicate dictionary entries: should not affect result
    assert(canBreakWord("aaaa", {"a", "a", "aa"}) == true);
    assert(canBreakWord("ab", {"a", "b", "a"}) == true);
    assert(canBreakWord("abc", {"a", "ab", "bc"}) == true); // "a"+"bc" works
    assert(canBreakWord("abcd", {"a", "abc", "d"}) == true);
    assert(canBreakWord("abcd", {"ab", "cd", "bc"}) == false); // cannot combine

    // Long string repeated pattern
    string longStr = "abcabcabcabc";
    vector<string> dict2 = {"abc", "a", "bc"};
    assert(canBreakWord(longStr, dict2) == true);

    // Overlapping words
    assert(canBreakWord("pineapplepenapple", {"apple", "pen", "applepen", "pine", "pineapple"}) == true);
    assert(canBreakWord("catsandog", {"cats", "dog", "sand", "and", "cat"}) == false);

    return 0;
}

#include <string>
#include <vector>

// Returns true if s can be segmented into dictionary words.
// Uses memoization (top-down DP) with dp[i] = -1 unknown, 0 false, 1 true.
bool canBreakWord(const std::string& s, const std::vector<std::string>& wordDict) {
    const int n = static_cast<int>(s.size());
    std::vector<int> memo(n + 1, -1);

    // Recursive lambda for clean implementation (C++14 style).
    std::function<bool(int)> rec = [&](int i) -> bool {
        if (i == n) return true; // empty suffix is always segmentable
        if (memo[i] != -1) return memo[i] == 1;

        bool ok = false;
        for (const std::string& w : wordDict) {
            int wlen = static_cast<int>(w.size());
            // Skip if word longer than remaining substring
            if (i + wlen <= n && s.compare(i, wlen, w) == 0) {
                if (rec(i + wlen)) {
                    ok = true;
                    break;
                }
            }
        }
        memo[i] = ok ? 1 : 0;
        return ok;
    };

    return rec(0);
}

// The problem is a classic word break DP. Define `dp[i]` as whether the substring starting at index `i` can be segmented. The recurrence: `dp[i]` is true if for any word `w` in the dictionary, `s.substr(i, w.size()) == w` and `dp[i + w.size()]` is true. Base case: `dp[s.size()] = true` (empty suffix). Use memoization with a vector `memo` of size `n+1` initialized to `-1` (unknown), `0` (false), `1` (true). Recursively explore from index 0. Edge cases: empty string (return true if dictionary is non-empty? Actually, the typical definition returns true for empty string because you can segment trivially – but here the problem says "one or more dictionary words", so for empty string we can return true if `wordDict` contains empty string? Usually word break returns true for empty string. However, in the original code, `rec(i==s.size())` returns true, so empty string returns true regardless. We will follow that: empty string returns true. Also handle case where dictionary contains a word longer than the remaining substring – skip. Duplicate words in dict are handled naturally. Time complexity: O(n * m * L) where n = string length, m = number of dictionary words, L = average word length, because for each index we try each word and do substring comparison. Space: O(n) for DP array plus recursion stack depth O(n).
