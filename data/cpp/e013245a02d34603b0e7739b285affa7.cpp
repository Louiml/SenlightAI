/*
Write a standalone C++ function that parses a fully parenthesized arithmetic expression string containing integer literals and the operators `+`, `-`, `*`, `/`, `>` (greater than), `<` (less than), `=` (equality), `&` (logical AND), `|` (logical OR), `!` (logical NOT), and ternary `?:` (written as `(left : right ? condition)` in this grammar). The grammar is: an expression is either a literal integer, a binary operation of the form `(expr op expr)`, a unary negation `(!expr)`, or a ternary `(expr : expr ? expr)`. The function should evaluate the expression and return an integer result: arithmetic operators return integers; comparison and logical operators return `1` (true) or `0` (false); the ternary operator evaluates `condition` — if nonzero, returns the value of the left operand, otherwise returns the value of the right operand. The input string is guaranteed to be syntactically valid, fully parenthesized, and contain no spaces. Write a free function `int evaluateExpression(const std::string& expr)` that parses and evaluates the expression recursively. You may use only the C++ standard library.
*/

#include <string>
#include <cctype>

// Evaluate a fully parenthesized expression string with integer literals
// and operators + - * / > < = & | ! and ternary ?: (written as (:?)).
int evaluateExpression(const std::string& expr) {
    std::size_t pos = 0;

    // Recursive helper that parses and evaluates an expression from the current position.
    auto parse = [&](auto&& self) -> int {
        // Skip any spaces (though input is guaranteed no spaces, be safe).
        while (pos < expr.size() && std::isspace(static_cast<unsigned char>(expr[pos]))) {
            ++pos;
        }

        // Literal (possibly negative integer).
        if (std::isdigit(static_cast<unsigned char>(expr[pos])) ||
            (expr[pos] == '-' && pos + 1 < expr.size() &&
             std::isdigit(static_cast<unsigned char>(expr[pos + 1])))) {
            bool negative = false;
            if (expr[pos] == '-') {
                negative = true;
                ++pos;
            }
            int value = 0;
            while (pos < expr.size() && std::isdigit(static_cast<unsigned char>(expr[pos]))) {
                value = value * 10 + (expr[pos] - '0');
                ++pos;
            }
            return negative ? -value : value;
        }

        // Otherwise, it must be a parenthesized expression.
        // Expect '('.
        ++pos;  // consume '('

        // Check for unary negation.
        if (pos < expr.size() && expr[pos] == '!') {
            ++pos;  // consume '!'
            int operand = self(self);
            ++pos;  // consume ')'
            return (operand == 0) ? 1 : 0;
        }

        // Parse left operand.
        int left = self(self);

        // Read operator.
        char op = expr[pos];
        ++pos;

        // Ternary operator: syntax is (left : right ? condition)
        if (op == ':') {
            int right = self(self);
            ++pos;  // consume '?'
            int condition = self(self);
            ++pos;  // consume ')'
            return condition != 0 ? left : right;
        }

        // Parse right operand for binary operator.
        int right = self(self);
        ++pos;  // consume ')'

        switch (op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            case '>': return (left > right) ? 1 : 0;
            case '<': return (left < right) ? 1 : 0;
            case '=': return (left == right) ? 1 : 0;
            case '&': return ((left != 0) && (right != 0)) ? 1 : 0;
            case '|': return ((left != 0) || (right != 0)) ? 1 : 0;
            default: return 0;  // unreachable for valid input
        }
    };

    return parse(parse);
}

#include <cassert>

int main() {
    // Arithmetic
    assert(evaluateExpression("(1+2)") == 3);
    assert(evaluateExpression("((10-3)*4)") == 28);
    assert(evaluateExpression("(20/5)") == 4);
    assert(evaluateExpression("(-5+2)") == -3);

    // Comparisons
    assert(evaluateExpression("(3>2)") == 1);
    assert(evaluateExpression("(2>3)") == 0);
    assert(evaluateExpression("(2<3)") == 1);
    assert(evaluateExpression("(3=3)") == 1);
    assert(evaluateExpression("(3=4)") == 0);

    // Logical
    assert(evaluateExpression("(1&1)") == 1);
    assert(evaluateExpression("(1&0)") == 0);
    assert(evaluateExpression("(0|1)") == 1);
    assert(evaluateExpression("(0|0)") == 0);

    // Negation
    assert(evaluateExpression("(!0)") == 1);
    assert(evaluateExpression("(!1)") == 0);
    assert(evaluateExpression("(!5)") == 0);

    // Ternary
    assert(evaluateExpression("(10:20?1)") == 10);
    assert(evaluateExpression("(10:20?0)") == 20);
    assert(evaluateExpression("((1+2):(3+4)?(5>3))") == 3);

    // Nested complex expression
    assert(evaluateExpression("((((1+2)*3):(4-1)?(2<1))|0)") == 1);

    return 0;
}

// The solution uses a recursive descent parser with an index pointer into the string. The main function `parse()` reads a token: if it is a digit (or a minus sign followed by a digit, for negative literals), it parses an integer literal and returns its value. If it is an opening parenthesis `(`, it parses the left subexpression, then reads the operator, then parses either a right operand or, for ternary, a right operand followed by `?` and a condition, then expects a closing `)`. For unary negation `!`, after reading `(`, it reads `!`, parses the operand, and returns the logical NOT of that operand (converting nonzero to 0, zero to 1). For binary operators, it applies the corresponding C++ operation on the evaluated left and right values. Comparison and logical operators return `1` or `0`. The ternary operator evaluates the condition (the third parsed expression) and chooses between left and right. Edge cases: negative literals are parsed by checking for a leading `-` followed by digits; division by zero is not tested (guaranteed valid input); logical AND/OR should use short-circuit? The grammar requires both operands to be evaluated (since they are parsed simultaneously), so use non-short-circuit bitwise-like semantics: `(left != 0) && (right != 0)` and `(left != 0) || (right != 0)` produce the correct integer result. The recursion depth equals the nesting depth of parentheses, which is at most proportional to input length. Time complexity is O(n) where n is the string length, because each character is processed exactly once. Space complexity is O(d) for the recursion stack, where d is the nesting depth, worst-case O(n).
