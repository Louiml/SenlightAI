// Write a C++ function named `sumOfDigits` that takes a non-negative integer as input and returns the sum of its decimal digits. For example, given `123`, the function should return `6` (1+2+3), and given `0`, it should return `0`. The function must handle numbers with multiple digits, including numbers that end in zeros (e.g., `100` → `1`), and must not rely on any external libraries beyond standard headers. The function signature should be `int sumOfDigits(int n);` and it should be pure (no side effects like printing).

#include <cassert>

int sumOfDigits(int n); // declaration for testing

int main() {
    assert(sumOfDigits(0) == 0);
    assert(sumOfDigits(1) == 1);
    assert(sumOfDigits(9) == 9);
    assert(sumOfDigits(10) == 1);
    assert(sumOfDigits(99) == 18);
    assert(sumOfDigits(123) == 6);
    assert(sumOfDigits(1000) == 1);
    assert(sumOfDigits(987654) == 39);
    assert(sumOfDigits(1000000000) == 1);
    assert(sumOfDigits(2147483647) == 46); // largest 32-bit int
    return 0;
}

#include <cstdint>

// Return the sum of decimal digits of a non-negative integer.
// Example: sumOfDigits(123) == 6, sumOfDigits(0) == 0.
int sumOfDigits(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

// The solution repeatedly extracts the last digit of the number using the modulo operator (`n % 10`), adds it to an accumulator, and then removes that digit using integer division (`n / 10`). This process continues until the number becomes zero. Edge cases: if the input is `0`, the loop does not run, and the sum remains `0` — correctly returning `0`. For numbers like `100`, the loop processes digits 0, 0, 1 in that order, summing to `1`. Negative inputs are not allowed per the specification, but if they were passed, the algorithm would still work with absolute values if handled explicitly (but we assume non-negative). Time complexity is \(O(\log_{10} n)\) — proportional to the number of decimal digits. Space complexity is \(O(1)\) — only a few integer variables are used.
