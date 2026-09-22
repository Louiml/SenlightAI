/*
Write a C++ function named `reversePositiveInteger` that accepts a single positive `int` parameter and returns a new `int` representing the digits of the input in reverse order (e.g., `1234` becomes `4321`). The function must handle only positive inputs; you may assume the caller will never pass zero or negative numbers (but for safety, if the input is less than or equal to zero, return `0`). Additionally, write a complete program that continuously asks the user for positive integers. For each positive integer entered, the program prints its reversed version by calling `reversePositiveInteger`. The program terminates when the user enters a negative integer (the program should not print anything for a negative input). The function should be written as a free function with `const` correctness where applicable (i.e., the parameter should be passed by value or const reference, but since we need to modify a copy, pass by value). Include necessary headers and provide detailed comments inside the function. Do not include `main` in the function definition, but the test section will contain a `main` that asserts the function's correctness.
*/
#include <iostream>

/**
 * Reverses the digits of a positive integer.
 * If the input is not positive, returns 0.
 * Uses a loop that extracts digits from the least significant end.
 */
int reversePositiveInteger(int input) {
    if (input <= 0) {
        return 0;  // Defensive: spec says only positive inputs are used.
    }

    int reversed = 0;
    while (input > 0) {
        reversed = reversed * 10 + (input % 10);
        input /= 10;
    }
    return reversed;
}
#include <cassert>

// Forward declaration of the solution function (if not already included).
int reversePositiveInteger(int input);

int main() {
    // Basic reversal cases.
    assert(reversePositiveInteger(1234) == 4321);
    assert(reversePositiveInteger(1) == 1);
    assert(reversePositiveInteger(10) == 1);
    assert(reversePositiveInteger(100) == 1);
    assert(reversePositiveInteger(987654321) == 123456789);
    assert(reversePositiveInteger(120) == 21);
    assert(reversePositiveInteger(5) == 5);

    // Defensive case for non-positive input.
    assert(reversePositiveInteger(0) == 0);
    assert(reversePositiveInteger(-123) == 0);

    return 0;
}
// The core algorithm reverses an integer by repeatedly extracting the last digit using the modulo operator (`% 10`) and appending it to a result accumulator. Start with `result = 0`. While the input is greater than zero, do: `result = result * 10 + (input % 10)`, then `input /= 10`. This shifts existing digits left by one position and adds the new last digit. For example, input `123`: first iteration gives `result = 0*10 + 3 = 3`, input becomes `12`; next gives `result = 3*10 + 2 = 32`, input becomes `1`; last gives `result = 32*10 + 1 = 321`, input becomes `0`. Edge cases: the digit `0` at the end of the input (e.g., `120`) will produce `21` (the leading zero is lost, which is correct for integer reversal). Input `0` is not valid per spec, but the function returns `0` defensively. For very large positive integers near `INT_MAX`, reversing may overflow; we can ignore that for a basic task or use a `long long` accumulator to be safe. Time complexity is O(d), where d is the number of digits, and space complexity is O(1). The main loop in the program uses `std::cin` in a `while(true)` loop; on negative input, it breaks.
