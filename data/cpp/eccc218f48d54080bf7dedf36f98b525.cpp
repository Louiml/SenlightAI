/*
Given a prime factorization of a positive integer in the form of a list of pairs (prime, exponent), write a C++ function that returns a tuple of three numbers: the total number of divisors, the sum of all divisors, and the product of all divisors, all computed modulo `1e9+7`. The input list may be empty (representing the number 1), may contain primes in any order, and exponents are positive integers. The function must handle large exponents (up to 10^18) and large numbers of factors (up to 10^5). Use fast modular exponentiation, modular inverse for division (since modulus is prime), and careful handling of the product of divisors using modular arithmetic on the exponent modulo `mod-1` due to Fermat's little theorem. The output must be a `std::tuple<ll, ll, ll>` where the first is the divisor count, second is the divisor sum, and third is the divisor product, each modulo `1e9+7`.
*/

#include <cstdint>
#include <tuple>
#include <vector>

using ll = long long;
constexpr ll MOD = 1000000007LL;

// Fast modular exponentiation: (base^exp) % mod
static ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Modular inverse for prime mod using Fermat's little theorem
static ll mod_inv(ll a, ll mod) {
    return mod_pow(a, mod - 2, mod);
}

// Compute number, sum, product of divisors for a prime factorization.
// Input: vector of pairs (prime, exponent). Returns tuple (count, sum, product) modulo MOD.
std::tuple<ll, ll, ll> divisorProperties(const std::vector<std::pair<ll, ll>>& factors) {
    ll count = 1; // total number of divisors
    ll sum = 1;   // sum of divisors
    ll product = 1; // product of divisors

    if (factors.empty()) {
        // Number is 1
        return {1, 1, 1};
    }

    // Part 1: count of divisors
    for (const auto& [p, a] : factors) {
        count = (count * ((a + 1) % MOD)) % MOD;
    }

    // Part 2: sum of divisors = product over (p^(a+1)-1)/(p-1)
    for (const auto& [p, a] : factors) {
        ll numerator = mod_pow(p % MOD, a + 1, MOD);
        numerator = (numerator - 1 + MOD) % MOD;
        ll denominator = (p - 1) % MOD;
        if (denominator < 0) denominator += MOD; // p>=2, but safe
        ll geometric = (numerator * mod_inv(denominator, MOD)) % MOD;
        sum = (sum * geometric) % MOD;
    }

    // Part 3: product of divisors
    // Need to compute product = N^(D/2) where D = product of (a_i+1), 
    // but if all a_i even, use separate formula.
    bool hasOdd = false;
    for (const auto& [p, a] : factors) {
        if (a % 2 == 1) {
            hasOdd = true;
            break;
        }
    }

    if (hasOdd) {
        // Find one odd exponent and half it.
        ll exponentFactor = 1;
        bool usedOne = false;
        for (const auto& [p, a] : factors) {
            ll add = a + 1;
            if (!usedOne && (a % 2 == 1)) {
                add /= 2; // integer division
                usedOne = true;
            }
            exponentFactor = (exponentFactor * (add % (MOD - 1))) % (MOD - 1);
        }
        for (const auto& [p, a] : factors) {
            ll e = (a % (MOD - 1)) * exponentFactor % (MOD - 1);
            product = (product * mod_pow(p % MOD, e, MOD)) % MOD;
        }
    } else {
        // All exponents even.
        ll exponentFactor = 1;
        for (const auto& [p, a] : factors) {
            exponentFactor = (exponentFactor * ((a + 1) % (MOD - 1))) % (MOD - 1);
        }
        for (const auto& [p, a] : factors) {
            ll half = a / 2; // integer since a even
            ll e = (half % (MOD - 1)) * exponentFactor % (MOD - 1);
            product = (product * mod_pow(p % MOD, e, MOD)) % MOD;
        }
    }

    return {count, sum, product};
}

#include <cassert>
#include <utility>
#include <vector>

// The function declaration (same as in solution)
std::tuple<ll, ll, ll> divisorProperties(const std::vector<std::pair<ll, ll>>& factors);

