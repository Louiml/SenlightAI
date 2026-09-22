// Write a C++ function named `reverseIntegerWithOverflowCheck` that takes a 32-bit signed integer `input` and returns its decimal digits reversed as a 32-bit signed integer. If reversing the digits causes the result to fall outside the range of a 32-bit signed integer (`INT_MIN` to `INT_MAX`), the function must return 0. The function must handle negative numbers correctly (the sign is preserved, only the magnitude is reversed) and must also handle the special case where the input is `INT_MIN`, whose reversal would always overflow. The function should be a standalone free function, not part of a class, and must not use any external libraries beyond standard headers. The function must be `const`-correct for any helper variables it uses internally and must not modify its parameter.

#include <cassert>
#include <climits>

// Function under test (declared here for completeness; in practice include the solution)
int reverseIntegerWithOverflowCheck(int input);

int main() {
    // Basic positive case
    assert(reverseIntegerWithOverflowCheck(123) == 321);
    // Basic negative case
    assert(reverseIntegerWithOverflowCheck(-123) == -321);
    // Zero input
    assert(reverseIntegerWithOverflowCheck(0) == 0);
    // Single digit
    assert(reverseIntegerWithOverflowCheck(5) == 5);
    assert(reverseIntegerWithOverflowCheck(-5) == -5);
    // Trailing zeros in input are lost in reversal
    assert(reverseIntegerWithOverflowCheck(120) == 21);
    assert(reverseIntegerWithOverflowCheck(-120) == -21);
    // Overflow cases must return 0
    assert(reverseIntegerWithOverflowCheck(1534236469) == 0);
    assert(reverseIntegerWithOverflowCheck(INT_MAX) == 0); // 7463847412 overflows
    assert(reverseIntegerWithOverflowCheck(INT_MIN) == 0); // -8463847412 overflows
    // Boundary that fits
    assert(reverseIntegerWithOverflowCheck(1463847412) == 2147483641); // fits within INT_MAX
    assert(reverseIntegerWithOverflowCheck(-1463847412) == -2147483641); // fits within INT_MIN
    return 0;
}

#include <climits>

// Reverse the decimal digits of a 32-bit signed integer.
// Returns 0 if the reversed value overflows the 32-bit signed range.
int reverseIntegerWithOverflowCheck(int input) {
    long reversed = 0;
    while (input != 0) {
        reversed = reversed * 10 + input % 10;
        input /= 10;
    }
    // Check if the accumulated result fits in a 32-bit signed int
    if (reversed > INT_MAX || reversed < INT_MIN) {
        return 0;
    }
    return static_cast<int>(reversed);
}

// The core algorithm repeatedly extracts the last digit of the input using the modulo operator (`% 10`) and appends it to a running result by multiplying the current result by 10 and adding the digit. Then the input is divided by 10 to remove that last digit. This continues until the input becomes 0. The challenge is overflow detection: since the intermediate result may exceed `INT_MAX` or go below `INT_MIN` before the final check, we store the result in a `long` (which on typical platforms is 64-bit and can hold the reversed value of any 32-bit integer without overflow, because the maximum absolute value of a 32-bit integer is about 2.1 billion, and reversing that yields a value that still fits in a 64-bit `long`). After the loop, we check if the accumulated `long` value is within the range `[INT_MIN, INT_MAX]`; if not, return 0. Edge cases include: 
// - Input `0` returns `0`.
// - Positive and negative numbers: the modulo and division operations in C++ for negative numbers preserve the sign correctly (e.g., `-123 % 10` yields `-3`, and `-123 / 10` yields `-12`), so the same loop works for negatives without special handling.
// - Input `INT_MIN` (`-2147483648`): its reversal would be `-8463847412`, which clearly overflows, so the check catches it.
// - Input `1534236469`: its reversal is `9646324351`, which exceeds `INT_MAX`, so returns 0.
// The time complexity is O(d), where d is the number of digits in the input (at most 10 for a 32-bit integer), and the space complexity is O(1) auxiliary.
