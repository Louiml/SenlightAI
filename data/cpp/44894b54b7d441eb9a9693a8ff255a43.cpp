// Write a C++ function `evaluate(int a, int b, char op)` that performs the arithmetic operation indicated by `op` (`+`, `-`, `*`, or `/`) using only the `+` operator for all underlying computations. For subtraction, use the additive inverse; for multiplication, use repeated addition; for division, use repeated subtraction via addition of the divisor’s magnitude. The function must handle negative operands correctly, throw `std::overflow_error` when division by zero is attempted, and return an integer result (truncating toward zero for division, as in standard C++ integer division). Assume all inputs are within the range of `int` and that no overflow occurs in the final result.

#include <cassert>
#include <stdexcept>

// Declare the function to test.
int evaluate(int a, int b, char op);

int main() {
    // Addition
    assert(evaluate(5, 3, '+') == 8);
    assert(evaluate(-5, -3, '+') == -8);
    assert(evaluate(0, 0, '+') == 0);

    // Subtraction
    assert(evaluate(10, 4, '-') == 6);
    assert(evaluate(4, 10, '-') == -6);
    assert(evaluate(-7, -2, '-') == -5);
    assert(evaluate(0, 5, '-') == -5);

    // Multiplication
    assert(evaluate(6, 7, '*') == 42);
    assert(evaluate(-6, 7, '*') == -42);
    assert(evaluate(6, -7, '*') == -42);
    assert(evaluate(-6, -7, '*') == 42);
    assert(evaluate(0, 999, '*') == 0);

    // Division
    assert(evaluate(10, 2, '/') == 5);
    assert(evaluate(-10, 2, '/') == -5);
    assert(evaluate(10, -2, '/') == -5);
    assert(evaluate(-10, -2, '/') == 5);
    assert(evaluate(7, 2, '/') == 3);  // Truncation toward zero
    assert(evaluate(-7, 2, '/') == -3); // Truncation toward zero
    assert(evaluate(0, 5, '/') == 0);

    // Division by zero throws
    bool caught = false;
    try {
        evaluate(1, 0, '/');
    } catch (const std::overflow_error&) {
        caught = true;
    }
    assert(caught);

    return 0;
}

#include <stdexcept>

// Compute -a using only additions.
int negative(int a) {
    int b = a;
    if (a > 0) {
        while (a > 0) {
            b += (-2);
            --a;
        }
    } else {
        while (a < 0) {
            b += 2;
            ++a;
        }
    }
    return b;
}

// Compute absolute value using only additions.
int magnitude(int a) {
    return (a > 0) ? a : negative(a);
}

// Evaluate a op b using only additions for subtraction, multiplication, division.
int evaluate(int a, int b, char op) {
    if (op == '+') {
        return a + b;
    }
    if (op == '-') {
        return a + negative(b);
    }
    if (op == '*') {
        int c = 0;
        for (int i = magnitude(a); i > 0; --i) {
            c += b;
        }
        return (a < 0) ? negative(c) : c;
    }
    if (op == '/') {
        if (b == 0) {
            throw std::overflow_error("division by zero");
        }
        int sign = ((a > 0 && b > 0) || (a < 0 && b < 0)) ? 1 : -1;
        int a_mag = magnitude(a);
        int b_mag = magnitude(b);
        int c = 0;
        int d = 0;
        while (c + b_mag <= a_mag) {
            c += b_mag;
            ++d;
        }
        return (sign == 1) ? d : negative(d);
    }
    // Should never reach here for valid op, but return 0 for safety.
    return 0;
}

// The core challenge is to implement arithmetic using only `+`.  
// - **Negation**: To compute `-x`, note that `-x = x + (-2x)` if we can generate multiples of -2. Simpler: since we can only use `+`, we can build `-x` by adding `-1` repeatedly `|x|` times, but that requires a loop and an auxiliary counter. Alternatively, we can implement `negative(a)` by starting with `a` and adding `2` if `a` is negative, or `-2` if `a` is positive, decrementing the absolute value counter. That yields `-a` in O(|a|) steps.  
// - **Subtraction**: `a - b` = `a + negative(b)`.  
// - **Multiplication**: Compute `|a| * b` by adding `b` a total of `|a|` times, then if `a` is negative, negate the result. This works because multiplication is commutative; we avoid iterating over the possibly larger magnitude. Complexity O(|a|).  
// - **Division**: For `a / b` (where `b != 0`), first determine the sign of the quotient. Use magnitudes: repeatedly add `b_mag` to a running sum `c` while `c + b_mag <= a_mag`, counting how many times this succeeds. The count `d` is `|a| / |b|` truncated. Then apply the sign. Complexity O(|a| / |b|) in the worst case, but O(|a|) since |b| ≥ 1.  
// - **Edge cases**: Division by zero throws. Negative divisors and dividends are handled via magnitude and sign inversion. For `a = 0` and `b != 0`, the loop doesn’t execute, returning 0. For `b = ±1`, the loop runs `|a|` times, which is correct. All functions must avoid using `-`, `*`, `/`, `%` operators explicitly.
