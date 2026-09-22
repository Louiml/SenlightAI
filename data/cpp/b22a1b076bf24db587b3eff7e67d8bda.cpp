// Write a C++ function that takes two integers `a` and `b` as input parameters and returns their product as an `int`. The function should be named `multiply` and must handle edge cases where the product may overflow the 32-bit `int` range; in such cases, the function should return the result computed with modular arithmetic (i.e., wrap around naturally using `unsigned int` conversion or arithmetic on `long long` then casting back). The function must be self-contained, use `const` parameters, and include appropriate headers. No `main` function or I/O is needed in the solution; instead, the test code will call the function directly.
#include <cassert>
#include <climits>

int main() {
    // Basic multiplication
    assert(multiply(0, 0) == 0);
    assert(multiply(3, 4) == 12);
    assert(multiply(-3, 4) == -12);
    assert(multiply(3, -4) == -12);
    assert(multiply(-3, -4) == 12);

    // Boundary and overflow cases
    assert(multiply(INT_MAX, 1) == INT_MAX);
    assert(multiply(INT_MIN, 1) == INT_MIN);
    assert(multiply(65536, 65536) == 0); // 2^32 wraps to 0
    assert(multiply(INT_MAX, 2) == -2); // wrap-around on 32-bit two's complement
    assert(multiply(INT_MIN, -1) == INT_MIN); // overflow wraps to same value

    // Large magnitude but fitting in 32-bit
    assert(multiply(46340, 46340) == 2147395600); // within INT_MAX
}
#include <cstdint>

// Return the product of two integers, with overflow handled via 64-bit intermediate.
int multiply(const int a, const int b) {
    // Cast to long long to avoid undefined behavior on overflow.
    const long long product = static_cast<long long>(a) * static_cast<long long>(b);
    // Cast back to int (implementation-defined if out of range, but wraps in practice).
    return static_cast<int>(product);
}
// The core operation is simple multiplication of two `int` values. However, the main challenge is handling potential overflow when the product exceeds the `INT_MAX` or `INT_MIN` range. In C++, signed integer overflow is undefined behavior, so we must avoid it. The simplest portable approach is to cast the inputs to `long long` (guaranteed to be at least 64 bits) before multiplying, then cast the result back to `int`. This ensures that intermediate arithmetic is performed in a type that cannot overflow for typical `int` sizes (32-bit). The final cast back to `int` will produce implementation-defined behavior if the value does not fit, but in practice, it yields the two's complement wrap-around, which matches the typical modular arithmetic expectation. Edge cases include: both inputs zero (product zero), one positive and one negative (negative product), large positive values like `INT_MAX` and `INT_MIN` (overflow wraps), and combinations that produce exact boundaries (e.g., `65536 * 65536 = 2^32`, which wraps to 0). Time complexity is O(1) and space complexity is O(1), as only arithmetic on fixed-size integers is performed.
