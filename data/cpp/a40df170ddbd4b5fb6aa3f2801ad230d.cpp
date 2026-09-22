// Write a C++ function named `classifyQuadraticRoots` that takes three integers `a`, `b`, and `c` representing the coefficients of a quadratic equation \( ax^2 + bx + c = 0 \). The function should return a string with two parts separated by a newline: the first line describes the nature of the roots as one of `"Real and Distinct"`, `"Real and Equal"`, or `"Imaginary"`. The second line should contain the roots (for real cases) formatted as `"r1 r2"` where `r1` is the smaller root and `r2` is the larger root. For imaginary roots, the second line should be empty (i.e., just the nature line). The roots must be computed using double precision, output with default formatting (e.g., `cout` default precision). Assume `a` is never zero. Handle edge cases where the discriminant is negative (output only `"Imaginary"`), zero (output equal roots), or positive (output both roots). The function should be `const`-correct and not modify inputs.
The solution computes the discriminant `D = b*b - 4*a*c`. Since `a`, `b`, `c` are integers, `D` may overflow if computed as `int`, so we compute it in `long long` to avoid overflow. Then we compare `D` to zero. If `D > 0`, we compute the two real roots using the quadratic formula: `r1 = (-b - sqrt(D)) / (2*a)` and `r2 = (-b + sqrt(D)) / (2*a)`. Order them so `r1 < r2`; note that the formula naturally gives the smaller root with the minus sign if `a > 0`, but the ordering can swap if `a < 0`. To be safe, we compute both and then swap if `r1 > r2`. For `D == 0`, we compute the single root `-b / (2*a)` and output it twice. For `D < 0`, we return only the nature line. We must use `double` for roots to avoid integer division issues (e.g., `-1*b/2*a` in the original code is wrong due to operator precedence: it computes `(-b/2) * a`). The time complexity is O(1), space O(1). Edge cases include very large coefficients (handled by `long long` for discriminant), negative `a` (root ordering), and zero discriminant (root duplication).
#include <string>
#include <cmath>
#include <algorithm>

// Classify roots of ax^2 + bx + c = 0 and return a formatted string.
std::string classifyQuadraticRoots(int a, int b, int c) {
    const long long discriminant = static_cast<long long>(b) * b - 4LL * a * c;
    
    if (discriminant > 0) {
        const double sqrtD = std::sqrt(static_cast<double>(discriminant));
        const double denom = 2.0 * a;
        double r1 = (-static_cast<double>(b) - sqrtD) / denom;
        double r2 = (-static_cast<double>(b) + sqrtD) / denom;
        if (r1 > r2) {
            std::swap(r1, r2);
        }
        return "Real and Distinct\n" + std::to_string(r1) + " " + std::to_string(r2);
    } else if (discriminant == 0) {
        const double root = -static_cast<double>(b) / (2.0 * a);
        return "Real and Equal\n" + std::to_string(root) + " " + std::to_string(root);
    } else {
        return "Imaginary\n";
    }
}
#include <cassert>
#include <string>
#include <cmath>

// Declare the solution function (if not already declared above)
std::string classifyQuadraticRoots(int a, int b, int c);

// Helper to compare floating point strings ignoring small formatting differences.
bool areRootsClose(const std::string& result, const std::string& expected) {
    // For simplicity, we'll expect exact string match because std::to_string produces consistent output.
    (void)result; (void)expected; // placeholder; actual comparison below
    return result == expected;
}

int main() {
    // x^2 - 5x + 6 = 0 -> roots 2 and 3
    assert(classifyQuadraticRoots(1, -5, 6) == "Real and Distinct\n2.000000 3.000000");
    // x^2 - 2x + 1 = 0 -> root 1
    assert(classifyQuadraticRoots(1, -2, 1) == "Real and Equal\n1.000000 1.000000");
    // x^2 + 1 = 0 -> imaginary
    assert(classifyQuadraticRoots(1, 0, 1) == "Imaginary\n");
    // -x^2 + 4x - 4 = 0 -> root 2 (still equal, note a is negative)
    assert(classifyQuadraticRoots(-1, 4, -4) == "Real and Equal\n2.000000 2.000000");
    // 2x^2 + 4x + 2 = 0 -> root -1 equal
    assert(classifyQuadraticRoots(2, 4, 2) == "Real and Equal\n-1.000000 -1.000000");
    // x^2 + 4x + 3 = 0 -> roots -3 and -1 (ordered)
    assert(classifyQuadraticRoots(1, 4, 3) == "Real and Distinct\n-3.000000 -1.000000");
    // Large coefficients: x^2 + 2000000000x + 1 = 0 (discriminant fits in long long)
    // but roots are extreme; just check nature line (we avoid exact string due to precision)
    std::string big = classifyQuadraticRoots(1, 2000000000, 1);
    assert(big.find("Real and Distinct") == 0);
    return 0;
}
