Write a C++ function `countPrimeMultiplicativeWays` that takes two integers `n` and `m` as input, where \( n \ge 1 \) and \( m \ge 1 \), and returns the number of ordered \( n \)-tuples of positive integers whose product equals \( m \), modulo \( 10^9+7 \). For example, if \( n=2 \) and \( m=6 \), the tuples are \((1,6),(2,3),(3,2),(6,1)\), so the answer is 4. The function must handle large inputs (up to \( n, m \le 10^9 \)) efficiently.

// The problem reduces to counting the number of ways to distribute the prime factors of \( m \) into \( n \) ordered boxes (each box becomes a factor of the tuple). Factorize \( m \) into \( m = \prod p_i^{e_i} \). For each prime \( p_i \) with exponent \( e_i \), the number of ways to distribute \( e_i \) identical prime factors into \( n \) distinct boxes is the number of weak compositions: \(\binom{n+e_i-1}{e_i} = \binom{n+e_i-1}{n-1}\). Since the distributions for different primes are independent, the total answer is the product over all primes \( p_i \) of \(\binom{n+e_i-1}{e_i} \mod (10^9+7)\). Edge cases: if \( m=1 \), there are no primes, so the product is empty and the answer is 1 (each box must be 1). Also, if \( m \) itself is a prime after dividing out small factors, the exponent is 1, so the contribution is \(\binom{n}{1} = n\). The complexity is \(O(\sqrt{m})\) for trial division factorization, plus \(O(\log \text{mod})\) for modular inverses using Fermat's little theorem (since mod is prime and we need inverses of factorials). Space complexity is \(O(\sqrt{m})\) or \(O(\log m)\) if we precompute factorials up to \(n+\log_2 m\) but here we only need factorials up to \( n + \max e_i \), and \( \max e_i \le \log_2 m \le 30 \), so we can compute factorials on the fly up to \(n+30\) — that's \(O(n)\) space, which is too large for \(n\) up to \(10^9\). Instead, we compute each binomial coefficient directly using multiplicative formula with modular inverse for small exponents, avoiding large factorial precomputation. We only need inverses of small numbers (up to \(n\) and up to \(e_i\)), which we can compute via modular exponentiation in \(O(\log \text{mod})\) each. Total time: \(O(\sqrt{m} \cdot \log \text{mod} + \#\text{primes} \cdot \log \text{mod})\). For \(m \le 10^9\), \(\sqrt{m} \le 31623\), fine.

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
constexpr int64 MOD = 1000000007LL;

// Modular exponentiation: b^p % MOD
int64 mod_pow(int64 b, int64 p) {
    int64 result = 1;
    b %= MOD;
    while (p > 0) {
        if (p & 1) result = result * b % MOD;
        b = b * b % MOD;
        p >>= 1;
    }
    return result;
}

// Modular inverse using Fermat's little theorem (MOD is prime)
int64 mod_inv(int64 x) {
    return mod_pow(x, MOD - 2);
}

// Count ordered n-tuples of positive integers with product m, modulo MOD
int64 countPrimeMultiplicativeWays(int64 n, int64 m) {
    if (m == 1) return 1; // only all ones

    int64 ans = 1;
    int64 remaining = m;

    // Trial division up to sqrt(m)
    for (int64 p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            int64 e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                e++;
            }
            // Multiply by C(n + e - 1, e) = (n+e-1)! / (e! * (n-1)!)
            // Compute using product formula: ∏_{k=1}^{e} (n - 1 + k) / k
            int64 term = 1;
            for (int64 k = 1; k <= e; ++k) {
                term = term * ((n - 1 + k) % MOD) % MOD;
                term = term * mod_inv(k % MOD) % MOD;
            }
            ans = ans * term % MOD;
        }
    }
    // If remaining > 1, it's a prime with exponent 1, contribution C(n,1)=n
    if (remaining > 1) {
        ans = ans * (n % MOD) % MOD;
    }
    return ans;
}

#include <cassert>
#include <cstdint>

int main() {
    // Basic examples
    assert(countPrimeMultiplicativeWays(1, 1) == 1);           // (1)
    assert(countPrimeMultiplicativeWays(1, 6) == 1);           // only (6)
    assert(countPrimeMultiplicativeWays(2, 6) == 4);           // (1,6),(2,3),(3,2),(6,1)
    assert(countPrimeMultiplicativeWays(3, 6) == 9);           // distribute 2^1 and 3^1 into 3 boxes: 3*3

    // Prime m
    assert(countPrimeMultiplicativeWays(5, 7) == 5);           // one prime factor, n choices

    // m = p^e
    assert(countPrimeMultiplicativeWays(2, 4) == 3);           // (1,4),(2,2),(4,1)
    assert(countPrimeMultiplicativeWays(3, 8) == 10);          // C(3+3-1,3)=10

    // Larger values, check consistency with known combinatorics
    // For n=10, m=2^5 -> C(10+5-1,5) = C(14,5)=2002
    assert(countPrimeMultiplicativeWays(10, 32) == 2002);

    // Mod correctness for large n, small m
    int64 mod = 1000000007LL;
    assert(countPrimeMultiplicativeWays(1000000000LL, 2) == 1000000000LL % mod);

    // Multiple primes
    assert(countPrimeMultiplicativeWays(2, 12) == 6);          // 2^2 * 3: C(2+2-1,2)=3 * C(2,1)=2 => 6
    assert(countPrimeMultiplicativeWays(4, 12) == 40);         // C(4+2-1,2)=10 * C(4,1)=4 => 40

    // m=1 for any n
    assert(countPrimeMultiplicativeWays(123, 1) == 1);

    // Edge: n=1 -> only one tuple, always 1
    assert(countPrimeMultiplicativeWays(1, 100) == 1);
    assert(countPrimeMultiplicativeWays(1, 99991) == 1);       // prime

    return 0;
}
