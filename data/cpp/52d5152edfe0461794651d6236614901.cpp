// You are given an integer `n` and then a sequence of `n` integers where each value is either `1` or `2`. Write a C++ function `arrangeCount(long long n, const std::vector<long long>& values)` that returns the number of distinct permutations of this multiset such that no two `2`s are adjacent. The result must be computed modulo `1'000'000'007`. The function should handle up to `n = 10^6` values and must be efficient. Note that the original snippet had a special condition: if all values are `2` or `1`, the result is `0` or `1` respectively, and the formula is based on binomial coefficients combined with factorial of the count of `2`s, but you must derive the general correct recurrence: first, count the number of ways to arrange the `2`s in the gaps between `1`s, then multiply by the factorial of the number of `1`s (which are distinct by position). If `b` (number of `2`s) is greater than `a+1` (where `a` is number of `1`s), no valid arrangement exists, so return `0`. Otherwise, the number of ways is `C(a+1, b) * a! mod`, but you must also consider that the `1`s are distinct and the `2`s are distinct, so the actual count is `(a! * (a+1)! / ((a+1-b)!)) mod`? Carefully derive the correct formula: treat the `1`s as distinct placeholders that create `a+1` gaps (including ends), choose `b` gaps for the distinct `2`s, then multiply by `b!` for ordering the `2`s and `a!` for ordering the `1`s. So the total is `a! * b! * C(a+1, b)`. Implement this efficiently using precomputed factorials and modular inverses, or using a recurrence if you prefer. Provide a function that takes `n` and the vector and returns the correct count modulo `1e9+7`.

Let `a` = number of 1s, `b` = number of 2s. If `b == 0`, any arrangement of distinct 1s is valid, so result = `a!`. If `a == 0`: if `b > 1`, no valid arrangement (two 2s adjacent), so result = 0; if `b == 1`, result = 1 (the single 2). General: place the `a` distinct 1s first. This creates `a+1` gaps (before first 1, between consecutive 1s, and after last 1). To avoid adjacent 2s, each gap can contain at most one 2. So we need to choose `b` gaps out of `a+1` to place the `b` distinct 2s. The number of ways to select gaps is `C(a+1, b)`, then arrange the `b` distinct 2s in those gaps in `b!` ways, and arrange the `a` distinct 1s in `a!` ways. Total = `a! * b! * C(a+1, b)`. If `b > a+1`, no solution, return 0. To compute efficiently for large `n` (up to 1e6), precompute factorials and inverse factorials up to `n+1` (since `a+1` ≤ n). Use modular exponentiation for inverses. Complexity: O(n) for precomputation, O(1) for each query. Space: O(n) for factorial arrays. Edge cases: `n=0` not needed but handle gracefully; all 1s, all 2s (b=1 works, b>1 returns 0). The modulo is prime, so use Fermat's little theorem for inverses. Time: O(n), space O(n).

#include <vector>
#include <cstdint>

// Compute a^b mod mod using fast exponentiation.
static long long modPow(long long a, long long b, long long mod) {
    long long res = 1;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

// Return number of valid permutations modulo 1e9+7.
long long arrangeCount(long long n, const std::vector<long long>& values) {
    const long long MOD = 1000000007LL;
    long long a = 0, b = 0;
    for (long long v : values) {
        if (v == 1) ++a;
        else if (v == 2) ++b;
        // ignore invalid values if any, but per problem only 1,2 appear.
    }
    // If no 2s, all permutations of distinct 1s.
    if (b == 0) {
        // compute a! mod
        long long fact = 1;
        for (long long i = 2; i <= a; ++i) fact = fact * i % MOD;
        return fact;
    }
    // If no 1s
    if (a == 0) {
        return b == 1 ? 1 : 0;
    }
    // If more 2s than gaps between 1s (a+1 gaps), impossible.
    if (b > a + 1) {
        return 0;
    }
    // Precompute factorials up to a+1 (since n is given, but we just need up to a+1)
    long long maxVal = a + 1;  // because we need (a+1)!
    std::vector<long long> fact(maxVal + 1);
    fact[0] = 1;
    for (long long i = 1; i <= maxVal; ++i) fact[i] = fact[i-1] * i % MOD;
    // invFact for factorial inverse using Fermat
    std::vector<long long> invFact(maxVal + 1);
    invFact[maxVal] = modPow(fact[maxVal], MOD-2, MOD);
    for (long long i = maxVal; i > 0; --i) {
        invFact[i-1] = invFact[i] * i % MOD;
    }
    // C(a+1, b) = fact[a+1] * invFact[b] * invFact[a+1-b]
    long long choose = fact[a+1] * invFact[b] % MOD * invFact[a+1 - b] % MOD;
    long long ans = fact[a] * fact[b] % MOD;
    ans = ans * choose % MOD;
    return ans;
}

#include <cassert>
#include <vector>

long long arrangeCount(long long, const std::vector<long long>&); // declaration

int main() {
    // Test cases
    // a=2, b=0 -> C(3,0)=1
    assert(arrangeCount(2, {1,1}) == 1);
    // a=2, b=1 -> C(3,1)=3
    assert(arrangeCount(3, {1,2,1}) == 3);
    // a=1, b=1 -> C(2,1)=2
    assert(arrangeCount(2, {1,2}) == 2);
    // a=0, b=1 -> 1
    assert(arrangeCount(1, {2}) == 1);
    // a=0, b=2 -> 0 (two 2s adjacent)
    assert(arrangeCount(2, {2,2}) == 0);
    // a=3, b=2 -> C(4,2)=6
    assert(arrangeCount(5, {1,2,1,2,1}) == 6);
    // a=0, b=0 -> 1 (empty)
    assert(arrangeCount(0, {}) == 1);
    // a=1, b=3 -> b > a+1 (3>2) -> 0
    assert(arrangeCount(4, {1,2,2,2}) == 0);
    // large case: a=1000000, b=0 -> 1
    std::vector<long long> big(1000000, 1);
    assert(arrangeCount(1000000, big) == 1);
    // a=4, b=2 -> C(5,2)=10
    assert(arrangeCount(6, {1,1,2,1,2,1}) == 10);
    return 0;
}
