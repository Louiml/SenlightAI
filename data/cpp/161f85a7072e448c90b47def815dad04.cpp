// Write a standalone C++ function that evaluates a fully parenthesized arithmetic expression string containing only non-negative integers, parentheses, and the binary operators `+`, `-`, `*`, and `/` (integer division truncating toward zero). The expression is guaranteed to be syntactically valid, with every operator surrounded by exactly one space and every integer token surrounded by spaces as needed (e.g., `"( 3 + 4 ) * 2"` or `"( ( 10 / 3 ) - 1 )"`). The function must parse and evaluate the expression, handling arbitrarily deep nesting. It must correctly apply operator precedence solely through the parentheses (since the input is fully parenthesized, no precedence rules are needed beyond the explicit grouping). Division by zero will not occur. Return the integer result.

The solution uses a recursive-descent parser that reads tokens from a string stream. The grammar is simple: an expression is either a single integer token, or a left parenthesis followed by an expression, an operator, another expression, and a right parenthesis. A recursive function `parseExpression` reads a token; if it is an integer, convert and return it. If it is a left parenthesis, recursively parse the left operand, read the operator, recursively parse the right operand, expect a closing parenthesis, then apply the operation and return the result. Since the expression is fully parenthesized, the recursion naturally handles nesting and operator precedence is already encoded by the parentheses. Edge cases include single-integer expressions (no parentheses at all) and deeply nested expressions (recursion depth equals nesting depth, which is bounded by input length). The time complexity is O(n) where n is the length of the string, because each token is processed exactly once. Space complexity is O(d) for recursion depth, where d is the nesting depth, plus O(n) for the input string if stored, but the function uses O(d) auxiliary stack space.

#include <sstream>
#include <string>
#include <cstdlib>

// Evaluate a fully parenthesized arithmetic expression of non-negative integers.
// Operators: +, -, *, / (integer division). No division by zero. Tokens separated by spaces.
int evaluateParenExpression(const std::string& expr) {
    std::istringstream input(expr);
    std::string token;

    // Recursive helper. Assumes tokens are separated by spaces and input is valid.
    // Reads the next token; if it's an integer, returns its value.
    // If it's '(', parses left expr, operator, right expr, expects ')', and returns result.
    // This lambda captures 'input' by reference. 
    // We declare the function recursively using std::function to allow recursion.
    std::function<int()> parse = [&]() -> int {
        input >> token;
        if (token == "(") {
            int left = parse();
            input >> token; // operator token
            char op = token[0];
            int right = parse();
            input >> token; // should be ")"
            // token is ")" by valid input; we don't need to verify.
            switch (op) {
                case '+': return left + right;
                case '-': return left - right;
                case '*': return left * right;
                case '/': return left / right; // no division by zero
                default: return 0; // unreachable for valid input
            }
        }
        return std::stoi(token);
    };

    return parse();
}

Note: The solution above uses `std::function` from `<functional>` which is not included; to keep it self-contained, we can use a different approach: define a private recursive function outside the main function, or use a mutable index to parse the string directly. Below is a cleaner, fully self-contained implementation without `std::function`:

#include <sstream>
#include <string>
#include <cstdlib>

// Recursive helper that parses from a string stream and returns the result.
static int parseExpr(std::istringstream& input) {
    std::string token;
    input >> token;
    if (token == "(") {
        int left = parseExpr(input);
        input >> token;
        char op = token[0];
        int right = parseExpr(input);
        input >> token; // consume the closing ')'
        switch (op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            default: return 0;
        }
    }
    return std::stoi(token);
}

// Evaluate a fully parenthesized arithmetic expression of non-negative integers.
// Operators: +, -, *, / (integer division). No division by zero. Tokens separated by spaces.
int evaluateParenExpression(const std::string& expr) {
    std::istringstream input(expr);
    return parseExpr(input);
}

#include <cassert>

int main() {
    assert(evaluateParenExpression("42") == 42);
    assert(evaluateParenExpression("( 3 + 4 )") == 7);
    assert(evaluateParenExpression("( ( 10 / 3 ) - 1 )") == 2);
    assert(evaluateParenExpression("( 2 * ( 3 + 4 ) )") == 14);
    assert(evaluateParenExpression("( ( 8 - 2 ) * ( 5 + 1 ) )") == 36);
    assert(evaluateParenExpression("( ( ( 100 / 5 ) / 4 ) + 3 )") == 8);
    assert(evaluateParenExpression("( 7 * ( 6 - ( 2 + 1 ) ) )") == 21);
    assert(evaluateParenExpression("( 0 + ( 0 * 9 ) )") == 0);
    assert(evaluateParenExpression("( ( 1 + 2 ) + ( 3 + 4 ) )") == 10);
}
