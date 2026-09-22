Write a C++ function that takes two positive integers and returns the number of positive divisors of their greatest common divisor (GCD). For example, if the inputs are 12 and 18, their GCD is 6, which has divisors 1, 2, 3, and 6, so the function should return 4. The function must compute the GCD using recursion (not `std::gcd`), then count divisors efficiently by iterating only up to the square root of the GCD. Handle the edge case where both numbers are equal and the GCD is a perfect square (e.g., 9 and 9) to avoid double-counting the square root.
The solution has two main parts: computing the GCD and counting its divisors. For the GCD, use the Euclidean algorithm recursively: `gcd(a, b) = gcd(b, a % b)` with the base case when `b == 0`, returning `a`. This handles all positive integer inputs in `O(log(min(a,b)))` time.

For counting divisors of the GCD `g`, observe that if `d` is a divisor of `g`, then `g/d` is also a divisor. So we only need to iterate `i` from 1 to `sqrt(g)`. For each `i` that divides `g`, we add 2 to the count (one for `i`, one for `g/i`). However, if `i*i == g`, then `i` and `g/i` are the same divisor, so we would have overcounted by 1. Thus, after the loop, if `i*i == g` (where `i` is the last checked value), subtract 1. Edge cases: when `g == 1`, the loop runs once (`i=1`), adds 2, then `i*i==1` subtracts 1, yielding 1 correct divisor. When one input is 1, the GCD is 1, answer is 1. Time complexity is `O(sqrt(g))` for the divisor count plus `O(log(min(a,b)))` for GCD; space complexity is `O(log(min(a,b)))` due to recursion stack.
#include <algorithm>

// Returns the number of positive divisors of the GCD of two positive integers.
int countDivisorsOfGCD(int a, int b) {
    // Recursive Euclidean algorithm to compute the greatest common divisor.
    auto gcd = [](int x, int y, auto& self) -> int {
        if (y == 0) {
            return x;
        }
        return self(y, x % y, self);
    };
    int g = gcd(std::max(a, b), std::min(a, b), gcd);

    // Count divisors by iterating up to the square root of g.
    int count = 0;
    int i = 1;
    for (; i * i <= g; ++i) {
        if (g % i == 0) {
            count += 2; // i and g/i are divisors.
        }
    }
    --i;
    if (i * i == g) {
        --count; // Avoid double-counting the square root.
    }
    return count;
}
#include <cassert>

int main() {
    // Basic cases
    assert(countDivisorsOfGCD(12, 18) == 4); // GCD=6, divisors: 1,2,3,6
    assert(countDivisorsOfGCD(8, 12) == 4);  // GCD=4, divisors: 1,2,4
    assert(countDivisorsOfGCD(7, 13) == 1);  // GCD=1, only divisor 1
    assert(countDivisorsOfGCD(36, 36) == 9); // GCD=36, divisors: 1,2,3,4,6,9,12,18,36
    // Perfect square GCD
    assert(countDivisorsOfGCD(9, 9) == 3);   // GCD=9, divisors: 1,3,9
    assert(countDivisorsOfGCD(16, 48) == 5); // GCD=16, divisors: 1,2,4,8,16
    // One input is 1
    assert(countDivisorsOfGCD(1, 100) == 1);
    assert(countDivisorsOfGCD(100, 1) == 1);
    // Larger numbers
    assert(countDivisorsOfGCD(100, 75) == 6); // GCD=25, divisors: 1,5,25
    assert(countDivisorsOfGCD(123456, 7890) == 4); // GCD=6, divisors: 1,2,3,6
    return 0;
}
