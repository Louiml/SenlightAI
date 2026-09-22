/*
Write a C++ function that takes a short integer value representing the diameter of a circle as its only parameter, and returns the circle's area as a `float`, using the formula `area = (diameter * diameter * PI) / 4`, where `PI` is a constant equal to `3.14`. The function must be named `calculateCircleAreaFromDiameter`, be `const`-correct (the parameter should not be modifiable), and handle any valid `short` input, including zero and negative values (the area for a negative diameter should be computed using the absolute value of the diameter, since diameter is a length; for zero, the area is zero). Do not include any input/output logic in the function itself — just the pure computation.
*/

// Returns the area of a circle given its diameter.
// PI is a constant with value 3.14.
// The function uses the provided diameter (taking its absolute value to handle negatives)
// and computes (diameter^2 * PI) / 4.
float calculateCircleAreaFromDiameter(const short diameter) {
    const float PI = 3.14f;
    const short absDiameter = (diameter < 0) ? -diameter : diameter;
    return (absDiameter * absDiameter * PI) / 4.0f;
}

#include <cassert>
#include <cmath>

int main() {
    // Test a positive diameter
    assert(std::fabs(calculateCircleAreaFromDiameter(4) - 12.56f) < 0.001f);
    // Test a zero diameter
    assert(calculateCircleAreaFromDiameter(0) == 0.0f);
    // Test a negative diameter (should use absolute value)
    assert(std::fabs(calculateCircleAreaFromDiameter(-4) - 12.56f) < 0.001f);
    // Test a small positive value
    assert(std::fabs(calculateCircleAreaFromDiameter(1) - 0.785f) < 0.001f);
    // Test a large short value (32767)
    float largeArea = calculateCircleAreaFromDiameter(32767);
    float expectedLarge = (32767.0f * 32767.0f * 3.14f) / 4.0f;
    assert(std::fabs(largeArea - expectedLarge) < 0.01f);
    // Test a negative large short value (-32768)
    float negLargeArea = calculateCircleAreaFromDiameter(-32768);
    float expectedNegLarge = (32768.0f * 32768.0f * 3.14f) / 4.0f;
    assert(std::fabs(negLargeArea - expectedNegLarge) < 0.01f);
    return 0;
}

// The solution is a straightforward application of the circle area formula using the diameter relationship: since the radius is half the diameter, area = π × (d/2)² = (d² × π) / 4. The task specifies `PI = 3.14` as a constant. To handle negative diameters (which mathematically shouldn't occur but the input may provide), we take the absolute value of the parameter before squaring, because squaring a negative number yields a positive result anyway, but using the absolute value avoids any ambiguity. For zero, the area is zero. The time complexity is O(1) because only constant arithmetic operations are performed, and the space complexity is O(1) as no additional data structures are used. Edge cases include the smallest and largest possible `short` values (e.g., -32768 and 32767) — squaring them may overflow a `short` but since the computation is done in `float` arithmetic (by multiplication of `float` operands), the result is fine. The function is marked `const` for the parameter to indicate it is read-only.
