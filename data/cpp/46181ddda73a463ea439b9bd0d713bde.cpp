Write a C++ function `std::string postfixToInfix(const std::string& expr)` that converts a valid postfix expression into infix notation with full parentheses. The expression may contain floating-point numbers, binary operators `+`, `-`, `*`, `/`, and unary functions `sin`, `cos`, `exp`, `log`, and nullary constants `pi`, `e`. Tokens are separated by single spaces. The function must return the fully parenthesized infix form (e.g., `"3 4 +"` → `"(3+4)"`, `"5 sin"` → `"sin(5)"`, `"pi"` → `"pi"`). You may assume the input is always valid postfix. Implement without external parsing libraries; use standard C++17.

// The algorithm processes tokens left-to-right using a stack of strings. For each whitespace-separated token:
// - If it is a recognized nullary constant (`pi`, `e`) or a numeric literal (e.g., `3.14`, `-2.5`), push it directly onto the stack.
// - If it is a recognized unary function (`sin`, `cos`, `exp`, `log`), pop one operand from the stack, wrap it as `func(operand)`, and push back.
// - If it is a binary operator (`+`, `-`, `*`, `/`), pop two operands (second popped is the left operand), wrap them as `(left op right)`, and push back.
// - Any unknown token: return empty string or handle gracefully (in this task we assume valid input, so we can ignore or throw).
//
// Edge cases: single-element input (constant or number) returns as-is; nested functions and operators build naturally; negative numbers are handled because tokenization by space keeps them intact; floating-point numbers are recognized by checking if the token starts with a digit, a sign followed by a digit, or contains a decimal point. Time complexity is O(n * L) where n is number of tokens and L is the average length of accumulated strings (since string concatenation copies intermediate results); for simplicity treat as O(n^2) in worst case due to repeated copying. Space complexity is O(n * L) for the stack.

#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <algorithm>

// Check whether a token is a numeric literal (including decimals and leading sign)
bool isNumeric(const std::string& tok) {
    if (tok.empty()) return false;
    size_t i = 0;
    if (tok[i] == '+' || tok[i] == '-') i++;
    bool hasDigit = false;
    bool hasDecimal = false;
    for (; i < tok.size(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(tok[i]))) hasDigit = true;
        else if (tok[i] == '.' && !hasDecimal) hasDecimal = true;
        else return false;
    }
    return hasDigit;
}

// Main conversion function: postfix to fully parenthesized infix
std::string postfixToInfix(const std::string& expr) {
    std::istringstream iss(expr);
    std::string token;
    std::vector<std::string> stack;

    while (iss >> token) {
        // Nullary constants
        if (token == "pi" || token == "e") {
            stack.push_back(token);
        }
        // Numeric literals
        else if (isNumeric(token)) {
            stack.push_back(token);
        }
        // Unary functions
        else if (token == "sin" || token == "cos" || token == "exp" || token == "log") {
            if (stack.empty()) return ""; // invalid
            std::string operand = stack.back();
            stack.pop_back();
            stack.push_back(token + "(" + operand + ")");
        }
        // Binary operators
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2) return ""; // invalid
            std::string right = stack.back(); stack.pop_back();
            std::string left  = stack.back(); stack.pop_back();
            stack.push_back("(" + left + token + right + ")");
        }
        // Unknown token -> return empty to signal error
        else {
            return "";
        }
    }

    // Valid postfix should leave exactly one item on the stack
    return stack.size() == 1 ? stack.back() : "";
}

#include <cassert>
#include <string>

// Function declaration from solution (assumed included above)

int main() {
    // Basic binary operations
    assert(postfixToInfix("3 4 +") == "(3+4)");
    assert(postfixToInfix("5 2 -") == "(5-2)");
    assert(postfixToInfix("6 7 *") == "(6*7)");
    assert(postfixToInfix("8 2 /") == "(8/2)");
    
    // Nested binary
    assert(postfixToInfix("3 4 + 5 *") == "((3+4)*5)");
    assert(postfixToInfix("3 4 5 + *") == "(3*(4+5))");
    
    // Unary functions
    assert(postfixToInfix("5 sin") == "sin(5)");
    assert(postfixToInfix("3 4 + cos") == "cos((3+4))");
    assert(postfixToInfix("2 exp") == "exp(2)");
    assert(postfixToInfix("10 log") == "log(10)");
    
    // Constants and decimals
    assert(postfixToInfix("pi") == "pi");
    assert(postfixToInfix("e") == "e");
    assert(postfixToInfix("3.14 2.5 +") == "(3.14+2.5)");
    
    // Negative numbers (space-separated)
    assert(postfixToInfix("-5 3 +") == "(-5+3)");
    
    // Complex combination
    assert(postfixToInfix("5 pi * sin") == "sin((5*pi))");
    
    // Multiple unary nesting
    assert(postfixToInfix("2 sin cos") == "cos(sin(2))");
    
    return 0;
}
