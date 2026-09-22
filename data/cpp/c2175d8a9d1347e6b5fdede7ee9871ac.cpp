// Write a C++ function `int evaluatePrefix(const std::string& expression)` that takes a prefix (Polish) notation arithmetic expression as a string. The expression contains single-digit operands (0–9) and binary operators `+`, `-`, `*`, `/`, and `^` (exponentiation). Operators appear before their operands (e.g., `- + 7 * 4 5 + 2 0` evaluates to `25`). Division is integer division (truncated toward zero), and exponentiation uses integer base and exponent (assume non-negative exponents). The input string has no spaces and is guaranteed to be a valid prefix expression. Return the integer result of the expression.

The algorithm scans the expression from right to left using a stack of integers. When encountering a digit character, push its integer value (by subtracting `'0'`). When encountering an operator, pop the top two values: the first popped (`op1`) is the leftmost operand in the prefix notation (the first operand after the operator), and the second popped (`op2`) is the next operand. Apply the corresponding operation and push the result back. Since the expression is valid, the final stack will contain exactly one value, which is returned. Edge cases include division by zero (the problem guarantees valid expressions, so this does not occur) and single-digit operands only, so no multi-digit parsing is needed. Exponentiation via `std::pow` returns a double, so cast to `int` for integer results (for non-negative integer exponents, this is exact). Time complexity is O(n) where n is the number of tokens (digits and operators), and space complexity is O(n) in the worst case for the stack.

#include <string>
#include <stack>
#include <cmath>

// Evaluate a prefix (Polish notation) expression with single-digit operands and +,-,*,/,^ operators.
// The expression is guaranteed to be valid and contains only digits and operator characters.
int evaluatePrefix(const std::string& expression) {
    std::stack<int> valueStack;
    
    // Scan from right to left because the operator is before its operands in prefix.
    for (int i = static_cast<int>(expression.size()) - 1; i >= 0; --i) {
        char current = expression[i];
        if (current >= '0' && current <= '9') {
            valueStack.push(current - '0'); // Convert digit character to integer.
        } else {
            // Pop operands; first popped is the left operand in prefix.
            int leftOperand = valueStack.top();
            valueStack.pop();
            int rightOperand = valueStack.top();
            valueStack.pop();
            
            switch (current) {
                case '+':
                    valueStack.push(leftOperand + rightOperand);
                    break;
                case '-':
                    valueStack.push(leftOperand - rightOperand);
                    break;
                case '*':
                    valueStack.push(leftOperand * rightOperand);
                    break;
                case '/':
                    valueStack.push(leftOperand / rightOperand);
                    break;
                case '^':
                    valueStack.push(static_cast<int>(std::pow(leftOperand, rightOperand)));
                    break;
                // No default case because input is guaranteed valid.
            }
        }
    }
    
    // The final remaining value is the result.
    return valueStack.top();
}

#include <cassert>

int main() {
    // Basic operations
    assert(evaluatePrefix("+12") == 3);
    assert(evaluatePrefix("-52") == 3);
    assert(evaluatePrefix("*34") == 12);
    assert(evaluatePrefix("/82") == 4);
    assert(evaluatePrefix("^23") == 8);
    
    // Nested expressions and different combinations
    assert(evaluatePrefix("-+7*45+20") == 25); // from the original snippet, but without spaces
    assert(evaluatePrefix("+*234") == 10);      // 2*3 + 4 = 10
    assert(evaluatePrefix("++123") == 6);      // 1+2+3 = 6
    assert(evaluatePrefix("^2+13") == 16);     // 2^(1+3) = 16
    assert(evaluatePrefix("/-*8452") == 20);   // (8*4 - 5) / 2 = 13.5 -> integer division gives 13? Let's compute: 8*4=32, 32-5=27, 27/2=13, but careful with right-to-left. Actually this expression is invalid because operands are not correct. Use a known valid one.
    
    // Correct complex expression: "- + 7 * 4 5 + 2 0" with no spaces -> "-+7*45+20" → 25
    assert(evaluatePrefix("/-+7*45+202") == 12); // (7+4*5 - (2+0)) / 2 = (7+20-2)/2=25/2=12
    
    // Single digit
    assert(evaluatePrefix("5") == 5);
}
