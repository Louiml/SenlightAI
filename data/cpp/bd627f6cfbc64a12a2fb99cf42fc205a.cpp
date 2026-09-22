// Write a C++ function `double evaluateExpression(int a, char op, int b)` that performs basic arithmetic based on the operator character: `'+'` for addition, `'-'` for subtraction, `'*'` for multiplication, `'/'` for division, and `'^'` for exponentiation (using `std::pow`). The function must safely handle division by zero: if `b == 0` and the operator is `'/'`, return `std::numeric_limits<double>::quiet_NaN()`. For any other operator (e.g., `'%'` or `'?'`), return `std::numeric_limits<double>::quiet_NaN()` as well. The operands are integers, but the result is a double to support non‑integer division results (e.g., `5 / 2 = 2.5`). The function must be `const`‑correct (no mutation of inputs) and use meaningful parameter names. Do not include `main()` in the solution; provide it only in the test section.
#include <cassert>
#include <cmath>
#include <limits>

// Assume evaluateExpression is defined above (or included from the solution).

int main() {
    // Basic arithmetic
    assert(evaluateExpression(10, '+', 5) == 15.0);
    assert(evaluateExpression(10, '-', 5) == 5.0);
    assert(evaluateExpression(10, '*', 5) == 50.0);
    assert(evaluateExpression(10, '/', 5) == 2.0);
    assert(evaluateExpression(10, '/', 4) == 2.5);

    // Exponent with positive and negative exponent
    assert(evaluateExpression(2, '^', 3) == 8.0);
    assert(std::abs(evaluateExpression(2, '^', -1) - 0.5) < 1e-9);

    // Negative numbers
    assert(evaluateExpression(-6, '+', 3) == -3.0);
    assert(evaluateExpression(-6, '-', -3) == -3.0);
    assert(evaluateExpression(-6, '*', 3) == -18.0);
    assert(evaluateExpression(-6, '/', 3) == -2.0);

    // Division by zero -> NaN
    assert(std::isnan(evaluateExpression(5, '/', 0)));

    // Unsupported operator -> NaN
    assert(std::isnan(evaluateExpression(5, '%', 2)));

    // Zero exponent
    assert(evaluateExpression(7, '^', 0) == 1.0);

    // Zero numerator and denominator (valid division 0/3 = 0)
    assert(evaluateExpression(0, '/', 3) == 0.0);
}
#include <cmath>
#include <limits>

// Evaluate integer operands with a given operator.
// Returns NaN for division by zero or unsupported operators.
double evaluateExpression(int a, char op, int b) {
    const double NaN = std::numeric_limits<double>::quiet_NaN();
    switch (op) {
        case '+':
            return static_cast<double>(a) + b;
        case '-':
            return static_cast<double>(a) - b;
        case '*':
            return static_cast<double>(a) * b;
        case '/':
            if (b == 0) {
                return NaN;
            }
            return static_cast<double>(a) / b;
        case '^':
            return std::pow(static_cast<double>(a), static_cast<double>(b));
        default:
            return NaN;
    }
}
// The solution is straightforward: a single function that examines the operator character using a chain of `if`‑`else` statements or a `switch`. For each valid operator, compute and return the corresponding result as a `double`. For division, check for a zero denominator before performing the division to avoid undefined behavior; return `NaN` if the denominator is zero. For exponentiation, use `std::pow(a, b)` which accepts integers and returns a double. For any unrecognized operator, return `NaN` to signal invalid input. The time complexity is O(1) because the work is constant regardless of input size, and the space complexity is O(1) since no additional data structures are used. Edge cases to consider: `b` can be negative for exponentiation (e.g., `2^-1 = 0.5`), which `std::pow` handles correctly; division with a negative numerator or denominator yields a double with sign as expected; and if the denominator is zero, we must return `NaN` without performing the division.
