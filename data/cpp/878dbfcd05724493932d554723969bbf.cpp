// Write a C++ function named `double evaluateInfixExpression(const std::string& expression)` that takes a valid infix arithmetic expression containing single-digit operands from 1 to 9 and the operators `+`, `-`, `*`, `/` along with parentheses `(` and `)`, and returns the result as a `double`. The function must internally convert the infix expression to postfix notation and then evaluate that postfix expression, mimicking the behavior of the provided snippet but as a single callable function. The expression is guaranteed to be syntactically valid and will not contain spaces or other characters. Division is real division (e.g., `7/2` returns `3.5`), and subtraction is left-associative. The result should be accurate to at least 1e-9. Do not include a `main` function in the solution; only provide the function definition and any necessary helper functions.

The solution follows the standard shunting-yard algorithm for infix-to-postfix conversion and then a stack-based evaluation of the postfix expression. For conversion: iterate over each character of the input string. If the character is an operand (digit 1-9), append it to the postfix string. If it is an opening parenthesis, push it onto a stack. If it is a closing parenthesis, pop operators from the stack and append them to postfix until an opening parenthesis is encountered, then pop and discard the opening parenthesis. If it is an operator, pop operators from the stack that have precedence greater than or equal to the current operator (and are not an opening parenthesis), append them to postfix, then push the current operator. After processing all characters, pop any remaining operators from the stack and append them. For evaluation: use a stack of doubles. For each character in the postfix string, if it is a digit, push its numeric value (digit - '0'). If it is an operator, pop the top two values (val1 then val2), apply the operator (with val2 being the left operand and val1 the right operand), and push the result. At the end, the stack contains one value, which is the result. Edge cases include single-operand expressions, nested parentheses, and operator precedence. The provided snippet has a bug in evaluating division: it only stores the remainder incorrectly and does not handle the result properly; our implementation avoids that by computing the division directly as a double. Time complexity is O(n) where n is the length of the expression, and space complexity is O(n) for the stacks and postfix string.

#include <string>
#include <stack>

// Helper: returns precedence of an operator; higher means higher precedence.
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Helper: convert infix expression (single-digit operands) to postfix.
std::string infixToPostfix(const std::string& infix) {
    std::string postfix;
    std::stack<char> ops;
    for (char ch : infix) {
        if (ch >= '1' && ch <= '9') {
            postfix += ch;
        } else if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            while (!ops.empty() && ops.top() != '(') {
                postfix += ops.top();
                ops.pop();
            }
            if (!ops.empty()) ops.pop(); // discard '('
        } else { // operator
            while (!ops.empty() && ops.top() != '(' && precedence(ch) <= precedence(ops.top())) {
                postfix += ops.top();
                ops.pop();
            }
            ops.push(ch);
        }
    }
    while (!ops.empty()) {
        postfix += ops.top();
        ops.pop();
    }
    return postfix;
}

// Evaluates a valid infix expression with single-digit operands 1-9 and + - * / and parentheses.
double evaluateInfixExpression(const std::string& expression) {
    std::string postfix = infixToPostfix(expression);
    std::stack<double> values;
    for (char ch : postfix) {
        if (ch >= '1' && ch <= '9') {
            values.push(static_cast<double>(ch - '0'));
        } else {
            double right = values.top(); values.pop();
            double left = values.top(); values.pop();
            switch (ch) {
                case '+': values.push(left + right); break;
                case '-': values.push(left - right); break;
                case '*': values.push(left * right); break;
                case '/': values.push(left / right); break;
            }
        }
    }
    return values.top();
}

#include <cassert>
#include <cmath>

// The solution function is assumed to be defined above.
// This file provides a main() that tests the function with assert.
int main() {
    // Basic operations
    assert(evaluateInfixExpression("1+2") == 3.0);
    assert(evaluateInfixExpression("9-5") == 4.0);
    assert(evaluateInfixExpression("3*4") == 12.0);
    assert(evaluateInfixExpression("8/2") == 4.0);
    // Precedence and parentheses
    assert(evaluateInfixExpression("1+2*3") == 7.0);
    assert(evaluateInfixExpression("(1+2)*3") == 9.0);
    assert(evaluateInfixExpression("2*(3+4)-5") == 9.0);
    // Division yields double
    assert(std::abs(evaluateInfixExpression("7/2") - 3.5) < 1e-9);
    assert(std::abs(evaluateInfixExpression("(5+5)/(2+2)") - 2.5) < 1e-9);
    // Nested parentheses and left-associativity
    assert(evaluateInfixExpression("((1+2)*(3+4))") == 21.0);
    assert(evaluateInfixExpression("9-3-2") == 4.0); // left-associative: (9-3)-2
    // Single operand
    assert(evaluateInfixExpression("5") == 5.0);
    return 0;
}
