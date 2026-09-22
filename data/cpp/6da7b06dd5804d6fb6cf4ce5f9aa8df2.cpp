Write a standalone C++ function named `hmacSha256` that computes the HMAC-SHA256 message authentication code for a given message and key. The function must take four parameters: a pointer to the message data (`const unsigned char* msg`), the message length (`size_t msgLen`), a pointer to the key data (`const unsigned char* key`), and the key length (`size_t keyLen`). It must output the 32‑byte HMAC digest into a caller‑provided buffer (`unsigned char out[32]`). You may not use any external crypto libraries; you must implement SHA‑256 yourself. The function should handle keys longer than the SHA‑256 block size (64 bytes) by first hashing the key with SHA‑256, pad keys shorter than 64 bytes with zeros, and correctly apply the inner and outer padding as defined by the HMAC standard. The solution must be self‑contained with all necessary helper functions (e.g., SHA‑256 compression) embedded.

#include <cassert>
#include <cstring>

int main() {
    unsigned char out[32];

    // Test vectors from RFC 4231
    // Test case 1: key = 0x0b repeated 20 times, msg = "Hi There"
    unsigned char key1[20];
    std::memset(key1, 0x0b, 20);
    const char* msg1 = "Hi There";
    hmacSha256((const unsigned char*)msg1, strlen(msg1), key1, 20, out);
    unsigned char expected1[32] = {
        0xb0,0x34,0x4c,0x61,0xd8,0xdb,0x38,0x53,
        0x5c,0xa8,0xaf,0xce,0xaf,0x0b,0xf1,0x2b,
        0x88,0x1d,0xc2,0x00,0xc9,0x83,0x3d,0xa7,
        0x26,0xe9,0x37,0x6c,0x2e,0x32,0xcf,0xf7
    };
    assert(std::memcmp(out, expected1, 32) == 0);

    // Test case 2: key = "Jefe", msg = "what do ya want for nothing?"
    const char* key2 = "Jefe";
    const char* msg2 = "what do ya want for nothing?";
    hmacSha256((const unsigned char*)msg2, strlen(msg2),
               (const unsigned char*)key2, strlen(key2), out);
    unsigned char expected2[32] = {
        0x5b,0xdc,0xc1,0x46,0xbf,0x60,0x75,0x4e,
        0x6a,0x04,0x24,0x26,0x08,0x95,0x75,0xc7,
        0x5a,0x00,0x3f,0x08,0x9d,0x27,0x39,0x83,
        0x9d,0xec,0x58,0xb9,0x64,0xec,0x38,0x43
    };
    assert(std::memcmp(out, expected2, 32) == 0);

    // Test case 3: key = 0xaa repeated 20, msg = 0xdd repeated 50
    unsigned char key3[20];
    std::memset(key3, 0xaa, 20);
    unsigned char msg3[50];
    std::memset(msg3, 0xdd, 50);
    hmacSha256(msg3, 50, key3, 20, out);
    unsigned char expected3[32] = {
        0x77,0x3e,0xa9,0x1e,0x36,0x80,0x0e,0x46,
        0x85,0x4d,0xb8,0xeb,0xd0,0x91,0x81,0xa7,
        0x29,0x59,0x09,0x8b,0x3e,0xf8,0xc1,0x22,
        0xd9,0x63,0x55,0x14,0xce,0xd5,0x65,0xfe
    };
    assert(std::memcmp(out, expected3, 32) == 0);

    // Test case with empty message
    unsigned char key4[1] = {0x0b};
    hmacSha256(nullptr, 0, key4, 1, out);
    unsigned char expected4[32] = {
        0xfb,0xf1,0x38,0x9c,0x17,0x12,0x2a,0x74,
        0xfe,0x4e,0x81,0x0c,0xe2,0x9b,0x1f,0x0b,
        0x5c,0x0a,0x2a,0x7d,0x8b,0xe7,0x1e,0x8a,
        0x7a,0x1d,0x6f,0x0b,0x4a,0x5d,0x8a,0x4f
    };
    assert(std::memcmp(out, expected4, 32) == 0);

    // Test with key longer than 64 bytes (use key = 0xaa repeated 131)
    unsigned char key5[131];
    std::memset(key5, 0xaa, 131);
    hmacSha256((const unsigned char*)"Test Using Larger Than Block-Size Key - Hash Key First",
               54, key5, 131, out);
    unsigned char expected5[32] = {
        0x60,0xe4,0x31,0x59,0x1e,0xe0,0xb6,0x7f,
        0x0d,0x8a,0x26,0x23,0xa7,0xcb,0x5e,0xf7,
        0x9d,0x8d,0x7e,0x2a,0x9e,0x4d,0x1b,0x8c,
        0x5c,0x3b,0x0f,0x2e,0x1a,0x3c,0x6e,0x9a
    };
    assert(std::memcmp(out, expected5, 32) == 0);

    return 0;
}

#include <cstdint>
#include <cstring>

// SHA-256 implementation (compact, non-streaming).
struct CSHA256 {
    uint32_t h[8];
    uint64_t totalLen;
    unsigned char buffer[64];
    size_t bufferLen;

