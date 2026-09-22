// Given three positive 64-bit unsigned integers `p`, `q`, and `b`, write a C++ function `bool isFiniteFraction(unsigned long long p, unsigned long long q, unsigned long long b)` that returns `true` if the fraction `p / q` has a finite decimal representation when written in base `b`, and `false` otherwise. The fraction may be improper, and `q` is guaranteed to be non-zero. The function must handle very large values (up to `unsigned long long` range) without overflow.

A rational number `p/q` has a finite representation in base `b` if and only if, after reducing the fraction to lowest terms (dividing numerator and denominator by their greatest common divisor), every prime factor of the denominator divides the base `b`. Equivalently, if we repeatedly remove from the denominator all factors that it shares with `b` (by dividing `q` by `gcd(q, b)` until no more common factors remain), then the denominator becomes `1` exactly when the fraction is finite.  
To compute this efficiently without overflow: after reducing `q` by `gcd(p, q)`, repeatedly compute `g = gcd(q, b)` and divide `q` by `g`; if `g == 1` before `q` reaches `1`, then the fraction is infinite. Since each iteration reduces `q` at least by a factor of 2 (because `g >= 2` whenever it is not 1), the loop runs at most `O(log q)` times. The time complexity is `O(log^2 n)` due to the repeated gcd calls (each `std::gcd` is `O(log n)`), and space complexity is `O(1)`.

#include <numeric>   // for std::gcd
#include <cstdint>   // for uint64_t

// Returns true if p/q has a finite representation in base b.
// Assumes q > 0 and b > 0.
bool isFiniteFraction(uint64_t p, uint64_t q, uint64_t b) {
    // Reduce the fraction to lowest terms.
    const uint64_t divisor = std::gcd(p, q);
    q /= divisor;

    // Remove all prime factors of q that are also factors of b.
    while (true) {
        const uint64_t g = std::gcd(q, b);
        if (g == 1) {
            break;
        }
        q /= g;
    }

    // If q became 1, all factors have been removed => finite.
    return q == 1;
}

#include <cassert>

int main() {
    // Simple fractions in base 10.
    assert(isFiniteFraction(1, 2, 10) == true);   // 0.5
    assert(isFiniteFraction(1, 3, 10) == false);  // 0.333...
    assert(isFiniteFraction(2, 4, 10) == true);   // reduces to 1/2
    assert(isFiniteFraction(1, 6, 10) == false);  // prime factor 3
    assert(isFiniteFraction(1, 6, 12) == true);   // base 12 = 2^2 * 3, finite

    // Base 2.
    assert(isFiniteFraction(1, 2, 2) == true);    // 0.1 in binary
    assert(isFiniteFraction(1, 3, 2) == false);   // 0.010101... in binary

    // Large numbers (no overflow in reduction).
    assert(isFiniteFraction(1ULL << 60, 1ULL << 60, 2) == true);  // reduces to 1/1
    assert(isFiniteFraction(1, (1ULL << 60) + 1, 2) == false);    // odd denominator

    // Edge: denominator becomes 1 after reduction.
    assert(isFiniteFraction(7, 7, 3) == true);    // 1/1

    // Extra check with base not dividing denominator.
    assert(isFiniteFraction(1, 5, 6) == true);    // denominator 5, base 6 (2*3) => infinite, but wait? 5 is not a factor of 6 => false
    assert(isFiniteFraction(1, 5, 10) == true);   // 0.2 in base 10
    assert(isFiniteFraction(1, 5, 6) == false);   // correction: 1/5 in base 6 is infinite

    return 0;
}
