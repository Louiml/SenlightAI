Write a C++ function named `solveExpression` that takes a single string argument representing a mathematical expression containing non-negative integers, the operators `+`, `-`, `*`, and parentheses `(` and `)`. The expression is guaranteed to be syntactically valid, contains no spaces, and all numbers are between 0 and 1000. The function must evaluate the expression following standard operator precedence (multiplication before addition/subtraction, parentheses override) and return the integer result. The result is guaranteed to fit in a 32-bit signed integer. Handle unary minus (e.g., `-5` or `-(3+2)`) correctly. Your function should be self-contained and not rely on external libraries beyond standard C++.
The solution uses a recursive descent parser with two main recursive functions: one for parsing expressions with addition/subtraction (`parseExpr`) and one for parsing terms with multiplication (`parseTerm`). A helper `parseFactor` handles numbers, parentheses, and unary minus. The parser maintains an index into the input string, advancing it as tokens are consumed.  
- `parseExpr`: parses a term, then while the next character is `+` or `-`, consumes that operator, parses another term, and performs the operation.  
- `parseTerm`: parses a factor, then while the next character is `*`, consumes it, parses another factor, and multiplies.  
- `parseFactor`: if the current character is `-`, it consumes it and returns the negative of parsing a factor (handling unary minus recursively); if it is `(`, it consumes it, calls `parseExpr`, expects `)`, and returns the inner value; otherwise, it reads consecutive digits into an integer.  
Edge cases: unary minus can appear at the start or after an operator (e.g., `-5+3` or `2*-3`), and nested parentheses. Empty input is not expected. Time complexity is O(n) where n is the length of the string, since each character is visited once. Space complexity is O(d) for recursion depth, worst-case O(n) for deeply nested parentheses.
#include <string>
#include <cctype>

// Evaluate a mathematical expression with +, -, *, parentheses, and unary minus.
int solveExpression(const std::string& s) {
    size_t index = 0;

    // Forward declarations for mutual recursion
    int parseExpr();
    int parseTerm();
    int parseFactor();

    int parseExpr() {
        int value = parseTerm();
        while (index < s.size() && (s[index] == '+' || s[index] == '-')) {
            char op = s[index++];
            int rhs = parseTerm();
            if (op == '+') {
                value += rhs;
            } else {
                value -= rhs;
            }
        }
        return value;
    }

    int parseTerm() {
        int value = parseFactor();
        while (index < s.size() && s[index] == '*') {
            index++; // consume '*'
            int rhs = parseFactor();
            value *= rhs;
        }
        return value;
    }

    int parseFactor() {
        if (s[index] == '-') {
            index++; // consume '-'
            return -parseFactor();
        }
        if (s[index] == '(') {
            index++; // consume '('
            int value = parseExpr();
            index++; // consume ')'
            return value;
        }
        int value = 0;
        while (index < s.size() && std::isdigit(static_cast<unsigned char>(s[index]))) {
            value = value * 10 + (s[index] - '0');
            index++;
        }
        return value;
    }

    return parseExpr();
}
#include <cassert>

// Helper for tests
int main() {
    // Basic precedence and operators
    assert(solveExpression("1+2*3") == 7);
    assert(solveExpression("2*3+4") == 10);
    assert(solveExpression("10-3-2") == 5);
    // Parentheses
    assert(solveExpression("(1+2)*3") == 9);
    assert(solveExpression("((2+3)*(4-1))") == 15);
    // Unary minus
    assert(solveExpression("-5") == -5);
    assert(solveExpression("--5") == 5);
    assert(solveExpression("2*-3") == -6);
    assert(solveExpression("-(3+2)*4") == -20);
    // Mixed and larger numbers
    assert(solveExpression("100+20*3") == 160);
    assert(solveExpression("0*99+1") == 1);
    assert(solveExpression("7-(2+3)") == 2);
    assert(solveExpression("50-5*8") == 10);
    // Deep nesting
    assert(solveExpression("((((1))))") == 1);
    assert(solveExpression("(((2+3)*4)-5)") == 15);
    return 0;
}
