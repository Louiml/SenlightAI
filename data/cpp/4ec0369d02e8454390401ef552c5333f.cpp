Write a C++ function `bool evaluateExpression(const std::string& expr)` that determines whether a given arithmetic expression is syntactically valid according to these rules: it contains only non-negative integers, parentheses, and the binary operators `+`, `-`, `*`, `/`, `%`, `^`, and unary minus `-` (appearing immediately before an operand, e.g., `-5` or `-(3+2)`, but not `--5`). An expression is valid if parentheses are balanced, no operator appears consecutively (except unary minus after another operator or opening parenthesis), the first token is not a binary operator (except unary minus), the last token is not an operator or a closing parenthesis, operands may be multi-digit, and unary minus is only allowed before an operand or after an opening parenthesis or another operator. The function should return `true` for valid expressions and `false` for invalid ones, treating whitespace as insignificant. You may assume the input string length is at most 1,000,000.

#include <cassert>
#include <string>

// Function declaration from solution.
bool evaluateExpression(const std::string& expr);

int main() {
    // Valid expressions.
    assert(evaluateExpression("1+2*3") == true);
    assert(evaluateExpression("(1+2)*3") == true);
    assert(evaluateExpression("-5+3") == true);
    assert(evaluateExpression("-(3+2)") == true);
    assert(evaluateExpression("2^-3") == true);
    assert(evaluateExpression("  ( 10 / 2 ) % 3 ") == true);
    assert(evaluateExpression("123") == true);
    
    // Invalid expressions.
    assert(evaluateExpression("1+") == false);
    assert(evaluateExpression("+1") == false);
    assert(evaluateExpression("1 2") == false);
    assert(evaluateExpression("(1+)") == false);
    assert(evaluateExpression("()") == false);
    assert(evaluateExpression("1*-2") == true); // unary minus after binary operator is valid
    assert(evaluateExpression("1+-2") == true); // unary minus after binary operator is valid
    assert(evaluateExpression("1--2") == false); // double unary minus not allowed
    assert(evaluateExpression(")1(") == false);
    assert(evaluateExpression("1+*2") == false);
    assert(evaluateExpression("(1+2") == false);
    
    // Edge cases.
    assert(evaluateExpression("") == false);
    assert(evaluateExpression("   ") == false);
    assert(evaluateExpression("-(1)") == true);
    assert(evaluateExpression("-(1)+2") == true);
    assert(evaluateExpression("2/-") == false);
    assert(evaluateExpression("(-5)") == true);
    assert(evaluateExpression("5-") == false);
    
    return 0;
}

#include <string>
#include <vector>
#include <stack>

// A token can be an operand, an operator, or a parenthesis.
struct Token {
    enum Type { Operand, Operator, Parenthesis } type;
    int value; // For operator: 1='~' (unary), 2='^',3='/',4='%',5='*',6='+',7='-'
               // For parenthesis: 1='(', -1=')'
};

