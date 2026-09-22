/*
Implement a C++ function that computes the CRC-16 checksum of a byte buffer using the CCITT polynomial \(0x1021\) (i.e., \(x^{16} + x^{12} + x^5 + x^0\)), with an initial CRC value of `0xffff` and a final XOR of `0x0000`. The function must take a pointer to the data and a length in bytes, and return the 16-bit checksum as an `unsigned short`. The implementation should not rely on a precomputed static table (i.e., it must compute the CRC bit-by-bit), handle empty input gracefully (returning the initial value after final XOR), and treat the input as an array of unsigned bytes. Use `const` appropriately for the input pointer and avoid any global mutation.
*/
#include <cstddef>
#include <cstdint>

// Compute CRC-16 (CCITT) of a byte buffer using polynomial 0x1021,
// initial value 0xffff, and final XOR 0x0000.
unsigned short crc16_ccitt(const void* data, std::size_t length) {
    const std::uint8_t* bytes = static_cast<const std::uint8_t*>(data);
    std::uint16_t crc = 0xffff;

    for (std::size_t i = 0; i < length; ++i) {
        crc ^= static_cast<std::uint16_t>(bytes[i]) << 8;
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x8000) {
                crc = static_cast<std::uint16_t>((crc << 1) ^ 0x1021);
            } else {
                crc = static_cast<std::uint16_t>(crc << 1);
            }
        }
    }

    return crc; // final XOR with 0x0000 is a no-op
}
#include <cassert>
#include <cstdint>
#include <cstddef>

// (Solution function declaration as above)
unsigned short crc16_ccitt(const void* data, std::size_t length);

int main() {
    // Test 1: Empty input
    assert(crc16_ccitt(nullptr, 0) == 0xffff);

    // Test 2: Single byte 0x00
    uint8_t zero = 0x00;
    assert(crc16_ccitt(&zero, 1) == 0x1021);

    // Test 3: Single byte 0x01
    uint8_t one = 0x01;
    assert(crc16_ccitt(&one, 1) == 0x1020);

    // Test 4: Known message "123456789" (standard CRC-16/CCITT test)
    const char* msg = "123456789";
    assert(crc16_ccitt(msg, 9) == 0x29b1);

    // Test 5: Buffer with two bytes 0xff 0xff
    uint8_t ff[2] = {0xff, 0xff};
    assert(crc16_ccitt(ff, 2) == 0xffff);

    // Test 6: Buffer with 0x01 0x02 0x03 0x04
    uint8_t seq[4] = {0x01, 0x02, 0x03, 0x04};
    assert(crc16_ccitt(seq, 4) == 0x800d);

    // Test 7: Long buffer of zeros (length 256)
    uint8_t zeros[256] = {0};
    assert(crc16_ccitt(zeros, 256) == 0xe2f8);

    return 0;
}
// The standard CRC-16 calculation processes each byte from most significant bit to least, updating a 16-bit register (`crc`) according to the polynomial. For each bit, the register is shifted left by one. If the bit shifted out (the old MSB) is `1`, the register is XORed with the polynomial `0x1021`. To simplify the implementation, we can process each byte by XORing it with the high byte of the current CRC, then iterating over 8 bits. For each bit: if the MSB of the register is `1`, shift left and XOR with `0x1021`; otherwise, shift left. The algorithm directly follows the polynomial definition: `0x1021` corresponds to the binary pattern with bits set at positions 12, 5, and 0 (excluding the implicit x^16 term). Edge cases include empty input: with no bytes, the initial value `0xffff` is returned after XOR with `0x0000` (which is a no-op). The algorithm runs in \(O(8 \cdot n) = O(n)\) time per byte, using \(O(1)\) auxiliary space. No static table is needed, making the function fully self-contained and suitable for independent use.
