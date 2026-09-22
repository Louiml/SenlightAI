Write a C++ function named `findFirstFreeSlot` that takes a pointer to a contiguous array of bytes (`unsigned char`) and the total number of bits available in that array, and returns the index of the first bit that is set to `0`. The bits are numbered from `0` (least significant bit of the first byte) upward, so bit `i` is located at byte `i/8`, within the mask `1 << (i % 8)`. The array is guaranteed to contain at least one zero bit. If no zero bit exists within the given number of bits, the function should return `-1` to indicate failure. You may **not** use standard library bit manipulation utilities; you must implement bit access manually.
The solution scans the byte array byte-by-byte, skipping any byte that is fully set (`0xFF`), because such a byte contains no zero bits. For each non-fully-set byte, the function examines each of its 8 bits (from least significant to most significant) using a manual mask and bitwise AND. The first time it finds a bit where the result is `0`, it computes the global bit index as `(byte_offset * 8 + bit_offset)` and returns it. Important edge cases: (1) the array may be empty or the number of bits may be zero — in that case, the function immediately returns `-1` because no zero bit exists; (2) the number of bits might not be a multiple of 8, so the last byte may have unused high bits that must be ignored — the function must stop scanning at `totalBits` and not report a false zero in the unused region; (3) If the array contains bytes that are not all `0xFF` but the zero bits only occur beyond `totalBits`, the function must return `-1`. Time complexity is O(totalBits/8 + 8) = O(totalBits), but in practice it stops at the first non-fully-set byte, so worst-case is O(totalBits) when all bytes are `0xFF` except possibly the last. Space complexity is O(1).
#include <cstddef>

// Returns the index of the first zero bit in the bit array, or -1 if none exists within totalBits.
int findFirstFreeSlot(const unsigned char* array, size_t totalBits) {
    if (array == nullptr || totalBits == 0) {
        return -1;
    }

    size_t fullBytes = totalBits / 8;
    size_t remainingBits = totalBits % 8;

    // Scan full bytes.
    for (size_t byteIdx = 0; byteIdx < fullBytes; ++byteIdx) {
        unsigned char byte = array[byteIdx];
        if (byte == 0xFF) {
            continue; // All bits set in this byte, skip.
        }
        // Find the first zero bit in this byte.
        for (int bit = 0; bit < 8; ++bit) {
            unsigned char mask = static_cast<unsigned char>(1 << bit);
            if ((byte & mask) == 0) {
                return static_cast<int>(byteIdx * 8 + bit);
            }
        }
    }

    // Check the partial last byte, if any.
    if (remainingBits > 0) {
        unsigned char byte = array[fullBytes];
        if (byte != 0xFF) { // Only check if there might be a zero bit.
            for (size_t bit = 0; bit < remainingBits; ++bit) {
                unsigned char mask = static_cast<unsigned char>(1 << bit);
                if ((byte & mask) == 0) {
                    return static_cast<int>(fullBytes * 8 + bit);
                }
            }
        }
    }

    return -1; // No zero bit found within the given range.
}
#include <cassert>

int main() {
    // Case 1: first zero is bit 0 in the first byte.
    unsigned char a1[1] = {0x7F}; // bits 0..6 set, bit 7 zero
    assert(findFirstFreeSlot(a1, 8) == 7);

    // Case 2: first zero is bit 3 in the first byte.
    unsigned char a2[1] = {0xF7}; // binary 11110111, bit 3 is zero
    assert(findFirstFreeSlot(a2, 8) == 3);

    // Case 3: first byte fully set, second byte has zero at bit 1.
    unsigned char a3[2] = {0xFF, 0xFD}; // second byte: 11111101, bit 1 zero
    assert(findFirstFreeSlot(a3, 16) == 9);

    // Case 4: all bits zero in the first byte.
    unsigned char a4[1] = {0x00};
    assert(findFirstFreeSlot(a4, 8) == 0);

    // Case 5: array with all bits set but totalBits limited to 8, and the only zero is outside range.
    unsigned char a5[2] = {0xFF, 0x00};
    assert(findFirstFreeSlot(a5, 8) == -1);

    // Case 6: partial last byte, only first 4 bits considered.
    unsigned char a6[1] = {0x0F}; // bits 0-3 set, bits 4-7 zero but not considered
    assert(findFirstFreeSlot(a6, 4) == -1);

    // Case 7: partial last byte, zero inside the considered range.
    unsigned char a7[1] = {0x0E}; // bits 1-3 set, bit 0 zero
    assert(findFirstFreeSlot(a7, 4) == 0);

    // Case 8: empty array or zero totalBits.
    unsigned char a8[1] = {0x00};
    assert(findFirstFreeSlot(a8, 0) == -1);
    assert(findFirstFreeSlot(nullptr, 8) == -1);

    // Case 9: first byte has zero at bit 5, but totalBits is 8.
    unsigned char a9[1] = {0xDF}; // 11011111, bit 5 zero
    assert(findFirstFreeSlot(a9, 8) == 5);

    return 0;
}
