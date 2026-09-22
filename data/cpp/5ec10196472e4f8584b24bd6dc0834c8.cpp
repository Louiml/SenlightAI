// Write a C++ function named `divideIntegers` that takes two integer parameters, `dividend` and `divisor`, and returns their integer division result (truncated toward zero) as an `int`. The function must handle all edge cases where the mathematical result would overflow a 32-bit signed integer: if `dividend` is `INT_MIN` (`-2147483648`) and `divisor` is `-1`, the result would be `2147483648` which exceeds `INT_MAX`, so the function must return `INT_MAX`. Also ensure that division by zero is handled gracefully — since the problem context assumes no zero divisor, you may simply use standard integer division semantics but must document that precondition. The function should compute the result using absolute values (using `long` or `long long` to avoid overflow during absolute value of `INT_MIN`) and then apply the correct sign based on whether exactly one of the operands is negative. Do not use multiplication, division, or modulo operators in your solution (you must implement division via repeated subtraction or bit manipulation) — however, for simplicity in this task you may use the built-in division operator, but you must at least discuss the approach. Provide an implementation that is robust, const-correct (parameters are values, so no const needed), and includes clear comments.
// The core problem is to perform integer division with truncation toward zero while preventing undefined behavior from an overflow of the result. The main edge case is `INT_MIN / -1`, which mathematically equals `2147483648`, but `INT_MAX` is the largest representable `int`, so the standard behavior in languages like C++ is undefined. We explicitly catch that case and return `INT_MAX`. The second special case is when `dividend == INT_MIN` and `divisor == 1`, which equals `INT_MIN` (no overflow). For all other cases, we can safely convert both operands to their absolute values using a `long` (which is at least 32 bits but typically 64 bits on many platforms, enough to hold `2147483648`). Then we compute the quotient using the built-in division operator (or, in the full spirit of the problem, via repeated subtraction — but for brevity we use the built-in division for the core value since the challenge is overflow handling). Finally, we apply a negative sign if exactly one of the operands is negative. The time complexity is O(1) if using built-in division, and the space complexity is O(1). If implementing division via repeated subtraction, the worst-case time would be O(abs(dividend)) which is huge, so a bit-manipulation method (like the one in the original LeetCode problem) would be O(log n). For this task, we assume built-in division is allowed, but the analysis should mention alternatives.
#include <climits>

// Perform integer division of dividend by divisor, truncating toward zero.
// Returns INT_MAX if the result would overflow (INT_MIN / -1 case).
// Precondition: divisor != 0 (standard division rule, not explicitly handled).
int divideIntegers(int dividend, int divisor) {
    // Handle the only overflow case.
    if (dividend == INT_MIN && divisor == -1) {
        return INT_MAX;
    }

    // Use long to safely hold the absolute value of INT_MIN.
    long dd = (dividend < 0) ? -(long)dividend : (long)dividend;
    long dv = (divisor < 0) ? -(long)divisor : (long)divisor;

    // Compute the quotient using built-in division.
    long res = dd / dv;

    // If exactly one of the operands is negative, the result is negative.
    if ((dividend < 0) ^ (divisor < 0)) {
        return -(int)res;
    }
    return (int)res;
}
#include <cassert>
#include <climits>

// Declare the function being tested (included via the solution).
int divideIntegers(int, int);

int main() {
    // Basic positive division.
    assert(divideIntegers(10, 3) == 3);
    assert(divideIntegers(10, -3) == -3);
    assert(divideIntegers(-10, 3) == -3);
    assert(divideIntegers(-10, -3) == 3);
    
    // Division by 1 and -1.
    assert(divideIntegers(INT_MAX, 1) == INT_MAX);
    assert(divideIntegers(INT_MAX, -1) == -INT_MAX); // Note: -INT_MAX is representable.
    assert(divideIntegers(INT_MIN, 1) == INT_MIN);
    
    // Overflow case.
    assert(divideIntegers(INT_MIN, -1) == INT_MAX);
    
    // Exact division.
    assert(divideIntegers(100, 5) == 20);
    assert(divideIntegers(-100, 5) == -20);
    assert(divideIntegers(100, -5) == -20);
    assert(divideIntegers(-100, -5) == 20);
    
    // Division resulting in zero (truncation toward zero).
    assert(divideIntegers(1, 2) == 0);
    assert(divideIntegers(-1, 2) == 0);
    assert(divideIntegers(1, -2) == 0);
    assert(divideIntegers(-1, -2) == 0);
    
    // Larger values but within safe range.
    assert(divideIntegers(2000000000, 2) == 1000000000);
    assert(divideIntegers(-2000000000, 2) == -1000000000);
    
    return 0;
}
