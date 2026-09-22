// Write a C++ function named `isUglyNumber` that takes a single integer `n` and returns `true` if `n` is an ugly number, or `false` otherwise. An ugly number is a positive integer whose prime factors are limited to 2, 3, and 5. By convention, 1 is considered ugly. The function must correctly handle all integer inputs, including zero and negative numbers, which are not considered ugly. The function signature should be `bool isUglyNumber(int n)`.

// The solution repeatedly divides the number by 2, 3, and 5 while it is divisible by each, using a loop for each prime factor. Because these operations are distributive, any factor of 2, 3, or 5 is stripped away. If after removing all such factors the remaining value is 1, then the original number consisted only of those primes and is ugly; otherwise, it contains some other prime factor and is not ugly. Edge cases: `n == 0` must be handled first because dividing zero leads to undefined behavior (modulo by zero). Negative numbers are not positive, so they are immediately false. The number 1 is ugly and is handled as the base case. The time complexity is O(log n) because each division reduces the number significantly; more precisely, the number of divisions is at most the sum of the exponents of 2, 3, and 5 in the prime factorization. Space complexity is O(1) as only a few integer variables are used.

#include <cstdbool>

// Determine whether a given integer is an ugly number.
// Ugly numbers are positive integers whose prime factors are only 2, 3, and 5.
// 1 is considered ugly. Zero and negative numbers are not ugly.
bool isUglyNumber(int n) {
    if (n <= 0) {
        return false;
    }
    if (n == 1) {
        return true;
    }
    
    while (n % 2 == 0) {
        n /= 2;
    }
    while (n % 3 == 0) {
        n /= 3;
    }
    while (n % 5 == 0) {
        n /= 5;
    }
    
    return n == 1;
}

#include <cassert>

int main() {
    // Basic positive examples
    assert(isUglyNumber(1) == true);
    assert(isUglyNumber(2) == true);
    assert(isUglyNumber(3) == true);
    assert(isUglyNumber(5) == true);
    assert(isUglyNumber(6) == true);   // 2 * 3
    assert(isUglyNumber(8) == true);   // 2^3
    assert(isUglyNumber(15) == true);  // 3 * 5
    assert(isUglyNumber(30) == true);  // 2 * 3 * 5

    // Non-ugly positives
    assert(isUglyNumber(7) == false);
    assert(isUglyNumber(14) == false);
    assert(isUglyNumber(49) == false);

    // Edge cases: zero and negative
    assert(isUglyNumber(0) == false);
    assert(isUglyNumber(-1) == false);
    assert(isUglyNumber(-6) == false);

    // Larger ugly number
    assert(isUglyNumber(200) == true);  // 2^3 * 5^2

    // Mixed with a prime larger than 5
    assert(isUglyNumber(22) == false);  // 2 * 11

    return 0;
}
