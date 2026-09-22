/*
Write a C++ function named `reverseDigits` that accepts a non-negative integer and returns an integer representing the digits of the input in reversed order (e.g., 1234 becomes 4321, 100 becomes 1, and 0 becomes 0). The function must not use any standard library string conversion, arrays, vectors, or other containers; it should compute the result using arithmetic operations only. The input is guaranteed to be a non-negative integer within the range of `int` (0 to 2,147,483,647). For numbers whose reversed value would overflow the `int` range (e.g., 2,147,483,647 reversed is 7,463,847,412), the function should return -1 to signal overflow. Use `const` correctness for any parameters that are not modified.
*/
#include <climits>

// Returns the reversed digits of a non-negative integer.
// Returns -1 if the reversed value would overflow the int range.
int reverseDigits(int num) {
    int reversed = 0;
    while (num != 0) {
        int digit = num % 10;
        // Check for overflow before multiplying and adding.
        if (reversed > (INT_MAX - digit) / 10) {
            return -1;
        }
        reversed = reversed * 10 + digit;
        num /= 10;
    }
    return reversed;
}
#include <cassert>

int main() {
    assert(reverseDigits(0) == 0);
    assert(reverseDigits(5) == 5);
    assert(reverseDigits(12) == 21);
    assert(reverseDigits(1234) == 4321);
    assert(reverseDigits(100) == 1);
    assert(reverseDigits(1000) == 1);
    assert(reverseDigits(12345) == 54321);
    assert(reverseDigits(INT_MAX) == -1); // 2147483647 reversed overflows
    assert(reverseDigits(1000000003) == -1); // reversed is 3000000001 > INT_MAX
    assert(reverseDigits(123456789) == 987654321);
}
// The algorithm repeatedly extracts the last digit of the input using the modulo operator (`% 10`) and appends it to a result accumulator. This is done by multiplying the current result by 10 and adding the extracted digit. After each extraction, the input is divided by 10 to remove the last digit. The loop continues until the input becomes 0. A critical edge case is when the input is 0: the loop would not run if we pre-check, but the natural handling is to initialize the result to 0 and process the digit 0 once, yielding 0 correctly. Overflow is detected by checking before multiplying: if the current result is greater than `(INT_MAX - digit) / 10`, then multiplying by 10 and adding the digit would exceed the maximum integer value. In that case, return -1. For inputs with trailing zeros (e.g., 100), the reversed result correctly becomes 1 because the leading digit is 1 and subsequent zeros do not add value (e.g., 100 → digit 0 → result 0, digit 0 → result 0, digit 1 → result 1). Time complexity is O(d), where d is the number of digits in the input, and space complexity is O(1) auxiliary.
