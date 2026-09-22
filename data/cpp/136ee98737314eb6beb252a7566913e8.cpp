// Write a C++ function `bool isSelfNegating()` that determines whether the value of a `boost::multiprecision::cpp_int` becomes zero after applying the unary minus operator. Specifically, the function should return `true` if `-i == i` for a given `cpp_int` value `i`, and `false` otherwise. The function must handle extremely large integers (beyond 64-bit range) and be robust against edge cases such as zero, positive numbers, negative numbers, and values where negation overflows in fixed-width integer types (though `cpp_int` is arbitrary precision, the comparison itself must be carefully implemented). The function should take a `const cpp_int&` parameter and return a `bool`.
// The core task is to check whether negating a `cpp_int` produces the same value. In arbitrary-precision arithmetic, the only integer that satisfies `-x == x` is `0`. This is because for any non-zero positive integer, negation yields a different negative value, and vice versa. The naive approach of writing `return (-i == i);` would work correctly in `cpp_int` since it supports arbitrary precision and comparison. However, there is a subtlety: the `cpp_int` type in Boost.Multiprecision has a `signed_magnitude` or `unsigned_magnitude` backend, and for certain configurations, unary minus on an unsigned type might produce unexpected behavior (e.g., in the provided snippet, `unsigned_magnitude` is used, and `i = -i;` would wrap around modulo 2^32). To make the function robust regardless of the backend's signedness, we should avoid relying on unary minus behavior directly. Instead, we can compare the value to zero using `i == 0`, which is the mathematical definition. The algorithm is O(1) in time and O(1) in space, as it only involves a single comparison. Edge cases: zero returns true; any non-zero (positive or negative) returns false. There is no overflow concern because `cpp_int` is arbitrary precision. The solution is to simply return `i == 0`. This is both correct and avoids any potential undefined behavior from negation on unusual backends.
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

// Returns true if the given cpp_int equals its own negation (i.e., zero).
bool isSelfNegating(const cpp_int& value) {
    // In arbitrary-precision arithmetic, the only number equal to its negation is zero.
    return value == 0;
}
#include <cassert>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

bool isSelfNegating(const cpp_int& value) {
    return value == 0;
}

int main() {
    cpp_int zero = 0;
    assert(isSelfNegating(zero));

    cpp_int one = 1;
    assert(!isSelfNegating(one));

    cpp_int minusOne = -1;
    assert(!isSelfNegating(minusOne));

    cpp_int large = 1;
    large <<= 256;  // 2^256
    assert(!isSelfNegating(large));

    cpp_int largeNegative = -large;
    assert(!isSelfNegating(largeNegative));

    cpp_int manyBits = 123456789012345678901234567890;
    assert(!isSelfNegating(manyBits));

    // Check with a negative zero (if supported by the backend, though cpp_int normalizes to 0)
    cpp_int maybeNegativeZero = 0;
    maybeNegativeZero = -maybeNegativeZero;
    assert(isSelfNegating(maybeNegativeZero));
}
