Write a C++ function named `crc32Update` that takes a 32-bit unsigned integer `crc` representing the current CRC-32 state, a pointer to a byte buffer `data`, and a `size_t` length `size`, and returns the updated CRC-32 value as a `uint32_t`. The function must implement the standard CRC-32 algorithm using the reflected polynomial `0xEDB88320` with the standard initialization `0xFFFFFFFF` and final XOR `0xFFFFFFFF`. The function should process the buffer byte-by-byte, updating the CRC state according to the table-driven method: for each byte, compute `table[(crc & 0xFF) ^ byte] ^ (crc >> 8)`. The table must be precomputed once (e.g., using a static local lambda or a constant array) and reused across calls. The caller is responsible for applying the initial and final XORs, so the function itself must only update the state given an arbitrary starting `crc` value. The function must be `const`-correct with respect to the input buffer (i.e., `const uint8_t*`), and must handle `size == 0` gracefully by returning the input `crc` unchanged.

// The solution relies on the standard table-driven CRC-32 implementation. The main algorithm is: precompute a 256-entry lookup table where each entry corresponds to the CRC remainder for a single byte value using the polynomial `0xEDB88320`. This is done by iterating over each value from 0 to 255, and for each, performing 8 bit-wise shifts: if the least significant bit is 1, shift right and XOR with the polynomial; otherwise just shift right. After the table is ready, updating the CRC for each input byte `b` is a constant-time operation: `crc = table[(crc & 0xFF) ^ b] ^ (crc >> 8)`. Important edge cases include: (1) `size == 0` — the function must return the input `crc` untouched; (2) `data` being a null pointer when `size == 0` is allowed (no dereference), but if `size > 0` and `data` is null it is undefined behavior (the function does not need to validate); (3) the initial and final XORs are not handled inside the function — the caller must pass `0xFFFFFFFF` initially and XOR the result with `0xFFFFFFFF` at the end, which is a common pattern to allow incremental updates. Time complexity is O(n) for n bytes, where each byte’s update is O(1) due to the table lookup. Space complexity is O(1) for the table (fixed 256 entries) plus the input buffer, which is constant relative to the input size. The table can be initialized once via a static local lambda that returns a `std::array<uint32_t, 256>`, ensuring thread-safe initialization in C++11 and later.

#include <cstddef>
#include <cstdint>
#include <array>

// Update a CRC-32 value with one byte using the standard reflected polynomial.
static inline uint32_t crc32Byte(uint32_t crc, uint8_t byte) {
    static const std::array<uint32_t, 256> table = [] {
        std::array<uint32_t, 256> t{};
        const uint32_t polynomial = 0xEDB88320u;
        for (uint32_t i = 0; i < 256; ++i) {
            uint32_t r = i;
            for (int j = 0; j < 8; ++j) {
                if (r & 1u) {
                    r = (r >> 1) ^ polynomial;
                } else {
                    r >>= 1;
                }
            }
            t[i] = r;
        }
        return t;
    }();

    return table[(crc & 0xFFu) ^ byte] ^ (crc >> 8);
}

// Update a CRC-32 state over a byte buffer. The caller is responsible for
// initializing the crc to 0xFFFFFFFF and XORing the result with 0xFFFFFFFF.
uint32_t crc32Update(uint32_t crc, const uint8_t* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        crc = crc32Byte(crc, data[i]);
    }
    return crc;
}

#include <cassert>
#include <cstdint>
#include <cstdio>

// Forward declaration of the solution function (already defined above).
uint32_t crc32Update(uint32_t crc, const uint8_t* data, size_t size);

int main() {
    // Standard CRC-32 of the empty string is 0x00000000 when starting with
    // 0xFFFFFFFF and XORing the final result with 0xFFFFFFFF.
    uint32_t crc = crc32Update(0xFFFFFFFFu, nullptr, 0);
    crc ^= 0xFFFFFFFFu;
    assert(crc == 0x00000000u);

    // CRC-32 of "123456789" (the standard check string) is 0xCBF43926.
    const uint8_t data1[] = {'1','2','3','4','5','6','7','8','9'};
    crc = crc32Update(0xFFFFFFFFu, data1, sizeof(data1));
    crc ^= 0xFFFFFFFFu;
    assert(crc == 0xCBF43926u);

    // CRC-32 of "a" is 0xE8B7BE43.
    const uint8_t data2[] = {'a'};
    crc = crc32Update(0xFFFFFFFFu, data2, 1);
    crc ^= 0xFFFFFFFFu;
    assert(crc == 0xE8B7BE43u);

    // CRC-32 of "abc" is 0x352441C2.
    const uint8_t data3[] = {'a','b','c'};
    crc = crc32Update(0xFFFFFFFFu, data3, 3);
    crc ^= 0xFFFFFFFFu;
    assert(crc == 0x352441C2u);

    // Incremental update: same as processing whole buffer at once.
    const uint8_t data4[] = {0x00, 0xFF, 0x10, 0x20};
    uint32_t full = crc32Update(0xFFFFFFFFu, data4, 4);
    uint32_t inc = crc32Update(0xFFFFFFFFu, data4, 1);
    inc = crc32Update(inc, data4 + 1, 1);
    inc = crc32Update(inc, data4 + 2, 2);
    assert(full == inc);

    // A longer input: bytes from 0 to 255.
    uint8_t data5[256];
    for (int i = 0; i < 256; ++i) data5[i] = static_cast<uint8_t>(i);
    crc = crc32Update(0xFFFFFFFFu, data5, 256);
    crc ^= 0xFFFFFFFFu;
    // Known CRC-32 for this sequence (computed via standard tools).
    assert(crc == 0x29058B73u);

    printf("All CRC-32 tests passed.\n");
    return 0;
}
