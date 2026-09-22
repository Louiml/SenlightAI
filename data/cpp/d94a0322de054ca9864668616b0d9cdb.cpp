Write a C++ function named `isDivisibleBySeven` that takes a single 64-bit integer `n` as input and returns a boolean value indicating whether `n` is divisible by 7 without a remainder. The function must handle all possible values of `long long`, including negative numbers, zero, and the minimum and maximum representable values. The function should be pure (no I/O) and const-correct, and it must be suitable for use in both positive and negative test cases.
The core algorithm is straightforward: a number is divisible by 7 if `n % 7 == 0` in C++. The modulo operator `%` in C++ returns a remainder with the same sign as the dividend, but for divisibility checks, the sign of the remainder is irrelevant; zero is zero. For example, `-7 % 7 == 0` and `14 % 7 == 0`. Edge cases include `n == 0` (which is divisible by every integer, so returns true), negative numbers (e.g., -7, -14), and extreme values like `LLONG_MIN` and `LLONG_MAX` — since `%` works without overflow on these values, the operation is safe. Time complexity is O(1) as it performs a single modulo operation, and space complexity is O(1) as no additional data structures are used.
#include <cstdint>

// Returns true if the 64-bit integer n is divisible by 7, false otherwise.
bool isDivisibleBySeven(const long long n) {
    // The modulo operator returns 0 exactly when n is a multiple of 7.
    // This works for positive, negative, and zero values without overflow.
    return (n % 7) == 0;
}
#include <cassert>
#include <cstdint>
#include <climits>

// Provided solution function (declared for completeness).
bool isDivisibleBySeven(const long long n);

int main() {
    // Basic positive cases
    assert(isDivisibleBySeven(7) == true);
    assert(isDivisibleBySeven(14) == true);
    assert(isDivisibleBySeven(21) == true);
    
    // Basic negative cases
    assert(isDivisibleBySeven(-7) == true);
    assert(isDivisibleBySeven(-14) == true);
    assert(isDivisibleBySeven(-21) == true);
    
    // Non-divisible numbers
    assert(isDivisibleBySeven(1) == false);
    assert(isDivisibleBySeven(8) == false);
    assert(isDivisibleBySeven(-1) == false);
    assert(isDivisibleBySeven(-8) == false);
    
    // Zero is divisible by any non-zero integer
    assert(isDivisibleBySeven(0) == true);
    
    // Extreme values
    assert(isDivisibleBySeven(LLONG_MAX) == false);
    assert(isDivisibleBySeven(LLONG_MIN) == false);
    assert(isDivisibleBySeven(LLONG_MAX - (LLONG_MAX % 7)) == true);
    assert(isDivisibleBySeven(LLONG_MIN - (LLONG_MIN % 7)) == true);
    
    return 0;
}
