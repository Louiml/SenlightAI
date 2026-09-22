/*
Write a C++ function named `circleMeasurements` that takes an integer radius `r` and returns a `std::vector<double>` containing exactly two values: the circumference of a circle at index 0 and the area of the circle at index 1. Use the value of π as 3.14 exactly (not a more precise constant). The function must handle non-negative radius values, including 0, and return the results as floating-point values even if the inputs are integers. No external libraries beyond the standard ones are required.
*/
#include <vector>

// Compute the circumference and area of a circle given radius r.
// Returns a vector where index 0 is circumference and index 1 is area.
std::vector<double> circleMeasurements(int r) {
    const double PI = 3.14;
    std::vector<double> result;
    result.push_back(2 * PI * r);      // circumference
    result.push_back(PI * r * r);      // area
    return result;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test radius 1
    std::vector<double> v1 = circleMeasurements(1);
    assert(std::fabs(v1[0] - 6.28) < 1e-6);  // 2 * 3.14 * 1
    assert(std::fabs(v1[1] - 3.14) < 1e-6);  // 3.14 * 1 * 1

    // Test radius 0
    std::vector<double> v0 = circleMeasurements(0);
    assert(v0[0] == 0.0);
    assert(v0[1] == 0.0);

    // Test radius 5
    std::vector<double> v5 = circleMeasurements(5);
    assert(std::fabs(v5[0] - 31.4) < 1e-6);  // 2 * 3.14 * 5
    assert(std::fabs(v5[1] - 78.5) < 1e-6);  // 3.14 * 25

    // Test radius 10
    std::vector<double> v10 = circleMeasurements(10);
    assert(std::fabs(v10[0] - 62.8) < 1e-6);
    assert(std::fabs(v10[1] - 314.0) < 1e-6);

    // Test radius 100 (square avoids integer overflow)
    std::vector<double> v100 = circleMeasurements(100);
    assert(std::fabs(v100[0] - 628.0) < 1e-6);
    assert(std::fabs(v100[1] - 31400.0) < 1e-6);
}
// The solution is straightforward: compute the circumference as `2 * π * r` and the area as `π * r * r`, where π is a compile-time constant set to 3.14. Since the radius is an integer, multiplying by a double constant will produce a `double` result, so no explicit casts are needed. The function returns a `std::vector<double>` with the two computed values pushed in order. Edge cases include `r = 0`, which yields both circumference and area equal to 0.0 (returned as floating-point), and large integer radii that might cause overflow if squared into an integer, but because we multiply by the double constant `PI` directly, the multiplication is performed in floating-point and avoids integer overflow. The time complexity is O(1) because only constant arithmetic operations are performed, and the space complexity is O(1) because only a fixed-size vector is returned.
