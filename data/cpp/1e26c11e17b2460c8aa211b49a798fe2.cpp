// Write a C++ function named `basicCalculator` that takes three parameters: two `double` operands and a `char` operator (`+`, `-`, `*`, or `/`), and returns a `string` representing the result in the format `"a op b = result"` with the result formatted to two decimal places (e.g., `"5 + 3 = 8.00"`). If the operator is invalid, return `"Invalid operator"`. If the operator is `/` and the second operand is zero, return `"Division by zero"`. The function must be `const`-correct, handle negative numbers, and not print anything to the console (return the string instead).
// The solution uses a `switch` statement on the operator character. For `+`, `-`, and `*`, compute the result directly. For `/`, check if the divisor is zero; if so, return an error string, otherwise compute the division. For any other character, return an invalid-operator message. Use `std::ostringstream` (or `std::to_string` combined with formatting) to produce the output string with exactly two decimal places, e.g., via `std::fixed` and `std::setprecision(2)`. Edge cases include negative numbers (handled naturally by floating-point arithmetic), zero divisor, and invalid operators. Time complexity is O(1), as it performs a fixed number of operations regardless of input. Space complexity is O(1) for computation, plus the size of the returned string.
#include <string>
#include <sstream>
#include <iomanip>

// Perform a basic arithmetic operation and return the formatted result.
// Returns an error message for invalid operators or division by zero.
std::string basicCalculator(double a, char op, double b) {
    double result = 0.0;
    bool valid = true;
    bool error = false;

    switch (op) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b != 0.0) {
                result = a / b;
            } else {
                error = true;
            }
            break;
        default:
            valid = false;
            break;
    }

    if (!valid) {
        return "Invalid operator";
    }
    if (error) {
        return "Division by zero";
    }

    std::ostringstream out;
    out << std::fixed << std::setprecision(2);
    out << a << " " << op << " " << b << " = " << result;
    return out.str();
}
#include <cassert>
#include <string>
#include <cmath>

int main() {
    assert(basicCalculator(5, '+', 3) == "5.00 + 3.00 = 8.00");
    assert(basicCalculator(10, '-', 4) == "10.00 - 4.00 = 6.00");
    assert(basicCalculator(6, '*', 7) == "6.00 * 7.00 = 42.00");
    assert(basicCalculator(8, '/', 2) == "8.00 / 2.00 = 4.00");
    assert(basicCalculator(9, '/', 0) == "Division by zero");
    assert(basicCalculator(3, '%', 2) == "Invalid operator");
    assert(basicCalculator(-5, '+', 2) == "-5.00 + 2.00 = -3.00");
    assert(basicCalculator(-6, '/', -3) == "-6.00 / -3.00 = 2.00");
    assert(basicCalculator(2, '*', -4) == "2.00 * -4.00 = -8.00");
    // Floating-point tolerance check for a division result
    std::string res = basicCalculator(1, '/', 3);
    assert(res.compare(0, 16, "1.00 / 3.00 = 0.") == 0);
}
