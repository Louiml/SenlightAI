Write a C++ function that takes a fraction represented by two integers (numerator and denominator) and returns the fraction in its simplest reduced form as a pair of integers. The input may contain negative numerators or denominators, zero numerators, and you must ensure the denominator is never zero. The function should compute the greatest common divisor (GCD) of the numerator and denominator, divide both by it, and return the reduced fraction. Handle the case where the numerator is zero (the reduced fraction should be 0/1), and ensure the denominator is positive in the output (e.g., if both numerator and denominator are negative, the result should have a positive denominator).
The main algorithm is straightforward: compute the GCD of the absolute values of the numerator and denominator using the Euclidean algorithm, then divide both by this GCD. Special edge cases include a denominator of zero (which should be handled by throwing an exception or returning a sentinel value—for this task, assume valid input with non-zero denominator), and reducing a fraction where the numerator is zero, which yields 0/1. To ensure the denominator is positive, if the denominator is negative, multiply both numerator and denominator by -1. The time complexity is O(log(min(|a|,|b|))) for the Euclidean algorithm, and space complexity is O(1). No floating-point arithmetic is involved, so precision is exact.
#include <cstdlib>
#include <utility>
#include <stdexcept>

// Compute the greatest common divisor of two non-negative integers.
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Reduce a fraction to its simplest form.
// Precondition: denominator != 0.
// Returns a pair (numerator, denominator) in reduced form with a positive denominator.
std::pair<long long, long long> reduceFraction(long long numerator, long long denominator) {
    if (denominator == 0) {
        throw std::invalid_argument("Denominator cannot be zero.");
    }
    
    // Normalize sign: ensure denominator is positive.
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
    
    if (numerator == 0) {
        return {0, 1};
    }
    
    long long divisor = gcd(std::llabs(numerator), std::llabs(denominator));
    numerator /= divisor;
    denominator /= divisor;
    
    return {numerator, denominator};
}
#include <cassert>
#include <utility>

int main() {
    // Basic reduction
    auto r1 = reduceFraction(4, 8);
    assert(r1.first == 1 && r1.second == 2);
    
    // Negative numerator
    auto r2 = reduceFraction(-6, 4);
    assert(r2.first == -3 && r2.second == 2);
    
    // Negative denominator
    auto r3 = reduceFraction(3, -9);
    assert(r3.first == -1 && r3.second == 3);
    
    // Both negative
    auto r4 = reduceFraction(-10, -5);
    assert(r4.first == 2 && r4.second == 1);
    
    // Zero numerator
    auto r5 = reduceFraction(0, 7);
    assert(r5.first == 0 && r5.second == 1);
    
    // Already reduced
    auto r6 = reduceFraction(5, 7);
    assert(r6.first == 5 && r6.second == 7);
    
    // Large values
    auto r7 = reduceFraction(123456789, 987654321);
    // GCD of these is 9, so result is 13717421/109739369
    assert(r7.first == 13717421 && r7.second == 109739369);
    
    // Numerator equal to denominator (except zero)
    auto r8 = reduceFraction(4, 4);
    assert(r8.first == 1 && r8.second == 1);
    
    // Numerator is negative zero? Not typical, but ensure works
    auto r9 = reduceFraction(0, -5);
    assert(r9.first == 0 && r9.second == 1);
}
