Write a C++ function that, given three integers `a`, `b`, and `c` representing the coefficients of a quadratic equation \(ax^2 + bx + c = 0\), returns a string describing the number of real roots and their values. The output must follow this exact format: if there are no real roots, return `"0"`; if there is exactly one real root, return `"1\n<root>"`; if there are two distinct real roots, return `"2\n<smaller_root> <larger_root>"`. All roots must be printed with fixed-point notation, showing the decimal point, and exactly 6 digits after the decimal point. You may assume `a` is non-zero. The function should compute the discriminant, handle the three cases (negative, zero, positive) with proper ordering of roots, and format the output using `std::fixed`, `std::showpoint`, and `std::setprecision(6)`. The solution must be implemented as a free function named `solveQuadratic`.

#include <cassert>
#include <string>

// Assume solveQuadratic is declared above in the same translation unit.

int main() {
    // Two distinct roots: x^2 - 5x + 6 => roots 2 and 3.
    assert(solveQuadratic(1, -5, 6) == "2\n2.000000 3.000000");

    // One repeated root: x^2 - 4x + 4 => root 2.
    assert(solveQuadratic(1, -4, 4) == "1\n2.000000");

    // No real roots: x^2 + x + 1.
    assert(solveQuadratic(1, 1, 1) == "0");

    // Negative a: -x^2 + 3x - 2 => roots 1 and 2.
    assert(solveQuadratic(-1, 3, -2) == "2\n1.000000 2.000000");

    // b=0: x^2 - 4 => roots -2 and 2.
    assert(solveQuadratic(1, 0, -4) == "2\n-2.000000 2.000000");

    // Single root with negative a: -x^2 + 2x - 1 => root 1.
    assert(solveQuadratic(-1, 2, -1) == "1\n1.000000");

    // Two roots where larger root is negative: x^2 - x - 2 => roots -1 and 2.
    assert(solveQuadratic(1, -1, -2) == "2\n-1.000000 2.000000");

    // Large coefficients: 10x^2 - 20x + 10 => root 1 (repeated).
    assert(solveQuadratic(10, -20, 10) == "1\n1.000000");

    // Roots with fractional results: 2x^2 - 5x + 2 => roots 0.5 and 2.
    assert(solveQuadratic(2, -5, 2) == "2\n0.500000 2.000000");

    // Negative b: x^2 + 2x - 3 => roots -3 and 1.
    assert(solveQuadratic(1, 2, -3) == "2\n-3.000000 1.000000");

    return 0;
}

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

// Solve quadratic equation ax^2 + bx + c = 0 and return formatted result.
std::string solveQuadratic(int a, int b, int c) {
    const int discriminant = b * b - 4 * a * c;
    std::ostringstream output;
    output << std::fixed << std::showpoint << std::setprecision(6);

    if (discriminant < 0) {
        output << 0;
    } else if (discriminant == 0) {
        const float root = -b / (2.0f * a);
        output << 1 << '\n' << root;
    } else {
        const float root1 = (-b - std::sqrt(static_cast<float>(discriminant))) / (2.0f * a);
        const float root2 = (-b + std::sqrt(static_cast<float>(discriminant))) / (2.0f * a);
        if (root1 < root2) {
            output << 2 << '\n' << root1 << ' ' << root2;
        } else {
            output << 2 << '\n' << root2 << ' ' << root1;
        }
    }

    return output.str();
}

// The solution begins by reading the three integer coefficients and computing the discriminant \(D = b^2 - 4ac\). If \(D < 0\), there are no real roots, so the function returns `"0"` followed by a newline. If \(D == 0\), there is one repeated real root, computed as \(-b / (2a)\) using floating-point arithmetic to avoid integer division; the output is `"1\n"` plus the formatted root. If \(D > 0\), compute the two distinct roots using the quadratic formula: \(\frac{-b - \sqrt{D}}{2a}\) and \(\frac{-b + \sqrt{D}}{2a}\). Since `a` may be negative, the order of `x1` and `x2` is not guaranteed, so after computing both, compare them and emit the smaller root first, followed by the larger root, both formatted with exactly 6 decimal places. Edge cases include `a` negative (which flips the ordering of roots), large coefficients that may overflow the squared term (though this is typical for an exercise and not specifically mitigated), and the case where `b` is zero, yielding symmetric roots. The formatting is handled by constructing a `std::ostringstream` and applying the manipulators before inserting numbers. The time complexity is \(O(1)\) because only arithmetic and square root operations are performed, and the space complexity is \(O(1)\), excluding the returned string.
