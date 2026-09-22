/*
Write a C++ free function named `evaluatePolynomial` that takes five integer parameters representing a three-term polynomial: two coefficients, two exponents, and a constant term, along with the value of `x` at which to evaluate the polynomial. The function must compute and return the value of the polynomial `coeff1 * x^exp1 + coeff2 * x^exp2 + constant`, correctly handling negative values of `x` and any integer exponents (including zero). Assume all inputs are integers and that the exponents are non-negative. Do not perform any input/output inside the function; only compute and return the result. Use `std::pow` for exponentiation (or implement a loop-based power for integer exponents if you prefer, but be careful with zero exponent returning 1). The solution must compile standalone with C++11 or later, include necessary headers, and apply `const` correctness to parameters where appropriate.
*/
#include <cmath>
#include <cstdint>

// Evaluate coeff1 * x^exp1 + coeff2 * x^exp2 + constant for integer inputs.
// Exponents are assumed non-negative. Returns the integer result.
std::int64_t evaluatePolynomial(
    const int coeff1,
    const int exp1,
    const int coeff2,
    const int exp2,
    const int constant,
    const int x
) {
    // Helper to compute integer power, handling exp==0 specially.
    auto intPow = [](const int base, const int exp) -> std::int64_t {
        if (exp == 0) {
            return 1;
        }
        std::int64_t result = 1;
        for (int i = 0; i < exp; ++i) {
            result *= base;
        }
        return result;
    };

    // Compute each term as int64 to avoid overflow during intermediate sums.
    std::int64_t term1 = static_cast<std::int64_t>(coeff1) * intPow(x, exp1);
    std::int64_t term2 = static_cast<std::int64_t>(coeff2) * intPow(x, exp2);
    std::int64_t total = term1 + term2 + constant;
    return total;
}
#include <cassert>
#include <cstdint>

// Function declaration is assumed from the solution above.
// We replicate the signature here for completeness.

int main() {
    // Basic polynomial: x^2 + x + 1, evaluate at x=3 => 9+3+1=13
    assert(evaluatePolynomial(1,2,1,1,1,3) == 13);

    // Constant polynomial: 0*x^0 + 0*x^1 + 5 => 5
    assert(evaluatePolynomial(0,0,0,1,5,10) == 5);

    // Negative coefficients and x: -2*x^3 + 3*x^2 + 4, at x=-1 => -2*(-1)^3 + 3*1 + 4 = 2+3+4=9
    assert(evaluatePolynomial(-2,3,3,2,4,-1) == 9);

    // Zero exponent: coeff1*x^0 = coeff1, so 7 + 2*x^1 + constant at x=2 => 7 + 4 + 11 = 22
    assert(evaluatePolynomial(7,0,2,1,11,2) == 22);

    // Exponent zero for both, constant only: 3 + 4 + 5 = 12
    assert(evaluatePolynomial(3,0,4,0,5,123) == 12);

    // Large values: 10^10 + 10^10 + 0 with x=10, exp=10 => 2*10^10 = 20000000000
    assert(evaluatePolynomial(1,10,1,10,0,10) == 20000000000LL);

    // Negative base with even exponent: (-2)^2 = 4, plus 1 => 5
    assert(evaluatePolynomial(1,2,0,1,1,-2) == 4 + 1); // = 5

    // Mixed signs: 2*x^1 - 3*x^2 + 6 at x=4 => 8 - 48 + 6 = -34
    assert(evaluatePolynomial(2,1,-3,2,6,4) == -34);

    // x=0: 0^0 is 1, so 5*x^0 + 2*x^1 + 3 => 5 + 0 + 3 = 8
    assert(evaluatePolynomial(5,0,2,1,3,0) == 8);

    // Negative exponent? Not allowed but test zero exponent and constant only.
    assert(evaluatePolynomial(0,0,0,0,-7,100) == -7);
}
// The core task is straightforward arithmetic evaluation. The function must accept five integer inputs plus the evaluation variable `x`, and return an integer. The main algorithm: read the coefficients and exponents, then compute `coeff1 * pow(x, exp1) + coeff2 * pow(x, exp2) + constant`. Important edge cases: 
// - When `exp` is 0, `pow(x,0)` should be 1 for any `x` (including 0). `std::pow(0,0)` returns 1 in C++ (though technically `std::pow(0,0)` may be implementation-defined, but in practice most compilers return 1). To be safe, we can handle a zero exponent explicitly returning 1.
// - Negative `x` with integer exponents works fine with `std::pow` as long as the exponent is an integer, since `std::pow(double, double)` returns a double, but for negative bases with integer exponents, the result is correct (e.g., `pow(-2,3) = -8`). However, `std::pow` returns a floating-point value; for large exponents and large coefficients, there might be precision loss, but since inputs are integers and exponents are non-negative small, we can cast the result to `long long` to avoid overflow for typical test cases. 
// - The polynomial has three terms: sum of two power terms plus a constant, so the constant is added directly.
// Time complexity: O(1) because we perform a fixed number of arithmetic operations and calls to `pow`. Space complexity: O(1).
