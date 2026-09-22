// Write a C++ function `int evaluateExpression(const std::string& expr)` that parses and evaluates an arithmetic expression consisting only of single-digit numbers (0–9) and the four binary operators `+`, `-`, `*`, `/` (with standard precedence: multiplication and division before addition and subtraction, left-to-right associativity for equal precedence). The input string will contain no spaces, no parentheses, and no leading/trailing whitespace. Division is integer division (truncating toward zero). The function should return the integer result. If the expression is invalid (e.g., contains a non-digit, non-operator character, starts with an operator, ends with an operator, contains two operators in a row, or attempts division by zero), the function should throw a `std::invalid_argument` exception with a descriptive message. The function must handle single-digit operands only and must not rely on any global state. Your solution must implement a parsing algorithm, not use `std::eval` or similar facilities, and must be self-contained within the function (you may define helper classes/functions inside the same file).

// The task requires evaluating an infix expression with standard operator precedence and left-to-right associativity. Since operands are single digits and the expression length is small, we can use two stacks: one for operands (integers) and one for operators (characters). A common approach is the shunting-yard algorithm, but here we can solve it directly with two passes: first, handle all multiplication and division (left-to-right), then handle addition and subtraction (left-to-right). However, that is tricky because we need to preserve the order of operations across mixed precedence. A cleaner method is the standard two-stack evaluator:
// - Scan the string left to right.
// - If the current character is a digit, push its integer value onto the operand stack.
// - If it's an operator, while the operator stack is not empty and the top operator has precedence >= current operator, pop the top operator, pop two operands (second operand is top, first is next), apply the operation, and push the result. Then push the current operator.
// - After the scan, while the operator stack is not empty, pop and apply.
// - At the end, the operand stack should have exactly one element, which is the result.
// Edge cases: 
// - Input length must be odd (alternating digit/operator/digit/...).
// - First and last characters must be digits.
// - No two operators adjacent.
// - Division by zero must be detected and cause an exception.
// - The operator `-` and `/` require operands in correct order: for `a - b`, we pop `b` (top) then `a` (next), compute `a - b`. For `/`, compute `a / b`.
// - Precedence: `*` and `/` have higher precedence (2) than `+` and `-` (1). When an operator with lower or equal precedence arrives, pop higher/equal precedence operators first.
// Time complexity: O(n) where n is the string length. Space complexity: O(n) in the worst case for the stacks, but practically O(depth) which is limited.

#include <string>
#include <stack>
#include <stdexcept>
#include <cctype>

// Evaluates an arithmetic expression with single-digit operands and + - * / operators.
// Throws std::invalid_argument on invalid input or division by zero.
int evaluateExpression(const std::string& expr) {
    if (expr.empty()) {
        throw std::invalid_argument("Empty expression");
    }

    // Validate basic structure: odd length, starts and ends with a digit,
    // alternates digit/operator/digit, and only allowed characters.
    if (expr.length() % 2 == 0) {
        throw std::invalid_argument("Expression must alternate digit/operator/digit");
    }
    if (!std::isdigit(static_cast<unsigned char>(expr[0])) ||
        !std::isdigit(static_cast<unsigned char>(expr[expr.length() - 1]))) {
        throw std::invalid_argument("Expression must start and end with a digit");
    }
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (i % 2 == 0) {
            if (!std::isdigit(static_cast<unsigned char>(ch))) {
                throw std::invalid_argument("Expected a digit at position " + std::to_string(i));
            }
        } else {
            if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
                throw std::invalid_argument("Invalid operator at position " + std::to_string(i));
            }
        }
    }

    // Define precedence: higher value = higher precedence
    auto precedence = [](char op) -> int {
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    };

    // Apply an operation given operator and two operands (a op b)
    auto applyOp = [](int a, int b, char op) -> int {
        if (op == '+') return a + b;
        if (op == '-') return a - b;
        if (op == '*') return a * b;
        if (op == '/') {
            if (b == 0) throw std::invalid_argument("Division by zero");
            return a / b;
        }
        throw std::invalid_argument("Unknown operator");
    };

    std::stack<int> operands;
    std::stack<char> operators;

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            operands.push(ch - '0');
        } else {
            // Process higher or equal precedence operators already on stack
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                char op = operators.top();
                operators.pop();
                int right = operands.top(); operands.pop();
                int left = operands.top(); operands.pop();
                operands.push(applyOp(left, right, op));
            }
            operators.push(ch);
        }
    }

    // Pop remaining operators
    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        int right = operands.top(); operands.pop();
        int left = operands.top(); operands.pop();
        operands.push(applyOp(left, right, op));
    }

    if (operands.size() != 1) {
        throw std::invalid_argument("Invalid expression structure");
    }
    return operands.top();
}

#include <cassert>
#include <string>
#include <stdexcept>

// Declaration of the function being tested
int evaluateExpression(const std::string& expr);

int main() {
    // Basic valid expressions
    assert(evaluateExpression("2+3") == 5);
    assert(evaluateExpression("2+3*4") == 14); // 2+12=14
    assert(evaluateExpression("2*3+4") == 10); // 6+4=10
    assert(evaluateExpression("2+3*4/3-2") == 4); // 2+12/3-2 = 2+4-2=4
    assert(evaluateExpression("5/2") == 2); // integer division
    assert(evaluateExpression("9-3-2") == 4); // left associativity: (9-3)-2=4
    assert(evaluateExpression("8/2/2") == 2); // (8/2)/2=2
    assert(evaluateExpression("1*2*3/6") == 1); // (1*2*3)/6 = 6/6=1
    assert(evaluateExpression("0*5+7") == 7);

    // Single digit
    assert(evaluateExpression("7") == 7);

    // Expressions with multiple operations including precedence
    assert(evaluateExpression("2+2+2+2") == 8);
    assert(evaluateExpression("10/2+3*2") == 11); // 5+6=11

    // Invalid inputs should throw
    bool caught = false;
    try { evaluateExpression(""); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("12+3"); } catch (const std::invalid_argument&) { caught = true; } // two-digit number
    assert(caught);

    caught = false;
    try { evaluateExpression("1+"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("+1+2"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("1++2"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("1/0"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("1+2a"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    caught = false;
    try { evaluateExpression("1+2*"); } catch (const std::invalid_argument&) { caught = true; }
    assert(caught);

    return 0;
}
