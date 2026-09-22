// Write a C++ function that computes the SHA-1 hash of a given byte vector and returns the 20-byte digest as a `std::vector<uint8_t>`. The input is a sequence of bytes (octets). The function must correctly handle inputs of any length, including empty input, and must return the standard SHA-1 message digest in big-endian byte order (i.e., the digest is 20 bytes, where the first byte is the most significant byte of the first 32-bit state word). Do not use any external cryptographic libraries; implement the compression logic from scratch based on the SHA-1 specification, paying careful attention to message padding (append 0x80, then zeros, then the 64-bit big-endian bit length of the original message) and the processing of 512-bit blocks.

#include <cassert>
#include <cstdint>
#include <vector>

// The sha1 function declaration (as above) is assumed to be available.

static bool equals(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    return a == b;
}

int main() {
    // Test case 1: Empty input (well-known SHA-1).
    std::vector<uint8_t> empty;
    std::vector<uint8_t> hash1 = sha1(empty);
    std::vector<uint8_t> expected1 = {0xda, 0x39, 0xa3, 0xee, 0x5e, 0x6b, 0x4b, 0x0d,
                                      0x32, 0x55, 0xbf, 0xef, 0x95, 0x60, 0x18, 0x90,
                                      0xaf, 0xd8, 0x07, 0x09};
    assert(equals(hash1, expected1));

    // Test case 2: "abc" (well-known SHA-1).
    std::vector<uint8_t> abc = {'a', 'b', 'c'};
    std::vector<uint8_t> hash2 = sha1(abc);
    std::vector<uint8_t> expected2 = {0xa9, 0x99, 0x3e, 0x36, 0x47, 0x06, 0x81, 0x6a,
                                      0xba, 0x3e, 0x25, 0x71, 0x78, 0x50, 0xc2, 0x6c,
                                      0x9c, 0xd0, 0xd8, 0x9d};
    assert(equals(hash2, expected2));

    // Test case 3: Single byte message "a".
    std::vector<uint8_t> single = {'a'};
    std::vector<uint8_t> hash3 = sha1(single);
    // SHA-1("a") = 86f7e437faa5a7fce15d1ddcb9eaeaea377667b8
    std::vector<uint8_t> expected3 = {0x86, 0xf7, 0xe4, 0x37, 0xfa, 0xa5, 0xa7, 0xfc,
                                      0xe1, 0x5d, 0x1d, 0xdc, 0xb9, 0xea, 0xea, 0xea,
                                      0x37, 0x76, 0x67, 0xb8};
    assert(equals(hash3, expected3));

    // Test case 4: 55-byte message (exactly 55 bytes, which is just below the padding threshold of 56 bytes).
    std::vector<uint8_t> msg55;
    for (int i = 0; i < 55; ++i) msg55.push_back(static_cast<uint8_t>(i));
    std::vector<uint8_t> hash4 = sha1(msg55);
    // Since known correct values are hard to recall, just verify length and that it doesn't crash.
    assert(hash4.size() == 20);

    // Test case 5: 56-byte message (requires one full padding block after the 0x80 and length).
    std::vector<uint8_t> msg56;
    for (int i = 0; i < 56; ++i) msg56.push_back(static_cast<uint8_t>(0xFF - i));
    std::vector<uint8_t> hash5 = sha1(msg56);
    assert(hash5.size() == 20);

    // Test case 6: 64-byte message (exactly one block before padding, so two blocks total).
    std::vector<uint8_t> msg64(64, 0x42);
    std::vector<uint8_t> hash6 = sha1(msg64);
    assert(hash6.size() == 20);

    // Test case 7: Large message (e.g., 1000 bytes) to test block iteration.
    std::vector<uint8_t> big(1000);
    for (size_t i = 0; i < big.size(); ++i) big[i] = static_cast<uint8_t>(i * 7 + 3);
    std::vector<uint8_t> hash7 = sha1(big);
    assert(hash7.size() == 20);

    // Test case 8: All zero bytes of length 1, 2, and 3.
    std::vector<uint8_t> zeros1(1, 0);
    std::vector<uint8_t> zeros2(2, 0);
    std::vector<uint8_t> zeros3(3, 0);
    assert(sha1(zeros1).size() == 20);
    assert(sha1(zeros2).size() == 20);
    assert(sha1(zeros3).size() == 20);

    return 0;
}

#include <cstdint>
#include <vector>

// Rotate a 32-bit value left by n bits (n in [0,31]).
inline uint32_t leftRotate(uint32_t value, unsigned n) {
    return (value << n) | (value >> (32 - n));
}

