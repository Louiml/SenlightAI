/*
Write a C++ function named `reverseInteger` that takes a 32-bit signed integer `x` and returns its decimal digits reversed as a 32-bit signed integer. If reversing the digits causes the result to overflow the 32-bit signed integer range (i.e., outside `[−2^31, 2^31−1]`), return `0`. The function must handle negative numbers correctly (e.g., `-123` reversed becomes `-321`), and must not use any helper libraries beyond the C++ standard library. The function should be `const`-correct and should not modify its input.
*/

#include <climits>  // for INT_MAX and INT_MIN

// Reverse the decimal digits of a 32-bit signed integer.
// Returns 0 if the reversed value overflows the 32-bit signed int range.
int reverseInteger(int x) {
    long reversed = 0;  // use long to safely detect overflow
    while (x != 0) {
        reversed = reversed * 10 + x % 10;  // append last digit
        x /= 10;                            // drop the last digit
    }
    if (reversed > INT_MAX || reversed < INT_MIN) {
        return 0;  // overflow condition
    }
    return static_cast<int>(reversed);
}

#include <cassert>

int main() {
    // Basic positive and negative cases
    assert(reverseInteger(123) == 321);
    assert(reverseInteger(-123) == -321);
    assert(reverseInteger(120) == 21);        // trailing zero dropped
    assert(reverseInteger(0) == 0);            // zero input
    assert(reverseInteger(1) == 1);            // single digit
    assert(reverseInteger(-1) == -1);

    // Overflow cases: reversed value exceeds 32-bit range
    assert(reverseInteger(1534236469) == 0);   // reversed = 9646324351 > INT_MAX
    assert(reverseInteger(-2147483648) == 0);  // reversed = -8463847412 < INT_MIN
    assert(reverseInteger(2147483647) == 0);   // reversed = 7463847412 > INT_MAX

    // Near-boundary but valid cases
    assert(reverseInteger(1000000003) == 3000000001);  // within int range
    assert(reverseInteger(-1000000003) == -3000000001);
}

// The core algorithm is to repeatedly extract the last digit of `x` using the modulo operator (`% 10`) and append it to a running result. Since the input may be negative, the modulo operation in C++ yields a negative remainder for negative dividends (e.g., `-123 % 10` gives `-3`), which conveniently preserves the sign when building the reversed number. The result is accumulated in a `long` variable to detect overflow before casting back to `int`. Each iteration updates the result as `result = result * 10 + x % 10` and reduces `x` by integer division (`x /= 10`). The loop continues until `x` becomes zero. After the loop, check if the accumulated `long` value is outside the `int` range using `INT_MAX` and `INT_MIN` from `<climits>`; if so, return `0`. Otherwise, cast the `long` to `int` and return it. Edge cases include: `x = 0` (loop does not execute, result stays `0`), positive numbers with trailing zeros (e.g., `120` reversed to `21` because the leading zero is dropped), and overflow cases like `x = 2147483647` (reversed exceeds `INT_MAX`). Time complexity is \(O(\log_{10}|x|)\) since the number of iterations equals the number of digits, and auxiliary space is \(O(1)\).
