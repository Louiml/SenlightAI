Write a C++ function named `derive_ckdf_key` that implements the NIST SP 800-108 KDF in Counter Mode using AES-CMAC as the PRF, matching the behavior of the provided `ckdf` function. The function must take a 16-byte or 32-byte key, a label (as a byte array), a vector of context chunks (each a byte array), and an output size in bytes. It must produce an output byte vector of exactly that size by generating AES-CMACs over the concatenation `i || label || 0x00 || context_chunks... || L` for each counter block `i = 1, 2, ...` where `L` is the output size in bits, both `i` and `L` in network byte order (big-endian). The output must be the concatenation of the truncated CMACs. The function must return a boolean indicating success (true) or failure (false) for invalid key sizes, allocation errors, or OpenSSL operation errors. Your implementation must be self-contained (not rely on keymaster types), use `EVP_aes_128_cbc` and `EVP_aes_256_cbc` for the CMAC algorithm depending on key size, and include proper `const` correctness and header includes.
The solution needs to replicate the counter-mode KDF logic. The main algorithm is: given an output length `out_len_bytes`, compute the number of AES blocks needed by rounding up (`ceil(out_len / 16)`). For each block index `i` from 1 to that count, construct a message `i (4-byte big-endian) || label || 0x00 || context (concatenated) || L (4-byte big-endian)`. Compute the AES-CMAC of that message with the given key. Append the full 16-byte CMAC to the output, but for the last block truncate to the remaining bytes if the output size is not a multiple of 16. Important edge cases: key must be exactly 16 or 32 bytes; if output length is zero, the function should return true with an empty output (no blocks computed); ensure the OpenSSL CMAC context is reset after each block and finalized correctly; handle potential truncation when the remaining output bytes are fewer than 16. Time complexity is O(n * m) where n is number of blocks and m is total input length per block (label + context), which is linear in output size plus input lengths. Space complexity is O(out_len + context total length) for the output and the message buffer if we construct one, but we can stream the message parts directly into CMAC_Update to keep auxiliary space O(1) aside from the output. The implementation must use `CMAC_Init`, `CMAC_Update`, `CMAC_Final`, and `CMAC_Reset`. Also need to convert `uint32_t` to network byte order using `htonl` (from `<arpa/inet.h>` or `<netinet/in.h>` on POSIX; for portability we can manually byte-swap). We'll assume a big-endian conversion helper using `htonl`.
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <openssl/cmac.h>
#include <openssl/evp.h>
#include <openssl/err.h>

// Helper to convert a uint32_t to network byte order (big-endian)
static uint32_t hton32(uint32_t value) {
    // Assume little-endian or big-endian; use explicit byte shuffle
    return ((value & 0xFF) << 24) |
           ((value & 0xFF00) << 8) |
           ((value & 0xFF0000) >> 8) |
           ((value >> 24) & 0xFF);
}

// Structure to hold binary data
struct ByteArray {
    const uint8_t* data;
    size_t size;
};

