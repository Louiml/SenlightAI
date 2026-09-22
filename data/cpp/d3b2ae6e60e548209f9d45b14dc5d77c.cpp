/*
Write a C++ function `bool isArmstrongNumber(int n)` that determines whether a given non-negative integer is an Armstrong number (also called a narcissistic number). An Armstrong number is a number that equals the sum of its own digits each raised to the power of the number of digits in the number. For example, 153 is an Armstrong number because \(1^3 + 5^3 + 3^3 = 153\), and 9474 is an Armstrong number because \(9^4 + 4^4 + 7^4 + 4^4 = 9474\). The function should handle the edge case of `n = 0`, which is considered an Armstrong number (since it has one digit and \(0^1 = 0\)). Negative numbers should return `false` immediately. The function must be `const`-correct where applicable and must not modify the input.
*/

#include <cmath>

// Determine if a non-negative integer is an Armstrong number.
// An Armstrong number equals the sum of its digits each raised to the power
// of the number of digits. Negative numbers return false; 0 returns true.
bool isArmstrongNumber(int n) {
    if (n < 0) {
        return false;
    }
    if (n == 0) {
        return true;  // 0 has one digit: 0^1 = 0
    }

    // Count the number of digits.
    int temp = n;
    int digitCount = 0;
    while (temp > 0) {
        temp /= 10;
        ++digitCount;
    }

    // Compute sum of digits raised to digitCount.
    long long sum = 0;
    temp = n;
    while (temp > 0) {
        int digit = temp % 10;
        temp /= 10;
        // Compute digit^digitCount using integer power.
        long long power = 1;
        for (int i = 0; i < digitCount; ++i) {
            power *= digit;
        }
        sum += power;
    }

    return sum == n;
}

#include <cassert>

int main() {
    // Single-digit numbers are always Armstrong (digit^1 = digit).
    assert(isArmstrongNumber(0) == true);
    assert(isArmstrongNumber(1) == true);
    assert(isArmstrongNumber(5) == true);
    assert(isArmstrongNumber(9) == true);

    // Classic examples.
    assert(isArmstrongNumber(153) == true);
    assert(isArmstrongNumber(370) == true);
    assert(isArmstrongNumber(371) == true);
    assert(isArmstrongNumber(407) == true);
    assert(isArmstrongNumber(9474) == true);

    // Non-Armstrong numbers.
    assert(isArmstrongNumber(10) == false);
    assert(isArmstrongNumber(100) == false);
    assert(isArmstrongNumber(123) == false);
    assert(isArmstrongNumber(9475) == false);

    // Negative numbers are not Armstrong.
    assert(isArmstrongNumber(-153) == false);

    // Large number that is not Armstrong (will not overflow due to long long).
    assert(isArmstrongNumber(1000000000) == false);
}

// The main algorithm is straightforward: first count the number of digits in the input `n` by repeatedly dividing by 10 in a loop, counting each division. Store this count in an integer (not a double, as the original snippet uses). Then, create a copy of the original number and iterate through its digits: extract the last digit using modulo 10, divide the temp number by 10 to remove that digit, and add `digit^digitCount` to a running sum using integer exponentiation (e.g., a loop or `std::pow` with rounding). After processing all digits, compare the sum to the original number; if equal, return `true`, otherwise `false`. Important edge cases: `n = 0` has one digit and should return `true` (the loop for counting digits must handle 0 correctly, e.g., treat `n = 0` as having 1 digit explicitly). Negative numbers return `false`. For very large numbers, the sum may overflow an `int`, so use a `long long` type for the sum. Time complexity is \(O(d)\) where \(d\) is the number of digits (since we do two passes: one for counting, one for summing, each linear in digit count). Space complexity is \(O(1)\) auxiliary.
