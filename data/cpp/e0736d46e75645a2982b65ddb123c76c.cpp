Write a standalone C++ function that evaluates a simple arithmetic expression provided as a string containing floating-point numbers and the operators `+`, `-`, `*`, `/`, `%`, and parentheses `(` and `)`. The function must respect the usual precedence rules: parentheses first, then unary plus/minus, then multiplicative operators (`*`, `/`, `%`), then additive operators (`+`, `-`). The `%` operator applies to integers (truncate operands toward zero before applying modulo), and division by zero must be detected and reported. The expression is guaranteed to be syntactically valid, contain only supported characters, and may include spaces. The function should return the computed double result. If an error occurs (division by zero or remainder by zero), throw a `std::runtime_error` with a descriptive message. The function signature is `double evaluate(const std::string& expr)`. You are not allowed to use `std::function`, `std::variant`, or any dynamic memory allocation; implement a recursive-descent parser with a token stream similar to the given snippet but adapted to read from a string instead of `cin`.

// The solution uses a recursive-descent parser with three mutually recursive functions: `primary()`, `term()`, and `expression()`, mirroring the grammar from the snippet but treating the input as a `std::string` with an internal index pointer. A helper tokenizer reads the next token from the string, skipping whitespace, and returns either a number (double), a character for operators and parentheses, or an invalid token if unexpected input occurs. `primary()` handles numbers, parenthesized expressions, and unary plus/minus. `term()` handles `*`, `/`, and `%` with left associativity, checking for division/remainder by zero and throwing `std::runtime_error`. `expression()` handles `+` and `-`. The main `evaluate` function initializes the token stream and calls `expression()`, then optionally checks that the entire input has been consumed. Edge cases: unary minus with negative numbers, nested parentheses, `%` with non-integer operands (truncated toward zero), and spaces anywhere. Time complexity is O(n) for n characters of input, as each token is processed once. Space complexity is O(1) aside from the constant call stack depth proportional to nesting depth (worst case O(n)).

#include <string>
#include <stdexcept>
#include <cctype>

class TokenStream {
public:
    explicit TokenStream(const std::string& s) : src(s), pos(0) {}

    struct Token {
        char kind;      // 'n' for number, otherwise operator char
        double value;   // for numbers
        Token(char k) : kind(k), value(0) {}
        Token(char k, double v) : kind(k), value(v) {}
    };

    Token get() {
        // Skip whitespace
        while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) {
            ++pos;
        }
        if (pos >= src.size()) {
            return Token('\0'); // end of input
        }

        char ch = src[pos++];
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
            ch == '(' || ch == ')') {
            return Token(ch);
        }

        // Number (starts with digit or dot)
        if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.') {
            size_t start = pos - 1;
            while (pos < src.size() &&
                   (std::isdigit(static_cast<unsigned char>(src[pos])) || src[pos] == '.')) {
                ++pos;
            }
            std::string numStr = src.substr(start, pos - start);
            try {
                size_t processed = 0;
                double val = std::stod(numStr, &processed);
                if (processed != numStr.size()) {
                    throw std::runtime_error("Invalid number format");
                }
                return Token('n', val);
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Number out of range");
            }
        }

        throw std::runtime_error("Unexpected character in expression");
    }

private:
    const std::string& src;
    size_t pos;
};

// Forward declarations
double expression(TokenStream& ts);

double primary(TokenStream& ts) {
    Token t = ts.get();
    if (t.kind == 'n') {
        return t.value;
    }
    if (t.kind == '(') {
        double d = expression(ts);
        Token close = ts.get();
        if (close.kind != ')') {
            throw std::runtime_error("Expected ')'");
        }
        return d;
    }
    if (t.kind == '-') {
        return -primary(ts);
    }
    if (t.kind == '+') {
        return primary(ts);
    }
    throw std::runtime_error("Expected a primary expression");
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
            if (d == 0.0) {
                throw std::runtime_error("Division by zero");
            }
            left /= d;
            t = ts.get();
        } else if (t.kind == '%') {
            int i1 = static_cast<int>(left);
            double d = primary(ts);
            int i2 = static_cast<int>(d);
            if (i2 == 0) {
                throw std::runtime_error("Modulo by zero");
            }
            left = i1 % i2;
            t = ts.get();
        } else {
            break;
        }
    }
    // Put back the unprocessed token
    // Since our TokenStream doesn't support putback, we need to handle it differently.
    // For simplicity, we'll store the lookahead token in a small buffer.
    // We'll modify the TokenStream to support putback.
    // Implementation below.
    return left;
}

