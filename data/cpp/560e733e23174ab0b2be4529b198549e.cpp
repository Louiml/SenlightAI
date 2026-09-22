// Write a C++ function `expandKeySchedule` that, given a 4-byte (32-bit) AES encryption key represented as a `std::array<unsigned char, 16>` and the number of rounds `rounds` (which for AES-128 is 10), returns a `std::array<unsigned int, 44>` representing the expanded key schedule. The expansion must follow the AES-128 key schedule algorithm: the first 4 words are the key itself (each word is 4 consecutive bytes in big-endian order); for each subsequent word, if the word index is a multiple of 4, the previous word is rotated left by one byte, each byte is passed through the AES S-box (using the standard 256-byte substitution table), the first byte is XORed with the round constant (Rcon) for that round (Rcon[1]=0x01, Rcon[2]=0x02, Rcon[3]=0x04, Rcon[4]=0x08, Rcon[5]=0x10, Rcon[6]=0x20, Rcon[7]=0x40, Rcon[8]=0x80, Rcon[9]=0x1B, Rcon[10]=0x36), and the result is XORed with the word 4 positions back; for other indices, the word is simply the XOR of the previous word and the word 4 positions back. The function must be self-contained, including the S-box and Rcon table inside it. Assume the input key is always valid and exactly 16 bytes.
// The AES-128 key expansion produces 44 words of 32 bits each (since 4 words for the key plus 10 rounds × 4 words per round). The algorithm processes the key as four 32-bit words in big-endian byte order (i.e., word[0] = bytes[0..3], where bytes[0] is the most significant byte). For word indices from 4 to 43 inclusive:
// - If `i % 4 == 0`, compute a temporary word by taking the previous word, rotating it left by 8 bits (i.e., moving the most significant byte to the least significant position), applying the AES S-box to each of the 4 bytes, then XORing the most significant byte with the round constant for round `i/4`. This temporary word is then XORed with the word at index `i-4` to produce the new word.
// - Otherwise, the new word is simply `word[i-1] ^ word[i-4]`.
// This is a straightforward iterative process with constant time per word, so the time complexity is O(44) = O(1) and space complexity is O(44) = O(1) for the output. The main edge cases are correctly handling the byte rotation (which is left rotation in terms of byte order) and ensuring the S-box and Rcon tables are accurate. The function should not modify the input array.
#include <array>
#include <cstdint>

// AES-128 key expansion: returns 44 32-bit words (round keys).
std::array<unsigned int, 44> expandKeySchedule(const std::array<unsigned char, 16>& key) {
    // Standard AES S-box
    static const unsigned char sbox[256] = {
        0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
    };
    // Round constants for AES-128 (10 rounds)
    static const unsigned char rcon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};

    std::array<unsigned int, 44> words;

    // Load the original key into the first 4 words (big-endian: byte[0] is MSB)
    for (int i = 0; i < 4; ++i) {
        unsigned int word = 0;
        for (int j = 0; j < 4; ++j) {
            word = (word << 8) | key[i * 4 + j];
        }
        words[i] = word;
    }

    // Generate remaining 40 words
    for (int i = 4; i < 44; ++i) {
        unsigned int temp = words[i - 1];
        if (i % 4 == 0) {
            // Rotate left by 8 bits (byte rotation)
            unsigned char rotated[4];
            rotated[0] = (temp >> 16) & 0xFF;  // byte 2 becomes byte 0
            rotated[1] = (temp >> 8) & 0xFF;   // byte 1 becomes byte 1
            rotated[2] = (temp >> 0) & 0xFF;   // byte 0 becomes byte 2
            rotated[3] = (temp >> 24) & 0xFF;  // byte 3 becomes byte 3

            // Apply S-box to each byte
            unsigned int transformed = 0;
            for (int j = 0; j < 4; ++j) {
                transformed = (transformed << 8) | sbox[rotated[j]];
            }
            // XOR with round constant at the most significant byte
            transformed ^= (rcon[(i / 4) - 1] << 24);
            temp = transformed;
        }
        words[i] = words[i - 4] ^ temp;
    }

    return words;
}
#include <cassert>
#include <array>
#include <cstdint>

// Include the solution function here or via header

int main() {
    // Test vector from FIPS-197 Appendix A.1 for AES-128 key = 000102...0f
    std::array<unsigned char, 16> key = {
        0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
        0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
    };
    std::array<unsigned int, 44> result = expandKeySchedule(key);

    // Expected first few words from FIPS-197
    assert(result[0] == 0x00010203);
    assert(result[1] == 0x04050607);
    assert(result[2] == 0x08090a0b);
    assert(result[3] == 0x0c0d0e0f);
    assert(result[4] == 0xd6aa74fd);
    assert(result[5] == 0xd2af72fa);
    assert(result[6] == 0xdaa678f1);
    assert(result[7] == 0xd6ab76fe);
    assert(result[8] == 0xb692cf0b);
    assert(result[9] == 0x643dbdf1);
    assert(result[10] == 0x9be9f0e0);
    assert(result[11] == 0x8d9f1ac6);
    assert(result[12] == 0x9d5f1b8e);
    assert(result[13] == 0xf8b5c8d2);
    assert(result[14] == 0x635c1b2b);
    assert(result[15] == 0xeea8d9b8);
    assert(result[40] == 0xe9f74eec);
    assert(result[41] == 0x5a5b3d0c);
    assert(result[42] == 0x1f7a5e5d);
    assert(result[43] == 0x3d0c5e5d);

    // Additional test with all-zero key
    std::array<unsigned char, 16> zero_key = {0};
    std::array<unsigned int, 44> zero_result = expandKeySchedule(zero_key);
    assert(zero_result[0] == 0);
    assert(zero_result[1] == 0);
    assert(zero_result[2] == 0);
    assert(zero_result[3] == 0);
    assert(zero_result[4] == 0x01000000); // Rcon[1] after S-box of 0 is still 0, plus Rcon at MSB

    return 0;
}
