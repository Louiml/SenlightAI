Write a C++ function named `evaluateExpression` that accepts a single `double` value `x` and computes the mathematical result of the given formula:  
\( y = \frac{x}{(x-3)(x-1)} + x \cdot \sqrt[5]{x-1} \cdot \frac{1}{x^3} - 2(x-1)^3 \)  
where \(\sqrt[5]{z}\) denotes the real fifth root of \(z\) (which is defined for all real \(z\), including negative values). The function must return the computed `double` result. Handle all valid real inputs except where the expression is undefined (e.g., \(x = 0\), \(x = 1\), \(x = 3\)). For invalid inputs, the function should return `std::numeric_limits<double>::quiet_NaN()` to signal an error. Ensure that the fifth-root computation is performed correctly for negative bases (since the default `std::pow` with a fractional exponent would return `NaN` for negative bases).

#include <cmath>
#include <cassert>
#include <limits>

int main() {
    const double eps = 1e-9;

    // Test with x = 2 (valid, simple integer)
    double expected2 = (2.0 / ((2.0-3.0)*(2.0-1.0))) + 2.0 * std::pow(1.0, 0.2) * (1.0/8.0) - 2.0*std::pow(1.0, 3.0);
    assert(std::abs(evaluateExpression(2.0) - expected2) < eps);

    // Test with x = 4 (valid, base of fifth root is positive)
    double expected4 = (4.0 / ((4.0-3.0)*(4.0-1.0))) + 4.0 * std::pow(3.0, 0.2) * (1.0/64.0) - 2.0*std::pow(3.0, 3.0);
    assert(std::abs(evaluateExpression(4.0) - expected4) < eps);

    // Test with x = 0.5 (valid, base of fifth root is negative)
    double baseNeg = -0.5;
    double fifthNeg = -std::pow(0.5, 0.2);
    double expected05 = (0.5 / ((0.5-3.0)*(0.5-1.0))) + 0.5 * fifthNeg * (1.0/(0.5*0.5*0.5)) - 2.0*std::pow(-0.5, 3.0);
    assert(std::abs(evaluateExpression(0.5) - expected05) < eps);

    // Test invalid inputs
    assert(std::isnan(evaluateExpression(0.0)));
    assert(std::isnan(evaluateExpression(1.0)));
    assert(std::isnan(evaluateExpression(3.0)));

    // Test with x = -2 (valid, base negative and x not zero)
    double expectedNeg2 = ((-2.0) / ((-2.0-3.0)*(-2.0-1.0))) + (-2.0) * (-std::pow(3.0, 0.2)) * (1.0/(-8.0)) - 2.0*std::pow(-3.0, 3.0);
    assert(std::abs(evaluateExpression(-2.0) - expectedNeg2) < eps);

    return 0;
}

#include <cmath>
#include <limits>

// Computes the real fifth root of a double, supporting negative bases.
double fifthRoot(double z) {
    if (z == 0.0) return 0.0;
    double magnitude = std::pow(std::abs(z), 1.0 / 5.0);
    return (z < 0.0) ? -magnitude : magnitude;
}

// Evaluates the given mathematical expression for a real input x.
// Returns quiet_NaN if x is 0, 1, or 3 (undefined denominators).
double evaluateExpression(double x) {
    if (x == 0.0 || x == 1.0 || x == 3.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    const double term1 = x / ((x - 3.0) * (x - 1.0));
    const double term2 = x * fifthRoot(x - 1.0) * (1.0 / (x * x * x));
    const double term3 = 2.0 * std::pow(x - 1.0, 3.0);

    return term1 + term2 - term3;
}

// The main challenge is correctly computing the fifth root of a possibly negative base \( (x-1) \). Since \(x-1\) can be negative, we cannot use `std::pow(base, 1.0/5.0)` directly because that would yield `NaN` for negative bases. Instead, we use the identity: for any real \(z\), \(\sqrt[5]{z} = \text{sign}(z) \cdot |z|^{1/5}\). We compute this with `std::pow(std::abs(z), 1.0/5.0)` and then multiply the result by the sign of \(z\) (which is \(-1\), \(0\), or \(1\)).  
//
// The expression is undefined when any denominator is zero or when \(x^3\) is zero (i.e., \(x=0\)), which also aligns with the fifth root of \(x-1\) at \(x=1\) (but that is not undefined; it just gives 0). The denominators are: \( (x-3)(x-1) \) in the first term and \(x^3\) in the second term. So the function returns `NaN` if \(x == 0\) or \(x == 1\) or \(x == 3\). For all other real \(x\), we compute:  
// 1. First term: \( x / ((x-3)*(x-1)) \)  
// 2. Second term: \( x \cdot \text{fifthRoot}(x-1) \cdot (1.0 / (x^3)) \)  
// 3. Third term: \( 2 \cdot \text{pow}(x-1, 3) \)  
// Then sum first + second - third.  
// Time complexity: \(O(1)\) (constant number of operations). Space complexity: \(O(1)\) (only a few local variables). Edge cases: `x` being exactly 0, 1, or 3 yields `NaN`; `x` very close to these values may produce large finite results (no overflow handling needed for typical double range, but we could clamp if necessary, but not required).
