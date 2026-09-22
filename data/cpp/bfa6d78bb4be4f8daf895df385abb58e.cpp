// Write a C++ function `power` that computes \(x^n\) for a given double base `x` and integer exponent `n`, where `n` can be negative, zero, or positive. The function must handle the edge case where `n` is the minimum 32-bit signed integer (`-2147483648`), for which taking the absolute value would overflow. The solution must use recursion with a divide-and-conquer approach (exponentiation by squaring) to achieve logarithmic time complexity, and must not use any built-in power functions (e.g., `std::pow`). The function should be robust for any valid double input, including fractional and negative bases, and should return the mathematically correct result without relying on floating-point precision beyond what is inherent in the computation.

// The core algorithm is exponentiation by squaring: for a positive `n`, compute `half = power(x, n/2)` recursively, then if `n` is even, return `half * half`, else return `half * half * x`. The recursion terminates at `n == 0`, returning `1.0`. For negative `n`, we use the identity \(x^{-n} = 1 / x^n\). The tricky part is when `n` equals `-2147483648` (the minimum `int` value). Negating this overflows to itself, so we cannot simply compute `-n`. Instead, we use the decomposition: \(x^{-2147483648} = 1 / (x^{-2147483647} \cdot x)\). This is safe because `-2147483647` fits in `int`. Another approach is to compute `power(x, n+1)` and divide by `x`, but the safest is the decomposition above. Edge cases include `x == 0` with negative `n` (which would lead to division by zero; but the task does not specify handling of that, so we assume the caller provides valid inputs, or we can leave that undefined—in practice, for a self-contained task, we can ignore that or return `infinity`/`NaN` as the compiler dictates). Time complexity is \(O(\log |n|)\) due to halving the exponent at each recursive step. Space complexity is \(O(\log |n|)\) due to the recursion stack depth, which is also logarithmic.

#include <cmath> // for std::isnan? Not needed, but for completeness of math functions we might not need. Actually we only use standard operators.

// Compute x raised to the power n for integer exponent n (including negative and min int).
// Uses exponentiation by squaring with recursion.
double power(double x, int n) {
    // Base case: any number to the power 0 is 1.
    if (n == 0) {
        return 1.0;
    }
    
    // Positive exponent case.
    if (n > 0) {
        double half = power(x, n / 2);
        if (n % 2 == 0) {
            return half * half;
        } else {
            return half * half * x;
        }
    }
    
    // Negative exponent case.
    // Special case: n == -2147483648 (INT_MIN) cannot be negated without overflow.
    if (n == -2147483647 - 1) { // equivalent to INT_MIN
        // Use: x^(-2147483648) = 1 / (x^(-2147483647) * x)
        double temp = power(x, -2147483647); // n+1 is NOT safe because n is INT_MIN; but -2147483647 is safe to negate.
        // Actually we need x^(-2147483648) = 1 / (x^2147483648). We can compute x^2147483647 and multiply by x.
        // Since -2147483647 is representable, we compute power(x, -2147483647) = 1/x^2147483647.
        // Then x^2147483648 = x * x^2147483647. So inverse = 1/(x * x^2147483647) = (1/x) * (1/x^2147483647) = (1/x) * power(x, -2147483647).
        // So return (1.0 / x) * power(x, -2147483647); But careful: power(x, -2147483647) is 1/x^2147483647. Then multiplying by (1/x) gives 1/x^2147483648. Yes.
        // But we must avoid overflow in exponent. Let's compute as follows:
        // return 1.0 / ( power(x, -2147483647) * x );   // because -n-1 = 2147483647, and -n = 2147483648, but 2147483648 overflows int. So we compute power(x, -(-2147483647)) = power(x, 2147483647) and multiply by x.
        // But power(x, -2147483647) using recursion will eventually compute positive exponent 2147483647, fine.
        // So:
        double positivePart = power(x, 2147483647); // 2147483647 is safe.
        return 1.0 / (positivePart * x);
    }
    
    // General negative exponent (not INT_MIN).
    return 1.0 / power(x, -n);
}

#include <cassert>
#include <cmath>
#include <iostream>

// Include the solution function here (or declare it).
// For the test, we replicate a simplified version or include the above.
// To keep the test self-contained, we put the solution above and then test.

int main() {
    // Basic positive exponent.
    assert(std::abs(power(3.0, 3) - 27.0) < 1e-12);
    assert(std::abs(power(2.0, 10) - 1024.0) < 1e-12);
    assert(std::abs(power(5.0, 0) - 1.0) < 1e-12);
    
    // Negative exponent.
    assert(std::abs(power(2.0, -2) - 0.25) < 1e-12);
    assert(std::abs(power(3.0, -3) - (1.0/27.0)) < 1e-12);
    
    // Odd and even exponents with decimal base.
    assert(std::abs(power(0.5, 4) - 0.0625) < 1e-12);
    assert(std::abs(power(0.5, -2) - 4.0) < 1e-12);
    
    // Negative base.
    assert(std::abs(power(-2.0, 3) - (-8.0)) < 1e-12);
    assert(std::abs(power(-2.0, 2) - 4.0) < 1e-12);
    
    // Edge case: minimum int exponent.
    // x=2, n=-2147483648 => result is 2^-2147483648 = 1/(2^2147483648). This is extremely small, but for test we can compare with 0.0 within tolerance? Actually it's a tiny number, but we can compute using long double if needed. Instead test with x=1, because 1^anything = 1.
    assert(std::abs(power(1.0, -2147483647-1) - 1.0) < 1e-12);
    // Test with x=2 and check that result is very close to 0 (but not zero exactly).
    double minIntResult = power(2.0, -2147483647-1);
    assert(minIntResult > 0.0 && minIntResult < 1e-300); // It's 2^-2147483648, extremely tiny.
    
    // Another edge: x=0.5, n = INT_MIN.
    // 0.5^-2147483648 = 2^2147483648, which is infinity for double, but we can't check infinity easily. Let's use negative base with even |n|.
    assert(std::abs(power(-1.0, -2147483647-1) - 1.0) < 1e-12); // (-1)^even = 1.
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
