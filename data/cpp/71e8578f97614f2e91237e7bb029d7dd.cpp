Given a string representing a prefix-notation arithmetic expression (e.g., "+ 3 * 4 5"), write a C++ function that evaluates and returns the result as a `double`. The expression consists of operators (`+`, `-`, `*`, `/`, `sin`, `cos`) and non-negative integer operands, all separated by single spaces. `+`, `-`, `*`, `/` are binary (two operands), while `sin` and `cos` are unary (one operand). The input is guaranteed to be syntactically valid. For division by zero, return `NaN` (use `std::numeric_limits<double>::quiet_NaN()`). The function should handle nested expressions recursively. For example: `"+ 3 * 4 5"` returns `23.0`; `"- 10 / 2 0"` returns `NaN`; `"sin 0"` returns `0.0`.
The expression is in prefix notation, meaning the operator appears before its operands. The natural approach is recursive descent or a simple recursive evaluator. Parse the input stream token by token: read a token; if it is an operator, recursively evaluate its required number of operands (2 for binary, 1 for unary), then apply the operation. If it is a number, convert to `double` and return it. For division, check if the divisor is zero; if so, return `NaN`. The recursion naturally handles nesting because each operand of an operator can itself be an operator with sub-expressions. Edge cases include unary operators (sin, cos) which take exactly one operand, and division by zero which must return `NaN` without crashing. Time complexity is `O(n)` where `n` is the number of tokens, since each token is processed once. Space complexity is `O(d)` where `d` is the maximum depth of nesting (due to recursion stack), which is at most `n`.
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>

// Recursive helper: evaluates tokens from a string stream.
double parseExpression(std::istringstream& input) {
    std::string token;
    input >> token;
    
    // If token is a number (starts with digit or negative sign)
    if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1 && std::isdigit(token[1]))) {
        return std::stod(token);
    }
    
    if (token == "sin") {
        double operand = parseExpression(input);
        return std::sin(operand);
    }
    if (token == "cos") {
        double operand = parseExpression(input);
        return std::cos(operand);
    }
    
    // Binary operators: evaluate two operands
    double left = parseExpression(input);
    double right = parseExpression(input);
    
    if (token == "+") return left + right;
    if (token == "-") return left - right;
    if (token == "*") return left * right;
    if (token == "/") {
        if (right == 0.0) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return left / right;
    }
    
    // Should not reach here for valid input
    return 0.0;
}

// Public function: evaluates a prefix expression string.
double evaluatePrefix(const std::string& expression) {
    std::istringstream input(expression);
    return parseExpression(input);
}
#include <cassert>
#include <cmath>
#include <string>

// Assume evaluatePrefix is declared above (or included here).

int main() {
    // Basic arithmetic
    assert(evaluatePrefix("+ 3 4") == 7.0);
    assert(evaluatePrefix("- 10 4") == 6.0);
    assert(evaluatePrefix("* 3 4") == 12.0);
    assert(evaluatePrefix("/ 10 2") == 5.0);
    
    // Nested expressions
    assert(evaluatePrefix("+ 3 * 4 5") == 23.0);
    assert(evaluatePrefix("+ * 2 3 - 10 4") == 12.0);
    assert(evaluatePrefix("- / 20 5 * 2 3") == -2.0);
    
    // Unary operators
    assert(std::abs(evaluatePrefix("sin 0") - 0.0) < 1e-9);
    assert(std::abs(evaluatePrefix("cos 0") - 1.0) < 1e-9);
    assert(std::abs(evaluatePrefix("sin * 0 5") - 0.0) < 1e-9);
    
    // Division by zero returns NaN
    double result = evaluatePrefix("/ 5 0");
    assert(std::isnan(result));
    
    // Non-linear nesting
    assert(evaluatePrefix("+ + + 1 1 1 1") == 4.0);
    
    // All operators together
    assert(std::abs(evaluatePrefix("+ sin 0 * / 8 2 3") - 12.0) < 1e-9);
}
