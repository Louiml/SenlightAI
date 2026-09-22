Write a C++ function named `computeOperation` that takes three arguments: two `double` values `x` and `y`, and a `char` operator (`+`, `-`, `*`, or `/`). The function must return a `std::string` describing the result in the exact format: for addition `"Suma = <result>"`, for subtraction `"Roznica = <result>"`, for multiplication `"Iloczyn = <result>"`, for division (when `y != 0`) `"Iloraz = <result>"`, for division by zero return `"Nie dzielimy przez zero"`, and for any invalid operator return `"Nie poprawny znak !"`. The result values must be printed with default floating-point precision (up to 6 decimal places) using `std::ostringstream`. The function must be `const`-correct, use no global variables, and handle negative numbers and edge cases like `y` very close to zero naturally without special floating‑point comparisons. You must not write a `main` function in the solution section; only the free function is required. The function should compute and format the result without any interactive I/O.

#include <cassert>
#include <string>

// Declare the function from the solution (for testing; in practice, it would be in a header).
std::string computeOperation(double x, double y, char op);

int main() {
    // Basic arithmetic
    assert(computeOperation(5.0, 3.0, '+') == "Suma = 8");
    assert(computeOperation(5.0, 3.0, '-') == "Roznica = 2");
    assert(computeOperation(5.0, 3.0, '*') == "Iloczyn = 15");
    assert(computeOperation(5.0, 3.0, '/') == "Iloraz = 1.66667"); // default precision
  
    // Division by zero
    assert(computeOperation(5.0, 0.0, '/') == "Nie dzielimy przez zero");
  
    // Invalid operator
    assert(computeOperation(5.0, 3.0, '%') == "Nie poprawny znak !");
  
    // Negative numbers
    assert(computeOperation(-4.0, 2.0, '+') == "Suma = -2");
    assert(computeOperation(-4.0, 2.0, '*') == "Iloczyn = -8");
  
    // Fractional results
    assert(computeOperation(2.0, 4.0, '/') == "Iloraz = 0.5");
  
    // Zero values for non-division ops
    assert(computeOperation(0.0, -5.0, '+') == "Suma = -5");
  
    // Double precision edge
    assert(computeOperation(1e308, 1e308, '*') == "Iloczyn = inf"); // overflow behavior
  
    return 0;
}

#include <string>
#include <sstream>

// Computes the arithmetic operation and returns a formatted result string.
std::string computeOperation(double x, double y, char op) {
    std::ostringstream oss;
    oss << x << " " << op << " " << y << " = ";
    
    switch (op) {
        case '+': {
            double result = x + y;
            oss << "Suma = " << result;
            break;
        }
        case '-': {
            double result = x - y;
            oss << "Roznica = " << result;
            break;
        }
        case '*': {
            double result = x * y;
            oss << "Iloczyn = " << result;
            break;
        }
        case '/': {
            if (y != 0.0) {
                double result = x / y;
                oss << "Iloraz = " << result;
            } else {
                return "Nie dzielimy przez zero";
            }
            break;
        }
        default:
            return "Nie poprawny znak !";
    }
    return oss.str();
}
*Note: The above solution returns the full expression for valid operators (e.g., "5 + 3 = Suma = 8"), but to match the original snippet's exact output (which only prints "Suma = 8"), I’ll adjust the solution to return only the operation label and result. The test section will expect only the label and number. For clarity, I’ll provide a corrected version below.*

[Corrected Solution]
#include <string>
#include <sstream>

// Computes the arithmetic operation and returns a formatted result string.
std::string computeOperation(double x, double y, char op) {
    if (op == '/') {
        if (y == 0.0) {
            return "Nie dzielimy przez zero";
        }
        std::ostringstream oss;
        oss << "Iloraz = " << (x / y);
        return oss.str();
    }
    
    std::ostringstream oss;
    switch (op) {
        case '+':
            oss << "Suma = " << (x + y);
            break;
        case '-':
            oss << "Roznica = " << (x - y);
            break;
        case '*':
            oss << "Iloczyn = " << (x * y);
            break;
        default:
            return "Nie poprawny znak !";
    }
    return oss.str();
}

// The core algorithm is a direct mapping from the input operator character to the arithmetic operation. For each valid operator, we compute the result and format it using a string stream with default precision (which is 6 significant digits). We must guard against division by zero explicitly by checking `y != 0.0` (for `double`, exact equality with zero is acceptable in this context because the user input is a literal; we do not need to handle floating-point epsilon comparisons for this simple task). For invalid operators, return the fixed error message. Edge cases include `y` being zero for division, or the operator being any character other than the four allowed. Because the function uses only constant-space local variables and a string stream, the time complexity is O(1) and space complexity is O(1) (aside from the output string). Using `double` instead of `float` avoids precision loss for typical inputs. The function should be `const`-correct by not modifying its parameters (they are passed by value, so they are inherently const within the function) and by using local constants where appropriate.
