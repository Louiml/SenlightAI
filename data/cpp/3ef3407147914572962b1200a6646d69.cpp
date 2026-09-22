/*
Create a C++ function that takes two objects of a custom `Rational` class, each representing a fraction with a numerator and a denominator (both integers, with a non-zero denominator), and returns a new `Rational` object that is the sum of the two input fractions. The sum must be reduced to its simplest form (e.g., 2/4 becomes 1/2), and the denominator of the result must always be positive. Assume inputs are already valid (denominator non-zero), but this function should handle both positive and negative numerators and denominators. The class must have a private `numerator` and `denominator`, a constructor with default values (0/1), and a method `display()` that prints "num/den" (and if the denominator is 1, just prints the numerator). The addition operator should be implemented as a friend function named `operator+`. You only need to provide the implementation of the operator function, not the entire class definition, but ensure it uses proper `const` and accesses private members via friendship.
*/

#include <numeric>   // for std::gcd
#include <cstdlib>   // for std::abs

// Forward declaration of the Rational class (to be defined in user's code)
class Rational;

// Friend function to overload + operator for Rational class
// Assumes Rational has private members: int numerator, int denominator
// and a public constructor Rational(int num = 0, int den = 1)
Rational operator+(const Rational& n1, const Rational& n2) {
    // Compute raw sum
    int newNum = n1.numerator * n2.denominator + n2.numerator * n1.denominator;
    int newDen = n1.denominator * n2.denominator;

    // Reduce by GCD of absolute values
    int gcd = std::gcd(std::abs(newNum), std::abs(newDen));
    if (gcd != 0) {
        newNum /= gcd;
        newDen /= gcd;
    }

    // Ensure denominator is positive
    if (newDen < 0) {
        newNum = -newNum;
        newDen = -newDen;
    }

    return Rational(newNum, newDen);
}

#include <cassert>
#include <sstream>

// Minimal Rational class definition for testing (must match the task's expected interface)
class Rational {
private:
    int numerator;
    int denominator;
public:
    Rational(int num = 0, int den = 1) : numerator(num), denominator(den) {}
    friend Rational operator+(const Rational&, const Rational&);

    // Helper to compare for testing
    bool equals(int num, int den) const {
        return numerator == num && denominator == den;
    }
};

// Include the solution function here (or link it)
Rational operator+(const Rational& n1, const Rational& n2) {
    int newNum = n1.numerator * n2.denominator + n2.numerator * n1.denominator;
    int newDen = n1.denominator * n2.denominator;
    int gcd = std::gcd(std::abs(newNum), std::abs(newDen));
    if (gcd != 0) {
        newNum /= gcd;
        newDen /= gcd;
    }
    if (newDen < 0) {
        newNum = -newNum;
        newDen = -newDen;
    }
    return Rational(newNum, newDen);
}

int main() {
    // Test 1: Simple addition 1/2 + 1/3 = 5/6
    Rational r1(1, 2), r2(1, 3);
    Rational sum = r1 + r2;
    assert(sum.equals(5, 6));

    // Test 2: Reduce result 1/4 + 1/4 = 1/2 (not 2/4)
    Rational r3(1, 4), r4(1, 4);
    sum = r3 + r4;
    assert(sum.equals(1, 2));

    // Test 3: Negative denominator input 1/-2 + 1/2 = 0/1
    Rational r5(1, -2), r6(1, 2);
    sum = r5 + r6;
    assert(sum.equals(0, 1));

    // Test 4: Both negative -> negative result -1/3 + -1/6 = -1/2
    Rational r7(-1, 3), r8(-1, 6);
    sum = r7 + r8;
    assert(sum.equals(-1, 2));

    // Test 5: Whole numbers 5/1 + 3/1 = 8/1
    Rational r9(5, 1), r10(3, 1);
    sum = r9 + r10;
    assert(sum.equals(8, 1));

    // Test 6: Zero plus something 0/7 + 2/3 = 2/3
    Rational r11(0, 7), r12(2, 3);
    sum = r11 + r12;
    assert(sum.equals(2, 3));

    // Test 7: Large reduction 100/200 + 50/100 = 1/1
    Rational r13(100, 200), r14(50, 100);
    sum = r13 + r14;
    assert(sum.equals(1, 1));

    // Test 8: Negative denominator normalization (1/-3 + 1/-3 = -2/3)
    Rational r15(1, -3), r16(1, -3);
    sum = r15 + r16;
    assert(sum.equals(-2, 3));

    return 0;
}

// The solution must implement `operator+` for a `Rational` class. The main algorithm:  
// 1. Compute the sum numerator as `n1.numerator * n2.denominator + n2.numerator * n1.denominator`.  
// 2. Compute the sum denominator as `n1.denominator * n2.denominator`.  
// 3. Reduce the fraction by dividing both by their greatest common divisor (GCD) computed using the Euclidean algorithm (using `std::gcd` from `<numeric>` or a manual loop).  
// 4. Ensure the denominator is positive: if the denominator is negative, multiply both numerator and denominator by -1.  
// 5. Return a new `Rational` object with the simplified numerator and denominator.  
//
// Edge cases:  
// - Inputs may have negative denominators (e.g., 1/-2). The function must normalize the sign.  
// - Zero numerators: result is 0/1.  
// - Large numbers: use `long long` or `int`? Since the original snippet uses `int`, we'll stay with `int`, but note potential overflow; for simplicity, assume values fit in `int`.  
// - The GCD of absolute values to avoid negative GCD.  
//
// Time complexity: O(log(min(a,b))) for GCD calculation, space O(1).
