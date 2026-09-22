// Write a standalone C++ function that computes the alternating infinite series sum: \( S = \sum_{k=1}^{\infty} \frac{6 \cdot (-1)^{k-1}}{(2k-1)^3} \). The function should take a positive double `tolerance` as a parameter and return the computed partial sum once the absolute value of the current term falls below that tolerance. The function must handle edge cases such as very small or very large tolerances, ensure no infinite loop (e.g., cap iterations at a reasonable limit like 1,000,000), and return the sum as a `double`. The task is to implement this series approximation in a self-contained, reusable function—no `main` required—with proper `const` correctness and clear comments.

#include <cassert>
#include <cmath>

int main() {
    // The true value of the series is π^3/32 ≈ 0.969323... 
    const double trueValue = 0.969323; // approximate for checking
    double result = computeSeriesSum(1e-6);
    assert(std::abs(result - trueValue) < 1e-5);

    // Very small tolerance should give very close result
    double resultTight = computeSeriesSum(1e-12);
    assert(std::abs(resultTight - trueValue) < 1e-9);

    // Non-positive tolerance should not hang; returns at least the first term
    double resultNonPositive = computeSeriesSum(-1.0);
    assert(resultNonPositive >= 5.0 && resultNonPositive <= 7.0); // first term is 6

    // Very large tolerance terminates immediately
    double resultLarge = computeSeriesSum(100.0);
    assert(resultLarge == 6.0); // first term = 6, then |term| < 100 so stop

    // Tolerance exactly zero -> default assigned
    double resultZero = computeSeriesSum(0.0);
    assert(std::abs(resultZero - trueValue) < 1e-10);

    return 0;
}

#include <cmath>
#include <stdexcept>

// Compute the alternating series sum S = sum_{k=1}^{inf} 6*(-1)^(k-1)/(2k-1)^3
// until the absolute value of the current term is less than tolerance.
// If tolerance is <= 0, use a default of 1e-15 to avoid infinite loops.
// A maximum iteration cap of 1,000,000 prevents infinite loops from floating-point underflow.
double computeSeriesSum(double tolerance) {
    const double effectiveTolerance = (tolerance > 0.0) ? tolerance : 1e-15;
    const int maxIterations = 1000000;
    
    double sum = 0.0;
    for (int k = 1; k <= maxIterations; ++k) {
        const double term = 6.0 * std::pow(-1, k - 1) / std::pow(2 * k - 1, 3);
        sum += term;
        if (std::abs(term) < effectiveTolerance) {
            break;
        }
    }
    return sum;
}

// The series term for index `k` is given directly by the formula: `term(k) = 6 * (-1)^(k-1) / ((2k-1)^3)`. The algorithm initializes the sum to 0.0, sets `k = 1`, then in a loop computes the current term, adds it to the sum, increments `k`, and stops when the absolute value of the most recent term is less than the provided tolerance. Because the terms alternate in sign and monotonically decrease in magnitude (since the denominator grows cubically), the series converges; the stopping criterion based on the magnitude of the last term is a standard approximation for alternating series. Edge cases: if `tolerance` is non-positive (e.g., 0 or negative), the loop would never terminate; therefore, clamp it to a small positive value like `1e-15` or handle it by treating non-positive as default. If `tolerance` is extremely large (e.g., `1e10`), the loop will terminate on the first iteration since the first term (~6) is likely smaller; that's fine. To prevent an accidental infinite loop due to floating-point underflow (terms becoming exactly 0.0), also add a maximum iteration cap (e.g., 1,000,000) and break when `k` exceeds it. The primary operation cost is computing powers and divisions per iteration; using `std::pow` is fine for simplicity, but a more efficient version could compute powers incrementally. Time complexity is \(O(\text{iterations})\), where iterations ~ \( \lceil \frac{\sqrt[3]{6/\text{tolerance}}}{2} \rceil \) roughly; for tolerance = 1e-6, about 140 iterations. Space complexity is \(O(1)\) as we only store a few doubles.