Given the complexity of putback, here is the complete correct solution with a proper `putback` mechanism:

#include <string>
#include <stdexcept>
#include <cctype>
#include <cstdlib>

class Token {
public:
    char kind;
    double value;
    Token(char k) : kind(k), value(0) {}
    Token(char k, double v) : kind(k), value(v) {}
};

class TokenStream {
public:
    TokenStream(const std::string& s) : src(s), pos(0), buffer(), full(false) {}
    Token get() {
        if (full) { full = false; return buffer; }
        while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) ++pos;
        if (pos >= src.size()) return Token('\0');
        char ch = src[pos++];
        switch (ch) {
            case '+': case '-': case '*': case '/': case '%': case '(': case ')':
                return Token(ch);
            default: {
                if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.') {
                    size_t start = pos - 1;
                    while (pos < src.size() && (std::isdigit(static_cast<unsigned char>(src[pos])) || src[pos] == '.')) ++pos;
                    std::string numStr = src.substr(start, pos - start);
                    try {
                        size_t processed = 0;
                        double val = std::stod(numStr, &processed);
                        if (processed != numStr.size()) throw std::runtime_error("Invalid number");
                        return Token('n', val);
                    } catch (const std::out_of_range&) {
                        throw std::runtime_error("Number out of range");
                    }
                }
                throw std::runtime_error("Unexpected character");
            }
        }
    }
    void putback(Token t) {
        if (full) throw std::runtime_error("putback into full buffer");
        buffer = t; full = true;
    }
private:
    const std::string& src;
    size_t pos;
    Token buffer;
    bool full;
};

double expression(TokenStream& ts);
double primary(TokenStream& ts) {
    Token t = ts.get();
    if (t.kind == 'n') return t.value;
    if (t.kind == '(') {
        double d = expression(ts);
        Token close = ts.get();
        if (close.kind != ')') throw std::runtime_error("Expected ')'");
        return d;
    }
    if (t.kind == '-') return -primary(ts);
    if (t.kind == '+') return primary(ts);
    throw std::runtime_error("Expected primary");
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
            if (d == 0.0) throw std::runtime_error("Division by zero");
            left /= d;
            t = ts.get();
        } else if (t.kind == '%') {
            int i1 = static_cast<int>(left);
            double d = primary(ts);
            int i2 = static_cast<int>(d);
            if (i2 == 0) throw std::runtime_error("Modulo by zero");
            left = i1 % i2;
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

// Main solution function
double evaluate(const std::string& expr) {
    TokenStream ts(expr);
    double result = expression(ts);
    Token t = ts.get();
    if (t.kind != '\0') {
        throw std::runtime_error("Unexpected trailing input");
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <string>

int main() {
    // Basic arithmetic
    assert(evaluate("1+2") == 3.0);
    assert(evaluate("2*3+4") == 10.0);
    assert(evaluate("10-2-3") == 5.0);
    assert(evaluate("20/5") == 4.0);
    assert(evaluate("7%3") == 1.0);

    // Parentheses and unary
    assert(evaluate("(2+3)*4") == 20.0);
    assert(evaluate("-(5+2)") == -7.0);
    assert(evaluate("+5") == 5.0);
    assert(evaluate("((2))") == 2.0);

    // Decimals and negatives
    assert(std::abs(evaluate("2.5*2") - 5.0) < 1e-9);
    assert(std::abs(evaluate("-3.5+1") - (-2.5)) < 1e-9);

    // Spaces and complex
    assert(evaluate(" 1 + 2 * ( 3 - 1 ) ") == 5.0);
    assert(evaluate("10 % 3 + 1") == 2.0);

    // Error cases
    bool threwDivZero = false;
    try { evaluate("1/0"); } catch (const std::runtime_error&) { threwDivZero = true; }
    assert(threwDivZero);

    bool threwModZero = false;
    try { evaluate("5%0"); } catch (const std::runtime_error&) { threwModZero = true; }
    assert(threwModZero);

    // Large numbers
    assert(evaluate("1000000*1000000") == 1e12);
}
