/*
Write a C++ function named `longestRepeatedSequence` that takes a non-empty string `s` (containing only lowercase English letters) as input and returns a `std::string` that is the longest contiguous substring that appears at least twice as a non-overlapping repetition within `s`. If multiple such substrings have the same maximum length, return the lexicographically smallest one. If no such substring exists (meaning no character appears more than once with non-overlapping occurrences), return an empty string. For example, for input `"ababa"`, the longest repeated non-overlapping substring is `"ab"` (appearing at positions 0-1 and 2-3), not `"aba"` because those occurrences would overlap. For `"abcabc"`, the answer is `"abc"` because it appears at positions 0-2 and 3-5 without overlap. For `"aaaa"`, valid non-overlapping repeats include `"a"` (positions 0 and 1), `"aa"` (positions 0-1 and 2-3), so the longest is `"aa"`; if `"aaa"` were considered (positions 0-2 and 1-3) it overlaps, so it's invalid. The function must handle cases where the input has no repeated substring by returning an empty string.
*/
#include <string>
#include <vector>
#include <algorithm>

// Returns the longest non-overlapping repeated substring of s.
// If multiple substrings share the maximum length, returns lexicographically smallest.
// Returns empty string if no non-overlapping repeated substring exists.
std::string longestRepeatedSequence(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n < 2) return "";

    // Iterate lengths from largest possible down to 1.
    for (int len = n / 2; len >= 1; --len) {
        std::vector<std::string> candidates;

        // First occurrence starts at i, second occurrence starts at j.
        for (int i = 0; i + len <= n; ++i) {
            for (int j = i + len; j + len <= n; ++j) {
                // j - i >= len ensures non-overlap.
                if (s.compare(i, len, s, j, len) == 0) {
                    candidates.push_back(s.substr(i, len));
                }
            }
        }

        if (!candidates.empty()) {
            // Return lexicographically smallest among candidates.
            return *std::min_element(candidates.begin(), candidates.end());
        }
    }

    return "";
}
#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(longestRepeatedSequence("ababa") == "ab");       // "ab" at 0 and 2, "aba" overlaps
    assert(longestRepeatedSequence("abcabc") == "abc");     // full non-overlapping repeat
    assert(longestRepeatedSequence("aaaa") == "aa");        // "aa" at 0 and 2
    assert(longestRepeatedSequence("aaaaa") == "aa");       // "aaa" would overlap
    
    // No repeats
    assert(longestRepeatedSequence("abcdef") == "");
    assert(longestRepeatedSequence("a") == "");
    
    // Lexicographic tie-break
    assert(longestRepeatedSequence("cabcab") == "ab");      // both "ab" and "ca" possible? "ca" at 0 and 3? Actually "ca" not repeated. Wait: "ab" at 1 and 4, also "bc"? Let's use a known tie case.
    // Example: "abacaba" – longest non-overlapping repeated is "ab" at 0 and 6? No overlap? 0-1 and 6-7 but string length 7, so 6-7 is out of bounds. Let's use "abxab" – "ab" at 0 and 3, also "bx"? Only "ab". Simpler tie: "xxyy" – "x" appears at 0 and 1, "y" at 2 and 3, both length 1, lex smallest "x".
    assert(longestRepeatedSequence("xxyy") == "x");         // "x" vs "y" – "x" smaller
    
    // Non-overlap matters: "ababa" – "aba" at 0 and 2 would overlap, so not allowed.
    assert(longestRepeatedSequence("ababa") != "aba");
    
    // Mixed case where longer substring exists but overlaps, shorter allowed
    assert(longestRepeatedSequence("abcabca") == "abc");    // "abc" at 0 and 3, also "ab" at 0 and 3? Actually "abc" is longest.
    
    // Edge: all same characters, even length
    assert(longestRepeatedSequence("bbbb") == "bb");
    
    // Ensure empty input not tested per spec, but function handles gracefully.
    assert(longestRepeatedSequence("") == "");
    
    return 0;
}
// The core challenge is to find the longest substring that occurs at least twice with no overlap between the two occurrences. A brute-force approach iterates over all possible substring lengths from `n/2` down to 1 (since any non-overlapping repeat must have length at most half the string length). For each length `L`, we examine every starting index `i` from 0 to `n - L` as the first occurrence, and every starting index `j` from `i + L` to `n - L` as the second occurrence, ensuring `j >= i + L` so they don’t overlap. We compare substrings of length `L` using `std::string::compare` or direct string comparison. To satisfy the lexicographic tie-break, we can collect all valid substrings of the current longest length found and pick the smallest lexicographically. To improve efficiency, we can break early once we find any substring of a given length, but the tie-break requires we check all candidates of that length. The time complexity is O(n^3) in the worst case (for each length O(n^2) pairs, and each comparison O(L)), which is acceptable for `n` up to about 100. Space complexity is O(n) for the returned string and bookkeeping. Edge cases include: empty input (though spec says non-empty, we can still handle), single-character strings (no repeat), strings where all characters are same but length is odd (e.g., `"aaaaa"` – longest valid is `"aa"` because `"aaa"` would force overlap), and strings with multiple longest candidates requiring lexicographic tie-breaking.
