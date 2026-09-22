// Write a C++ function `modular_exponentiation` that computes \(a^b \bmod m\) for non-negative integers \(a\), \(b\), and a positive modulus \(m\), using the fast exponentiation by squaring method. The function must handle the case where \(a\) or intermediate results could exceed the range of `int`, so it should use `long long` for all arithmetic. It must also correctly return \(1\) when \(b = 0\), even if \(a = 0\) or \(m = 1\) (in which case the result is \(0\) for all other inputs). The function should take three arguments and return a `long long` value.
// The standard approach is binary exponentiation: initialize `result = 1`, then while the exponent `b` is greater than 0, check if the least significant bit of `b` is set (`b & 1`). If so, multiply `result` by `a` modulo `m`. Then square `a` modulo `m` and shift `b` right by one bit. This reduces the number of multiplications from \(O(b)\) to \(O(\log b)\). Edge cases: when `b == 0`, the loop does not execute and `result` remains `1` (correct even if `m == 1`). When `m == 1`, any multiplication modulo 1 yields `0`, so the function returns `0` for any `b > 0`; for `b == 0` it returns `1` (which is mathematically consistent with the definition \(a^0 = 1\) modulo 1, though some might expect 0, but it's safe to define). Negative exponents are not in scope (inputs are non-negative). Time complexity is \(O(\log b)\), space is \(O(1)\) auxiliary.
#include <cstdint>

// Computes (a^b) % m using fast exponentiation by squaring.
// Requires: b >= 0, m > 0. Uses long long to avoid overflow.
long long modular_exponentiation(long long a, long long b, long long m) {
    long long result = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(modular_exponentiation(3, 2, 2) == 1); // 9 % 2 = 1
    assert(modular_exponentiation(2, 10, 1000) == 24); // 1024 % 1000
    assert(modular_exponentiation(5, 0, 7) == 1); // anything^0 = 1
    assert(modular_exponentiation(0, 5, 10) == 0); // 0^5 = 0
    // Large exponent and base
    assert(modular_exponentiation(123456789, 987654321, 1000000007) == 917152219); // known value
    // Edge with modulus 1
    assert(modular_exponentiation(7, 3, 1) == 0); // mod 1 = 0 for positive exponent
    assert(modular_exponentiation(7, 0, 1) == 1); // b==0 returns 1
    // a larger than modulus
    assert(modular_exponentiation(1000000000000LL, 2, 7) == 1); // (10^12)^2 mod 7
    // Check with small values
    assert(modular_exponentiation(4, 3, 5) == 4); // 64 % 5 = 4
    assert(modular_exponentiation(1, 1000000, 999) == 1); // 1^anything
    return 0;
}
