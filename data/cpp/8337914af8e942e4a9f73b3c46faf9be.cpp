Write a C++ function that takes a string `s` consisting of only lowercase English letters and returns the minimum number of cuts needed to partition the string into substrings such that every substring is a palindrome. A palindrome is a string that reads the same forwards and backwards. For example, the string `"aab"` can be partitioned as `"aa"` and `"b"` with 1 cut, returning 1, while `"aba"` is already a palindrome and requires 0 cuts. The function should handle strings of length from 1 to 1000, and if the string is empty, return 0. The solution must be efficient enough to handle the upper bound within typical time limits.

// The core problem is to find the minimum number of cuts such that every resulting substring is a palindrome. A direct recursive approach checks all possible cut positions: for each starting index `i`, try every end index `j` from `i` to `n-1`; if the substring `s[i..j]` is a palindrome, then the cost is 1 (the cut after `j`) plus the optimal cost for the remaining suffix starting at `j+1`. The base case is when `i == n`, meaning no more characters, so cost is 0. The naive recursion has overlapping subproblems, so memoization or dynamic programming is used. For each index `i`, we compute the minimum cuts needed for the suffix starting at `i`. The recurrence is `dp[i] = min over j from i to n-1 where isPalindrome(i,j) of (1 + dp[j+1])`. To avoid an extra cut at the very end (since partitioning the whole string as one palindrome requires 0 cuts, not 1), we subtract 1 from `dp[0]` if the entire string is a palindrome or if multiple segments exist—actually the recurrence always adds 1 for the first segment, but the final segment after the last cut shouldn't be counted as a cut. Since we add 1 for each segment except the last, the total number of cuts is `dp[0] - 1`. Edge cases: empty string returns 0; a string that is already a palindrome returns 0 (because `dp[0]` would be 1, minus 1 equals 0); single characters are palindromes, so a string of length n always has at most n-1 cuts. Time complexity is O(n^3) in the basic version, but by precomputing palindrome tables or using expand-around-center, we can reduce palindrome checking to O(n^2) total, leading to O(n^2) overall DP. For this task, we will implement the DP with O(n^2) palindrome precomputation, giving O(n^2) time and O(n^2) space for the palindrome table.

#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of cuts needed to partition s into palindromic substrings.
int minPalindromePartition(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n <= 1) return 0;

    // palindrome[i][j] is true if s[i..j] is a palindrome
    std::vector<std::vector<bool>> palindrome(n, std::vector<bool>(n, false));

    // All substrings of length 1 are palindromes
    for (int i = 0; i < n; ++i) palindrome[i][i] = true;

    // Check substrings of length 2 and above
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j]) {
                if (len == 2) {
                    palindrome[i][j] = true;
                } else {
                    palindrome[i][j] = palindrome[i + 1][j - 1];
                }
            }
        }
    }

    // dp[i] = min cuts needed for suffix starting at i
    std::vector<int> dp(n + 1, 0);

    for (int i = n - 1; i >= 0; --i) {
        int minCuts = INT_MAX;
        for (int j = i; j < n; ++j) {
            if (palindrome[i][j]) {
                int cost = 1 + dp[j + 1]; // one cut after j, plus rest
                minCuts = std::min(minCuts, cost);
            }
        }
        dp[i] = minCuts;
    }

    // dp[0] counts one cut for the last segment; subtract it
    return dp[0] - 1;
}

#include <cassert>

int main() {
    // Empty and single character
    assert(minPalindromePartition("") == 0);
    assert(minPalindromePartition("a") == 0);

    // Already palindrome
    assert(minPalindromePartition("aba") == 0);
    assert(minPalindromePartition("abba") == 0);
    assert(minPalindromePartition("racecar") == 0);

    // Simple cases
    assert(minPalindromePartition("aab") == 1);   // "aa" | "b"
    assert(minPalindromePartition("abc") == 2);   // "a"|"b"|"c"
    assert(minPalindromePartition("ab") == 1);    // "a"|"b"
    assert(minPalindromePartition("abbab") == 1); // "abba" | "b" or "a"|"bbab"? Actually "abba"+"b" => 1 cut

    // More complex
    assert(minPalindromePartition("abac") == 1);  // "aba" | "c"
    assert(minPalindromePartition("aaaa") == 0);  // already all same letters
    assert(minPalindromePartition("abcba") == 0); // palindrome

    // Case with all distinct
    assert(minPalindromePartition("abcd") == 3);  // each char alone

    // Longer repeated pattern
    assert(minPalindromePartition("ababb") == 2); // "aba"|"b"|"b" or "a"|"bab"|"b" etc.

    // Mixed, check a known optimal: "aaabba" => "aa"|"abba" => 1
    assert(minPalindromePartition("aaabba") == 1);
}
