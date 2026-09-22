Write a standalone C++ function named `sumOfDigits` that accepts a non-negative integer as input and returns the sum of its decimal digits. For example, given 568, the function must return 19 (since 5+6+8=19). The function must handle the edge case where the input is 0, as well as numbers with multiple trailing or leading zeros (e.g., 1000), and must not use string conversion. The function should be implemented iteratively using integer division and modulo operations.

The main algorithm repeatedly extracts the last digit of the number using the modulo operator (`% 10`), adds it to an accumulator variable, and then removes that digit via integer division (`/= 10`), continuing until the number becomes zero. Edge cases: if the input is 0, the loop never executes, but the initial sum should be 0, which is correct — we must ensure the function returns 0 for 0. For numbers like 1000, the algorithm correctly processes zeros in the middle and at the end; for example, 1000 gives digits 0,0,0,1 in order, summing to 1. No negative inputs are expected per specification, but if a negative number were passed, the loop condition `n > 0` would fail, so we can guard or document the contract. Time complexity is O(d) where d is the number of digits in the input (at most about 10 for a 32-bit integer, so effectively O(1)). Space complexity is O(1) because only a few integer variables are used.

// Return the sum of the decimal digits of a non-negative integer.
// For example, sumOfDigits(568) == 19, sumOfDigits(0) == 0.
int sumOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;   // add the last digit
        n /= 10;         // discard the last digit
    }
    return sum;
}

#include <cassert>

int sumOfDigits(int n);

int main() {
    // Basic examples
    assert(sumOfDigits(568) == 19);
    assert(sumOfDigits(0) == 0);
    assert(sumOfDigits(7) == 7);
    // Numbers with zeros
    assert(sumOfDigits(100) == 1);
    assert(sumOfDigits(1010) == 2);
    assert(sumOfDigits(90909) == 27);
    // Large numbers
    assert(sumOfDigits(999999999) == 81);
    assert(sumOfDigits(123456789) == 45);
    // Single digit and edge cases
    assert(sumOfDigits(1) == 1);
    assert(sumOfDigits(10) == 1);
    assert(sumOfDigits(00000123) == 6); // leading zeros ignored by integer literal
    return 0;
}