// NIST SP 800-108 KDF in Counter Mode using AES-CMAC
bool derive_ckdf_key(const std::vector<uint8_t>& key,
                     const ByteArray& label,
                     const std::vector<ByteArray>& context_chunks,
                     size_t output_size,
                     std::vector<uint8_t>& output) {
    if (key.size() != 16 && key.size() != 32) {
        return false;
    }

    output.clear();
    if (output_size == 0) {
        return true;
    }

    const uint32_t blocks = (output_size + 15) / 16;  // ceil division
    const uint32_t L = static_cast<uint32_t>(output_size * 8);  // bits

    CMAC_CTX* ctx = CMAC_CTX_new();
    if (!ctx) return false;

    const EVP_CIPHER* algo = (key.size() == 16) ? EVP_aes_128_cbc() : EVP_aes_256_cbc();
    if (!CMAC_Init(ctx, key.data(), key.size(), algo, nullptr)) {
        CMAC_CTX_free(ctx);
        return false;
    }

    uint8_t net_order_L[4];
    uint32_t htonL = hton32(L);
    std::memcpy(net_order_L, &htonL, sizeof(net_order_L));

    output.resize(output_size);
    uint8_t* output_pos = output.data();

    for (uint32_t i = 1; i <= blocks; ++i) {
        // i in network byte order
        uint32_t net_i = hton32(i);
        uint8_t net_i_bytes[4];
        std::memcpy(net_i_bytes, &net_i, sizeof(net_i));

        if (!CMAC_Update(ctx, net_i_bytes, sizeof(net_i_bytes))) {
            CMAC_CTX_free(ctx);
            return false;
        }

        if (label.data != nullptr && label.size > 0) {
            if (!CMAC_Update(ctx, label.data, label.size)) {
                CMAC_CTX_free(ctx);
                return false;
            }
        }

        uint8_t zero = 0;
        if (!CMAC_Update(ctx, &zero, sizeof(zero))) {
            CMAC_CTX_free(ctx);
            return false;
        }

        for (const auto& chunk : context_chunks) {
            if (chunk.data != nullptr && chunk.size > 0) {
                if (!CMAC_Update(ctx, chunk.data, chunk.size)) {
                    CMAC_CTX_free(ctx);
                    return false;
                }
            }
        }

        if (!CMAC_Update(ctx, net_order_L, sizeof(net_order_L))) {
            CMAC_CTX_free(ctx);
            return false;
        }

        size_t out_len = 0;
        uint8_t cmac[16];
        if (!CMAC_Final(ctx, cmac, &out_len)) {
            CMAC_CTX_free(ctx);
            return false;
        }

        if (output_size >= 16) {
            std::memcpy(output_pos, cmac, out_len);
            output_pos += out_len;
            output_size -= out_len;
        } else {
            // Truncate the last CMAC to remaining bytes
            std::memcpy(output_pos, cmac, output_size);
            output_pos += output_size;
            output_size = 0;
        }

        if (i < blocks) {
            if (!CMAC_Reset(ctx)) {
                CMAC_CTX_free(ctx);
                return false;
            }
        }
    }

    CMAC_CTX_free(ctx);
    return true;
}
#include <cassert>
#include <vector>
#include <cstdint>
#include <cstring>

// The solution function is as above. Test it here.
// (In a real solution, include the definition from above.)

int main() {
    // Test with a 16-byte key, known label and context, derive 32 bytes
    std::vector<uint8_t> key = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    ByteArray label = { reinterpret_cast<const uint8_t*>("test-label"), 10 };
    std::vector<ByteArray> contexts;
    ByteArray ctx1 = { reinterpret_cast<const uint8_t*>("ctx1"), 4 };
    ByteArray ctx2 = { nullptr, 0 };  // empty chunk
    contexts.push_back(ctx1);
    contexts.push_back(ctx2);

    std::vector<uint8_t> output;
    bool ok = derive_ckdf_key(key, label, contexts, 32, output);
    assert(ok);
    assert(output.size() == 32);

    // Test that output is deterministic
    std::vector<uint8_t> output2;
    ok = derive_ckdf_key(key, label, contexts, 32, output2);
    assert(ok && output == output2);

    // Test zero output size
    std::vector<uint8_t> empty_out;
    ok = derive_ckdf_key(key, label, contexts, 0, empty_out);
    assert(ok && empty_out.empty());

    // Test invalid key size
    std::vector<uint8_t> bad_key = { 0x01, 0x02, 0x03 };  // 3 bytes
    ok = derive_ckdf_key(bad_key, label, contexts, 16, output);
    assert(!ok);

    // Test 32-byte key
    std::vector<uint8_t> key32(32, 0xab);
    ok = derive_ckdf_key(key32, label, contexts, 20, output);
    assert(ok && output.size() == 20);

    // Test output size not multiple of 16 (e.g., 17 bytes)
    ok = derive_ckdf_key(key, label, contexts, 17, output);
    assert(ok && output.size() == 17);

    // Compare with a manual simple case: key all zeros, label empty, no context, output 16 bytes
    std::vector<uint8_t> zero_key(16, 0x00);
    ByteArray empty_label = { nullptr, 0 };
    std::vector<ByteArray> no_ctx;
    ok = derive_ckdf_key(zero_key, empty_label, no_ctx, 16, output);
    assert(ok && output.size() == 16);

    // The first block message is: 0x00000001 || (empty label) || 0x00 || (no context) || 0x00000080
    // Expected CMAC can be computed externally, but just verify it's non-zero and repeatable
    std::vector<uint8_t> output_copy = output;
    ok = derive_ckdf_key(zero_key, empty_label, no_ctx, 16, output2);
    assert(ok && output2 == output_copy);

    // Ensure different key produces different output
    std::vector<uint8_t> zero_key2(16, 0x00);
    zero_key2[0] = 0x01;
    ok = derive_ckdf_key(zero_key2, empty_label, no_ctx, 16, output2);
    assert(ok && output2 != output_copy);

    return 0;
}
