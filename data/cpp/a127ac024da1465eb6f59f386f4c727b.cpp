// Write a C++ function that takes two strings, `pattern` and `text`, and returns a vector of integers containing the 1-based starting positions (indices) of every occurrence of `pattern` as a substring within `text`. If no occurrences exist, the vector should contain exactly one element: `-1`. The function must implement the Knuth-Morris-Pratt (KMP) string matching algorithm, which preprocesses the pattern to build a longest proper prefix-suffix (LPS) array, then scans the text in linear time. The inputs may contain any printable ASCII characters, including spaces, and the pattern length will be at least 1. The function should be efficient for large inputs and must not use any built-in substring search functions (e.g., `std::string::find`). Return positions in increasing order, each position being the index (1-based) where the match starts in the text.

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic matches
    assert(kmpSearch("ab", "abcabc") == std::vector<int>({1, 4}));
    assert(kmpSearch("abc", "ababcabc") == std::vector<int>({3, 6}));
    
    // No match
    assert(kmpSearch("xyz", "abcabc") == std::vector<int>({-1}));
    
    // Pattern longer than text
    assert(kmpSearch("abcd", "abc") == std::vector<int>({-1}));
    
    // Empty text
    assert(kmpSearch("a", "") == std::vector<int>({-1}));
    
    // Single character pattern
    assert(kmpSearch("a", "banana") == std::vector<int>({2, 4, 6}));
    
    // Overlapping matches
    assert(kmpSearch("aaa", "aaaa") == std::vector<int>({1, 2}));
    
    // Repeated pattern with overlap
    assert(kmpSearch("abab", "ababab") == std::vector<int>({1, 3}));
    
    // Pattern with internal repetition
    assert(kmpSearch("aab", "aaab") == std::vector<int>({2}));
    
    // Pattern appears at end
    assert(kmpSearch("bcd", "abcd") == std::vector<int>({2}));
    
    // Spaces in pattern and text
    assert(kmpSearch("a b", "x a b y") == std::vector<int>({3}));
    
    return 0;
}

#include <vector>
#include <string>

// Returns 1-based starting positions of all occurrences of pattern in text.
// If none are found, returns {-1}.
std::vector<int> kmpSearch(const std::string& pattern, const std::string& text) {
    int m = pattern.length();
    int n = text.length();
    
    // Build LPS (longest proper prefix-suffix) array
    std::vector<int> lps(m, 0);
    int len = 0; // length of previous longest prefix-suffix
    for (int i = 1; i < m; ) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    std::vector<int> result;
    int i = 0; // index for text
    int j = 0; // index for pattern
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        if (j == m) {
            result.push_back(i - j + 1); // 1-based position
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    if (result.empty()) {
        result.push_back(-1);
    }
    return result;
}

// The solution uses the KMP algorithm, which avoids backtracking in the text by precomputing the LPS (longest proper prefix which is also a suffix) array for the pattern. The LPS array is built in O(m) time where m is the pattern length: initialize `lps[0]=0`, then iterate through the pattern with two pointers `i` (tracking current matched prefix length) and `j` (current position). If characters match, extend the prefix and store `lps[j] = i+1`; if they mismatch, fall back `i` to `lps[i-1]` unless it is already zero, in which case `lps[j]=0`. After building LPS, scan the text with indices `i` (text pointer) and `j` (pattern pointer). When characters match, increment both; when `j` equals pattern length, record the start position as `i - j + 1` (since `i` has advanced past the match), then set `j = lps[j-1]` to allow overlapping matches. On mismatch, if `j` is non-zero, use the LPS array to shift `j`; otherwise increment `i`. The algorithm runs in O(n + m) time, where n is text length, and uses O(m) extra space for the LPS array. Edge cases include an empty text (no matches → return `{-1}`), a pattern longer than the text (no matches), patterns with repeated characters (LPS correctness), and overlapping matches (handled by fallback after a full match). If no match is found after scanning, return `{-1}`.
