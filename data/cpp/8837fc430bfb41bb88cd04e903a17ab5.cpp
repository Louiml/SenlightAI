Write a C++ function `Polynomial<int> derivative(const Polynomial<int>& p)` that returns the derivative of a polynomial represented by the provided `Polynomial<int>` class. The derivative of a polynomial \(a_n x^n + a_{n-1} x^{n-1} + \dots + a_1 x + a_0\) is \(n a_n x^{n-1} + (n-1) a_{n-1} x^{n-2} + \dots + a_1\). Handle the zero polynomial (degree −1) by returning the zero polynomial. The function should preserve the internal invariant that zero coefficients are not stored, and it must work for any polynomial, including constant polynomials (degree 0) whose derivative is the zero polynomial. Use only the public interface of `Polynomial<int>` (constructors, `Degree()`, `operator[]`, iteration, and arithmetic operators if needed) to construct the result, relying on the class’s own zero‑removal logic.
int main() {
    // Test 1: Zero polynomial -> zero polynomial
    Polynomial<int> zero;
    assert(derivative(zero).Degree() == -1);
    assert(derivative(zero).operator[](0) == 0);

    // Test 2: Constant polynomial -> zero polynomial
    Polynomial<int> constant(5);
    assert(derivative(constant).Degree() == -1);
    assert(derivative(constant).operator[](0) == 0);

    // Test 3: Linear polynomial 3x + 2 -> derivative 3 (constant polynomial)
    std::vector<int> linear = {2, 3}; // 2 + 3x
    Polynomial<int> p_linear(linear);
    Polynomial<int> d_linear = derivative(p_linear);
    assert(d_linear.Degree() == 0);
    assert(d_linear.operator[](0) == 3);

    // Test 4: Quadratic polynomial 4x^2 + 3x + 2 -> derivative 8x + 3
    std::vector<int> quad = {2, 3, 4}; // 2 + 3x + 4x^2
    Polynomial<int> p_quad(quad);
    Polynomial<int> d_quad = derivative(p_quad);
    assert(d_quad.Degree() == 1);
    assert(d_quad.operator[](0) == 3);
    assert(d_quad.operator[](1) == 8);

    // Test 5: Polynomial with missing middle terms: 5x^4 - 2x^2 + 7
    // derivative: 20x^3 - 4x
    std::vector<int> sparse = {7, 0, -2, 0, 5}; // 7 - 2x^2 + 5x^4
    Polynomial<int> p_sparse(sparse);
    Polynomial<int> d_sparse = derivative(p_sparse);
    assert(d_sparse.Degree() == 3);
    assert(d_sparse.operator[](0) == 0); // no constant term
    assert(d_sparse.operator[](1) == -4);
    assert(d_sparse.operator[](2) == 0);
    assert(d_sparse.operator[](3) == 20);

    // Test 6: Verify no zero coefficients are stored in derivative result
    for (auto it = d_sparse.begin(); it != d_sparse.end(); ++it) {
        assert(it->second != 0);
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Compute the derivative of a polynomial with integer coefficients.
// Returns a new Polynomial<int> representing d/dx of the input.
Polynomial<int> derivative(const Polynomial<int>& p) {
    // For the zero polynomial or a constant, derivative is the zero polynomial.
    int deg = p.Degree();
    if (deg < 0) {
        return Polynomial<int>(); // zero polynomial
    }

    // The derivative's degree is at most original degree - 1.
    // Use a vector where index = power, value = coefficient.
    std::vector<int> derivativeCoef(deg, 0); // size = deg, because max power is deg-1
    for (auto it = p.begin(); it != p.end(); ++it) {
        int power = it->first;
        int coef = it->second;
        if (power > 0) {
            derivativeCoef[power - 1] = coef * power;
        }
        // power == 0 contributes nothing (derivative of constant is 0)
    }

    // Construct Polynomial from vector; it discards zeros internally.
    return Polynomial<int>(derivativeCoef);
}
// The derivative of a polynomial can be computed by iterating over all stored nonzero coefficients. For each term with power \(k > 0\) and coefficient \(c\), the derivative contributes a term with power \(k-1\) and coefficient \(c \cdot k\). Terms with power 0 vanish because their derivative is zero. The implementation can construct a `Polynomial<int>` from a `std::vector<int>` where the vector’s index represents the power; the constructor automatically ignores coefficients equal to zero, so we only need to fill in the derivative coefficients. For the zero polynomial and constant polynomials, the resulting vector is empty or all zero, yielding the zero polynomial. Time complexity is \(O(d)\) where \(d\) is the degree of the input, because we must examine each nonzero term once. Space complexity is \(O(d)\) for the temporary vector and the resulting polynomial. Edge cases: the zero polynomial (degree −1) returns zero polynomial; a constant polynomial (degree 0) returns zero polynomial; a monomial \(c x^k\) returns \(c \cdot k x^{k-1}\). Since `Polynomial<int>` provides `rbegin()`/`rend()` for reverse iteration but we can also use `begin()`/`end()` and just check `power > 0`. The `operator[]` is const and returns `T(0)` for missing powers, but iterating over the map ensures we only see stored nonzero coefficients.
