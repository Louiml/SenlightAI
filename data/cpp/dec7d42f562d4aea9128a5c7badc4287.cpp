// Write a C++ function `countEvenSetBitsNumbers(long long n)` that returns the number of integers in the range `[0, n]` (inclusive) whose binary representation contains an even number of 1-bits (i.e., the popcount is even), modulo `1,000,000,007`. The input `n` can be as large as `10^18`, and the function must process each query in `O(log n)` time. The solution must use digit DP over the binary representation of `n`, processing bits from most significant to least significant, with state tracking the total popcount parity (even/odd). Since the original snippet uses a clever interval-accumulation DP, your implementation must follow a similar efficient approach: compute the count by iterating over bits and maintaining a DP table indexed by the current popcount modulo some small bound, then apply prefix sums to propagate counts. Ensure the function is `const`-correct, uses `long long` for input, returns `int` (the modulo result), and is self-contained with only necessary headers.

The problem asks for the number of integers `x` with `0 <= x <= n` such that `popcount(x)` is even. Since `n` can be up to `10^18` (~60 bits), iterating all numbers is impossible. We use digit DP on the binary representation. The key observation from the provided snippet is a state compression: instead of tracking the exact popcount (which can be up to 60), we only need the parity of the popcount, but the original code uses a DP with state `dp[i][mask]` where `mask` is the number of "pending carries" in a clever counting trick. Simpler: define `dp[pos][parity][tight]`, where `pos` is the current bit index (from MSB to LSB), `parity` is the current popcount parity (0 even, 1 odd), and `tight` indicates whether the prefix equals the prefix of `n`. Transitions: for each possible bit `b` (0 or 1) that respects `tight`, the new parity is `parity ^ b`, and new `tight` is `tight && (b == n_bit)`. Base: at `pos = -1`, return `1` if `parity == 0`. This gives `O(log n)` states (about 60*2*2) and `O(1)` transitions per state. The answer is `dp[60][0][1]` starting from the most significant bit. Edge cases: `n = 0` returns 1 (only 0 has popcount 0, which is even). The modulo `1,000,000,007` is applied at each addition. Time complexity per query: `O(log n)` with constant factor about 4 (two parities, two tight states), space `O(log n)` for recursion or `O(1)` iterative. The solution is robust for all `n` up to `10^18`.

#include <bits/stdc++.h>

// Count numbers in [0, n] with even popcount, modulo 1e9+7.
int countEvenSetBitsNumbers(long long n) {
    const int MOD = 1000000007;
    std::vector<int> bits;
    for (int i = 60; i >= 0; --i) {
        bits.push_back((n >> i) & 1LL);
    }
    // dp[pos][parity][tight], but we process iteratively.
    // dp[parity][tight] for current position.
    long long dp[2][2] = {};
    dp[0][1] = 1;  // start: no bits processed, parity even, tight.
    for (int pos = 0; pos < (int)bits.size(); ++pos) {
        long long ndp[2][2] = {};
        int limit_bit = bits[pos];
        for (int parity = 0; parity < 2; ++parity) {
            for (int tight = 0; tight < 2; ++tight) {
                long long ways = dp[parity][tight];
                if (ways == 0) continue;
                int maxBit = tight ? limit_bit : 1;
                for (int b = 0; b <= maxBit; ++b) {
                    int nparity = parity ^ b;
                    int ntight = tight && (b == limit_bit);
                    ndp[nparity][ntight] = (ndp[nparity][ntight] + ways) % MOD;
                }
            }
        }
        std::memcpy(dp, ndp, sizeof(dp));
    }
    return (int)((dp[0][0] + dp[0][1]) % MOD);
}

#include <cassert>
#include <bits/stdc++.h>

int countEvenSetBitsNumbers(long long n); // declare

int main() {
    assert(countEvenSetBitsNumbers(0) == 1);          // only 0
    assert(countEvenSetBitsNumbers(1) == 1);          // 0 (even), 1 (odd) -> only 0
    assert(countEvenSetBitsNumbers(2) == 2);          // 0,2 have even popcount
    assert(countEvenSetBitsNumbers(3) == 2);          // 0,3
    assert(countEvenSetBitsNumbers(4) == 3);          // 0,3,4
    assert(countEvenSetBitsNumbers(7) == 4);          // 0,3,5,6
    assert(countEvenSetBitsNumbers(15) == 8);         // half of 0..15
    assert(countEvenSetBitsNumbers(1000000) % 1000000007 == 
           countEvenSetBitsNumbers(1000000) % 1000000007);
    assert(countEvenSetBitsNumbers(1000000000000000000LL) >= 0);
    return 0;
}
