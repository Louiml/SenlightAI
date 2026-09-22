Write a standalone C++ function `uint32_t reverseBytes(uint32_t value)` that returns the input 32-bit unsigned integer with its bytes reversed in big-endian order. That is, given a value stored in native little-endian byte order on typical x86 systems, the function must reorder its four bytes so that the most significant byte becomes the least significant byte, the second most significant becomes the second least, and so on. The function must not depend on the host's endianness; it should work identically on both little‑endian and big‑endian machines by extracting each byte explicitly using bit shifts and masks. You must implement the byte extraction logic manually using bit operations only—do not use `std::reverse`, `memcpy`, unions, or any compiler built-ins like `__builtin_bswap32`. The solution should be a single free function with the specified signature, and it must handle all possible `uint32_t` inputs, including `0`, `0xFFFFFFFF`, and values with repeated byte patterns.
// The core idea is to decompose the 32‑bit input into four bytes, then reassemble them in reverse order. Extract each byte by shifting the value right by `8 * position` and masking with `0xFF` to isolate the byte. For example, the least significant byte (position 0) is obtained as `(value >> 0) & 0xFF`, the next as `(value >> 8) & 0xFF`, and so on. To reverse, place the original byte at position 0 into the destination at position 3 (i.e., `(byte0 << 24)`), the original byte at position 1 into position 2 (`(byte1 << 16)`), the original byte at position 2 into position 1 (`(byte2 << 8)`), and the original byte at position 3 into position 0 (`byte3`). Combining these with bitwise OR yields the correctly reversed value. This approach is endian-independent because it treats the integer purely as a sequence of bits, not relying on how the host stores it in memory. Edge cases include all-zero (`0`) which maps to `0`, and `0xFFFFFFFF` which maps to itself since all bytes are identical; both naturally fall out of the manual extraction. The algorithm runs in constant time \(O(1)\) and uses constant extra space \(O(1)\), as it only involves a fixed number of shifts, masks, and OR operations.
#include <cstdint>

// Reverse the bytes of a 32-bit unsigned integer.
// The result has the byte order reversed: byte0 becomes byte3, byte1 becomes byte2, etc.
// This function is endian-independent and uses only bit operations.
uint32_t reverseBytes(uint32_t value) {
    // Extract each byte.
    uint32_t byte0 = (value >> 24) & 0xFF;  // Most significant byte.
    uint32_t byte1 = (value >> 16) & 0xFF;  // Second byte.
    uint32_t byte2 = (value >> 8)  & 0xFF;  // Third byte.
    uint32_t byte3 = value         & 0xFF;  // Least significant byte.

    // Reassemble in reversed order.
    return (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0;
}
#include <cassert>
#include <cstdint>

// Declaration of the function being tested.
uint32_t reverseBytes(uint32_t value);

int main() {
    // Simple case: 0x12345678 reversed becomes 0x78563412.
    assert(reverseBytes(0x12345678u) == 0x78563412u);

    // Zero remains zero.
    assert(reverseBytes(0x00000000u) == 0x00000000u);

    // All ones remains all ones (each byte is 0xFF).
    assert(reverseBytes(0xFFFFFFFFu) == 0xFFFFFFFFu);

    // Single byte set (0xAB000000) becomes 0x000000AB.
    assert(reverseBytes(0xAB000000u) == 0x000000ABu);

    // Single byte at LSB (0x000000CD) becomes 0xCD000000.
    assert(reverseBytes(0x000000CDu) == 0xCD000000u);

    // Reversing twice returns the original.
    assert(reverseBytes(reverseBytes(0xDEADBEEFu)) == 0xDEADBEEFu);

    // Pattern with repeating bytes.
    assert(reverseBytes(0x01020304u) == 0x04030201u);

    // Value with alternating bits.
    assert(reverseBytes(0xAAAAAAAAu) == 0xAAAAAAAAu);
    assert(reverseBytes(0x55555555u) == 0x55555555u);

    // Maximum non-symmetric value.
    assert(reverseBytes(0x80000001u) == 0x01000080u);
}
