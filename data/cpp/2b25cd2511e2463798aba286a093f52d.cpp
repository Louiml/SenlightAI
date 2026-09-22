/*
Write a C++ function that takes two non-zero integers as parameters and returns a string indicating whether the first integer is a multiple of the second, or the second is a multiple of the first. If either integer divides the other evenly, the function should return "Sao Multiplos" (Portuguese for "They are multiples"); otherwise it should return "Nao sao Multiplos" (Portuguese for "They are not multiples"). The function must handle both positive and negative integers, and the order of the parameters should not affect the result—the function only checks if one is a multiple of the other, not which one is larger. The function should be named `areMultiples` and accept two `int` parameters by value.
*/
#include <string>

// Returns "Sao Multiplos" if either a is a multiple of b or b is a multiple of a,
// otherwise "Nao sao Multiplos". Both parameters must be non-zero.
std::string areMultiples(const int a, const int b) {
    if (a % b == 0 || b % a == 0) {
        return "Sao Multiplos";
    }
    return "Nao sao Multiplos";
}
#include <cassert>
#include <string>

// Free function declared here for the test (in a real scenario, it would be included from a header)
std::string areMultiples(const int a, const int b) {
    if (a % b == 0 || b % a == 0) {
        return "Sao Multiplos";
    }
    return "Nao sao Multiplos";
}

int main() {
    // Basic case: first is a multiple of second
    assert(areMultiples(6, 2) == "Sao Multiplos");
    // Basic case: second is a multiple of first
    assert(areMultiples(2, 6) == "Sao Multiplos");
    // Negative numbers
    assert(areMultiples(-10, 5) == "Sao Multiplos");
    assert(areMultiples(7, -3) == "Nao sao Multiplos");
    // Equal numbers
    assert(areMultiples(8, 8) == "Sao Multiplos");
    // Non-multiples
    assert(areMultiples(15, 4) == "Nao sao Multiplos");
    // One is 1 (always a multiple)
    assert(areMultiples(7, 1) == "Sao Multiplos");
    // Large values
    assert(areMultiples(1000000, 1000) == "Sao Multiplos");
    assert(areMultiples(999999, 2) == "Nao sao Multiplos");
    // Negative and positive with exact divisibility
    assert(areMultiples(-12, 4) == "Sao Multiplos");
    assert(areMultiples(5, -5) == "Sao Multiplos");
    return 0;
}
// The solution must check divisibility in both directions without assuming which number is larger. A direct approach is to use the modulus operator: if either `a % b == 0` or `b % a == 0`, they are multiples. However, the modulus operator with negative numbers in C++ returns a remainder with the sign of the dividend, but for divisibility, `a % b == 0` is still valid because if `a` is a multiple of `b`, the remainder is zero regardless of sign. Edge cases: the input integers are guaranteed non-zero, so no division by zero occurs. Also, both numbers could be equal (e.g., `5` and `5`), where `a % b == 0` is true, so they are multiples. Negative numbers like `-6` and `3` work because `-6 % 3 == 0`. Time complexity is \(O(1)\) with constant space, as the function performs only a couple of modulus operations and comparisons.
