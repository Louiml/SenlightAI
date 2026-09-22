// Write a C++ function that, given a non-empty pattern string and a non-empty text string, returns a `std::vector<int>` containing the starting indices (0‑based) of all non-overlapping occurrences of the pattern in the text, using the Knuth–Morris–Pratt (KMP) algorithm. Overlapping occurrences should be detected as well (i.e., after a match, the search resumes using the longest proper prefix that is also a suffix of the matched portion), so all valid starting positions must be reported. The function must be named `findPatternOccurrences` and must accept the pattern and text as `const std::string&` parameters.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above; here we test it.

int main() {
    // Basic case with non-overlapping matches
    std::vector<int> res1 = findPatternOccurrences("aaba", "aabaacaadaabaaba");
    assert(res1 == std::vector<int>({0, 9, 12}));

    // Overlapping matches
    std::vector<int> res2 = findPatternOccurrences("aaa", "aaaa");
    assert(res2 == std::vector<int>({0, 1}));

    // Pattern appears at the end
    std::vector<int> res3 = findPatternOccurrences("abc", "xxabc");
    assert(res3 == std::vector<int>({2}));

    // Pattern not present
    std::vector<int> res4 = findPatternOccurrences("xyz", "abcabc");
    assert(res4.empty());

    // Pattern longer than text
    std::vector<int> res5 = findPatternOccurrences("longer", "short");
    assert(res5.empty());

    // Pattern with repeated characters and overlapping
    std::vector<int> res6 = findPatternOccurrences("aba", "ababa");
    assert(res6 == std::vector<int>({0, 2}));

    // Single character pattern
    std::vector<int> res7 = findPatternOccurrences("a", "banana");
    assert(res7 == std::vector<int>({1, 3, 5}));

    // Exact match
    std::vector<int> res8 = findPatternOccurrences("same", "same");
    assert(res8 == std::vector<int>({0}));

    // Pattern appears multiple times with gaps
    std::vector<int> res9 = findPatternOccurrences("ab", "abxabyab");
    assert(res9 == std::vector<int>({0, 3, 6}));

    // Empty pattern should yield no matches
    std::vector<int> res10 = findPatternOccurrences("", "text");
    assert(res10.empty());
}
#include <vector>
#include <string>

// Build the longest proper prefix which is also a suffix (LPS) array for the pattern.
void buildLps(const std::string& pattern, std::vector<int>& lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < pattern.length()) {
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
}

// Return all starting indices (0‑based) where pattern occurs in text using KMP.
std::vector<int> findPatternOccurrences(const std::string& pattern, const std::string& text) {
    std::vector<int> result;
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || m > n) {
        return result;
    }

    std::vector<int> lps(m);
    buildLps(pattern, lps);

    int i = 0; // index for text
    int j = 0; // index for pattern
    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
                result.push_back(i - j);
                j = lps[j - 1];
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return result;
}
// The solution uses the KMP preprocessing step to build the LPS (longest proper prefix which is also a suffix) array for the pattern. This array allows the search phase to avoid re‑comparing characters that have already been matched when a mismatch occurs. During the search, two indices `i` (text) and `j` (pattern) are advanced. Whenever `text[i] == pattern[j]`, both are incremented; when `j` reaches the pattern length, a match is recorded at `i - j`, and `j` is set to `lps[j-1]` to allow overlapping matches. On a mismatch, if `j != 0`, `j` is updated to `lps[j-1]`; otherwise `i` is incremented. Edge cases include a pattern longer than the text (no matches), patterns with repeated characters (e.g., "aaa" in "aaaa" must yield indices 0 and 1), and patterns where the LPS vector must be correctly initialized. The time complexity is O(n + m) for preprocessing and searching, where n is the text length and m is the pattern length; auxiliary space is O(m) for the LPS array.
