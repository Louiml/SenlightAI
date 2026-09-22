Write a standalone C++ function named `isPalindromeNumber` that takes a single integer `x` as input and returns a `bool` indicating whether `x` is a palindrome (reads the same forward and backward). The function should handle negative numbers correctly by returning `false` for any negative input (since the minus sign prevents symmetry). The function must work for the entire range of 32-bit signed integers, including values like `2147447412`. Do not use string conversion or any auxiliary data structures; solve the problem by reversing only the last half of the number (or the full number carefully) using integer arithmetic, and compare with the first half. The solution must be efficient for large inputs and avoid integer overflow when reversing (e.g., use a `long long` or reverse only half of the digits). Include appropriate `const` correctness in the function signature and implementation.
The classic approach is to reverse the entire number and compare it to the original, but this risks overflow for large integers like `2147483647` where the reverse `7463847412` overflows a 32-bit `int`. A safer method is to reverse only the second half of the digits and compare it with the first half. For example, for `1221`, we can repeatedly take the last digit of `x` (`x % 10`) and build a reversed number `rev`, while dividing `x` by 10. Stop when `x <= rev` (since we’ve passed the middle). For an even number of digits, `x` and `rev` should be equal; for an odd number of digits, `rev` has one extra digit (the middle digit), so we should compare `x` with `rev / 10`. Negative numbers cannot be palindromes because the leading minus sign disrupts symmetry, so we return `false` immediately for `x < 0`. Also, numbers ending in 0 (except 0 itself) are not palindromes (e.g., 10, 100), because the reverse would have a leading zero, which is invalid. Edge cases include `0` (true), single-digit numbers (true), `121` (true), `12321` (true), `-121` (false), `10` (false), and `1001` (true). Time complexity is O(log₁₀ n) because we process each digit once. Space complexity is O(1) as only a few integer variables are used.
#include <cstdint>

bool isPalindromeNumber(int x) {
    // Negative numbers and numbers ending with zero (except zero itself) are not palindromes.
    if (x < 0 || (x % 10 == 0 && x != 0)) {
        return false;
    }

    int reversed_half = 0;
    while (x > reversed_half) {
        reversed_half = reversed_half * 10 + x % 10;
        x /= 10;
    }

    // For even digit lengths, x == reversed_half.
    // For odd digit lengths, we ignore the middle digit by dividing reversed_half by 10.
    return x == reversed_half || x == reversed_half / 10;
}
#include <cassert>

int main() {
    assert(isPalindromeNumber(0) == true);
    assert(isPalindromeNumber(7) == true);
    assert(isPalindromeNumber(121) == true);
    assert(isPalindromeNumber(-121) == false);
    assert(isPalindromeNumber(10) == false);
    assert(isPalindromeNumber(1001) == true);
    assert(isPalindromeNumber(12321) == true);
    assert(isPalindromeNumber(2147447412) == true);
    assert(isPalindromeNumber(2147483647) == false);
    assert(isPalindromeNumber(1000021) == false);
    return 0;
}
