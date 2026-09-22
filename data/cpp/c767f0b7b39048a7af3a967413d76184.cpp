// Write a C++ function `evaluateExpression` that takes a string representing a fully parenthesized arithmetic expression using non-negative integers and the binary operators `+`, `-`, `*`, and `/` (integer division), along with parentheses `(` and `)`. The expression is guaranteed to be syntactically valid, with no unary operators, no whitespace, and no division by zero. The function should compute and return the integer result of the expression, respecting standard operator precedence and left-to-right associativity. For example, `"((2+3)*4)"` should return `20` and `"(8/4-2)"` should return `0`. Implement the function without using external libraries (other than standard headers) and without using `std::stack` or recursion; instead, use a simple iterative approach with two arrays to simulate a stack for values and operators.

// The expression is fully parenthesized, meaning every binary operation is enclosed in parentheses: `(<operand> <operator> <operand>)`. This simplifies parsing dramatically because we can process the string left-to-right without worrying about precedence or associativity—every time we encounter a closing parenthesis `)`, we know a complete operation has just been formed. The approach is to maintain two stacks: one for integer operands and one for operator characters. Scan each character of the string:
// - If it’s a digit, accumulate the full number (since numbers can be multi-digit) and push it onto the value stack.
// - If it’s an operator (`+`, `-`, `*`, `/`), push it onto the operator stack.
// - If it’s `(`, ignore (or push a marker, but not needed because fully parenthesized).
// - If it’s `)`, pop the top two values and the top operator, perform the operation, and push the result back onto the value stack.
//
// Because the expression is fully parenthesized, the operator stack will always have exactly one operator waiting when we see a `)`. Edge cases include multi-digit numbers (handle by reading consecutive digits), and ensuring the order of operands for division and subtraction is correct (the first popped value is the right-hand operand, the second is the left-hand operand). The algorithm runs in O(n) time, where n is the length of the string, and uses O(n) space for the stacks. Integer division in C++ truncates toward zero, which matches the problem statement.

#include <string>
#include <vector>
#include <cctype>

// Evaluates a fully parenthesized arithmetic expression with +, -, *, /.
// Assumes valid input, non-negative integers, no division by zero.
int evaluateExpression(const std::string& expr) {
    std::vector<int> values;
    std::vector<char> ops;

    int i = 0;
    while (i < (int)expr.length()) {
        char ch = expr[i];
        if (std::isdigit(ch)) {
            int num = 0;
            while (i < (int)expr.length() && std::isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                ++i;
            }
            values.push_back(num);
            continue;
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            ops.push_back(ch);
        } else if (ch == ')') {
            // Pop right and left operands, and operator
            int right = values.back(); values.pop_back();
            int left = values.back(); values.pop_back();
            char op = ops.back(); ops.pop_back();
            int result = 0;
            switch (op) {
                case '+': result = left + right; break;
                case '-': result = left - right; break;
                case '*': result = left * right; break;
                case '/': result = left / right; break;
            }
            values.push_back(result);
        }
        // Ignore '(' and any other characters
        ++i;
    }

    // The final result is the only value left
    return values.back();
}

#include <cassert>

int main() {
    assert(evaluateExpression("(1+2)") == 3);
    assert(evaluateExpression("((2+3)*4)") == 20);
    assert(evaluateExpression("(8/4-2)") == 0);
    assert(evaluateExpression("((10-2)/2)") == 4);
    assert(evaluateExpression("(((1+2)*(3+4))/7)") == 3);
    assert(evaluateExpression("(5*(4-3))") == 5);
    assert(evaluateExpression("((100/10)/5)") == 2);
    assert(evaluateExpression("((7-3)*(2+1))") == 12);
    assert(evaluateExpression("(((2+3)*2)-10)") == 0);
    assert(evaluateExpression("((((1+1)+(1+1))+(1+1))+(1+1))") == 8);
}
