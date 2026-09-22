Design a C++ function that implements a simplified version of the scrypt key-derivation algorithm without external cryptographic libraries. The function must take a password (as a vector of bytes), a salt (as a vector of bytes), and a small integer `N` (a power of two between 2 and 1024) as inputs, and return a fixed-size 64-byte derived key. The algorithm must perform exactly two rounds of PBKDF2-HMAC-SHA256—first to expand the password and salt into a 128-byte intermediate block, then again to produce the final output—with a memory-hard mixing loop in between that repeatedly applies a simple 8-round Salsa20/8 core to 64-byte chunks. You are not allowed to use external libraries like OpenSSL; instead, implement SHA-256, HMAC, PBKDF2, and Salsa20/8 from scratch. The output must be deterministic for the same inputs and `N`. Handle edge cases such as empty password, empty salt, or `N` not being a power of two by either rejecting them or documenting defined behavior.
The solution requires implementing several foundational cryptographic primitives from scratch. Start with SHA-256: define the standard 64-entry constant table, a padding routine that appends a `0x80` byte, zero bytes, and a 64-bit big-endian length, then process 512-bit blocks through the compression function using right-rotate helpers. Build HMAC-SHA256 by creating inner and outer padded keys (64 bytes each) and hashing them accordingly. PBKDF2 with one iteration reduces to `HMAC(password, salt || 0x00000001)` for each output block—since we need 128 bytes, generate 2 blocks of 32 bytes and concatenate them; for the final 64-byte output, generate 2 blocks similarly. The memory-hard core: take the 128-byte intermediate from the first PBKDF2 call, split it into two 64-byte halves, initialize a 65,536-byte buffer (for `N` up to 1024) by repeatedly applying a mix function that composes two Salsa20/8 calls—one on the first half and one on the second, with each call treating its input as a 16-word state and applying the standard column/row rounds—then store each 64-byte result sequentially. Next, perform `N` iterations: use an index extracted from the current block (e.g., the last 32-bit word modulo `N`) to XOR-fetch a stored block, apply the mix function again, and update the block. Finally, feed this mixed 128-byte block as salt into the second PBKDF2 call with the original password to produce the 64-byte key. Complexity: SHA-256 is O(m) per hash where m is message length; HMAC adds constant overhead; PBKDF2 with one iteration is O(m). The core loop is O(N * 64) time and O(N * 64) memory. Edge cases: empty password/salt still work because HMAC handles empty keys/messages; if `N` is not a power of two, either floor it to the nearest power of two or throw an exception—choose the latter for safety. Use `std::vector<uint8_t>` for byte arrays and ensure all multi-byte values are big-endian.
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>

// Right-rotate a 32-bit value.
static uint32_t rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

// SHA-256 implementation (single-message hash).
class SHA256 {
public:
    SHA256() : h{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19} { }

    void update(const uint8_t* data, size_t len) {
        // Accumulate bytes into a buffer, process complete 64-byte blocks.
        while (len > 0) {
            size_t to_copy = std::min(len, 64 - buffer_len);
            std::memcpy(buffer + buffer_len, data, to_copy);
            buffer_len += to_copy;
            data += to_copy;
            len -= to_copy;
            if (buffer_len == 64) {
                process_block(buffer);
                buffer_len = 0;
            }
        }
    }

    std::vector<uint8_t> digest() {
        // Pad: append 0x80, zeros, then 64-bit length in bits.
        uint64_t bit_count = (total_len + buffer_len) * 8;
        uint8_t pad = 0x80;
        update(&pad, 1);
        uint8_t zero = 0x00;
        while (buffer_len != 56) {
            update(&zero, 1);
        }
        // Append 8-byte big-endian bit count.
        for (int i = 7; i >= 0; --i) {
            uint8_t byte = static_cast<uint8_t>((bit_count >> (i * 8)) & 0xff);
            update(&byte, 1);
        }
        // Finalize.
        std::vector<uint8_t> out(32);
        for (size_t i = 0; i < 8; ++i) {
            out[i * 4 + 0] = (h[i] >> 24) & 0xff;
            out[i * 4 + 1] = (h[i] >> 16) & 0xff;
            out[i * 4 + 2] = (h[i] >> 8) & 0xff;
            out[i * 4 + 3] = h[i] & 0xff;
        }
        // Reset for potential reuse.
        h = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
             0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
        buffer_len = 0;
        total_len = 0;
        return out;
    }

private:
    uint32_t h[8];
    uint8_t buffer[64];
    size_t buffer_len = 0;
    size_t total_len = 0;

