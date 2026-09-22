/*
Write a C++ function named `reverseInteger` that takes a 32-bit signed integer `x` and returns its decimal digits reversed, while preserving the sign. If the reversed value falls outside the range of a 32-bit signed integer (`INT_MIN` to `INT_MAX`), the function must return `0`. The input may be negative, positive, or zero. The function must not use any 64-bit integer types for storing the final result (you may use `long` for intermediate overflow detection, but the return type must be `int`). Handle the case where the reversed number has leading zeros (e.g., `120` → `21`), and ensure that the reversal of `INT_MIN` or `INT_MAX` correctly returns `0` due to overflow.
*/
#include <string>
#include <algorithm>
#include <climits>

// Reverses the digits of a 32-bit signed integer, preserving sign.
// Returns 0 if the reversed value overflows the 32-bit int range.
int reverseInteger(int x) {
    std::string numStr = std::to_string(x);
    std::string reversedStr;
    bool isNegative = false;

    if (numStr[0] == '-') {
        isNegative = true;
        // Remove the sign character for reversal, then reattach later.
        reversedStr = numStr.substr(1);
    } else {
        reversedStr = numStr;
    }

    std::reverse(reversedStr.begin(), reversedStr.end());

    if (isNegative) {
        reversedStr.insert(reversedStr.begin(), '-');
    }

    // Use long to detect overflow beyond 32-bit int range.
    long result = std::stol(reversedStr);

    if (result > INT_MAX || result < INT_MIN) {
        return 0;
    }

    return static_cast<int>(result);
}
#include <cassert>
#include <climits>

int main() {
    // Basic positive numbers
    assert(reverseInteger(123) == 321);
    assert(reverseInteger(120) == 21);   // leading zero after reversal
    assert(reverseInteger(0) == 0);
    
    // Negative numbers
    assert(reverseInteger(-123) == -321);
    assert(reverseInteger(-120) == -21);
    
    // Overflow cases
    assert(reverseInteger(INT_MAX) == 0);        // 2147483647 -> 7463847412 (overflow)
    assert(reverseInteger(INT_MIN) == 0);        // -2147483648 -> -8463847412 (overflow)
    assert(reverseInteger(1534236469) == 0);     // 1534236469 -> 9646324351 (overflow)
    
    // Boundary values that just fit
    assert(reverseInteger(1463847412) == 2147483641);   // reversed within range
    assert(reverseInteger(-1463847412) == -2147483641); // reversed within range
    
    // Numbers ending with zeros
    assert(reverseInteger(100) == 1);
    assert(reverseInteger(-1000) == -1);
    
    return 0;
}
// The solution converts the integer to a string using `std::to_string`. If the number is negative, we strip the leading `'-'` sign, reverse the remaining digits, then reattach the sign. For positive numbers, we reverse the entire string directly. After reversal, we use `std::stol` to parse the string into a `long` (which can hold values beyond 32-bit range) so we can check for overflow against `INT_MAX` and `INT_MIN`. If the result is outside this range, we return `0`; otherwise, we cast it back to `int` and return. Edge cases include: `x = 0` (reversal yields "0" → returns `0`), numbers ending in zeros (e.g., `120` → "021" → parsed as `21`), negative numbers (sign preserved), and extreme values (`INT_MIN` = -2147483648 reverses to -8463847412, which overflows, so returns `0`; `INT_MAX` = 2147483647 reverses to 7463847412, which also overflows). Time complexity is \(O(d)\) where \(d\) is the number of decimal digits (at most 10 for 32-bit ints) due to string operations, and space complexity is \(O(d)\) for the temporary string. No loops over digits are needed, making this solution simple and readable.
