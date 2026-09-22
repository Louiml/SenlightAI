Write a C++ function that computes the area of a trapezoid given its bottom base `b`, top base `a`, and height `t`. The function should accept three parameters of type `double` and return the area as a `double`. The area must be calculated using the formula `0.5 * (a + b) * t`. Ensure the function is robust for non-negative inputs and handles cases where any base or height is zero (yielding an area of zero). The function should be named `computeTrapezoidArea` and should not perform any input/output operations.
// The main algorithm is straightforward: the area of a trapezoid is given by `0.5 * (a + b) * t`. Because all inputs are `double`, the multiplication and addition are done with floating-point precision. Edge cases include zero or negative inputs: if any dimension is zero, the area is zero; negative dimensions are physically impossible, but the formula still produces a value (which might be negative if `a+b` is negative and `t` is positive, etc.). Since the task does not specify validation, we can assume inputs are non-negative; however, the function will still compute correctly for any `double` values. Time complexity is O(1) and space complexity is O(1) since it only performs a few arithmetic operations.
#include <cmath> // not strictly needed but good practice for general math

/**
 * Compute the area of a trapezoid given its two bases and height.
 * 
 * @param a Top base length (positive double).
 * @param b Bottom base length (positive double).
 * @param t Height (positive double).
 * @return The area computed as 0.5 * (a + b) * t.
 */
double computeTrapezoidArea(double a, double b, double t) {
    return 0.5 * (a + b) * t;
}
#include <cassert>

int main() {
    // Basic case: bases 10 and 20, height 5 -> area 75
    assert(computeTrapezoidArea(10.0, 20.0, 5.0) == 75.0);
    
    // Symmetric bases: 7 and 7, height 3 -> area 21
    assert(computeTrapezoidArea(7.0, 7.0, 3.0) == 21.0);
    
    // Zero height gives zero area
    assert(computeTrapezoidArea(3.0, 4.0, 0.0) == 0.0);
    
    // Zero base gives area as half of other base times height
    assert(computeTrapezoidArea(0.0, 4.0, 2.0) == 4.0);
    
    // Small decimal values
    assert(computeTrapezoidArea(1.5, 2.5, 1.0) == 2.0);
    
    // Large numbers
    assert(computeTrapezoidArea(1e6, 2e6, 3e6) == 4.5e12);
    
    // Very small numbers (edge case)
    assert(computeTrapezoidArea(0.0001, 0.0002, 0.0003) == 0.5 * (0.0003) * 0.0003); // 4.5e-8
    
    // Negative inputs (not physically valid but function computes formula)
    assert(computeTrapezoidArea(-2.0, -3.0, 4.0) == -10.0);
    
    return 0;
}
