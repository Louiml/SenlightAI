// Write a standalone C++ function named `polyphaseSynthesisStep` that simulates the core structure of the provided MP3 polyphase synthesis filter bank for a simpler, educational purpose. The function should accept a 32-element array of subband samples (integers) representing the frequency-domain input for one channel, and a 576-element circular buffer of integers representing the decoder state. It must perform three operations in sequence: (1) apply a simple symmetric equalizer that halves the magnitude of every sample at odd indices (to mimic the equalizer effect), (2) apply a simplified "DCT-like" transform that reverses the second half of the array and adds it to the first half (this mimics the split/merge operations in the snippet), and (3) produce 32 integer output PCM samples by convolving the transformed data with a fixed 32-tap filter window (use the coefficients `window[i] = i + 1` for `i` from 0 to 31, but only use the first 32 values of the buffer after shifting). Specifically, after the transform, the function should write to an output array of 32 integers where `out[j] = sum_{k=0..31} transformed[k] * window[(j + k) % 32]` (modular convolution). Finally, the function should update the circular buffer by moving the last 480 elements to the front and zeroing the remaining 96 elements, similar to the `pv_memmove` and clearing in the snippet. The function signature: `void polyphaseSynthesisStep(const int32_t subbandSamples[32], int32_t circBuffer[576], int32_t outPcm[32])`. The function must not use any external libraries beyond standard headers, and must be self-contained. The input subband samples can be any integers (including negative), and the circular buffer is assumed to contain valid initial values (for simplicity, you may initialize it to zeros in the test).

#include <cassert>
#include <cstdint>
#include <iostream>

// Declaration of the function under test
void polyphaseSynthesisStep(const int32_t subbandSamples[32], int32_t circBuffer[576], int32_t outPcm[32]);

