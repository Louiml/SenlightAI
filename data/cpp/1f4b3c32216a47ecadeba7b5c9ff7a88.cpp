// Write a C++ function named `isArmstrongNumber` that takes a non-negative integer as input and returns a `bool` indicating whether the number is an Armstrong number (also called a narcissistic number). An Armstrong number is an integer such that the sum of its own digits each raised to the power of the number of digits equals the number itself. For example, 153 is a 3-digit Armstrong number because 1³ + 5³ + 3³ = 153, and 9474 is a 4-digit Armstrong number because 9⁴ + 4⁴ + 7⁴ + 4⁴ = 9474. The function must correctly handle all non-negative integers, including 0 (which is an Armstrong number) and single-digit numbers (which are always Armstrong numbers). You may assume the input will not exceed the maximum value of an `int`.
The solution requires computing the number of digits first, then summing each digit raised to the power of the digit count, and finally comparing the result with the original number. The main algorithm: (1) Handle the edge case where the input is 0 → it is an Armstrong number (0¹ = 0). (2) Count the digits by repeatedly dividing a copy of the number by 10. (3) Sum each digit raised to the power of the digit count using a loop or `std::pow`. Since `std::pow` works with floating‑point, for integer precision it is safer to implement a small integer power function or use repeated multiplication in a loop. (4) Compare the sum to the original number. Edge cases: single‑digit numbers (0–9) always return `true`; numbers like 10 or 100 are not Armstrong numbers; very large inputs (e.g., up to 9 digits for 32‑bit `int`) require careful power computation without overflow beyond `int` (the sum itself could exceed `int` for large numbers, so use `long long` for accumulation). Time complexity is O(d²) if computing each digit power by repeated multiplication (d digits, each power takes O(d) multiplications), or O(d) if using an efficient integer power function. For practical `int` sizes, this is at most O(81) operations. Space complexity is O(1).
#include <cmath>  // for std::pow (optional; we use a custom integer power for precision)

// Return true if the given non-negative integer is an Armstrong number.
bool isArmstrongNumber(int num) {
    if (num < 0) return false;  // not defined for negative numbers

    // Single digits and zero are always Armstrong numbers.
    if (num < 10) return true;

    // Count the number of digits.
    int digits = 0;
    int temp = num;
    while (temp != 0) {
        digits++;
        temp /= 10;
    }

    // Compute sum of each digit raised to the power of digit count.
    long long sum = 0;
    temp = num;
    while (temp != 0) {
        int digit = temp % 10;
        // Compute digit^digits using integer multiplication (no floating point).
        int power = 1;
        for (int i = 0; i < digits; ++i) {
            power *= digit;
        }
        sum += power;
        temp /= 10;
    }

    return sum == num;
}
#include <cassert>

int main() {
    // Single-digit numbers are always Armstrong numbers.
    assert(isArmstrongNumber(0) == true);
    assert(isArmstrongNumber(5) == true);
    assert(isArmstrongNumber(9) == true);

    // Classic 3-digit Armstrong numbers.
    assert(isArmstrongNumber(153) == true);
    assert(isArmstrongNumber(370) == true);
    assert(isArmstrongNumber(371) == true);
    assert(isArmstrongNumber(407) == true);

    // 4-digit Armstrong number.
    assert(isArmstrongNumber(9474) == true);

    // Known non-Armstrong numbers.
    assert(isArmstrongNumber(10) == false);
    assert(isArmstrongNumber(100) == false);
    assert(isArmstrongNumber(123) == false);
    assert(isArmstrongNumber(999) == false);
    assert(isArmstrongNumber(9475) == false);

    // Larger 3-digit non-Armstrong.
    assert(isArmstrongNumber(12) == false);

    return 0;
}
