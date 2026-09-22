Write a standalone C++ function that numerically solves a first-order ordinary differential equation (ODE) of the form \( y' = f(x, y) \) using the classical fourth-order Runge-Kutta method (RK4). The function must accept a mathematical expression for \( f(x, y) \) as a string (supporting basic arithmetic operators +, -, *, /, parentheses, and the symbols `x` and `y`), a step size `h`, a number of steps `n`, and initial conditions `x0` and `y0`. It must return a `std::vector<std::pair<double, double>>` where each pair is `(x_i, y_i)` for i = 0 to n, inclusive. The function must validate that the step count is non-negative, the step size is non-zero, and the expression string is non-empty and contains valid syntax. For simplicity, you may assume the expression only uses `x`, `y`, real numeric literals, and these operators: `+`, `-`, `*`, `/`, and parentheses (no functions like sin or exp). Because evaluating arbitrary string expressions requires a parser, implement a simple recursive-descent parser internally that converts the expression into an evaluator function taking `(x, y)` as inputs. The solution should be self-contained (no external libraries beyond `<string>`, `<vector>`, `<utility>`, `<cctype>`, `<stdexcept>`, `<cmath>`, `<sstream>` if needed), and must not use global mutable state.

The core challenge is twofold: (a) parse and evaluate the user-supplied expression `f(x,y)`, and (b) apply the RK4 update formula. For parsing, a recursive-descent parser is suitable: tokenize the string into numbers, variables (`x`, `y`), operators, and parentheses, then build an abstract syntax tree (AST) or evaluate directly during parsing. The grammar: `expression -> term (('+'|'-') term)*`, `term -> factor (('*'|'/') factor)*`, `factor -> number | 'x' | 'y' | '(' expression ')'`. Each parse function returns a `double` given current `x` and `y` values, or we can build nodes storing the operation and children. For simplicity, implement a parser that evaluates directly during parsing by passing current `x,y` to a recursive evaluator that also parses subexpressions on the fly. The tricky part is handling precedence and parentheses correctly; we can use a tokenizer that extracts numbers (including decimals), identifiers, and operators. Edge cases: empty or whitespace-only string → throw `std::invalid_argument`; unknown character → throw; division by zero during evaluation → throw `std::domain_error`; negative step count → throw `std::invalid_argument`; zero step size → throw. Also need to handle unary minus (e.g., `-x`), which can be treated as `0 - x` in the grammar by allowing a leading `+` or `-` in `factor`. For RK4, the algorithm is standard: for each step from i=0 to n-1, compute k1 = h*f(x_i, y_i), k2 = h*f(x_i+h/2, y_i+k1/2), k3 = h*f(x_i+h/2, y_i+k2/2), k4 = h*f(x_i+h, y_i+k3), then update y_{i+1} = y_i + (k1+2k2+2k3+k4)/6 and x_{i+1} = x_i + h. Store all points including the initial one. Time complexity: O(n * m) where m is the expression size (each evaluation parses the expression, but we can pre-parse once into an AST and reuse for all evaluations, making it O(n * m) for AST evaluation, or O(n * m) if we parse each time; we'll pre-parse to an AST for efficiency). Space: O(m) for AST and O(n) for output vector. The parser must handle negative numbers like `-2.5`, and whitespace between tokens.

#include <string>
#include <vector>
#include <utility>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include <memory>

// Node in the expression AST
struct ExprNode {
    virtual ~ExprNode() = default;
    virtual double eval(double x, double y) const = 0;
};

struct NumberNode : ExprNode {
    double value;
    explicit NumberNode(double v) : value(v) {}
    double eval(double, double) const override { return value; }
};

struct VariableNode : ExprNode {
    bool isX;
    explicit VariableNode(bool x) : isX(x) {}
    double eval(double x, double y) const override { return isX ? x : y; }
};

struct BinaryNode : ExprNode {
    char op;
    std::unique_ptr<ExprNode> left, right;
    BinaryNode(char o, std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    double eval(double x, double y) const override {
        double l = left->eval(x, y);
        double r = right->eval(x, y);
        switch (op) {
            case '+': return l + r;
            case '-': return l - r;
            case '*': return l * r;
            case '/':
                if (r == 0.0) throw std::domain_error("Division by zero in expression");
                return l / r;
            default: throw std::runtime_error("Unknown operator");
        }
    }
};

// Parser class
class ExpressionParser {
public:
    explicit ExpressionParser(const std::string& expr) : s(expr), pos(0) {
        skipWhitespace();
        if (pos >= s.size()) throw std::invalid_argument("Empty expression");
        root = parseExpression();
        skipWhitespace();
        if (pos < s.size()) throw std::invalid_argument("Unexpected character in expression");
    }
    double evaluate(double x, double y) const { return root->eval(x, y); }

private:
    std::string s;
    size_t pos;
    std::unique_ptr<ExprNode> root;

    void skipWhitespace() { while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos; }

    char peek() const { return pos < s.size() ? s[pos] : '\0'; }

    std::unique_ptr<ExprNode> parseExpression() {
        auto left = parseTerm();
        while (true) {
            skipWhitespace();
            char c = peek();
            if (c == '+' || c == '-') {
                ++pos;
                auto right = parseTerm();
                left = std::make_unique<BinaryNode>(c, std::move(left), std::move(right));
            } else {
                break;
            }
        }
        return left;
    }

    std::unique_ptr<ExprNode> parseTerm() {
        auto left = parseFactor();
        while (true) {
            skipWhitespace();
            char c = peek();
            if (c == '*' || c == '/') {
                ++pos;
                auto right = parseFactor();
                left = std::make_unique<BinaryNode>(c, std::move(left), std::move(right));
            } else {
                break;
            }
        }
        return left;
    }

    std::unique_ptr<ExprNode> parseFactor() {
        skipWhitespace();
        char c = peek();
        // Unary plus/minus
        if (c == '+') {
            ++pos;
            return parseFactor();
        } else if (c == '-') {
            ++pos;
            auto inner = parseFactor();
            // Build 0 - inner
            auto zero = std::make_unique<NumberNode>(0.0);
            return std::make_unique<BinaryNode>('-', std::move(zero), std::move(inner));
        }
        if (c == '(') {
            ++pos;
            auto expr = parseExpression();
            skipWhitespace();
            if (peek() != ')') throw std::invalid_argument("Missing closing parenthesis");
            ++pos; // consume ')'
            return expr;
        }
        // Number or variable
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::string numStr;
            while (pos < s.size() && (std::isdigit(static_cast<unsigned char>(s[pos])) || s[pos] == '.')) {
                numStr += s[pos++];
            }
            // Ensure it's a valid number (not like "1..2")
            size_t dotCount = 0;
            for (char ch : numStr) if (ch == '.') ++dotCount;
            if (dotCount > 1) throw std::invalid_argument("Invalid number format");
            try {
                double val = std::stod(numStr);
                return std::make_unique<NumberNode>(val);
            } catch (...) {
                throw std::invalid_argument("Invalid number");
            }
        }
        // Variable
        if (c == 'x' || c == 'y') {
            ++pos;
            return std::make_unique<VariableNode>(c == 'x');
        }
        throw std::invalid_argument(std::string("Unexpected character '") + c + "'");
    }
};

// Main solution function
// Solves y' = f(x,y) using RK4 with given step size and number of steps.
// Returns vector of (x, y) pairs starting at (x0, y0) and ending at x0 + n*h.
std::vector<std::pair<double, double>> solveRungeKutta4(
    const std::string& function,
    double h,
    size_t n,
    double x0,
    double y0) {
    if (h == 0.0) throw std::invalid_argument("Step size h must be non-zero");
    // Parse the expression once
    ExpressionParser parser(function);

    std::vector<std::pair<double, double>> points;
    points.reserve(n + 1);
    double x = x0, y = y0;
    points.emplace_back(x, y);
    for (size_t i = 0; i < n; ++i) {
        double k1 = h * parser.evaluate(x, y);
        double k2 = h * parser.evaluate(x + h/2, y + k1/2);
        double k3 = h * parser.evaluate(x + h/2, y + k2/2);
        double k4 = h * parser.evaluate(x + h, y + k3);
        y += (k1 + 2*k2 + 2*k3 + k4) / 6.0;
        x += h;
        points.emplace_back(x, y);
    }
    return points;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple dy/dx = y, y(0)=1, h=0.1, n=1 -> y(0.1) ≈ 1.10517
    auto p1 = solveRungeKutta4("y", 0.1, 1, 0.0, 1.0);
    assert(p1.size() == 2);
    assert(std::abs(p1[0].first - 0.0) < 1e-12);
    assert(std::abs(p1[0].second - 1.0) < 1e-12);
    assert(std::abs(p1[1].first - 0.1) < 1e-12);
    assert(std::abs(p1[1].second - 1.10517) < 1e-4);

    // Test 2: dy/dx = x, y(0)=0, h=0.5, n=2 -> y(1) ≈ 0.5 (exact: 0.5)
    auto p2 = solveRungeKutta4("x", 0.5, 2, 0.0, 0.0);
    assert(p2.size() == 3);
    assert(std::abs(p2[2].first - 1.0) < 1e-12);
    assert(std::abs(p2[2].second - 0.5) < 1e-12);

    // Test 3: dy/dx = 2*x + y, y(0)=0, h=0.1, n=1 -> exact y(0.1) = 0.0107 (approx)
    auto p3 = solveRungeKutta4("2*x + y", 0.1, 1, 0.0, 0.0);
    assert(std::abs(p3[1].second - 0.0107) < 1e-4);

    // Test 4: Expression with parentheses and division
    auto p4 = solveRungeKutta4("(x + y) / 2", 0.2, 1, 1.0, 1.0);
    // Manual RK4: f(1,1)=1, k1=0.2*1=0.2; f(1.1,1.1)=1.1, k2=0.22; f(1.1,1.11)=1.105, k3=0.221; f(1.2,1.221)=1.2105, k4=0.2421; y1=1+(0.2+0.44+0.442+0.2421)/6=1.22068333...
    assert(std::abs(p4[1].second - 1.22068333) < 1e-5);

    // Test 5: Negative coefficients and unary minus
    auto p5 = solveRungeKutta4("-y + 1", 0.1, 1, 0.0, 0.0);
    // Exact solution: y = 1 - e^{-x}, at x=0.1 => 1 - e^{-0.1} ≈ 0.09516
    assert(std::abs(p5[1].second - 0.0951626) < 1e-4);

    // Test 6: Zero steps returns only initial point
    auto p6 = solveRungeKutta4("x", 0.5, 0, 3.0, -2.0);
    assert(p6.size() == 1);
    assert(p6[0].first == 3.0 && p6[0].second == -2.0);

    // Test 7: Invalid expression should throw
    bool threw = false;
    try { solveRungeKutta4("x +", 0.1, 1, 0, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test 8: Invalid step size zero should throw
    threw = false;
    try { solveRungeKutta4("x", 0.0, 1, 0, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test 9: Division by zero inside expression throws
    threw = false;
    try { solveRungeKutta4("x / (y - 0)", 0.1, 1, 0, 0); } catch (const std::domain_error&) { threw = true; }
    assert(threw);

    // Test 10: Mixed operations with decimals
    auto p10 = solveRungeKutta4("1.5*x - 0.5*y + 2.0", 0.2, 1, 1.0, 1.0);
    // Manual: f(1,1)=1.5-0.5+2=3; k1=0.6; f(1.1,1.3)=1.65-0.65+2=3; k2=0.6; k3=0.6; f(1.2,1.6)=1.8-0.8+2=3; k4=0.6; y1=1+(0.6+1.2+1.2+0.6)/6=1.6
    assert(std::abs(p10[1].second - 1.6) < 1e-12);
}
