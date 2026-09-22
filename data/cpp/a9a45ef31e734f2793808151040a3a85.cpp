// Write a C++ function `vector<int> kmpMatch(const string& s, const string& t)` that returns all starting positions (0-indexed) where the pattern string `t` occurs as a contiguous substring in the text string `s`. The function must implement the KMP algorithm from scratch, including building the prefix (failure) function for `t`. The pattern and text may contain any printable ASCII characters (including spaces, digits, punctuation). If `t` is empty, return an empty vector. The positions must be in increasing order. Your function must not use any external pattern-matching functions such as `std::string::find` — only KMP logic.
#include <cassert>
#include <vector>
#include <string>

// (Function definition from Solution section goes here)

int main() {
    // Basic cases
    assert(kmpMatch("hello world", "world") == std::vector<int>({6}));
    assert(kmpMatch("hello world", "hello") == std::vector<int>({0}));
    assert(kmpMatch("abcabcabc", "abc") == std::vector<int>({0, 3, 6}));
    assert(kmpMatch("aaaa", "aa") == std::vector<int>({0, 1, 2}));
    assert(kmpMatch("abc", "def") == std::vector<int>({}));

    // Empty pattern
    assert(kmpMatch("anything", "") == std::vector<int>({}));

    // Pattern longer than text
    assert(kmpMatch("ab", "abc") == std::vector<int>({}));

    // Overlapping matches with repeated pattern
    assert(kmpMatch("abababa", "aba") == std::vector<int>({0, 2, 4}));

    // Single character pattern
    assert(kmpMatch("mississippi", "s") == std::vector<int>({2, 3, 5, 6}));

    // Exact match
    assert(kmpMatch("abc", "abc") == std::vector<int>({0}));

    // Text exactly equal to pattern multiple times (edge case for prefix update)
    assert(kmpMatch("ababa", "aba") == std::vector<int>({0, 2}));

    // Pattern with repeated prefix/suffix (classic KMP test)
    assert(kmpMatch("aaabaaaab", "aaab") == std::vector<int>({0, 5}));
    assert(kmpMatch("ababab", "ab") == std::vector<int>({0, 2, 4}));

    return 0;
}
#include <string>
#include <vector>

// Returns all 0-indexed start positions where pattern t occurs in text s using KMP.
std::vector<int> kmpMatch(const std::string& s, const std::string& t) {
    const int n = static_cast<int>(s.size());
    const int m = static_cast<int>(t.size());
    if (m == 0) return {};

    // Build prefix function (failure function)
    std::vector<int> ne(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && t[i] != t[j]) {
            j = ne[j - 1];
        }
        if (t[i] == t[j]) {
            ++j;
        }
        ne[i] = j;
    }

    // Scan text
    std::vector<int> positions;
    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && s[i] != t[j]) {
            j = ne[j - 1];
        }
        if (s[i] == t[j]) {
            ++j;
        }
        if (j == m) { // found a match
            positions.push_back(i - m + 1); // 0-indexed start
            j = ne[j - 1]; // continue for overlapping matches
        }
    }
    return positions;
}
// The KMP algorithm precomputes a prefix function (often called `ne` or `lps`) for the pattern that stores, for each prefix of the pattern, the length of the longest proper prefix that is also a suffix. This allows efficient backtracking when a mismatch occurs during scanning. First, build `ne` of length `m` (pattern length) in O(m) time. Then iterate through the text with two indices: `i` for the text and `j` for the current matched length in the pattern. On a match, advance both. On mismatch, if `j>0`, set `j = ne[j-1]` and do not advance `i`; otherwise advance `i`. When `j` reaches `m`, record the start index `i - j + 1` (0-indexed in C++), then set `j = ne[j-1]` to continue searching for overlapping occurrences. Edge cases: empty pattern returns empty; pattern longer than text returns empty; repeated patterns and overlapping matches (e.g., "aaa" in "aaaa") must be handled correctly. Time complexity: O(n + m), where n is text length and m is pattern length. Space complexity: O(m) for the prefix array, plus O(1) extra for scanning.
