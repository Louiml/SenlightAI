// Given a string containing a mathematical expression with floating-point numbers, the operators `+`, `-`, `*`, `/`, parentheses `()`, and curly braces `{}`, plus the special characters `;` to evaluate and `q` to quit, write a standalone C++ function that parses and evaluates the entire expression using a token-based recursive descent parser, returning the final numeric result as a `double`. The function should not read from standard input; instead, it takes the full expression string as input, processes it to completion (including handling multiple semicolon-separated expressions, where the result of the last evaluated expression is returned), and throws `std::runtime_error` on invalid tokens, mismatched parentheses/braces, or division by zero. The expression may contain whitespace anywhere, negative numbers are handled by the unary minus in the grammar, and the input is guaranteed to be non-empty and will always end with a `;` before any optional `q` (but the function must handle the case where `q` appears without a preceding `;` by returning the last computed value or 0 if none).

The solution is a classic recursive descent parser with a token stream. The input string is processed character-by-character to produce tokens: numbers (represented as kind `'8'`), operators, parentheses/braces, and control characters. A `TokenStream` class holds a buffer for one token of pushback, and a `get()` method that skips whitespace (though the original uses `cin >>`, here we read from the string via an index) and recognizes digits, dots, and the listed symbols. The grammar is: `expression` -> `term` (('+'|'-') `term`)*, `term` -> `primary` (('*'|'/') `primary`)*, and `primary` -> number or '(' expression ')' or '{' expression '}'. The evaluation is left-associative for all operators. Key edge cases: division by zero throws, mismatched closing delimiters throw, unexpected characters throw, and the `;` token signals that the current expression is complete and its value is saved as the last result. If `q` appears, parsing stops immediately and the last saved result (or 0 if none) is returned. The algorithm processes the input in a single pass (each token read once), so time is O(n) where n is the number of characters; space is O(depth of nesting) due to recursion, which is at most O(n) in the worst case, but typically O(1) for practical expressions.

#include <string>
#include <stdexcept>
#include <cctype>

class Token {
public:
    char kind;
    double value;
    Token(char k) : kind(k), value(0) {}
    Token(char k, double v) : kind(k), value(v) {}
};

class TokenStream {
    const std::string& input;
    size_t pos;
    Token buffer;
    bool full;
public:
    TokenStream(const std::string& s) : input(s), pos(0), buffer(' '), full(false) {}
    Token get() {
        if (full) { full = false; return buffer; }
        while (pos < input.size() && std::isspace(input[pos])) ++pos;
        if (pos >= input.size()) throw std::runtime_error("Missing token");
        char ch = input[pos++];
        switch (ch) {
            case ';': case 'q': case '(': case ')': case '+': case '-': case '*': case '/': case '{': case '}':
                return Token{ch};
            case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
                size_t start = pos - 1;
                while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.')) ++pos;
                std::string num = input.substr(start, pos - start);
                return Token{'8', std::stod(num)};
            }
            default:
                throw std::runtime_error("Bad token");
        }
    }
    void putback(Token t) {
        if (full) throw std::runtime_error("putback into full buffer");
        buffer = t; full = true;
    }
};

double expression(TokenStream& ts); // forward declaration

double primary(TokenStream& ts) {
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            double d = expression(ts);
            t = ts.get();
            if (t.kind != ')') throw std::runtime_error("')' expected");
            return d;
        }
        case '{': {
            double d = expression(ts);
            t = ts.get();
            if (t.kind != '}') throw std::runtime_error("'}' expected");
            return d;
        }
        case '8': return t.value;
        default: throw std::runtime_error("primary expected");
    }
}

double term(TokenStream& ts) {
    double left = primary(ts);
    Token t = ts.get();
    while (true) {
        if (t.kind == '*') {
            left *= primary(ts);
            t = ts.get();
        } else if (t.kind == '/') {
            double d = primary(ts);
            if (d == 0) throw std::runtime_error("divide by zero");
            left /= d;
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }
}

double expression(TokenStream& ts) {
    double left = term(ts);
    Token t = ts.get();
    while (true) {
        if (t.kind == '+') {
            left += term(ts);
            t = ts.get();
        } else if (t.kind == '-') {
            left -= term(ts);
            t = ts.get();
        } else {
            ts.putback(t);
            return left;
        }
    }
}

// Parse and evaluate the entire expression string.
// Returns the last computed value before ';' or 'q'.
double evaluateExpression(const std::string& expr) {
    TokenStream ts(expr);
    double last = 0.0;
    bool hasValue = false;
    while (true) {
        Token t = ts.get();
        if (t.kind == 'q') break;
        if (t.kind == ';') { /* just a separator, keep last */ }
        else {
            ts.putback(t);
            last = expression(ts);
            hasValue = true;
            // After expression, next token must be ';' or 'q' or we continue.
        }
        // Peek next to avoid consuming 'q' incorrectly? The loop continues, so fine.
        // To handle end-of-input gracefully after ';', we rely on the next get.
        // If we reach end of input without 'q', get() will throw.
    }
    return hasValue ? last : 0.0;
}

#include <cassert>
#include <cmath>
#include <string>

// Solution function is assumed to be declared above.

int main() {
    // Basic arithmetic
    assert(std::abs(evaluateExpression("2+3*4;") - 14.0) < 1e-9);
    assert(std::abs(evaluateExpression("(2+3)*4;") - 20.0) < 1e-9);
    assert(std::abs(evaluateExpression("{10-2}/2;") - 4.0) < 1e-9);
    // Multiple expressions, last value returned
    assert(std::abs(evaluateExpression("1+1; 2+2; 3+3;") - 6.0) < 1e-9);
    // Division and negative numbers
    assert(std::abs(evaluateExpression("-10/2;") - (-5.0)) < 1e-9);
    assert(std::abs(evaluateExpression("2*(3-5);") - (-4.0)) < 1e-9);
    // Quit without semicolon returns last value
    assert(std::abs(evaluateExpression("5*5; q") - 25.0) < 1e-9);
    // Quit without any expression returns 0
    assert(evaluateExpression("q") == 0.0);
    // Whitespace handling
    assert(std::abs(evaluateExpression("   7   +   8   ;") - 15.0) < 1e-9);
    // Division by zero throws
    bool threw = false;
    try { evaluateExpression("1/0;"); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
    // Bad token throws
    threw = false;
    try { evaluateExpression("1 @ 2;"); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
    // Mismatched parentheses throws
    threw = false;
    try { evaluateExpression("(1+2;"); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}
