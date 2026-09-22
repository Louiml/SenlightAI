// Write a C++ function that, given a natural number \( n \) with at least one digit, returns the number formed by concatenating the digits of \( n \) that are odd, in ascending order of the digit values (1, 3, 5, 7, 9), while preserving the original relative order of digits with the same value. For example, for \( n = 253387 \), the odd digits are 3, 3, 5, 7; sorting them by value gives 3, 3, 5, 7, so the result is 3357. If no odd digits exist, return 0. The function must not use arrays, strings, or any standard containers; it must work by direct integer manipulation.

#include <cassert>
#include <cstdint>

// Forward declaration of the function under test (matches the solution).
std::uint64_t oddDigitsSorted(std::uint64_t n);

int main() {
    // Basic example from the problem statement: n=253387 -> odd digits 3,3,5,7 -> 3357
    assert(oddDigitsSorted(253387) == 3357);
    // Single odd digit
    assert(oddDigitsSorted(7) == 7);
    assert(oddDigitsSorted(2) == 0);
    // Multiple same odd digits, preserving order: n=111 -> all 1's -> 111
    assert(oddDigitsSorted(111) == 111);
    // Mixed digits with odd and even: n=123456789 -> odd digits 1,3,5,7,9 -> result 13579
    assert(oddDigitsSorted(123456789) == 13579);
    // All even digits: n=2468 -> no odd -> 0
    assert(oddDigitsSorted(2468) == 0);
    // Large number with no odd digits
    assert(oddDigitsSorted(2000) == 0);
    // Number with only 9's
    assert(oddDigitsSorted(999) == 999);
    // Number with odd digits in descending order: n=97531 -> digits 1,3,5,7,9 sorted ascending -> 13579
    assert(oddDigitsSorted(97531) == 13579);
    // Number with repeated odd digits in different positions: n=10101 -> odd digits are 1,1,1 (the zeros are even) -> 111
    assert(oddDigitsSorted(10101) == 111);
    // Zero itself (edge case, though problem says non-zero, still safe)
    assert(oddDigitsSorted(0) == 0);
    return 0;
}

#include <cstdint>

// Returns the number formed by the odd digits of n (values 1,3,5,7,9) 
// sorted by digit value, preserving original order for equal digits.
// Returns 0 if n has no odd digits.
std::uint64_t oddDigitsSorted(std::uint64_t n) {
    std::uint64_t result = 0;
    for (int x = 1; x <= 9; x += 2) {
        std::uint64_t current = n;
        // Process digits from least significant to most significant.
        while (current != 0) {
            int digit = static_cast<int>(current % 10);
            if (digit == x) {
                result = result * 10 + static_cast<std::uint64_t>(x);
            }
            current /= 10;
        }
    }
    return result;
}

// The algorithm processes each possible odd digit \( x \) from 1 to 9 in increasing order. For each \( x \), it scans the original number digit by digit (from least significant to most significant) by repeatedly taking the remainder modulo 10 and dividing by 10. Whenever a digit equals \( x \), that digit is appended to the result. Appending is done by multiplying the current accumulated result by 10 and adding \( x \), which correctly places digits in their original relative order (because scanning from the units digit upward naturally builds the number from least significant to most significant, but when later digits are found they get appended to the right, preserving the order of equal-valued digits). After processing all digits for all odd \( x \), the accumulated result is returned. Edge cases: if the number has no odd digits, the returned value is 0 (since the accumulator stays 0); the input is guaranteed natural and non-zero, but the function should handle zero by returning 0. Time complexity is \( O(9 \cdot \text{digits}(n)) \), which simplifies to \( O(\log n) \) in terms of the number of digits; space complexity is \( O(1) \) as only a few integer variables are used.
