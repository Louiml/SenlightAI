/*
Write a C++ function named `lcmOfPositiveInts` that accepts a count `n` followed by `n` positive integers as a variadic argument list (using `std::va_list`), and returns their least common multiple (LCM) as an `int`. The function must compute the LCM by first multiplying all numbers and then dividing by the greatest common divisor (GCD) of the entire set. The GCD of multiple numbers is computed iteratively: start with the first number as the current GCD, then for each subsequent number compute the GCD of the current GCD and that number using the Euclidean algorithm (with modulo operation and handling of zeros). The input is guaranteed to contain at least one positive integer, and all intermediate products and final LCM are guaranteed to fit within a 32-bit `int` (no overflow concerns). The function must not read from standard input; it must only process the variadic arguments. Provide a signature `int lcmOfPositiveInts(int n, ...)` with proper use of `<cstdarg>` and `<iostream>` only for potential debugging. The function must be const-correct where applicable. Handle the case `n == 1` by returning that single number directly (since LCM of one number is itself), and for `n >= 2` use the product / GCD approach.
*/

#include <cstdarg>

// Compute the greatest common divisor of two positive integers using Euclidean algorithm.
int gcdPositive(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Return the least common multiple of n positive integers passed as variadic arguments.
// Precondition: n >= 1 and all arguments are positive ints. The result fits in int.
int lcmOfPositiveInts(int n, ...) {
    va_list args;
    
    // Compute the product of all numbers.
    va_start(args, n);
    int product = 1;
    for (int i = 0; i < n; ++i) {
        int value = va_arg(args, int);
        product *= value;
    }
    va_end(args);
    
    // If there is only one number, its LCM is itself.
    if (n == 1) {
        return product;
    }
    
    // Compute the GCD of all numbers.
    va_start(args, n);
    int currentGcd = va_arg(args, int);
    for (int i = 1; i < n; ++i) {
        int nextValue = va_arg(args, int);
        currentGcd = gcdPositive(currentGcd, nextValue);
    }
    va_end(args);
    
    // LCM = product divided by the overall GCD.
    return product / currentGcd;
}

#include <cassert>

int main() {
    // Single number
    assert(lcmOfPositiveInts(1, 7) == 7);
    
    // Two numbers
    assert(lcmOfPositiveInts(2, 4, 6) == 12);
    
    // Three numbers where one divides the others
    assert(lcmOfPositiveInts(3, 6, 12, 24) == 24);
    
    // Five numbers with varying values from the snippet
    assert(lcmOfPositiveInts(5, 6, 14, 22, 11, 32) == 7392);
    
    // Six numbers from the snippet
    assert(lcmOfPositiveInts(6, 43, 14, 24, 53, 21, 78) == 21004248);
    
    // Coprime numbers
    assert(lcmOfPositiveInts(3, 3, 5, 7) == 105);
    
    // Repeated numbers
    assert(lcmOfPositiveInts(4, 5, 5, 5, 5) == 5);
    
    // Large but within int range
    assert(lcmOfPositiveInts(2, 12345, 67890) == 55884990);
    
    // Numbers where product is large but GCD reduces it
    assert(lcmOfPositiveInts(3, 100, 50, 25) == 100);
    
    return 0;
}

// The core algorithm involves two phases: (1) compute the product of all given integers, and (2) compute the GCD of the entire set, then divide the product by that GCD. For phase 2, the GCD is computed incrementally: initialize `currentGcd` with the first argument, then for each of the remaining `n-1` arguments, compute `currentGcd = gcd(currentGcd, nextValue)` using the Euclidean algorithm. The Euclidean algorithm handles positive integers; if either value is zero, the GCD is the other non-zero value; but since the problem specifies all inputs are positive, this edge case is trivial but should be handled robustly with a while-loop that swaps and uses modulo until one becomes zero. After computing the product and the overall GCD, the LCM is `product / overallGcd`. Time complexity is `O(n * log(maxValue))` because each GCD computation takes `O(log(min(a,b)))` due to the Euclidean algorithm, and there are `n-1` such computations plus a linear pass for the product. Space complexity is `O(1)` auxiliary because the variadic list is traversed twice (once for product, once for GCD) using a single `va_list` that is restarted with `va_start` after `va_end`; no extra storage is needed. Edge cases: `n == 1` should return that single number; `n == 0` is not allowed by the task specification. For `n >= 2`, the product is computed first, then the GCD is computed by reading the list again. Important: the variadic arguments are read sequentially; to read them twice, you must call `va_start` after `va_end` to reset the list. The function must be declared with `...` and use `va_arg` with `int` type.
