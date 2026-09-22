Write a C++ function `computeFactorial(int n)` that takes a non-negative integer `n` and returns its factorial as an `int`. The factorial of 0 and 1 is defined as 1. The function must handle the edge case where the input is negative by returning 1 (treating negative inputs as invalid but safe). Additionally, for inputs where the factorial exceeds the maximum value of `int` (e.g., 13! = 6227020800 > 2147483647), the result may overflow; to keep the task simple, assume the input `n` will be between 0 and 12 inclusive, but the function should still be robust and clearly document this limitation. Provide a free function (no `main`) with a descriptive name, proper `const` correctness for parameters (though `n` is passed by value, mark it `const`), and include necessary headers. The function must be recursive to mirror the given snippet, but you may also provide an iterative version if preferred; however, for this task, use recursion because it directly aligns with the original code.

// The solution uses recursion to compute the factorial: `factorial(n) = n * factorial(n-1)` with base case `factorial(0) = 1` and `factorial(1) = 1`. The recursion terminates when `n <= 1`, returning 1 immediately. Since the problem constrains `n` to be at most 12 to avoid integer overflow, no special overflow handling is needed beyond a comment. If `n` is negative, the function returns 1 to avoid infinite recursion, treating it as an invalid input in a benign way. Time complexity is O(n) because there are exactly `n` recursive calls, each doing constant work. Space complexity is O(n) due to the call stack depth, which is fine for `n <= 12`. Edge cases include `n = 0` (result 1), `n = 1` (result 1), and any negative `n` (result 1). The function is `const`-correct by marking the input parameter as `const int n` (though by-value, this is a stylistic choice) and the function itself is not modifying external state.

#include <cstdint>  // For potential future use; not strictly needed here but good practice

// Compute the factorial of a non-negative integer n recursively.
// Assumes n is between 0 and 12 inclusive to avoid integer overflow in 'int'.
// Returns 1 for n <= 1 and for negative inputs (treated as invalid but safe).
int computeFactorial(const int n) {
    if (n <= 1) {
        return 1;
    }
    return n * computeFactorial(n - 1);
}

#include <cassert>

int main() {
    // Base cases
    assert(computeFactorial(0) == 1);
    assert(computeFactorial(1) == 1);
    // Small positive numbers
    assert(computeFactorial(2) == 2);
    assert(computeFactorial(3) == 6);
    assert(computeFactorial(4) == 24);
    // Larger but still within safe range
    assert(computeFactorial(5) == 120);
    assert(computeFactorial(6) == 720);
    assert(computeFactorial(7) == 5040);
    assert(computeFactorial(8) == 40320);
    assert(computeFactorial(9) == 362880);
    assert(computeFactorial(10) == 3628800);
    assert(computeFactorial(12) == 479001600);
    // Edge case: negative input returns 1 (safe fallback)
    assert(computeFactorial(-1) == 1);
    assert(computeFactorial(-100) == 1);
    return 0;
}
