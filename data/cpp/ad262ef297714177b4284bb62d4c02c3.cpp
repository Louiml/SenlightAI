Write a C++ function `int minimalCutoff(int n, double p, const std::vector<int>& coins)` that solves the following problem. You are given `n` distinct coin denominations (positive integers) and a target probability `p` (0.0 ≤ p ≤ 1.0). Consider all `2^n` possible subsets of these coins (including the empty subset with total value 0). You need to find the smallest non-negative integer `sumLimit` such that the probability (i.e., fraction) of subsets whose total sum is **less than or equal to** `sumLimit` is **at least** `p`. In other words, sort all subset sums (with multiplicities, as each subset is distinct), and return the smallest value `S` where the cumulative proportion of subsets with sum ≤ S reaches or exceeds `p`. For example, with coins {1, 3} and p = 0.75, subset sums are 0,1,3,4; cumulative proportions are 0.25 (sum≤0), 0.5 (sum≤1), 0.75 (sum≤3), 1.0 (sum≤4), so the answer is 3. If p=1.0, the answer is the total sum of all coins. Assume `n` ≤ 20, coin values up to 10^4, and total sum fits in a 64-bit integer, but subset counts may exceed 32-bit; the answer is always ≤ total sum. The function must be deterministic and correct for all valid inputs.
// The problem is a classic dynamic programming counting problem: count how many subsets produce each possible sum. Because `n` ≤ 20, the number of subsets is up to \(2^{20} = 1,048,576\), and the total sum can be up to 20×10⁴ = 200,000. We use a 1D DP array `dp[s]` (of type `long long` or `unsigned long long`) where `dp[s]` = number of subsets (using processed coins) that have sum exactly `s`. Initialize `dp[0] = 1` (empty subset). For each coin value `v`, update the DP in decreasing sum order: `dp[s] += dp[s-v]` for `s` from `totalSum` down to `v`. This counts each subset exactly once. After processing all coins, the total number of subsets is `2^n` (which may be up to ~1,048,576, fitting in a 64-bit integer). Then iterate `s` from 0 to totalSum, accumulating `tot += dp[s]`. After adding each `s`, check if `tot / (double)(1LL<<n) >= p`. Because floating-point precision is fine for this comparison (since values are exact integers and we compare a fraction), we can alternatively use integer arithmetic: `tot * 1'000'000'000 >= p * (1LL<<n) * 1'000'000'000` but simpler to convert to double. However, to avoid floating-point round-off near boundary (e.g., p=0.5 and tot/2^n exactly equals 0.5), using double comparison `>=` is safe because we are looking for the first `s` where the cumulative fraction is at least p; if the fraction is exactly p, it should return that `s`. Edge cases: p=0.0 → answer is 0 (empty subset sum 0 counts). p=1.0 → answer is total sum. If coins have duplicates? The problem states "distinct coin denominations" but even if duplicates were allowed, the DP would still count subsets correctly (treating coins as distinct items). Time complexity: O(n·totalSum) which is at most 20×200,000 = 4 million operations, fast. Space complexity: O(totalSum) ~ 200,000 entries, each 8 bytes, ~1.6 MB.
#include <vector>
#include <cstdint>
#include <algorithm>

// Given n distinct coin values, and a probability p in [0,1],
// return the smallest integer sum S such that the fraction of all 2^n subsets
// whose total sum is <= S is at least p.
int minimalCutoff(int n, double p, const std::vector<int>& coins) {
    // Compute total sum of all coins
    long long totalSum = 0;
    for (int v : coins) {
        totalSum += v;
    }

    // dp[s] = number of subsets summing to s
    std::vector<long long> dp(totalSum + 1, 0);
    dp[0] = 1;  // empty subset

    for (int v : coins) {
        // Update in descending order to avoid reusing the same coin multiple times
        for (long long s = totalSum; s >= v; --s) {
            dp[s] += dp[s - v];
        }
    }

    const long long totalSubsets = 1LL << n;  // n <= 20, fits in 64-bit
    long long cumCount = 0;

    for (int s = 0; s <= totalSum; ++s) {
        cumCount += dp[s];
        // Compare fractions without floating point issues using cross-multiplication
        // cumCount / totalSubsets >= p  <=> cumCount >= p * totalSubsets
        // Use double for simplicity, but cross-multiplication is exact.
        // We'll use double; precision is fine for n<=20 and exact fractions.
        if (static_cast<double>(cumCount) / static_cast<double>(totalSubsets) >= p) {
            return s;
        }
    }
    // Should never reach here if p <= 1.0, but fallback
    return static_cast<int>(totalSum);
}
#include <cassert>
#include <vector>
#include "solution.h"  // In practice, the function is in the same file.

