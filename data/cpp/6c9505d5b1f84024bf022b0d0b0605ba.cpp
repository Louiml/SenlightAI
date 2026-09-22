Write a C++ function that takes a positive integer as input and returns an integer whose decimal representation is the binary representation of the input, treating the original number as an unsigned binary value. For example, if the input is 5, the function should return 101; if the input is 0, it should return 0. The function must handle inputs up to 2^31 - 1 (the typical maximum for a signed 32-bit integer) and must not use any built-in binary conversion or bit-manipulation functions like `std::bitset`. Use a loop to repeatedly divide by 2, collect the remainder (0 or 1) at each step, and build the resulting decimal number that looks like the binary string.
// The given snippet incorrectly loops for `i=n` times but uses `n` inside the loop, effectively reducing `n` to zero after the first iteration, so it only prints the least significant bit. The correct approach is to repeatedly divide the input by 2 until it becomes 0, storing the remainder (which is `input % 2`) at each step. To construct the result such that the last remainder becomes the most significant digit, we can either build the result by multiplying a positional factor by 10 each step (like in the snippet), or collect digits in a string and reverse. For numeric output, we start `result = 0` and `place = 1`; for each remainder `bit`, we add `bit * place` to `result` and multiply `place` by 10. This way, the first remainder (least significant binary digit) gets placed in the ones digit, the next in tens, etc. Edge cases: input = 0 should return 0; input = 1 returns 1; input = 2 returns 10; large values like 2147483647 produce a long decimal number (31 digits) that fits in a 64-bit integer but not in 32-bit `int`; therefore the return type should be `long long` to avoid overflow. Time complexity is O(log n) because we divide by 2 each iteration, and space complexity is O(1) aside from the return value.
#include <cstdint>

// Convert a positive integer to a decimal numeral representing its binary form.
// Returns a long long because for inputs near 2^31-1 the binary representation
// has 31 digits, which exceeds the 32-bit int range.
long long toBinaryDecimal(int n) {
    if (n == 0) return 0;

    long long result = 0;
    long long place = 1; // positional multiplier for building the decimal number

    while (n > 0) {
        int bit = n % 2;            // extract least significant bit
        result += bit * place;      // place this bit at the current position
        place *= 10;                // move to next higher decimal position
        n /= 2;                     // shift right in binary
    }

    return result;
}
#include <cassert>

int main() {
    // Basic cases
    assert(toBinaryDecimal(0) == 0);
    assert(toBinaryDecimal(1) == 1);
    assert(toBinaryDecimal(2) == 10);
    assert(toBinaryDecimal(3) == 11);
    assert(toBinaryDecimal(4) == 100);
    assert(toBinaryDecimal(5) == 101);
    assert(toBinaryDecimal(6) == 110);
    assert(toBinaryDecimal(7) == 111);
    assert(toBinaryDecimal(8) == 1000);

    // Larger values
    assert(toBinaryDecimal(10) == 1010);
    assert(toBinaryDecimal(15) == 1111);
    assert(toBinaryDecimal(16) == 10000);
    assert(toBinaryDecimal(255) == 11111111);
    assert(toBinaryDecimal(256) == 100000000);

    // Edge case: maximum 32-bit signed integer
    assert(toBinaryDecimal(2147483647) == 1111111111111111111111111111111LL);
}
