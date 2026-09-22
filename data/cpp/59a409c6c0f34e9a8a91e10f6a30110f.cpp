// Write a C++ function that takes two strings, `pattern` and `text`, and returns a vector of integers containing the starting indices (0-based) of all occurrences of `pattern` in `text`. The function must use the KMP (Knuth-Morris-Pratt) string matching algorithm with an explicitly built LPS (longest proper prefix which is also suffix) array. Handle overlapping occurrences correctly (e.g., pattern "aa" in text "aaa" should return indices 0 and 1). The input strings may contain any printable ASCII characters, and if the pattern is empty or longer than the text, the function should return an empty vector. The function should be efficient and avoid unnecessary copying.

#include <cassert>
#include <vector>
#include <string>

int main() {
    using std::vector;
    using std::string;

    assert(kmpFindAll("", "abc") == vector<int>{});
    assert(kmpFindAll("abc", "ab") == vector<int>{});
    assert(kmpFindAll("a", "banana") == vector<int>({1, 3, 5}));
    assert(kmpFindAll("ana", "banana") == vector<int>({1, 3}));
    assert(kmpFindAll("aa", "aaa") == vector<int>({0, 1}));
    assert(kmpFindAll("aba", "ababa") == vector<int>({0, 2}));
    assert(kmpFindAll("abc", "abcabcabc") == vector<int>({0, 3, 6}));
    assert(kmpFindAll("xyz", "abcxyzxyz") == vector<int>({3, 6}));
    assert(kmpFindAll("test", "this is a test test") == vector<int>({10, 15}));
    assert(kmpFindAll("!!!", "!!!") == vector<int>({0}));

    return 0;
}

#include <vector>
#include <string>

// Build the LPS (longest proper prefix which is also suffix) array for the given pattern.
std::vector<int> buildLPS(const std::string& pattern) {
    int m = pattern.size();
    std::vector<int> lps(m, 0);
    int len = 0;  // length of previous longest prefix-suffix
    for (int i = 1; i < m; ++i) {
        while (len > 0 && pattern[i] != pattern[len]) {
            len = lps[len - 1];
        }
        if (pattern[i] == pattern[len]) {
            ++len;
        }
        lps[i] = len;
    }
    return lps;
}

// Return all starting indices (0-based) where pattern occurs in text using KMP.
std::vector<int> kmpFindAll(const std::string& pattern, const std::string& text) {
    std::vector<int> result;
    if (pattern.empty() || pattern.size() > text.size()) {
        return result;
    }

    std::vector<int> lps = buildLPS(pattern);
    int len = 0;  // number of characters matched in current window

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        while (len > 0 && text[i] != pattern[len]) {
            len = lps[len - 1];
        }
        if (text[i] == pattern[len]) {
            ++len;
        }
        if (len == static_cast<int>(pattern.size())) {
            result.push_back(i - pattern.size() + 1);
            len = lps[len - 1];  // allow overlapping matches
        }
    }
    return result;
}

// The solution follows the classic KMP algorithm. First, compute the LPS array for the pattern. The LPS array stores, for each prefix of the pattern ending at index `i`, the length of the longest proper prefix which is also a proper suffix. Construction iterates through the pattern, using a pointer `len` that tracks the current longest prefix-suffix length. If the characters match, extend `len`; otherwise, fall back to `lps[len-1]` (and retry). Edge cases include an empty pattern (return empty vector immediately) and a pattern longer than the text (also return empty). After building LPS, scan the text with index `i` maintaining a `len` that represents how many characters of the pattern match the current suffix of the text. When a match occurs, increment `len`; if `len` equals the pattern length, record the start index (`i - pattern.size() + 1`) and update `len` to `lps[len-1]` for overlapping matches. If a mismatch occurs and `len` > 0, fall back similarly. Time complexity: O(|pattern| + |text|) since both loops advance at most linearly. Space complexity: O(|pattern|) for the LPS array.
