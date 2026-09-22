/*
Write a C++ function `lexicographicallySmallestByLocalSwap(const std::string& s, const std::string& target)` that takes two lowercase alphabetic strings `s` and `target`. The function must return the lexicographically smallest string that can be obtained from `s` by performing **at most one swap** of any two characters at different positions (i.e., you may either leave `s` unchanged or swap exactly one pair of distinct indices). If that resulting string is lexicographically strictly smaller than `target`, return it; otherwise, return the string `"---"`. The function must handle empty strings gracefully (if `s` is empty, return `""` if `target` is also empty; otherwise return `"---"` since an empty string is never strictly smaller than a non-empty target). Assume all strings consist only of lowercase English letters. The goal is to find the best possible single-swap result (or the original if no swap helps) and compare it correctly.
*/

#include <string>
#include <algorithm>

// Returns the lexicographically smallest string obtainable by at most one swap
// of two different positions, but only if it is strictly smaller than target.
// Otherwise, returns "---". If s is empty, returns "" only if target is also empty,
// else "---".
std::string lexicographicallySmallestByLocalSwap(const std::string& s, const std::string& target) {
    if (s.empty()) {
        return target.empty() ? "" : "---";
    }
    
    std::string best = s; // candidate: original string (zero swaps)
    std::string sorted = s;
    std::sort(sorted.begin(), sorted.end());
    
    // Find first position where s differs from its sorted version.
    int i = 0;
    while (i < static_cast<int>(s.size()) && s[i] == sorted[i]) {
        ++i;
    }
    
    // If a mismatch exists, performing a swap at that position yields the best
    // possible result (swapping s[i] with the last occurrence of sorted[i]).
    if (i < static_cast<int>(s.size())) {
        std::string swapped = s;
        std::iter_swap(swapped.begin() + i, swapped.begin() + swapped.find_last_of(sorted[i]));
        best = swapped;
    }
    
    return (best < target) ? best : "---";
}

#include <cassert>
#include <string>

// The solution function is declared above; this main tests it.
int main() {
    // Basic cases
    assert(lexicographicallySmallestByLocalSwap("abc", "abd") == "abc"); // already smallest, less than target
    assert(lexicographicallySmallestByLocalSwap("cba", "zzz") == "abc"); // one swap to sorted
    assert(lexicographicallySmallestByLocalSwap("cba", "abc") == "---"); // best is "abc", not strictly less than "abc"
    
    // Duplicate characters: swap with last occurrence
    assert(lexicographicallySmallestByLocalSwap("abac", "abca") == "aabc"); // swap index 1 ('b') with last 'a' at index 2 -> "aabc"
    assert(lexicographicallySmallestByLocalSwap("aa", "ab") == "aa"); // already sorted, no improvement
    
    // Empty string cases
    assert(lexicographicallySmallestByLocalSwap("", "") == "");
    assert(lexicographicallySmallestByLocalSwap("", "a") == "---");
    
    // No swap helps (original is already the best but not less than target)
    assert(lexicographicallySmallestByLocalSwap("ab", "aa") == "---"); // best is "ab", not < "aa"
    
    // Larger example
    assert(lexicographicallySmallestByLocalSwap("bdca", "c") == "abcd"); // swap to sorted "abcd", which is < "c"
    assert(lexicographicallySmallestByLocalSwap("abcd", "abce") == "abcd"); // original is best and < target
    
    // Edge case: all same characters
    assert(lexicographicallySmallestByLocalSwap("aaa", "aab") == "aaa"); // no improvement possible
    
    // Target very small, must return "---"
    assert(lexicographicallySmallestByLocalSwap("z", "a") == "---"); // "z" > "a", no swap helps
    
    return 0;
}

// The key idea is to find the position `i` where the original string differs from its sorted version (the lexicographically smallest possible arrangement of its characters). If such a position exists, then the optimal single swap is to place the smallest possible character that can come to that position while keeping the prefix identical to the sorted prefix. Specifically, after sorting a copy of `s`, locate the first index `i` where `s[i] != sorted[i]`. Then swap `s[i]` with the **last** occurrence of `sorted[i]` in `s` (using `find_last_of`) because swapping with the last occurrence ensures the suffix after the swap is as lexicographically small as possible after moving that character forward. For example, in `"abcab"`, sorted is `"aabbc"`, first mismatch at `i=1` (`s[1]='b'` vs `'a'`), and the last `'a'` is at index 3; swapping yields `"aacbb"`, which is the best achievable with one swap. If `s` is already sorted (no mismatch), then no swap can improve it, so the best candidate is the original string. Edge cases include duplicates: using `find_last_of` guarantees we take the rightmost occurrence to minimize the suffix. Also, if `s` is empty, the result is trivially `""` or `"---"` depending on `target`. Time complexity is O(n) for the swap plus O(n log n) for sorting, or we could do a linear-time scan to find the first mismatch using the sorted string; total O(n log n) due to sorting. Space complexity is O(n) for the temporary sorted string. Important: the comparison `s < target` is lexicographic, and we must return `"---"` if the best candidate is not strictly smaller. The function should be `const` correct and avoid modifying the input.