int main() {
    // Example: N = 12 = 2^2 * 3^1
    auto r1 = divisorProperties({{2,2},{3,1}});
    // Divisors: 1,2,3,4,6,12 -> count=6, sum=28, product=1*2*3*4*6*12=1728 mod 1e9+7 = 1728
    assert(std::get<0>(r1) == 6);
    assert(std::get<1>(r1) == 28);
    assert(std::get<2>(r1) == 1728);

    // N = 1 (empty list)
    auto r2 = divisorProperties({});
    assert(std::get<0>(r2) == 1);
    assert(std::get<1>(r2) == 1);
    assert(std::get<2>(r2) == 1);

    // N = p (prime), e.g., p=7
    auto r3 = divisorProperties({{7,1}});
    // Divisors: 1,7 -> count=2, sum=8, product=7
    assert(std::get<0>(r3) == 2);
    assert(std::get<1>(r3) == 8);
    assert(std::get<2>(r3) == 7);

    // N = p^2, p=5 => 25, divisors: 1,5,25 -> count=3, sum=31, product=125
    auto r4 = divisorProperties({{5,2}});
    assert(std::get<0>(r4) == 3);
    assert(std::get<1>(r4) == 31);
    assert(std::get<2>(r4) == 125);

    // N = 2^1 * 2^1? Actually distinct primes: 2^1 * 3^1 = 6 -> divisors 1,2,3,6 -> count=4, sum=12, product=36
    auto r5 = divisorProperties({{2,1},{3,1}});
    assert(std::get<0>(r5) == 4);
    assert(std::get<1>(r5) == 12);
    assert(std::get<2>(r5) == 36);

    // N = 2^10 * 3^10 (all even exponents) -> 6^10? Actually distinct, product of divisors = (2^10 * 3^10)^( (11*11)/2 ) = (6^10)^(121/2=? actually D=121 odd, so product = (2^10*3^10)^(121/2) not integer, but our formula computes as product of p^(a_i/2 * D) = 2^(5*121)*3^(5*121). Let's just compute modulo manually? We'll trust function for known small? For simplicity, test N=16 (2^4) -> divisors:1,2,4,8,16 -> count=5, sum=31, product=1024
    auto r6 = divisorProperties({{2,4}});
    assert(std::get<0>(r6) == 5);
    assert(std::get<1>(r6) == 31);
    assert(std::get<2>(r6) == 1024);

    // N=2^2 * 3^2 = 36 -> divisors:1,2,3,4,6,9,12,18,36 -> count=9, sum=91, product=1*2*3*4*6*9*12*18*36= (product of all divisors) = 36^(9/2)=36^4.5 not integer, but formula gives product = 2^( (2/2)*9 ) * 3^( (2/2)*9 ) = 2^9 * 3^9 = 512*19683=10077696 mod MOD = 10077696
    auto r7 = divisorProperties({{2,2},{3,2}});
    assert(std::get<0>(r7) == 9);
    assert(std::get<1>(r7) == 91);
    assert(std::get<2>(r7) == 10077696LL % 1000000007LL); // 10077696

    return 0;
}

// The task is based on standard number-theoretic formulas. Let the prime factorization be \(N = \prod p_i^{a_i}\). 
// 1. **Number of divisors** = \(\prod (a_i+1)\). Compute directly by multiplying `(a_i+1) % mod` using modular multiplication.
// 2. **Sum of divisors** = \(\prod \frac{p_i^{a_i+1}-1}{p_i-1}\). Compute each geometric sum using fast modular exponentiation for \(p_i^{a_i+1}\), subtract 1, then multiply by the modular inverse of \(p_i-1\) modulo `mod` (since `mod` is prime and \(p_i \neq mod\), denominator is invertible). Multiply all results.
// 3. **Product of divisors** is more subtle. The product of all divisors equals \(N^{d(N)/2}\) where \(d(N)\) is the number of divisors. However, exponent \(d(N)/2\) may not be integer. There are two cases:
//    - If at least one exponent \(a_i\) is odd, then \(d(N)\) is even, so the product is \(N^{d(N)/2}\). We compute the exponent \(E = d(N)/2\) modulo `mod-1` using modular arithmetic, then compute \(N^E\) mod `mod` by multiplying \(p_i^{a_i E}\) for each prime (exponentiation modulo `mod`). Since the actual exponent might be huge, we reduce modulo `mod-1` because modulo prime \(p\), \(a^{p-1} \equiv 1\) when \(a\) not divisible by \(p\). Here primes are less than `mod`, so safe.
//    - If all exponents are even, then \(d(N)\) is odd, and \(d(N)/2\) is not an integer. Instead, we can use the formula: product of divisors = \(\prod p_i^{(a_i/2) \cdot d(N)}\). Since each \(a_i\) is even, \(a_i/2\) is integer. Then compute each factor \(p_i^{(a_i/2) \cdot d(N)}\) mod `mod` by reducing exponent modulo `mod-1`, and multiply. This is equivalent to the general formula, but we must handle the case without a "position" trick.
//
// The code snippet in the prompt uses a "position" variable to detect an odd exponent; if found, it computes outer = (other exponents+1 product) * ((a_pos+1)/2) modulo `mod-1`, then raises each prime to exponent * outer. If all even, it computes outer = product of (a_i+1) modulo `mod-1`, then for each prime raises to (a_i/2) * outer. This actually works for both cases: when one exponent is odd, we can do the "half" on that exponent, and when all even, we half each exponent directly. The reference solution will implement this logically.
//
// Edge cases: empty list means N=1. Then number=1, sum=1, product=1. Prime could equal `mod`? The problem likely assumes primes < mod, but to be safe, if a prime equals `mod`, the formulas involving inverse of (p-1) would fail because inverse of (mod-1) exists, but mod-1 is not zero, so it's fine. But p could be 0? No, primes are positive. The modulus is 1e9+7 which is prime. The inverse of (p-1) exists as long as (p-1) is not a multiple of mod; since p<mod, p-1 < mod and >0, so invertible. Exponents can be large; use `long long` for exponents. Time complexity O(n log exponent) for each modular exponentiation, so O(n log max_exp). Space O(1) besides input storage.
