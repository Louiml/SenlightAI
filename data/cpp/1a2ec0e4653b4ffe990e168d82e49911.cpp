Write a C++ function named `isArmstrongNumber` that takes a non-negative integer as input and returns `true` if the number is an Armstrong number (also called a narcissistic number), and `false` otherwise. For a number with `d` digits, it is an Armstrong number if the sum of each digit raised to the power `d` equals the original number. For example, 153 (3 digits) is Armstrong because 1³ + 5³ + 3³ = 153, and 9474 (4 digits) is Armstrong because 9⁴ + 4⁴ + 7⁴ + 4⁴ = 9474. The function must handle all valid non-negative integers that fit in a standard C++ `int` type, including 0 (which is Armstrong, since 0¹ = 0) and 1-digit numbers (which are all Armstrong). You must use a loop and manual arithmetic — no use of `std::pow` is allowed; instead compute `digit^d` via repeated multiplication. Ensure the function is `const`-correct where applicable and avoid modifying the input parameter.
#include <cassert>

// Declaration of the function under test (already defined above).
bool isArmstrongNumber(int number);

int main() {
    // Known Armstrong numbers.
    assert(isArmstrongNumber(0) == true);
    assert(isArmstrongNumber(1) == true);
    assert(isArmstrongNumber(5) == true);
    assert(isArmstrongNumber(153) == true);
    assert(isArmstrongNumber(370) == true);
    assert(isArmstrongNumber(371) == true);
    assert(isArmstrongNumber(407) == true);
    assert(isArmstrongNumber(9474) == true);
    assert(isArmstrongNumber(9475) == false);
    assert(isArmstrongNumber(100) == false);
    assert(isArmstrongNumber(123) == false);
    return 0;
}
#include <cstdint>

// Returns true if `number` is an Armstrong number (sum of each digit raised to
// the power of the number of digits equals the number itself).
bool isArmstrongNumber(int number) {
    if (number < 0) {
        return false; // Negative numbers are not considered Armstrong numbers.
    }
    if (number == 0) {
        return true; // 0 has 1 digit, 0^1 = 0.
    }

    // Count the number of digits.
    int temp = number;
    int digitCount = 0;
    while (temp > 0) {
        ++digitCount;
        temp /= 10;
    }

    // Compute the sum of each digit raised to digitCount.
    int sum = 0;
    temp = number;
    while (temp > 0) {
        int digit = temp % 10;
        int power = 1;
        for (int i = 0; i < digitCount; ++i) {
            power *= digit;
        }
        sum += power;
        // Early exit to avoid unnecessary overflow/computation.
        if (sum > number) {
            return false;
        }
        temp /= 10;
    }

    return sum == number;
}
// The core idea is to first count the number of digits `d` in the given number (by repeatedly dividing by 10), then iterate through each digit of a copy of the number, computing `digit^d` using a small inner loop (e.g., multiply `digit` by itself `d` times), and accumulating the sum. After processing all digits, compare the sum to the original number. Edge cases: 0 has 1 digit and 0^1 = 0, so it returns true; single-digit numbers always return true; numbers like 10, 100, etc. are not Armstrong because their digit powers sum to less than the number itself; very large numbers close to `INT_MAX` may cause integer overflow during the power computation, but within the typical `int` range and for valid input (e.g., up to 2,147,483,647) the intermediate sums for non-Armstrong numbers will overflow, but we can mitigate by early stopping: if the sum ever exceeds the original number, we can return `false` immediately because adding more positive terms will only increase the sum. For an Armstrong number, the sum equals the number, so it won't overflow if the number fits in `int`. Time complexity is O(d²) because for each of the `d` digits we do `d` multiplications; for up to 10 digits (since max int has 10 digits) this is at most 100 operations per call, which is constant. Space complexity is O(1) as we use a few integer variables.
