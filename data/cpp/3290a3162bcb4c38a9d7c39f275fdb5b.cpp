/*
Write a C++ function that takes a positive integer as input and returns its digits in reverse order as a new integer. For example, given 1234, the function should return 4321. The input is guaranteed to be a positive integer fitting within the range of `int`. The function must handle numbers ending in zero correctly (e.g., 120 → 21, not 021). The returned value should be an `int`.
*/

#include <cstdint>

// Reverse the digits of a positive integer and return the reversed number.
// Example: reverseDigits(1234) -> 4321, reverseDigits(120) -> 21.
// Precondition: input > 0 and reversed value must fit within int range.
int reverseDigits(int num) {
    int reversed = 0;
    while (num > 0) {
        int digit = num % 10;
        reversed = reversed * 10 + digit;
        num /= 10;
    }
    return reversed;
}

#include <cassert>

int main() {
    assert(reverseDigits(1234) == 4321);
    assert(reverseDigits(5) == 5);
    assert(reverseDigits(120) == 21);
    assert(reverseDigits(1000) == 1);
    assert(reverseDigits(987654321) == 123456789);
    assert(reverseDigits(2147483412) == 2143847412); // Example near max, still fits
    assert(reverseDigits(10) == 1);
    assert(reverseDigits(1111) == 1111);
    assert(reverseDigits(100) == 1);
    assert(reverseDigits(123456789) == 987654321);
    return 0;
}

// The core idea is to extract digits from the least significant position using modulo 10 and division by 10, then rebuild the reversed number by accumulating digits in reverse order. A common approach as shown in the snippet uses a stack to store digits in the order they are extracted (from least significant to most significant) and then pops them to reconstruct the number, which yields the digits in reverse order naturally. However, a more direct and memory-efficient approach is to reverse the number without any auxiliary container: while the original number is non-zero, take the last digit (num % 10), append it to the result (result = result * 10 + digit), and then divide the original number by 10.
//
// Edge cases: The input is always positive, so no need to handle negatives. If the number ends with one or more zeros, those zeros become leading zeros in the reversed number and are naturally discarded by the integer representation (e.g., 120 → 21). The number 0 is not a possible input since it is not positive, but if it were allowed, the function would return 0. The largest possible input might cause overflow when reversing, e.g., 2147483647 reverses to 7463847412 which exceeds `int` max. Since the task states input fits within `int`, we can assume the reversed value also fits, but we note this as a limitation. The algorithm runs in O(d) time where d is the number of digits (at most 10 for 32-bit int), and uses O(1) auxiliary space (excluding the stack from the snippet, but our improved version is O(1)).
