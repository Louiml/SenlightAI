// Write a C++ function that takes two positive integers and returns their least common multiple (LCM). The function must compute the LCM using the Euclidean algorithm for the greatest common divisor (GCD), and it must handle values up to \(10^9\) without overflow, meaning you should reduce the product by dividing before multiplying. The function should be declared as `long long lcm(long long a, long long b)` and must not rely on any external libraries beyond standard headers.

#include <cassert>

int main() {
    assert(lcm(4, 6) == 12);
    assert(lcm(21, 6) == 42);
    assert(lcm(1, 1) == 1);
    assert(lcm(1, 1000000000LL) == 1000000000LL);
    assert(lcm(1000000000LL, 999999937LL) == 999999937000000000LL);
    assert(lcm(7, 13) == 91);
    assert(lcm(12, 8) == 24);
    assert(lcm(5, 5) == 5);
    assert(lcm(100, 99) == 9900);
    assert(lcm(123456789LL, 987654321LL) == 13576230319004901LL);
    return 0;
}

#include <cstdint>

// Compute the greatest common divisor of two positive integers using the Euclidean algorithm.
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

// Compute the least common multiple of two positive integers without overflow.
long long lcm(long long a, long long b) {
    long long g = gcd(a, b);
    // Divide first to avoid potential overflow.
    return (a / g) * b;
}

// The LCM of two numbers \(a\) and \(b\) satisfies the identity: \(\text{lcm}(a,b) = \frac{a \cdot b}{\gcd(a,b)}\). To avoid integer overflow when \(a\) and \(b\) are large (up to \(10^9\), product up to \(10^{18}\) fits in 64-bit signed but could be borderline), compute it as \(\frac{a}{\gcd(a,b)} \cdot b\) or \(\frac{b}{\gcd(a,b)} \cdot a\). The GCD is computed efficiently using the Euclidean algorithm, which repeatedly replaces \((x,y)\) with \((y, x \bmod y)\) until \(y=0\). Edge cases: if either input is zero, the mathematical LCM is undefined; for this task, inputs are guaranteed positive, so no special handling is needed. Time complexity is \(O(\log(\min(a,b)))\) for the GCD, and space complexity is \(O(1)\).
