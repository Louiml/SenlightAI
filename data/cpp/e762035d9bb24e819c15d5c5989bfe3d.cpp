Write a C++ function named `evaluateExpression` that takes two integers (`A` and `B`) and a character operator (`op`) as parameters, performs the arithmetic operation indicated by `op` (`+`, `-`, `*`, or `/`), and returns the result as an `int`. For division, if `B` is zero, the function must return a sentinel value of `INT_MIN` to indicate an error (since the division result could legitimately be `INT_MIN` in extreme edge cases, but that is extremely unlikely with typical inputs, so the sentinel serves as an error marker). The function must also handle invalid operators by returning `INT_MIN` as well. The function should use a `switch` statement and ensure `const` correctness for all parameters. The function must not print anything; all input/output handling should be done outside, and the function should only compute and return a value.

// The solution uses a `switch` statement on the character operator `op` to handle each case. For addition, subtraction, and multiplication, the result is computed directly. For division, before performing `A / B`, the function checks if `B == 0`; if so, it returns `INT_MIN` (the sentinel), otherwise it returns the integer quotient (note that C++ integer division truncates toward zero). For an invalid operator, the `default` case returns `INT_MIN`. Since all inputs are integers, overflow is possible for addition, subtraction, or multiplication, but the task does not require overflow detection; the function simply performs the operation directly. The main algorithm has a time complexity of O(1) and space complexity of O(1). The critical edge cases are: division by zero, invalid operators, and the behavior of integer division with negative numbers (C++ truncates toward zero, which matches the original snippet’s behavior). The sentinel `INT_MIN` is chosen because it is a valid integer that can only be obtained from an actual computation if the inputs are `A = INT_MIN` and `B = 1` or `A = INT_MIN` and `B = -1` (for multiplication/division), which is extremely specific; to avoid ambiguity, the test cases will avoid such pathological inputs.

#include <climits>

// Evaluate arithmetic expression A op B.
// Returns the result as int. Returns INT_MIN on division by zero or invalid operator.
int evaluateExpression(const int A, const int B, const char op) {
    switch (op) {
        case '+':
            return A + B;
        case '-':
            return A - B;
        case '*':
            return A * B;
        case '/':
            if (B == 0) {
                return INT_MIN; // Division by zero error sentinel
            }
            return A / B;
        default:
            return INT_MIN; // Invalid operator error sentinel
    }
}

#include <cassert>
#include <climits>

// The solution function is declared above; here is the main test harness.
int main() {
    // Basic arithmetic
    assert(evaluateExpression(10, 5, '+') == 15);
    assert(evaluateExpression(10, 5, '-') == 5);
    assert(evaluateExpression(10, 5, '*') == 50);
    assert(evaluateExpression(10, 5, '/') == 2);

    // Division with negative numbers (truncation toward zero)
    assert(evaluateExpression(-7, 2, '/') == -3);
    assert(evaluateExpression(7, -2, '/') == -3);
    assert(evaluateExpression(-7, -2, '/') == 3);

    // Division by zero returns INT_MIN
    assert(evaluateExpression(10, 0, '/') == INT_MIN);
    assert(evaluateExpression(-10, 0, '/') == INT_MIN);

    // Invalid operator returns INT_MIN
    assert(evaluateExpression(10, 5, '%') == INT_MIN);
    assert(evaluateExpression(10, 5, 'a') == INT_MIN);

    // Zero operands
    assert(evaluateExpression(0, 5, '+') == 5);
    assert(evaluateExpression(0, 5, '*') == 0);
    assert(evaluateExpression(0, 5, '/') == 0);

    // Large values within int range
    assert(evaluateExpression(1000000, 2000000, '+') == 3000000);
    assert(evaluateExpression(1000000, -2000000, '-') == 3000000);

    return 0;
}
