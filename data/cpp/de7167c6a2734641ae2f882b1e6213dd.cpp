// Write a C++ function named `evaluatePrefixExpression` that takes a string containing a valid prefix (Polish notation) arithmetic expression with non-negative integer operands and the binary operators `+`, `-`, `*`, `/` separated by single spaces. The function must return the integer result of evaluating the expression using a stack-based algorithm. The input string is guaranteed to be non-empty and well-formed (i.e., enough operands for each operator, no division by zero, and all tokens are either integers or one of the four operators). The function must handle multi-digit numbers, and it must not modify the input string. Return the final computed value. If the expression is malformed (e.g., insufficient operands when an operator is encountered, or invalid token), return `INT_MIN` to indicate an error. The function should use a custom minimal stack implementation (you may use `std::stack<int>` for simplicity) and must be self-contained with appropriate headers.

The standard algorithm for evaluating a prefix expression is to process tokens from right to left. Start by splitting the input string into tokens using a string stream or manual parsing, then reverse the token order, or equivalently iterate over the original token list backwards. Maintain a stack of integers. For each token processed from right to left: if it is an operand, push its integer value onto the stack; if it is an operator, pop two operands (the first popped is the left operand, the second is the right operand, because we are reversing the natural order), apply the operator, and push the result. If at any point popping fails (stack has fewer than two elements) or the token is invalid, return `INT_MIN`. At the end, the stack should contain exactly one element, which is the result; if stack size is not 1, return `INT_MIN`. Edge cases include single-operand expressions (e.g., `"42"`), operators with zero division (should not happen per problem statement, but guard against it by returning `INT_MIN` if denominator is zero), and negative results from subtraction. Time complexity is O(n) where n is the number of tokens, and space complexity is O(n) for the token vector and stack.

#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <climits>
#include <cstdlib>

// Evaluate a prefix expression with non-negative integer operands and +, -, *, /.
// Returns the integer result, or INT_MIN on malformed input or division by zero.
int evaluatePrefixExpression(const std::string& expression) {
    // Tokenize the input by splitting on spaces.
    std::vector<std::string> tokens;
    std::istringstream stream(expression);
    std::string token;
    while (stream >> token) {
        tokens.push_back(token);
    }

    std::stack<int> operands;

    // Process tokens from right to left.
    for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; --i) {
        const std::string& t = tokens[i];
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            if (operands.size() < 2) return INT_MIN;
            int left = operands.top(); operands.pop();
            int right = operands.top(); operands.pop();
            int result;
            if (t == "+") result = left + right;
            else if (t == "-") result = left - right;
            else if (t == "*") result = left * right;
            else { // division
                if (right == 0) return INT_MIN;
                result = left / right;
            }
            operands.push(result);
        } else {
            // Since operands are non-negative integers, try converting.
            char* end = nullptr;
            long val = std::strtol(t.c_str(), &end, 10);
            if (end == t.c_str() || *end != '\0' || val < 0 || val > INT_MAX) {
                return INT_MIN;
            }
            operands.push(static_cast<int>(val));
        }
    }

    if (operands.size() != 1) return INT_MIN;
    return operands.top();
}

#include <cassert>
#include <climits>

int main() {
    // Basic expressions
    assert(evaluatePrefixExpression("+ 2 3") == 5);
    assert(evaluatePrefixExpression("* 2 3") == 6);
    assert(evaluatePrefixExpression("- 5 2") == 3);
    assert(evaluatePrefixExpression("/ 6 2") == 3);

    // Multi-digit numbers and nested expressions
    assert(evaluatePrefixExpression("+ 12 34") == 46);
    assert(evaluatePrefixExpression("- + * 2 3 * 5 4 9") == -7); // (2*3 + 5*4) - 9 = 17
    assert(evaluatePrefixExpression("* - 10 4 + 2 3") == 30);     // (10-4) * (2+3) = 30

    // Single operand
    assert(evaluatePrefixExpression("42") == 42);
    assert(evaluatePrefixExpression("0") == 0);

    // Negative results and division
    assert(evaluatePrefixExpression("- 2 10") == -8);
    assert(evaluatePrefixExpression("/ 7 2") == 3); // integer division

    // Malformed input cases
    assert(evaluatePrefixExpression("+ 1") == INT_MIN);        // insufficient operands
    assert(evaluatePrefixExpression("1 2") == INT_MIN);        // extra operands
    assert(evaluatePrefixExpression("abc") == INT_MIN);        // invalid token
    assert(evaluatePrefixExpression("/ 1 0") == INT_MIN);      // division by zero

    return 0;
}
