/*
Write a standalone C++ function named `subtractComplex` that accepts two complex numbers represented as pairs of integers (real and imaginary parts) and returns their difference as another complex number pair. The function should implement subtraction component-wise: the real part of the result is the first number's real part minus the second number's real part, and the imaginary part is similarly subtracted. Your function must not use any external complex-number library; instead, it should operate on a simple struct `Complex` with two `int` fields `real` and `imag`. The function should be `const`-correct (take parameters by `const&` where appropriate), should not modify the inputs, and should return a `Complex` by value. Also, include a helper `makeComplex` function (or constructor) to easily create `Complex` objects. The solution must be self-contained (only include necessary headers) and suitable for testing with `assert`.
*/
#include <utility> // for std::pair? Not needed, but we use custom struct.

// Represents a complex number with integer real and imaginary parts.
struct Complex {
    int real;
    int imag;
};

// Helper to construct a Complex object.
Complex makeComplex(int r, int i) {
    return Complex{r, i};
}

// Returns the difference (lhs - rhs) of two complex numbers.
// Subtraction is performed component-wise: (a1 - a2, b1 - b2).
Complex subtractComplex(const Complex& lhs, const Complex& rhs) {
    return Complex{lhs.real - rhs.real, lhs.imag - rhs.imag};
}
#include <cassert>

int main() {
    // Basic subtraction
    Complex a = makeComplex(4, 5);
    Complex b = makeComplex(3, 4);
    Complex c = subtractComplex(a, b);
    assert(c.real == 1 && c.imag == 1);

    // Negative results
    Complex d = makeComplex(-2, 7);
    Complex e = makeComplex(5, -3);
    Complex f = subtractComplex(d, e);
    assert(f.real == -7 && f.imag == 10);

    // Zero components
    Complex g = makeComplex(0, 0);
    Complex h = makeComplex(0, 0);
    Complex i = subtractComplex(g, h);
    assert(i.real == 0 && i.imag == 0);

    // Subtracting a complex from itself yields zero
    Complex j = makeComplex(10, -10);
    Complex k = subtractComplex(j, j);
    assert(k.real == 0 && k.imag == 0);

    // Large values (within int range)
    Complex l = makeComplex(2147483647, -2147483647);
    Complex m = makeComplex(-1, 1);
    Complex n = subtractComplex(l, m);
    assert(n.real == -2147483648 && n.imag == -2147483648);

    // Ensure inputs are not modified
    Complex p = makeComplex(8, 9);
    Complex q = makeComplex(2, 3);
    Complex r = subtractComplex(p, q);
    assert(p.real == 8 && p.imag == 9);
    assert(q.real == 2 && q.imag == 3);
    assert(r.real == 6 && r.imag == 6);

    return 0;
}
// The solution defines a `Complex` struct with public `int real` and `int imag` fields for simplicity. The key algorithm is direct component-wise subtraction: `result.real = lhs.real - rhs.real` and `result.imag = lhs.imag - rhs.imag`. There are no special edge cases because integer subtraction is well-defined for all `int` values (including negatives and zeros). The function takes two `const Complex&` parameters to avoid copying and to enforce non-modification, and returns a new `Complex` object. Since we only do constant-time arithmetic, both time complexity is O(1) and auxiliary space is O(1). The helper `makeComplex` simply aggregates initialization; it’s not strictly necessary but improves readability. The test harness uses `assert` to compare the returned struct’s fields against expected values using `==` on each component, since `Complex` doesn’t have an overloaded `operator==` (we could add one, but direct field comparison in tests is clearer and keeps the solution minimal).
