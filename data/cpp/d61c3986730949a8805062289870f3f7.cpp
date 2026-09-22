/*
Write a C++ function that simulates the evaluation of a specific expression using integer variables. Given three integers `a`, `b`, and `c` initialized to 10, 20, and 0 respectively, compute the value of the expression `++a + 2*a + b++` and return both the final computed value and the updated values of `a` and `b` after the evaluation. The function should accept the initial values of `a` and `b` as parameters (defaulting to 10 and 20 if not provided), and return a structure containing the computed result, the final `a`, and the final `b`. Ensure the behavior matches C++ operator precedence and pre/post-increment semantics exactly. The function must be `const`-correct and avoid any side effects outside its return value.
*/
#include <tuple>

// Structure to hold the result and updated variables.
struct EvaluationResult {
    int result;
    int a_final;
    int b_final;
};

// Simulate the expression: ++a + 2*a + b++
// Returns the computed result and the final values of a and b.
EvaluationResult evaluateExpression(int a, int b) {
    // Evaluate the expression exactly as in the snippet.
    int result = ++a + 2 * a + b++;
    // a is already updated (pre-increment), b is updated (post-increment).
    return {result, a, b};
}
#include <cassert>

int main() {
    // Test with default initial values (10, 20).
    auto r1 = evaluateExpression(10, 20);
    assert(r1.result == 53);
    assert(r1.a_final == 11);
    assert(r1.b_final == 21);

    // Test with zero initial values.
    auto r2 = evaluateExpression(0, 0);
    // ++a -> 1, 2*a -> 2, b++ -> 0 (then b becomes 1)
    // result = 1 + 2 + 0 = 3
    assert(r2.result == 3);
    assert(r2.a_final == 1);
    assert(r2.b_final == 1);

    // Test with negative values.
    auto r3 = evaluateExpression(-5, -2);
    // ++a -> -4, 2*a -> -8, b++ -> -2 (then b becomes -1)
    // result = -4 + (-8) + (-2) = -14
    assert(r3.result == -14);
    assert(r3.a_final == -4);
    assert(r3.b_final == -1);

    // Test with large values to ensure no overflow in normal int range.
    auto r4 = evaluateExpression(1000, 1000);
    // ++a -> 1001, 2*a -> 2002, b++ -> 1000 (then b becomes 1001)
    // result = 1001 + 2002 + 1000 = 4003
    assert(r4.result == 4003);
    assert(r4.a_final == 1001);
    assert(r4.b_final == 1001);

    // Test with custom values where a and b are equal.
    auto r5 = evaluateExpression(7, 7);
    // ++a -> 8, 2*a -> 16, b++ -> 7 (then b becomes 8)
    // result = 8 + 16 + 7 = 31
    assert(r5.result == 31);
    assert(r5.a_final == 8);
    assert(r5.b_final == 8);
}
// The expression `++a + 2*a + b++` is evaluated left-to-right with standard C++ operator precedence. First, `++a` pre-increments `a` from 10 to 11, and that operand evaluates to 11. Next, `2*a` multiplies the already-incremented `a` (11) by 2, giving 22. Then `b++` uses the current value of `b` (20) in the addition, and then increments `b` to 21 after the expression is evaluated. The sum is 11 + 22 + 20 = 53. After the expression, `a` remains 11 and `b` becomes 21. Edge cases: if the function is called with custom initial values, the logic remains the same: pre-increment modifies `a` before use, and post-increment modifies `b` after use. The function should return all three values via a struct or tuple. Time complexity is O(1) and space complexity is O(1), as only a few integer operations are performed.
