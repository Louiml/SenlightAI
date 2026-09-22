Write a C++ function named `computeAverage` that takes two integers as input parameters and returns their arithmetic mean as a `double`. The function must handle the case where the sum of the two integers exceeds the range of `int` without causing overflow, and it must return the exact mathematical average (e.g., for inputs 1 and 2, return 1.5). The function should be `const`-correct and use appropriate type conversions to avoid integer division truncation. Additionally, write a separate test program (with a `main` function) that verifies the function using `assert` checks for typical, boundary, and overflow-avoidance cases.
// The core problem is to compute the average of two integers as a floating-point number. The naive approach `(num1 + num2) / 2.0` works for most cases but risks integer overflow if `num1 + num2` exceeds `INT_MAX` (e.g., `INT_MAX` and `INT_MAX`). To avoid this, compute the average as `num1 / 2.0 + num2 / 2.0`, but this can lose precision for odd halves (e.g., 1/2.0 + 2/2.0 = 0.5 + 1.0 = 1.5, but 1/2.0 + 1/2.0 = 0.5 + 0.5 = 1.0, correct; however, for large numbers, floating-point precision may cause slight errors). A safer method is to use `static_cast<double>(num1) / 2.0 + static_cast<double>(num2) / 2.0`, which avoids overflow and yields correct results for all representable integers. Edge cases include negative numbers, where division by 2.0 works smoothly, and extremes like `INT_MAX` and `INT_MIN`. The algorithm takes O(1) time and O(1) auxiliary space.
#include <cstdint>  // for INT_MAX/INT_MIN if needed

// Compute the average of two integers as a double without overflow.
double computeAverage(int a, int b) {
    // Convert to double before division to avoid overflow and integer truncation.
    return static_cast<double>(a) / 2.0 + static_cast<double>(b) / 2.0;
}
#include <cassert>
#include <limits>
#include <cmath>

int main() {
    // Basic cases
    assert(computeAverage(1, 2) == 1.5);
    assert(computeAverage(5, 5) == 5.0);
    assert(computeAverage(-3, 3) == 0.0);
    
    // Negative numbers
    assert(computeAverage(-10, -20) == -15.0);
    assert(computeAverage(-1, 1) == 0.0);
    
    // Boundary extremes (avoid overflow)
    assert(computeAverage(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()) == static_cast<double>(std::numeric_limits<int>::max()));
    assert(computeAverage(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) == static_cast<double>(std::numeric_limits<int>::min()));
    
    // Mixed extremes
    double extremeAvg = computeAverage(std::numeric_limits<int>::max(), std::numeric_limits<int>::min());
    assert(std::abs(extremeAvg - (-0.5)) < 1e-9);
}
