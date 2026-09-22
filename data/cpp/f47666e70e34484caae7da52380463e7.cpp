Write a C++ function that takes two integers (which may be negative, zero, or positive) and returns their least common multiple (LCM) as a non-negative integer. The function must handle the case where either input is zero by returning 0 (since the LCM of 0 and any number is conventionally 0). It should compute the LCM using the greatest common divisor (GCD) formula: `lcm(a, b) = |a * b| / gcd(a, b)`. You must implement your own GCD function (recursive or iterative) and ensure that no integer overflow occurs during the multiplication of the two absolute values; if the product exceeds the range of `int`, the result should be computed using `long long` internally and then returned as `long long`. The function signature must be `long long lcm(long long a, long long b)`.
// The core algorithm is based on the mathematical relation between GCD and LCM. First, handle the trivial case: if either `a` or `b` is zero, return 0 (since the LCM is undefined or conventionally 0). Otherwise, take absolute values of both inputs to avoid sign complications. Compute the GCD using Euclid’s algorithm: `gcd(x, y) = gcd(y % x, x)` with base case when `x == 0` returning `y` (or when one is zero, return the other). To avoid overflow in `a * b`, use `long long` arithmetic; since the inputs are `long long`, the product `absA * absB` may still overflow `long long` for very large values (up to ~9e18), but in practice the problem constraints likely keep within `int` range (or `long long`). A safe approach is to divide first: `lcm = absA / gcd(absA, absB) * absB`, which avoids overflow if the quotient fits. This works because `absA / gcd` is an integer and the product of that with `absB` is the LCM. Edge cases: negative numbers are handled by taking absolute values; zero inputs return 0; when both are equal, the LCM equals the number itself (e.g., `lcm(6,6)=6`). Time complexity is O(log min(a,b)) due to Euclid’s algorithm, and space complexity is O(1) for an iterative GCD or O(log n) for recursive stack depth (but typically small).
#include <cstdlib>   // for std::llabs
#include <algorithm> // for std::gcd if needed, but we implement our own

// Recursive GCD implementation using Euclid's algorithm.
long long gcd(long long a, long long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// Return the least common multiple of two integers (including zero handling).
// Uses the formula lcm = (|a| / gcd) * |b| to avoid overflow.
long long lcm(long long a, long long b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    long long absA = std::llabs(a);
    long long absB = std::llabs(b);
    long long g = gcd(absA, absB);
    // Division first avoids overflow in the product.
    return (absA / g) * absB;
}
#include <cassert>

int main() {
    // Basic cases
    assert(lcm(4, 6) == 12);
    assert(lcm(21, 6) == 42);
    assert(lcm(0, 5) == 0);
    assert(lcm(5, 0) == 0);
    // Negative numbers
    assert(lcm(-4, 6) == 12);
    assert(lcm(4, -6) == 12);
    assert(lcm(-4, -6) == 12);
    // Equal numbers
    assert(lcm(7, 7) == 7);
    // One is a multiple of the other
    assert(lcm(3, 9) == 9);
    assert(lcm(10, 5) == 10);
    // Larger values within long long range
    assert(lcm(1000000, 999999) == 999999000000LL);
    return 0;
}
