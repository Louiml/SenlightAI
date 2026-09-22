Write a C++ function named `evaluatePostfix` that takes a string representing a mathematical expression in postfix (reverse Polish) notation using single-digit operands (0-9) and the binary operators `+`, `-`, `*`, and `/`. The function should evaluate the expression and return the result as a `double`. Division must be performed as floating-point division (not integer division). The input will be a valid postfix expression with no spaces, so each character is either a digit or an operator. The function must correctly handle any valid sequence, including expressions that produce fractional results and negative results. Assume no division by zero occurs. The function should mimic the behavior of evaluating a stack-based postfix calculator. Do not include a `main` function; only provide the requested function.
The solution uses a stack of `double` values. We iterate over each character of the input string. If the character is a digit (`'0'` to `'9'`), we convert it to a number (by subtracting `'0'`) and push it onto the stack. If the character is an operator, we pop the top two values from the stack. Since postfix notation has the operands in order, the first popped value is the right operand, and the second popped value is the left operand. We then apply the operator: for `+`, we add; for `-`, we subtract; for `*`, we multiply; for `/`, we divide as doubles. The result is pushed back onto the stack. After processing all characters, the stack should contain exactly one value, which is the final result; we return that value. Edge cases include expressions with a single digit (e.g., `"5"` returns `5.0`), expressions that yield negative results (e.g., `"3 5 -"` yields `-2`), and expressions with division yielding non-integer results (e.g., `"7 2 /"` yields `3.5`). Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case if the expression is all digits, but typically O(depth of stack) which is at most the number of digits.
#include <string>
#include <stack>

// Evaluate a postfix expression with single-digit operands and +, -, *, / operators.
// Returns the result as a double. Assumes valid input and no division by zero.
double evaluatePostfix(const std::string& expr) {
    std::stack<double> stk;
    for (char ch : expr) {
        if (ch >= '0' && ch <= '9') {
            stk.push(static_cast<double>(ch - '0'));
        } else {
            // Pop right then left operand
            double right = stk.top(); stk.pop();
            double left = stk.top(); stk.pop();
            double result = 0.0;
            switch (ch) {
                case '+': result = left + right; break;
                case '-': result = left - right; break;
                case '*': result = left * right; break;
                case '/': result = left / right; break;
                // No default needed; input is assumed valid
            }
            stk.push(result);
        }
    }
    return stk.top();
}
#include <cassert>
#include <cmath>

int main() {
    // Basic arithmetic
    assert(evaluatePostfix("23+") == 5.0);          // 2 + 3
    assert(evaluatePostfix("53-") == 2.0);          // 5 - 3
    assert(evaluatePostfix("32*") == 6.0);          // 3 * 2
    assert(evaluatePostfix("84/") == 2.0);          // 8 / 4

    // Single digit
    assert(evaluatePostfix("7") == 7.0);

    // Negative result
    assert(evaluatePostfix("35-") == -2.0);         // 3 - 5 = -2

    // Fractional division
    assert(fabs(evaluatePostfix("72/") - 3.5) < 1e-9);  // 7 / 2 = 3.5

    // Nested expression
    assert(evaluatePostfix("23+4*") == 20.0);       // (2+3)*4
    assert(evaluatePostfix("234*+") == 14.0);       // 2 + (3*4)

    // Chained operations
    assert(evaluatePostfix("92/3+") == 7.5);        // (9/2)+3

    return 0;
}
