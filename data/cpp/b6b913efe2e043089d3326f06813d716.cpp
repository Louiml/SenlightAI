Given an arithmetic expression string that may contain non-negative integers, the operators `+`, `-`, `*`, `/`, parentheses `(` and `)`, and an optional unary leading minus (e.g., `-5+3` or `(-5+2)*3`), write a C++ function `std::string evaluateExpressionToPostfixAndValue(const std::string& infix)` that returns a single string containing the postfix (reverse Polish) equivalent followed by a space and then the evaluated numeric result (as a string representation of a floating-point number). The input is guaranteed to be a valid infix expression with no decimal points, no whitespace, and no division by zero. The output postfix must have tokens separated by single spaces, with each number appearing as its original digit sequence (no leading zeros, except the single digit `0`) and the unary minus attached to the leading number (e.g., input `-5+3` → postfix `-5 3 +` and result `-2`). For parenthesized unary minus like `(-5+2)`, treat it as a negative number token `-5` in postfix. The result value must be printed with default formatting (e.g., `-2`, `15`, `2.5`).

The solution mirrors the provided snippet but improves robustness and clarity. First, convert the infix expression to postfix using a stack of operators, respecting precedence (`+`/`-` = 1, `*`/`/` = 2) and parentheses. Handle a special case: if the expression starts with an operator or a `(` followed by `-`, we treat that leading minus as part of the first operand (a negative number). In the conversion loop: digits are accumulated into multi-digit numbers; when a non-digit or end-of-string follows, append the number and a space. Parentheses are pushed onto the stack; on `)`, pop until `(`. For operators, pop while the stack top has precedence ≥ current operator, append the popped operator and a space, then push the current operator. After the loop, pop any remaining operators. For unary negative at the start, we directly output the minus sign as part of the number token by copying it into the postfix and adjusting the loop index (as in the snippet). Then evaluate the postfix: use a stack for numbers. For each token: if it’s a number (including negative), parse it (accumulate digits, handle leading minus) and push; if it’s an operator, pop two values, apply the operation, and push the result. The final stack top is the result. Edge cases: single number, unary minus alone (e.g., `-7`), parentheses around a negative number, multi-digit numbers, and operations with fractional results (e.g., `1/2` → `0.5`). Time complexity is O(n) for both conversion and evaluation, and space complexity is O(n) due to the stacks and output string.

#include <string>
#include <stack>
#include <cctype>

// Helper: check if a character is a digit.
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Helper: check if a character is an operator.
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Helper: precedence of an operator (higher value = higher precedence).
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Helper: perform an arithmetic operation.
float applyOperation(float a, float b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0.0f;
    }
}

// Convert an infix expression to postfix and evaluate it.
// Returns "postfix result" as a single string.
std::string evaluateExpressionToPostfixAndValue(const std::string& infix) {
    // --- Part 1: Infix to Postfix ---
    std::stack<char> ops;
    std::string postfix;
    int i = 0;

    // Handle leading unary minus (either at start or after an opening parenthesis).
    if (isOperator(infix[0]) && infix[0] == '-') {
        postfix += '-';
        i = 1;
    } else if (infix[0] == '(' && infix[1] == '-') {
        postfix += '-';
        i = 2; // Skip "(-" and process the number after the minus
    }

    for (; i < (int)infix.size(); ++i) {
        char ch = infix[i];

        if (isDigit(ch)) {
            // Accumulate multi-digit numbers.
            postfix += ch;
            // If next char is not a digit, add a space separator.
            if (i + 1 == (int)infix.size() || !isDigit(infix[i + 1])) {
                postfix += ' ';
            }
        } else if (ch == '(') {
            ops.push(ch);
        } else if (ch == ')') {
            // Pop until matching '('.
            while (!ops.empty() && ops.top() != '(') {
                postfix += ops.top();
                postfix += ' ';
                ops.pop();
            }
            if (!ops.empty()) ops.pop(); // Remove '('
        } else if (isOperator(ch)) {
            // Pop operators with higher or equal precedence.
            while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                postfix += ops.top();
                postfix += ' ';
                ops.pop();
            }
            ops.push(ch);
        }
    }

    // Pop remaining operators.
    while (!ops.empty()) {
        postfix += ops.top();
        postfix += ' ';
        ops.pop();
    }

    // Remove trailing space (optional, for clean output).
    if (!postfix.empty() && postfix.back() == ' ') {
        postfix.pop_back();
    }

    // --- Part 2: Evaluate Postfix ---
    std::stack<float> values;
    int j = 0;
    while (j < (int)postfix.size()) {
        char c = postfix[j];

        if (c == ' ') {
            ++j;
            continue;
        }

        if (isDigit(c) || (c == '-' && j + 1 < (int)postfix.size() && isDigit(postfix[j + 1]))) {
            // Parse a (possibly negative) number.
            bool negative = (c == '-');
            if (negative) ++j;
            float num = 0.0f;
            while (j < (int)postfix.size() && isDigit(postfix[j])) {
                num = num * 10.0f + (postfix[j] - '0');
                ++j;
            }
            if (negative) num = -num;
            values.push(num);
        } else if (isOperator(c)) {
            // Pop two operands and apply the operator.
            float b = values.top(); values.pop();
            float a = values.top(); values.pop();
            values.push(applyOperation(a, b, c));
            ++j;
        } else {
            ++j; // skip any unexpected character
        }
    }

    float result = values.top();
    // Convert result to string without extra formatting issues.
    std::string resultStr = std::to_string(result);
    // Remove trailing ".000000" if it's an integer.
    if (resultStr.find('.') != std::string::npos) {
        while (resultStr.back() == '0') resultStr.pop_back();
        if (resultStr.back() == '.') resultStr.pop_back();
    }

    return postfix + " " + resultStr;
}

#include <cassert>
#include <string>

// The solution function is declared here (or include the header).
std::string evaluateExpressionToPostfixAndValue(const std::string& infix);

int main() {
    // Basic arithmetic and precedence.
    assert(evaluateExpressionToPostfixAndValue("1+2") == "1 2 + 3");
    assert(evaluateExpressionToPostfixAndValue("2*3+4") == "2 3 * 4 + 10");
    assert(evaluateExpressionToPostfixAndValue("2+3*4") == "2 3 4 * + 14");
    assert(evaluateExpressionToPostfixAndValue("10/2") == "10 2 / 5");

    // Parentheses.
    assert(evaluateExpressionToPostfixAndValue("(1+2)*3") == "1 2 + 3 * 9");
    assert(evaluateExpressionToPostfixAndValue("(2+3)*(4-1)") == "2 3 + 4 1 - * 15");

    // Unary leading minus.
    assert(evaluateExpressionToPostfixAndValue("-5+3") == "-5 3 + -2");
    assert(evaluateExpressionToPostfixAndValue("(-5+2)*3") == "-5 2 + 3 * -9");
    assert(evaluateExpressionToPostfixAndValue("-7") == "-7 -7");

    // Multi-digit and fractional results.
    assert(evaluateExpressionToPostfixAndValue("12+34") == "12 34 + 46");
    assert(evaluateExpressionToPostfixAndValue("1/2") == "1 2 / 0.5");
    assert(evaluateExpressionToPostfixAndValue("5/2+1") == "5 2 / 1 + 3.5");

    // Nested parentheses and combined precedence.
    assert(evaluateExpressionToPostfixAndValue("((2+3)*4)/5") == "2 3 + 4 * 5 / 4");
    assert(evaluateExpressionToPostfixAndValue("(2-3)*(4+5)") == "2 3 - 4 5 + * -9");

    return 0;
}
