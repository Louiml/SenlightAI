Write a C++ function `std::optional<double> evaluate_expression(const std::string& expr)` that parses and evaluates a mathematical expression string. The expression may contain non-negative numbers (including decimals), the binary operators `+`, `-`, `*`, `/`, `^` (exponentiation), parentheses, and the unary functions `sin` and `cos` (applied to a parenthesized argument). Whitespace in the input should be ignored. The function must return `std::nullopt` if the expression is invalid (e.g., unbalanced parentheses, unknown characters, misplaced operators/operands, a function not followed by `(`, division by zero, or exponentiation domain errors). Otherwise it returns the computed double result. The expression grammar: `expr := term {(+|-) term}`; `term := factor {(*|/) factor}`; `factor := power {^ power}`; `power := number | function ( expr ) | ( expr )`. Unary plus/minus are not allowed except as binary operators (however, a leading `+` or `-` immediately after `(` should be treated as `0+`/`0-` for convenience). Use a standard approach: tokenize, parse into an AST or use recursive descent, then evaluate.
// The solution uses **recursive descent parsing** to directly evaluate the expression while checking validity. First, strip all whitespace. Then implement a recursive parser with an index pointer: `parseExpression` handles addition/subtraction, `parseTerm` handles multiplication/division, `parseFactor` handles exponentiation (right-associative), and `parsePrimary` handles numbers, parentheses, and functions (`sin`, `cos`). At each level, check for expected tokens and set an error flag on any mismatch. Handle the `(+`/`(-` cases by injecting a `0` token. After parsing, ensure the entire string is consumed. For exponentiation, check `errno` after `pow` to detect domain errors (e.g., negative base with fractional exponent). Division by zero is detected when denominator is zero. Edge cases: empty string, unbalanced parentheses, consecutive operators, missing operands, unknown characters, numbers like `12.3.4` (reject by parsing the number with `strtod` and checking the full token), and function names that aren't followed by `(`. The algorithm runs in O(n) time and O(depth) stack space for recursion, where depth is proportional to nesting.
#include <optional>
#include <string>
#include <cctype>
#include <cmath>
#include <cerrno>

