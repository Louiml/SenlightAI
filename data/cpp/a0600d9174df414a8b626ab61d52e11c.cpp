// Write a C++ function `double findUnimodalMaximum(int n, const double coefficients[], double left, double right)` that, given a polynomial of degree `n` (with real coefficients stored in descending order: the coefficient of `x^n` first, followed by `x^(n-1)`, …, down to the constant term), and an interval `[left, right]` on which the polynomial is strictly unimodal (i.e., it has exactly one maximum and is strictly increasing then strictly decreasing), returns the `x`-coordinate of that maximum with an absolute error no greater than `1e-6`. The polynomial is guaranteed to be unimodal on the given interval. Use the golden-section search or ternary search method. Do not use any external math libraries beyond `<cmath>` (if needed) and standard headers. Your function should be self-contained and not rely on any global variables.
#include <cassert>
#include <cmath>

// The solution function is defined above (assume it's included).

int main() {
    // Test 1: simple parabola -x^2 + 4x + 1, maximum at x = 2
    {
        double coeff[] = {-1.0, 4.0, 1.0}; // -x^2 + 4x + 1
        double result = findUnimodalMaximum(2, coeff, 0.0, 4.0);
        assert(std::fabs(result - 2.0) < 1e-6);
    }
    // Test 2: cubic with maximum in interval, e.g., -x^3 + 3x^2 + 2, max at x = 2 (check: derivative -3x^2+6x = 0 -> x=0 or x=2; on [1,3] max at 2)
    {
        double coeff[] = {-1.0, 3.0, 0.0, 2.0}; // -x^3 + 3x^2 + 2
        double result = findUnimodalMaximum(3, coeff, 1.0, 3.0);
        assert(std::fabs(result - 2.0) < 1e-6);
    }
    // Test 3: linear increasing? Not unimodal normally, but on a narrow interval? Better test: constant - just any point? But assume unimodal.
    {
        double coeff[] = {0.5, -3.0}; // 0.5x - 3, increasing. Not unimodal with maximum inside, but if interval is just a point? Instead test parabola with peak at 0.5
        // Use -x^2 + x + 2, max at 0.5
        double coeff2[] = {-1.0, 1.0, 2.0};
        double result = findUnimodalMaximum(2, coeff2, -1.0, 2.0);
        assert(std::fabs(result - 0.5) < 1e-6);
    }
    // Test 4: very narrow interval
    {
        double coeff[] = {-2.0, 6.0, -1.0}; // -2x^2 + 6x -1, max at x = 1.5
        double result = findUnimodalMaximum(2, coeff, 1.4, 1.6);
        assert(std::fabs(result - 1.5) < 1e-6);
    }
    // Test 5: higher degree, e.g., quartic with single peak
    {
        double coeff[] = {1.0, 0.0, -2.0, 0.0, 1.0}; // x^4 - 2x^2 + 1, has two minima and a maximum at 0? Actually at x=0, f=1; at x=±1, f=0; so max at 0 on [-0.5,0.5]? But that's not unimodal? Actually it's not strictly unimodal. Use simpler: -x^4 + 2x^2 + 3, max at x=0? Actually f'=-4x^3+4x, roots at 0,±1; on [-0.5,0.5] max at 0? Check f(-0.5)= -0.0625+0.5+3=3.4375, f(0)=3, so max isn't at 0. Use -x^4 + 8x^3 -24x^2 +32x -16? Let's just test with a known simple polynomial: -x^4 + 1, max at 0, but that's not unimodal on [-1,1] because flat? Use -x^4 + 4x^2 -2? Let's just skip.
    }
    // Test 6: degree 0 constant (trivial, but should return left or right)
    {
        double coeff[] = {5.0};
        double result = findUnimodalMaximum(0, coeff, -3.0, 7.0);
        // Since constant, any point works; it should be within interval
        assert(result >= -3.0 && result <= 7.0);
    }
    // Additional correctness check: for a parabola, the found point gives value very close to true max
    {
        double coeff[] = {-3.0, 12.0, -5.0}; // -3x^2 + 12x -5, max at x=2
        double result = findUnimodalMaximum(2, coeff, 0.0, 4.0);
        assert(std::fabs(result - 2.0) < 1e-6);
    }
    return 0;
}
#include <vector>
#include <algorithm>
#include <cmath>

// Evaluate polynomial with coefficients in descending order (coeff[0] = a_n, ..., coeff[n] = a_0)
double evaluatePolynomial(int n, const double coefficients[], double x) {
    double result = 0.0;
    for (int i = 0; i <= n; ++i) {
        result = result * x + coefficients[i];
    }
    return result;
}

// Find the x-coordinate of the maximum of a unimodal polynomial on [left, right].
double findUnimodalMaximum(int n, const double coefficients[], double left, double right) {
    const double epsilon = 1e-6;
    while (right - left > epsilon) {
        double third = (right - left) / 3.0;
        double mid1 = left + third;
        double mid2 = right - third;
        double f1 = evaluatePolynomial(n, coefficients, mid1);
        double f2 = evaluatePolynomial(n, coefficients, mid2);
        if (f1 > f2) {
            right = mid2;
        } else {
            left = mid1;
        }
    }
    return left;
}
// The key observation is that the function is unimodal on the interval, so we can apply a ternary search (or golden-section search) to narrow the interval around the maximum. At each step, we compute two interior points, `mid1 = left + (right - left)/3` and `mid2 = right - (right - left)/3`. If `f(mid1) > f(mid2)`, the maximum cannot lie to the right of `mid2`, so we set `right = mid2`; otherwise, the maximum lies to the right of `mid1`, so we set `left = mid1`. We repeat this until the interval width is smaller than a tolerance such as `1e-6`. To evaluate the polynomial efficiently, we use Horner’s method, which evaluates an `n`-degree polynomial in `O(n)` time per evaluation. The ternary search takes `O(log((right-left)/epsilon))` iterations, each performing two evaluations, so the total time is `O(n * log((right-left)/epsilon))`, and the space complexity is `O(1)` beyond the input array. Edge cases include degree 0 (constant) — but the problem guarantees unimodality, so either the constant is the maximum everywhere or the interval is a single point; in any case the algorithm still returns a valid point. Also, if the interval is initially very narrow, the loop may not execute, and we return `left` as is.
