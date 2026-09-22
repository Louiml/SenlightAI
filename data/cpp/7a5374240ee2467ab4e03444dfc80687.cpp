Write a C++ function `fibonacciValue(int n)` that returns the nth Fibonacci number, where the sequence is defined as F(0) = 0, F(1) = 1, and F(n) = F(n-1) + F(n-2) for n ≥ 2. The function must handle non-negative integers (n ≥ 0), and for n = 0 and n = 1 it should return 0 and 1 respectively. The implementation must be iterative (not recursive) to avoid exponential time complexity, and it must use constant auxiliary space (i.e., not allocate an array of size `n`). Ensure that the function works correctly for large `n` values that fit within `int` (up to around n = 46, where F(46) = 1836311903, the largest Fibonacci number that fits in a 32-bit signed int).

The solution uses a simple iterative algorithm with two variables that represent the two most recent Fibonacci numbers. We start with `a = 0` (F(0)) and `b = 1` (F(1)). If `n == 0`, return `a`. If `n == 1`, return `b`. For `n >= 2`, we loop from 2 to `n`, updating `next = a + b`, then shifting `a = b` and `b = next`. After the loop, `b` holds F(n). This runs in O(n) time and uses O(1) auxiliary space, making it significantly better than the recursive approach (which has exponential time) or using a full array (which uses O(n) space). Edge cases to handle: `n = 0` returns 0, `n = 1` returns 1, and very large `n` may cause integer overflow; the problem statement implicitly limits `n` to values where the result fits in `int`. No special handling is needed for negative `n` since the specification says non-negative.

#include <stdexcept>

// Returns the nth Fibonacci number (0-indexed: F(0)=0, F(1)=1).
// Precondition: n >= 0 and results must fit within int.
int fibonacciValue(int n) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }
    int a = 0; // F(0)
    int b = 1; // F(1)
    if (n == 0) {
        return a;
    }
    if (n == 1) {
        return b;
    }
    for (int i = 2; i <= n; ++i) {
        int next = a + b;
        a = b;
        b = next;
    }
    return b;
}

#include <cassert>

int main() {
    assert(fibonacciValue(0) == 0);
    assert(fibonacciValue(1) == 1);
    assert(fibonacciValue(2) == 1);
    assert(fibonacciValue(3) == 2);
    assert(fibonacciValue(4) == 3);
    assert(fibonacciValue(5) == 5);
    assert(fibonacciValue(6) == 8);
    assert(fibonacciValue(10) == 55);
    assert(fibonacciValue(20) == 6765);
    assert(fibonacciValue(46) == 1836311903);
}
