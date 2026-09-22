// Write a C++ function that takes a 32-bit unsigned integer and returns an unsigned integer whose bits are the exact reverse of the input's bits. For example, if the input has its least significant bit set (bit 0 = 1), then the output must have its most significant bit set (bit 31 = 1). The function must consider all 32 bits, including leading zeros, and return the fully reversed value. Handle any valid `uint32_t` input without undefined behavior.
// The core idea is to iterate over the 32 bits of the input from the least significant bit (LSB) to the most significant bit (MSB). For each bit position `i` (from 0 to 31), we extract the current LSB using `(n & 1)`. We then place that bit into the result at the mirrored position: the bit from position `i` must go to position `31 - i`. We use a bitwise OR to set that bit in the result: `result |= ((n & 1) << (31 - i))`. After processing a bit, we shift the input right by one (`n >>= 1`) to inspect the next bit. After 32 iterations, the input becomes zero and will not affect anything else. Edge cases include inputs with all zeros (result remains zero), all ones (result is all ones, since reversing all ones yields all ones), and any single-bit set — that bit will be mirrored exactly. The algorithm runs in exactly 32 iterations, so time complexity is O(1) (constant, since the number of bits is fixed). Space complexity is O(1), using only a few local variables.
#include <cstdint>

// Reverse the 32 bits of a 32-bit unsigned integer.
// Returns a new uint32_t whose bit order is the reverse of the input's.
std::uint32_t reverseBits(std::uint32_t n) {
    std::uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        // Extract the least significant bit and place it at the mirrored position.
        result |= ((n & 1U) << (31 - i));
        n >>= 1;  // Move to the next bit of the original input.
    }
    return result;
}
#include <cassert>
#include <cstdint>

// Function prototype (declaration) for the solution function.
std::uint32_t reverseBits(std::uint32_t n);

int main() {
    // Reversing zero yields zero.
    assert(reverseBits(0) == 0);
    // Reversing UINT32_MAX (all ones) yields itself (all ones).
    assert(reverseBits(0xFFFFFFFFU) == 0xFFFFFFFFU);
    // Single LSB set -> becomes single MSB set (bit 31).
    assert(reverseBits(1) == 0x80000000U);
    // Single MSB set -> becomes single LSB set.
    assert(reverseBits(0x80000000U) == 1);
    // Alternating pattern: 0xAAAAAAAA (bits 1,3,5,... set) reversed -> 0x55555555.
    assert(reverseBits(0xAAAAAAAAU) == 0x55555555U);
    // A small number: 5 = 0b101 -> reversed over 32 bits: 0xA0000000.
    assert(reverseBits(5) == 0xA0000000U);
    // 0x12345678 reversed (known value).
    assert(reverseBits(0x12345678U) == 0x1E6A2C48U);
    // 0x01020304 reversed.
    assert(reverseBits(0x01020304U) == 0x20C04080U);
    // Another pattern: 0x0F0F0F0F reversed -> 0xF0F0F0F0.
    assert(reverseBits(0x0F0F0F0FU) == 0xF0F0F0F0U);
    // Value with alternating bits from LSB: 0x55555555 reversed -> 0xAAAAAAAA.
    assert(reverseBits(0x55555555U) == 0xAAAAAAAAU);
    return 0;
}
