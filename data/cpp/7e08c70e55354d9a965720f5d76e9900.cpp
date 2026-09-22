/*
Write a C++ function called `checkMultiplication` that takes three double values `a`, `b`, and `c` as parameters. The function should return a boolean value indicating whether `c` equals the exact product of `a` and `b` (`a * b == c`). Additionally, write a second function called `formatResult` that takes the same three doubles and returns a string describing whether the answer was correct or incorrect. If incorrect, the string must include the correct product formatted with `std::to_string`. The program should repeatedly prompt the user for numbers in a loop (without using a main function in the solution) and terminate when the user enters "t" — however, for the standalone task, your solution should only contain the two free functions described, with no I/O logic.
*/
#include <string>
#include <cmath>

// Returns true iff c is exactly equal to a * b.
bool checkMultiplication(double a, double b, double c) {
    return a * b == c;
}

// Returns a human-readable string indicating if the multiplication was correct.
// If incorrect, includes the correct product using to_string.
std::string formatResult(double a, double b, double c) {
    const double product = a * b;
    if (product == c) {
        return "Wynik poprawny!";
    }
    return "Wynik niepoprawny! Poprawny wynik to " + std::to_string(product);
}
#include <cassert>
#include <string>
#include <cmath>

// Declare the functions (they would be in the solution file)
bool checkMultiplication(double a, double b, double c);
std::string formatResult(double a, double b, double c);

int main() {
    // Basic correct cases
    assert(checkMultiplication(2.0, 3.0, 6.0) == true);
    assert(formatResult(2.0, 3.0, 6.0) == "Wynik poprawny!");

    // Basic incorrect cases
    assert(checkMultiplication(2.0, 3.0, 7.0) == false);
    assert(formatResult(2.0, 3.0, 7.0) == "Wynik niepoprawny! Poprawny wynik to 6.000000");

    // Negative numbers
    assert(checkMultiplication(-4.0, 5.0, -20.0) == true);
    assert(checkMultiplication(-4.0, 5.0, 20.0) == false);

    // Zero
    assert(checkMultiplication(0.0, 5.0, 0.0) == true);
    assert(checkMultiplication(0.0, 5.0, 1.0) == false);

    // Fractional results
    assert(checkMultiplication(0.5, 0.2, 0.1) == true);
    assert(formatResult(0.5, 0.2, 0.1) == "Wynik poprawny!");

    // Very large numbers (potential overflow -> inf, but comparison still works)
    double huge = 1e308;
    assert(checkMultiplication(huge, huge, 1e616) == false); // product overflows to inf, not equal to c

    // NaN case: any comparison with NaN is false
    assert(checkMultiplication(1.0, 2.0, std::nan("")) == false);

    // Incorrect format check includes correct product
    assert(formatResult(3.0, 4.0, 11.0) == "Wynik niepoprawny! Poprawny wynik to 12.000000");

    return 0;
}
// The core algorithm is a direct comparison of `a * b` against `c` using `==`. However, floating-point arithmetic can introduce precision errors, so a more robust approach would compare the absolute difference against a small epsilon (e.g., `1e-9`). For the task, we can implement both a strict equality check and a format function that uses the same check. Edge cases include very large or very small numbers where the product might overflow or underflow, and cases where `c` is `NaN` or infinity — the equality check will naturally fail for `NaN`. Time complexity is \(O(1)\) and space complexity is \(O(1)\) for the check function, while the format function allocates a string of size proportional to the output length, but that is not dependent on input size. The solution functions are pure and do not perform any I/O, making them testable.
