/*
Write a C++ function that computes the integer power of a non-negative integer base raised to a non-negative integer exponent using recursion. The function must return the result as a `long long` to safely accommodate larger results (e.g., 10^10) that exceed the range of `int`. Handle the base case where the exponent is 0 (return 1), and for positive exponents, recursively compute `base * power(base, exponent - 1)`. Assume the inputs are non-negative, so no special handling for negative exponents is required. The function should be named `recursivePower` and take two parameters: the base and the exponent.
*/
#include <cstdint>

// Recursively compute base^exponent for non-negative inputs.
// Uses long long to accommodate larger results.
long long recursivePower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    return static_cast<long long>(base) * recursivePower(base, exponent - 1);
}
#include <cassert>

int main() {
    // Basic cases
    assert(recursivePower(2, 3) == 8);
    assert(recursivePower(5, 0) == 1);
    assert(recursivePower(0, 5) == 0);
    assert(recursivePower(0, 0) == 1);
    // Larger exponent and base
    assert(recursivePower(10, 4) == 10000);
    assert(recursivePower(3, 10) == 59049);
    // Edge: exponent 1
    assert(recursivePower(7, 1) == 7);
    // Larger base with moderate exponent
    assert(recursivePower(12, 6) == 2985984);
    // Another small case
    assert(recursivePower(4, 2) == 16);
    // Check a slightly bigger one
    assert(recursivePower(2, 20) == 1048576);
    return 0;
}
// The core algorithm is straightforward recursion: if the exponent is 0, return 1; otherwise, multiply the base by the result of the recursive call with exponent decreased by 1. This mirrors the mathematical definition of exponentiation. Important edge cases include exponent 0 (always returns 1, even for base 0), and base 0 with positive exponent (returns 0). Another subtle case: base 0 and exponent 0 returns 1, which is a common convention. Using `long long` avoids overflow for moderate inputs (e.g., up to 10^18 for base 10 and exponent 18), but very large exponents could still overflow. The time complexity is O(exponent) because there is exactly one recursive call per decrement of the exponent, and each call does constant work. The space complexity is also O(exponent) due to the recursion stack depth, which could be a limitation for large exponents (e.g., 10^6 would cause stack overflow). For typical teaching examples, this is acceptable.