// Evaluate a mathematical expression; returns nullopt on invalid input.
std::optional<double> evaluate_expression(const std::string& expr) {
    std::string s;
    for (char c : expr) {
        if (!std::isspace(static_cast<unsigned char>(c))) s.push_back(c);
    }

    size_t pos = 0;
    bool error = false;

    // Parse a number starting at pos, advance pos.
    auto parseNumber = [&]() -> std::optional<double> {
        size_t start = pos;
        if (pos < s.size() && (std::isdigit(static_cast<unsigned char>(s[pos])) || s[pos] == '.')) {
            while (pos < s.size() && (std::isdigit(static_cast<unsigned char>(s[pos])) || s[pos] == '.')) pos++;
            char* end = nullptr;
            double val = std::strtod(s.c_str() + start, &end);
            if (end != s.c_str() + pos) { error = true; return std::nullopt; }
            return val;
        }
        error = true;
        return std::nullopt;
    };

    // Forward declarations.
    std::function<std::optional<double>()> parseExpression;
    std::function<std::optional<double>()> parsePrimary;

    // Primary: number, function ( expr ), or ( expr )
    parsePrimary = [&]() -> std::optional<double> {
        if (pos >= s.size()) { error = true; return std::nullopt; }
        char c = s[pos];

        // Handle (+ or (- as (0+ or (0-
        if (c == '(') {
            if (pos + 1 < s.size() && (s[pos+1] == '+' || s[pos+1] == '-')) {
                s.insert(pos+1, "0");
                // pos now points to '0', continue normally.
            }
        }

        // number
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            return parseNumber();
        }

        // function
        if (std::isalpha(static_cast<unsigned char>(c))) {
            size_t start = pos;
            while (pos < s.size() && std::isalpha(static_cast<unsigned char>(s[pos]))) pos++;
            std::string name = s.substr(start, pos - start);
            if (name != "sin" && name != "cos") { error = true; return std::nullopt; }
            if (pos >= s.size() || s[pos] != '(') { error = true; return std::nullopt; }
            pos++; // consume '('
            auto arg = parseExpression();
            if (!arg || error) return std::nullopt;
            if (pos >= s.size() || s[pos] != ')') { error = true; return std::nullopt; }
            pos++; // consume ')'
            if (name == "sin") return std::sin(*arg);
            else return std::cos(*arg);
        }

        // '(' expression ')'
        if (c == '(') {
            pos++; // consume '('
            auto val = parseExpression();
            if (!val || error) return std::nullopt;
            if (pos >= s.size() || s[pos] != ')') { error = true; return std::nullopt; }
            pos++; // consume ')'
            return val;
        }

        error = true;
        return std::nullopt;
    };

    // Factor: handle exponentiation (right-associative)
    std::function<std::optional<double>()> parseFactor;
    parseFactor = [&]() -> std::optional<double> {
        auto base = parsePrimary();
        if (!base || error) return std::nullopt;
        if (pos < s.size() && s[pos] == '^') {
            pos++;
            auto exp_opt = parseFactor(); // right-assoc
            if (!exp_opt || error) return std::nullopt;
            errno = 0;
            double result = std::pow(*base, *exp_opt);
            if (errno != 0) { error = true; return std::nullopt; }
            return result;
        }
        return base;
    };

    // Term: *, /
    std::function<std::optional<double>()> parseTerm;
    parseTerm = [&]() -> std::optional<double> {
        auto val = parseFactor();
        if (!val || error) return std::nullopt;
        while (pos < s.size() && (s[pos] == '*' || s[pos] == '/')) {
            char op = s[pos++];
            auto rhs = parseFactor();
            if (!rhs || error) return std::nullopt;
            if (op == '*') {
                val = *val * *rhs;
            } else {
                if (*rhs == 0.0) { error = true; return std::nullopt; }
                val = *val / *rhs;
            }
        }
        return val;
    };

    // Expression: +, -
    parseExpression = [&]() -> std::optional<double> {
        auto val = parseTerm();
        if (!val || error) return std::nullopt;
        while (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) {
            char op = s[pos++];
            auto rhs = parseTerm();
            if (!rhs || error) return std::nullopt;
            if (op == '+') val = *val + *rhs;
            else val = *val - *rhs;
        }
        return val;
    };

    auto result = parseExpression();
    if (error || pos != s.size() || !result) return std::nullopt;
    return result;
}
#include <cassert>
#include <cmath>
#include <iostream>
#include <optional>

// Declare the function (assuming the solution is above or included)
std::optional<double> evaluate_expression(const std::string& expr);

int main() {
    // Basic arithmetic
    assert(evaluate_expression("1+2") == std::optional<double>(3.0));
    assert(evaluate_expression("2*3+4") == std::optional<double>(10.0));
    assert(evaluate_expression("10/4") == std::optional<double>(2.5));
    assert(evaluate_expression("2^3") == std::optional<double>(8.0));

    // Parentheses and functions
    assert(evaluate_expression("(1+2)*3") == std::optional<double>(9.0));
    assert(evaluate_expression("sin(0)") == std::optional<double>(0.0));
    assert(evaluate_expression("cos(0)") == std::optional<double>(1.0));
    assert(std::abs(*evaluate_expression("2*sin(0.5)") - 2*std::sin(0.5)) < 1e-9);

    // Leading + or - inside parentheses
    assert(evaluate_expression("(+5)") == std::optional<double>(5.0));
    assert(evaluate_expression("(-3)") == std::optional<double>(-3.0));

    // Whitespace handling
    assert(evaluate_expression("  1 + 2 ") == std::optional<double>(3.0));

    // Invalid expressions should return nullopt
    assert(!evaluate_expression(""));
    assert(!evaluate_expression("1+"));
    assert(!evaluate_expression("+1"));
    assert(!evaluate_expression("1/0"));
    assert(!evaluate_expression("(-1)^0.5")); // domain error
    assert(!evaluate_expression("sin 0"));
    assert(!evaluate_expression("(1+2"));
    assert(!evaluate_expression("1..2"));
    assert(!evaluate_expression("abc"));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
