Write a C++ function named `isUglyNumber` that takes an integer `n` as input and returns `true` if `n` is an "ugly number," and `false` otherwise. An ugly number is a positive integer whose prime factors are limited to 2, 3, and 5. For this task, the number 1 is considered ugly (since it has no prime factors). Negative numbers and zero are not ugly. The function should handle all possible integer inputs, including very large values, without using extra data structures beyond simple variables. The function must be efficient and avoid recursion, using iterative division instead.

The solution repeatedly divides the input by the prime factors 2, 3, and 5 until it can no longer be divided by any of them. If the final result equals 1, then the original number's prime factors are exclusively 2, 3, and 5, making it ugly; otherwise, it is not. Edge cases include: `n = 0` → return `false` (since 0 is not positive); `n = 1` → return `true` (special case); negative numbers → return `false` immediately (since ugly numbers are positive). The algorithm works by first checking for non-positive inputs, then looping to divide by 2 as many times as possible, then by 3, then by 5 (order does not matter). After all divisions, compare the remaining value to 1. Time complexity is O(log n) because each division reduces the number by a constant factor, and space complexity is O(1) since only a few integer variables are used. The logic is robust for large integers because division is performed directly on the input.

#include <cstdint>

// Returns true if the positive integer n has only prime factors 2, 3, and 5.
// Zero and negative numbers are not considered ugly. The number 1 is ugly.
bool isUglyNumber(int64_t n) {
    // Ugly numbers are positive, so zero and negatives are not ugly.
    if (n <= 0) {
        return false;
    }
    
    // Divide by 2 as many times as possible.
    while (n % 2 == 0) {
        n /= 2;
    }
    // Divide by 3 as many times as possible.
    while (n % 3 == 0) {
        n /= 3;
    }
    // Divide by 5 as many times as possible.
    while (n % 5 == 0) {
        n /= 5;
    }
    
    // If after removing all 2,3,5 factors we are left with 1, it is ugly.
    return n == 1;
}

#include <cassert>

int main() {
    // Basic cases
    assert(isUglyNumber(1) == true);
    assert(isUglyNumber(2) == true);
    assert(isUglyNumber(3) == true);
    assert(isUglyNumber(5) == true);
    assert(isUglyNumber(6) == true);
    assert(isUglyNumber(8) == true);
    assert(isUglyNumber(14) == false);
    assert(isUglyNumber(25) == true);
    assert(isUglyNumber(30) == true);
    
    // Edge cases: zero and negatives
    assert(isUglyNumber(0) == false);
    assert(isUglyNumber(-1) == false);
    assert(isUglyNumber(-6) == false);
    
    // Larger numbers
    assert(isUglyNumber(100) == true);       // 2^2 * 5^2
    assert(isUglyNumber(101) == false);      // prime 101
    assert(isUglyNumber(1024) == true);      // 2^10
    assert(isUglyNumber(900) == true);       // 2^2 * 3^2 * 5^2
    assert(isUglyNumber(1000) == true);      // 2^3 * 5^3
    assert(isUglyNumber(121) == false);      // 11^2
    
    // Very large value that is ugly (2^31 = 2147483648)
    assert(isUglyNumber(2147483648LL) == true);
    // Very large value that is not ugly (prime 97 * 2^30)
    assert(isUglyNumber(104152956928LL) == false); // 97 * 2^30
    
    return 0;
}
