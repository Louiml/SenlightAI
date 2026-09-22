// Write a standalone C++ function that computes the sum of the reciprocals of even integers (1/2 + 1/4 + 1/6 + ...) until the current term becomes smaller than a given positive epsilon (default 1e-6). The function should take no explicit parameters (use an internal default epsilon constant) and return a `double` representing the accumulated sum. The series starts at 1/2 and adds terms with denominators increasing by 2 each step. The loop must stop *before* adding any term that is strictly less than epsilon. The function should be declared with `const`-correctness where appropriate, and the computation must avoid integer division pitfalls.

#include <cassert>
#include <cmath>

// Declaration of the function under test (assume it is defined elsewhere)
double sumEvenReciprocals();

int main() {
    // The series sum is half of the harmonic series truncated at n=1e6,
    // approximately 0.5 * (ln(1e6) + gamma) roughly 7.5. Exact value is about 7.485.
    double result = sumEvenReciprocals();

    // Basic sanity checks using approximate equality due to floating-point
    assert(result > 7.0 && result < 8.0);

    // Check that the sum is monotonically increasing and positive
    assert(result > 0.5);

    // Compare with a manual computation using a loop with same epsilon
    double manual_sum = 0.0;
    for (int denom = 2; (1.0 / denom) >= 1e-6; denom += 2) {
        manual_sum += 1.0 / denom;
    }
    assert(std::fabs(result - manual_sum) < 1e-9);

    // Verify the last term added is >= epsilon and the next term is < epsilon
    // Find last denominator used: since loop stops when term < epsilon,
    // the last denominator is the largest even number with 1/d >= 1e-6.
    int last_denom = 2;
    while ((1.0 / (last_denom + 2)) >= 1e-6) {
        last_denom += 2;
    }
    assert(1.0 / last_denom >= 1e-6);
    assert(1.0 / (last_denom + 2) < 1e-6);

    // Check result is finite and not NaN
    assert(std::isfinite(result));

    return 0;
}

#include <cmath>

// Computes the sum of reciprocals of even integers (1/2 + 1/4 + ...)
// until the current term is less than the internal epsilon constant.
double sumEvenReciprocals() {
    const double epsilon = 1e-6;  // Stopping threshold
    double sum = 0.0;
    double term = 0.5;            // 1/2
    int denominator = 2;

    while (term >= epsilon) {
        sum += term;
        denominator += 2;
        term = 1.0 / denominator;  // Use double division
    }

    return sum;
}

// The problem is a straightforward summation of a divergent harmonic-like series but truncated when terms fall below a threshold. The algorithm initializes the sum to 0.0, the first denominator to 2, and the current term to 1.0/2.0. While the current term is greater than or equal to epsilon (1e-6), we add it to the sum, then compute the next term as 1.0/(denominator+2). A critical edge case is the initial term: if epsilon were larger than 0.5, the loop might not run, but with epsilon=1e-6 it always does. Another edge case is ensuring we use floating-point division (e.g., `1.0 / denominator`) rather than integer division to avoid truncation to zero. The loop terminates when term < epsilon, so the final added term is ≥ epsilon. Time complexity is O(1/epsilon) terms, i.e., roughly 500,000 iterations for epsilon=1e-6, which is fine. Space complexity is O(1). Use `double` for higher precision than the original `float`.
