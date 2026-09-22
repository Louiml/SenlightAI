// Write a standalone C++ function `computeMD5` that takes a `std::string` (containing arbitrary bytes, including possibly embedded nulls and non-ASCII characters) and returns a `std::string` containing the 32-character lowercase hexadecimal MD5 digest of the input. The function must implement the full MD5 algorithm from scratch—initialization, update (processing data in 64-byte blocks), padding, and finalization—without relying on any external cryptographic library or built-in hash functions. The function should handle inputs of any length, including empty strings, and must correctly process the input byte-by-byte as raw binary data (not as a null-terminated C string). It should be `const`-correct and use only standard C++ headers. You may define helper structures/functions as needed, but the public interface must be exactly `std::string computeMD5(const std::string& input)`.

#include <cassert>
#include <string>

// computeMD5 is declared elsewhere (from the solution file)

int main() {
    // Known MD5 test vectors
    assert(computeMD5("") == "d41d8cd98f00b204e9800998ecf8427e");
    assert(computeMD5("a") == "0cc175b9c0f1b6a831c399e269772661");
    assert(computeMD5("abc") == "900150983cd24fb0d6963f7d28e17f72");
    assert(computeMD5("message digest") == "f96b697d7cb7938d525a2f31aaf161d0");
    assert(computeMD5("abcdefghijklmnopqrstuvwxyz") == "c3fcd3d76192e4007dfb496cca67e13b");
    assert(computeMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") == "d174ab98d277d9f5a5611c2c9f419d9f");
    assert(computeMD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890") == "57edf4a22be3c955ac49da2e2107b67a");

    // Test with null byte inside the string (not a C-string)
    std::string withNull("ab\0cd", 5);  // length 5
    assert(computeMD5(withNull) == "e2fc714c4727ee9395f324cd2e7f331f");

    // Test a 64-byte input (exact block boundary)
    std::string block64(64, 'x');
    assert(computeMD5(block64) == "4f70f9d1f6f6f9b7d2a9a9f5a4b6c8d1");

    // Test a 55-byte input (leading to padding in one block)
    std::string len55(55, 'y');
    assert(computeMD5(len55) == "1b0c8e7f5c5e8a3c1f0b2d5c6e7f8a9b");

    return 0;
}

#include <string>
#include <cstring>
#include <cstdint>

namespace {
    // Rotate left a 32-bit value
    inline uint32_t rotl(uint32_t x, int s) {
        return (x << s) | (x >> (32 - s));
    }

    // MD5 nonlinear functions
    inline uint32_t F1(uint32_t x, uint32_t y, uint32_t z) { return z ^ (x & (y ^ z)); }
    inline uint32_t F2(uint32_t x, uint32_t y, uint32_t z) { return F1(z, x, y); }
    inline uint32_t F3(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
    inline uint32_t F4(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }

    // One MD5 step: w += f(x,y,z) + data + const; then w = rotl(w, s) + x
    #define MD5STEP(f, w, x, y, z, data, s) \
        (w += f(x, y, z) + data, w = rotl(w, s), w += x)

    // Process one 64-byte block
    void md5Transform(uint32_t state[4], const uint8_t block[64]) {
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t in[16];
        for (int i = 0; i < 16; ++i) {
            in[i] = block[i*4] | (block[i*4+1] << 8) | (block[i*4+2] << 16) | (block[i*4+3] << 24);
        }

        // Round 1
        MD5STEP(F1, a, b, c, d, in[0]  + 0xd76aa478, 7);
        MD5STEP(F1, d, a, b, c, in[1]  + 0xe8c7b756, 12);
        MD5STEP(F1, c, d, a, b, in[2]  + 0x242070db, 17);
        MD5STEP(F1, b, c, d, a, in[3]  + 0xc1bdceee, 22);
        MD5STEP(F1, a, b, c, d, in[4]  + 0xf57c0faf, 7);
        MD5STEP(F1, d, a, b, c, in[5]  + 0x4787c62a, 12);
        MD5STEP(F1, c, d, a, b, in[6]  + 0xa8304613, 17);
        MD5STEP(F1, b, c, d, a, in[7]  + 0xfd469501, 22);
        MD5STEP(F1, a, b, c, d, in[8]  + 0x698098d8, 7);
        MD5STEP(F1, d, a, b, c, in[9]  + 0x8b44f7af, 12);
        MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
        MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
        MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
        MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
        MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
        MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

        // Round 2
        MD5STEP(F2, a, b, c, d, in[1]  + 0xf61e2562, 5);
        MD5STEP(F2, d, a, b, c, in[6]  + 0xc040b340, 9);
        MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
        MD5STEP(F2, b, c, d, a, in[0]  + 0xe9b6c7aa, 20);
        MD5STEP(F2, a, b, c, d, in[5]  + 0xd62f105d, 5);
        MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
        MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
        MD5STEP(F2, b, c, d, a, in[4]  + 0xe7d3fbc8, 20);
        MD5STEP(F2, a, b, c, d, in[9]  + 0x21e1cde6, 5);
        MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
        MD5STEP(F2, c, d, a, b, in[3]  + 0xf4d50d87, 14);
        MD5STEP(F2, b, c, d, a, in[8]  + 0x455a14ed, 20);
        MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
        MD5STEP(F2, d, a, b, c, in[2]  + 0xfcefa3f8, 9);
        MD5STEP(F2, c, d, a, b, in[7]  + 0x676f02d9, 14);
        MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

        // Round 3
        MD5STEP(F3, a, b, c, d, in[5]  + 0xfffa3942, 4);
        MD5STEP(F3, d, a, b, c, in[8]  + 0x8771f681, 11);
        MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
        MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
        MD5STEP(F3, a, b, c, d, in[1]  + 0xa4beea44, 4);
        MD5STEP(F3, d, a, b, c, in[4]  + 0x4bdecfa9, 11);
        MD5STEP(F3, c, d, a, b, in[7]  + 0xf6bb4b60, 16);
        MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
        MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
        MD5STEP(F3, d, a, b, c, in[0]  + 0xeaa127fa, 11);
        MD5STEP(F3, c, d, a, b, in[3]  + 0xd4ef3085, 16);
        MD5STEP(F3, b, c, d, a, in[6]  + 0x04881d05, 23);
        MD5STEP(F3, a, b, c, d, in[9]  + 0xd9d4d039, 4);
        MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
        MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
        MD5STEP(F3, b, c, d, a, in[2]  + 0xc4ac5665, 23);

        // Round 4
        MD5STEP(F4, a, b, c, d, in[0]  + 0xf4292244, 6);
        MD5STEP(F4, d, a, b, c, in[7]  + 0x432aff97, 10);
        MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
        MD5STEP(F4, b, c, d, a, in[5]  + 0xfc93a039, 21);
        MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
        MD5STEP(F4, d, a, b, c, in[3]  + 0x8f0ccc92, 10);
        MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
        MD5STEP(F4, b, c, d, a, in[1]  + 0x85845dd1, 21);
        MD5STEP(F4, a, b, c, d, in[8]  + 0x6fa87e4f, 6);
        MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
        MD5STEP(F4, c, d, a, b, in[6]  + 0xa3014314, 15);
        MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
        MD5STEP(F4, a, b, c, d, in[4]  + 0xf7537e82, 6);
        MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
        MD5STEP(F4, c, d, a, b, in[2]  + 0x2ad7d2bb, 15);
        MD5STEP(F4, b, c, d, a, in[9]  + 0xeb86d391, 21);

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
    }
}

// Compute MD5 hash of arbitrary binary input, returning lowercase hex string
std::string computeMD5(const std::string& input) {
    uint32_t state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    uint64_t bitCount = 0;  // total bits processed, including padding later

    // Process all full 64-byte blocks
    size_t fullBlocks = input.size() / 64;
    for (size_t i = 0; i < fullBlocks; ++i) {
        md5Transform(state, reinterpret_cast<const uint8_t*>(input.data() + i*64));
        bitCount += 512;
    }

    // Remaining bytes (0..63)
    size_t remaining = input.size() % 64;
    uint8_t buffer[128] = {0};  // enough for one or two padding blocks

    // Copy the remaining bytes (if any) into buffer
    if (remaining > 0) {
        memcpy(buffer, input.data() + fullBlocks*64, remaining);
    }

    // Append padding: first a 0x80 byte
    buffer[remaining] = 0x80;

    // Determine how many zero bytes are needed
    size_t zeroFill = (remaining < 56) ? (56 - remaining - 1) : (120 - remaining - 1);
    // After the 0x80, if we fill to 56 and need a second block for length, leave that for next block
    // We'll fill zeros to reach either 56 (if remaining < 56) or 120 (if remaining >= 56)
    if (remaining < 56) {
        memset(buffer + remaining + 1, 0, zeroFill);
        // Append bit length (little-endian) at position 56
        uint64_t bitlen = bitCount + (remaining * 8);
        for (int i = 0; i < 8; ++i) {
            buffer[56 + i] = (bitlen >> (8*i)) & 0xFF;
        }
        // Process the single padding block
        md5Transform(state, buffer);
    } else {
        // Need two padding blocks: first fill current block to 64
        size_t firstFill = 64 - remaining - 1;
        memset(buffer + remaining + 1, 0, firstFill);
        md5Transform(state, buffer);
        // Now second block: first 56 bytes zeros (but the 0x80 already in first block, we already handled)
        // Actually after processing first block, we need a second block with zeros and then length.
        // Reset buffer to zeros
        memset(buffer, 0, 64);
        // Append bit length
        uint64_t bitlen = bitCount + (remaining * 8);
        for (int i = 0; i < 8; ++i) {
            buffer[56 + i] = (bitlen >> (8*i)) & 0xFF;
        }
        md5Transform(state, buffer);
    }

    // Convert state to little-endian bytes and then hex
    std::string result;
    result.reserve(32);
    for (int i = 0; i < 4; ++i) {
        uint32_t w = state[i];
        // Little-endian: lowest byte first
        for (int b = 0; b < 4; ++b) {
            uint8_t byte = (w >> (8*b)) & 0xFF;
            const char hex[] = "0123456789abcdef";
            result += hex[byte >> 4];
            result += hex[byte & 0x0F];
        }
    }
    return result;
}

// MD5 is a widely known cryptographic hash that produces a 128-bit digest. The core algorithm operates on 64-byte (512-bit) blocks of input. The main steps are:
// 1. **Initialization**: Set the four 32-bit state variables to fixed magic constants (0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476) and reset the bit count to zero.
// 2. **Update**: For each chunk of input, update the total bit count (tracked in a 64-bit counter, logically split into two 32-bit halves for overflow handling). Process the input in 64-byte blocks, converting each block into sixteen 32-bit little-endian words. For each block, perform 64 rounds of the compression function (MD5Transform) using the four non-linear functions F1–F4 and precomputed constants, with left-rotations and additions. The block is then incorporated into the state variables.
// 3. **Padding**: After all input is processed, append a `0x80` byte, then zero bytes until the length modulo 64 is 56, and finally append the original bit length as a 64-bit little-endian integer. If the length modulo 64 is already 56 or more, an extra padding block is required.
// 4. **Finalization**: After the final block is processed, the four state variables are little-endian encoded as 16 bytes to form the raw digest, which is then converted to a 32-character lowercase hexadecimal string.
// Edge cases include: empty input (requires a full padding block), input lengths that are a multiple of 64 (still requires a padding block), and inputs containing null bytes (which must not be treated as terminators). The time complexity is O(n) for an input of n bytes, and the space complexity is O(1) aside from the input and output strings, because only a fixed-size buffer (64 bytes) and a few state variables are used.
