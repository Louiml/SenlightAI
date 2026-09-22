Write a C++ function `decode_3i40_14bits` that decodes a 14-bit algebraic codebook index and a 3-bit sign field into a fixed codebook excitation vector of length `L_SUBFR = 40` (define this constant inside the header or as a `constexpr`). The function takes a `std::uint16_t` for `sign` (only the lower 3 bits used), a `std::uint16_t` for `index` (14 bits used), and a `std::array<int, 40>&` or `std::vector<int>&` output parameter `cod`. The decoding follows the exact bit manipulation algorithm from the provided snippet: extract three pulse positions (each in range 0–39) using a series of shifts and masks, then for each pulse set the corresponding element in `cod` to either `8191` (if the corresponding sign bit is 1) or `-8192` (if the sign bit is 0), and initialize all elements of `cod` to zero first. The function must handle any bit pattern in `sign` and `index` (including unused higher bits) by masking appropriately. The output vector should have exactly 40 elements.

// The core algorithm extracts three pulse positions from the 14-bit index using a bit-field layout: the first pulse position is stored in the lowest 3 bits, then a 1-bit flag, the second pulse position in the next 3 bits, then another 1-bit flag, and the third pulse position in the top 3 bits. Specifically, we mask `index & 0x7` to get `i` (0–7), then `pos[0] = i*5` (yielding 0,5,...,35). Then shift right by 3 to get the first flag `j` (0 or 1) and shift right again by 1; then mask `index & 0x7` for the second position `i`, compute `pos[1] = i*5 + j*2 + 1` (yielding values 1,2,...,38). Repeat for the third pulse: shift right by 3, get flag `j`, shift right by 1, mask `index & 0x7`, compute `pos[2] = i*5 + j*2 + 2` (yielding 2,3,...,39). After building the three positions, set all 40 entries of `cod` to zero. Then for each pulse `j` from 0 to 2, inspect the `j`-th bit of `sign` (by shifting right and masking with 1); if the bit is 1, set `cod[pos[j]] = 8191`, else set `cod[pos[j]] = -8192`. Edge cases: any 14-bit index or 3-bit sign value works; positions are always valid because i is 0–7, j is 0–1, and the formula yields max 7*5+1*2+1 = 38 for pos[1] and 7*5+1*2+2 = 39 for pos[2], all within 0–39. The function should handle sign and index as integers (possibly larger than 14 bits) by masking internally. Time complexity is O(L_SUBFR) for clearing the vector plus O(3) for setting pulses, which is O(1) since L_SUBFR is constant. Space usage is O(L_SUBFR) for the output vector.

#include <array>
#include <cstdint>

constexpr int L_SUBFR = 40;

// Decode a 14-bit algebraic codebook index and 3-bit sign into a fixed codebook excitation.
// The output cod is a 40-element array of int values, each either 0, 8191, or -8192.
void decode_3i40_14bits(std::uint16_t sign, std::uint16_t index, std::array<int, L_SUBFR>& cod) {
    // Bit fields: index holds three 3-bit position codes interleaved with 1-bit flags.
    // sign holds three bits, one per pulse.

    int pos[3];

    // First pulse position: low 3 bits of index, multiplied by 5.
    int i = index & 0x7;
    pos[0] = i * 5;
    index >>= 3;

    // Second pulse: next 3 bits after a 1-bit flag.
    int j = index & 0x1;
    index >>= 1;
    i = index & 0x7;
    pos[1] = i * 5 + j * 2 + 1;
    index >>= 3;

    // Third pulse: next 3 bits after another 1-bit flag.
    j = index & 0x1;
    index >>= 1;
    i = index & 0x7;
    pos[2] = i * 5 + j * 2 + 2;

    // Initialize all entries to zero.
    cod.fill(0);

    // Apply sign and set the three pulses.
    for (int pulse = 0; pulse < 3; ++pulse) {
        int bit = (sign >> pulse) & 1;
        // If bit is 1 -> 8191, else -8192.
        cod[pos[pulse]] = bit * 16383 - 8192;
    }
}

