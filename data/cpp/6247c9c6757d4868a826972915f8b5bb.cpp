/*
Write a C++ function that takes a non-negative integer `value` representing a binary number using only the digits `0` and `1` (no leading zeros, except the number `0` itself), and returns its decimal equivalent as an `int`. For example, input `101` should return `5`, and input `0` should return `0`. The function must not use any built-in binary-to-decimal conversion utilities and must handle the entire integer digit-by-digit from right to left.
*/
#include <cstddef>   // for size_t (not strictly needed but good practice)

// Convert a non-negative integer whose decimal representation uses only 0/1
// into its decimal value by interpreting it as a binary number.
// Example: binaryToDecimal(1101) returns 13.
int binaryToDecimal(int value) {
    int decimal = 0;
    int power = 1;  // 2^0

    while (value > 0) {
        const int digit = value % 10;      // extract rightmost digit
        decimal += digit * power;          // add weighted contribution
        power *= 2;                        // next power of two
        value /= 10;                       // remove processed digit
    }

    return decimal;
}
#include <cassert>

int main() {
    assert(binaryToDecimal(0) == 0);
    assert(binaryToDecimal(1) == 1);
    assert(binaryToDecimal(10) == 2);
    assert(binaryToDecimal(11) == 3);
    assert(binaryToDecimal(101) == 5);
    assert(binaryToDecimal(1111) == 15);
    assert(binaryToDecimal(10000) == 16);
    assert(binaryToDecimal(101010) == 42);
    assert(binaryToDecimal(110110) == 54);
    assert(binaryToDecimal(11111111) == 255);
    return 0;
}
// The algorithm processes the binary number as a base-10 integer that visually looks like binary. We extract the least significant digit using `value % 10`, multiply it by the current power of two (starting at 1 and doubling each iteration), add that product to a running sum, then remove the processed digit with integer division by 10. This continues until `value` becomes 0. Edge cases: the input `0` immediately produces output `0` because the loop never executes, and any trailing zeros in the middle or end are naturally handled because they contribute `0 * power`. No negative inputs are allowed per the specification. Time complexity is O(d) where d is the number of digits, and space complexity is O(1) besides the return value.