int main() {
    // Test 1: All zeros input, buffer all zeros -> output all zeros, buffer unchanged (unchanged zeroed)
    {
        int32_t sub[32] = {0};
        int32_t buf[576] = {0};
        int32_t out[32];
        polyphaseSynthesisStep(sub, buf, out);
        for (int i = 0; i < 32; ++i) assert(out[i] == 0);
        for (int i = 0; i < 576; ++i) assert(buf[i] == 0);
    }

    // Test 2: Single non-zero subband sample (only index 0 = 2, others 0)
    {
        int32_t sub[32] = {0};
        sub[0] = 2;
        int32_t buf[576] = {0};
        int32_t out[32];
        polyphaseSynthesisStep(sub, buf, out);
        // For only sub[0]=2, equalized stays 2, transform temp[0]=2, temp[16]=0, others 0, merge: transformed[0]=2, transformed[16]=2, others 0.
        // Convolution with window k+1: out[j] = transformed[0]*1 + transformed[16]*17 (when j+16 %32 =16)
        // Need to check carefully: For any j, idx = (j+0)%32 = j and (j+16)%32 = (j+16)%32.
        // So out[j] = 2*(1) if j==0, 2*(17) if (j+16)%32==0 => j==16? Actually (j+16)%32==0 => j=16 (since j in 0..31, j=16 works). Also j=0 gives idx=0 -> window[0]=1. So out[0]=2*1=2, out[16]=2*17=34? Let's verify: For j=16, idx k=0 -> (16+0)%32=16 -> transformed[16]=2, window[0]=1 -> 2; k=16 -> (16+16)%32=0 -> transformed[0]=2, window[16]=17 -> 34. So out[16]=2+34=36? Wait, the sum includes both k=0 and k=16 contributions. Let's compute manually:
        // Convolution sum over k of transformed[(j+k)%32] * (k+1). For j=16, k=0: idx=16 -> 2*1=2. k=16: idx=32%32=0 -> 2*17=34. Other k give 0. Sum=36. Similarly for j=0: k=0 idx=0 ->2*1=2, k=16 idx=16 ->2*17=34, sum=36. Actually both j=0 and j=16 have same sum? For j=0: k=0 idx=0 ->2*1=2, k=16 idx=16 ->2*17=34 =>36. For j=1: k=15? No, only k=0 gives idx=1 ->0, k=16 gives idx=17 ->0, so 0. So out[0]=36, out[16]=36, others 0.
        int32_t expected[32] = {0};
        expected[0] = 36;
        expected[16] = 36;
        for (int i = 0; i < 32; ++i) assert(out[i] == expected[i]);
        // Buffer update: all zeros, still zeros.
        for (int i = 0; i < 576; ++i) assert(buf[i] == 0);
    }

    // Test 3: Check buffer update with non-zero initial buffer
    {
        // Fill buffer with values 0..575
        int32_t sub[32] = {0};
        int32_t buf[576];
        for (int i = 0; i < 576; ++i) buf[i] = i;
        int32_t out[32];
        polyphaseSynthesisStep(sub, buf, out);
        // After update, first 480 positions should be old[96..575], i.e., values 96..575
        for (int i = 0; i < 480; ++i) assert(buf[i] == 96 + i);
        // Positions 480..575 should be zero
        for (int i = 480; i < 576; ++i) assert(buf[i] == 0);
        // Output should be all zeros because sub is all zeros
        for (int i = 0; i < 32; ++i) assert(out[i] == 0);
    }

    // Test 4: Negative odd index equalization
    {
        int32_t sub[32] = {0};
        sub[1] = -5;  // odd index, halve to -2 (truncate toward zero)
        sub[2] = 10;  // even, unchanged
        int32_t buf[576] = {0};
        int32_t out[32];
        polyphaseSynthesisStep(sub, buf, out);
        // After equalizer: equalized[1] = -2, equalized[2] = 10, others 0.
        // Transform: temp[0..15] = equalized[0..15]; temp[16..31] = equalized[31..16] reversed.
        // So temp[1] = -2, temp[2] = 10, temp[16] = equalized[31]=0, temp[31] = equalized[16]=0, etc.
        // Merge: transformed[i] = temp[i] + temp[(i+16)%32].
        // For i=1: temp[1] + temp[17] = -2 + 0 = -2.
        // For i=2: temp[2] + temp[18] = 10 + 0 = 10.
        // For i=17: temp[17] + temp[1] = 0 + (-2) = -2.
        // For i=18: temp[18] + temp[2] = 0 + 10 = 10.
        // All others 0.
        // Convolution: Only k where transformed index is 1,2,17,18 contribute.
        // For each output j, sum over all k in 0..31 of transformed[(j+k)%32]*(k+1).
        // We can compute expected manually but simpler: check that output is symmetric? Instead just check some specific values.
        // Let's compute out[1] (j=1): For transformed nonzero at idx=1,2,17,18.
        // For idx=1: need (1+k)%32=1 => k=0 -> window[0]=1 -> -2*1=-2
        // For idx=2: need (1+k)%32=2 => k=1 -> window[1]=2 -> 10*2=20
        // For idx=17: need (1+k)%32=17 => k=16 -> window[16]=17 -> -2*17=-34
        // For idx=18: need (1+k)%32=18 => k=17 -> window[17]=18 -> 10*18=180
        // Sum = -2+20-34+180 = 164. So out[1] should be 164.
        assert(out[1] == 164);
        // Similarly, out[2] (j=2): idx=1? (2+31)%32=1? Actually need (2+k)%32=1 => k=31 -> window[31]=32 -> -2*32=-64
        // idx=2: k=0 -> 10*1=10
        // idx=17: k=15 -> window[15]=16 -> -2*16=-32
        // idx=18: k=16 -> window[16]=17 -> 10*17=170
        // Sum = -64+10-32+170 = 84
        assert(out[2] == 84);
        // Buffer all zeros
        for (int i = 0; i < 576; ++i) assert(buf[i] == 0);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <cstdint>
#include <cstring>

/**
 * @brief Performs a simplified polyphase synthesis step mimicking an MP3 decoder.
 * 
 * Applies equalization, a simulated DCT transform, a polyphase filter convolution,
 * and updates the circular buffer state.
 * 
 * @param subbandSamples[32] Input frequency-domain subband samples (modified in-place conceptually).
 * @param circBuffer[576] Circular buffer of decoder state (updated in-place).
 * @param outPcm[32] Output 32 PCM samples.
 */
void polyphaseSynthesisStep(const int32_t subbandSamples[32], int32_t circBuffer[576], int32_t outPcm[32]) {
    // Step 1: Equalizer - halve odd-indexed samples (truncating toward zero)
    int32_t equalized[32];
    for (int i = 0; i < 32; ++i) {
        if (i % 2 == 1) {
            equalized[i] = subbandSamples[i] / 2;
        } else {
            equalized[i] = subbandSamples[i];
        }
    }

    // Step 2: Simplified DCT split/merge
    int32_t temp[32];
    // Split: first half as-is, second half reversed
    for (int i = 0; i < 16; ++i) {
        temp[i] = equalized[i];
        temp[16 + i] = equalized[31 - i];
    }
    // Merge with cross-coupling
    int32_t transformed[32];
    for (int i = 0; i < 32; ++i) {
        transformed[i] = temp[i] + temp[(i + 16) % 32];
    }

    // Step 3: Polyphase filter window convolution
    // Window coefficients: window[k] = k + 1 for k = 0..31
    for (int j = 0; j < 32; ++j) {
        int32_t sum = 0;
        for (int k = 0; k < 32; ++k) {
            int idx = (j + k) % 32;
            sum += transformed[idx] * (k + 1);
        }
        outPcm[j] = sum;
    }

    // Step 4: Update circular buffer (move last 480 elements to front, zero the rest)
    // The snippet moves circ_buffer[576..1055] to circ_buffer[0..479]? Wait: original code:
    // pv_memmove(&pChVars->circ_buffer[576], pChVars->circ_buffer, 480*sizeof(*...));
    // In original, circ_buffer size is larger (not given), but here fixed 576.
    // We interpret: move elements [96..575] (480 elements) to positions [0..479], fill [480..575] with 0.
    int32_t tempBuffer[576];
    std::memcpy(tempBuffer, circBuffer, sizeof(circBuffer[0]) * 576);
    // Copy last 480 elements (indices 96..575) to front
    for (int i = 0; i < 480; ++i) {
        circBuffer[i] = tempBuffer[96 + i];
    }
    // Zero the remaining 96 elements
    for (int i = 480; i < 576; ++i) {
        circBuffer[i] = 0;
    }
}

// The solution models the given MP3 decoder snippet but simplifies it for clarity. The main algorithm proceeds in three phases:  
// 1. **Equalizer**: Iterate through the 32 subband samples, and for each odd index, divide the value by 2 (integer division, truncating toward zero). For even indices, keep the value unchanged. This mirrors the `pvmp3_equalizer` call but with a trivial filter.  
// 2. **Transform (simulated DCT split/merge)**: The snippet performs `pvmp3_split`, two 16-point DCTs, and `pvmp3_merge_in_place_N32`. We simplify this to a single reversible-ish operation: create a temporary array `temp` of size 32. Set `temp[i] = subbandSamples[i]` for `i` 0..15, and `temp[i] = subbandSamples[47 - i]` for `i` 16..31 (this reverses the second half and places it after the first half). Then, for `i` 0..31, set `transformed[i] = temp[i] + temp[(i + 16) % 32]` (a crossover sum). This retains the spirit of combining even/odd terms and is easy to implement.  
// 3. **Polyphase filter window**: For each output sample `j` from 0 to 31, compute a convolution using a fixed window `window[k] = k + 1` for `k` 0..31. The output is `out[j] = sum_{k=0}^{31} transformed[(j + k) % 32] * window[k]`. This produces 32 integer outputs.  
// 4. **Buffer update**: Copy the last 480 elements of `circBuffer` (indices 96..575) to the front (indices 0..479), then set indices 480..575 to zero. This simulates the `pv_memmove` and clearing for the next iteration, ensuring the buffer maintains its size.  
// Edge cases: The input subband samples may contain zeros, negatives, or repeated values; integer division for the equalizer should truncate toward zero (C++ default for `int32_t`). The convolution uses modular indexing to wrap around, which is safe for any input values. The buffer update assumes the buffer is exactly 576 elements; no size checking is needed. Time complexity is O(32) for equalizer, O(32) for transform, O(32*32) = O(1024) for convolution (constant), and O(576) for buffer update. Since all sizes are fixed constants, the overall complexity is O(1) with a small constant. Space complexity is O(32) for temporary arrays, plus O(1) for the fixed window.
