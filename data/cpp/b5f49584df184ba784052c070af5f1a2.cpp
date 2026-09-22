Given an integer `n` (1 ≤ n ≤ 10^5), an integer base `x` (1 ≤ x ≤ 10^9), and a sequence of `n` integers `a[0], a[1], ..., a[n-1]` (each between 0 and 10^9), write a C++ function `long long evaluatePolynomial(const std::vector<int>& a, int x)` that computes the value of the polynomial `a[0]*x^(n-1) + a[1]*x^(n-2) + ... + a[n-2]*x + a[n-1]`, where terms are combined left to right starting from the highest degree. The result must be returned modulo `1'000'000'007` (a prime). The function must handle very large intermediate powers of `x` efficiently using fast modular exponentiation (binary exponentiation). The input coefficients may be large, and all arithmetic should be performed modulo the given modulus. Assume the modulus is constant and no overflow should occur during intermediate multiplications when using 64-bit integers.
#include <cassert>
#include <vector>

int main() {
    // Test 1: simple polynomial x^2 + 2x + 3 with x=5 => 25+10+3=38
    assert(evaluatePolynomial({1, 2, 3}, 5) == 38);

    // Test 2: constant polynomial: a[0] only, x=10 => 7
    assert(evaluatePolynomial({7}, 10) == 7);

    // Test 3: zero base, exponent >0: 0^2 = 0 => 0 + 0 + 3 = 3
    assert(evaluatePolynomial({1, 2, 3}, 0) == 3);

    // Test 4: large x and exponents: use n=3, x=1e9, coefficients [1,1,1]
    // 1 * (1e9)^2 + 1*1e9 + 1 mod MOD
    long long expected = (modPow(1000000000LL, 2, MOD) + 1000000000LL + 1) % MOD;
    assert(evaluatePolynomial({1, 1, 1}, 1000000000) == expected);

    // Test 5: coefficients that are large but modulo is applied
    // a = [1e9, 1e9], n=2, x=2 => 1e9*2 + 1e9 = 3e9 mod MOD
    assert(evaluatePolynomial({1000000000, 1000000000}, 2) == (3000000000LL % MOD));

    // Test 6: n=4, x=2, polynomial: 1*8 + 2*4 + 3*2 + 4 = 8+8+6+4=26
    assert(evaluatePolynomial({1, 2, 3, 4}, 2) == 26);

    // Test 7: all-zero coefficients gives 0
    assert(evaluatePolynomial({0, 0, 0}, 123456) == 0);

    // Test 8: negative coefficients: -1*x + 5 => with x=3 => -3+5=2
    assert(evaluatePolynomial({-1, 5}, 3) == 2);

    // Test 9: single coefficient with x large and exponent 0 => a[0] % MOD
    assert(evaluatePolynomial({123456789}, 999999937) == 123456789 % MOD);

    // Test 10: n=5, x=3, polynomial 2x^4 + 0x^3 + 1x^2 + 0x + 4
    // 2*81 + 0 + 1*9 + 0 + 4 = 162+9+4=175
    assert(evaluatePolynomial({2, 0, 1, 0, 4}, 3) == 175);

    return 0;
}
#include <vector>

const long long MOD = 1000000007LL;

// Fast modular exponentiation: returns (base^exp) % mod
long long modPow(long long base, long long exp, long long mod) {
    base %= mod;
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Evaluate polynomial a[0]*x^(n-1) + a[1]*x^(n-2) + ... + a[n-1] modulo MOD
long long evaluatePolynomial(const std::vector<int>& a, int x) {
    int n = static_cast<int>(a.size());
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        long long coeff = (a[i] % MOD + MOD) % MOD; // handle potential negative if needed
        long long power = modPow(static_cast<long long>(x), static_cast<long long>(n - 1 - i), MOD);
        sum = (sum + coeff * power) % MOD;
    }
    return sum;
}
// The goal is to evaluate a polynomial given in coefficient form where the first coefficient corresponds to the highest power of `x`. A naive approach would compute each power `x^(n-1-i)` separately and multiply by `a[i]`, which could take O(n log n) time overall due to repeated exponentiation, but that's acceptable for n up to 10^5 only if each exponentiation is O(log n) — resulting in O(n log n) time. However, a more efficient approach is to use Horner's method: start from the highest-degree coefficient and repeatedly multiply by `x` and add the next coefficient. But since the given snippet used binary exponentiation for each term, we can also follow that pattern: compute `binpow(x, n-1-i, MOD)` for each `i` using fast exponentiation (O(log n) per term). The total time is O(n log n) and space O(1) beyond storing the input vector. Edge cases include: `n = 1` (only the constant term), `x = 0` (then `0^0` should be treated as 1 if the exponent is 0; otherwise `0` for positive exponents — handle by using `binpow` which returns 1 for exponent 0), and large coefficients that need modulo reduction before multiplication. Because the modulus is prime, `binpow` is valid for all bases. All intermediate products fit in 64-bit because `MOD` is about 1e9, and `(a%MOD) * (b%MOD)` is at most ~1e18, which fits in `long long` (signed 64-bit, max ~9e18). The modulo operation must be applied after each multiplication to keep values small.
