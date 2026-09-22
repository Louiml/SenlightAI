Write a standalone C++ function named `computeHmacSha256` that implements HMAC-SHA256 as defined by RFC 2104. The function must accept a message (as `const unsigned char*` plus a length) and a key (also as `const unsigned char*` plus a length), and produce a 32-byte authentication tag. Use only the SHA-256 compression algorithm internally — you may implement SHA-256 from scratch or use a minimal internal helper class, but you must not call any external cryptographic library. The function must handle keys of arbitrary length: keys longer than 64 bytes must be hashed first, keys shorter or equal to 64 bytes are padded with zeros to 64 bytes. Then compute the HMAC as: `SHA256( (key XOR opad) || SHA256( (key XOR ipad) || message ) )`, where `opad = 0x5c` repeated 64 times and `ipad = 0x36` repeated 64 times. The output must be written into a caller-provided 32-byte buffer. Your solution should be self-contained and include all necessary SHA-256 logic (padding, message schedule, compression, and finalization) in the same code.

#include <cassert>
#include <cstring>

int main() {
    // Test vector 1: RFC 4231 test case 1 (key = 0x0b repeated 20, message = "Hi There")
    unsigned char key1[20];
    memset(key1, 0x0b, 20);
    const char* msg1 = "Hi There";
    unsigned char hmac1[32];
    computeHmacSha256(key1, 20, reinterpret_cast<const unsigned char*>(msg1), strlen(msg1), hmac1);
    unsigned char expected1[32] = {
        0xb0,0x34,0x4c,0x61,0xd8,0xdb,0x38,0x53,
        0x5c,0xa8,0xaf,0xce,0xaf,0x0b,0xf1,0x2b,
        0x88,0x1d,0xc2,0x00,0xc9,0x83,0x3d,0xa7,
        0x26,0xe9,0x37,0x6c,0x2e,0x32,0xcf,0xf7
    };
    assert(memcmp(hmac1, expected1, 32) == 0);

    // Test vector 2: RFC 4231 test case 2 (key = "Jefe", message = "what do ya want for nothing?")
    const char* key2 = "Jefe";
    const char* msg2 = "what do ya want for nothing?";
    unsigned char hmac2[32];
    computeHmacSha256(reinterpret_cast<const unsigned char*>(key2), strlen(key2),
                      reinterpret_cast<const unsigned char*>(msg2), strlen(msg2), hmac2);
    unsigned char expected2[32] = {
        0x5b,0xdc,0xc1,0x46,0xbf,0x60,0x75,0x4e,
        0x6a,0x04,0x24,0x26,0x08,0x95,0x75,0xc7,
        0x5a,0x00,0x3f,0x08,0x9d,0x27,0x39,0x83,
        0x9d,0xec,0x58,0xb9,0x64,0xec,0x38,0x43
    };
    assert(memcmp(hmac2, expected2, 32) == 0);

    // Test vector 3: Empty message with zero-length key
    unsigned char hmac3[32];
    computeHmacSha256(nullptr, 0, nullptr, 0, hmac3);
    unsigned char expected3[32] = {
        0xb6,0x13,0x67,0x9a,0x08,0x14,0x4b,0xa0,
        0x91,0x9c,0x25,0x5c,0x75,0xb2,0x3c,0x9e,
        0x70,0x0e,0x8c,0x7c,0x4d,0x82,0x0d,0x8f,
        0x7c,0x6c,0x0e,0x74,0xb9,0xe4,0x0e,0x3a
    };
    assert(memcmp(hmac3, expected3, 32) == 0);

    // Test vector 4: Key longer than 64 bytes (using 80 bytes of 0xaa)
    unsigned char key4[80];
    memset(key4, 0xaa, 80);
    const char* msg4 = "Test Using Larger Than Block-Size Key - Hash Key First";
    unsigned char hmac4[32];
    computeHmacSha256(key4, 80, reinterpret_cast<const unsigned char*>(msg4), strlen(msg4), hmac4);
    unsigned char expected4[32] = {
        0x60,0xe4,0x31,0x59,0x1e,0xe0,0xb6,0x7f,
        0x0d,0x8a,0x26,0xaa,0xcb,0xf5,0xb7,0x7f,
        0x8e,0x0b,0xc6,0x21,0x37,0x28,0xc5,0x14,
        0x05,0x46,0x8c,0x0a,0x30,0xbd,0x8f,0x1c
    };
    assert(memcmp(hmac4, expected4, 32) == 0);

    // Test vector 5: Key exactly 64 bytes (all 0x0b)
    unsigned char key5[64];
    memset(key5, 0x0b, 64);
    const char* msg5 = "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data";
    unsigned char hmac5[32];
    computeHmacSha256(key5, 64, reinterpret_cast<const unsigned char*>(msg5), strlen(msg5), hmac5);
    unsigned char expected5[32] = {
        0x68,0x19,0xd9,0x15,0xc7,0x62,0x8d,0x1d,
        0x1e,0x6e,0x8b,0x4e,0x8f,0x8f,0x6b,0x2a,
        0x1d,0x6c,0x3d,0x8a,0x4e,0x6c,0x9c,0x2a,
        0x9c,0x4e,0x0d,0x6b,0x1c,0x9a,0x2d,0x1e
    };
    // Note: This expected value is not from RFC but computed for demonstration; adjust if necessary.
    // For a correct standalone test, you may replace with a known RFC value.
    // Here we just check that the function runs without error and returns 32 bytes.
    // For strict verification, use a known test vector.
    // Uncomment the line below if you have a correct expected value:
    // assert(memcmp(hmac5, expected5, 32) == 0);
    // The above expected5 is placeholder and may be incorrect. For a reliable test, use RFC 4231 case 7.
    // To keep the test runnable, we only assert size and non-null:
    assert(hmac5[0] != 0 || hmac5[31] != 0); // just a dummy check to silence unused warning

    // Additional simple consistency check: HMAC of empty message with a known key produces a 32-byte output.
    unsigned char hmac6[32];
    unsigned char key6[] = {0x01, 0x02, 0x03};
    computeHmacSha256(key6, 3, nullptr, 0, hmac6);
    // No assertion on value, but ensure we have 32 bytes (we can check first byte is not all zeros? not reliable)
    // Just verify that all bytes are present (already copied). Use a dummy check.
    assert(sizeof(hmac6) == 32);

    return 0;
}

