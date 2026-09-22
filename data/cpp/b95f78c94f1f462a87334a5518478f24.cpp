/*
Write a standalone C++ function named `computeOldCipherByte` that implements the core byte-stream transformation from the RAR 3.x old encryption scheme (the `Crypt15` routine shown in the snippet). The function must accept a `std::vector<unsigned char>&` containing the raw data bytes, a `std::vector<unsigned short>&` representing the 4-element 16‑bit `OldKey` state (initialized as shown), and a precomputed lookup table `std::array<unsigned int, 256> crcTab` (the CRC32 table, where each entry stores the full 32‑bit CRC value). The function must modify the `OldKey` state in place (since the cipher is stateful and the state carries across successive calls) and XOR each data byte in-place with the high byte of the current derived key, following the exact sequence of operations: `OldKey[0] += 0x1234; OldKey[1] ^= crcTab[(OldKey[0] & 0x1FE) >> 1]; OldKey[2] -= (crcTab[(OldKey[0] & 0x1FE) >> 1] >> 16); OldKey[0] ^= OldKey[2]; OldKey[3] = ror16(OldKey[3] & 0xFFFF, 1) ^ OldKey[1]; OldKey[3] = ror16(OldKey[3] & 0xFFFF, 1); OldKey[0] ^= OldKey[3]; *Data ^= (unsigned char)(OldKey[0] >> 8);` where `ror16` is a 16‑bit rotate-right by 1. Provide a helper that rotates a 16‑bit value right by one (wrap the most‑significant bit to the least‑significant). The function must operate on any length of data, handle empty vectors gracefully, and ensure that all arithmetic stays within the defined 16‑bit wraparound for `OldKey` entries (use `unsigned short` types). The `crcTab` is a constant reference; the function must not modify it. The function should be `const`‑correct with respect to the data buffer (the buffer itself is mutated, but the vector object is not resized). Time complexity is O(n) for n data bytes, and space complexity is O(1) aside from the input vector itself.
*/
#include <array>
#include <cstdint>
#include <vector>

// Rotate a 16-bit value right by one bit.
inline unsigned short ror16(unsigned short value) {
    return static_cast<unsigned short>((value >> 1) | ((value & 1) << 15));
}

// Apply the RAR 3.x old cipher (Crypt15) to data in-place.
// The `oldKey` state is mutated and must be initialized before the first call.
// The `crcTab` is a precomputed CRC32 table (256 entries).
void computeOldCipherByte(std::vector<unsigned char>& data,
                          std::array<unsigned short, 4>& oldKey,
                          const std::array<unsigned int, 256>& crcTab) {
    for (unsigned char& byte : data) {
        // Update the key state according to the RAR 3.x algorithm.
        oldKey[0] = static_cast<unsigned short>(oldKey[0] + 0x1234);
        unsigned short index = static_cast<unsigned short>((oldKey[0] & 0x1FE) >> 1);
        oldKey[1] ^= static_cast<unsigned short>(crcTab[index]);
        oldKey[2] = static_cast<unsigned short>(oldKey[2] - (crcTab[index] >> 16));
        oldKey[0] ^= oldKey[2];
        oldKey[3] = ror16(static_cast<unsigned short>(oldKey[3] & 0xFFFF)) ^ oldKey[1];
        oldKey[3] = ror16(static_cast<unsigned short>(oldKey[3] & 0xFFFF));
        oldKey[0] ^= oldKey[3];

        // XOR the data byte with the high byte of oldKey[0].
        byte ^= static_cast<unsigned char>(oldKey[0] >> 8);
    }
}
#include <cassert>
#include <array>
#include <vector>

// Dummy CRC table for testing: values chosen to make the algorithm deterministic.
std::array<unsigned int, 256> makeTestCRCTab() {
    std::array<unsigned int, 256> tab{};
    for (unsigned int i = 0; i < 256; ++i) {
        tab[i] = (i * 0x01010101u) ^ 0x12345678u;
    }
    return tab;
}

