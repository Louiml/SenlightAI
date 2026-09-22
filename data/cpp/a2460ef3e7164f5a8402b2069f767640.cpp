Write a C++ function `int calculateExpression(const std::string& s)` that evaluates a mathematical expression given as a string containing non-negative integers and the operators `+`, `-`, `*`, `/` (integer division, truncated toward zero), with possible parentheses `(` and `)`. The expression is guaranteed to be valid (balanced parentheses, correct operator placement, no division by zero). Ignore any spaces in the input string. The function should return the integer result of evaluating the expression respecting standard operator precedence (`*` and `/` before `+` and `-`) and parentheses. The input length can be up to 10^5 characters, and all intermediate and final results fit within a 32-bit signed integer.
The solution uses two stacks: one for operands (integer values) and one for operators. We scan the string left to right, building multi-digit numbers when we see digits. When we encounter an operator or a closing parenthesis, we need to resolve any pending higher-or-equal-precedence operators on the operator stack before pushing the new operator. Specifically:
- If the current token is `(` , push it onto the operator stack.
- If it is `)`, keep popping operators and computing until we find the matching `(`, then pop the `(`.
- If it is an operator `+`, `-`, `*`, or `/`, check the top of the operator stack. While the top is also an operator and its precedence is greater than or equal to the current operator's precedence, we pop and compute. Then push the current operator.
- After the entire string is processed, pop and compute any remaining operators.

The `compute` function pops the top two operands and the top operator, performs the arithmetic (with the left operand being the earlier one), and pushes the result back. For division, C++ integer division truncates toward zero for positive operands (and for negative ones in C++11 onward, it truncates toward zero as well, but since inputs are non-negative, division is straightforward). Edge cases include numbers with multiple digits, nested parentheses, spaces anywhere, and expressions that reduce to a single number without any operators. Time complexity is O(n) because each character is processed once and each operator is pushed and popped at most once. Space complexity is O(n) in the worst case for the stacks (e.g., deeply nested parentheses or many unprocessed operators).
#include <string>
#include <cctype>
#include <stack>
#include <unordered_map>

// Evaluate a valid arithmetic expression with +, -, *, / and parentheses.
// Non-negative integers only, no division by zero. Ignores spaces.
int calculateExpression(const std::string& s) {
    static const std::unordered_map<char, int> precedence = {
        {'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}
    };

    std::stack<long long> operands;
    std::stack<char> operators;

    auto compute = [&operands, &operators]() {
        long long right = operands.top(); operands.pop();
        long long left = operands.top(); operands.pop();
        char op = operators.top(); operators.pop();
        if (op == '+') operands.push(left + right);
        else if (op == '-') operands.push(left - right);
        else if (op == '*') operands.push(left * right);
        else if (op == '/') operands.push(left / right);
    };

    long long currentNumber = 0;
    bool hasCurrentNumber = false;

    for (size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == ' ') {
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            currentNumber = currentNumber * 10 + (ch - '0');
            hasCurrentNumber = true;
        } else {
            if (hasCurrentNumber) {
                operands.push(currentNumber);
                currentNumber = 0;
                hasCurrentNumber = false;
            }
            if (ch == '(') {
                operators.push(ch);
            } else if (ch == ')') {
                while (operators.top() != '(') {
                    compute();
                }
                operators.pop();
            } else if (precedence.count(ch)) {
                while (!operators.empty() && precedence.count(operators.top()) &&
                       precedence.at(operators.top()) >= precedence.at(ch)) {
                    compute();
                }
                operators.push(ch);
            }
        }
    }
    if (hasCurrentNumber) {
        operands.push(currentNumber);
    }
    while (!operators.empty()) {
        compute();
    }
    return static_cast<int>(operands.top());
}
#include <cassert>

int main() {
    // Basic operations
    assert(calculateExpression("1+2") == 3);
    assert(calculateExpression("2-5") == -3);
    assert(calculateExpression("3*4") == 12);
    assert(calculateExpression("8/2") == 4);

    // Precedence
    assert(calculateExpression("1+2*3") == 7);
    assert(calculateExpression("10-2*3") == 4);
    assert(calculateExpression("2*3+4") == 10);
    assert(calculateExpression("8/2/2") == 2);
    assert(calculateExpression("5+2-3") == 4);

    // Parentheses
    assert(calculateExpression("(1+2)*3") == 9);
    assert(calculateExpression("2*(3+4)") == 14);
    assert(calculateExpression("((2+3)*4)") == 20);
    assert(calculateExpression("6/(2+1)") == 2);

    // Multi-digit numbers and spaces
    assert(calculateExpression(" 12 + 34 ") == 46);
    assert(calculateExpression(" 100 - 25 * 3 ") == 25);
    assert(calculateExpression(" 100 / 5 / 4 ") == 5);
    assert(calculateExpression("  ( 7 + 8 ) * 2 ") == 30);

    // Nested parentheses and mixed ops
    assert(calculateExpression("(1+(2*3))") == 7);
    assert(calculateExpression("((1+2)*(3+4))") == 21);
    assert(calculateExpression("2*(3+(4*5))") == 46);
    assert(calculateExpression("(1+2)+3*4") == 15);

    // Negative results (but numbers are non-negative)
    assert(calculateExpression("0-5") == -5);
    assert(calculateExpression("1-(2+3)") == -4);

    // Single number
    assert(calculateExpression("42") == 42);
    assert(calculateExpression("  0  ") == 0);

    // Division truncates toward zero
    assert(calculateExpression("7/3") == 2);
    assert(calculateExpression("7*2/3") == 4);

    // Complex chain with parenthesized division
    assert(calculateExpression("(100/10)*2+1") == 21);
    assert(calculateExpression("100/((10+10))") == 5);
}
