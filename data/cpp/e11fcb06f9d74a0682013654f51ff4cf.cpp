// Create a C++ class representing a rational number (fraction) with integer numerator and denominator, supporting addition, subtraction, multiplication, and division. Each arithmetic operation must return a new fraction in reduced form (simplified by dividing both terms by their greatest common divisor). Handle zero denominators by throwing an exception (e.g., `std::invalid_argument`) during construction or arithmetic. Implement an output stream operator `<<` that prints the fraction as `numerator` if the denominator is 1, otherwise as `numerator/denominator` (including negative signs appropriately, e.g., `-3/4`). Provide a default constructor that initializes to `0/1`. Your solution must be a class definition with methods and the appropriate overloaded operators; do not include a main function. The task is to write the complete class implementation.
// The main algorithm involves computing the greatest common divisor (GCD) of two integers using the Euclidean algorithm (iterative modulus method). For each arithmetic operation:
// - Addition: `(a*d + c*b) / (b*d)`.
// - Subtraction: `(a*d - c*b) / (b*d)`.
// - Multiplication: `(a*c) / (b*d)`.
// - Division: `(a*d) / (b*c)` (throw if `c == 0` because division by zero fraction).
// After computing the raw numerator and denominator, reduce by dividing both by `gcd(abs(numerator), abs(denominator))`. Ensure the denominator is always positive after reduction (if negative, multiply both numerator and denominator by -1). Edge cases: zero numerator (result becomes 0/1), denominator zero in construction (throw `std::invalid_argument`), division by zero numerator (throw), negative denominators from input (normalize during construction). Time complexity per operation is O(log(min(|numerator|, |denominator|))) due to GCD. Space complexity is O(1) auxiliary.
#include <iostream>
#include <stdexcept>
#include <cstdlib>  // for std::abs

class Fraction {
private:
    int numerator_;
    int denominator_;

    // Euclidean algorithm for GCD (non-negative)
    static int gcd(int a, int b) {
        a = std::abs(a);
        b = std::abs(b);
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Reduce the fraction to its lowest terms and ensure denominator > 0
    void reduce() {
        if (denominator_ == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int g = gcd(numerator_, denominator_);
        numerator_ /= g;
        denominator_ /= g;
    }

public:
    // Default constructor: 0/1
    Fraction() : numerator_(0), denominator_(1) {}

    // Parameterized constructor: normalizes denominator to be positive and reduces
    Fraction(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
        reduce();
    }

    // Addition
    Fraction operator+(const Fraction& other) const {
        int num = numerator_ * other.denominator_ + other.numerator_ * denominator_;
        int den = denominator_ * other.denominator_;
        Fraction result(num, den);
        return result;
    }

    // Subtraction
    Fraction operator-(const Fraction& other) const {
        int num = numerator_ * other.denominator_ - other.numerator_ * denominator_;
        int den = denominator_ * other.denominator_;
        Fraction result(num, den);
        return result;
    }

    // Multiplication
    Fraction operator*(const Fraction& other) const {
        int num = numerator_ * other.numerator_;
        int den = denominator_ * other.denominator_;
        Fraction result(num, den);
        return result;
    }

    // Division (throws if other.numerator_ == 0)
    Fraction operator/(const Fraction& other) const {
        if (other.numerator_ == 0) {
            throw std::invalid_argument("Division by zero fraction");
        }
        int num = numerator_ * other.denominator_;
        int den = denominator_ * other.numerator_;
        Fraction result(num, den);
        return result;
    }

    // Friend output operator
    friend std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
        if (frac.denominator_ == 1) {
            os << frac.numerator_;
        } else {
            os << frac.numerator_ << "/" << frac.denominator_;
        }
        return os;
    }
};
#include <cassert>
#include <sstream>

// Solution code is assumed to be included above.

int main() {
    // Basic addition
    Fraction a(1, 2), b(1, 3);
    Fraction sum = a + b;
    assert(sum == Fraction(5, 6));  // requires operator==, so let's check via stream output

    // But we don't have operator==, so use strings for verification
    std::ostringstream oss1;
    oss1 << (a + b);
    assert(oss1.str() == "5/6");

    // Subtraction
    std::ostringstream oss2;
    oss2 << (a - b);  // 1/2 - 1/3 = 1/6
    assert(oss2.str() == "1/6");

    // Multiplication
    std::ostringstream oss3;
    oss3 << (a * b);  // 1/2 * 1/3 = 1/6
    assert(oss3.str() == "1/6");

    // Division
    std::ostringstream oss4;
    oss4 << (a / b);  // 1/2 / 1/3 = 3/2
    assert(oss4.str() == "3/2");

    // Negative denominator normalization
    Fraction c(1, -2);
    std::ostringstream oss5;
    oss5 << c;
    assert(oss5.str() == "-1/2");

    // Zero numerator
    Fraction d(0, 5);
    std::ostringstream oss6;
    oss6 << d;
    assert(oss6.str() == "0");

    // Reduction: 4/8 -> 1/2
    Fraction e(4, 8);
    std::ostringstream oss7;
    oss7 << e;
    assert(oss7.str() == "1/2");

    // Integer denominator 1
    Fraction f(3, 1);
    std::ostringstream oss8;
    oss8 << f;
    assert(oss8.str() == "3");

    // Throwing on zero denominator construction
    bool threw = false;
    try {
        Fraction bad(1, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Throwing on division by zero numerator
    threw = false;
    try {
        Fraction g(1, 2);
        Fraction zero(0, 1);
        g / zero;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
