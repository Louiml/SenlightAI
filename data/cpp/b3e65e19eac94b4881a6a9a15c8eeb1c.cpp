/*
Write a C++ function `template <typename T> std::string binaryReverseString(T n)` that returns a string representing the binary of the input integer `n` with its bits reversed (mirrored) across the full width of type `T`. The returned string must be exactly `sizeof(T) * 8` characters long (no leading zeros trimmed), showing the reversed bit pattern. The input `n` is an unsigned integral type (e.g., `unsigned int`, `unsigned long`). The function should not print anything; it should only return the binary string of the reversed bits. Handle the case where `n` is zero, where the result is a string of all zeros of the appropriate length. Also handle the case where the type has more than 32 bits (e.g., `unsigned long long`) without overflow.
*/
#include <string>
#include <bitset>
#include <cstdint>

// Return a string of length sizeof(T)*8 representing the binary of the input integer with bits reversed.
template <typename T>
std::string binaryReverseString(T n) {
    constexpr std::size_t numBits = sizeof(T) * 8;
    T reversed = 0;

    // Iterate over each bit of the input.
    for (std::size_t i = 0; i < numBits; ++i) {
        // Use a 64-bit mask to handle types wider than 32 bits.
        if (n & (static_cast<std::uint64_t>(1) << i)) {
            reversed |= static_cast<T>(static_cast<std::uint64_t>(1) << (numBits - 1 - i));
        }
    }

    // Convert the reversed integer to a fixed-width binary string.
    return std::bitset<numBits>(reversed).to_string();
}
#include <cassert>
#include <string>
#include <cstdint>

// The template function is defined above (included from the solution).

int main() {
    // unsigned int, 32 bits: input 0 -> all zeros
    assert(binaryReverseString(0u) == "00000000000000000000000000000000");

    // unsigned int, 32 bits: input 1 -> leftmost bit becomes 1
    assert(binaryReverseString(1u) == "10000000000000000000000000000000");

    // unsigned int, 32 bits: input 0x80000000 -> rightmost bit becomes 1
    assert(binaryReverseString(0x80000000u) == "00000000000000000000000000000001");

    // unsigned int: input with alternating pattern 0xAAAAAAAA (1010...)
    assert(binaryReverseString(0xAAAAAAAAu) == "01010101010101010101010101010101");

    // unsigned short (16 bits): input 0x0001 -> reversed 0x8000
    assert(binaryReverseString(static_cast<unsigned short>(0x0001)) == "1000000000000000");

    // unsigned long long (64 bits): input 1ULL -> leftmost bit becomes 1
    assert(binaryReverseString(1ULL) == "1000000000000000000000000000000000000000000000000000000000000000");

    // unsigned char (8 bits): input 0x0F -> reversed 0xF0
    assert(binaryReverseString(static_cast<unsigned char>(0x0F)) == "11110000");

    // unsigned int: all bits set -> reversed remains all bits set
    assert(binaryReverseString(0xFFFFFFFFu) == "11111111111111111111111111111111");

    // unsigned int: middle bit only
    assert(binaryReverseString(0x00010000u) == "00000000000000001000000000000000");

    // unsigned long (assuming 64-bit on this platform): input 0x8000000000000000ULL -> rightmost bit
    assert(binaryReverseString(static_cast<unsigned long>(0x8000000000000000ULL)) == "0000000000000000000000000000000000000000000000000000000000000001");

    return 0;
}
// The core algorithm iterates over each bit position from 0 to `numBits-1` (where `numBits = sizeof(T)*8`). For each bit `i`, we check if the bit is set in `n` using a bitmask `(1ULL << i)`. If set, we set the corresponding mirrored bit in the reversed value at position `numBits - 1 - i`. To build the output string, we can either construct the reversed integer first and then convert it to a binary string using `std::bitset<numBits>`, or directly build the string character by character. Using `std::bitset` is simpler and avoids manual character loops. Edge cases: zero input produces an all-zeros string; for types wider than 32 bits, we must use a 64-bit shift mask (e.g., `1ULL`) to avoid overflow; the bitwise operations work correctly on unsigned types because they are well-defined. Time complexity is O(numBits) per call, and space complexity is O(numBits) for the returned string (which is required).
