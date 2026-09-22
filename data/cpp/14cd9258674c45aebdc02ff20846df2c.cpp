// Write a standalone C++ function named `murmurHash3` that takes a 32-bit unsigned integer seed and a `std::vector<unsigned char>` data buffer, and returns a 32-bit unsigned integer hash using the MurmurHash3 x86_32 algorithm. The implementation must be self-contained (no external libraries beyond standard C++ headers), must handle empty input correctly (returning the finalized seed with length 0), and must process data in 4-byte little-endian blocks followed by a tail of 1–3 bytes. Use the standard MurmurHash3 constants (`0xcc9e2d51`, `0x1b873593`, `0x85ebca6b`, `0xc2b2ae35`, `0xe6546b64`) and the exact mixing operations as specified in the canonical algorithm. The returned value is an unsigned 32-bit integer (the function should return `uint32_t`).
#include <cassert>
#include <vector>
#include <cstdint>

// Declare the function under test (already defined in the solution)
uint32_t murmurHash3(uint32_t seed, const std::vector<unsigned char>& data);

int main() {
    // Test 1: Empty input, seed 0
    assert(murmurHash3(0, {}) == 0);

    // Test 2: Empty input, non-zero seed (finalization still applies)
    std::vector<unsigned char> empty;
    assert(murmurHash3(12345, empty) == 1764350154u); // Known value for seed 12345 with empty data

    // Test 3: Single byte
    std::vector<unsigned char> one = {0x42};
    assert(murmurHash3(0, one) == 2926514893u);

    // Test 4: Exactly 4 bytes (full block, no tail)
    std::vector<unsigned char> four = {0x01, 0x02, 0x03, 0x04};
    assert(murmurHash3(0, four) == 1719334978u);

    // Test 5: 5 bytes (one block + one tail byte)
    std::vector<unsigned char> five = {0x01, 0x02, 0x03, 0x04, 0x05};
    assert(murmurHash3(0, five) == 372591401u);

    // Test 6: 7 bytes (one block + three tail bytes)
    std::vector<unsigned char> seven = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
    assert(murmurHash3(0, seven) == 2596644869u);

    // Test 7: Larger input, non-zero seed
    std::vector<unsigned char> data;
    for (uint8_t i = 0; i < 100; ++i) {
        data.push_back(i * 3);
    }
    // Computed with a verified reference implementation
    assert(murmurHash3(987654321u, data) == 704033118u);

    return 0;
}
#include <cstdint>
#include <vector>

// Helper: rotate left 32-bit integer by r bits (r must be in 1..31)
inline uint32_t rotl32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

// Compute MurmurHash3 x86_32 for a given seed and data.
uint32_t murmurHash3(uint32_t seed, const std::vector<unsigned char>& data) {
    uint32_t h1 = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const size_t len = data.size();

    if (len > 0) {
        // Process full 4-byte blocks
        const size_t nblocks = len / 4;
        const uint8_t* blocks = data.data() + nblocks * 4;

        for (size_t i = 0; i < nblocks; ++i) {
            // Read little-endian 32-bit word
            uint32_t k1 = static_cast<uint32_t>(blocks[i * 4]) |
                          (static_cast<uint32_t>(blocks[i * 4 + 1]) << 8) |
                          (static_cast<uint32_t>(blocks[i * 4 + 2]) << 16) |
                          (static_cast<uint32_t>(blocks[i * 4 + 3]) << 24);

            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = rotl32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        // Handle tail (0–3 remaining bytes)
        const uint8_t* tail = data.data() + nblocks * 4;
        uint32_t k1 = 0;

        switch (len & 3) {
            case 3:
                k1 ^= static_cast<uint32_t>(tail[2]) << 16;
                [[fallthrough]];
            case 2:
                k1 ^= static_cast<uint32_t>(tail[1]) << 8;
                [[fallthrough]];
            case 1:
                k1 ^= static_cast<uint32_t>(tail[0]);
                k1 *= c1;
                k1 = rotl32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
            default:
                break;
        }
    }

    // Finalization
    h1 ^= static_cast<uint32_t>(len);
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}
// The solution replicates the MurmurHash3 x86_32 algorithm. First, the seed is copied into `h1`. If the input vector is empty, no body or tail processing is performed; the finalization step is still applied, which XORs the length (0) and does three mixing multiplications/shifts. For non‑empty input, we process the main body in chunks of 4 bytes: for each 32‑bit little‑endian word `k1`, multiply by `c1`, rotate left by 15, multiply by `c2`, XOR into `h1`, rotate left by 13, multiply by 5 and add the constant `0xe6546b64`. Then we handle the remaining 0–3 bytes in the tail: the bytes are assembled into `k1` using shifts of 16, 8, and 0 accordingly, and then the same `k1` mixing (multiply c1, rotate 15, multiply c2) is applied and XORed into `h1`. Finally, the result is finalized: XOR with the total length, then apply the three‑step avalanche (right shift 16, multiply `0x85ebca6b`, right shift 13, multiply `0xc2b2ae35`, right shift 16). Edge cases include an empty vector (only finalization), a size not divisible by 4 (tail logic), and ensuring all operations use unsigned 32‑bit arithmetic to avoid undefined behavior. Time complexity is O(n) where n is the number of bytes, and space complexity is O(1) beyond the input vector itself.
