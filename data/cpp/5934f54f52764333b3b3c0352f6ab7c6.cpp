// Given a list of rational numbers, where each number is represented by its numerator \(a_i\) and denominator \(b_i\) (both positive integers up to \(10^9\)), write a C++ function `sumRationalsModulo` that computes the sum of these fractions modulo \(1{,}000{,}000{,}007\) (a prime). For each fraction, reduce it to its simplest form (divide numerator and denominator by their greatest common divisor), then compute its modular inverse using Fermat's Little Theorem (since the modulus is prime), and accumulate the contribution \(a_i \cdot (b_i^{-1} \bmod M)\). Return the final sum modulo \(M\). The input is provided as two vectors of equal length: a vector of numerators and a vector of denominators. The function must be exception-free, handle at most \(10^5\) fractions, and be efficient enough for large inputs.

The core idea is to treat each fraction \(a/b\) as \(a \cdot b^{-1} \pmod{M}\), where \(M = 1\,000\,000\,007\). Since \(M\) is prime and every \(b_i\) is not divisible by \(M\) (as \(b_i \leq 10^9 < M\)), the modular inverse exists. We compute the inverse via fast exponentiation \(b^{M-2} \bmod M\) using Fermat's Little Theorem. We first reduce each fraction by dividing numerator and denominator by their GCD, which does not change the rational value but simplifies the numbers (though mathematically optional, it is shown in the snippet). We then compute `(a * modInv(b)) % M` and accumulate over all fractions, taking modulo after each addition to avoid overflow (using `long long` for intermediate products). Edge cases include denominators that are already 1 (inverse is 1), and the case of a single fraction. The time complexity is \(O(k \log M)\) where \(k\) is the number of fractions, since each inversion uses binary exponentiation taking \(O(\log M)\) multiplications, and each GCD is \(O(\log \max(a,b))\). Space complexity is \(O(1)\) auxiliary.

#include <vector>
#include <numeric>
#include <cstdint>

const long long MOD = 1000000007LL;

// Fast modular exponentiation: base^exp % MOD
long long modPow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Compute (a/b) mod MOD as a * b^{-1} mod MOD, with b positive.
long long sumRationalsModulo(const std::vector<long long>& numerators,
                             const std::vector<long long>& denominators) {
    long long total = 0;
    const size_t k = numerators.size();
    for (size_t i = 0; i < k; ++i) {
        long long a = numerators[i];
        long long b = denominators[i];

        // Reduce the fraction to simplest form (optional but good practice).
        long long g = std::gcd(a, b);
        a /= g;
        b /= g;

        // b is guaranteed < MOD and positive, so inverse exists.
        long long inv_b = modPow(b, MOD - 2);
        long long term = (a % MOD) * inv_b % MOD;
        total = (total + term) % MOD;
    }
    return total;
}

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Single fraction: 1/2 = 500000004 mod 1e9+7
    {
        std::vector<long long> num = {1};
        std::vector<long long> den = {2};
        assert(sumRationalsModulo(num, den) == 500000004LL);
    }
    // Two fractions: 1/3 + 1/3 = 2/3 = 666666672 (since 2*333333336 = 666666672)
    {
        std::vector<long long> num = {1, 1};
        std::vector<long long> den = {3, 3};
        assert(sumRationalsModulo(num, den) == 666666672LL);
    }
    // Fractions that reduce: 2/4 = 1/2 -> 500000004
    {
        std::vector<long long> num = {2};
        std::vector<long long> den = {4};
        assert(sumRationalsModulo(num, den) == 500000004LL);
    }
    // Mixed: 1/2 + 1/4 = 3/4 = 3 * 250000002 = 750000006
    {
        std::vector<long long> num = {1, 1};
        std::vector<long long> den = {2, 4};
        assert(sumRationalsModulo(num, den) == 750000006LL);
    }
    // Sum of fractions that reduce to integers: 2/1 + 3/1 = 5
    {
        std::vector<long long> num = {2, 3};
        std::vector<long long> den = {1, 1};
        assert(sumRationalsModulo(num, den) == 5LL);
    }
    // Denominators that are large but less than MOD: 1/1000000000
    // Inverse of 1e9 modulo 1e9+7 is computed correctly.
    {
        std::vector<long long> num = {1};
        std::vector<long long> den = {1000000000LL};
        long long expected = 570000004LL; // (1000000000)^(MOD-2) mod MOD known value
        assert(sumRationalsModulo(num, den) == expected);
    }
    // Empty input should return 0 (though not specified, handle gracefully)
    {
        std::vector<long long> num = {};
        std::vector<long long> den = {};
        assert(sumRationalsModulo(num, den) == 0LL);
    }
    return 0;
}
