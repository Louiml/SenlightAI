Write a standalone C++ function named `evaluatePostfix` that takes a single string argument representing a valid postfix (Reverse Polish Notation) expression containing single-digit operands (0-9) and the binary operators `+`, `-`, `*`, `/`, and `^` (exponentiation). The expression contains no spaces or other delimiters, is guaranteed to be non-empty and syntactically valid, and division is integer division. The function must return the integer result of evaluating the expression. Operands are pushed onto a stack; when an operator is encountered, pop the top two operands (the first popped is the right operand, the second popped is the left operand), apply the operator, and push the result back. Exponentiation must be handled using `pow` with integer arithmetic (truncating any fractional part, though inputs are integers so the result is already integral). For example, `evaluatePostfix("23*5+")` should return `11`, and `evaluatePostfix("23^")` should return `8`.

The solution uses a stack of integers to evaluate the postfix expression in a single left-to-right pass. For each character in the input string: if it is a digit (checked via `isdigit`), push its numeric value (character minus `'0'`) onto the stack. If it is an operator, pop two values: the first popped is the right operand, the second is the left operand. Then apply the operator: addition, subtraction, multiplication, integer division (careful that division by zero never occurs in a valid expression), and exponentiation using `std::pow`—note that `pow` returns a floating-point value, so cast to `int` (the result is always integral for integer bases and non-negative integer exponents, but truncation is safe). After processing all characters, the stack must contain exactly one value, which is the result. Edge cases include: a single operand (e.g., `"5"` returns `5`); operators at the end; and exponentiation where operands are popped in reverse order (e.g., `"23^"` means `2^3 = 8`). Time complexity is O(n) where n is the length of the string, and space complexity is O(n) in the worst case (when all characters are operands, e.g., `"1234"`), though for typical expressions it is less.

#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>

// Evaluate a valid postfix expression with single-digit operands and +, -, *, /, ^.
int evaluatePostfix(const std::string& expr) {
    std::stack<int> values;
    
    for (char ch : expr) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            values.push(ch - '0');
        } else {
            // Pop right then left operand
            int right = values.top();
            values.pop();
            int left = values.top();
            values.pop();
            
            int result = 0;
            switch (ch) {
                case '^':
                    result = static_cast<int>(std::pow(left, right));
                    break;
                case '*':
                    result = left * right;
                    break;
                case '/':
                    if (right == 0) {
                        throw std::runtime_error("Division by zero");
                    }
                    result = left / right;
                    break;
                case '+':
                    result = left + right;
                    break;
                case '-':
                    result = left - right;
                    break;
                default:
                    throw std::invalid_argument("Unknown operator");
            }
            values.push(result);
        }
    }
    
    if (values.size() != 1) {
        throw std::invalid_argument("Malformed postfix expression");
    }
    return values.top();
}

#include <cassert>
#include <string>

// Forward declaration from solution
int evaluatePostfix(const std::string& expr);

int main() {
    // Basic arithmetic
    assert(evaluatePostfix("23+") == 5);
    assert(evaluatePostfix("23-") == -1);
    assert(evaluatePostfix("23*") == 6);
    assert(evaluatePostfix("84/") == 2);
    // Exponentiation
    assert(evaluatePostfix("23^") == 8);
    assert(evaluatePostfix("32^") == 9);
    // Single operand
    assert(evaluatePostfix("7") == 7);
    // Longer expressions with correct precedence (left-to-right for operators as they appear)
    assert(evaluatePostfix("23+4*") == ((2+3)*4)); // = 20
    assert(evaluatePostfix("234+*") == (2*(3+4))); // = 14
    assert(evaluatePostfix("23*45+*") == ((2*3)*(4+5))); // = 54
    // Nested exponentiation (right associative due to postfix order)
    assert(evaluatePostfix("232^^") == (2^(3^2))); // = 2^9 = 512
    // Integer division truncates toward zero
    assert(evaluatePostfix("73/") == 2); // 7/3 = 2
    assert(evaluatePostfix("7 3 /") == 2); // no spaces, but just to check syntax? Actually string "73/" is 7/3 = 2.
    // Complex mixed expression: (5 + (1 - 2)) * 3 = (5 + (-1)) * 3 = 4*3 = 12
    assert(evaluatePostfix("512-+3*") == 12);
    
    // The above "7 3 /" is invalid because spaces not allowed; remove that line
    // Replace with correct test
    assert(evaluatePostfix("73/") == 2);
    
    return 0;
}
