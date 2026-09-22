// Write a C++ function `std::vector<int> findAllOccurrences(const std::string& text, const std::string& pattern)` that returns the starting indices (0-based) of every non-overlapping occurrence of `pattern` in `text`. The function must implement the Knuth-Morris-Pratt (KMP) string matching algorithm. If the pattern is empty or occurs nowhere, return an empty vector. The search should be case-sensitive and handle arbitrary ASCII text, including characters like spaces, digits, and punctuation. The returned indices must be in increasing order. For example, for `text = "aaaa"` and `pattern = "aa"`, the result should be `{0, 2}` (non-overlapping occurrences), not `{0, 1, 2}`. Ensure your implementation does not use built-in `std::string::find` or any substring search library functions.

#include <cassert>
#include <vector>
#include <string>

// Solution function declaration (must match the provided implementation)
std::vector<int> findAllOccurrences(const std::string& text, const std::string& pattern);

int main() {
    // Basic case
    assert(findAllOccurrences("abxabcdabxabcdabcdabcy", "abcdabcy") == std::vector<int>({9}));
    // Multiple non-overlapping occurrences
    assert(findAllOccurrences("aaaa", "aa") == std::vector<int>({0, 2}));
    // Pattern at beginning and end
    assert(findAllOccurrences("abcabcab", "abc") == std::vector<int>({0, 3}));
    // Pattern not found
    assert(findAllOccurrences("hello world", "xyz") == std::vector<int>({}));
    // Empty pattern returns empty
    assert(findAllOccurrences("any text", "") == std::vector<int>({}));
    // Pattern longer than text
    assert(findAllOccurrences("short", "longerpattern") == std::vector<int>({}));
    // Single character pattern
    assert(findAllOccurrences("banana", "a") == std::vector<int>({1, 3, 5}));
    // Pattern with characters repeated
    assert(findAllOccurrences("abababa", "aba") == std::vector<int>({0, 4})); // non-overlapping
    // Pattern with spaces and punctuation
    assert(findAllOccurrences("a b a c a b", "a b") == std::vector<int>({0, 4}));
    // Identical text and pattern
    assert(findAllOccurrences("test", "test") == std::vector<int>({0}));
    return 0;
}

#include <vector>
#include <string>

// Compute the longest prefix-suffix (LPS) array for KMP matching.
static std::vector<int> computeLPS(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> lps(m, 0);
    int j = 0;  // length of previous longest prefix-suffix
    for (int i = 1; i < m; ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = lps[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            ++j;
            lps[i] = j;
        }
        // else lps[i] remains 0
    }
    return lps;
}

// Find all non-overlapping starting indices of pattern in text using KMP.
std::vector<int> findAllOccurrences(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0) {
        return matches;  // empty pattern yields no matches per specification
    }
    if (m > n) {
        return matches;  // pattern cannot fit
    }

    std::vector<int> lps = computeLPS(pattern);
    int i = 0;  // index for text
    int j = 0;  // index for pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
            if (j == m) {
                matches.push_back(i - j);  // starting index of match
                j = 0;  // reset to avoid overlapping matches
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }
    return matches;
}

// The solution uses the KMP algorithm, which precomputes the longest proper prefix which is also a suffix (LPS) array for the pattern. This array allows the search to avoid re-comparing characters that are already known to match, giving linear time complexity. The LPS array is built by comparing the pattern with itself: for each position `i` (starting at 1), maintain a pointer `j` that tracks the length of the current prefix-suffix match. If `pattern[i] == pattern[j]`, we extend the match and set `lps[i] = j+1`. Otherwise, we fall back using `j = lps[j-1]` (if `j > 0`) or reset `lps[i] = 0`. In the main search, we iterate over text with indices `i` (text) and `j` (pattern). On a character match, both indices advance; when `j` reaches pattern length, we record the starting position `i - m` and then set `j = lps[j-1]` to look for the next occurrence. Crucially, to ensure non-overlapping occurrences, after recording a match we reset `j` to 0 (instead of allowing overlap). The fallback logic for mismatches is standard. Edge cases: empty pattern returns empty vector; pattern longer than text returns empty vector; overlapping patterns must not be reported multiple times (handled by resetting `j` to 0 after match). Time complexity is `O(n + m)` where `n` is text length and `m` is pattern length; space complexity is `O(m)` for the LPS array.
