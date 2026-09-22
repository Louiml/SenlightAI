// Implement a C++ function `double geometricDilutionOfPrecision(double horizontalDop, double verticalDop, double timeDop)` that computes the geometric dilution of precision (GDOP) as defined by the snippet: first compute the position DOP as the Euclidean norm of the horizontal and vertical DOP components (i.e., `sqrt(hDOP^2 + vDOP^2)`), then compute the geometric DOP as the Euclidean norm of that position DOP and the time DOP (i.e., `sqrt(PDOP^2 + tDOP^2)`). The function must handle invalid inputs (negative values, NaN, or infinity) by returning a quiet NaN (e.g., `std::numeric_limits<double>::quiet_NaN()`). Use only standard C++ libraries (e.g., `<cmath>`), avoid any platform-specific math functions, and ensure the function is `const`-correct (though free functions don't need this, make the parameters `const` references if needed for clarity). Provide the implementation with proper comments, and ensure it works correctly for edge cases like zero values (e.g., all zero inputs should produce 0.0) and very large finite values (should produce a finite result if within representable range).

// The solution computes GDOP in two steps, mirroring the snippet’s logic. First, compute `pdop = sqrt(h^2 + v^2)`. Second, compute `gdop = sqrt(pdop^2 + t^2)`. The main algorithm is straightforward: use `std::hypot` to compute the Euclidean norm efficiently and safely, which handles overflow better than naive squaring. For each step, validate inputs: if any input is negative, NaN, or infinite, return a quiet NaN. Also check intermediate results (e.g., if `hypot` returns infinity due to overflow, return NaN as well). Edge cases: all zero values produce `0.0`; one zero value is fine; negative values are invalid; NaN propagates as NaN; infinity is invalid. Time complexity is O(1) (constant number of operations), and space complexity is O(1). The solution uses `std::hypot` to avoid overflow when squaring large numbers, and explicitly checks for non-finite or negative inputs to mimic the snippet’s error handling (which returned NaN on any `Math` error). The reference implementation is self-contained with necessary headers (`<cmath>`, `<limits>`).

#include <cmath>
#include <limits>

// Computes the geometric dilution of precision (GDOP) from horizontal, vertical,
// and time DOP components. Returns a quiet NaN if any input is negative, NaN,
// infinite, or if intermediate computations overflow.
double geometricDilutionOfPrecision(double horizontalDop, double verticalDop, double timeDop) {
    // Helper to check validity: must be finite and non-negative.
    auto isValid = [](double value) {
        return std::isfinite(value) && value >= 0.0;
    };

    // Validate all inputs.
    if (!isValid(horizontalDop) || !isValid(verticalDop) || !isValid(timeDop)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Compute position DOP: sqrt(h^2 + v^2) using hypot for numerical stability.
    double positionDop = std::hypot(horizontalDop, verticalDop);
    if (!std::isfinite(positionDop)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Compute geometric DOP: sqrt(pdop^2 + t^2) using hypot again.
    double geometricDop = std::hypot(positionDop, timeDop);
    if (!std::isfinite(geometricDop)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return geometricDop;
}

#include <cassert>
#include <cmath>
#include <limits>

// Forward declaration of the solution function (as defined in the solution).
double geometricDilutionOfPrecision(double horizontalDop, double verticalDop, double timeDop);

int main() {
    // Basic case: all positive finite values.
    double result = geometricDilutionOfPrecision(3.0, 4.0, 12.0);
    // PDOP = sqrt(9+16) = 5, GDOP = sqrt(25+144) = sqrt(169) = 13.
    assert(std::fabs(result - 13.0) < 1e-9);

    // Zero DOP components produce zero GDOP.
    result = geometricDilutionOfPrecision(0.0, 0.0, 0.0);
    assert(result == 0.0);

    // One component zero, others positive.
    result = geometricDilutionOfPrecision(3.0, 4.0, 0.0);
    // PDOP = 5, GDOP = 5.
    assert(std::fabs(result - 5.0) < 1e-9);

    // Negative input returns NaN.
    result = geometricDilutionOfPrecision(-1.0, 2.0, 3.0);
    assert(std::isnan(result));

    // NaN input returns NaN.
    result = geometricDilutionOfPrecision(std::numeric_limits<double>::quiet_NaN(), 1.0, 1.0);
    assert(std::isnan(result));

    // Infinity input returns NaN.
    result = geometricDilutionOfPrecision(1.0, std::numeric_limits<double>::infinity(), 1.0);
    assert(std::isnan(result));

    // Large finite values (e.g., 1e154) should still produce a finite GDOP (avoid overflow).
    result = geometricDilutionOfPrecision(1e154, 1e154, 0.0);
    // PDOP = 1e154 * sqrt(2) ≈ 1.4142e154, GDOP = same (since time=0).
    assert(std::isfinite(result));
    assert(result > 0.0);

    // Very small values (near zero) fine.
    result = geometricDilutionOfPrecision(1e-154, 1e-154, 1e-154);
    assert(std::isfinite(result) && result > 0.0);

    return 0;
}
