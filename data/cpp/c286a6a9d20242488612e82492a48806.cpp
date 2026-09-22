// Given a string `s` consisting of `'G'` and `'S'` characters of length `n`, write a C++ function `longestGWithOneS(const std::string& s)` that returns the length of the longest contiguous substring that contains at most one `'S'`. The function must handle the case where the string contains only `'S'` (return 0) or only `'G'` (return `n`). You may assume `n ≥ 1`. The solution should avoid using global variables and must be self-contained.

#include <cassert>
#include <string>

// Function declaration (matching the solution above)
int longestGWithOneS(const std::string& s);

int main() {
    assert(longestGWithOneS("GGG") == 3);           // all G
    assert(longestGWithOneS("SSS") == 0);           // all S -> return 0 per spec
    assert(longestGWithOneS("G") == 1);
    assert(longestGWithOneS("S") == 0);
    assert(longestGWithOneS("GSG") == 3);           // can include one S
    assert(longestGWithOneS("GGSSG") == 3);         // window "GGS" or "SSG"? Actually "GGS"=3,"
    assert(longestGWithOneS("SGGG") == 3);          // "GGG" length 3
    assert(longestGWithOneS("GGGSSS") == 3);        // "GGG" length 3
    assert(longestGWithOneS("GSSG") == 2);          // "GS" or "SG" length 2
    assert(longestGWithOneS("GSSGG") == 3);         // "SGG" or "GSS"? "SGG" length 3
    return 0;
}

#include <string>
#include <algorithm>

// Returns the length of the longest contiguous substring of s containing at most one 'S'.
// s consists only of characters 'G' and 'S'.
int longestGWithOneS(const std::string& s) {
    int n = static_cast<int>(s.size());
    int left = 0;
    int sCount = 0;
    int best = 0;

    for (int right = 0; right < n; ++right) {
        if (s[right] == 'S') {
            ++sCount;
        }
        while (sCount > 1) {
            if (s[left] == 'S') {
                --sCount;
            }
            ++left;
        }
        best = std::max(best, right - left + 1);
    }

    // Special case: if all characters are 'S', best would be 1,
    // but the problem asks to return 0 in that situation.
    // Detect if the string has no 'G' at all.
    bool hasG = false;
    for (char c : s) {
        if (c == 'G') {
            hasG = true;
            break;
        }
    }
    if (!hasG) return 0;
    return best;
}

// The problem is equivalent to finding the maximum length of a subarray of `'G'`s that can include at most one `'S'`. A direct sliding window approach works: maintain two pointers `left` and `right`, and a counter of `'S'` in the current window. Expand `right` one character at a time; if the count of `'S'` exceeds 1, move `left` rightward until the count is back to ≤1. Track the maximum window length. This yields O(n) time and O(1) auxiliary space. Edge cases: when the string has no `'S'`, the entire string qualifies; when it has only `'S'`, the result is 0 (the longest substring with at most one `'S'` is a single `'S'`, but the problem specifies return 0 for all-`'S'`; for mixed strings the window handles it naturally). The function uses const correctness and no external state.
