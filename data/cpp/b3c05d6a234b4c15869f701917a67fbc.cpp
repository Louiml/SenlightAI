Write a C++ function `reverseDigits(int n)` that takes a non-negative integer and returns its digits reversed as a new integer. For example, `12345` becomes `54321`, and `100` becomes `1` (leading zeros are dropped because the result is a standard integer). The function must handle the special case `n == 0` correctly by returning `0`. The function should use only integer arithmetic, without converting to a string. If the reversed value would exceed the range of a 32-bit signed integer, the behavior is undefined (you may assume the input is chosen so that the reversed value fits). Provide a solution with proper `const` correctness where applicable.

#include <cassert>

int main() {
    assert(reverseDigits(0) == 0);
    assert(reverseDigits(1) == 1);
    assert(reverseDigits(12) == 21);
    assert(reverseDigits(12345) == 54321);
    assert(reverseDigits(100) == 1);
    assert(reverseDigits(1000) == 1);
    assert(reverseDigits(1200) == 21);
    assert(reverseDigits(987654321) == 123456789); // 987654321 reversed = 123456789, fits
    return 0;
}

#include <cstdint>

// Reverse the decimal digits of a non-negative integer.
// Returns the reversed integer; for input 0, returns 0.
// Assumes the reversed value fits in a 32-bit signed integer.
int reverseDigits(int n) {
    int reversed = 0;
    while (n != 0) {
        int digit = n % 10;
        reversed = reversed * 10 + digit;
        n /= 10;
    }
    return reversed;
}

// The main algorithm is a straightforward digit extraction and reconstruction loop: repeatedly take the last digit of `n` using `n % 10`, append it to the result by multiplying the current result by 10 and adding the digit, then discard the last digit using integer division `n / 10`. Continue until `n` becomes 0. Edge cases: if `n` is 0, the loop does not execute and the initial result value must be 0, so initialize the result to 0. For numbers ending with zeros (e.g., 100), the loop processes the zero digits first, but they contribute leading zeros to the result, which are mathematically insignificant (e.g., `100` → first digit 0, result = 0*10+0=0, second digit 0 → result = 0, third digit 1 → result = 1). This matches expected behavior. The time complexity is \(O(d)\) where \(d\) is the number of digits in `n` (at most 10 for a 32-bit int). The space complexity is \(O(1)\) auxiliary, since only a few integer variables are used.
