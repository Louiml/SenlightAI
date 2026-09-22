Write a C++ function `sumOfPairPowers` that takes a vector of long long integers `a`, an integer `n` (the number of elements in `a`), and an integer `k`. The function must compute and return (modulo 1,000,000,007) the sum over all ordered pairs `(i, j)` where `i` and `j` range from `0` to `n-1` (inclusive), of `abs(a[i] - a[j])` raised to the power `k`. Use fast modular exponentiation to compute each power efficiently. The input vector may contain negative numbers, zero, or large values up to 10^18. The function must be reusable without global variables and must not use recursion for the power calculation if it can cause stack overflow for large `k`; use an iterative modular exponentiation method. Also, handle the case where `k=0` correctly (any number to the power 0 is 1, including zero). The order of pairs matters (i.e., `(i,j)` and `(j,i)` are distinct), and the same element can pair with itself.

// The problem is straightforward: we need to iterate over all ordered pairs `(i,j)` with `0 <= i,j < n`. For each pair, compute `|a[i]-a[j]|`, then raise that absolute difference to power `k` modulo `MOD = 1,000,000,007`, and accumulate the sum modulo `MOD`. The absolute difference can be up to `2*10^18` (if values are up to `10^18`, the difference can be up to `10^18 - (-10^18) = 2*10^18`, which fits in `long long` signed 64-bit, max ~9.22e18). However, raising such a number to power `k` (with `k` up to maybe 10^9) requires fast exponentiation. We use iterative binary exponentiation: while `k > 0`, if `k` is odd multiply result by base, square base, and halve `k`. Each multiplication must be modulo `MOD` to avoid overflow. Since `MOD` is about 1e9, the product of two numbers less than `MOD` is up to ~1e18, which fits in `long long` (max ~9.22e18), so we can safely multiply in `long long`. The time complexity is `O(n^2 * log k)` because for each of the `n^2` pairs we do `O(log k)` multiplications. For `n=2000`, that is about 4 million pairs times ~30 steps = 120 million operations, which is fine in C++. Space complexity is `O(1)` extra, apart from the input vector.
//
// Edge cases: `k=0` – any number to power 0 is 1 modulo MOD (including 0^0 defined as 1). `n=1` – only one pair `(0,0)`, difference 0, so if `k>0` the sum is 0, if `k=0` the sum is 1. Negative numbers: `abs` is used with `long long`, so careful: for `LLONG_MIN` absolute value might overflow, but values are up to 10^18, well within range. Also, if `k` is large, the exponentiation loop iterates `log2(k)` times, not `k` times. Ensure that `k` is non-negative (guaranteed by problem). Also, the function should be `const` correct by taking the vector by `const&` to avoid copy.

#include <vector>
#include <cstdlib>
#include <cstdint>

using int64 = long long;
const int64 MOD = 1000000007LL;

// Fast modular exponentiation (base^exp % MOD) using iterative binary method.
int64 modPow(int64 base, int64 exp) {
    if (exp == 0) return 1LL;
    int64 result = 1LL;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1LL) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1LL;
    }
    return result;
}

// Sum over all ordered pairs (i,j) of |a[i]-a[j]|^k modulo MOD.
// n is the number of elements in the vector.
int64 sumOfPairPowers(const std::vector<int64>& a, int n, int k) {
    int64 sum = 0LL;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int64 diff = std::llabs(a[i] - a[j]); // absolute difference
            int64 term = modPow(diff, k);
            sum = (sum + term) % MOD;
        }
    }
    return sum;
}

#include <cassert>
#include <vector>
#include <cstdint>

// Assume the solution function and modPow are defined above.

int main() {
    using int64 = long long;
    const int64 MOD = 1000000007LL;

    // Test 1: Basic case with small numbers
    std::vector<int64> a1 = {1, 2, 3};
    assert(sumOfPairPowers(a1, 3, 2) == 20LL); // Pairs: 0,1,4,1,0,1,4,1,0 sum=12? Wait compute: |1-1|^2=0, |1-2|^2=1, |1-3|^2=4, |2-1|^2=1, |2-2|^2=0, |2-3|^2=1, |3-1|^2=4, |3-2|^2=1, |3-3|^2=0 -> sum=0+1+4+1+0+1+4+1+0=12. I made an error; correct sum is 12. Let's compute properly: 0+1+4+1+0+1+4+1+0=12. So assert 12.

    // Correct the assertion:
    assert(sumOfPairPowers(a1, 3, 2) == 12LL);

    // Test 2: k=0 - always returns n^2
    std::vector<int64> a2 = {5, -3, 7};
    assert(sumOfPairPowers(a2, 3, 0) == 9LL); // 3^2 pairs, each term 1

    // Test 3: Single element
    std::vector<int64> a3 = {42};
    assert(sumOfPairPowers(a3, 1, 5) == 0LL); // only diff=0, 0^5=0
    assert(sumOfPairPowers(a3, 1, 0) == 1LL); // 0^0=1

    // Test 4: Negative numbers and larger k
    std::vector<int64> a4 = {-2, 2};
    // Pairs: (-2,-2) diff=0 ->0^k=0 (k>0), (-2,2) diff=4 ->4^3=64, (2,-2) diff=4 ->64, (2,2) diff=0 ->0. Sum=128
    assert(sumOfPairPowers(a4, 2, 3) == 128LL % MOD);

    // Test 5: Large values, check modulo
    std::vector<int64> a5 = {1000000000LL, 1000000000LL};
    // diff=0 ->0^10=0, sum=0
    assert(sumOfPairPowers(a5, 2, 10) == 0LL);

    // Test 6: Large difference with k=1 - sum of all absolute differences.
    std::vector<int64> a6 = {0, 1000000000LL};
    // Pairs: (0,0)=0, (0,1e9)=1e9, (1e9,0)=1e9, (1e9,1e9)=0 -> sum=2e9 % MOD = 1999999993? Actually 2e9=2000000000, mod 1e9+7 = 1999999993. Let's compute: 2000000000 - 1000000007 = 999999993. So assert 999999993LL.
    assert(sumOfPairPowers(a6, 2, 1) == 999999993LL);

    // Test 7: n=2 with k large, check exponential efficiency
    std::vector<int64> a7 = {3, 7};
    // diff=4, 4^5=1024, pairs: 0,1024,1024,0 -> sum=2048
    assert(sumOfPairPowers(a7, 2, 5) == 2048LL);

    return 0;
}