// Determine if an expression string is syntactically valid per given rules.
bool evaluateExpression(const std::string& expr) {
    std::vector<Token> tokens;
    int i = 0;
    const int n = static_cast<int>(expr.size());
    
    // Tokenize the input.
    while (i < n) {
        char c = expr[i];
        if (c == ' ' || c == '\t') { i++; continue; }
        if (c == '(') {
            tokens.push_back({Token::Parenthesis, 1});
            i++;
        } else if (c == ')') {
            tokens.push_back({Token::Parenthesis, -1});
            i++;
        } else if (c >= '0' && c <= '9') {
            long long val = 0;
            while (i < n && expr[i] >= '0' && expr[i] <= '9') {
                val = val * 10 + (expr[i] - '0');
                if (val > 1000000000) return false; // avoid overflow
                i++;
            }
            tokens.push_back({Token::Operand, static_cast<int>(val)});
        } else if (c == '^' || c == '/' || c == '%' || c == '*' || c == '+' || c == '-') {
            int opVal;
            if (c == '^') opVal = 2;
            else if (c == '/') opVal = 3;
            else if (c == '%') opVal = 4;
            else if (c == '*') opVal = 5;
            else if (c == '+') opVal = 6;
            else opVal = 7; // '-'
            tokens.push_back({Token::Operator, opVal});
            i++;
        } else {
            return false; // illegal character
        }
    }
    
    if (tokens.empty()) return false;
    
    // Convert a binary '-' (value 7) to unary '~' (value 1) when appropriate.
    // It becomes unary if it is the first token, or the previous token is an operator or an opening parenthesis.
    for (int idx = 0; idx < static_cast<int>(tokens.size()); ++idx) {
        if (tokens[idx].type == Token::Operator && tokens[idx].value == 7) {
            bool prevIsOperatorOrOpen = (idx == 0) || 
                (tokens[idx-1].type == Token::Operator) || 
                (tokens[idx-1].type == Token::Parenthesis && tokens[idx-1].value == 1);
            if (prevIsOperatorOrOpen) {
                tokens[idx].value = 1; // unary minus
            }
        }
    }
    
    // Syntax validation over adjacent token pairs.
    bool valid = true;
    for (int idx = 0; idx < static_cast<int>(tokens.size()) - 1; ++idx) {
        const Token& a = tokens[idx];
        const Token& b = tokens[idx+1];
        if (a.type == Token::Operand) {
            // An operand cannot be followed by an operand, an opening parenthesis, or a unary minus.
            if (b.type == Token::Operand ||
                (b.type == Token::Parenthesis && b.value == 1) ||
                (b.type == Token::Operator && b.value == 1)) {
                valid = false;
                break;
            }
        } else if (a.type == Token::Operator) {
            if (a.value == 1) {
                // Unary minus must be followed by an operand or an opening parenthesis.
                if (b.type == Token::Operand ||
                    (b.type == Token::Parenthesis && b.value == 1)) {
                    // okay
                } else {
                    valid = false;
                    break;
                }
            } else {
                // Binary operator must be followed by an operand or an opening parenthesis.
                if (b.type == Token::Operand ||
                    (b.type == Token::Parenthesis && b.value == 1)) {
                    // okay
                } else {
                    valid = false;
                    break;
                }
            }
        } else { // a is parenthesis
            if (a.value == 1) { // '('
                // '(' must be followed by operand, opening parenthesis, or unary minus.
                if (!(b.type == Token::Operand ||
                      (b.type == Token::Parenthesis && b.value == 1) ||
                      (b.type == Token::Operator && b.value == 1))) {
                    valid = false;
                    break;
                }
            } else { // ')'
                // ')' must be followed by a binary operator or a closing parenthesis.
                if (!(b.type == Token::Operator && b.value != 1) &&
                    !(b.type == Token::Parenthesis && b.value == -1)) {
                    valid = false;
                    break;
                }
            }
        }
    }
    
    if (!valid) return false;
    
    // Check first and last token constraints.
    if (tokens.front().type == Token::Operator && tokens.front().value != 1) return false; // first cannot be binary op
    if (tokens.back().type == Token::Operator) return false; // last cannot be any operator
    if (tokens.back().type == Token::Parenthesis && tokens.back().value == 1) return false; // last cannot be '('
    
    // Check that any binary operator is not at the beginning or end (already checked), and that there is no binary operator followed by ')' (already in pair validation).
    
    // Parenthesis balance check.
    std::stack<char> parenStack;
    for (const Token& t : tokens) {
        if (t.type == Token::Parenthesis) {
            if (t.value == 1) parenStack.push('(');
            else {
                if (parenStack.empty() || parenStack.top() != '(') return false;
                parenStack.pop();
            }
        }
    }
    if (!parenStack.empty()) return false;
    
    return true;
}

// The approach is to tokenize the input into a sequence of token structs (operand, operator, parenthesis) and then apply two validation passes: first, a syntax checker that examines token pairs for illegal sequences (e.g., two operands in a row, binary operator followed by another binary operator, an operand followed by a closing parenthesis, etc.), and second, a parenthesis balance check using a stack. Key edge cases include handling multi-digit numbers without overflow (use direct digit accumulation), treating unary minus as a special operator that can legally appear at the start or after another operator or opening parenthesis, and rejecting expressions that start or end with a binary operator or have a trailing unary minus. The original code snippet uses a specialized token parser and a checker function, but we can simplify it by constructing tokens while reading and then validating in two passes. Time complexity is `O(n)` for both tokenization and validation, space complexity is `O(n)` for storing tokens (though we could stream, but for clarity we store them).
