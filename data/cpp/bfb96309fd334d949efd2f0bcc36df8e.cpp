// Write a C++ function named `fastPower` that takes two integers, `base` and `exponent`, where `exponent` is guaranteed to be non-negative, and returns the result of `base` raised to the power `exponent` as an integer. The function must use the binary exponentiation technique (exponentiation by squaring) as shown in the snippet: if the current exponent is odd, multiply the running result by the current base and decrement the exponent; if even, square the current base and halve the exponent. You may assume that the result fits within the range of a 32-bit signed integer, and that `base` and `exponent` are both within `[-10^9, 10^9]` and `[0, 10^9]` respectively. Handle the edge case of `exponent == 0` by returning `1`. Do not use any built-in power functions or loops that count from 1 to exponent.
The solution uses binary exponentiation, which reduces the number of multiplications from O(exponent) to O(log exponent) by exploiting the binary representation of the exponent. At each step, if the least significant bit of the exponent is 1 (i.e., the exponent is odd), the result is multiplied by the current base and the exponent is reduced by 1. If the bit is 0 (exponent even), the base is squared and the exponent is halved. This works because `base^exp = (base^2)^(exp/2)` when exp is even, and `base^exp = base * base^(exp-1)` when exp is odd. Edge cases: exponent 0 returns 1 immediately (since any number to the power 0 is 1); base 0 with positive exponent returns 0; negative base works fine because multiplication handles sign naturally. The loop runs at most O(log exponent) iterations, each performing a constant number of operations, so time complexity is O(log exponent) and space complexity is O(1) (no extra data structures). Since the result fits in 32-bit int, we use `int` for the return type but cast intermediate multiplications to `long long` temporarily to avoid overflow during computation, then cast back.
#include <cstdint>

// Computes base^exponent using binary exponentiation (exponentiation by squaring).
// Precondition: exponent >= 0, result fits in int32_t.
int fastPower(int base, int exponent) {
    long long result = 1;
    long long currentBase = base;
    int exp = exponent;

    while (exp > 0) {
        if (exp & 1) {  // exponent is odd
            result *= currentBase;
            exp -= 1;
        } else {        // exponent is even
            currentBase *= currentBase;
            exp /= 2;
        }
    }

    return static_cast<int>(result);
}
#include <cassert>

int fastPower(int base, int exponent); // declaration for linking

int main() {
    // Basic cases
    assert(fastPower(2, 0) == 1);
    assert(fastPower(2, 1) == 2);
    assert(fastPower(2, 10) == 1024);
    assert(fastPower(3, 4) == 81);
    
    // Negative base with even and odd exponents
    assert(fastPower(-2, 3) == -8);
    assert(fastPower(-2, 4) == 16);
    
    // Base 0
    assert(fastPower(0, 5) == 0);
    assert(fastPower(0, 0) == 1);
    
    // Large exponent, but result fits in int (e.g., base=2, exp=30)
    assert(fastPower(2, 30) == 1073741824);
    
    // Base 1 or -1
    assert(fastPower(1, 1000000) == 1);
    assert(fastPower(-1, 999999) == -1);
    assert(fastPower(-1, 1000000) == 1);
    
    // Mixed even/odd exponent chains
    assert(fastPower(5, 7) == 78125);
    assert(fastPower(7, 5) == 16807);
    
    return 0;
}
