Write a standalone C++ function named `factorialOf` that takes a single non-negative integer `n` as input and returns its factorial as a `long` value, ensuring correct handling of the special case `0! = 1` and guarding against invalid negative inputs by returning `1` for them (or alternatively, you may choose to return `-1` for negative inputs, but specify your choice). The function must not read from standard input or produce any output; it should be purely computational, using a loop (not recursion) and proper `const` correctness for the parameter. The function should be self-contained with appropriate headers, and the implementation must avoid overflow by assuming the factorial fits within `long`, but you should document that assumption with a comment.
#include <cassert>

int main() {
    // Basic cases
    assert(factorialOf(0) == 1);
    assert(factorialOf(1) == 1);
    assert(factorialOf(2) == 2);
    assert(factorialOf(3) == 6);
    assert(factorialOf(4) == 24);
    assert(factorialOf(5) == 120);
    assert(factorialOf(10) == 3628800);
    // Edge case: negative input (returns 1 by design)
    assert(factorialOf(-1) == 1);
    assert(factorialOf(-100) == 1);
    // Larger valid value (still fits in long)
    assert(factorialOf(12) == 479001600);
    assert(factorialOf(15) == 1307674368000L);
    return 0;
}
#include <cstddef> // For std::size_t if needed, but not required

// Computes the factorial of a non-negative integer n.
// Returns 1 for n == 0 and for any negative n (as a safe default).
// Assumes the result fits into a long (i.e., n <= 20 on typical systems).
long factorialOf(const int n) {
    if (n <= 0) {
        return 1; // Handles n == 0 and negative inputs
    }
    long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}
// The factorial of a non-negative integer \(n\), denoted \(n!\), is defined as the product of all positive integers from 1 to \(n\), with the convention that \(0! = 1\). The algorithm iteratively multiplies a result variable initialized to `1` by each integer from 1 through `n`, inclusive. Edge cases: if `n == 0`, the loop does not execute, and the function returns `1`, which is correct. For negative inputs, factorial is mathematically undefined; the implementation returns `1` as a safe default (documented in comments). Time complexity is \(O(n)\) because exactly \(n\) multiplications are performed (or \(n\) iterations of the loop). Space complexity is \(O(1)\) because only a single accumulator variable and the loop counter are used, independent of input size. The use of `long` limits the valid range to approximately \(n \le 20\) on typical systems, beyond which overflow occurs; this is unavoidable with fixed-width types, so the task assumes inputs stay within that range.