#include <cassert>
#include <array>

int main() {
    // Test case 1: all zeros in index and sign.
    std::array<int, L_SUBFR> cod;
    decode_3i40_14bits(0, 0, cod);
    assert(cod[0] == -8192);
    assert(cod[1] == -8192);
    assert(cod[2] == -8192);
    for (int i = 3; i < L_SUBFR; ++i) assert(cod[i] == 0);

    // Test case 2: sign all ones, index = 0 -> pulses at positions 0,1,2 with +8191.
    decode_3i40_14bits(0x7, 0, cod);
    assert(cod[0] == 8191);
    assert(cod[1] == 8191);
    assert(cod[2] == 8191);
    assert(cod[3] == 0);

    // Test case 3: index corresponds to max positions 35,38,39.
    // Construct index: pos0=35 -> i=7 -> bits 111 (low 3 bits) => starting index bits 0111.
    // Then flag=1, pos1=38 -> i=7 (since 7*5+1*2+1=38) -> after shift, bits 111.
    // Then flag=1, pos2=39 -> i=7 (since 7*5+1*2+2=39) -> bits 111.
    // Combine: index = (7<<8) | (1<<7) | (7<<4) | (1<<3) | 7
    std::uint16_t idx = (7u << 8) | (1u << 7) | (7u << 4) | (1u << 3) | 7u;
    decode_3i40_14bits(0x5, idx, cod); // signs: bit0=1, bit1=0, bit2=1
    assert(cod[35] == 8191);
    assert(cod[38] == -8192);
    assert(cod[39] == 8191);

    // Test case 4: edge positions where j=0 and i=0.
    // For pos0=0, pos1=1, pos2=2 with all flags 0 and i=0 -> index=0 already done.

    // Test case 5: verify that index bits beyond bit 13 are ignored.
    decode_3i40_14bits(0, 0xFFFF, cod); // high bits ignored
    // Same as index=0 since only low 13 bits matter (14 bits actually: 3+1+3+1+3 = 11? Actually 3+1+3+1+3 = 11, but provided code uses 14 bits total).
    // The actual used bits are 11 bits (3+1+3+1+3). Test with a value that has extra upper bits set.
    std::uint16_t idx2 = 0x1FFF; // 13 bits set, but only low 11 are used? Let's compute: 14 bits total but formulas use up to bit 11? Actually the extraction uses up to 11 bits (3+1+3+1+3 = 11). Extra bits are ignored.
    decode_3i40_14bits(0x7, idx2, cod);
    // For idx2 = 0x1FFF = 0b11111111111 (11 ones). Then:
    // i=7 -> pos0=35; index>>=3 -> 0x3FF; j= index&1 =1; index>>=1 -> 0x1FF; i= index&7 =7 -> pos1=7*5+1*2+1=38; index>>=3 -> 0x3F; j= index&1 =1; index>>=1 -> 0x1F; i= index&7 =7 -> pos2=7*5+1*2+2=39.
    // Signs all 1 -> all 8191.
    assert(cod[35] == 8191);
    assert(cod[38] == 8191);
    assert(cod[39] == 8191);
    assert(cod[0] == 0);

    // Test case 6: random index and sign.
    decode_3i40_14bits(0b001, 0b110101, cod); // let's manually verify quickly:
    // index=54 (binary 110110? Actually 0b110101=53). 53 = 0b110101.
    // low 3 bits = 101 =5 -> pos0=25; shift >>3 => 0b110 =6; j=0; shift >>1 => 0b11=3; i=3 -> pos1=3*5+0*2+1=16; shift >>3 =>0; j=0; shift >>1 =>0; i=0 -> pos2=0*5+0*2+2=2.
    // signs: bit0=1 -> pos25=8191; bit1=0 -> pos16=-8192; bit2=0 -> pos2=-8192.
    decode_3i40_14bits(0b001, 0b110101, cod);
    assert(cod[25] == 8191);
    assert(cod[16] == -8192);
    assert(cod[2] == -8192);

    return 0;
}
