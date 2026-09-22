// Write a C++ function `ctr_seek` that simulates the counter update logic of the CTR mode block cipher from the given snippet. Given an initial 16-byte counter array (`std::array<uint8_t, 16>`) and a 64-bit iteration count, the function must update the counter in place by adding the iteration count to the big-endian counter value, handling carry propagation across bytes. The counter is treated as a 128-bit big-endian integer (most significant byte at index 0, least significant at index 15). The function should handle iteration counts up to `2^64 - 1` and correctly wrap around if adding causes overflow beyond the 128-bit range (simulate modulo 2^128). The original snippet’s `SeekToIteration` only updates the low 8 bytes from a `lword`; your function must be general and update the entire 16-byte counter with the given 64-bit value. Ensure the function is `const`-correct with respect to the input parameter (it takes the array by non-const reference since it modifies it). Provide only the function, not a main program.

// The core algorithm mirrors the manual addition of a 64-bit value to a 128-bit big-endian counter, similar to how you would add numbers by hand from least significant digit to most significant. Start from the last byte (index 15) and work backward to index 8 (since the iteration count is 64-bit, it can only affect the low 8 bytes; higher bytes remain unchanged unless there’s an overflow carry that propagates into the upper bytes). For each position from 15 down to 8, compute `sum = current_byte + (iterationCount & 0xFF) + carry`. The carry is initially 0, and for each byte, the new byte value is `(byte)sum`, and the carry is `sum >> 8` (0 or 1). After processing byte 15, shift the iteration count right by 8 bits for the next byte. After processing byte 8, if there is a carry left (which can happen only if the 64-bit addition overflows into the 65th bit, but that’s impossible for a 64-bit input; however, the carry can propagate into bytes 7..0 if the low 8 bytes are all 0xFF and the iteration count causes an overflow beyond the low 64 bits—this is the case when `m_register` has 0xFF in bytes 8–15 and we add a value that overflows the 64-bit boundary, producing a carry into byte 7). To be safe and match the spirit of the original (which uses a 128-bit counter), handle the carry propagation through all 16 bytes, not just the low 8. The original `SeekToIteration` only updates the low 8 bytes and ignores carry into the upper 8 bytes because `lword` is likely 64-bit and the counter is incremented by iteration count modulo 2^64; however, the counter is conceptually 128-bit, and in practice the upper bytes are used for nonce/IV and are not incremented by `SeekToIteration`. Since the task says "update the entire 16-byte counter with the given 64-bit value", you should add the 64‑bit value to the entire 128‑bit counter, which means the low 8 bytes are added directly, and any carry propagates into the upper 8 bytes. The algorithm: for i from 15 down to 0, if i >= 8 then add `(iterationCount >> (8*(i-8))) & 0xFF` plus carry; else just add carry. But since `iterationCount` is 64-bit, for i >= 8 we extract the corresponding byte, and for i < 8 we only add carry. This is straightforward. Edge case: if the whole 128-bit counter is all 0xFF and we add 1, it wraps to 0 (mod 2^128). Time complexity O(16) = O(1) per call, space O(1). Correctness: manual big-endian addition with carry propagation.

#include <array>
#include <cstdint>

// Add a 64-bit iteration count to a 128-bit big-endian counter in-place.
// The counter is stored as 16 bytes, most significant byte at index 0.
// The addition is performed modulo 2^128 (wrap-around on overflow).
void ctr_seek(std::array<uint8_t, 16>& counter, uint64_t iterationCount) {
    uint8_t carry = 0;
    // Process from least significant byte (index 15) to most significant (index 0).
    for (int i = 15; i >= 0; --i) {
        // For bytes 8..15, extract the corresponding byte from iterationCount.
        uint8_t addend = 0;
        if (i >= 8) {
            // The byte at index i corresponds to bits 8*(i-8) .. 8*(i-8)+7.
            addend = static_cast<uint8_t>(iterationCount >> (8 * (i - 8)));
        }
        uint16_t sum = static_cast<uint16_t>(counter[i]) + addend + carry;
        counter[i] = static_cast<uint8_t>(sum & 0xFF);
        carry = static_cast<uint8_t>(sum >> 8);
    }
    // 'carry' is ignored for the final wrap-around (modulo 2^128).
}

#include <array>
#include <cassert>
#include <cstdint>

// Function declaration (from solution)
void ctr_seek(std::array<uint8_t, 16>& counter, uint64_t iterationCount);

int main() {
    // Test 1: Add 1 to zero counter.
    std::array<uint8_t, 16> c1{};
    ctr_seek(c1, 1);
    std::array<uint8_t, 16> expected1{};
    expected1[15] = 1;
    assert(c1 == expected1);

    // Test 2: Add 256 (0x100) to zero counter -> sets byte 14 to 1.
    std::array<uint8_t, 16> c2{};
    ctr_seek(c2, 256);
    std::array<uint8_t, 16> expected2{};
    expected2[14] = 1;
    assert(c2 == expected2);

    // Test 3: Counter with low 8 bytes all 0xFF, add 1 -> carry into byte 7.
    std::array<uint8_t, 16> c3{};
    c3[8] = 0xFF; c3[9] = 0xFF; c3[10] = 0xFF; c3[11] = 0xFF;
    c3[12] = 0xFF; c3[13] = 0xFF; c3[14] = 0xFF; c3[15] = 0xFF;
    ctr_seek(c3, 1);
    std::array<uint8_t, 16> expected3{};
    expected3[7] = 1; // carry propagated into byte 7
    assert(c3 == expected3);

    // Test 4: Counter all 0xFF, add 1 -> wrap to all zeros.
    std::array<uint8_t, 16> c4;
    c4.fill(0xFF);
    ctr_seek(c4, 1);
    std::array<uint8_t, 16> expected4{};
    assert(c4 == expected4);

    // Test 5: Add large value to a non-zero counter.
    std::array<uint8_t, 16> c5{};
    c5[15] = 0xFE;
    ctr_seek(c5, 3); // 0xFE + 3 = 0x101 -> byte becomes 0x01, carry to byte 14
    std::array<uint8_t, 16> expected5{};
    expected5[14] = 1;
    expected5[15] = 1;
    assert(c5 == expected5);

    // Test 6: Add 0 to a counter (no change).
    std::array<uint8_t, 16> c6{};
    c6[0] = 0x12; c6[15] = 0x34;
    auto original = c6;
    ctr_seek(c6, 0);
    assert(c6 == original);

    // Test 7: Add max uint64_t to a zero counter.
    std::array<uint8_t, 16> c7{};
    ctr_seek(c7, UINT64_MAX);
    std::array<uint8_t, 16> expected7{};
    for (int i = 8; i < 16; ++i) expected7[i] = 0xFF;
    assert(c7 == expected7);

    // Test 8: Add 2 to a counter with value 0xFFFFFFFFFFFFFFFF (low 64 bits).
    std::array<uint8_t, 16> c8{};
    for (int i = 8; i < 16; ++i) c8[i] = 0xFF;
    ctr_seek(c8, 2);
    std::array<uint8_t, 16> expected8{};
    expected8[7] = 1; // carry into byte 7
    expected8[8] = 1; // low 64 bits wrap to 1
    assert(c8 == expected8);
}
