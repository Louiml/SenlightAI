Write a C++ function that computes a simple paged attention score for a single query head against a sequence of key blocks stored in a paged cache, producing a vector of logits (pre-softmax attention scores). The function should accept a query vector of length `HEAD_SIZE` (a compile-time constant), a pointer to the key cache laid out as `[num_blocks, HEAD_SIZE/x, block_size, x]` where `x = 16/sizeof(scalar_t)` and `scalar_t` is `float` or a 16-bit type (for simplicity assume `float` only), a block size (fixed at 16), the number of blocks, the scale factor, and an output buffer for logits of length `num_blocks * block_size`. The function must compute for each block and each token in the block the dot product between the query and the corresponding key vector (respecting the padded layout where the last dimension `x` holds contiguous elements), multiply by the scale, and write the result into the logits buffer. Handle the case where the last block may be partially filled (i.e., the actual sequence length is known); for tokens beyond the sequence length, write zero into logits. The solution must be self-contained, usable for any `HEAD_SIZE` multiple of 4, and operate in-place on the logits buffer without external allocations.
The core operation is a standard matrix-vector product between the query (size `HEAD_SIZE`) and each key vector (also size `HEAD_SIZE`) extracted from the cache. The cache layout for keys is `[num_blocks, HEAD_SIZE/x, block_size, x]` where `x = 16/sizeof(scalar_t)`. For `float`, `x = 4`, meaning each key vector of length `HEAD_SIZE` is stored as `HEAD_SIZE/x` groups, each group containing `block_size` (16) rows of `x` (4) contiguous floats. To compute the dot product for a given token index `token_idx` in a block, we iterate over the `HEAD_SIZE/x` groups, and within each group, sum the products of the query elements (which are contiguous in a flat array of length `HEAD_SIZE`) and the key elements at positions `[group_idx * block_size * x + token_idx * x + lane]` for `lane` in 0..x-1. After accumulating the dot product, multiply by the scale and store in the logits buffer at position `block_idx * block_size + token_idx`. For partially filled last block, only iterate up to the actual token count; fill the remainder of the logits buffer for that block with zeros. Edge cases: if `HEAD_SIZE` is not divisible by `x`, it's invalid (we assume multiples of 4 for float). If the sequence length is zero, the function does nothing. Time complexity is O(num_blocks * block_size * HEAD_SIZE), which is linear in the total number of key tokens times head size; space is O(1) auxiliary. The implementation must be careful with pointer arithmetic based on the layout.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <algorithm>

