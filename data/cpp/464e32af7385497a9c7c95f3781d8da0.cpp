/*
Write a C++ function named `swapOddEvenBits` that takes an unsigned integer `n` (where `0 ≤ n ≤ 255`) and returns a new unsigned integer obtained by swapping every pair of adjacent bits: bit 0 (least significant) with bit 1, bit 2 with bit 3, bit 4 with bit 5, and bit 6 with bit 7. This means that in the binary representation (padded to 8 bits), the bits at odd positions (1,3,5,7) move to the adjacent even positions (0,2,4,6), and vice versa. For example, `23` (binary `00010111`) becomes `43` (binary `00101011`). The function must work for all 8-bit unsigned values, including `0`, `1`, `255`, and numbers where swapping produces the same value (like `0` or `85` in binary `01010101`). Do not use string conversion; solve it using bitwise operations only.
*/
#include <cstdint>

// Swap adjacent bits (bit 0 with bit 1, bit 2 with 3, etc.) in an 8-bit value.
// Returns the new value after swapping every pair.
uint8_t swapOddEvenBits(uint8_t n) {
    // Even bits (positions 0,2,4,6) move to odd positions (1,3,5,7).
    uint8_t evenBits = (n & 0x55U) << 1;
    // Odd bits (positions 1,3,5,7) move to even positions (0,2,4,6).
    uint8_t oddBits = (n & 0xAAU) >> 1;
    return evenBits | oddBits;
}
#include <cassert>
#include <cstdint>

// Declare the function to test (or include the solution header).
uint8_t swapOddEvenBits(uint8_t n);

int main() {
    assert(swapOddEvenBits(23) == 43);
    assert(swapOddEvenBits(2) == 1);
    assert(swapOddEvenBits(0) == 0);
    assert(swapOddEvenBits(255) == 255);
    assert(swapOddEvenBits(1) == 2);
    assert(swapOddEvenBits(128) == 64);
    assert(swapOddEvenBits(85) == 170);  // 01010101 -> 10101010
    assert(swapOddEvenBits(170) == 85);  // 10101010 -> 01010101
    assert(swapOddEvenBits(0b11110000) == 0b11110000); // pairs 11 11 00 00 -> same
    assert(swapOddEvenBits(0b01001011) == 0b10000111); // 75 -> 135
}
// The task is to swap adjacent bit pairs in an 8-bit integer. The standard approach uses bit masks to extract all even-position bits and all odd-position bits, then shift them appropriately. Specifically, the even bits (positions 0,2,4,6) are at values `0x55` (binary `01010101`). The odd bits (positions 1,3,5,7) are at values `0xAA` (binary `10101010`). To swap: take `n & 0x55`, shift left by 1 to move even bits to odd positions. Take `n & 0xAA`, shift right by 1 to move odd bits to even positions. Then OR the two results. This works because the masks isolate the bits and the shifts move them to their new positions without overlap. Edge cases: `n=0` gives `0`, `n=255` (binary `11111111`) becomes `255` (since every pair is `11` and swapping gives the same), and values like `85` (`01010101`) also stay unchanged because each pair has `01`, and swapping gives `10` which is `170`? Actually need to verify: `85` binary `01010101`, swapping each pair gives `10101010` = `170`. So check carefully. The function should handle any unsigned integer but we restrict to 8-bit because the problem specification uses 8 bits. Time complexity is O(1) with constant bitwise operations. Space complexity O(1).
