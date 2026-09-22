Write a C++ function named `quadraticRoots` that takes three `float` parameters `a`, `b`, and `c` representing the coefficients of a quadratic equation \(ax^2 + bx + c = 0\). The function should return a `std::string` that describes the roots of the equation. For real, distinct roots, return a string in the format `"X1 = <value>, X2 = <value>"` with values formatted to two decimal places. For a double root, return `"X1 = X2 = <value>"`. For complex roots, return `"X1 = <real> + <imag>i, X2 = <real> - <imag>i"` where `<real>` and `<imag>` are formatted to two decimal places. The function must handle the case where `a == 0` by throwing a `std::invalid_argument` exception with message `"Not a quadratic equation"`. Use the discriminant \(D = b^2 - 4ac\) to determine the type of roots.

#include <cassert>
#include <string>
#include <stdexcept>

// The solution function is assumed to be declared above.
int main() {
    // Test distinct real roots: x^2 - 5x + 6 = 0 -> roots 2 and 3
    assert(quadraticRoots(1.0f, -5.0f, 6.0f) == "X1 = 3.00, X2 = 2.00");
    
    // Test double root: x^2 - 2x + 1 = 0 -> root 1
    assert(quadraticRoots(1.0f, -2.0f, 1.0f) == "X1 = X2 = 1.00");
    
    // Test complex roots: x^2 + 1 = 0 -> roots ±i
    assert(quadraticRoots(1.0f, 0.0f, 1.0f) == "X1 = 0.00 + 1.00i, X2 = 0.00 - 1.00i");
    
    // Test complex roots with real part: x^2 + 2x + 5 = 0 -> roots -1 ± 2i
    assert(quadraticRoots(1.0f, 2.0f, 5.0f) == "X1 = -1.00 + 2.00i, X2 = -1.00 - 2.00i");
    
    // Test a = 0 throws
    bool threw = false;
    try {
        quadraticRoots(0.0f, 2.0f, 3.0f);
    } catch (const std::invalid_argument& e) {
        threw = true;
    }
    assert(threw);
    
    // Test negative coefficients: -x^2 + 4x - 3 = 0 -> roots 1 and 3
    assert(quadraticRoots(-1.0f, 4.0f, -3.0f) == "X1 = 1.00, X2 = 3.00");
    
    // Test all zeros except a=1, b=0, c=0 -> double root 0
    assert(quadraticRoots(1.0f, 0.0f, 0.0f) == "X1 = X2 = 0.00");
    
    return 0;
}

#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Solve quadratic equation ax^2 + bx + c = 0 and return a formatted string of roots.
std::string quadraticRoots(float a, float b, float c) {
    if (a == 0.0f) {
        throw std::invalid_argument("Not a quadratic equation");
    }
    
    float discriminant = b * b - 4.0f * a * c;
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    if (discriminant > 0.0f) {
        float sqrtD = std::sqrt(discriminant);
        float x1 = (-b + sqrtD) / (2.0f * a);
        float x2 = (-b - sqrtD) / (2.0f * a);
        oss << "X1 = " << x1 << ", X2 = " << x2;
    } else if (discriminant == 0.0f) {
        float x = -b / (2.0f * a);
        oss << "X1 = X2 = " << x;
    } else {
        float realPart = -b / (2.0f * a);
        float imagPart = std::sqrt(-discriminant) / (2.0f * a);
        oss << "X1 = " << realPart << " + " << imagPart << "i, X2 = " << realPart << " - " << imagPart << "i";
    }
    
    return oss.str();
}

// The solution computes the discriminant \(D = b^2 - 4ac\). If \(a == 0\), the equation is linear, not quadratic, so throw `std::invalid_argument`. If \(D > 0\), there are two distinct real roots: \(x_1 = (-b + \sqrt{D})/(2a)\) and \(x_2 = (-b - \sqrt{D})/(2a)\). If \(D == 0\), there is one double root \(x = -b/(2a)\). If \(D < 0\), the roots are complex conjugates: the real part is \(-b/(2a)\), and the imaginary part is \(\sqrt{-D}/(2a)\). Format all output values to two decimal places using `std::fixed` and `std::setprecision(2)`. Edge cases include very large or very small coefficients, which may cause floating-point precision issues; use `float` for simplicity and match the original snippet. Time complexity is \(O(1)\) as only arithmetic operations and a single square root are performed; space complexity is \(O(1)\) apart from the returned string.