// Computes paged attention logits for one query head.
// q: pointer to query vector of length HEAD_SIZE (row-major).
// k_cache: pointer to key cache layout [num_blocks, HEAD_SIZE/4, block_size, 4] for float.
// logits: output buffer of length num_blocks * block_size (float).
// scale: multiplicative factor for dot product.
// seq_len: total number of valid tokens across all blocks (may be less than num_blocks*block_size).
// head_size: dimension of query/key vectors (must be multiple of 4).
// block_size: fixed at 16.
// num_blocks: number of blocks in cache.
template <typename scalar_t, int HEAD_SIZE, int BLOCK_SIZE>
void paged_attention_logits(const scalar_t* q, const scalar_t* k_cache,
                            float* logits, float scale,
                            int seq_len, int num_blocks) {
    constexpr int x = 16 / sizeof(scalar_t); // For float, x=4
    static_assert(HEAD_SIZE % x == 0, "HEAD_SIZE must be multiple of x");
    static_assert(BLOCK_SIZE == 16, "BLOCK_SIZE must be 16");

    int tokens_in_full_blocks = 0;
    for (int block_idx = 0; block_idx < num_blocks; ++block_idx) {
        int tokens_in_this_block = BLOCK_SIZE;
        if (block_idx == num_blocks - 1) {
            tokens_in_this_block = seq_len - (num_blocks - 1) * BLOCK_SIZE;
            if (tokens_in_this_block < 0) tokens_in_this_block = 0;
            // If the last block has no tokens (seq_len <= (num_blocks-1)*BLOCK_SIZE), skip? Actually num_blocks should be computed from seq_len.
        }
        // For safety, clamp tokens to valid range
        int actual_tokens = std::min(tokens_in_this_block, BLOCK_SIZE);
        if (actual_tokens <= 0) actual_tokens = 0;

        const scalar_t* k_block = k_cache + block_idx * (HEAD_SIZE / x) * BLOCK_SIZE * x;

        for (int token_idx = 0; token_idx < actual_tokens; ++token_idx) {
            float dot = 0.0f;
            // Iterate over groups of x contiguous elements
            for (int group = 0; group < HEAD_SIZE / x; ++group) {
                const scalar_t* k_group = k_block + group * BLOCK_SIZE * x + token_idx * x;
                const scalar_t* q_group = q + group * x;
                for (int lane = 0; lane < x; ++lane) {
                    dot += static_cast<float>(q_group[lane]) * static_cast<float>(k_group[lane]);
                }
            }
            logits[block_idx * BLOCK_SIZE + token_idx] = dot * scale;
        }

        // Fill remaining positions with zero (for partial last block)
        for (int token_idx = actual_tokens; token_idx < BLOCK_SIZE; ++token_idx) {
            logits[block_idx * BLOCK_SIZE + token_idx] = 0.0f;
        }
        tokens_in_full_blocks += actual_tokens;
    }
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Simple case with HEAD_SIZE=4, BLOCK_SIZE=16, 1 full block
    constexpr int HEAD_SIZE = 4;
    constexpr int BLOCK_SIZE = 16;
    constexpr int x = 4; // float
    constexpr int NUM_BLOCKS = 1;
    
    // Query: [1,2,3,4]
    float q[HEAD_SIZE] = {1,2,3,4};
    
    // Key cache layout [num_blocks=1, HEAD_SIZE/x=1, block_size=16, x=4]
    // For simplicity, fill with known values: token i vector = [i, i, i, i] for i=0..15
    float k_cache[NUM_BLOCKS * 1 * BLOCK_SIZE * x];
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        for (int l = 0; l < x; ++l) {
            k_cache[i * x + l] = static_cast<float>(i);
        }
    }
    
    float logits[NUM_BLOCKS * BLOCK_SIZE] = {0};
    float scale = 1.0f;
    int seq_len = 16;
    paged_attention_logits<float, HEAD_SIZE, BLOCK_SIZE>(q, k_cache, logits, scale, seq_len, NUM_BLOCKS);
    
    // Expected: dot = 1*i + 2*i + 3*i + 4*i = 10*i
    for (int i = 0; i < 16; ++i) {
        assert(std::abs(logits[i] - 10.0f * i) < 1e-5);
    }
    
    // Test 2: Partial last block (seq_len = 5, 1 block)
    seq_len = 5;
    // Reuse same cache but only first 5 tokens have valid scores
    paged_attention_logits<float, HEAD_SIZE, BLOCK_SIZE>(q, k_cache, logits, scale, seq_len, NUM_BLOCKS);
    for (int i = 0; i < 5; ++i) {
        assert(std::abs(logits[i] - 10.0f * i) < 1e-5);
    }
    for (int i = 5; i < 16; ++i) {
        assert(logits[i] == 0.0f);
    }
    
    // Test 3: Multiple blocks (2 blocks, second partial)
    constexpr int NUM_BLOCKS2 = 2;
    float k_cache2[NUM_BLOCKS2 * 1 * BLOCK_SIZE * x];
    // Fill block 0: tokens 0..15 with values i*1.0 (as before)
    for (int i = 0; i < 16; ++i) {
        for (int l = 0; l < x; ++l) {
            k_cache2[i * x + l] = static_cast<float>(i);
        }
    }
    // Fill block 1: tokens 16..31 with values i*2.0 (for i=0..15 meaning token index i+16)
    for (int i = 0; i < 16; ++i) {
        for (int l = 0; l < x; ++l) {
            k_cache2[(16 + i) * x + l] = static_cast<float>(2 * i);
        }
    }
    // Query same [1,2,3,4] -> dot for block0 token i = 10*i, block1 token i = 10*2*i = 20*i
    float logits2[NUM_BLOCKS2 * BLOCK_SIZE] = {0};
    int seq_len2 = 20; // 16 full + 4 in second block
    paged_attention_logits<float, HEAD_SIZE, BLOCK_SIZE>(q, k_cache2, logits2, scale, seq_len2, NUM_BLOCKS2);
    for (int i = 0; i < 16; ++i) {
        assert(std::abs(logits2[i] - 10.0f * i) < 1e-5);
    }
    // Second block tokens 0..3 correspond to global tokens 16..19, values = 2*i for local i=0..3
    for (int i = 0; i < 4; ++i) {
        assert(std::abs(logits2[16 + i] - 20.0f * i) < 1e-5);
    }
    for (int i = 4; i < 16; ++i) {
        assert(logits2[16 + i] == 0.0f);
    }
    
    // Test 4: Scale factor
    scale = 0.5f;
    paged_attention_logits<float, HEAD_SIZE, BLOCK_SIZE>(q, k_cache2, logits2, scale, seq_len2, NUM_BLOCKS2);
    for (int i = 0; i < 16; ++i) {
        assert(std::abs(logits2[i] - 5.0f * i) < 1e-5); // 10*i * 0.5
    }
    for (int i = 0; i < 4; ++i) {
        assert(std::abs(logits2[16 + i] - 10.0f * i) < 1e-5); // 20*i * 0.5
    }
    
    // Test 5: Zero length
    seq_len2 = 0;
    paged_attention_logits<float, HEAD_SIZE, BLOCK_SIZE>(q, k_cache2, logits2, scale, seq_len2, NUM_BLOCKS2);
    for (int i = 0; i < NUM_BLOCKS2 * BLOCK_SIZE; ++i) {
        assert(logits2[i] == 0.0f);
    }
    
    return 0;
}
