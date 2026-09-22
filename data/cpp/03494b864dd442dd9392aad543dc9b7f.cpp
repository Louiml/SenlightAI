/*
Given a string `s` consisting of lowercase English letters, write a C++ function `int countAfterOneSwap(const std::string& s)` that returns the number of distinct strings that can be obtained by performing **exactly one swap** of two characters in `s`. A swap must involve two distinct positions. If the resulting string is identical to the original (e.g., swapping two equal characters), it still counts once. However, if multiple swaps produce the same resulting string, count that string only once. For example, for `"ab"`, the swaps are `(0,1)` → `"ba"`, so answer = 1. For `"aa"`, the only swap `(0,1)` produces `"aa"`, so answer = 1. For `"aab"`, all possible swaps: `(0,1)`→`"aab"`, `(0,2)`→`"baa"`, `(1,2)`→`"aba"`; distinct results are `"aab"`, `"baa"`, `"aba"`, so answer = 3. The function must handle strings of length 1 (answer = 0, since at least two positions are needed) and length up to \(10^5\). Assume input contains only lowercase letters.
*/

#include <string>
#include <vector>

// Count distinct strings obtainable by exactly one swap of two characters.
// Two positions must be distinct. If swapping equal characters yields original, count it once.
int countAfterOneSwap(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n < 2) return 0;  // No swap possible

    // Frequency of each lowercase letter
    std::vector<int> freq(26, 0);
    for (char ch : s) {
        ++freq[ch - 'a'];
    }

    // Total number of unordered pairs of positions
    long long total_pairs = 1LL * n * (n - 1) / 2;
    long long same_pairs = 0;
    bool has_duplicate = false;

    for (int f : freq) {
        if (f >= 2) {
            has_duplicate = true;
            same_pairs += 1LL * f * (f - 1) / 2;
        }
    }

    long long different_pairs = total_pairs - same_pairs;
    // Each different-character swap gives a unique string; equal-character swap yields original.
    return static_cast<int>(different_pairs + (has_duplicate ? 1 : 0));
}

#include <cassert>
#include <string>

int countAfterOneSwap(const std::string& s);

int main() {
    // Test 1: two different characters
    assert(countAfterOneSwap("ab") == 1);
    // Test 2: two same characters
    assert(countAfterOneSwap("aa") == 1);
    // Test 3: single character - no swap possible
    assert(countAfterOneSwap("a") == 0);
    // Test 4: all distinct characters
    assert(countAfterOneSwap("abc") == 3);
    // Test 5: has duplicate and distinct characters
    assert(countAfterOneSwap("aab") == 3);
    // Test 6: string with repeated character, no duplicates of others
    assert(countAfterOneSwap("abca") == 5);
    // Test 7: all same characters
    assert(countAfterOneSwap("aaaa") == 1);
    // Test 8: string with all distinct, length 4
    assert(countAfterOneSwap("wxyz") == 6);
    // Test 9: mixed duplicates: "aabb" → total pairs=6, same pairs: a:1, b:1 → 2, different=4, has dup → +1 =5
    assert(countAfterOneSwap("aabb") == 5);
    // Test 10: longer string with repeated characters
    assert(countAfterOneSwap("abcabc") == 15);
    return 0;
}

// The goal is to count distinct strings obtainable by exactly one swap. A naive approach would generate all \(\binom{n}{2}\) swaps, but that is \(O(n^2)\) and infeasible for \(n=10^5\). The key observation: each swap picks two positions \(i<j\). If the characters at those positions are equal, the resulting string is identical to the original; all such swaps produce the same string (the original). If they are different, swapping them produces a new string, and two different pairs of positions yield the same resulting string if and only if they swap the same two distinct characters (regardless of positions). For example, in `"abca"`, swapping positions (0,1) gives `"baca"` and swapping (0,3) gives `"abca"` (original) because both are 'a', but swapping (0,1) and (1,3) both give `"baca"`? Actually (1,3) swaps 'b' and 'a' → `"aabc"`, no. Let's reason systematically: For each unordered pair of distinct characters \(c_1 \neq c_2\), exactly one distinct resulting string is obtained by swapping any occurrence of \(c_1\) with any occurrence of \(c_2\). That string is characterized by the multiset of characters (same as original) and the positions of the swapped characters; but since we swap one of each, the result is uniquely determined by the two characters and the fact that we swap them (positions don't matter for the resulting string because swapping positions of two different characters always yields the same multiset of characters at those two positions? Actually if there are multiple occurrences of \(c_1\) and \(c_2\), swapping position of first \(c_1\) with first \(c_2\) vs second \(c_1\) with second \(c_2\) yields the same string? Example `"abab"`: swap positions (0,1) → `"baab"`; swap (0,3) → `"baba"` (different). So positions do matter. The correct approach: Count pairs \((i,j)\) with \(i<j\) and \(s[i] \neq s[j]\). Then add 1 if there exists at least one pair of equal characters (so that swapping them produces the original string once). But is that correct? Consider `"aba"`: pairs with different chars: (0,1) → `"baa"`, (0,2) swaps 'a' and 'a' (same) → original `"aba"`, (1,2) → `"aab"`. So different-char pairs: (0,1) and (1,2) → 2 strings, plus original because equal pair exists → total 3. That matches. For `"ab"`: only pair (0,1) different → 1, no equal pair → total 1. For `"aa"`: no different pairs, has equal pair → total 1. For `"abc"`: different pairs: (0,1),(0,2),(1,2) → 3 strings, no equal pair → total 3. For `"aab"`: different pairs: (0,1) and (1,2) → 2 strings, plus original because equal pair exists (positions 0 and 2 both 'a') → total 3. Yes, the formula is: number of pairs \(i<j\) with \(s[i] \neq s[j]\) + (1 if there exists any character that appears at least twice, else 0). Because swapping two equal characters gives the original string, and that is the only way to get the original string (swapping different characters always changes the string). And distinct strings from different-character swaps are all unique because if two different swaps of different-character pairs produced the same string, that would require swapping the same two characters at the same two positions, which is the same pair. So count of distinct strings = number of unordered pairs of positions with different characters + indicator of having a duplicate character. To compute efficiently: total pairs = \(\binom{n}{2}\). For each character, let freq[c] be its count. Pairs with same character = \(\sum_c \binom{freq[c]}{2}\). Then pairs with different characters = total pairs - sum of same-character pairs. Then add 1 if any freq[c] >= 2. Time: O(n + alphabet) = O(n), space O(1) for alphabet size 26. Edge cases: length 1 → 0 (no swaps possible; the formula gives total pairs=0, different=0, no duplicate → 0). Length 2 with same char → total pairs=1, same pair=1, different=0, has duplicate → 1. Length 2 with different → total pairs=1, same=0, different=1, no duplicate → 1. All good.
