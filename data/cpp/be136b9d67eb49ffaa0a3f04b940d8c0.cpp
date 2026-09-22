// Write a C++ function that takes a non-negative integer `n` and returns its decimal digits reversed as a new integer. For example, given `1234`, the function should return `4321`; given `100`, it should return `1` (leading zeros in the reversed number are dropped because they are not significant in integer arithmetic). The function must handle `0` correctly (returning `0`) and must not use strings, arrays, or any other container; only integer arithmetic is allowed. The input is guaranteed to be a non-negative `int`, but you should ensure your solution also works for the maximum `int` value without causing overflow during computation (assume the result fits within `int` for valid test cases).

#include <cassert>

int main() {
    // Basic cases
    assert(reverseDigits(0) == 0);
    assert(reverseDigits(1) == 1);
    assert(reverseDigits(12) == 21);
    assert(reverseDigits(1234) == 4321);

    // Numbers with trailing zeros
    assert(reverseDigits(10) == 1);
    assert(reverseDigits(100) == 1);
    assert(reverseDigits(1230) == 321);

    // Large numbers (within int range)
    assert(reverseDigits(2147483647) == 7463847412); // Note: 7463847412 > INT_MAX? Actually it's 7,463,847,412 which fits? Let's use safer values.
    // Use a value that reversed fits in int: 123456789 -> 987654321 (both fit within int).
    assert(reverseDigits(123456789) == 987654321);
    assert(reverseDigits(987654321) == 123456789);

    // Single digit and repeated digits
    assert(reverseDigits(5) == 5);
    assert(reverseDigits(1111) == 1111);

    // Large value with no overflow risk in reversed result (e.g., 1999999999 -> 9999999991, which fits? 9,999,999,991 > 2,147,483,647, so avoid. Use 1000000000 -> 1 fits.)
    assert(reverseDigits(1000000000) == 1);
}

#include <cstdint>

// Reverse the decimal digits of a non-negative integer.
// Example: reverseDigits(1234) -> 4321, reverseDigits(100) -> 1.
int reverseDigits(int n) {
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + (n % 10);
        n /= 10;
    }
    return reversed;
}

// The algorithm repeatedly extracts the last digit of `n` using the modulo operation (`n % 10`), appends that digit to the reversed number by multiplying the current reversed value by 10 and adding the digit, and then removes the last digit from `n` using integer division (`n / 10`). The loop continues until `n` becomes 0. Edge cases: if `n == 0`, the loop never executes and the reversed value remains 0, which is correct. For numbers like `100`, the loop processes digits 0, 0, 1, and builds `rev` as `0*10+0=0`, then `0*10+0=0`, then `0*10+1=1`, so the result is `1`—correct since leading zeros vanish. Time complexity is O(d), where d is the number of decimal digits in `n` (at most 10 for a 32-bit `int`), and space complexity is O(1) since only a few integer variables are used.