    CSHA256() {
        h[0] = 0x6a09e667;
        h[1] = 0xbb67ae85;
        h[2] = 0x3c6ef372;
        h[3] = 0xa54ff53a;
        h[4] = 0x510e527f;
        h[5] = 0x9b05688c;
        h[6] = 0x1f83d9ab;
        h[7] = 0x5be0cd19;
        totalLen = 0;
        bufferLen = 0;
    }

    static uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

    void processBlock(const unsigned char* block) {
        uint32_t w[64];
        for (int i = 0; i < 16; i++) {
            w[i] = (uint32_t)block[i*4] << 24 | (uint32_t)block[i*4+1] << 16 |
                   (uint32_t)block[i*4+2] << 8 | (uint32_t)block[i*4+3];
        }
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = rotr(w[i-15], 7) ^ rotr(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32_t s1 = rotr(w[i-2], 17) ^ rotr(w[i-2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];

        static const uint32_t K[64] = {
            0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
            0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
            0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
            0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
            0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
            0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
            0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
            0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
        };

        for (int i = 0; i < 64; i++) {
            uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = hh + S1 + ch + K[i] + w[i];
            uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;
            hh = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    void write(const unsigned char* data, size_t len) {
        totalLen += len;
        while (len > 0) {
            size_t toCopy = 64 - bufferLen;
            if (toCopy > len) toCopy = len;
            std::memcpy(buffer + bufferLen, data, toCopy);
            bufferLen += toCopy;
            data += toCopy;
            len -= toCopy;
            if (bufferLen == 64) {
                processBlock(buffer);
                bufferLen = 0;
            }
        }
    }

    void finalize(unsigned char out[32]) {
        uint64_t bitLen = totalLen * 8;
        unsigned char pad = 0x80;
        write(&pad, 1);
        unsigned char zero = 0;
        while (bufferLen != 56) {
            write(&zero, 1);
        }
        unsigned char lenBytes[8];
        for (int i = 0; i < 8; i++) {
            lenBytes[i] = (unsigned char)(bitLen >> (56 - 8*i));
        }
        write(lenBytes, 8);

        for (int i = 0; i < 8; i++) {
            out[i*4]   = (unsigned char)(h[i] >> 24);
            out[i*4+1] = (unsigned char)(h[i] >> 16);
            out[i*4+2] = (unsigned char)(h[i] >> 8);
            out[i*4+3] = (unsigned char)h[i];
        }
    }
};

// Compute HMAC-SHA256 of message `msg` (length `msgLen`) with key `key` (length `keyLen`).
// Output is 32 bytes written to `out`.
void hmacSha256(const unsigned char* msg, size_t msgLen,
                const unsigned char* key, size_t keyLen,
                unsigned char out[32]) {
    unsigned char rkey[64];
    if (keyLen <= 64) {
        if (keyLen > 0) std::memcpy(rkey, key, keyLen);
        std::memset(rkey + keyLen, 0, 64 - keyLen);
    } else {
        CSHA256 hasher;
        hasher.write(key, keyLen);
        hasher.finalize(rkey);
        std::memset(rkey + 32, 0, 32);
    }

    unsigned char innerKey[64];
    unsigned char outerKey[64];
    for (int i = 0; i < 64; i++) {
        innerKey[i] = rkey[i] ^ 0x36;
        outerKey[i] = rkey[i] ^ 0x5c;
    }

    // Inner hash: SHA256(innerKey || msg)
    CSHA256 inner;
    inner.write(innerKey, 64);
    inner.write(msg, msgLen);
    unsigned char innerHash[32];
    inner.finalize(innerHash);

    // Outer hash: SHA256(outerKey || innerHash)
    CSHA256 outer;
    outer.write(outerKey, 64);
    outer.write(innerHash, 32);
    outer.finalize(out);
}

// The HMAC construction is defined as: `HMAC(K, m) = SHA256( (K' ⊕ opad) || SHA256( (K' ⊕ ipad) || m ) )`, where `K'` is the key padded to 64 bytes (if the original key is longer than 64 bytes, first replace it with SHA256(key) and zero‑pad the remaining 32 bytes). The `ipad` is the byte `0x36` repeated 64 times, and `opad` is `0x5c` repeated 64 times. The algorithm proceeds as:
// 1. Compute the effective key block `rkey[64]`:
//    - If `keyLen <= 64`, copy the key and zero‑pad to 64 bytes.
//    - If `keyLen > 64`, compute SHA256 of the key, place the 32‑byte digest at the start of `rkey`, and zero‑fill the remaining 32 bytes.
// 2. XOR `rkey` with `opad` to form the outer key block, and XOR a separate copy with `ipad` to form the inner key block.
// 3. Compute `innerHash = SHA256( (rkey XOR ipad) || message )`.
// 4. Compute `finalHash = SHA256( (rkey XOR opad) || innerHash )`, and copy it to the output buffer.
//
// The core dependency is a correct SHA‑256 implementation, which must handle arbitrary‑length messages, including padding (append a `0x80` byte, then zeros, then a 64‑bit big‑endian bit length) and process data in 64‑byte blocks. Edge cases include empty messages, empty keys, keys of exactly 64 bytes, and keys longer than 64 bytes. The time complexity is linear in the message length, \(O(n)\), and constant auxiliary space beyond the input and output buffers. The reference solution implements SHA‑256 incrementally with an internal state and a buffer, allowing streaming but the function here processes the entire message at once.
