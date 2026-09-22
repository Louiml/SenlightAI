Write a standalone C++ function that, given three integers `n`, `r`, and a prime modulus `MOD`, computes the binomial coefficient C(n, r) modulo MOD using precomputed factorials and modular inverse via Fermat’s Little Theorem. The function must handle large inputs (up to 100,000) efficiently and return the result as a 64-bit integer. Assume MOD is a prime (specifically 1000000007, which is prime). The function should throw an `std::invalid_argument` or return a sentinel value if `r > n`, and must correctly handle edge cases such as `r = 0`, `r = n`, and `n = 0`. You are allowed to precompute factorials up to a fixed maximum size (e.g., 100005) inside the function using a static local array with lazy initialization.
#include <cassert>
#include <stdexcept>

int main() {
    // Basic cases
    assert(nCrModPrime(5, 2) == 10);
    assert(nCrModPrime(5, 0) == 1);
    assert(nCrModPrime(5, 5) == 1);
    assert(nCrModPrime(0, 0) == 1);
    assert(nCrModPrime(1, 1) == 1);
    assert(nCrModPrime(10, 3) == 120);

    // Large values within limit
    assert(nCrModPrime(100000, 2) == 999985000);
    assert(nCrModPrime(100000, 100000) == 1);

    // Edge case: n=1, r=0 and r=1
    assert(nCrModPrime(1, 0) == 1);
    assert(nCrModPrime(1, 1) == 1);

    // Invalid inputs should throw
    bool threw = false;
    try { nCrModPrime(5, 6); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { nCrModPrime(-1, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { nCrModPrime(5, -1); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { nCrModPrime(100001, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Verify consistency: C(n,r) == C(n,n-r)
    assert(nCrModPrime(7, 3) == nCrModPrime(7, 4));
    assert(nCrModPrime(20, 8) == nCrModPrime(20, 12));
}
#include <stdexcept>
#include <cstdint>

// Computes C(n, r) modulo MOD (prime 1000000007) using precomputed factorials
// and modular inverse via Fermat's Little Theorem.
// Throws std::invalid_argument if n < 0, r < 0, or r > n.
// The maximum supported n is 100000.
int64_t nCrModPrime(int64_t n, int64_t r) {
    constexpr int64_t MOD = 1000000007LL;
    constexpr int MAX_N = 100000;

    if (n < 0 || r < 0 || r > n) {
        throw std::invalid_argument("Invalid parameters for nCr");
    }
    if (n > MAX_N) {
        throw std::invalid_argument("n exceeds precomputed limit");
    }

    // Static local array with lazy initialization
    static int64_t fact[MAX_N + 1];
    static bool initialized = false;
    if (!initialized) {
        fact[0] = 1;
        for (int i = 1; i <= MAX_N; ++i) {
            fact[i] = (fact[i - 1] * i) % MOD;
        }
        initialized = true;
    }

    // Fast exponentiation: base^exp % MOD
    auto modPow = [&](int64_t base, int64_t exp) -> int64_t {
        int64_t result = 1;
        while (exp > 0) {
            if (exp & 1) {
                result = (result * base) % MOD;
            }
            base = (base * base) % MOD;
            exp >>= 1;
        }
        return result;
    };

    // Fermat: inv(x) = x^(MOD-2) mod MOD
    int64_t inv_fact_r = modPow(fact[r], MOD - 2);
    int64_t inv_fact_nr = modPow(fact[n - r], MOD - 2);

    int64_t ans = fact[n];
    ans = (ans * inv_fact_r) % MOD;
    ans = (ans * inv_fact_nr) % MOD;
    return ans;
}
// The solution precomputes factorials modulo MOD for indices 0 through a maximum value (e.g., 100000) once, using a static local array to ensure the computation happens only on the first call. The factorial recurrence is `fact[i] = (i * fact[i-1]) % MOD`. To compute C(n, r), we use the formula: C(n, r) = fact[n] * inv(fact[r]) % MOD * inv(fact[n-r]) % MOD, where inv(x) is the modular inverse of x modulo MOD. Since MOD is prime, Fermat’s Little Theorem gives inv(x) = x^(MOD-2) % MOD, computed via fast exponentiation in O(log MOD) time. Direct computation of inverses for each query would be costly, so we precompute factorials but not inverses; instead, we compute inverse of the two factorials on demand using fast power. However, for efficiency, we could precompute inverse factorials as well, but for simplicity and clarity here, we compute two fast exponentiations per call, which is still O(log MOD) per query and acceptable given typical constraints. Edge cases: if `r > n`, return 0 or throw. If `r < 0` or `n < 0`, throw invalid_argument. If `n` exceeds the precomputed maximum (100000), we could either extend or throw, but here we assume input is within bounds. Time complexity: precomputation O(MAX) for factorials, plus each query O(log MOD) for two power computations. Space: O(MAX) for factorial array. Total for T queries: O(MAX + T log MOD).
