// Write a C++ function named `factorial` that takes a non-negative integer `n` and returns its factorial as a `double`. The factorial of `n` is defined as the product of all positive integers from 1 to `n`, with `0!` defined as 1. The function must handle the case where `n` is 0, and must use a loop (not recursion) to compute the product. Since the result can become very large, using `double` is acceptable for this exercise despite potential loss of precision; ensure the function accepts the input parameter as `int` and returns `double`. The function should not read from standard input or print to standard output; it must be a pure computational function ready to be called from test code.

#include <cassert>
#include <cmath>

int main() {
    assert(factorial(0) == 1.0);
    assert(factorial(1) == 1.0);
    assert(factorial(2) == 2.0);
    assert(factorial(3) == 6.0);
    assert(factorial(5) == 120.0);
    assert(factorial(10) == 3628800.0);
    // Check a larger value using a tolerance since double precision may vary.
    assert(std::fabs(factorial(20) - 2432902008176640000.0) < 1e6);
    return 0;
}

// Compute the factorial of a non-negative integer n as a double.
// Returns 1.0 for n == 0.
double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; ++i) {
        result *= static_cast<double>(i);
    }
    return result;
}

// The solution hinges on iterating from 1 up to the given integer `n`, multiplying an accumulator (initialized to `1.0`) by each integer in that range. This implements the definition of factorial directly. The primary edge case is `n = 0`, where the loop body never executes, leaving the accumulator as `1.0`, correctly returning `1`. Negative inputs are not valid per the specification, but the function can choose to return `1` or handle gracefully; for simplicity, assume the caller passes non-negative integers. Time complexity is \(O(n)\) because the loop runs exactly `n` iterations for positive `n`, and \(O(1)\) for `n = 0`. Auxiliary space is \(O(1)\) since only a single `double` accumulator is used. The use of `double` allows the function to store large values (up to about 170! before overflow to infinity), which is a trade-off for the exercise; however, precision for very large factorials will be approximate.
