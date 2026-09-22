Write a C++ function `int maximumGain(const std::string& s, int x, int y)` that computes the maximum total score obtainable by repeatedly removing occurrences of the substrings `"ab"` and `"ba"` from the given string `s`. Each removal of `"ab"` earns `x` points, and each removal of `"ba"` earns `y` points. You may remove substrings in any order, and after each removal, the remaining characters concatenate, potentially creating new removable substrings. The function should return the maximum possible total score. The input string consists only of lowercase letters `'a'` and `'b'` (but may also contain other characters which are never part of removable substrings and act as barriers). The values `x` and `y` are non-negative integers.

#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(maximumGain("ab", 1, 1) == 1);
    assert(maximumGain("ba", 1, 1) == 1);
    assert(maximumGain("abab", 2, 1) == 4);  // Remove both "ab"s
    assert(maximumGain("baba", 1, 2) == 4);  // Remove both "ba"s

    // Mixed order and priority
    assert(maximumGain("abba", 3, 2) == 5);  // Remove "ab" then "ba"
    assert(maximumGain("abba", 2, 3) == 5);  // Remove "ba" first then "ab"

    // No removable pairs
    assert(maximumGain("", 5, 5) == 0);
    assert(maximumGain("abc", 10, 10) == 0);  // 'c' is a barrier
    assert(maximumGain("aa", 7, 7) == 0);

    // Zero gain values
    assert(maximumGain("ab", 0, 5) == 0);  // Remove "ab" yields 0, but "ba" none
    assert(maximumGain("ba", 5, 0) == 0);

    // Larger string with barriers and multiple pairs
    assert(maximumGain("aabb", 3, 2) == 6);  // Two "ab" pairs (positions 1-2 and 3-4)
    assert(maximumGain("bbaa", 2, 3) == 6);  // Two "ba" pairs
    assert(maximumGain("abbaab", 4, 3) == 14); // Optimal: remove "ab" three times (3*4=12) plus one "ba" (3)
    // The string "abbaab": indices 0-1 "ab"->4, leaves "baab"; 0-1 "ba"->3, leaves "ab"; 0-1 "ab"->4, total 11? Let's verify:
    // Actually better order: remove "ab" at 0-1 (4) -> "baab"; then remove "ab" at 2-3 (4) -> "ba"; then remove "ba" (3) -> total 11? But we can also remove "ba" first? Let's just test consistency.

    // Known result from brute force for "abbaab", x=4,y=3: maximum is 14? Let's reason: 
    // Remove "ab" at positions 0-1 (4) -> "baab"; remove "ab" at pos 2-3 (4) -> "ba"; remove "ba" (3) total 11.
    // Alternative: remove "ba" at pos 1-2 (3) -> "abab"; remove "ab" at 0-1 (4) -> "ab"; remove "ab" (4) total 11.
    // Actually maximum might be 11. Let's correct assert to 11.
    assert(maximumGain("abbaab", 4, 3) == 11);

    return 0;
}

#include <string>
#include <algorithm>

// Helper that removes pairs of type (c1, c2) from s, gaining 'gain' per pair.
// Returns the new string without those pairs.
std::string removePairs(const std::string& s, char c1, char c2, int gain, int& total) {
    std::string result;
    for (char ch : s) {
        if (!result.empty() && result.back() == c1 && ch == c2) {
            total += gain;
            result.pop_back();
        } else {
            result.push_back(ch);
        }
    }
    return result;
}

// Compute maximum total gain by removing "ab" and "ba" substrings.
int maximumGain(const std::string& s, int x, int y) {
    int total = 0;
    std::string current = s;

    if (x >= y) {
        // First remove all "ab" (c1='a', c2='b') because x is higher or equal.
        current = removePairs(current, 'a', 'b', x, total);
        // Then remove all "ba" (c1='b', c2='a') from the remaining string.
        current = removePairs(current, 'b', 'a', y, total);
    } else {
        // First remove all "ba" (c1='b', c2='a') because y is higher.
        current = removePairs(current, 'b', 'a', y, total);
        // Then remove all "ab" (c1='a', c2='b') from the remaining string.
        current = removePairs(current, 'a', 'b', x, total);
    }

    return total;
}

// The key insight is that the order of removals affects the total score. To maximize the gain, always prioritize removing the higher-valued substring first. For example, if `x > y`, first remove all possible `"ab"` pairs (left to right), then remove `"ba"` pairs from the remaining string. This greedy strategy is optimal because removing a lower-valued pair first could destroy a higher-valued adjacency that would have been formed later, and the higher-valued removal always yields at least as much benefit per operation.
//
// A recursive approach works:  
// - If `x >= y`, call a helper that scans the string and removes `"ab"` pairs by using a stack-like buffer. When the current character is `'a'` and the top of the buffer is `'b'`, we gain `x` and pop; otherwise push the character. Characters other than `'a'` and `'b'` are simply pushed (they never form a pair and act as separators).
// - After finishing this pass, we have a new string (the buffer contents) that contains no `"ab"` if `x >= y` was chosen. Then we recursively handle the remaining `"ba"` pairs but with the roles swapped, because the next pass should remove `"ba"` (now the higher priority in the remaining string). We keep a counter to avoid infinite recursion: the recursion terminates after two passes (one for each pair type), because after removing the higher-priority pair, the lower-priority pair is handled next, and after that no more pairs can exist (since the first pass removed all of the first type, and the second removes all of the second type).
//
// Complexity: Each pass processes each character once, and we do at most two passes (since we alternate). So time is O(n) where n is string length, and space is O(n) for the buffer (the accumulated string). In practice, the recursion depth is at most 2, so stack space is O(n) due to the temporary string copy.
//
// Edge cases: Empty string, string with no removable pairs, characters other than 'a'/'b' (they act as barriers and are never removed), and cases where x or y is zero. If x == y, either order works, and the total gain is just the number of pairs times the common value, but the algorithm still works by choosing one priority first.
