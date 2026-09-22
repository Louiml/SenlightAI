Given two integers `a` and `b`, write a C++ function that returns their average as a double, correctly handling fractional results (e.g., for `6` and `4` the average is exactly `5`, but for `5` and `2` the average should be `3.5`). The function must take the two integers as parameters and compute `(a + b) / 2.0` to avoid integer truncation. No input/output is required inside the function; it simply returns the computed value.

#include <cassert>
#include <cmath>

int main() {
    // Basic cases
    assert(averageOfTwo(6, 4) == 5.0);
    assert(averageOfTwo(5, 2) == 3.5);
    assert(averageOfTwo(-1, 1) == 0.0);
    // Negative numbers
    assert(averageOfTwo(-4, -6) == -5.0);
    assert(averageOfTwo(-10, 10) == 0.0);
    // Large values
    assert(averageOfTwo(2000000000, 2000000000) == 2000000000.0);
    // Zero and one value
    assert(averageOfTwo(0, 0) == 0.0);
    assert(averageOfTwo(7, 7) == 7.0);
    // Odd sum produces .5
    assert(averageOfTwo(3, 4) == 3.5);
    // Precision check (within tolerance)
    assert(std::abs(averageOfTwo(1, 2) - 1.5) < 1e-9);
}

#include <stdexcept> // not needed, but included for best practice

// Return the arithmetic mean of two integers as a double.
double averageOfTwo(int a, int b) {
    return (static_cast<double>(a) + static_cast<double>(b)) / 2.0;
}

// The solution is straightforward: sum the two integers and divide by `2.0` (or `2.0 * 1.0`) to force floating-point division. Using `(a + b) / 2` with integers would truncate the decimal part, so the key edge case is when `a + b` is odd — the division must be done with at least one operand being a double. The algorithm runs in constant time \(O(1)\) and uses constant space \(O(1)\). No special handling is needed for negative numbers because floating-point division handles them correctly, and the result is symmetric. The function should be marked `const` if appropriate (though parameters are passed by value, so `const` applies to the parameter types). Return type is `double` to preserve precision.
