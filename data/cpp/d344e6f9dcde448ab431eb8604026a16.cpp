/*
Write a C++ function named `reverseDigits` that takes a positive integer `n` (with no leading zeros) and returns a new integer whose decimal digits are the reverse of the original number's digits. For example, `reverseDigits(12345)` should return `54321`, and `reverseDigits(7)` should return `7`. The function must handle numbers with 1 to 5 digits inclusive, and it should correctly process numbers like `100` (returning `1`) and `1200` (returning `21`). Do not use string conversion or any library functions for reversal; only integer arithmetic is allowed. The function signature is `int reverseDigits(int n)`. Assume input is always a positive integer within the range 1 to 99999.
*/

#include <cstdint>

// Reverse the decimal digits of a positive integer (1 to 99999).
// Returns the integer formed by reversing the digits; trailing zeros in the original become leading zeros in the reversed representation and are dropped.
int reverseDigits(int n) {
    int reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + (n % 10);
        n /= 10;
    }
    return reversed;
}

#include <cassert>

int main() {
    assert(reverseDigits(12345) == 54321);
    assert(reverseDigits(7) == 7);
    assert(reverseDigits(100) == 1);
    assert(reverseDigits(1200) == 21);
    assert(reverseDigits(1) == 1);
    assert(reverseDigits(99999) == 99999);
    assert(reverseDigits(10000) == 1);
    assert(reverseDigits(10203) == 30201);
    assert(reverseDigits(50000) == 5);
    assert(reverseDigits(11111) == 11111);
    return 0;
}

// The straightforward approach is to repeatedly extract the last digit of the number using the modulo operator and build the reversed number by multiplying the current result by 10 and adding that digit, then integer-dividing the original number by 10. This loop continues until the original number becomes 0. This naturally handles numbers with trailing zeros (e.g., `100` → `001` → `1`) because the leading zeros in the reversed representation are simply dropped when the result is built as an integer. Edge cases include single-digit numbers (the loop runs once and returns the same digit), and numbers like `10000` (reversal yields `1`). The algorithm runs in O(d) time where d is the number of digits in n (max 5), and uses O(1) auxiliary space. No special handling for negative numbers is needed because input is guaranteed positive.
