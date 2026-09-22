Implement a C++ function that simulates a simplified encryption/decryption round-trip for a 64-bit block cipher, given a 64-bit plaintext value and a fixed 32-byte (256-bit) key represented as an array of `unsigned char`. The function must apply a basic substitution-permutation network (SPN) that uses the key to perform 8 rounds of encryption and then the inverse operations for decryption, returning the original 64-bit value after the round-trip. The encryption should XOR the current block with a round-derived subkey, apply an S-box substitution to each byte, and then permute the bits using a fixed permutation table. Decryption must reverse these steps exactly (inverse permutation, inverse S-box, XOR with the same subkey but in reverse round order). The function signature should be: `unsigned long long simpleBlockCipherRoundTrip(unsigned long long plaintext, const unsigned char key[32])`. The key derivation is simple: for round `r` (0-7), the subkey is the 8-byte segment of the key starting at byte offset `(r * 8) % 24` (so rounds reuse portions of the key, cycling). The S-box is a fixed 256-entry table (you may define a simple pseudo-random-looking table, e.g., the first 256 values of a linear congruential generator). The permutation is a fixed 64-bit mapping that reverses the bit order (bit `i` moves to position `63-i`). The function must return the plaintext after a full encrypt-then-decrypt cycle. Edge cases: the plaintext can be any 64-bit value including zero and all ones; the key is always exactly 32 bytes; the function must be deterministic and identical for repeated calls. Time complexity should be O(8 * 64) per encryption/decryption, and space complexity O(1) besides constant tables.

// The solution models a simple SPN cipher. For encryption, start with the 64-bit block equal to `plaintext`. For each round `r` from 0 to 7: (1) Extract an 8-byte subkey from `key` at start index `(r * 8) % 24`. Build a 64-bit subkey by reading 8 bytes little-endian (i.e., byte `i` is at position `8*i`). XOR the block with this subkey. (2) Apply byte-wise S-box: for each of the 8 bytes of the block (from least significant to most significant), replace the byte with the S-box value at that byte index. (3) Apply the bit permutation: for each bit position `i` from 0 to 63, set the output bit at position `63-i` to the input bit at position `i`. This is equivalent to bit-reversing the 64-bit integer. For decryption, reverse the process: for rounds `r` from 7 down to 0: (1) Apply inverse permutation (which is the same bit-reversal, since reversing twice returns original). (2) Apply inverse S-box (the S-box is not necessarily invertible? For simplicity, we must ensure the S-box is a permutation of 0-255. Use a simple LCG that generates each value exactly once, e.g., start with seed=1, `value = (value * 1103515245 + 12345) & 0xFF`, skip duplicates? That is complex. Instead, define S-box as a simple deterministic permutation: e.g., S[i] = (i * 31 + 17) % 256. This is invertible because 31 is coprime to 256? 31 and 256 are coprime? 31 is odd, yes, gcd 31 and 256=1, so multiplication by 31 mod 256 is invertible. The inverse S-box: for each j from 0 to 255, find i such that (i*31+17)%256 == j. Precompute inverse table. (3) XOR with the same subkey. After all rounds, the block should equal the original plaintext. Complexity: Each round does 8 XORs, 8 S-box lookups, and 64 bit operations, so total O(512) per encrypt/decrypt, constant time. Memory: constant tables of 256 entries each.

#include <cstdint>
#include <cstring>

// S-box: linear permutation modulo 256 (invertible because 31 is odd)
const unsigned char SBOX[256] = [] {
    unsigned char s[256];
    for (int i = 0; i < 256; ++i) s[i] = (i * 31 + 17) & 0xFF;
    return s;
}();

const unsigned char INV_SBOX[256] = [] {
    unsigned char inv[256];
    for (int i = 0; i < 256; ++i) inv[SBOX[i]] = i;
    return inv;
}();

// Reverse the order of bits in a 64-bit value
static inline uint64_t bitReverse(uint64_t x) {
    x = ((x & 0xAAAAAAAAAAAAAAAAULL) >> 1) | ((x & 0x5555555555555555ULL) << 1);
    x = ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2) | ((x & 0x3333333333333333ULL) << 2);
    x = ((x & 0xF0F0F0F0F0F0F0F0ULL) >> 4) | ((x & 0x0F0F0F0F0F0F0F0FULL) << 4);
    x = ((x & 0xFF00FF00FF00FF00ULL) >> 8) | ((x & 0x00FF00FF00FF00FFULL) << 8);
    x = ((x & 0xFFFF0000FFFF0000ULL) >> 16) | ((x & 0x0000FFFF0000FFFFULL) << 16);
    x = (x >> 32) | (x << 32);
    return x;
}

// Get 64-bit subkey from key starting at byte offset (little-endian)
static inline uint64_t getSubkey(const unsigned char key[32], unsigned int round) {
    unsigned int start = (round * 8) % 24;
    uint64_t subkey = 0;
    for (unsigned int i = 0; i < 8; ++i) {
        subkey |= (static_cast<uint64_t>(key[start + i]) << (8 * i));
    }
    return subkey;
}

// Apply S-box to each byte of the 64-bit block
static inline uint64_t applySBox(uint64_t block, const unsigned char table[256]) {
    uint64_t result = 0;
    for (unsigned int i = 0; i < 8; ++i) {
        unsigned char byte = (block >> (8 * i)) & 0xFF;
        result |= (static_cast<uint64_t>(table[byte]) << (8 * i));
    }
    return result;
}

// Full round trip: encrypt then decrypt, returning original plaintext
uint64_t simpleBlockCipherRoundTrip(uint64_t plaintext, const unsigned char key[32]) {
    // Encryption
    uint64_t block = plaintext;
    for (unsigned int r = 0; r < 8; ++r) {
        block ^= getSubkey(key, r);
        block = applySBox(block, SBOX);
        block = bitReverse(block);
    }

    // Decryption (reverse rounds)
    for (unsigned int r = 8; r-- > 0; ) {
        block = bitReverse(block); // inverse of bitReverse
        block = applySBox(block, INV_SBOX);
        block ^= getSubkey(key, r);
    }

    return block;
}

#include <cassert>

int main() {
    unsigned char key[32] = {
        0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
        0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
        0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
        0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88
    };

    assert(simpleBlockCipherRoundTrip(0ULL, key) == 0ULL);
    assert(simpleBlockCipherRoundTrip(0xFFFFFFFFFFFFFFFFULL, key) == 0xFFFFFFFFFFFFFFFFULL);
    assert(simpleBlockCipherRoundTrip(0x0123456789ABCDEFULL, key) == 0x0123456789ABCDEFULL);
    assert(simpleBlockCipherRoundTrip(0xFEDCBA9876543210ULL, key) == 0xFEDCBA9876543210ULL);
    assert(simpleBlockCipherRoundTrip(0x0000000000000001ULL, key) == 0x0000000000000001ULL);
    assert(simpleBlockCipherRoundTrip(0x8000000000000000ULL, key) == 0x8000000000000000ULL);
    assert(simpleBlockCipherRoundTrip(0xDEADBEEFCAFEBABEULL, key) == 0xDEADBEEFCAFEBABEULL);
    assert(simpleBlockCipherRoundTrip(0x1111111111111111ULL, key) == 0x1111111111111111ULL);
    assert(simpleBlockCipherRoundTrip(0xAAAAAAAAAAAAAAAAULL, key) == 0xAAAAAAAAAAAAAAAAULL);
    assert(simpleBlockCipherRoundTrip(0x1234567890ABCDEFULL, key) == 0x1234567890ABCDEFULL);
    return 0;
}
