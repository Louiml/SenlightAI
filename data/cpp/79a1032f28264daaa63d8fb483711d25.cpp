Write a C++ function `int evaluateRPN(const std::vector<std::string>& tokens)` that evaluates an expression written in Reverse Polish Notation (RPN) and returns the integer result. The input vector contains tokens that are either integers (possibly negative, e.g., "-23") or one of the binary operators `+`, `-`, `*`, `/`. Division between two integers must truncate toward zero (C++ integer division). The expression is guaranteed to be valid: no empty vector, enough operands for each operator, and exactly one final value. Handle multi-digit numbers, negative numbers, and all operators correctly.

The standard approach uses a stack of integers. Iterate over tokens in order. If the token represents a number (i.e., its first character is a digit or it has length greater than 1 and starts with a minus sign), convert it with `std::stoi` and push onto the stack. Otherwise, the token is an operator: pop the top two values (the first popped is the right operand `b`, the second popped is the left operand `a`), apply the operation, and push the result. At the end, the stack contains exactly one value, which is the answer. Edge cases include negative numbers (checked by `c.size() > 1 || isdigit(c[0])`), division by zero is not possible per problem constraints, and integer overflow is not tested. Time complexity is O(N) where N is the number of tokens, and space complexity is O(N) for the stack in the worst case (all numbers before operators).

#include <vector>
#include <string>
#include <stack>
#include <cctype>
#include <stdexcept>

// Evaluate an expression in Reverse Polish Notation.
// Tokens are integers (possibly negative) or operators +, -, *, /.
// Division truncates toward zero. Assumes the input is valid.
int evaluateRPN(const std::vector<std::string>& tokens) {
    std::stack<int> operands;
    
    for (const std::string& token : tokens) {
        // A token is a number if it's longer than 1 (e.g., "-12", "34")
        // or if its first character is a digit (e.g., "0", "7").
        if (token.size() > 1 || std::isdigit(token[0])) {
            operands.push(std::stoi(token));
        } else {
            // Token is an operator; pop two operands.
            int b = operands.top();
            operands.pop();
            int a = operands.top();
            operands.pop();
            
            switch (token[0]) {
                case '+':
                    operands.push(a + b);
                    break;
                case '-':
                    operands.push(a - b);
                    break;
                case '*':
                    operands.push(a * b);
                    break;
                case '/':
                    operands.push(a / b); // Truncates toward zero
                    break;
                default:
                    // Should never happen for valid input.
                    throw std::invalid_argument("Invalid operator");
            }
        }
    }
    
    return operands.top();
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Simple arithmetic
    assert(evaluateRPN({"2", "1", "+", "3", "*"}) == 9);          // (2+1)*3 = 9
    assert(evaluateRPN({"4", "13", "5", "/", "+"}) == 6);         // 4 + (13/5) = 4 + 2 = 6
    assert(evaluateRPN({"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"}) == 22);
    
    // Negative numbers and subtraction
    assert(evaluateRPN({"-2", "3", "-"}) == -5);                  // -2 - 3 = -5
    assert(evaluateRPN({"5", "-7", "*"}) == -35);                 // 5 * -7 = -35
    
    // Division truncation toward zero
    assert(evaluateRPN({"-7", "2", "/"}) == -3);                  // -7/2 = -3 (not -4)
    assert(evaluateRPN({"7", "-2", "/"}) == -3);                  // 7 / -2 = -3
    
    // Single number
    assert(evaluateRPN({"42"}) == 42);
    
    // More complex nested expression
    assert(evaluateRPN({"2", "3", "11", "+", "*", "5", "-"}) == 23); // 2*(3+11)-5 = 23
    
    return 0;
}
