// Write a C++ function that computes the number of ways to distribute `m` indistinguishable balls into `n` distinguishable boxes, where each box can hold any number of balls (including zero). The result must be computed modulo `1000000007` (which is prime). The function should take two integers `n` (number of boxes) and `m` (number of balls) where `1 ≤ n, m ≤ 10^6`, and return an `int` (or `long long`) containing the answer. This is a classic "stars and bars" combinatorial problem, and the function must handle the large constraints efficiently by precomputing factorials up to a sufficient limit and using modular inverse via Fermat's Little Theorem.

// The number of ways to distribute `m` identical balls into `n` distinct boxes is given by the combinatorial formula `C(m + n - 1, m)`, which counts the number of weak compositions of `m` into `n` parts. To compute this modulo a prime `mod = 1e9+7` efficiently, we precompute factorials modulo `mod` up to the maximum possible `m + n - 1 = 2e6` (since both `n` and `m` can be up to `1e6`). Then `C(n, k) = fact[n] * inv(fact[k]) * inv(fact[n-k]) mod mod`. The modular inverse of a number `x` modulo a prime `mod` is `x^(mod-2) mod mod` by Fermat's Little Theorem, computed via fast exponentiation. Edge cases: if `m` or `n` is 0, the formula still works (though constraints say ≥1). The time complexity is `O(MAX)` for precomputing factorials, plus `O(log mod)` for the modular exponentiation, and `O(1)` per query. Space complexity is `O(MAX)` for the factorial array, where `MAX = 2e6+5`.

#include <vector>

// Precompute factorials modulo MOD up to a given limit.
std::vector<long long> precomputeFactorials(int limit, long long mod) {
    std::vector<long long> fact(limit + 1);
    fact[0] = 1;
    for (int i = 1; i <= limit; ++i) {
        fact[i] = fact[i - 1] * i % mod;
    }
    return fact;
}

// Compute (base^exp) % mod using binary exponentiation.
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = result * base % mod;
        }
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Compute modular inverse of a modulo mod (mod must be prime).
long long modInverse(long long a, long long mod) {
    return modPow(a, mod - 2, mod);
}

// Compute C(n, k) modulo MOD using precomputed factorials.
long long nCk(int n, int k, const std::vector<long long>& fact, long long mod) {
    if (k < 0 || k > n) {
        return 0;
    }
    long long numerator = fact[n];
    long long denominator = fact[k] * fact[n - k] % mod;
    return numerator * modInverse(denominator, mod) % mod;
}

// Return the number of ways to distribute m identical balls into n distinct boxes.
long long distributeBalls(int boxes, int balls) {
    const long long MOD = 1000000007LL;
    // Maximum n + m - 1 = boxes + balls - 1 can be up to 2e6+1.
    int maxIndex = boxes + balls - 1;
    static const int LIMIT = 2000005; // Since boxes, balls <= 1e6
    static std::vector<long long> fact = precomputeFactorials(LIMIT, MOD);
    // The answer is C(boxes + balls - 1, balls)
    return nCk(boxes + balls - 1, balls, fact, MOD);
}

#include <cassert>

int main() {
    // Simple single-ball distributions
    assert(distributeBalls(1, 1) == 1);  // Only one box, one way
    assert(distributeBalls(2, 1) == 2);  // 2 boxes, 1 ball -> 2 ways
    assert(distributeBalls(3, 1) == 3);  // 3 boxes, 1 ball -> 3 ways

    // Two balls
    assert(distributeBalls(2, 2) == 3);  // (2,0),(1,1),(0,2)
    assert(distributeBalls(3, 2) == 6);  // C(4,2) = 6

    // Larger values without overflow, verify using small known results
    assert(distributeBalls(5, 3) == 35); // C(7,3)=35
    assert(distributeBalls(10, 5) == 2002); // C(14,5)=2002

    // Edge case: one box
    assert(distributeBalls(1, 1000000) == 1); // All balls in one box

    // Large values modulo check (compare with direct small case)
    // C(8,4)=70, so boxes=5, balls=4? Actually C(5+4-1,4)=C(8,4)=70
    assert(distributeBalls(5, 4) == 70);

    // Modular check: 2 boxes and 1e6 balls should be 1e6+1 mod MOD
    // (C(1+1e6,1e6) = 1e6+1)
    assert(distributeBalls(2, 1000000) == 1000001LL);

    return 0;
}
