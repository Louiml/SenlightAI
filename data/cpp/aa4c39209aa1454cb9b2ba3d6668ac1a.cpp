// Write a standalone C++ function `quantizePitchGain` that mimics the behavior of the given `q_gain_pitch` routine from an AMR speech codec. The function receives an integer mode (where 2 represents MR795, 3 represents MR122, and any other value represents a generic mode), a pitch gain limit `gp_limit`, and a reference to a pitch gain value `gain` (all integers in Q14 fixed-point format). It must quantize the gain against a fixed 16-entry lookup table `{ 0, 2048, 4096, 6144, 8192, 10240, 12288, 14336, 16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720 }`, where the quantized value is the table entry closest to `gain` but only among entries less than or equal to `gp_limit`. The function should return the index of the selected table entry, update `gain` to the quantized value (with special handling for MR122: mask the two least significant bits, i.e., bitwise AND with 0xFFFC), and if mode is MR795, also fill two output arrays `gain_cand[3]` and `gain_cind[3]` with the three candidate gain values and their indices (as described below). For MR795, the three candidates are the selected index and its two immediate neighbors, except at the boundaries: if the selected index is 0, use indices 0,1,2; if it is 15 or if the index+1 entry exceeds `gp_limit`, use indices (index-2), (index-1), index; otherwise use (index-1), index, (index+1). For all other modes, the candidate arrays should be left untouched. The function signature should be: `int quantizePitchGain(int mode, int gp_limit, int& gain, int gain_cand[3], int gain_cind[3])`.

// The core algorithm is a linear search over the fixed 16-entry quantization table. For each entry from index 1 to 15, check if the table value is ≤ `gp_limit`; if so, compute the absolute difference with the input `gain` and track the index with the smallest error. The search starts with index 0 (and its corresponding error) regardless of whether it satisfies the limit, so the default candidate is always index 0. This is the main edge case: if no other entry meets the limit, index 0 is returned. After finding the best index, the behavior branches on mode. For MR122, the quantized gain is the table value with the two least significant bits cleared (bitwise AND 0xFFFC). For MR795, we determine the starting index for three candidate indices as per the boundary rules: if index is 0, start at 0; else if index is 15 or the entry at index+1 exceeds `gp_limit`, start at index-2; else start at index-1. Then we fill the candidate arrays for i=0..2 with the index and corresponding table value, incrementing the index each time (capped by the array bounds, though in practice the start values ensure indices stay within 0..15). The time complexity is O(16) for the search and O(1) for the candidate generation, so overall O(1) since the table size is constant. Space usage is O(1) beyond the fixed table and output arrays.

#include <cstddef>

// Fixed quantization table for pitch gain (Q14 format)
static const int kQuantTable[16] = {
    0, 2048, 4096, 6144, 8192, 10240, 12288, 14336,
    16384, 18432, 20480, 22528, 24576, 26624, 28672, 30720
};

// Quantize pitch gain against the fixed table with a limit.
// mode: 2=MR795, 3=MR122, other=generic
// gp_limit: maximum allowed quantized value
// gain: input value to quantize (Q14); updated to quantized value
// gain_cand: output array of 3 candidate gains (MR795 only)
// gain_cind: output array of 3 candidate indices (MR795 only)
// Returns: index of selected quantization entry
int quantizePitchGain(int mode, int gp_limit, int& gain,
                      int gain_cand[3], int gain_cind[3]) {
    const int kNumEntries = 16;

    int index = 0;
    int err_min = gain - kQuantTable[0];
    if (err_min < 0) err_min = -err_min;

    for (int i = 1; i < kNumEntries; ++i) {
        if (kQuantTable[i] <= gp_limit) {
            int err = gain - kQuantTable[i];
            if (err < 0) err = -err;
            if (err < err_min) {
                err_min = err;
                index = i;
            }
        }
    }

    if (mode == 2) { // MR795
        int start;
        if (index == 0) {
            start = 0;
        } else if (index == (kNumEntries - 1) ||
                   kQuantTable[index + 1] > gp_limit) {
            start = index - 2;
        } else {
            start = index - 1;
        }

        for (int i = 0; i < 3; ++i) {
            gain_cind[i] = start;
            gain_cand[i] = kQuantTable[start];
            ++start;
        }

        gain = kQuantTable[index];
    } else if (mode == 3) { // MR122
        gain = kQuantTable[index] & 0xFFFC;
    } else {
        gain = kQuantTable[index];
    }

    return index;
}

#include <cassert>

int main() {
    int gain_cand[3];
    int gain_cind[3];

    // Basic search with limit: only entries <= 10000 are considered
    int gain = 9000;
    int idx = quantizePitchGain(0, 10000, gain, gain_cand, gain_cind);
    assert(idx == 4); // table[4]=8192, table[5]=10240 exceeds limit
    assert(gain == 8192);

    // No limit (large gp_limit): full table available
    gain = 9000;
    idx = quantizePitchGain(0, 100000, gain, gain_cand, gain_cind);
    assert(idx == 4); // 8192 is closest to 9000
    assert(gain == 8192);

    // Limit below the first entry: index 0 still selected
    gain = 100;
    idx = quantizePitchGain(0, 0, gain, gain_cand, gain_cind);
    assert(idx == 0);
    assert(gain == 0);

    // MR122: clear two LSBs (mask 0xFFFC)
    gain = 9000;
    idx = quantizePitchGain(3, 100000, gain, gain_cand, gain_cind);
    assert(idx == 4);
    assert(gain == (8192 & 0xFFFC)); // 8192 & 0xFFFC = 8192
    // Another case: index 5 has value 10240, 10240 & 0xFFFC = 10240
    gain = 10000;
    idx = quantizePitchGain(3, 100000, gain, gain_cand, gain_cind);
    assert(idx == 5);
    assert(gain == (10240 & 0xFFFC));

    // MR795: normal case (not at boundary), candidates around index
    gain = 9000;
    idx = quantizePitchGain(2, 100000, gain, gain_cand, gain_cind);
    assert(idx == 4);
    assert(gain == 8192);
    assert(gain_cind[0] == 3 && gain_cand[0] == 6144);
    assert(gain_cind[1] == 4 && gain_cand[1] == 8192);
    assert(gain_cind[2] == 5 && gain_cand[2] == 10240);

    // MR795: index 0 boundary
    gain = 100;
    idx = quantizePitchGain(2, 1000, gain, gain_cand, gain_cind);
    assert(idx == 0);
    assert(gain == 0);
    assert(gain_cind[0] == 0 && gain_cand[0] == 0);
    assert(gain_cind[1] == 1 && gain_cand[1] == 2048);
    assert(gain_cind[2] == 2 && gain_cand[2] == 4096);

    // MR795: index at end (15) with a limit that still includes index 15
    gain = 30000;
    idx = quantizePitchGain(2, 40000, gain, gain_cand, gain_cind);
    assert(idx == 15);
    assert(gain_cind[0] == 13 && gain_cand[0] == 26624);
    assert(gain_cind[1] == 14 && gain_cand[1] == 28672);
    assert(gain_cind[2] == 15 && gain_cand[2] == 30720);

    // MR795: index = 15 but next index would exceed limit (uses index-2)
    gain = 30000;
    idx = quantizePitchGain(2, 30720, gain, gain_cand, gain_cind);
    // index 15 selected because table[15]=30720 <= limit
    // index+1 is out of bounds, so start = 13
    assert(idx == 15);
    assert(gain_cind[0] == 13);
    assert(gain_cand[2] == 30720);

    return 0;
}
