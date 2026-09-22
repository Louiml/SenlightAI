/*
Create a C++ function `calculateInterest` that takes three `double` parameters: `principal`, `rate` (annual interest rate as a percentage, e.g., 5 for 5%), and `time` (in years). The function must return a `struct` named `InterestResult` containing two `double` fields: `simple` and `compound`, representing the simple interest and compound interest (compounded annually) on the principal. The function should handle any non-negative inputs, including zero values (where both interests are zero) and fractional years/time values (e.g., 2.5 years). The compound interest formula should be `principal * (pow(1 + rate/100, time) - principal)`. Assume `time >= 0`, `rate >= 0`, and `principal >= 0`. The function must not print anything; it only computes and returns the result.
*/
#include <cmath>

struct InterestResult {
    double simple;
    double compound;
};

// Compute simple and compound interest (compounded annually) for given principal, rate (%), and time (years).
InterestResult calculateInterest(double principal, double rate, double time) {
    InterestResult result;
    result.simple = principal * rate * time / 100.0;
    result.compound = principal * std::pow(1.0 + rate / 100.0, time) - principal;
    return result;
}
#include <cassert>
#include <cmath>

// Include the InterestResult struct and calculateInterest function here (or via header).

int main() {
    // Test case 1: basic values from original snippet
    InterestResult res1 = calculateInterest(1000.0, 5.0, 2.0);
    assert(std::fabs(res1.simple - 100.0) < 1e-9);
    assert(std::fabs(res1.compound - 102.5) < 1e-9);

    // Test case 2: zero principal
    InterestResult res2 = calculateInterest(0.0, 7.5, 3.0);
    assert(res2.simple == 0.0);
    assert(res2.compound == 0.0);

    // Test case 3: zero rate
    InterestResult res3 = calculateInterest(5000.0, 0.0, 10.0);
    assert(res3.simple == 0.0);
    assert(res3.compound == 0.0);

    // Test case 4: zero time (both interests zero, even with principal and rate)
    InterestResult res4 = calculateInterest(2000.0, 4.0, 0.0);
    assert(res4.simple == 0.0);
    assert(res4.compound == 0.0);

    // Test case 5: fractional time (1.5 years)
    InterestResult res5 = calculateInterest(100.0, 10.0, 1.5);
    assert(std::fabs(res5.simple - 15.0) < 1e-9);
    double expected_ci = 100.0 * std::pow(1.1, 1.5) - 100.0;
    assert(std::fabs(res5.compound - expected_ci) < 1e-9);

    // Test case 6: large values (just ensure no crash, simple check)
    InterestResult res6 = calculateInterest(1e6, 12.0, 10.0);
    assert(res6.simple > 0.0);
    assert(res6.compound > res6.simple);

    // Test case 7: small fractional rate and time
    InterestResult res7 = calculateInterest(50.0, 0.5, 1.0);
    assert(std::fabs(res7.simple - 0.25) < 1e-9);
    assert(std::fabs(res7.compound - 0.25) < 1e-9);

    return 0;
}
// The solution uses the standard formulas: simple interest = `principal * rate * time / 100`, and compound interest = `principal * pow(1 + rate/100, time) - principal`. The `pow` function from `<cmath>` is used to handle integer or fractional time exponents. Edge cases: zero principal (both interests zero), zero rate (both zero), zero time (both zero), very large values (potential overflow is not guarded, but `double` handles typical ranges), and fractional time (pow handles non‑integer exponents). The algorithm is purely computational with constant time and space complexity: O(1) time and O(1) auxiliary space. The function is `const`‑correct because it does not modify any parameters and returns a value type.
