/*
Write a C++ function named `clearLowestBits` that takes a signed integer by reference and a non-negative integer `i`, and clears (sets to 0) the lowest `i` bits of the integer, leaving all higher bits unchanged. The function should modify the integer in place and not return a value. You must handle the edge case where `i` is 0 (no bits should be cleared) and where `i` is greater than or equal to the number of bits in the integer (in which case the integer should become 0). Use bitwise operations only, without relying on loops or division. The function must be `const`-correct for its parameters where applicable (note: the integer must be mutable, but `i` can be passed by value or const reference).
*/

#include <cstddef> // for size_t (optional)

// Clear the lowest i bits of num in place.
// If i is 0, num is unchanged. If i is >= bit width of int, num becomes 0.
void clearLowestBits(int &num, int i) {
    // Guard against i being too large, which would cause undefined
    // behavior in the left shift. If i is at least the number of bits
    // in an int, all bits are cleared.
    if (i >= static_cast<int>(sizeof(int) * 8)) {
        num = 0;
        return;
    }
    // Create a mask with 0s in the lowest i bits and 1s elsewhere.
    // Use unsigned int to avoid undefined behavior on signed shift.
    unsigned int mask = (~0U) << i;
    // Apply the mask using bitwise AND.
    num = num & static_cast<int>(mask);
}

#include <cassert>

int main() {
    int value;

    // Clear 0 bits: unchanged
    value = 15;
    clearLowestBits(value, 0);
    assert(value == 15);

    // Clear 1 bit: 15 (1111) -> 14 (1110)
    value = 15;
    clearLowestBits(value, 1);
    assert(value == 14);

    // Clear 2 bits: 15 (1111) -> 12 (1100)
    value = 15;
    clearLowestBits(value, 2);
    assert(value == 12);

    // Clear 3 bits: 15 (1111) -> 8 (1000)
    value = 15;
    clearLowestBits(value, 3);
    assert(value == 8);

    // Clear 4 bits: 15 (1111) -> 0
    value = 15;
    clearLowestBits(value, 4);
    assert(value == 0);

    // Clear 5 bits (greater than 4 for 4-bit representation, but int is 32-bit so 5 is fine)
    value = 255; // 11111111
    clearLowestBits(value, 8);
    assert(value == 0); // because 8 low bits cleared leaves 0 from 255 (which is 0b11111111)

    // Clear 32 bits: int has 32 bits, so all become 0
    value = 12345;
    clearLowestBits(value, 32);
    assert(value == 0);

    // Negative number: -1 is all 1s, clearing 2 bits gives -4 (binary ...11111100)
    value = -1;
    clearLowestBits(value, 2);
    assert(value == -4);

    // Larger than bit width: 33 should also result in 0
    value = 77;
    clearLowestBits(value, 33);
    assert(value == 0);

    // Mixed test with positive number
    value = 0xFFFF; // 65535
    clearLowestBits(value, 16);
    assert(value == 0);

    return 0;
}

// The main algorithm relies on creating a bitmask that has `i` low-order bits set to 0 and all higher bits set to 1. This is achieved by taking the bitwise complement of 0 (which yields all 1s) and left-shifting it by `i` positions. For example, if `i = 2`, `(~0) << 2` becomes `...11111100` in binary. Then, performing a bitwise AND (`&`) between the input integer and this mask clears the lowest `i` bits while preserving the higher bits. Edge cases: When `i = 0`, shifting by 0 gives an all-1s mask, so the integer remains unchanged. When `i` is >= the bit width of the integer (e.g., `i >= 32` for a 32-bit `int`), the shift operation results in undefined behavior in C++ (shifting a signed integer by more than or equal to its bit width). To avoid this, we must guard against large `i` by checking if `i >= sizeof(int)*8` and then setting the integer to 0 directly. Alternatively, one could cast the mask to an unsigned type and use unsigned shifts, but for simplicity and correctness, a conditional check is safer. Time complexity is O(1) as it involves a constant number of bitwise operations and comparisons. Space complexity is O(1) as no auxiliary data structures are used.
//
// Note: The code snippet provided uses `(~0) << i` which is fine for `i < 32` for a 32-bit int, but the challenge here is to handle any non-negative `i` safely. We will also avoid the UB by working with `unsigned int` for the mask when shifting, or by checking the bound first.
