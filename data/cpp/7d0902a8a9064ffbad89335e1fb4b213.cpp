Write a C++ function named `nCrModP` that takes three parameters: `n`, `r`, and `p` (all of type `long long`), where `0 ≤ r ≤ n ≤ 1,000,000` and `p` is a prime number (not necessarily the global constant in the snippet, but typically a large prime like `1000003`). The function must return the binomial coefficient \( \binom{n}{r} \mod p \). Precompute factorials modulo `p` up to `1,000,000` once (e.g., in a separate initialization function or lazily) and implement modular exponentiation to compute the modular inverse. The function must handle the edge case where `r` is `0` or `n` (the result is `1`), and also handle inputs where `n` is larger than the precomputed limit? Actually, the problem guarantees `n ≤ 1,000,000`. The function must not use global variables; instead, it should accept a precomputed `vector<long long>&` of factorials as an additional parameter, or you can design it to compute factorials on the fly up to `n` (but that would be inefficient for multiple calls). For this task, assume the function is called for a single pair `(n, r)`, so you may compute factorials up to `n` inside the function each time, but the expected solution is to precompute once and then answer many queries. Therefore, provide two functions: `precomputeFactorials(maxN, p)` that fills a global or passed vector, and `nCrModP(n, r, p, fact)` that uses it. Since the task asks for a single function, define `nCrModP(n, r, p)` that internally uses a static `vector<long long>` filled on first use (lazy initialization) and precomputes up to `1,000,000` per that prime `p`. The function must be self-contained and usable for multiple calls with the same modulus. If a different modulus is passed, the precomputation must be redone (e.g., store the modulus alongside the factorial vector). Ensure the function returns the correct result for all valid inputs, and use modular arithmetic to avoid overflow (multiplication may exceed 64-bit, so use the fact that `p` is at most around `10^7` to fit in 64-bit, but be safe). The code must be efficient and handle the full range `n` up to `1,000,000`.
#include <cassert>
#include <iostream>

// The solution function is declared above; here we test it.
int main() {
    // Test small values with known results
    assert(nCrModP(5, 2, 1000003) == 10);
    assert(nCrModP(5, 0, 1000003) == 1);
    assert(nCrModP(5, 5, 1000003) == 1);
    assert(nCrModP(10, 3, 1000003) == 120);
    assert(nCrModP(0, 0, 1000003) == 1);
    // Test with a different prime modulus
    assert(nCrModP(4, 2, 7) == 6 % 7); // 6
    assert(nCrModP(6, 3, 7) == 20 % 7); // 6
    assert(nCrModP(6, 3, 13) == 20 % 13); // 7
    // Test a larger value (with known mod result)
    // 1000 choose 500 mod 1000003 is not trivial, but we can compute with a small pair
    assert(nCrModP(1000000, 0, 1000003) == 1);
    assert(nCrModP(1000000, 1, 1000003) == 1000000 % 1000003);
    // Edge cases: r=n and r=0 for n=1e6
    assert(nCrModP(1000000, 1000000, 1000003) == 1);

    // Test that the function works with multiple calls (cached factorials)
    assert(nCrModP(4, 2, 1000003) == 6);
    assert(nCrModP(5, 4, 1000003) == 5);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <stdexcept>

// Compute nCr % p, where p is prime, n <= 1,000,000.
// Uses lazy precomputation of factorials modulo p.
long long nCrModP(long long n, long long r, long long p) {
    // Validate input
    if (r < 0 || r > n || n < 0 || p <= 1) {
        throw std::invalid_argument("Invalid input to nCrModP");
    }

    // Static storage for factorials and the modulus they were computed with.
    static std::vector<long long> fact;
    static long long mod = -1;
    static const long long MAXN = 1000000;

    // Recompute factorials if modulus changed or first call.
    if (mod != p || fact.empty()) {
        mod = p;
        fact.resize(MAXN + 1);
        fact[0] = 1;
        for (long long i = 1; i <= MAXN; ++i) {
            fact[i] = (fact[i - 1] * i) % p;
        }
    }

    // If n is larger than precomputed max, we could extend (but not required per problem).
    // For safety, we check and throw if exceeded.
    if (n > MAXN) {
        throw std::out_of_range("n exceeds precomputed limit");
    }

    // Modular exponentiation: base^exp % mod
    auto modPow = [&](long long base, long long exp) -> long long {
        long long result = 1;
        base %= p;
        while (exp > 0) {
            if (exp & 1) result = (result * base) % p;
            base = (base * base) % p;
            exp >>= 1;
        }
        return result;
    };

    // n! mod p
    long long numerator = fact[n];
    // r! * (n-r)! mod p
    long long denominator = (fact[r] * fact[n - r]) % p;
    // Modular inverse of denominator using Fermat's little theorem (p is prime)
    long long denominatorInv = modPow(denominator, p - 2);
    long long ans = (numerator * denominatorInv) % p;
    return ans;
}
// The binomial coefficient \( \binom{n}{r} = \frac{n!}{r!(n-r)!} \) modulo a prime `p` can be computed using Fermat’s little theorem: the modular inverse of `b` modulo `p` is `b^(p-2) mod p`. Thus, the formula is: `fact[n] * modInverse(fact[r] * fact[n-r]) % p`. To avoid recomputing factorials for each query, precompute `fact[i] = (i! mod p)` for all `i` from 0 to `maxN` (here 1,000,000) once. The modulo inverse is computed via fast exponentiation (binary exponentiation) in `O(log p)` time. The overall time complexity per query is `O(log p)` after `O(maxN)` preprocessing, and space complexity is `O(maxN)` for the factorial array. Edge cases: `r == 0` or `r == n` gives `1` (since `0!` is `1`). Also ensure that `n` is not greater than the precomputed size; if a new maximum `n` is encountered and is larger than the current vector size, we need to extend the precomputation (or assume `n` is within the given bound). Since the problem specifies `n ≤ 1,000,000`, we can precompute exactly that many. To make the function self-contained, use a static local variable for the factorial vector and the modulus, and check if the passed modulus differs from the stored one; if so, recompute. This handles multiple calls with different prime moduli. Use `long long` for all intermediate values and take modulo after each multiplication to avoid overflow.