#include <cstdint>
#include <cstring>
#include <vector>

// Minimal SHA-256 implementation
class Sha256 {
public:
    Sha256() {
        reset();
    }

    void reset() {
        h[0] = 0x6a09e667;
        h[1] = 0xbb67ae85;
        h[2] = 0x3c6ef372;
        h[3] = 0xa54ff53a;
        h[4] = 0x510e527f;
        h[5] = 0x9b05688c;
        h[6] = 0x1f83d9ab;
        h[7] = 0x5be0cd19;
        bitlen = 0;
        buffer_len = 0;
    }

    void update(const unsigned char* data, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            buffer[buffer_len++] = data[i];
            if (buffer_len == 64) {
                processBlock(buffer);
                buffer_len = 0;
            }
        }
        bitlen += len * 8;
    }

    void finalize(unsigned char hash[32]) {
        // Append 0x80
        buffer[buffer_len++] = 0x80;

        // Pad with zeros until 56 mod 64
        if (buffer_len > 56) {
            while (buffer_len < 64) buffer[buffer_len++] = 0;
            processBlock(buffer);
            buffer_len = 0;
        }
        while (buffer_len < 56) buffer[buffer_len++] = 0;

        // Append bit length as 64-bit big-endian
        uint64_t bits = bitlen;
        for (int i = 7; i >= 0; --i) {
            buffer[buffer_len++] = static_cast<unsigned char>((bits >> (i * 8)) & 0xff);
        }
        // The above writes exactly 8 bytes, now buffer_len = 56 + 8 = 64
        processBlock(buffer);

        // Output
        for (int i = 0; i < 8; ++i) {
            hash[i*4]   = static_cast<unsigned char>((h[i] >> 24) & 0xff);
            hash[i*4+1] = static_cast<unsigned char>((h[i] >> 16) & 0xff);
            hash[i*4+2] = static_cast<unsigned char>((h[i] >> 8) & 0xff);
            hash[i*4+3] = static_cast<unsigned char>(h[i] & 0xff);
        }
    }

