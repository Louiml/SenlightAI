// Write a standalone C++ function that takes three `double` coefficients `a`, `b`, and `c` representing a quadratic equation \(ax^2 + bx + c = 0\) and returns a `std::string` describing the real or complex solutions. The function must handle three cases: two distinct real roots, one real double root, and two complex conjugate roots. For real roots, output them in ascending order (or in the standard order of the quadratic formula but ensure they are sorted). For complex roots, output them in the form `"realPart + imaginaryParti and realPart - imaginaryParti"`, with each part formatted to a default precision (e.g., using `std::to_string` or `std::ostringstream` without extra formatting). The function must gracefully handle the case where `a` is zero, returning an error message `"Not a quadratic equation"`. Use `std::numeric_limits<double>::epsilon()` to compare the discriminant to zero for robustness against floating-point precision issues, but for simplicity, you may compare directly with zero if you prefer. Ensure the function is `const`-correct and does not rely on any external input. The signature should be `std::string quadratic_solutions(double a, double b, double c)`.

The solution computes the discriminant \(D = b^2 - 4ac\). If `a == 0`, reject as non-quadratic. For `D > 0` (with a tolerance using `std::abs(D) > epsilon`), compute two real roots using the quadratic formula: \(x_1 = (-b + \sqrt{D})/(2a)\) and \(x_2 = (-b - \sqrt{D})/(2a)\). Sort these two roots to ensure ascending order (or simply output in the formula order but the problem asks for ascending). For `D == 0` (within tolerance), compute the single root \(-b/(2a)\) and output as double root. For `D < 0`, compute real part \(-b/(2a)\) and imaginary part \(\sqrt{-D}/(2a)\), then output as complex conjugates. The main edge case is `a == 0`, and additionally handling floating-point comparisons. Time complexity is \(O(1)\) and space complexity is \(O(1)\) (excluding the returned string).

#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

// Solves a quadratic equation ax^2 + bx + c = 0, returning a description of the solutions.
// Returns an error message if a == 0. For real roots, returns them in ascending order.
std::string quadratic_solutions(double a, double b, double c) {
    if (a == 0.0) {
        return "Not a quadratic equation";
    }

    const double discriminant = b * b - 4.0 * a * c;
    const double eps = 1e-12; // tolerance for floating-point comparisons

    if (std::abs(discriminant) <= eps) {
        // One real double root
        const double root = -b / (2.0 * a);
        std::ostringstream oss;
        oss << "One real root: " << root;
        return oss.str();
    } else if (discriminant > 0.0) {
        // Two distinct real roots
        const double sqrt_disc = std::sqrt(discriminant);
        double root1 = (-b + sqrt_disc) / (2.0 * a);
        double root2 = (-b - sqrt_disc) / (2.0 * a);
        if (root1 > root2) std::swap(root1, root2);
        std::ostringstream oss;
        oss << "Two real roots: " << root1 << " and " << root2;
        return oss.str();
    } else {
        // Complex conjugate roots
        const double real_part = -b / (2.0 * a);
        const double imag_part = std::sqrt(-discriminant) / (2.0 * a);
        std::ostringstream oss;
        oss << "Complex roots: " << real_part << " + " << imag_part << "i and "
            << real_part << " - " << imag_part << "i";
        return oss.str();
    }
}

#include <cassert>
#include <string>
#include <sstream>

// Placeholder for the solution function; in a real test environment, include the solution above.
std::string quadratic_solutions(double a, double b, double c);

int main() {
    // Case: two real roots (x^2 - 5x + 6 = 0 -> roots 2 and 3)
    std::string res1 = quadratic_solutions(1.0, -5.0, 6.0);
    assert(res1 == "Two real roots: 2 and 3");

    // Case: double root (x^2 - 4x + 4 = 0 -> root 2)
    std::string res2 = quadratic_solutions(1.0, -4.0, 4.0);
    assert(res2 == "One real root: 2");

    // Case: complex roots (x^2 + 1 = 0 -> 0 ± 1i)
    std::string res3 = quadratic_solutions(1.0, 0.0, 1.0);
    assert(res3 == "Complex roots: 0 + 1i and 0 - 1i");

    // Case: a = 0
    std::string res4 = quadratic_solutions(0.0, 2.0, 3.0);
    assert(res4 == "Not a quadratic equation");

    // Case: roots with negative values (2x^2 + 5x + 2 = 0 -> roots -2 and -0.5, sorted)
    std::string res5 = quadratic_solutions(2.0, 5.0, 2.0);
    assert(res5 == "Two real roots: -2 and -0.5");

    // Case: a negative leading coefficient (-x^2 + 4x - 4 = 0 -> double root 2)
    std::string res6 = quadratic_solutions(-1.0, 4.0, -4.0);
    assert(res6 == "One real root: 2");
}
