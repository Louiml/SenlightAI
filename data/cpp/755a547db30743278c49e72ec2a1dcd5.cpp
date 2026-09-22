// Write a C++ function named `longestIdealSubsequence` that, given a string `s` consisting of lowercase English letters and an integer `k` (0 ≤ k ≤ 25), returns the length of the longest subsequence of `s` such that the absolute difference between the ASCII values of any two consecutive characters in the subsequence is at most `k`. A subsequence preserves the original order of characters but may skip characters. The function must handle empty strings (return 0), strings of length 1 (return 1), and cases where `k` is 0 (only identical consecutive characters allowed). The solution should be efficient for string lengths up to 10^5.
The core idea is dynamic programming with a twist: instead of a standard O(n²) DP (which would be too slow for 10^5), we maintain the best length of a valid subsequence ending with each character of the alphabet. Process the string left to right. For each character `c` at position `i`, the best subsequence ending with `c` is at least 1 (the character alone). To extend an existing subsequence, we look at all characters `d` such that `abs(c - d) ≤ k`, and take the maximum of `best[d] + 1` over those `d`, where `best[d]` is the best length of any subsequence ending with `d` seen so far. After computing the candidate for `c`, we update `best[c]` to be the maximum of its old value and the newly computed candidate. This works because we only need the best length per ending character, not per position—since future characters only care about the last character of the subsequence, and a longer subsequence ending with the same character is always at least as good as a shorter one. At the end, the answer is the maximum over all `best[c]`. Edge cases: empty string returns 0; when `k=0`, only the same character can extend, so `best` updates naturally. Time complexity is O(n * 26) = O(n), since for each character we loop over at most 26 letters. Space is O(1) (a fixed array of size 26).
#include <string>
#include <vector>
#include <algorithm>

// Returns the length of the longest subsequence where consecutive characters differ by at most k.
int longestIdealSubsequence(const std::string& s, int k) {
    if (s.empty()) return 0;
    
    std::vector<int> best(26, 0); // best[c] = longest subsequence ending with character 'a'+c
    
    int ans = 0;
    for (char ch : s) {
        int idx = ch - 'a';
        int current = 1; // Start with just this character
        // Check all characters within absolute difference <= k
        int lo = std::max(0, idx - k);
        int hi = std::min(25, idx + k);
        for (int d = lo; d <= hi; ++d) {
            current = std::max(current, best[d] + 1);
        }
        best[idx] = std::max(best[idx], current);
        ans = std::max(ans, current);
    }
    return ans;
}
#include <cassert>
#include <string>

int longestIdealSubsequence(const std::string& s, int k); // declaration from solution

int main() {
    // Basic cases
    assert(longestIdealSubsequence("", 0) == 0);
    assert(longestIdealSubsequence("a", 5) == 1);
    assert(longestIdealSubsequence("acf", 2) == 2); // "ac" or "cf" (diff 2 and 3? actually c-f diff 3 >2, but a-c diff 2 -> length 2)
    assert(longestIdealSubsequence("acf", 3) == 3); // a-c-f: diffs 2,3 => all valid
    assert(longestIdealSubsequence("abc", 1) == 2); // "ab" or "bc"
    assert(longestIdealSubsequence("abc", 0) == 1); // no adjacent identical, so any single char
    assert(longestIdealSubsequence("aaabbb", 1) == 6); // aaa can extend to bbb? diff 1, so a-b diff 1, so "aaabbb" all valid -> 6
    assert(longestIdealSubsequence("bxyz", 2) == 3); // "bxy" or "xyz" (diff b-x=22>2, so start with xyz -> len 3)
    assert(longestIdealSubsequence("edcba", 4) == 5); // all pairs differ <=4 -> entire string
    assert(longestIdealSubsequence("az", 25) == 2); // diff 25 <=25 -> whole string
    assert(longestIdealSubsequence("az", 24) == 1); // diff 25 >24 -> only one
    return 0;
}
