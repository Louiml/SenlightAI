// Write a C++ function that takes two fractions represented as pairs of integers (numerator and denominator) and returns their sum as a new pair of integers, reduced to lowest terms. The function should handle positive and negative numerators, positive denominators, and must never accept a zero denominator. It should use a helper function to compute the greatest common divisor (GCD) via the Euclidean algorithm. The result must ensure the denominator is positive (if the numerator is negative, keep the negative sign in the numerator). You may assume inputs are valid (non-zero denominators, no overflow in intermediate arithmetic for typical int ranges). The function signature should be `std::pair<int, int> addFractions(const std::pair<int, int>& f1, const std::pair<int, int>& f2)`.
#include <cassert>

int main() {
    // Basic positive fractions
    auto r1 = addFractions({1, 2}, {2, 3}); // 1/2 + 2/3 = 7/6
    assert(r1.first == 7 && r1.second == 6);

    // Sum resulting in whole number
    auto r2 = addFractions({1, 3}, {2, 3}); // 3/3 = 1
    assert(r2.first == 1 && r2.second == 1);

    // Negative numerator
    auto r3 = addFractions({-1, 4}, {1, 4}); // -1/4 + 1/4 = 0
    assert(r3.first == 0 && r3.second == 1);

    // Negative numerator result
    auto r4 = addFractions({-1, 2}, {-1, 3}); // -1/2 -1/3 = -5/6
    assert(r4.first == -5 && r4.second == 6);

    // Mixed signs
    auto r5 = addFractions({1, 2}, {-1, 3}); // 1/2 -1/3 = 1/6
    assert(r5.first == 1 && r5.second == 6);

    // Reduction needed
    auto r6 = addFractions({2, 6}, {3, 6}); // 1/3 + 1/2 = 5/6? Actually 2/6=1/3, 3/6=1/2, sum 5/6
    assert(r6.first == 5 && r6.second == 6);

    // Larger denominators
    auto r7 = addFractions({5, 6}, {7, 8}); // 5/6 +7/8 = (40+42)/48 = 82/48 = 41/24
    assert(r7.first == 41 && r7.second == 24);

    // Result can be negative zero? – handled
    auto r8 = addFractions({0, 1}, {5, 7}); // 0/1 + 5/7 = 5/7
    assert(r8.first == 5 && r8.second == 7);

    // Done
    return 0;
}
#include <utility>   // for std::pair
#include <cstdlib>   // for std::abs
#include <algorithm> // optional but not needed

// Helper: compute GCD of two integers (absolute values)
int findGCD(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a; // gcd, always non-negative
}

// Add two fractions given as pairs (numerator, denominator).
// Assumes denominators are non-zero. Returns reduced fraction with positive denominator.
std::pair<int, int> addFractions(const std::pair<int, int>& f1, const std::pair<int, int>& f2) {
    int num = f1.first * f2.second + f2.first * f1.second;
    int den = f1.second * f2.second;

    // Ensure denominator positive (though f1.second and f2.second are positive, den is positive)
    if (den < 0) {
        num = -num;
        den = -den;
    }

    if (num == 0) {
        return std::make_pair(0, 1);
    }

    int gcd = findGCD(num, den);
    return std::make_pair(num / gcd, den / gcd);
}
// The solution computes the sum of two fractions a/b and c/d as (a*d + c*b) / (b*d). Then it reduces the result by dividing both numerator and denominator by their greatest common divisor (GCD). The GCD is computed using the Euclidean algorithm, which works for negative numbers as long as we take absolute values. Edge cases: if the resulting numerator is zero, we simplify to 0/1. Also ensure the final denominator is positive: if the denominator becomes negative after reduction (which can happen if both denominators have opposite signs? No—since denominators are given positive, b*d is positive, so denominator stays positive). But if the numerator is negative, that’s fine; we keep the negative sign in numerator. For GCD, we use `std::abs` to handle negatives. Time complexity: O(log(min(a,b))) for Euclidean algorithm. Space complexity: O(1) auxiliary.
