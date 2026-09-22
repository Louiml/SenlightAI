/*
Write a C++ function `long long countGoodSubsequences(const std::vector<int>& nums)` that, given a vector of integers where each element is between 2 and 70 inclusive, returns the number of non-empty subsequences (not necessarily contiguous, preserving original order) whose product is a perfect square, modulo `1'000'000'007`. Each element may appear multiple times, and the subsequence may include repeated values from the input. For example, for `nums = {2, 2, 3}`, valid subsequences with square products are `{2, 2}` (product 4) and `{2, 2, 3}` is invalid (product 12), and `{3}` alone is invalid. The empty subsequence is not counted.
*/
#include <vector>

const long long MOD = 1000000007LL;

// Precomputed prime list up to 70 (19 primes)
const int PRIMES[19] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67};
const int MAX_VAL = 70;
const int MASK_COUNT = 1 << 19;

// Fast exponentiation modulo MOD
long long modPow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Build the parity mask for a given number
int buildMask(int x) {
    int mask = 0;
    for (int j = 0; j < 19; ++j) {
        while (x % PRIMES[j] == 0) {
            mask ^= (1 << j);
            x /= PRIMES[j];
        }
    }
    return mask;
}

// Main solution function
long long countGoodSubsequences(const std::vector<int>& nums) {
    // Frequency of each value from 2 to 70
    std::vector<int> cnt(MAX_VAL + 1, 0);
    for (int x : nums) {
        if (x >= 2 && x <= MAX_VAL) {
            cnt[x]++;
        }
    }

    // Precompute masks for values 2..70
    int mask[MAX_VAL + 1] = {0};
    for (int v = 2; v <= MAX_VAL; ++v) {
        mask[v] = buildMask(v);
    }

    // DP arrays: dp[mask] = number of ways to achieve that XOR mask
    std::vector<long long> dp(MASK_COUNT, 0);
    dp[0] = 1; // empty subsequence

    for (int v = 2; v <= MAX_VAL; ++v) {
        int c = cnt[v];
        if (c == 0) continue;

        // For a value appearing c times, number of even/odd selections is 2^(c-1)
        long long ways = modPow(2, c - 1);
        long long evenWays = ways;
        long long oddWays = ways;

        std::vector<long long> ndp(MASK_COUNT, 0);
        for (int m = 0; m < MASK_COUNT; ++m) {
            if (dp[m] == 0) continue;
            // Choose even number of v: mask unchanged
            ndp[m] = (ndp[m] + dp[m] * evenWays) % MOD;
            // Choose odd number of v: toggle mask
            int newMask = m ^ mask[v];
            ndp[newMask] = (ndp[newMask] + dp[m] * oddWays) % MOD;
        }
        dp.swap(ndp);
    }

    // Subtract 1 for the empty subsequence
    long long ans = (dp[0] - 1 + MOD) % MOD;
    return ans;
}
#include <cassert>
#include <vector>

// Function declaration must match the solution signature
long long countGoodSubsequences(const std::vector<int>& nums);

int main() {
    // Test 1: {2, 2} -> only {2,2} works, empty not counted
    assert(countGoodSubsequences({2, 2}) == 1);

    // Test 2: {2, 2, 3} -> {2,2} only, {3} not square, {2,2,3} not square, {2} not
    assert(countGoodSubsequences({2, 2, 3}) == 1);

    // Test 3: {4} -> 4 = 2^2, mask 0, whole set works, empty not counted
    assert(countGoodSubsequences({4}) == 1);

    // Test 4: {6, 6} -> 6 mask has bits for 2 and 3, product 36 = square, so {6,6} works
    assert(countGoodSubsequences({6, 6}) == 1);

    // Test 5: {2, 3} -> product 6 not square, no non-empty subsequence works
    assert(countGoodSubsequences({2, 3}) == 0);

    // Test 6: {2, 8} -> 2*8=16 square, also {8} not, {2} not, so only {2,8}
    assert(countGoodSubsequences({2, 8}) == 1);

    // Test 7: {} -> no non-empty subsequence
    assert(countGoodSubsequences({}) == 0);

    // Test 8: {2, 2, 2, 2} -> any even-size selection works: size 2 (C(4,2)=6), size 4 (1) -> total 7
    assert(countGoodSubsequences({2, 2, 2, 2}) == 7);

    // Test 9: {2, 3, 6} -> possible: {6} (mask 0), {2,3} (product 6 not), {2,6}? 12 not, {3,6}? 18 not, {2,3,6}=36 square → also {2,3,6}. So {6} and {2,3,6} -> 2
    assert(countGoodSubsequences({2, 3, 6}) == 2);

    return 0;
}
// The key observation is that the product of a subsequence is a perfect square if and only if the XOR (bitwise) of the parity vectors of the prime factorization exponents of the selected numbers is zero. Since all numbers are at most 70, we can precompute for each number from 2 to 70 a 19-bit mask (because there are 19 primes ≤ 70: 2,3,5,...,67) where the `j`-th bit is 1 if the exponent of the `j`-th prime in that number is odd. Then the product of a set of numbers is a square iff the XOR of their masks is 0.
//
// We count subsequences by dynamic programming over the masked states. For each possible value `v` from 2 to 70, let `cnt[v]` be its frequency in the input. For a value with mask `m`, any subsequence formed using that value can either use an even number of occurrences (which does not change the XOR state) or an odd number (which toggles the mask). For a value appearing `c` times, the number of ways to choose an even number of occurrences is `2^(c-1)` (if c ≥ 1; specifically for c=0 it is 1, but we can skip), and the number of ways to choose an odd number is also `2^(c-1)` when c>0 (because exactly half of the subsets have even size and half have odd size). Thus for each value, we have two choices in the DP transition: either keep the current state unchanged with a multiplier `evenWays`, or toggle the mask with multiplier `oddWays`. If the value does not appear, we simply skip it (or treat as a multiplier of 1 for both choices, but it is easier to ignore).
//
// Let `dp[mask]` be the number of ways to achieve XOR mask `mask` after processing some set of values. Initialize `dp[0] = 1` (empty set). For each value `v` from 2 to 70, if `cnt[v] > 0`, compute `evenWays = oddWays = 2^(cnt[v]-1)`. Then update a new DP: for each current mask `m`, add `dp_old[m] * evenWays` to `new_dp[m]` and `dp_old[m] * oddWays` to `new_dp[m ^ mask[v]]`. After processing all values, the answer is `dp[0] - 1` (subtract the empty subsequence). The DP has 2^19 = 524288 states, which is manageable. Time complexity is O(70 * 2^19) ≈ 36 million operations, which is acceptable. Space complexity is O(2^19) for DP arrays. Edge cases: if the input has no elements, the answer is 0; the modulo is applied at every step; and ensure that 2^(cnt-1) is computed with fast exponentiation modulo.
