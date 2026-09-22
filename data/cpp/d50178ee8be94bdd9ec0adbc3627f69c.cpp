Given two positive integers `a` and `b`, write a C++ function that computes the product of all integers from 1 up to the minimum of `a` and `b` (i.e., the factorial of `min(a, b)`). The function should accept two `long long` integers and return a `long long` (assume the result fits in 64-bit, i.e., `min(a, b) <= 20`). The function must be named `minFactorial`, be `const`-correct, and not use any global variables or macros. Handle edge cases where either input is 0 or 1 gracefully (factorial of 0 and 1 are both 1).
// The core idea is simple: compute `min(a, b)` first, then compute the factorial of that value using an iterative loop. Factorial of `n` is `1 * 2 * ... * n`. For `n = 0` or `n = 1`, the result is 1. Since we restrict inputs to fit in 64-bit, we can use a `long long` accumulator initialized to 1 and multiply by each integer from 2 up to `min(a, b)`. This avoids overflow for small values (up to 20!). Time complexity is O(min(a,b)) and space complexity is O(1). Edge cases: if either input is 0, then min is 0 and result is 1; if both are positive, just iterate. If inputs are equal, the result is the factorial of that number.
#include <algorithm>   // for std::min

// Return the factorial of the smaller of two non-negative integers.
// Assumes min(a, b) <= 20 so that result fits in long long.
long long minFactorial(long long a, long long b) {
    long long n = std::min(a, b);
    long long result = 1;
    for (long long i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}
#include <cassert>

int main() {
    assert(minFactorial(5, 10) == 120);          // 5! = 120
    assert(minFactorial(10, 5) == 120);          // symmetric
    assert(minFactorial(1, 1) == 1);             // 1! = 1
    assert(minFactorial(0, 7) == 1);             // 0! = 1
    assert(minFactorial(7, 0) == 1);
    assert(minFactorial(3, 3) == 6);             // 3! = 6
    assert(minFactorial(20, 20) == 2432902008176640000LL); // 20! fits
    assert(minFactorial(20, 1) == 1);
    assert(minFactorial(4, 6) == 24);
    assert(minFactorial(2, 100) == 2);           // 2! = 2
}
