// Given three integers `n`, `m`, and `k`, write a C++ function `numOfArrays` that returns the number of ways to construct a valid array of length `n` where each element is an integer between 1 and `m` (inclusive), such that there are exactly `k` strictly increasing "record breaks" — meaning when scanning from left to right, the current element is strictly greater than the maximum among all previous elements. Since the result can be very large, return it modulo `1e9+7`. For example, with `n = 2, m = 3, k = 1`, valid arrays are `[1,2]`, `[1,3]`, `[2,3]`, and also arrays where the first element is already the maximum, like `[3,1]`, `[3,2]`, `[3,3]`, `[2,2]`, `[2,1]`, `[1,1]` — but note that in `[1,1]`, the second element is not strictly greater than previous max, so that's invalid; in fact only arrays where exactly one strict increase occurs (the second element is greater than first) plus all arrays where the first element is equal to the overall maximum? Actually the definition: count the number of times `arr[i] > max(arr[0..i-1])` (with the first element always counted as a record break because it is greater than the empty prefix max). So the task is to count length-`n` arrays with elements from `1` to `m` that have exactly `k` record-breaking elements (including the first element). If `k` is impossible (e.g., `k > n` or `k > m`), return 0.

// This is a dynamic programming problem. Define `dp[i][k][curMax]` as the number of ways to fill positions from `i` to `n` (1-indexed) given that we have already used `k` record breaks so far and the current maximum value among the first `i-1` positions is `curMax`. The base case: when `k == 0` (meaning we have exactly used all required record breaks), the remaining `n-i+1` positions can be any value from 1 to `curMax` (since we cannot create any more record breaks, each element must be ≤ `curMax`). The number of such completions is `curMax^(n-i+1)` modulo `p`. If `i > n` but `k > 0`, it's impossible, return 0. For the recursive step, at position `i`, we have two choices: (1) place a value `j` from 1 to `curMax` — this does not create a new record break, so `k` stays the same and `curMax` remains unchanged; we sum over all such `j` the result of `dp[i+1][k][curMax]`. (2) place a value `j` from `curMax+1` to `m` — this creates a new record break (so `k` decreases by 1) and updates `curMax` to `j`. We sum over all such `j` the result of `dp[i+1][k-1][j]`. The answer is `dp[1][k][0]` because initially there is no previous max (we can treat `curMax=0` since all values are at least 1). However, careful: when `k=0` at the start (i.e., we need zero record breaks), but the first element always counts as a record break, so `k` must be at least 1 for any valid array. The recursion handles this because with `curMax=0`, the second loop covers `j` from 1 to `m`, each of which increments the record break count. The DP uses memoization with a 3D table `dp[55][55][101]` because `n ≤ 50`, `k ≤ 50`, and `m ≤ 100` (typical LeetCode constraints). The time complexity is `O(n * k * m * m)` due to the loops inside each state, but with memoization each state is computed once, and each state does up to `m` iterations for both loops, giving `O(n * k * m^2)` worst case (but for m=100, n=50, k=50 that’s about 50*50*100*100 = 25 million, feasible). Space complexity is `O(n * k * m)` for the DP table. Edge cases: `k > n` or `k > m` should return 0; also `k == 0` should return 0 because the first element always creates a record break unless `n=0`, but `n ≥ 1` by problem constraints. The use of modular arithmetics with `long long` prevents overflow during multiplication and addition.

#include <vector>
#include <cstring>

using ll = long long;
const ll MOD = 1000000007LL;

class Solution {
public:
    ll dp[55][55][101]; // dp[i][k][currentMax]
    int nGlobal, mGlobal, kGlobal;

    // Fast modular exponentiation: returns (x^y) % mod
    ll modPow(ll x, ll y, ll mod) {
        ll res = 1;
        x %= mod;
        while (y > 0) {
            if (y & 1) res = (res * x) % mod;
            y >>= 1;
            x = (x * x) % mod;
        }
        return res;
    }

    ll solve(int pos, int remK, int currentMax) {
        // If no more record breaks needed, fill remaining positions with <= currentMax
        if (remK == 0) {
            // How many positions remain? From pos to nGlobal inclusive
            int remaining = nGlobal - pos + 1;
            return modPow(currentMax, remaining, MOD);
        }
        // If we have exhausted positions but still need breaks, impossible
        if (pos > nGlobal) {
            return 0;
        }
        // Memoization
        if (dp[pos][remK][currentMax] != -1) {
            return dp[pos][remK][currentMax];
        }

        ll ans = 0;

        // Option 1: place a value from 1 to currentMax (no new record)
        // Note: currentMax can be 0 initially, then this loop is empty
        for (int val = 1; val <= currentMax; ++val) {
            ans = (ans + solve(pos + 1, remK, currentMax)) % MOD;
        }

        // Option 2: place a value from currentMax+1 to mGlobal (new record)
        for (int val = currentMax + 1; val <= mGlobal; ++val) {
            ans = (ans + solve(pos + 1, remK - 1, val)) % MOD;
        }

        dp[pos][remK][currentMax] = ans;
        return ans;
    }

    int numOfArrays(int n, int m, int k) {
        nGlobal = n;
        mGlobal = m;
        kGlobal = k;
        memset(dp, -1, sizeof(dp));
        // Start at position 1, need k record breaks, currentMax = 0 (since no previous element)
        // The first element will be handled by the second loop (val from 1 to m)
        ll result = solve(1, k, 0);
        return (int)result;
    }
};

#include <cassert>
#include <iostream>

// Assume the Solution class definition from above is included here

int main() {
    Solution sol;
    // Basic example: n=2, m=3, k=1 -> possible arrays: [1,2],[1,3],[2,3],[2,1],[3,1],[3,2],[3,3]? Wait: first element always breaks, so we need exactly one more break.
    // Better to manually compute: with k=1, only the first element is a break. So the rest must be <= first element.
    // Count = sum_{first=1}^3 (first)^1 = 1+2+3=6. Let's verify with code.
    assert(sol.numOfArrays(2, 3, 1) == 6);
    // n=3, m=3, k=2: first element always break, need exactly one more break.
    // Let's trust the result from known LeetCode problem, should be 22? Actually I can compute: 
    // Possible? Let's just test a known case: n=2, m=1, k=1 -> only array [1,1] has one break, so answer 1.
    assert(sol.numOfArrays(2, 1, 1) == 1);
    // n=2, m=1, k=2 -> impossible because max breaks = n=2 but m=1 means no second break, answer 0.
    assert(sol.numOfArrays(2, 1, 2) == 0);
    // n=1, m=5, k=1 -> any single element works, 5 ways.
    assert(sol.numOfArrays(1, 5, 1) == 5);
    // n=1, m=5, k=0 -> impossible, answer 0.
    assert(sol.numOfArrays(1, 5, 0) == 0);
    // n=3, m=2, k=3 -> impossible because max breaks = n=3, but values only 1,2, so max possible breaks is 2 (1 then 2, then no more). So answer 0.
    assert(sol.numOfArrays(3, 2, 3) == 0);
    // A known larger case: numOfArrays(2, 3, 2) means both elements are breaks, so array must be strictly increasing. For m=3, number of strictly increasing sequences of length 2 from 1..3 is C(3,2)=3: [1,2],[1,3],[2,3].
    assert(sol.numOfArrays(2, 3, 2) == 3);
    // Edge: n=50, m=100, k=50 should be >0 and modulo handled.
    assert(sol.numOfArrays(50, 100, 50) > 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
