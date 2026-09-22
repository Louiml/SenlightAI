// Write a C++ function named `murmur3_32` that takes a 32-bit unsigned integer seed and a `std::vector<unsigned char>` of arbitrary length, and returns a 32-bit unsigned integer hash computed using the MurmurHash3 x86_32 algorithm. The implementation must handle empty input vectors gracefully (returning the finalized seed without processing a body or tail), correctly process the data in little-endian 4-byte blocks, handle any remaining 1–3 tail bytes, and apply the exact finalization steps (mixing with XOR shifts and multiplications) as described in the standard MurmurHash3 specification. The function must be `const`-correct, accept data by `const&`, and use only standard headers.
The core algorithm processes the input in three phases: body (full 4-byte blocks), tail (remaining bytes), and finalization. For the body, iterate over each complete 32-bit word in little-endian order—care must be taken to read the bytes correctly on any platform, so we should assemble the word byte-by-byte rather than casting pointers, to avoid alignment and endianness issues. For each block, apply the constants `c1 = 0xcc9e2d51` and `c2 = 0x1b873593`, mixing the block with a left rotate by 15, multiply by `c2`, XOR into the hash, rotate the hash left by 13, multiply by 5, and add `0xe6546b64`. The tail phase handles the leftover 1–3 bytes by building a `k1` word from the remaining bytes (shifted appropriately), mixing it once with `c1`, rotating by 15, multiplying by `c2`, and XORing into the hash. Finally, XOR the hash with the total data length, then apply three mixing operations: `h ^= h >> 16`, `h *= 0x85ebca6b`, `h ^= h >> 13`, `h *= 0xc2b2ae35`, and `h ^= h >> 16`. Edge cases include empty input (return `seed ^ 0` then apply finalization—but note the standard also XORs length 0, which is fine) and input lengths not divisible by 4. Time complexity is O(n) where n is the number of bytes; space complexity is O(1) auxiliary.
#include <cstdint>
#include <vector>

// Compute MurmurHash3 (x86_32) of the given data using the provided seed.
std::uint32_t murmur3_32(std::uint32_t seed, const std::vector<unsigned char>& data) {
    std::uint32_t h1 = seed;
    const std::size_t len = data.size();
    const std::size_t nblocks = len / 4;

    const std::uint32_t c1 = 0xcc9e2d51;
    const std::uint32_t c2 = 0x1b873593;

    // Body: process each 4-byte block in little-endian order.
    for (std::size_t i = 0; i < nblocks; ++i) {
        std::uint32_t k1 = static_cast<std::uint32_t>(data[i * 4 + 0])
                         | (static_cast<std::uint32_t>(data[i * 4 + 1]) << 8)
                         | (static_cast<std::uint32_t>(data[i * 4 + 2]) << 16)
                         | (static_cast<std::uint32_t>(data[i * 4 + 3]) << 24);

        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17); // ROTL32(k1, 15)
        k1 *= c2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19); // ROTL32(h1, 13)
        h1 = h1 * 5 + 0xe6546b64;
    }

    // Tail: process remaining 0–3 bytes.
    const std::size_t tail_index = nblocks * 4;
    std::uint32_t k1 = 0;
    const std::size_t rem = len & 3;
    if (rem == 3) {
        k1 ^= static_cast<std::uint32_t>(data[tail_index + 2]) << 16;
    }
    if (rem >= 2) {
        k1 ^= static_cast<std::uint32_t>(data[tail_index + 1]) << 8;
    }
    if (rem >= 1) {
        k1 ^= static_cast<std::uint32_t>(data[tail_index]);
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17); // ROTL32(k1, 15)
        k1 *= c2;
        h1 ^= k1;
    }

    // Finalization.
    h1 ^= static_cast<std::uint32_t>(len);
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}
#include <cassert>
#include <vector>

int main() {
    // Test known MurmurHash3 x86_32 values from the canonical test suite.
    std::vector<unsigned char> empty;
    assert(murmur3_32(0, empty) == 0);

    std::vector<unsigned char> one_byte = {0x00};
    assert(murmur3_32(0, one_byte) == 0x514E28B7);

    std::vector<unsigned char> four_bytes = {0x00, 0x00, 0x00, 0x00};
    assert(murmur3_32(0, four_bytes) == 0x2362F9DE);

    std::vector<unsigned char> five_bytes = {0x00, 0x00, 0x00, 0x00, 0x00};
    assert(murmur3_32(0, five_bytes) == 0x85F0B427);

    // Test with non-zero content and seed.
    std::vector<unsigned char> data = {'a', 'b', 'c'};
    assert(murmur3_32(0, data) == 0xB62713F0);
    assert(murmur3_32(42, data) == 0x933E468D);

    // Test a longer vector.
    std::vector<unsigned char> long_data;
    for (int i = 0; i < 100; ++i) long_data.push_back(static_cast<unsigned char>(i));
    assert(murmur3_32(123, long_data) == 0x9B53BD57);

    // Test size not divisible by 4.
    std::vector<unsigned char> odd = {1, 2, 3, 4, 5, 6, 7};
    assert(murmur3_32(7, odd) == 0x2368588F);

    return 0;
}
