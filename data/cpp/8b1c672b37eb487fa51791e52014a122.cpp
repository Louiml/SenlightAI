// Write a C++ function that takes an `unsigned char` value (0–255) as input, swaps its high nibble (bits 7–4) with its low nibble (bits 3–0), and returns the resulting `unsigned char`. The function should handle all possible `unsigned char` values, including 0 and 255, and must not rely on external libraries beyond the standard `<cstdint>` or `<iostream>`. The operation is equivalent to `(input << 4) | (input >> 4)`, but must be implemented using only bitwise shifts and/or masks. The function should be `const`-correct and operate on a copy of the input.
The solution is straightforward: to swap the nibbles of an 8-bit value, we can shift the entire byte left by 4 positions and then OR it with the same byte shifted right by 4 positions. However, because `unsigned char` promotes to `int` in arithmetic operations, we must explicitly cast the result back to `unsigned char` to avoid sign issues or truncation warnings. The left shift `input << 4` moves the low nibble into the high nibble position, and the right shift `input >> 4` moves the high nibble into the low nibble position (since `unsigned char` is unsigned, right shift is logical, filling with zeros). ORing these two shifted values combines the swapped nibbles. For example, input `0xAB` becomes `0xBA`. Edge cases: input `0x00` returns `0x00`, input `0xFF` returns `0xFF`. Time complexity is O(1) and space complexity is O(1), as no additional storage is needed.
#include <cstdint>

// Swap the high and low nibbles of an unsigned char and return the result.
unsigned char swapNibbles(unsigned char value) {
    // Shift left and right by 4 bits, then combine with bitwise OR.
    // Cast back to unsigned char to avoid implicit int conversion.
    return static_cast<unsigned char>((value << 4) | (value >> 4));
}
#include <cassert>

int main() {
    // Basic nibble swap
    assert(swapNibbles(0x12) == 0x21);
    assert(swapNibbles(0xAB) == 0xBA);
    // Boundary values
    assert(swapNibbles(0x00) == 0x00);
    assert(swapNibbles(0xFF) == 0xFF);
    // Values with identical nibbles
    assert(swapNibbles(0x11) == 0x11);
    // Random values
    assert(swapNibbles(0x34) == 0x43);
    assert(swapNibbles(0x56) == 0x65);
    assert(swapNibbles(0x78) == 0x87);
    // Low nibble zero
    assert(swapNibbles(0x10) == 0x01);
    // High nibble zero
    assert(swapNibbles(0x01) == 0x10);
}