int main() {
    // Test 1: Example from prompt
    {
        std::vector<int> coins = {1, 3};
        assert(minimalCutoff(2, 0.75, coins) == 3);
        assert(minimalCutoff(2, 0.5, coins) == 1);
        assert(minimalCutoff(2, 0.25, coins) == 0);
        assert(minimalCutoff(2, 1.0, coins) == 4);
    }

    // Test 2: All coins same value? Distinct but could have same? We allow distinct. Single coin
    {
        std::vector<int> coins = {5};
        assert(minimalCutoff(1, 0.0, coins) == 0);
        assert(minimalCutoff(1, 0.5, coins) == 0); // 1 subset empty (sum 0) fraction=0.5
        assert(minimalCutoff(1, 0.51, coins) == 5); // need fraction≥0.51, only subset {5} adds sum 5
        assert(minimalCutoff(1, 1.0, coins) == 5);
    }

    // Test 3: Multiple coins with large sums
    {
        std::vector<int> coins = {10, 20, 40};
        // Subset sums: 0,10,20,30,40,50,60,70  (8)
        // p=0.5 -> need at least 4 subsets -> sums 0,10,20,30 -> cutoff 30
        assert(minimalCutoff(3, 0.5, coins) == 30);
        // p=0.75 -> need 6 subsets -> sums up to 50 -> cutoff 50
        assert(minimalCutoff(3, 0.75, coins) == 50);
        // p=0.125 -> need 1 subset -> sum 0
        assert(minimalCutoff(3, 0.125, coins) == 0);
        // p=0.999 -> need 8 subsets? Actually 0.999*8=7.992, so need 8 subsets -> sum 70
        assert(minimalCutoff(3, 0.999, coins) == 70);
    }

    // Test 4: Edge with duplicate values in coins vector (though problem says distinct)
    {
        std::vector<int> coins = {2, 2};
        // Subsets: {} =0, {first2}=2, {second2}=2, {both}=4 -> sums 0,2,2,4
        // p=0.5 -> need 2 subsets -> sums 0,2 -> cutoff 2
        assert(minimalCutoff(2, 0.5, coins) == 2);
        // p=0.75 -> need 3 subsets -> sums 0,2,2 -> cutoff 2
        assert(minimalCutoff(2, 0.75, coins) == 2);
        // p=1.0 -> sum 4
        assert(minimalCutoff(2, 1.0, coins) == 4);
    }

    // Test 5: Many coins, p=0.5 expected median of sums
    {
        std::vector<int> coins = {1, 2, 4, 8, 16};
        // total subsets 32. Half=16. Sums: all numbers 0..31 exactly once (binary representation)
        // The 16th smallest sum (0-indexed?) is 15? Actually sums from 0 to 31 each once. The cumulative count reaches 16 at sum=15.
        assert(minimalCutoff(5, 0.5, coins) == 15);
        // p=0.25 -> 8 subsets -> sum 7
        assert(minimalCutoff(5, 0.25, coins) == 7);
        // p=0.9 -> 28.8 -> need 29 subsets -> sum 28
        assert(minimalCutoff(5, 0.9, coins) == 28);
    }

    // Test 6: Extremely small p, and p=0
    {
        std::vector<int> coins = {100, 200};
        assert(minimalCutoff(2, 0.0, coins) == 0);
        assert(minimalCutoff(2, 0.1, coins) == 0); // 1/4 =0.25 >0.1 already
        assert(minimalCutoff(2, 0.25, coins) == 0); // exactly 0.25
        assert(minimalCutoff(2, 0.26, coins) == 100); // need 2nd subset sum 100
    }

    // Test 7: total sum large, n=20, but small values
    {
        std::vector<int> coins(20, 1);
        // All coins value 1, but since they are identical, number of subsets with sum s is C(20,s)
        // For p=0.5, need cumulative C(20,0)+...+C(20,k) >= 2^19 = 524288
        // Compute: k=9: sum=1+20+190+1140+4845+15504+38760+77520+125970+167960=sum up to k=9
        // Let's compute approximate: sum up to k=9 = 1+20+190+1140+4845+15504+38760+77520+125970+167960 = 431910? Actually sum for k=0..9 = 1+20+190+1140+4845+15504+38760+77520+125970+167960 = 431910? Let's compute: 1+20=21, +190=211, +1140=1351, +4845=6196, +15504=21700, +38760=60460, +77520=137980, +125970=263950, +167960=431910. That's <524288. Need k=10: sum C(20,10)=184756, total = 431910+184756=616666 >524288, so answer is 10.
        assert(minimalCutoff(20, 0.5, coins) == 10);
        // p=1.0 -> total sum 20
        assert(minimalCutoff(20, 1.0, coins) == 20);
    }

    return 0;
}
