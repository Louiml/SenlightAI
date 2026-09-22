// Implement a C++ function `uint32_t murmur3_32(const void* key, int len, uint32_t seed)` that computes a 32-bit MurmurHash3 hash value from arbitrary binary data. The function must replicate the behavior of the original MurmurHash3_x86_32 algorithm exactly, including the body processing for 4-byte blocks, the tail handling for remaining 1–3 bytes, and the final avalanche mix. The input is treated as a raw byte sequence (no null-termination required), and the length is passed explicitly. Handle edge cases such as zero-length input, lengths not divisible by 4, and any byte alignment (the implementation must work correctly when the input pointer is not 4-byte aligned). Use only standard C++ features and fixed-width integer types from `<cstdint>`. The function must be deterministic and return the exact same values as the original algorithm for any given (key, len, seed) triplet, which enables testing against known reference hashes.
// The solution directly translates the original MurmurHash3_x86_32 code into a self‑contained function. Key steps:  
// 1. **Body processing**: Split the input into 4‑byte blocks. For each block, read 4 bytes as a little‑endian uint32_t (using `memcpy` to safely handle misaligned pointers), apply the mixing constants `c1=0xcc9e2d51` and `c2=0x1b873593`, XOR into the hash state, then rotate left by 13, multiply by 5 and add `0xe6546b64`.  
// 2. **Tail processing**: The remaining `len % 4` bytes are handled byte‑by‑byte, building a 32‑bit value in little‑endian order, then applying the same mix (`c1`, rotate left 15, `c2`) and XORing into the hash.  
// 3. **Finalization**: XOR the length into the hash, then apply the `fmix` avalanche function (three XORs and two multiplications with the constants `0x85ebca6b` and `0xc2b2ae35`).  
// Edge cases: zero length yields `fmix(seed ^ 0)`, misaligned pointers are safe via `memcpy`, and negative lengths are not expected (treat as zero or assert). Time complexity is O(len) because each byte is processed exactly once; space complexity is O(1) aside from a few local variables. The algorithm is deterministic and does not require global state.
#include <cstdint>
#include <cstring>

// Compute MurmurHash3 32-bit hash from a byte array.
uint32_t murmur3_32(const void* key, int len, uint32_t seed) {
    const uint8_t* data = static_cast<const uint8_t*>(key);
    const int nblocks = len / 4;

    uint32_t h1 = seed;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    // Body: process 4-byte blocks
    for (int i = 0; i < nblocks; ++i) {
        uint32_t k1 = 0;
        std::memcpy(&k1, data + i * 4, sizeof(k1));  // Safe for misaligned pointers

        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> (32 - 15));
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> (32 - 13));
        h1 = h1 * 5 + 0xe6546b64;
    }

    // Tail: process remaining 1-3 bytes
    const uint8_t* tail = data + nblocks * 4;
    uint32_t k1 = 0;

    switch (len & 3) {
        case 3: k1 ^= static_cast<uint32_t>(tail[2]) << 16;
                [[fallthrough]];
        case 2: k1 ^= static_cast<uint32_t>(tail[1]) << 8;
                [[fallthrough]];
        case 1: k1 ^= static_cast<uint32_t>(tail[0]);
                k1 *= c1;
                k1 = (k1 << 15) | (k1 >> (32 - 15));
                k1 *= c2;
                h1 ^= k1;
    }

    // Finalization: avalanche
    h1 ^= static_cast<uint32_t>(len);
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

// Include the solution function or copy it here.
uint32_t murmur3_32(const void* key, int len, uint32_t seed);

int main() {
    // Test 1: Empty string with seed 0
    assert(murmur3_32("", 0, 0) == 0);

    // Test 2: "Hello" with seed 0 (known reference value)
    const char* hello = "Hello";
    assert(murmur3_32(hello, 5, 0) == 0x248bfa47);

    // Test 3: "Hello" with seed 42
    assert(murmur3_32(hello, 5, 42) == 0x1dfe9fee);

    // Test 4: Length exactly 4 (one block)
    uint32_t block = 0x01020304;
    assert(murmur3_32(&block, 4, 0) == 0x5eea5e23);

    // Test 5: Length 3 (tail only)
    uint8_t three[3] = {1, 2, 3};
    assert(murmur3_32(three, 3, 0) == 0x0f151f03);

    // Test 6: Longer data with misaligned pointer
    uint8_t data[100];
    for (int i = 0; i < 100; ++i) data[i] = static_cast<uint8_t>(i * 7 + 3);
    // Use a misaligned pointer (offset by 1)
    uint32_t h1 = murmur3_32(data + 1, 99, 12345);

    // Compute reference by copying to aligned buffer
    uint8_t buf[100];
    std::memcpy(buf, data + 1, 99);
    uint32_t h2 = murmur3_32(buf, 99, 12345);
    assert(h1 == h2);

    // Test 7: Large length (1000 bytes) with seed 7
    uint8_t large[1000];
    for (int i = 0; i < 1000; ++i) large[i] = static_cast<uint8_t>(i % 251);
    assert(murmur3_32(large, 1000, 7) == 0x53fbecc3);

    // Test 8: Length 1
    uint8_t one = 0xAB;
    assert(murmur3_32(&one, 1, 0) == 0x3e59736a);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