    static const uint32_t K[64];

    void process_block(const uint8_t* block) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) |
                   (block[i * 4 + 2] << 8) | block[i * 4 + 3];
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
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
        total_len += 64;
    }
};

const uint32_t SHA256::K[64] = {
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

// HMAC-SHA256.
std::vector<uint8_t> hmac_sha256(const std::vector<uint8_t>& key,
                                 const std::vector<uint8_t>& message) {
    std::vector<uint8_t> k(64, 0);
    if (key.size() > 64) {
        SHA256 sha;
        sha.update(key.data(), key.size());
        k = sha.digest();
        k.resize(64, 0);
    } else {
        std::copy(key.begin(), key.end(), k.begin());
    }

    std::vector<uint8_t> ipad(64), opad(64);
    for (size_t i = 0; i < 64; ++i) {
        ipad[i] = k[i] ^ 0x36;
        opad[i] = k[i] ^ 0x5c;
    }

    SHA256 inner;
    inner.update(ipad.data(), 64);
    inner.update(message.data(), message.size());
    auto inner_hash = inner.digest();

    SHA256 outer;
    outer.update(opad.data(), 64);
    outer.update(inner_hash.data(), inner_hash.size());
    return outer.digest();
}

// PBKDF2 with one iteration producing `out_len` bytes.
std::vector<uint8_t> pbkdf2_sha256_1iter(const std::vector<uint8_t>& password,
                                         const std::vector<uint8_t>& salt,
                                         size_t out_len) {
    std::vector<uint8_t> result;
    for (uint32_t block_index = 1; result.size() < out_len; ++block_index) {
        std::vector<uint8_t> msg = salt;
        msg.push_back(static_cast<uint8_t>((block_index >> 24) & 0xff));
        msg.push_back(static_cast<uint8_t>((block_index >> 16) & 0xff));
        msg.push_back(static_cast<uint8_t>((block_index >> 8) & 0xff));
        msg.push_back(static_cast<uint8_t>(block_index & 0xff));
        auto h = hmac_sha256(password, msg);
        result.insert(result.end(), h.begin(), h.end());
    }
    result.resize(out_len);
    return result;
}

// Salsa20/8 core: operates on a 64-byte block (16 uint32 words).
void salsa20_8(uint32_t state[16]) {
    uint32_t x[16];
    std::memcpy(x, state, 64);

    for (int i = 0; i < 4; ++i) { // 8 rounds -> 4 double rounds
        // Column round
        x[4] ^= rotr(x[0] + x[12], 7);
        x[9] ^= rotr(x[5] + x[1], 7);
        x[14] ^= rotr(x[10] + x[6], 7);
        x[3] ^= rotr(x[15] + x[11], 7);
        x[8] ^= rotr(x[4] + x[0], 9);
        x[13] ^= rotr(x[9] + x[5], 9);
        x[2] ^= rotr(x[14] + x[10], 9);
        x[7] ^= rotr(x[3] + x[15], 9);
        x[12] ^= rotr(x[8] + x[4], 13);
        x[1] ^= rotr(x[13] + x[9], 13);
        x[6] ^= rotr(x[2] + x[14], 13);
        x[11] ^= rotr(x[7] + x[3], 13);
        x[0] ^= rotr(x[12] + x[8], 18);
        x[5] ^= rotr(x[1] + x[13], 18);
        x[10] ^= rotr(x[6] + x[2], 18);
        x[15] ^= rotr(x[11] + x[7], 18);
        // Row round
        x[1] ^= rotr(x[0] + x[3], 7);
        x[6] ^= rotr(x[5] + x[4], 7);
        x[11] ^= rotr(x[10] + x[9], 7);
        x[12] ^= rotr(x[15] + x[14], 7);
        x[2] ^= rotr(x[1] + x[0], 9);
        x[7] ^= rotr(x[6] + x[5], 9);
        x[8] ^= rotr(x[11] + x[10], 9);
        x[13] ^= rotr(x[12] + x[15], 9);
        x[3] ^= rotr(x[2] + x[1], 13);
        x[4] ^= rotr(x[7] + x[6], 13);
        x[9] ^= rotr(x[8] + x[11], 13);
        x[14] ^= rotr(x[13] + x[12], 13);
        x[0] ^= rotr(x[3] + x[2], 18);
        x[5] ^= rotr(x[4] + x[7], 18);
        x[10] ^= rotr(x[9] + x[8], 18);
        x[15] ^= rotr(x[14] + x[13], 18);
    }

    for (int i = 0; i < 16; ++i) {
        state[i] += x[i];
    }
}

// Mix a 64-byte block in place using two salsa20/8 calls.
void mix64(uint8_t block[64]) {
    uint32_t half1[16], half2[16];
    // Interpret bytes as little-endian 32-bit words.
    for (int i = 0; i < 16; ++i) {
        uint32_t w = 0;
        for (int j = 3; j >= 0; --j) {
            w = (w << 8) | block[i * 4 + j];
        }
        half1[i] = w;
        w = 0;
        for (int j = 3; j >= 0; --j) {
            w = (w << 8) | block[64 + i * 4 + j];
        }
        half2[i] = w;
    }
    salsa20_8(half1);
    salsa20_8(half2);
    // Write back little-endian.
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            block[i * 4 + j] = (half1[i] >> (j * 8)) & 0xff;
        }
        for (int j = 0; j < 4; ++j) {
            block[64 + i * 4 + j] = (half2[i] >> (j * 8)) & 0xff;
        }
    }
}

