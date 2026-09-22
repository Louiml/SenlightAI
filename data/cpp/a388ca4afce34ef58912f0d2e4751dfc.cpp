// Write a C++ function `findPatternMatches` that takes two `std::string` parameters (the text and the pattern) and returns a `std::vector<int>` containing all starting indices (0‑based) where the pattern occurs in the text. The function must work correctly with overlapping matches (e.g., pattern “AAA” in text “AAAA” should return indices 0 and 1). The text may be empty, in which case the function returns an empty vector. The pattern may be empty, in which case the function should treat it as matching at every position from 0 up to the length of the text (inclusive of the end), i.e., return all indices `0..text.size()`. Handle inputs that cause worst‑case quadratic behavior in naive matching (e.g., pattern `"AAAB"` in text `"AAAAAAAAAAB"`) efficiently enough for typical test sizes (text length up to 10⁵, pattern length up to 10⁴) by using the Knuth–Morris–Pratt (KMP) algorithm. Do not use built‑in `std::string::find` or other pattern‑searching library functions for the core algorithm; you may use them only for validation in tests.

The main algorithm is KMP (Knuth–Morris–Pratt). Build a prefix function (longest proper prefix which is also a suffix) for the pattern. Then scan the text, maintaining a state `q` representing the length of the current matched prefix. For each character `ch` in the text, while `q > 0` and `pattern[q] != ch`, set `q = prefix[q-1]` (fallback). If `pattern[q] == ch`, increment `q`. If `q == m` (pattern length), a match is found ending at the current position; the starting index is `i - m + 1`, then set `q = prefix[q-1]` to allow overlapping matches.  

Edge cases:  
- Empty pattern: return all indices from 0 to `text.size()` inclusive (i.e., the empty pattern is found at every position, including after the last character). In implementation, initialize `m=0` and handle separately.  
- Text empty and pattern non‑empty: return empty vector.  
- Pattern longer than text: return empty vector.  
- Overlapping matches: handled by the fallback after each match.  

Complexity: O(n + m) time, O(m) auxiliary space for the prefix array. This is a dramatic improvement over the naive O(n·m) worst case.

#include <vector>
#include <string>

// Compute the prefix function (longest proper prefix which is also a suffix) for pattern.
std::vector<int> buildPrefixFunction(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> prefix(m, 0);
    int j = 0;
    for (int i = 1; i < m; ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = prefix[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            ++j;
        }
        prefix[i] = j;
    }
    return prefix;
}

// Returns all start indices (0‑based) where pattern occurs in text
// using the Knuth–Morris–Pratt algorithm. Handles overlapping matches.
std::vector<int> findPatternMatches(const std::string& text, const std::string& pattern) {
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    if (m == 0) {
        // Empty pattern matches at every position, including at the end.
        std::vector<int> allPositions;
        allPositions.reserve(n + 1);
        for (int i = 0; i <= n; ++i) {
            allPositions.push_back(i);
        }
        return allPositions;
    }

    if (m > n) {
        return {};
    }

    std::vector<int> prefix = buildPrefixFunction(pattern);
    std::vector<int> matches;
    matches.reserve(n / m + 1);

    int q = 0; // length of current matched prefix
    for (int i = 0; i < n; ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = prefix[q - 1];
        }
        if (pattern[q] == text[i]) {
            ++q;
        }
        if (q == m) {
            matches.push_back(i - m + 1);
            q = prefix[q - 1];
        }
    }
    return matches;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution code here (or link it) before the main function.

int main() {
    // Basic test with multiple non‑overlapping matches
    assert(findPatternMatches("abcabcabc", "abc") == std::vector<int>({0, 3, 6}));

    // Overlapping matches
    assert(findPatternMatches("AAAA", "AAA") == std::vector<int>({0, 1}));

    // Pattern not found
    assert(findPatternMatches("hello world", "xyz").empty());

    // Pattern longer than text
    assert(findPatternMatches("abc", "abcd").empty());

    // Empty text with non‑empty pattern
    assert(findPatternMatches("", "a").empty());

    // Empty pattern: should match at all positions 0 to text.size()
    assert(findPatternMatches("abc", "") == std::vector<int>({0, 1, 2, 3}));
    assert(findPatternMatches("", "") == std::vector<int>({0}));

    // Classic worst‑case for naive algorithm
    assert(findPatternMatches("AAAAAAAAAAB", "AAAB") == std::vector<int>({6}));

    // Repeated pattern with overlapping occurrence at end
    std::string text = "abababab";
    std::string pat = "aba";
    assert(findPatternMatches(text, pat) == std::vector<int>({0, 2, 4}));

    // Pattern at the very end of text
    assert(findPatternMatches("xxxy", "xy") == std::vector<int>({2}));

    // Single character pattern
    std::vector<int> single = findPatternMatches("aabaa", "a");
    assert(single == std::vector<int>({0, 1, 3, 4}));

    // Large test to ensure O(n+m) behavior (quick sanity, not exhaustive)
    std::string bigText(100000, 'A');
    std::string bigPat(5000, 'A');
    std::vector<int> bigMatches = findPatternMatches(bigText, bigPat);
    assert(bigMatches.size() == 100000 - 5000 + 1); // every start position
    assert(bigMatches.front() == 0);
    assert(bigMatches.back() == 95000);

    return 0;
}
