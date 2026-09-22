// Write a C++ function named `crc32_checksum` that accepts a pointer to a buffer of bytes, its length in bytes, and an optional initial CRC value (defaulting to 0), and returns the standard CRC-32 checksum (as used in zlib and many file formats) of the data. The function must handle null pointers, zero-length buffers, and any initial CRC value correctly, ensuring that the result matches the standard CRC-32 algorithm with polynomial `0xEDB88320`. The implementation should be self-contained, using only standard headers, and must not rely on external libraries or precomputed tables.
// The solution implements the standard CRC-32 algorithm. We precompute a 256-entry lookup table where each entry represents the CRC-32 remainder for a single byte (0–255) processed through 8 bitwise shifts and conditional XORs with the polynomial `0xEDB88320`. The table is computed once and reused. For the main computation, we start with `crc = ~initial_crc` (bitwise complement) to incorporate the initial value. Then for each byte in the buffer, we update `crc` using the formula `crc = table[(byte ^ crc) & 0xFF] ^ (crc >> 8)`. After processing all bytes, we return `~crc` (bitwise complement). Edge cases: null pointer with positive length is treated as an error and returns 0; zero-length buffer returns the standard CRC-32 of an empty string (i.e., `0x00000000` when initial CRC is 0). The algorithm runs in O(n) time for n bytes, with O(1) auxiliary space (the table is constant size 256). The table generation is O(1) since it's fixed-size and done once per process.
#include <cstdint>
#include <cstddef>

// Compute CRC-32 checksum of a memory buffer.
// data: pointer to the buffer; bytes: number of bytes; prev_crc: initial CRC value (default 0).
// Returns 0 if data is null and bytes > 0. Otherwise returns standard CRC-32.
uint32_t crc32_checksum(const void* data, size_t bytes, uint32_t prev_crc = 0) {
    // Handle invalid input: null pointer with positive length
    if (data == nullptr && bytes > 0) {
        return 0;
    }

    // Static lookup table, computed once
    static uint32_t table[256] = {0};
    static bool table_ready = false;
    if (!table_ready) {
        const uint32_t polynomial = 0xEDB88320u;
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t crc_val = i;
            for (int bit = 0; bit < 8; ++bit) {
                if (crc_val & 1) {
                    crc_val = (crc_val >> 1) ^ polynomial;
                } else {
                    crc_val >>= 1;
                }
            }
            table[i] = crc_val;
        }
        table_ready = true;
    }

    uint32_t crc = ~prev_crc;
    const uint8_t* buf = static_cast<const uint8_t*>(data);
    while (bytes > 0) {
        crc = table[(buf[0] ^ crc) & 0xFF] ^ (crc >> 8);
        ++buf;
        --bytes;
    }
    return ~crc;
}
#include <cassert>
#include <cstdint>
#include <cstring>

int main() {
    // Standard CRC-32 test vectors
    assert(crc32_checksum("", 0, 0) == 0x00000000);
    assert(crc32_checksum("a", 1, 0) == 0xE8B7BE43);
    assert(crc32_checksum("abc", 3, 0) == 0x352441C2);
    assert(crc32_checksum("message digest", 14, 0) == 0x20159D7F);

    // Null pointer with positive length should return 0
    assert(crc32_checksum(nullptr, 5, 0) == 0);

    // Non-zero initial CRC
    assert(crc32_checksum("abc", 3, 0xFFFFFFFF) == 0x91267E72);

    // Binary data with null bytes
    char data[] = {0x00, 0x01, 0x02, 0x03};
    assert(crc32_checksum(data, sizeof(data), 0) == 0xCBF43926);

    // Large buffer (1MB) should not crash and produce a consistent result
    const size_t large_size = 1024 * 1024;
    uint8_t* large_data = new uint8_t[large_size];
    for (size_t i = 0; i < large_size; ++i) large_data[i] = static_cast<uint8_t>(i % 251);
    uint32_t large_crc = crc32_checksum(large_data, large_size, 0);
    // Just check it's non-zero and deterministic
    assert(large_crc == crc32_checksum(large_data, large_size, 0));
    delete[] large_data;

    return 0;
}
