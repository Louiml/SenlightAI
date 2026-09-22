// Write a standalone C++ function named `createCopyWithIncrementedReal` that accepts a constant reference to a custom `ComplexNumber` class (defined in the included header) and returns a new `ComplexNumber` object. The returned object must have the same imaginary part as the input, but its real part must be exactly one greater than the input’s real part. The original object must remain unchanged after the function call. Additionally, the function must handle the case where the real part is the maximum possible integer value (`INT_MAX`) by setting the returned real part to `INT_MIN` (simulating overflow behavior), while still preserving the imaginary part. The function should be `const`-correct, not modify its argument, and be usable in a constant-expression context where possible.
#include <cassert>
#include <climits>

int main() {
    // Test normal increment
    ComplexNumber a(2, 4);
    ComplexNumber result1 = createCopyWithIncrementedReal(a);
    assert(result1.getReal() == 3);
    assert(result1.getImag() == 4);
    // Original unchanged
    assert(a.getReal() == 2);
    assert(a.getImag() == 4);

    // Test negative real
    ComplexNumber b(-5, 7);
    ComplexNumber result2 = createCopyWithIncrementedReal(b);
    assert(result2.getReal() == -4);
    assert(result2.getImag() == 7);

    // Test zero real
    ComplexNumber c(0, -3);
    ComplexNumber result3 = createCopyWithIncrementedReal(c);
    assert(result3.getReal() == 1);
    assert(result3.getImag() == -3);

    // Test INT_MAX overflow
    ComplexNumber d(INT_MAX, 10);
    ComplexNumber result4 = createCopyWithIncrementedReal(d);
    assert(result4.getReal() == INT_MIN);
    assert(result4.getImag() == 10);

    // Test const-correctness (calling on a const object is fine)
    const ComplexNumber e(5, 6);
    ComplexNumber result5 = createCopyWithIncrementedReal(e);
    assert(result5.getReal() == 6);
    assert(result5.getImag() == 6);
}
#include <climits>

class ComplexNumber {
private:
    int real;
    int imag;
public:
    ComplexNumber(int r, int i) : real(r), imag(i) {}
    
    int getReal() const { return real; }
    int getImag() const { return imag; }
};

// Returns a new ComplexNumber with real part incremented by 1 (with overflow handling) and same imag part.
ComplexNumber createCopyWithIncrementedReal(const ComplexNumber& c) {
    int newReal;
    if (c.getReal() == INT_MAX) {
        newReal = INT_MIN;
    } else {
        newReal = c.getReal() + 1;
    }
    return ComplexNumber(newReal, c.getImag());
}
// The solution requires accessing the private members of the `ComplexNumber` class, which are `real` and `imag`. Since the task provides a class definition but the function is free-standing, we need a way to read these values. The simplest approach is to add public getter methods (`getReal()` and `getImag()`) to the class (as part of the solution setup) or use a friend function. In the reference solution, we will define the class with getters and a constructor for convenience. The main algorithm: create a new `ComplexNumber` object using its constructor with `real + 1` (handling overflow by checking `if (real == INT_MAX) newReal = INT_MIN; else newReal = real + 1;`) and the same `imag`. The function returns this new object by value. Edge cases: `INT_MAX` causes integer overflow if we naively add 1, so we explicitly handle it. The input object is never modified because we only read its values. Time complexity is O(1) and space complexity is O(1) for the returned object. The function must be declared const-correct by taking `const ComplexNumber&` and returning a `ComplexNumber` (non-const) since a new object is created.