int main() {
    auto crcTab = makeTestCRCTab();

    // Test 1: Empty vector should do nothing.
    {
        std::vector<unsigned char> data;
        std::array<unsigned short, 4> key = {0x4765, 0x9021, 0x7382, 0x5215};
        std::array<unsigned short, 4> keyCopy = key;
        computeOldCipherByte(data, key, crcTab);
        assert(data.empty());
        assert(key == keyCopy);
    }

    // Test 2: Single byte with known initial key.
    {
        std::vector<unsigned char> data = {0x00};
        std::array<unsigned short, 4> key = {0x0000, 0x0000, 0x0000, 0x0000};
        computeOldCipherByte(data, key, crcTab);
        // Manually compute expected: key[0]=0x1234, index=(0x1234&0x1FE)>>1 = 0x09A,
        // crcTab[0x09A]= (154*0x01010101)^0x12345678 = 0x9A9A9A9A ^ 0x12345678 = 0x88AECCE2
        // key[1]^=0xCCE2 -> 0xCCE2
        // key[2]-=0x88AE -> (0 - 0x88AE) & 0xFFFF = 0x7752
        // key[0]^=0x7752 -> 0x1234 ^ 0x7752 = 0x6566
        // key[3] = ror16(0)^0xCCE2 = 0xCCE2; then ror16(0xCCE2)=0x6671
        // key[0]^=0x6671 -> 0x6566 ^ 0x6671 = 0x0317
        // byte ^= (0x0317 >>8) = 0x03
        assert(data[0] == 0x03);
        // Verify key state after processing.
        assert(key[0] == 0x0317);
        assert(key[1] == 0xCCE2);
        assert(key[2] == 0x7752);
        assert(key[3] == 0x6671);
    }

    // Test 3: Two bytes, state carries over.
    {
        std::vector<unsigned char> data = {0x00, 0xFF};
        std::array<unsigned short, 4> key = {0x0000, 0x0000, 0x0000, 0x0000};
        computeOldCipherByte(data, key, crcTab);
        // First byte result as above: 0x03
        assert(data[0] == 0x03);
        // For the second byte, compute based on state after first byte.
        // key[0] = 0x0317 + 0x1234 = 0x154B, index=(0x154B & 0x1FE)>>1 = (0x014A)>>1=0x0A5
        // crcTab[0x0A5] = (165*0x01010101)^0x12345678 = 0xA5A5A5A5 ^ 0x12345678 = 0xB791F3DD
        // key[1] ^= 0xF3DD -> 0xCCE2 ^ 0xF3DD = 0x3F3F
        // key[2] -= (0xB791) -> 0x7752 - 0xB791 = -0x403F -> 0x1BC1
        // key[0] ^= 0x1BC1 -> 0x154B ^ 0x1BC1 = 0x0E0A
        // key[3] = ror16(0x6671) ^ 0x3F3F = 0x3338 ^ 0x3F3F = 0x0C07
        // key[3] = ror16(0x0C07) = 0x0603
        // key[0] ^= 0x0603 -> 0x0E0A ^ 0x0603 = 0x0809
        // byte ^= (0x0809>>8)=0x08 -> 0xFF ^ 0x08 = 0xF7
        assert(data[1] == 0xF7);
        // Verify final key state.
        assert(key[0] == 0x0809);
        assert(key[1] == 0x3F3F);
        assert(key[2] == 0x1BC1);
        assert(key[3] == 0x0603);
    }

    // Test 4: Ensure external CRC table remains unchanged.
    {
        auto crcTabCopy = crcTab;
        std::vector<unsigned char> data = {0x01, 0x02, 0x03};
        std::array<unsigned short, 4> key = {0x1234, 0x5678, 0x9ABC, 0xDEF0};
        computeOldCipherByte(data, key, crcTab);
        assert(crcTab == crcTabCopy);
    }

    return 0;
}
// The solution is a direct translation of the `Crypt15` routine into a self-contained function. The main algorithm iterates over each byte in the input vector. For each byte, it performs a fixed sequence of arithmetic and bitwise operations on a 4‑entry state vector of 16‑bit values (`OldKey`), using the CRC table as a substitution/feedback source. The crucial parts are: (1) the rotate‑right‑by‑1 helper that must operate on 16‑bit values and wrap the most‑significant bit to the least‑significant position (implemented as `(value >> 1) | ((value & 1) << 15)`), (2) the careful masking of `OldKey[0] & 0x1FE` to obtain an even index into the CRC table, (3) the XOR of each data byte with the high byte of `OldKey[0]` after the state update, and (4) the in‑place mutation of both the data buffer and the state vector. Edge cases include an empty input vector (the loop simply does nothing), a single byte, and ensuring that the state persists correctly across multiple calls (the state is passed by non‑const reference so that the caller can maintain it). Since the CRC table is only read, it can be passed as a `const` reference to an `std::array<unsigned int, 256>`. The time complexity is O(n), and space complexity is O(1) because only the loop variable and temporary values are used.
