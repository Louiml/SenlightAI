// Write a standalone C++ function named `computeWeeklyPay` that takes an hourly rate (as a `double`) and the number of hours worked (as an `int`), validates that the rate is non-negative and the hours are between 0 and 168 (inclusive, representing a full week), and returns the weekly pay as a `double` (rate × hours). If either input is invalid, the function should return -1.0 to signal an error. The function must not read from or write to any standard input/output streams; it should be pure, testable, and use `const` for parameters where appropriate. Ensure the function handles boundary values correctly (e.g., 0 hours, 168 hours, zero rate) and that all calculations use standard double arithmetic (no rounding or truncation). The function must be self-contained, include all necessary headers, and be ready for inclusion in a larger program.
// The solution is straightforward: compute `rate * hours` after checking validity. The main algorithm:  
// 1. Validate `rate >= 0.0` and `hours >= 0 && hours <= 168`.  
// 2. If invalid, return -1.0.  
// 3. Otherwise, return `rate * hours`.  
//
// Edge cases include zero inputs (valid, pay = 0), maximum hours (168), negative rates (invalid), fractional rates (valid), and hours outside 0–168 (invalid). Since the function does no I/O and uses simple arithmetic, time complexity is O(1) and space complexity is O(1). No data structures are needed. The return of -1.0 as a sentinel is safe because weekly pay can never be negative for valid inputs, so it unambiguously signals an error.
#include <stdexcept> // Not used, but kept for potential future use; not required.

// Computes weekly pay as rate * hours, validating inputs.
// Returns -1.0 if rate is negative or hours are outside [0, 168].
// Otherwise returns the product.
double computeWeeklyPay(const double rate, const int hours) {
    if (rate < 0.0 || hours < 0 || hours > 168) {
        return -1.0;
    }
    return rate * hours;
}
#include <cassert>
#include <cmath>

int main() {
    // Valid cases
    assert(std::fabs(computeWeeklyPay(10.0, 40) - 400.0) < 1e-9);
    assert(std::fabs(computeWeeklyPay(0.0, 0) - 0.0) < 1e-9);
    assert(std::fabs(computeWeeklyPay(15.5, 168) - 2604.0) < 1e-9);
    assert(std::fabs(computeWeeklyPay(7.25, 168) - 1218.0) < 1e-9);

    // Invalid cases
    assert(computeWeeklyPay(-1.0, 40) == -1.0);
    assert(computeWeeklyPay(10.0, -1) == -1.0);
    assert(computeWeeklyPay(10.0, 169) == -1.0);
    assert(computeWeeklyPay(0.0, 168) == 0.0); // Boundary valid

    return 0;
}
