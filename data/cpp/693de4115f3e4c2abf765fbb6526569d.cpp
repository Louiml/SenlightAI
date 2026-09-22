// Given a string `s` of length `n-1` consisting only of characters `'<'` and `'>'`, and an integer `n` (1 ≤ n ≤ 3000), write a C++ function `countValidPermutations(int n, const std::string& s)` that returns the number of permutations `p` of `{1, 2, ..., n}` such that for each `i` from 1 to n-1, the inequality `p[i] < p[i+1]` holds if `s[i-1] == '<'`, and `p[i] > p[i+1]` holds if `s[i-1] == '>'`. Since the answer can be very large, return it modulo 1000000007. The function should compute this efficiently without enumerating all permutations.
// The key idea is to build the permutation left to right, but instead of tracking actual values (which could be any of up to 3000), we track the **relative rank** of the last chosen element among the elements placed so far. Define `dp[i][j]` = number of ways to place the first `i` elements such that the last placed element (the `i`-th element) has relative rank `j` (1-indexed, where `j=1` means it is the smallest among the first `i`, and `j=i` means it is the largest).  
//
// Initially, with one element, it has rank 1: `dp[1][1] = 1`.  
// For each step from `i` to `i+1`, we choose a new element. The new element's relative rank among `i+1` elements is `k` (1 ≤ k ≤ i+1). The old rank `j` shifts depending on whether the new element is smaller or larger than the last. Specifically:
// - If the inequality is `'<'`, then `p[i] < p[i+1]`, so the new element must be **larger** than the last. That means the new element's rank `k` must satisfy `k > j`. The old element's rank remains `j` because the new element is larger and doesn't disturb the relative order of the first `i` elements. So transition: `dp[i+1][k] += dp[i][j]` for all `k` from `j+1` to `i+1`.
// - If the inequality is `'>'`, then `p[i] > p[i+1]`, so the new element must be **smaller** than the last. The new element's rank `k` must satisfy `1 ≤ k ≤ j`. Transition: `dp[i+1][k] += dp[i][j]` for all `k` from `1` to `j`.
//
// After processing all `n-1` inequalities, the answer is the sum of `dp[n][j]` for all `j` from 1 to `n`. This DP correctly counts every permutation exactly once because each permutation corresponds to a unique sequence of relative ranks when inserting elements in order.  
// Time complexity is O(n^2) if we naively iterate over `k` in the inner loop, but with prefix sums we can reduce it to O(n^2) overall (since there are O(n^2) states and each transition is O(1) with prefix sums, or O(n) without, giving O(n^3) – but the given snippet uses O(n^3) which is acceptable for n≤3000? Actually 3000^3 is too large, so we must use prefix sums to get O(n^2)). The problem statement says n up to 3000, so O(n^3) is 2.7e10, too slow. The reference solution uses prefix sums to make each transition O(1), giving O(n^2). Space: O(n^2) for the DP table (3000x3000 = 9e6, fine). Edge cases: n=1, s empty, answer is 1. Also ensure modulo handling.
#include <vector>
#include <string>

constexpr long long MOD = 1000000007LL;

// Count permutations of size n satisfying the inequality pattern in s.
// s has length n-1, each character is '<' or '>'.
long long countValidPermutations(int n, const std::string& s) {
    if (n == 1) return 1; // no constraints

    // dp[i][j] = ways to place first i elements, last one has rank j (1-indexed)
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long>(n + 1, 0));
    dp[1][1] = 1;

    for (int i = 1; i < n; ++i) {
        // Build prefix sums for dp[i] to allow O(1) range transitions
        std::vector<long long> prefix(i + 2, 0);
        for (int j = 1; j <= i; ++j) {
            prefix[j] = (prefix[j - 1] + dp[i][j]) % MOD;
        }
        // prefix[j] = sum of dp[i][1..j]

        if (s[i - 1] == '<') {
            // New element must be larger than last, so new rank k > old rank j
            // dp[i+1][k] += sum_{j=1}^{k-1} dp[i][j]
            for (int k = 2; k <= i + 1; ++k) {
                dp[i + 1][k] = (dp[i + 1][k] + prefix[k - 1]) % MOD;
            }
        } else { // '>'
            // New element must be smaller than last, so new rank k <= old rank j
            // dp[i+1][k] += sum_{j=k}^{i} dp[i][j]
            // This equals total_sum - prefix[k-1]
            long long total = prefix[i];
            for (int k = 1; k <= i; ++k) {
                long long rangeSum = (total - prefix[k - 1] + MOD) % MOD;
                dp[i + 1][k] = (dp[i + 1][k] + rangeSum) % MOD;
            }
            // k=i+1 impossible for '>' because new element smaller than last, so rank at most i
        }
    }

    long long answer = 0;
    for (int j = 1; j <= n; ++j) {
        answer = (answer + dp[n][j]) % MOD;
    }
    return answer;
}
#include <cassert>
#include <string>

// The function declaration is assumed to be included from the solution.
long long countValidPermutations(int n, const std::string& s);

int main() {
    // Small cases manually verifiable
    assert(countValidPermutations(1, "") == 1);
    assert(countValidPermutations(2, "<") == 1); // [1,2]
    assert(countValidPermutations(2, ">") == 1); // [2,1]
    assert(countValidPermutations(3, "<<") == 1); // [1,2,3]
    assert(countValidPermutations(3, ">>") == 1); // [3,2,1]
    assert(countValidPermutations(3, "<>") == 2); // [1,3,2], [2,3,1]
    assert(countValidPermutations(3, "><") == 2); // [2,1,3], [3,1,2]
    assert(countValidPermutations(4, "<><") == 5); // known count for alternating pattern
    assert(countValidPermutations(4, "><>") == 5);
    // A larger n with all '<' yields exactly 1 (increasing permutation)
    std::string all_less(2999, '<');
    assert(countValidPermutations(3000, all_less) == 1);
    // All '>' yields 1 (decreasing permutation)
    std::string all_greater(2999, '>');
    assert(countValidPermutations(3000, all_greater) == 1);
    return 0;
}