// Simplified scrypt: returns a 64-byte derived key.
std::vector<uint8_t> simplified_scrypt(const std::vector<uint8_t>& password,
                                       const std::vector<uint8_t>& salt,
                                       size_t N) {
    if ((N & (N - 1)) != 0 || N < 2 || N > 1024) {
        throw std::invalid_argument("N must be a power of two between 2 and 1024");
    }

    // Step 1: PBKDF2 to produce 128-byte intermediate block.
    auto B = pbkdf2_sha256_1iter(password, salt, 128);

    // Step 2: Memory-hard expansion.
    std::vector<uint8_t> V(N * 64);
    for (size_t i = 0; i < N; ++i) {
        std::memcpy(&V[i * 64], &B[0], 64);
        mix64(&B[0]);
    }

    for (size_t i = 0; i < N; ++i) {
        // Index from the last 32-bit word of B (little-endian).
        uint32_t idx = (B[124] | (B[125] << 8) | (B[126] << 16) | (B[127] << 24)) & (N - 1);
        for (int k = 0; k < 64; ++k) {
            B[k] ^= V[idx * 64 + k];
        }
        mix64(&B[0]);
    }

    // Step 3: final PBKDF2 with B as salt.
    return pbkdf2_sha256_1iter(password, B, 64);
}
#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Basic test with known expected values (computed manually or via reference implementation).
    {
        std::vector<uint8_t> pw = {'p', 'a', 's', 's'};
        std::vector<uint8_t> salt = {'s', 'a', 'l', 't'};
        auto result = simplified_scrypt(pw, salt, 2);
        assert(result.size() == 64);
        // Since exact values depend on the primitive implementation, we only check size and determinism.
        auto result2 = simplified_scrypt(pw, salt, 2);
        assert(result == result2);
    }

    // Empty password and empty salt should still produce a 64-byte output.
    {
        std::vector<uint8_t> empty;
        auto result = simplified_scrypt(empty, empty, 4);
        assert(result.size() == 64);
    }

    // Different inputs should produce different outputs (collision probability negligible).
    {
        std::vector<uint8_t> pw1 = {'a'};
        std::vector<uint8_t> pw2 = {'b'};
        std::vector<uint8_t> salt = {'x'};
        auto r1 = simplified_scrypt(pw1, salt, 2);
        auto r2 = simplified_scrypt(pw2, salt, 2);
        assert(r1 != r2);
    }

    // Non-power-of-two N should be rejected.
    {
        std::vector<uint8_t> pw = {'p'};
        std::vector<uint8_t> salt = {'s'};
        bool threw = false;
        try {
            simplified_scrypt(pw, salt, 3);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // N=1 is not allowed (must be >=2).
    {
        std::vector<uint8_t> pw = {'p'};
        std::vector<uint8_t> salt = {'s'};
        bool threw = false;
        try {
            simplified_scrypt(pw, salt, 1);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Larger N should still work and produce a different result than small N.
    {
        std::vector<uint8_t> pw = {'p'};
        std::vector<uint8_t> salt = {'s'};
        auto r_small = simplified_scrypt(pw, salt, 2);
        auto r_large = simplified_scrypt(pw, salt, 8);
        assert(r_small != r_large);
    }

    return 0;
}
