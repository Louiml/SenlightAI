// Given a positive integer `n` and a modulus `mod`, write a C++ function that computes the sum of terms `fac[c] * (n * fac[n] % mod) % mod` iteratively, where `n` decreases by 1 each step, `c` increases by 1 each step, and `fac[i]` is the factorial of `i` modulo `mod`, precomputed for all needed indices up to the initial `n + 1`. The loop stops when `n` becomes zero. The function should return the final sum modulo `mod`. Ensure the factorial array is large enough for all intermediate `n` values, and handle cases where `mod` might be 1 (result always 0) or where `n` is small. The input values may be up to 10^5 for `n`, and `mod` may be up to 10^9+7. The function must be self-contained, take both `n` and `mod` as parameters, and return a `long long`.

// The problem is a straightforward simulation. We need to precompute factorials modulo `mod` up to `n+1` because the largest factorial used is for the initial `n` (and also we use `n` itself in the product). The loop runs exactly `n` times (from initial `n` down to 1, since each iteration decrements `n`). In each iteration, we compute `add = fac[c] * (current_n * fac[current_n] % mod) % mod` and add to a running total. The iterative process updates `current_n` by decrementing and `c` by incrementing. Edge cases: if `mod == 1`, all factorials and products are 0, so the result is 0. If `n == 0`, the loop does not run and the result is 0. The factorial precomputation uses `O(n)` time and `O(n)` space. The main loop also takes `O(n)` time, so total time is `O(n)` and space is `O(n)`. Since `n` can be up to 1e5, this is acceptable.

#include <vector>

// Computes the sum described, mod m, with n decreasing and c increasing.
long long computeFactorialSum(long long n, long long mod) {
    if (n <= 0 || mod == 1) {
        return 0;
    }

    const long long maxIndex = n + 1;
    std::vector<long long> fac(maxIndex + 1, 1);
    for (long long i = 2; i <= maxIndex; ++i) {
        fac[i] = (fac[i - 1] * i) % mod;
    }

    long long ans = 0;
    long long c = 1;
    long long current = n;
    while (current > 0) {
        long long add = (fac[c] * ((current * fac[current]) % mod)) % mod;
        ans = (ans + add) % mod;
        --current;
        ++c;
    }
    return ans;
}

#include <cassert>
#include <iostream>

// Function declaration (placed in solution above, but for test we include via copy)
long long computeFactorialSum(long long n, long long mod);

int main() {
    // Basic cases
    assert(computeFactorialSum(1, 100) == 1); // only term: fac[1]*(1*fac[1])=1*1=1
    assert(computeFactorialSum(2, 100) == 1*2 + 1*1 = 3); // manually: n=2,c=1: fac1=1, 2*fac2=2*2=4 -> 4; n=1,c=2: fac2=2, 1*fac1=1 -> 2; total 6? Wait recompute: (fac1=1)*(2*fac2=4)=4, then (fac2=2)*(1*fac1=1)=2, sum=6
    assert(computeFactorialSum(2, 100) == 6);
    assert(computeFactorialSum(3, 1000) == 1*6 + 2*2 + 6*1 = 6+4+6=16);
    // mod=1
    assert(computeFactorialSum(10, 1) == 0);
    // n=0
    assert(computeFactorialSum(0, 100) == 0);
    // Large mod (no overflow if mod fits in long long)
    assert(computeFactorialSum(5, 1000000007LL) == (1*120 + 2*24 + 6*6 + 24*2 + 120*1) % 1000000007LL); // compute: 120+48+36+48+120=372
    assert(computeFactorialSum(5, 1000000007LL) == 372);
    // Check with small mod where factorial may wrap
    assert(computeFactorialSum(4, 7) == ((1*24%7=3) + (2*6%7=5) + (6*2%7=5) + (24*1%7=3)) %7 = 16%7=2);
    assert(computeFactorialSum(4, 7) == 2);
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
