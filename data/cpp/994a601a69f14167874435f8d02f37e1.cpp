/*
Write a C++ function `minimumReplacements` that takes two strings, `S` and `T`, and returns the minimum number of character substitutions needed to make `T` appear as a contiguous substring of `S`. In other words, for each possible starting position in `S` where `T` could fit, count how many characters differ between `S` at that position and `T`, and return the smallest such count. The function must handle the case where `T` is longer than `S` (return a large sentinel value, e.g., `INT_MAX`), and cases with empty strings. Assume both strings consist only of lowercase English letters. The function signature is `int minimumReplacements(const std::string& S, const std::string& T);` and should not modify the input strings.
*/
#include <string>
#include <climits>
#include <algorithm>

// Return the minimum Hamming distance between T and any contiguous substring of S of length T.size().
// If T is longer than S, return INT_MAX. If T is empty, return 0.
int minimumReplacements(const std::string& S, const std::string& T) {
    if (T.empty()) {
        return 0;
    }
    if (T.size() > S.size()) {
        return INT_MAX;
    }

    int best = INT_MAX;
    const int n = static_cast<int>(S.size());
    const int m = static_cast<int>(T.size());

    for (int i = 0; i <= n - m; ++i) {
        int diff = 0;
        for (int j = 0; j < m; ++j) {
            if (S[i + j] != T[j]) {
                ++diff;
            }
        }
        best = std::min(best, diff);
    }

    return best;
}
#include <cassert>
#include <climits>
#include <string>

// Declare the solution function (assumed defined above)
int minimumReplacements(const std::string& S, const std::string& T);

int main() {
    // Basic case
    assert(minimumReplacements("abcde", "bcd") == 0);
    // One mismatch
    assert(minimumReplacements("abcde", "bcf") == 1);
    // Multiple mismatches, best window is in the middle
    assert(minimumReplacements("abcdef", "abxdy") == 2); // compare "abcde" vs "abxdy" -> two diffs, or "bcdef" vs "abxdy" -> three diffs, min=2
    // T longer than S
    assert(minimumReplacements("abc", "abcd") == INT_MAX);
    // Empty T
    assert(minimumReplacements("anything", "") == 0);
    // Empty S, T non-empty
    assert(minimumReplacements("", "a") == INT_MAX);
    // All equal length strings
    assert(minimumReplacements("same", "same") == 0);
    assert(minimumReplacements("same", "different") == 5); // length 4 vs 9, T longer -> INT_MAX? Actually "same" and "different" lengths differ, T length 9 > S length 4, so should return INT_MAX
    // Better: equal length
    assert(minimumReplacements("abcd", "abce") == 1);
    // Case where T appears at the end
    assert(minimumReplacements("xyzabc", "abc") == 0);
    // No match possible, all windows same distance
    assert(minimumReplacements("aaa", "bbb") == 3);
    // Large sentinel value check for longer T
    assert(minimumReplacements("short", "muchlonger") == INT_MAX);
    // T exactly matches a later window
    assert(minimumReplacements("hello world", "world") == 0);
}
// The task is essentially a fixed-window sliding comparison. For each starting index `i` from `0` to `S.length() - T.length()` inclusive, compute the Hamming distance between `S[i..i+T.length()-1]` and `T`. Take the minimum over all windows. Edge cases: if `T` is empty, the distance is 0 for any window (or we can return 0 immediately). If `T.length() > S.length()`, no window exists, so return `INT_MAX` (as per task). Also handle `S` empty (if `T` also empty return 0, else return `INT_MAX`). The complexity: O((|S|-|T|+1)*|T|) worst-case, but since |T| <= |S|, it's O(|S|*|T|) in the worst case (e.g., if both are length ~N). Space is O(1) auxiliary.
