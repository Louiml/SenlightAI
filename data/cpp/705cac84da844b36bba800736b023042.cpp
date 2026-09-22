/*
Write a C++ function named `evaluateBooleanExpression` that takes four integer parameters (a, b, c, d) and returns a boolean value indicating whether the sum of the first two integers is strictly less than the difference of the last two integers. The function must handle any integer inputs, including negative numbers, zeros, and extreme values like INT_MAX and INT_MIN, without causing undefined behavior. The core operation is `(a + b) < (c - d)`, but you must guard against integer overflow in both the addition and the subtraction. If overflow would occur in either operation, return `false` (since you cannot safely determine the comparison, and the task treats overflow as making the condition false). In all other cases, return the correct boolean result.
*/
#include <limits>

// Returns true if (a + b) < (c - d) without causing signed integer overflow.
// If overflow would occur in a+b or c-d, returns false.
bool evaluateBooleanExpression(int a, int b, int c, int d) {
    // Check if a+b overflows.
    if (b > 0 && a > std::numeric_limits<int>::max() - b) {
        return false;
    }
    if (b < 0 && a < std::numeric_limits<int>::min() - b) {
        return false;
    }

    // Compute left side safely (no overflow guaranteed now).
    int left = a + b;

    // Check if c-d overflows.
    if (d < 0 && c > std::numeric_limits<int>::max() + d) {
        return false;
    }
    if (d > 0 && c < std::numeric_limits<int>::min() + d) {
        return false;
    }

    // Compute right side safely.
    int right = c - d;

    return left < right;
}
#include <cassert>
#include <climits>

int main() {
    // Basic positive case from snippet: (3+2) < (13-6) => 5 < 7 => true.
    assert(evaluateBooleanExpression(3, 2, 13, 6) == true);
    // Basic opposite: (3+2) < (6-13) => 5 < -7 => false.
    assert(evaluateBooleanExpression(3, 2, 6, 13) == false);
    // Equal results: (1+1) < (3-1) => 2 < 2 => false.
    assert(evaluateBooleanExpression(1, 1, 3, 1) == false);
    // Negative numbers: (-5 + 2) < (-1 - 2) => -3 < -3? false.
    assert(evaluateBooleanExpression(-5, 2, -1, 2) == false);
    // Overflow in sum: INT_MAX + 1 would overflow, return false.
    assert(evaluateBooleanExpression(INT_MAX, 1, 0, 0) == false);
    // Overflow in difference: INT_MIN - 1 would overflow, return false.
    assert(evaluateBooleanExpression(0, 0, INT_MIN, 1) == false);
    // Safe with extreme but no overflow: INT_MAX + (-1) = INT_MAX-1, (INT_MAX) - (-1) overflow? Actually c=INT_MAX, d=-1 => INT_MAX -(-1)=INT_MAX+1 overflow -> false.
    assert(evaluateBooleanExpression(INT_MAX, -1, INT_MAX, -1) == false);
    // Same sign subtraction safe: c=INT_MIN, d=-1 => INT_MIN - (-1) = INT_MIN+1 safe, compare with a+b=0, so 0 < INT_MIN+1? false.
    assert(evaluateBooleanExpression(0, 0, INT_MIN, -1) == false);
    // Opposite sign addition safe: INT_MAX + (-1) = INT_MAX-1, c=0, d=0 => 0<0? false.
    assert(evaluateBooleanExpression(INT_MAX, -1, 0, 0) == false);
    // True with safe extreme: INT_MAX + 0 = INT_MAX, INT_MIN - (-1) = INT_MIN+1, compare INT_MAX < INT_MIN+1? false.
    assert(evaluateBooleanExpression(INT_MAX, 0, INT_MIN, -1) == false);
}
// The solution requires safely performing the comparison `(a + b) < (c - d)` without risking signed integer overflow. We need to check each operation separately. For the addition `a + b`, overflow occurs if both operands have the same sign and the result would exceed that sign's bounds. Specifically, if both are positive and `a > INT_MAX - b`, or both are negative and `a < INT_MIN - b`, overflow occurs. For the subtraction `c - d`, overflow occurs when the operands have opposite signs and subtracting would exceed bounds: if `c` is positive and `d` is negative and `c > INT_MAX + d` (since `d` is negative, `INT_MAX + d` is less than INT_MAX), or if `c` is negative and `d` is positive and `c < INT_MIN + d`. We can use careful comparisons without arithmetic that would itself overflow. Edge cases: when `a` and `b` have opposite signs, addition cannot overflow; when `c` and `d` have the same sign, subtraction cannot overflow. Time complexity is O(1), space complexity is O(1).
