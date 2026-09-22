// Write a C++ function named `pack_and_unpack_32bit` that takes a `const std::vector<uint32_t>&` of exactly 32 unsigned 32-bit integers, each assumed to be in the range `[0, 2^7 - 1]` (i.e., representable in 7 bits), and returns a `std::vector<uint32_t>` containing the same 32 original values after they have been bit-packed into consecutive 7-bit slots across a byte stream (backed by `uint32_t` words) and then unpacked back. The function must implement the bit-packing scheme where 32 values of `DELTA = 7` bits are packed sequentially: the first value occupies bits `[0,6]` of the first 32-bit word, the second occupies bits `[7,13]`, and so on, crossing word boundaries naturally. The unpacking must reconstruct the exact original values in order. You may not use external bit-packing libraries; implement the packing and unpacking logic manually using bitwise operations and masks. The function should work correctly when the input contains the maximum value `127`, small values like `0`, and values that cause bit fields to span across two or three 32-bit words.
// The core algorithm is bit-packing: for `DELTA = 7` bits and `N = 32` values, the total packed bit count is `32 * 7 = 224` bits, which fits into exactly 7 `uint32_t` words (7 * 32 = 224). The packing iterates over each input value and places its 7 bits at an absolute bit offset `bitpos = i * 7`. Within the packed output words, for each value we determine the starting word index (`bitpos / 32`) and the bit shift within that word (`bitpos % 32`). If the 7 bits fit entirely within one word (shift + 7 <= 32), we OR the masked value shifted left into that word. If they cross into the next word, we place the low `(32 - shift)` bits into the current word and the remaining `(7 - (32 - shift))` bits into the next word at shift 0. The unpacking reverses this: for each output index, compute the same absolute bit offset, extract the 7 bits by reading from the corresponding word(s). If the field fits in one word, extract via `(word >> shift) & mask`. If it crosses a boundary, combine the high bits from one word and the low bits from the next. Edge cases include shift = 0 (no OR needed, direct assignment), shift close to 32 (crossing exactly one bit), and the final value where the bit field may end exactly at the last word. After unpacking, compare the result to the input. Time complexity is `O(N)` = `O(32)` constant, space complexity is `O(1)` auxiliary besides the output vectors.
#include <cstdint>
#include <vector>

// Packs 32 uint32_t values (each in [0,127]) into 7-bit fields,
// then unpacks them back, returning the reconstructed vector.
std::vector<uint32_t> pack_and_unpack_32bit(const std::vector<uint32_t>& input) {
    constexpr uint32_t DELTA = 7;
    constexpr uint32_t N = 32;
    constexpr uint32_t MASK = (1u << DELTA) - 1;
    constexpr uint32_t WORDS = (N * DELTA + 31) / 32;  // = 7

    // Step 1: Pack the 32 values into WORDS 32-bit words.
    std::vector<uint32_t> packed(WORDS, 0);
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t value = input[i] & MASK;  // ensure only DELTA bits
        uint32_t bitpos = i * DELTA;
        uint32_t word_index = bitpos / 32;
        uint32_t shift = bitpos % 32;
        if (shift + DELTA <= 32) {
            // Fits entirely in current word.
            packed[word_index] |= (value << shift);
        } else {
            // Crosses into next word.
            uint32_t low_bits = 32 - shift;
            packed[word_index] |= (value << shift);  // lower (low_bits) bits
            uint32_t high_bits = DELTA - low_bits;
            packed[word_index + 1] |= (value >> low_bits) & ((1u << high_bits) - 1);
        }
    }

    // Step 2: Unpack the WORDS words back into N values.
    std::vector<uint32_t> output(N, 0);
    for (uint32_t i = 0; i < N; ++i) {
        uint32_t bitpos = i * DELTA;
        uint32_t word_index = bitpos / 32;
        uint32_t shift = bitpos % 32;
        uint32_t value;
        if (shift + DELTA <= 32) {
            value = (packed[word_index] >> shift) & MASK;
        } else {
            // Read from two adjacent words.
            uint32_t low_bits = 32 - shift;
            value = (packed[word_index] >> shift) & ((1u << low_bits) - 1);
            uint32_t high_bits = DELTA - low_bits;
            value |= (packed[word_index + 1] & ((1u << high_bits) - 1)) << low_bits;
        }
        output[i] = value;
    }
    return output;
}
#include <cassert>
#include <cstdint>
#include <vector>

// Solution function declaration (assume it is defined above or in a header).
std::vector<uint32_t> pack_and_unpack_32bit(const std::vector<uint32_t>& input);

int main() {
    // Test 1: All zeros.
    std::vector<uint32_t> zeros(32, 0);
    assert(pack_and_unpack_32bit(zeros) == zeros);

    // Test 2: All maximum value 127.
    std::vector<uint32_t> maxes(32, 127);
    assert(pack_and_unpack_32bit(maxes) == maxes);

    // Test 3: Alternating 0 and 127.
    std::vector<uint32_t> alt(32);
    for (int i = 0; i < 32; ++i) alt[i] = (i % 2 == 0) ? 0 : 127;
    assert(pack_and_unpack_32bit(alt) == alt);

    // Test 4: A pattern that forces crossing many word boundaries.
    std::vector<uint32_t> pattern(32);
    for (int i = 0; i < 32; ++i) pattern[i] = (i * 17) % 128;
    assert(pack_and_unpack_32bit(pattern) == pattern);

    // Test 5: Sequential values 0..31.
    std::vector<uint32_t> seq(32);
    for (int i = 0; i < 32; ++i) seq[i] = i;
    assert(pack_and_unpack_32bit(seq) == seq);

    // Test 6: Boundary values around word crossings (e.g., indices 4, 5, 9, 10, 14, 15, etc.).
    std::vector<uint32_t> boundary(32, 0);
    boundary[4] = 127;
    boundary[5] = 1;
    boundary[9] = 127;
    boundary[10] = 2;
    boundary[14] = 127;
    boundary[15] = 3;
    boundary[19] = 127;
    boundary[20] = 4;
    boundary[23] = 127;
    boundary[24] = 5;
    boundary[27] = 127;
    boundary[28] = 6;
    boundary[31] = 127;
    assert(pack_and_unpack_32bit(boundary) == boundary);

    return 0;
}
