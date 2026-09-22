Write a C++ function `int reverseInteger(int x)` that returns the decimal digits of `x` in reverse order. For example, `reverseInteger(-832)` returns `-238`. If reversing the digits causes the result to overflow a 32-bit signed integer (`INT_MIN` to `INT_MAX`), the function must return `0`. The input is guaranteed to fit in a 32-bit signed integer, but the reversed value may not. Handle both positive and negative numbers, and ignore leading zeros naturally produced by reversal (e.g., `120` becomes `21`). The function must be declared with `const` correctness where appropriate and cannot use external libraries beyond the standard C++ headers.

// The core idea is to repeatedly extract the last digit of `x` using `x % 10` (which correctly yields negative digits for negative inputs when using C++'s truncation toward zero), append it to a result accumulator with `result = result * 10 + digit`, and then remove the last digit via integer division `x /= 10`. Continue until `x` becomes zero.  
//
// The critical edge case is overflow: before performing `result * 10 + digit`, we must check if the current `result` would exceed `INT_MAX` or fall below `INT_MIN` after the operation. A safe check uses `INT_MAX / 10` and `INT_MIN / 10` boundaries:  
// - If `result > INT_MAX / 10` or (`result == INT_MAX / 10` and `digit > 7`), the positive overflow occurs (since `INT_MAX % 10 == 7`).  
// - Similarly, if `result < INT_MIN / 10` or (`result == INT_MIN / 10` and `digit < -8`), negative overflow occurs (since `INT_MIN % 10 == -8`).  
//
// If overflow is detected, return `0` immediately. Otherwise, continue the loop. After the loop, `result` holds the reversed number, with sign automatically preserved because we operate with negative digits for negative inputs.  
//
// Time complexity is O(d) where d is the number of digits in `x` (max ~10 for 32-bit integers), so O(log n) in general. Space complexity is O(1) auxiliary. No extra data structures are used.

#include <climits>   // for INT_MAX, INT_MIN

// Reverse the decimal digits of a 32-bit signed integer.
// Returns 0 if the reversed value overflows a 32-bit signed integer.
int reverseInteger(int x) {
    int result = 0;
    while (x != 0) {
        int digit = x % 10;  // works for negative numbers too
        // Check for overflow before updating result
        if (result > INT_MAX / 10 || 
            (result == INT_MAX / 10 && digit > 7)) {
            return 0;
        }
        if (result < INT_MIN / 10 || 
            (result == INT_MIN / 10 && digit < -8)) {
            return 0;
        }
        result = result * 10 + digit;
        x /= 10;
    }
    return result;
}

#include <cassert>
#include <climits>

int main() {
    // Basic positive and negative cases
    assert(reverseInteger(123) == 321);
    assert(reverseInteger(-832) == -238);
    assert(reverseInteger(0) == 0);
    
    // Leading zeros in input produce trailing zeros in output
    assert(reverseInteger(120) == 21);
    assert(reverseInteger(-1200) == -21);
    
    // Symmetric numbers remain unchanged
    assert(reverseInteger(121) == 121);
    
    // Overflow cases must return 0
    assert(reverseInteger(1534236469) == 0);   // reversed overflows INT_MAX
    assert(reverseInteger(INT_MAX) == 0);      // 2147483647 reversed overflows
    assert(reverseInteger(INT_MIN) == 0);      // -2147483648 reversed overflows
    
    return 0;
}
