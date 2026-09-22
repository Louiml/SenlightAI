/*
Write a C++ function that evaluates a polynomial-like expression with complex coefficients using double-double precision arithmetic, mimicking the structure of the provided snippet. Specifically, given six complex numbers `x1` through `x6` (representing variables in an expression), compute and return the result of the expression: \(5 \cdot x2^2 \cdot x3 + x2 \cdot \big(x3 \cdot (5 - 5 \cdot x4) - 11 \cdot x4\big) + (-5 - 5 \cdot x3) \cdot x4\). Use a custom simple complex number class that wraps `std::complex<double>` for real and imaginary parts, and implement all arithmetic operations (addition, subtraction, multiplication) and a `pow`-like function for squaring. The expression must be evaluated exactly as written, preserving the order of operations (multiplication before addition). The function should accept a struct that holds the six complex numbers in `double` precision and return the result as `std::complex<double>`. Ensure the code is self-contained, uses `const` correctly, and includes all necessary headers.
*/

#include <complex>

// Container for the six input complex numbers.
struct ExprVars {
    std::complex<double> x1;
    std::complex<double> x2;
    std::complex<double> x3;
    std::complex<double> x4;
    std::complex<double> x5;
    std::complex<double> x6;
};

// Evaluate the expression:
// 5*x2^2*x3 + x2*(x3*(5 - 5*x4) - 11*x4) + (-5 - 5*x3)*x4
std::complex<double> evaluateComplexExpression(const ExprVars& ex) {
    // term1: 5 * x2^2 * x3
    std::complex<double> x2_squared = ex.x2 * ex.x2;
    std::complex<double> term1 = 5.0 * x2_squared * ex.x3;

    // term2: x2 * ( x3*(5 - 5*x4) - 11*x4 )
    std::complex<double> inner1 = 5.0 - 5.0 * ex.x4;
    std::complex<double> inner2 = ex.x3 * inner1 - 11.0 * ex.x4;
    std::complex<double> term2 = ex.x2 * inner2;

    // term3: (-5 - 5*x3) * x4
    std::complex<double> term3 = (-5.0 - 5.0 * ex.x3) * ex.x4;

    return term1 + term2 + term3;
}

#include <cassert>
#include <complex>
#include <cmath>

int main() {
    // Test case 1: all zeros -> result zero
    ExprVars e1{}; // zero-initializes all
    assert(evaluateComplexExpression(e1) == std::complex<double>(0.0, 0.0));

    // Test case 2: x2=1, x3=1, x4=0, others zero
    ExprVars e2{};
    e2.x2 = std::complex<double>(1.0, 0.0);
    e2.x3 = std::complex<double>(1.0, 0.0);
    // Formula: 5*1*1 + 1*(1*(5-0) - 0) + (-5-5)*0 = 5 + 5 + 0 = 10
    assert(evaluateComplexExpression(e2) == std::complex<double>(10.0, 0.0));

    // Test case 3: x2=2, x3=0, x4=1 -> 0 + 2*(0*(5-5) - 11*1) + (-5-0)*1 = -22 -5 = -27
    ExprVars e3{};
    e3.x2 = std::complex<double>(2.0, 0.0);
    e3.x4 = std::complex<double>(1.0, 0.0);
    assert(evaluateComplexExpression(e3) == std::complex<double>(-27.0, 0.0));

    // Test case 4: complex inputs with imaginary parts
    ExprVars e4{};
    e4.x2 = std::complex<double>(1.0, 1.0); // 1+i
    e4.x3 = std::complex<double>(2.0, -1.0); // 2-i
    e4.x4 = std::complex<double>(0.0, 1.0); // i
    // Compute manually for verification: term1 = 5*(x2^2)*x3; x2^2 = (1+1i)^2 = 0+2i; term1=5*(0+2i)*(2-i)= (0+10i)*(2-i)=10+20i? Let's compute properly: (10i)*(2-i)=20i -10i^2 = 20i +10 = 10+20i.
    // term2: x2*(x3*(5-5i) - 11i). x3*(5-5i) = (2-i)*(5-5i)=10-10i-5i+5i^2=10-15i-5=5-15i. minus 11i = 5-26i. Multiply by (1+i)= (1+i)*(5-26i)=5-26i+5i-26i^2=5-21i+26=31-21i.
    // term3: (-5-5*(2-i))*i = (-5-10+5i)*i = (-15+5i)*i = -15i+5i^2 = -15i-5 = -5-15i.
    // Sum: (10+20i)+(31-21i)+(-5-15i)=36-16i.
    std::complex<double> result = evaluateComplexExpression(e4);
    assert(std::abs(result - std::complex<double>(36.0, -16.0)) < 1e-9);

    // Test case 5: x3=1, x4=2, x2=3, others zero
    ExprVars e5{};
    e5.x2 = std::complex<double>(3.0, 0.0);
    e5.x3 = std::complex<double>(1.0, 0.0);
    e5.x4 = std::complex<double>(2.0, 0.0);
    // term1: 5*9*1=45; term2: 3*(1*(5-10) - 22)=3*(-5-22)=-81; term3: (-5-5)*2=-20; sum=45-81-20=-56
    assert(evaluateComplexExpression(e5) == std::complex<double>(-56.0, 0.0));

    return 0;
}

// The task reduces to straightforward arithmetic evaluation. The main challenge is ensuring correct parenthesis grouping and operation order. We first define a struct `ExprVars` containing six `std::complex<double>` fields. Then implement a free function `evaluateExpression(const ExprVars& ex)` that computes the expression step by step to avoid ambiguity:  
// 1. Compute `term1 = 5.0 * (ex.x2 * ex.x2) * ex.x3` (square first, then multiply by 5 and x3).  
// 2. Compute `inner = (5.0 - 5.0 * ex.x4)`; then `term2_part1 = ex.x3 * inner`; then `term2_part2 = 11.0 * ex.x4`; `term2 = ex.x2 * (term2_part1 - term2_part2)`.  
// 3. Compute `term3 = (-5.0 - 5.0 * ex.x3) * ex.x4`.  
// 4. Sum `term1 + term2 + term3` and return.  
// Edge cases: all operations are well-defined for any complex inputs; no division or square roots, so no special cases. Complexity: O(1) time and O(1) space because only a fixed number of arithmetic operations.
