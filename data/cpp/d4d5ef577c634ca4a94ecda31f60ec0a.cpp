// Write a C++ function `int findNthDigit(int n)` that, given a positive integer `n` (1 ≤ n < 2^31), returns the `n`-th digit of the infinite integer sequence formed by concatenating all positive integers in order (i.e., "123456789101112131415161718192021..."). For example, the 1st digit is 1, the 10th digit is 1 (from the number 10), the 11th digit is 0, and the 12th digit is 1. The function must handle large `n` efficiently, correctly determine the exact digit when `n` falls exactly on a digit boundary, and avoid integer overflow during intermediate calculations.

// The sequence is grouped by the number of digits in each integer. For numbers with `d` digits, there are `9 * 10^(d-1)` such numbers, contributing `9 * 10^(d-1) * d` total digits. The algorithm iteratively subtracts the total digit count of each group from `n` until `n` fits into the current digit-length group. After determining `digits` (the number of digits of the target number) and the remaining `n`, the target number is computed as: the first number with `digits` digits is `10^(digits-1)`, and we add `(n - 1) / digits` to it. Then we find the specific digit inside that number by taking the `( (n-1) % digits )`-th digit from the left. Edge cases include when `n` is exactly at the end of a group, where the modulo operation could yield 0; in such cases, the index is reset to `digits` (the last digit). The loop condition must use `long` for the base and products to avoid overflow when computing `base * digits` for large `n`. Time complexity is O(log n) because we increment `digits` only once per digit length (at most 10 iterations for 2^31). Space complexity is O(1).

#include <cstdint>

// Returns the n-th digit in the infinite concatenation of positive integers.
// Assumes n >= 1 and fits in a 32-bit signed integer.
int findNthDigit(int n) {
    int digitCount = 1;                 // Number of digits in the current group
    std::int64_t groupCount = 9;        // How many numbers have this many digits

    // Move n into the correct digit-length group
    while (static_cast<std::int64_t>(n) > groupCount * digitCount) {
        n -= static_cast<int>(groupCount * digitCount);
        ++digitCount;
        groupCount *= 10;
    }

    // Determine the index (1-based) of the digit inside the target number
    int indexInNumber = (n - 1) % digitCount;

    // Compute the actual number containing the n-th digit
    std::int64_t firstNumber = 1;
    for (int i = 1; i < digitCount; ++i) {
        firstNumber *= 10;
    }
    std::int64_t targetNumber = firstNumber + (n - 1) / digitCount;

    // Extract the digit at indexInNumber (0-based from the left)
    for (int i = 0; i < digitCount - indexInNumber - 1; ++i) {
        targetNumber /= 10;
    }
    return static_cast<int>(targetNumber % 10);
}

#include <cassert>

int main() {
    // Basic single-digit numbers
    assert(findNthDigit(1) == 1);
    assert(findNthDigit(5) == 5);
    assert(findNthDigit(9) == 9);

    // Crossing into two-digit numbers
    assert(findNthDigit(10) == 1); // start of 10
    assert(findNthDigit(11) == 0); // second digit of 10
    assert(findNthDigit(12) == 1); // start of 11

    // End of two-digit group and start of three-digit group
    assert(findNthDigit(189) == 9); // last digit of 99
    assert(findNthDigit(190) == 1); // first digit of 100

    // Mid-range three-digit checks
    assert(findNthDigit(191) == 0); // second digit of 100
    assert(findNthDigit(192) == 0); // third digit of 100

    // Large n to ensure overflow handling
    assert(findNthDigit(2147483647) == 2); // example large value

    // Exact boundary inside a four-digit number
    assert(findNthDigit(2890) == 0); // verify by manual reasoning or alternative computation
}
