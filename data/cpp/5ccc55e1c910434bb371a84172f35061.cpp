// Write a C++ function that takes three integer inputs `a`, `b`, and `c` (where `a` and `b` are the lengths of the two legs of a right triangle, and `c` is the length of the hypotenuse) and returns the square of the hypotenuse using the Pythagorean theorem: `c² = a² + b²`. The function should accept the two leg lengths as parameters and return the computed value as an integer. Note: the input legs are guaranteed to be non-negative integers, and the result may be large, so use a 64-bit integer type (`long long`) for the computation and return type to avoid overflow. The function must be a standalone, free function (no `main`), properly const-correct where applicable.
#include <cassert>
#include <cstdint>

// The solution function (declared above)
std::int64_t hypotenuseSquare(std::int64_t a, std::int64_t b);

int main() {
    // Basic Pythagorean triples
    assert(hypotenuseSquare(3, 4) == 25);          // 3-4-5 triangle
    assert(hypotenuseSquare(5, 12) == 169);        // 5-12-13 triangle
    // Zero legs
    assert(hypotenuseSquare(0, 0) == 0);
    assert(hypotenuseSquare(0, 7) == 49);
    // Large values within 32-bit range
    assert(hypotenuseSquare(46340, 46340) == 4294967200LL); // 46340^2*2
    // Extreme near overflow (legs ~ 2^31-1)
    assert(hypotenuseSquare(2147483647LL, 0) == 4611686009837452809LL); // (2^31-1)^2
    assert(hypotenuseSquare(2147483647LL, 2147483647LL) == 9223372030412326418LL); // 2*(2^31-1)^2
    return 0;
}
#include <cstdint>

// Compute the square of the hypotenuse of a right triangle with legs a and b.
// Precondition: a and b are non-negative integers.
// Returns a^2 + b^2 as a 64-bit integer.
std::int64_t hypotenuseSquare(std::int64_t a, std::int64_t b) {
    return a * a + b * b;
}
// The solution is straightforward: compute `a*a + b*b` using the Pythagorean theorem. Since the legs are non-negative, no special handling for negative values is needed, but the result can grow large (e.g., if `a` and `b` are close to 2^31, the sum of squares can exceed 32-bit integer range), so use `long long` for both the parameters and the return type, converting the inputs to `long long` before multiplication to avoid integer overflow. The algorithm is `O(1)` time and `O(1)` auxiliary space. Edge cases: zero legs (returns 0 or the square of the other leg), large values near the maximum 32-bit integer (works correctly due to 64-bit arithmetic), and equal legs (no issue). The function should be marked `const`-qualified? Not applicable for free functions; but parameters should be passed by value (cheap for small integers) and `const` is not needed for by-value parameters. However, the function itself can be `noexcept` if desired, but not required.