// Compute the SHA-1 hash of the input byte vector.
// Returns a 20-byte digest in big-endian order.
std::vector<uint8_t> sha1(const std::vector<uint8_t>& input) {
    // Initial state constants (big-endian representations of the fractional parts of sqrt(2), sqrt(3), sqrt(5), sqrt(7), sqrt(11)).
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // Copy input and append padding.
    std::vector<uint8_t> message(input);
    uint64_t bitLength = static_cast<uint64_t>(message.size()) * 8ULL;

    // Step 1: append 0x80.
    message.push_back(0x80);

    // Step 2: append zeros until message length % 64 == 56.
    while (message.size() % 64 != 56) {
        message.push_back(0x00);
    }

    // Step 3: append the 64-bit big-endian bit length.
    for (int i = 7; i >= 0; --i) {
        message.push_back(static_cast<uint8_t>((bitLength >> (i * 8)) & 0xFF));
    }

    // Process each 512-bit block.
    for (size_t offset = 0; offset < message.size(); offset += 64) {
        uint32_t w[80];

        // Break block into sixteen 32-bit big-endian words.
        for (unsigned i = 0; i < 16; ++i) {
            w[i] = (static_cast<uint32_t>(message[offset + i * 4]) << 24)
                 | (static_cast<uint32_t>(message[offset + i * 4 + 1]) << 16)
                 | (static_cast<uint32_t>(message[offset + i * 4 + 2]) << 8)
                 | (static_cast<uint32_t>(message[offset + i * 4 + 3]));
        }

        // Extend to 80 words.
        for (unsigned i = 16; i < 80; ++i) {
            w[i] = leftRotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
        }

        // Initialize working variables.
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        // Main loop.
        for (unsigned i = 0; i < 80; ++i) {
            uint32_t f;
            uint32_t k;

            if (i < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (i < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = leftRotate(a, 5) + f + e + k + w[i];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        // Update state.
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Produce the 20-byte big-endian digest.
    std::vector<uint8_t> digest(20);
    auto store32 = [&](size_t index, uint32_t value) {
        digest[index] = static_cast<uint8_t>((value >> 24) & 0xFF);
        digest[index + 1] = static_cast<uint8_t>((value >> 16) & 0xFF);
        digest[index + 2] = static_cast<uint8_t>((value >> 8) & 0xFF);
        digest[index + 3] = static_cast<uint8_t>(value & 0xFF);
    };
    store32(0, h0);
    store32(4, h1);
    store32(8, h2);
    store32(12, h3);
    store32(16, h4);

    return digest;
}

// The solution must follow the SHA-1 algorithm:
// 1. Initialize the five 32-bit state variables to the standard constants: `0x67452301`, `0xEFCDAB89`, `0x98BADCFE`, `0x10325476`, `0xC3D2E1F0`.
// 2. Preprocess the message:
//    - Append a single `0x80` byte.
//    - Append `0x00` bytes until the message length modulo 64 is 56 bytes (i.e., 448 bits).
//    - Append the original message length in bits as a 64-bit big-endian integer.
// 3. Process each 512-bit block:
//    - Break the block into sixteen 32-bit big-endian words (`w[0..15]`).
//    - Extend to 80 words using `w[i] = leftrotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1)` for `i` from 16 to 79.
//    - Initialize `a,b,c,d,e` from the current state.
//    - For each round `i` from 0 to 79, use the appropriate function and constant:
//      - Rounds 0–19: `f = (b & c) | (~b & d)`, constant `0x5A827999`.
//      - Rounds 20–39: `f = b ^ c ^ d`, constant `0x6ED9EBA1`.
//      - Rounds 40–59: `f = (b & c) | (b & d) | (c & d)`, constant `0x8F1BBCDC`.
//      - Rounds 60–79: `f = b ^ c ^ d`, constant `0xCA62C1D6`.
//      - Update: `temp = leftrotate(a,5) + f + e + constant + w[i]`, then shift `e=d, d=c, c=leftrotate(b,30), b=a, a=temp`.
//    - Add the resulting `a,b,c,d,e` to the current state.
// 4. After all blocks, output the state as 20 big-endian bytes.
//
// Edge cases: empty input (must produce the well-known SHA-1 of empty string: `da39a3ee5e6b4b0d3255bfef95601890afd80709`), input lengths that are already a multiple of 64 (still require an extra padding block), large inputs (must not overflow), and arbitrary byte values including zero bytes. Time complexity is O(n) for n input bytes, with a constant factor for each 64-byte block; space complexity is O(1) for the state and block buffer, plus O(n) for the input copy if the vector is passed by value (but the function can take a const reference to avoid copying).
