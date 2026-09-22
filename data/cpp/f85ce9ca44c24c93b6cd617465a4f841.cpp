// Write a C++ function named `modularInverse` that takes two integers `a` and `m` (with `m > 0`) and returns the multiplicative inverse of `a` modulo `m`, i.e., the unique integer `x` in the range `[0, m-1]` such that `(a * x) % m == 1`. The function must throw a `std::invalid_argument` exception if `a` and `m` are not coprime (i.e., their greatest common divisor is not 1). Implement the extended Euclidean algorithm internally (you may use a helper function). The function should handle negative values of `a` by working with its modulo `m` representative, and must correctly process edge cases such as `a == 1` (inverse is `1`), `a == m-1` (inverse is `m-1`), and `m == 1` (where no inverse exists because every integer is congruent to 0). Ensure the function is `const`-correct (though it takes values, no member state), uses `long long` where appropriate to avoid overflow in intermediate multiplications, and does not depend on global state.
The solution uses the extended Euclidean algorithm to find integers `x` and `y` such that `a*x + m*y = gcd(a, m)`. If `gcd(a, m) != 1`, no inverse exists, so throw `std::invalid_argument`. Otherwise, `x` modulo `m` is the inverse. To handle negative `a`, first reduce `a` modulo `m` using `(a % m + m) % m` to get a value in `[0, m-1]`. If the reduced value is 0 and `m > 1`, then `gcd(0, m) = m != 1`, so it will throw naturally. If `m == 1`, the only residue class is 0, and `0*x % 1 == 0`, never 1, so it should throw (the extended Euclidean algorithm with `b=1` returns `g=0`? Actually `gcd(0,1)=1`, but since `a*x % 1 == 0`, the inverse does not exist; handle `m == 1` explicitly by throwing). The extended Euclidean recursion: if `b == 0`, set `x=1, y=0, return a`; else recurse with `(b, a % b)`, then update `x = y1`, `y = x1 - (a/b)*y1`. The final result is normalized to `[0, m-1]` by `(x % m + m) % m`. Time complexity is `O(log(min(|a|, m)))` for the Euclidean steps, space complexity is `O(log(...))` due to recursion depth (can be made iterative but recursion is fine for typical int sizes). Edge case: `m == 1` must throw, because no integer has an inverse modulo 1. Also ensure `m > 0`; if not, throw `std::invalid_argument` for invalid modulus (not required by spec, but safe to add). For very large inputs, use `long long` to avoid overflow in `(a / b) * y1`; the standard algorithm uses `int`, but we can cast to `long long` internally.
#include <stdexcept>

// Extended Euclidean algorithm: returns gcd(a, b) and sets x, y such that a*x + b*y = gcd(a, b).
long long extendedEuclid(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = extendedEuclid(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return d;
}

// Return the multiplicative inverse of a modulo m (m > 0), in [0, m-1].
// Throws std::invalid_argument if a and m are not coprime or if m <= 0.
long long modularInverse(long long a, long long m) {
    if (m <= 0) {
        throw std::invalid_argument("modulus must be positive");
    }
    // Reduce a modulo m to handle negative values.
    a = (a % m + m) % m;  // now a in [0, m-1]
    if (m == 1) {
        // No inverse exists because everything is congruent to 0 mod 1.
        throw std::invalid_argument("modulus 1 has no invertible elements");
    }
    long long x, y;
    long long g = extendedEuclid(a, m, x, y);
    if (g != 1) {
        throw std::invalid_argument("inverse does not exist (a and m are not coprime)");
    }
    // x is a solution to a*x ≡ g (mod m), and since g=1, x is the inverse.
    return (x % m + m) % m;
}
#include <cassert>
#include <stdexcept>

int main() {
    // Basic cases
    assert(modularInverse(3, 7) == 5);   // 3*5 = 15 ≡ 1 mod 7
    assert(modularInverse(1, 100) == 1);
    assert(modularInverse(99, 100) == 99); // 99*99 ≡ 1 mod 100
    assert(modularInverse(10, 17) == 12); // 10*12 = 120 ≡ 1 mod 17

    // Negative values of a
    assert(modularInverse(-4, 7) == 5);  // -4 ≡ 3 mod 7, inverse of 3 is 5
    assert(modularInverse(-1, 5) == 4);  // -1 ≡ 4 mod 5, inverse of 4 is 4

    // Non-coprime should throw
    bool threw = false;
    try { modularInverse(6, 9); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { modularInverse(0, 5); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Modulus 1 should throw
    threw = false;
    try { modularInverse(5, 1); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Invalid modulus (non-positive) should throw
    threw = false;
    try { modularInverse(5, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    threw = false;
    try { modularInverse(5, -3); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    return 0;
}