private:
    uint32_t h[8];
    unsigned char buffer[64];
    size_t buffer_len;
    uint64_t bitlen;

    static const uint32_t K[64];

    static uint32_t rotr(uint32_t x, int n) {
        return (x >> n) | (x << (32 - n));
    }

    void processBlock(const unsigned char block[64]) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (static_cast<uint32_t>(block[i*4]) << 24) |
                   (static_cast<uint32_t>(block[i*4+1]) << 16) |
                   (static_cast<uint32_t>(block[i*4+2]) << 8) |
                   (static_cast<uint32_t>(block[i*4+3]));
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr(w[i-15], 7) ^ rotr(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32_t s1 = rotr(w[i-2], 17) ^ rotr(w[i-2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = hh + S1 + ch + K[i] + w[i];
            uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            hh = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }
};

const uint32_t Sha256::K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Compute HMAC-SHA256 and write 32-byte digest to output.
void computeHmacSha256(const unsigned char* key, size_t key_len,
                       const unsigned char* message, size_t message_len,
                       unsigned char output[32]) {
    // Step 1: Derive 64-byte key block
    unsigned char key_block[64] = {0};
    if (key_len <= 64) {
        memcpy(key_block, key, key_len);
    } else {
        Sha256 hasher;
        hasher.update(key, key_len);
        hasher.finalize(key_block);
    }

    // Step 2: Create inner and outer key blocks
    unsigned char inner_key[64], outer_key[64];
    for (int i = 0; i < 64; ++i) {
        inner_key[i] = key_block[i] ^ 0x36;
        outer_key[i] = key_block[i] ^ 0x5c;
    }

    // Step 3: Inner hash = SHA256(inner_key || message)
    Sha256 inner_hasher;
    inner_hasher.update(inner_key, 64);
    inner_hasher.update(message, message_len);
    unsigned char inner_hash[32];
    inner_hasher.finalize(inner_hash);

    // Step 4: Outer hash = SHA256(outer_key || inner_hash)
    Sha256 outer_hasher;
    outer_hasher.update(outer_key, 64);
    outer_hasher.update(inner_hash, 32);
    outer_hasher.finalize(output);
}

// The core challenge is implementing SHA-256 correctly, then layering HMAC on top. For SHA-256, the algorithm processes the message in 64-byte blocks. First, append a single `0x80` bit, then pad with zeros until the length modulo 64 is 56, and finally append the original message length as a 64-bit big-endian integer. Each block is expanded into a 64-entry message schedule (32-bit words). Then 64 rounds of compression are applied using the standard SHA-256 constants (K) and initial hash values (H0–H7). After all blocks, the hash is the concatenation of the eight 32-bit registers in big-endian form.
//
// For HMAC, we first derive a 64-byte key block: if the key length ≤ 64, copy it and zero-pad; otherwise hash the key and zero-pad the 32-byte digest to 64 bytes. Then create two 64-byte blocks: `inner_key = key_block XOR 0x36` and `outer_key = key_block XOR 0x5c`. The inner hash is `SHA256( inner_key || message )`. The final HMAC is `SHA256( outer_key || inner_hash )`. This can be done by feeding the inner hash (32 bytes) directly after the outer key into a second SHA-256 instance.
//
// Edge cases: empty message (length 0) works naturally; key exactly 64 bytes (no padding); key longer than 64 bytes (hash first); message length that requires multiple blocks (including padding block boundaries) must be handled correctly. Time complexity is O(n) where n is the total bytes processed (message + padding + 128 bytes for the two HMAC key blocks). Space complexity is O(1) for the HMAC logic, but SHA-256 internally uses a 64-word schedule, so O(64) auxiliary storage — effectively O(1) in practical terms.
