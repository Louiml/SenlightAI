Write a C++ function named `isHarshadNumber` that takes a single positive integer as input and returns `true` if the number is divisible by the sum of its own digits (a Harshad or Niven number), and `false` otherwise. The function must handle numbers with multiple digits, including edge cases like single-digit numbers (which are always Harshad numbers) and numbers where the digit sum could be zero only if the input is zero (but assume input is positive, so no zero handling needed). The function should be const-correct, using pass-by-value for the integer, and should not modify the input parameter. The solution must be efficient for numbers up to at least 2 billion (fits in a 32-bit signed int).
The algorithm works by first copying the input value to a separate variable so the original parameter remains unchanged, then iteratively summing its digits by repeatedly taking the remainder when dividing by 10 and then dividing the number by 10 until the number becomes zero. After obtaining the digit sum, the function checks if the original number modulo the digit sum equals zero. Since we assume positive input, the digit sum will always be at least 1 (for numbers ≥1), so no division by zero risk. Key edge cases: single-digit numbers (e.g., 7) have a digit sum equal to the number itself, so they always satisfy the condition; numbers like 10 have digit sum 1, so they are Harshad; numbers like 11 have digit sum 2, and 11 % 2 = 1, so they are not. The time complexity is O(d) where d is the number of digits in the input (at most 10 for a 32-bit int), and space complexity is O(1) since only a few integer variables are used.
#include <string>
#include <vector>

// Return true if the input positive integer is divisible by the sum of its digits.
bool isHarshadNumber(const int x) {
    int remaining = x;   // copy to preserve original input
    int digitSum = 0;

    // Sum the digits of the number
    while (remaining > 0) {
        digitSum += remaining % 10;
        remaining /= 10;
    }

    // A positive number always has digitSum >= 1, so no division by zero.
    return (digitSum != 0) && (x % digitSum == 0);
}
#include <cassert>

int main() {
    // Single-digit numbers are always Harshad.
    assert(isHarshadNumber(1) == true);
    assert(isHarshadNumber(7) == true);
    assert(isHarshadNumber(9) == true);

    // Multiple-digit Harshad numbers.
    assert(isHarshadNumber(10) == true);   // digit sum 1, 10 % 1 == 0
    assert(isHarshadNumber(18) == true);   // digit sum 9, 18 % 9 == 0
    assert(isHarshadNumber(20) == true);   // digit sum 2, 20 % 2 == 0
    assert(isHarshadNumber(21) == true);   // digit sum 3, 21 % 3 == 0

    // Non-Harshad numbers.
    assert(isHarshadNumber(11) == false);  // digit sum 2, 11 % 2 == 1
    assert(isHarshadNumber(13) == false);  // digit sum 4, 13 % 4 == 1
    assert(isHarshadNumber(19) == false);  // digit sum 10, 19 % 10 == 9

    // Larger value.
    assert(isHarshadNumber(1729) == true); // digit sum 19, 1729 / 19 == 91
    assert(isHarshadNumber(2000) == true); // digit sum 2, 2000 % 2 == 0
    assert(isHarshadNumber(987654321) == false); // digit sum 45, not divisible.

    return 0;
}
