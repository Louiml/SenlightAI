Write a C++ function `double solveForX(double y)` that finds and returns a real number `x` in the interval `[0, 100]` such that the polynomial `f(x) = 8*x^4 + 7*x^3 + 2*x^2 + 3*x + 6` equals `y` within a tolerance of `1e-6`. If no such `x` exists in that interval for the given `y` (i.e., `y` is outside the range of `f` on `[0,100]`), the function should return `-1.0` to indicate no solution. Assume the input `y` is a positive real number; handle edge cases where `y` is too small or too large. Use binary search with a sufficiently small loop condition (e.g., `while (high - low > 1e-7)`) to achieve the required precision. Note that `f` is strictly increasing on `[0,100]`. The function must be self-contained (no external dependencies beyond standard headers) and must use `const` correctly where applicable.
The polynomial `f(x) = 8x^4 + 7x^3 + 2x^2 + 3x + 6` is monotonic increasing on `[0,100]` because its derivative is positive for all `x >= 0` (all coefficients are non-negative). The minimum value occurs at `x=0` giving `f(0)=6`, and the maximum at `x=100` giving `f(100) = 8*(10^8) + 7*(10^6) + 2*(10^4) + 300 + 6 = 807020306`. Thus, if `y < 6` or `y > 807020306`, we return `-1.0` immediately. Otherwise, we perform binary search on `[0,100]`. At each step, compute the midpoint `mid`, evaluate `f(mid)`, and if `f(mid) < y` then we need a larger `x`, so move the low bound to `mid`; otherwise move the high bound to `mid`. Continue until the interval width is less than `1e-7` (to ensure the final result printed to 4 decimals is correct). The returned value should be the high bound (or low, both are within tolerance). Time complexity is `O(log((100-0)/1e-7))` ≈ `O(30)` iterations, independent of input, so effectively `O(1)` per call. Space complexity is `O(1)`. Edge cases: if `y` is exactly 6, the loop will converge to `x=0`; if `y` is exactly 807020306, it converges to `x=100`.
#include <cmath>

// Returns the x in [0,100] such that f(x)=y, or -1.0 if no solution exists.
// f(x) = 8x^4 + 7x^3 + 2x^2 + 3x + 6, strictly increasing on [0,100].
double solveForX(const double y) {
    const double minY = 6.0;
    const double maxY = 807020306.0;
    if (y < minY || y > maxY) {
        return -1.0;
    }

    double low = 0.0;
    double high = 100.0;
    // Binary search with tolerance smaller than the required 1e-6.
    while (high - low > 1e-7) {
        const double mid = (low + high) / 2.0;
        // Evaluate f(mid) = ((8*mid + 7)*mid + 2)*mid + 3)*mid + 6
        const double f_mid = (((8.0 * mid + 7.0) * mid + 2.0) * mid + 3.0) * mid + 6.0;
        if (f_mid < y) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return high; // within 1e-7 of true solution
}
#include <cassert>
#include <cmath>

// Declare the function being tested (in real code, include the header).
double solveForX(double y);

int main() {
    // Test no-solution cases
    assert(solveForX(5.9999) == -1.0);
    assert(solveForX(807020307.0) == -1.0);

    // Test boundary values
    assert(std::fabs(solveForX(6.0) - 0.0) < 1e-4);
    assert(std::fabs(solveForX(807020306.0) - 100.0) < 1e-4);

    // Test a mid-range value: f(1) = 8+7+2+3+6 = 26
    assert(std::fabs(solveForX(26.0) - 1.0) < 1e-4);

    // Test f(0.5): compute manually: 8*(0.0625)+7*(0.125)+2*(0.25)+3*0.5+6 = 0.5+0.875+0.5+1.5+6 = 9.375
    assert(std::fabs(solveForX(9.375) - 0.5) < 1e-4);

    // Test a large value near the top: f(99.0) ~ compute approximate
    double x_test = 99.0;
    double y_expected = (((8.0*x_test + 7.0)*x_test + 2.0)*x_test + 3.0)*x_test + 6.0;
    assert(std::fabs(solveForX(y_expected) - x_test) < 1e-4);

    // Test random integer y between range (e.g., y=1000, which corresponds to roughly x~4.9)
    double x_approx = solveForX(1000.0);
    double f_approx = (((8.0*x_approx + 7.0)*x_approx + 2.0)*x_approx + 3.0)*x_approx + 6.0;
    assert(std::fabs(f_approx - 1000.0) < 1e-3);

    return 0;
}
