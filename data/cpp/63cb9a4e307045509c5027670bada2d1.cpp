// Write a C++ function named `scaleByPowerOfTen` that takes a reference to a `long` integer and an unsigned integer `order`. The function must multiply the referenced value by \(10^{order}\), modifying the original variable directly. The function should handle `order` values from `0` to any reasonable positive integer (e.g., up to 18 to avoid overflow, but no explicit check required). Ensure the function is `const`-correct where applicable, uses descriptive naming, and includes a comment explaining its behavior. The function must not return any value; it modifies the input in place.
#include <cassert>

// Declare the function (since it's not in a header)
void scaleByPowerOfTen(long& value, unsigned int order);

int main() {
    long a = 5;
    scaleByPowerOfTen(a, 0);
    assert(a == 5);

    long b = 7;
    scaleByPowerOfTen(b, 1);
    assert(b == 70);

    long c = 123;
    scaleByPowerOfTen(c, 3);
    assert(c == 123000);

    long d = -4;
    scaleByPowerOfTen(d, 2);
    assert(d == -400);

    long e = 0;
    scaleByPowerOfTen(e, 10);
    assert(e == 0);

    long f = 1;
    scaleByPowerOfTen(f, 5);
    assert(f == 100000);

    long g = 999;
    scaleByPowerOfTen(g, 2);
    assert(g == 99900);

    long h = 10;
    scaleByPowerOfTen(h, 0);
    assert(h == 10);

    long i = -1;
    scaleByPowerOfTen(i, 4);
    assert(i == -10000);

    long j = 2;
    scaleByPowerOfTen(j, 1);
    assert(j == 20);
}
#include <cstddef> // for std::size_t if needed (not strictly required, but shown for completeness)

/**
 * @brief Multiplies the referenced long by 10 raised to the power of order.
 * 
 * @param value Reference to the long to be modified in place.
 * @param order Number of times to multiply by 10 (non-negative).
 */
void scaleByPowerOfTen(long& value, unsigned int order) {
    for (unsigned int i = 0; i < order; ++i) {
        value *= 10;
    }
}
// The solution follows the pattern of the snippet: use a loop that multiplies the referenced variable by `10` exactly `order` times. For `order == 0`, the loop does not execute, leaving the value unchanged — this is a natural edge case. For positive orders, each iteration multiplies by 10, which shifts all decimal digits left by one position. A potential overflow occurs if multiplying beyond the range of `long`; since the task does not require overflow detection, we assume the caller supplies valid inputs. Time complexity is \(O(order)\), and space complexity is \(O(1)\) because we only use a loop counter. The function takes a non-`const` reference because it intentionally modifies the caller's variable, but the function body itself does not modify any local state that could be `const`; the loop counter is a local variable. The function is standalone and does not require any additional headers beyond `<cstddef>` if we use `std::size_t` for clarity, but a plain `unsigned int` is also fine.
