// Write a standalone C++ function `int evaluatePostfix(const std::string& expr)` that evaluates a postfix (Reverse Polish Notation) expression given as a string. The expression may contain single-digit operands (`'0'`–`'9'`) and the binary operators `+`, `-`, `*`, `/`, and `^` (exponentiation, assume integer exponent). All numbers are non-negative single digits, and division is integer division (truncating toward zero). The expression is guaranteed to be valid: every operator has exactly two operands available on the stack, and the final result is the single value left on the stack. The function must return the integer result.

The solution uses a stack to hold intermediate integer values. We scan the string character by character from left to right. If the character is a digit, we convert it to an integer (by subtracting `'0'`) and push it onto the stack. If it is an operator, we pop the top two elements: the first popped is the right operand (`op1`), the second popped is the left operand (`op2`). For each operator, we compute the result as follows: for `+` → `op2 + op1`, for `-` → `op2 - op1`, for `*` → `op2 * op1`, for `/` → `op2 / op1` (integer division), and for `^` → `std::pow(op2, op1)` which returns a double—since both are non-negative small integers, we can cast to `int` using `static_cast<int>(std::pow(op2, op1))` which is safe here (no rounding issues for typical small values, but for robustness, we can manually implement integer exponentiation via a loop to avoid floating-point issues). After processing all characters, the top of the stack is the result. **Edge cases:** The expression may be a single digit (e.g., `"5"`), which returns 5. Division by zero is not allowed per the problem guarantee. Operator precedence is implicit in postfix notation, so no parsing beyond the stack is needed. The input string contains no spaces or invalid characters. **Time complexity:** O(n) where n is the length of the string. **Space complexity:** O(n) in the worst case for the stack (e.g., expression of all digits before any operation), but typically it is bounded by the number of operands present.

#include <string>
#include <stack>
#include <cmath>

// Evaluate a postfix expression with single-digit operands and operators +, -, *, /, ^.
// Assumes the expression is valid and results fit in an int.
int evaluatePostfix(const std::string& expr) {
    std::stack<int> operands;
    
    for (char ch : expr) {
        if (ch >= '0' && ch <= '9') {
            operands.push(ch - '0');
        } else {
            int right = operands.top();
            operands.pop();
            int left = operands.top();
            operands.pop();
            
            switch (ch) {
                case '+':
                    operands.push(left + right);
                    break;
                case '-':
                    operands.push(left - right);
                    break;
                case '*':
                    operands.push(left * right);
                    break;
                case '/':
                    operands.push(left / right);
                    break;
                case '^': {
                    // Manual integer exponentiation for exact results
                    int result = 1;
                    for (int i = 0; i < right; ++i) {
                        result *= left;
                    }
                    operands.push(result);
                    break;
                }
                default:
                    // Should never happen for valid input
                    break;
            }
        }
    }
    return operands.top();
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    assert(evaluatePostfix("5") == 5);
    assert(evaluatePostfix("23+") == 5);          // 2 + 3
    assert(evaluatePostfix("23*5+") == 11);       // 2*3 + 5
    assert(evaluatePostfix("23-") == -1);         // 2 - 3
    assert(evaluatePostfix("84/") == 2);          // 8 / 4
    assert(evaluatePostfix("23^") == 8);          // 2^3 = 8
    assert(evaluatePostfix("23*5+") == 11);
    assert(evaluatePostfix("23+4*") == 20);       // (2+3)*4
    assert(evaluatePostfix("53+82-*") == 48);     // (5+3)*(8-2) = 8*6 = 48
    assert(evaluatePostfix("12+34+*") == 21);     // (1+2)*(3+4) = 3*7
    return 0;
}
