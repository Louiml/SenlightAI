/*
Write a standalone C++ function named `processAudioBlock` that implements a fixed-length circular delay line with a single delay time (in milliseconds) and optionally applies feedback. The function must process an entire block of audio samples in place: for each output sample, read the delayed sample from the circular buffer, write the current input sample (plus feedback * delayed sample, if feedback is enabled) into the buffer, and replace the input sample with the delayed sample. The function signature must be: `void processAudioBlock(float* samples, size_t numSamples, float delayMs, float feedback, float sampleRate, float* delayBuffer, size_t bufferSize, size_t& writeIndex)`. The delay time must be converted to whole samples (truncating toward zero), and if that number is less than 1, treat it as 1 sample delay. The circular buffer size is given and must be at least as large as the maximum possible delay samples; you may assume the caller guarantees that. The function should update `writeIndex` after processing the block so that the next call continues seamlessly.
*/
#include <cstddef>
#include <cmath>

/**
 * Processes a block of audio samples through a delay line with optional feedback.
 * The delay buffer is circular and owned by the caller. The write index is updated
 * in place so that consecutive calls process a continuous stream.
 *
 * @param samples      Input/output audio block. On return, contains the delayed signal.
 * @param numSamples   Number of samples in the block.
 * @param delayMs      Delay time in milliseconds (must be >= 0).
 * @param feedback     Feedback coefficient (typically 0.0 to <1.0). 0.0 disables feedback.
 * @param sampleRate   Sample rate in Hz.
 * @param delayBuffer  Preallocated circular buffer of size bufferSize.
 * @param bufferSize   Size of delayBuffer. Must be at least 2.
 * @param writeIndex   Current write index (0-based). Updated after processing.
 */
void processAudioBlock(float* samples, size_t numSamples,
                       float delayMs, float feedback,
                       float sampleRate,
                       float* delayBuffer, size_t bufferSize,
                       size_t& writeIndex) {
    // Convert delay from milliseconds to whole samples, clamp to at least 1.
    int delaySamples = static_cast<int>(delayMs * sampleRate / 1000.0f);
    if (delaySamples < 1) delaySamples = 1;

    // If bufferSize is 1, delay becomes 0 but we still need to store the delayed sample.
    // However, we assume bufferSize >= 2 per the spec, so we just proceed.

    for (size_t i = 0; i < numSamples; ++i) {
        // Read the delayed sample (the value at the current write index).
        float delayed = delayBuffer[writeIndex];

        // Write the new sample into the buffer, with feedback if desired.
        delayBuffer[writeIndex] = samples[i] + feedback * delayed;

        // Replace the input sample with the delayed one.
        samples[i] = delayed;

        // Advance write index circularly.
        writeIndex = (writeIndex + 1) % bufferSize;
    }
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple delay with no feedback, delay of 2 samples at 10 Hz -> 0.2s = 200ms
    {
        const float sr = 10.0f;
        const size_t bufSize = 8;
        std::vector<float> delay(bufSize, 0.0f);
        size_t idx = 0;
        std::vector<float> input = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        processAudioBlock(input.data(), input.size(), 200.0f, 0.0f, sr, delay.data(), bufSize, idx);
        // After first call: first two outputs are zero (empty delay), then samples appear shifted by 2.
        assert(input[0] == 0.0f);
        assert(input[1] == 0.0f);
        assert(input[2] == 1.0f);
        assert(input[3] == 2.0f);
        assert(input[4] == 3.0f);
        // The writeIndex should be 5 (5 samples written) mod 8 = 5.
        assert(idx == 5);
    }

    // Test 2: Feedback creates a simple recursive delay. Use a small delay of 1 sample.
    {
        const float sr = 100.0f;
        const size_t bufSize = 4;
        std::vector<float> delay(bufSize, 0.0f);
        size_t idx = 0;
        // Delay 10 ms at 100 Hz = 1 sample.
        std::vector<float> input = {1.0f, 0.0f, 0.0f, 0.0f};
        processAudioBlock(input.data(), input.size(), 10.0f, 0.5f, sr, delay.data(), bufSize, idx);
        // For feedback 0.5 with one-sample delay:
        // Sample 0: delayed=0, write 1, out=0
        // Sample 1: delayed=1, write 0+0.5*1=0.5, out=1
        // Sample 2: delayed=0.5, write 0+0.5*0.5=0.25, out=0.5
        // Sample 3: delayed=0.25, write 0+0.5*0.25=0.125, out=0.25
        assert(input[0] == 0.0f);
        assert(input[1] == 1.0f);
        assert(input[2] == 0.5f);
        assert(input[3] == 0.25f);
        assert(idx == 4 % 4); // should be 0
    }

    // Test 3: Zero milliseconds delay clamps to 1 sample.
    {
        const float sr = 10.0f;
        const size_t bufSize = 4;
        std::vector<float> delay(bufSize, 0.0f);
        size_t idx = 0;
        std::vector<float> input = {5.0f, 6.0f, 7.0f};
        processAudioBlock(input.data(), input.size(), 0.0f, 0.0f, sr, delay.data(), bufSize, idx);
        // Since clamp to 1 sample: output[0]=0, output[1]=5, output[2]=6
        assert(input[0] == 0.0f);
        assert(input[1] == 5.0f);
        assert(input[2] == 6.0f);
        assert(idx == 3);
    }

    // Test 4: Delay larger than bufferSize (should still wrap correctly).
    // With bufferSize=4 and delay of 3 samples (30ms at 100Hz).
    {
        const float sr = 100.0f;
        const size_t bufSize = 4;
        std::vector<float> delay(bufSize, 0.0f);
        size_t idx = 0;
        std::vector<float> input = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        processAudioBlock(input.data(), input.size(), 30.0f, 0.0f, sr, delay.data(), bufSize, idx);
        // After 5 samples with delay=3: output[0]=0, [1]=0, [2]=0, [3]=1, [4]=2
        assert(input[0] == 0.0f);
        assert(input[1] == 0.0f);
        assert(input[2] == 0.0f);
        assert(input[3] == 1.0f);
        assert(input[4] == 2.0f);
        assert(idx == 5 % 4); // 1
    }

    // Test 5: Continuity across multiple calls.
    {
        const float sr = 10.0f;
        const size_t bufSize = 4;
        std::vector<float> delay(bufSize, 0.0f);
        size_t idx = 0;
        // Delay 200ms = 2 samples.
        std::vector<float> in1 = {1.0f, 2.0f};
        std::vector<float> in2 = {3.0f, 4.0f};
        processAudioBlock(in1.data(), in1.size(), 200.0f, 0.0f, sr, delay.data(), bufSize, idx);
        processAudioBlock(in2.data(), in2.size(), 200.0f, 0.0f, sr, delay.data(), bufSize, idx);
        // First block: [0,0]. Second block: [1,2] (since delay=2)
        assert(in1[0] == 0.0f);
        assert(in1[1] == 0.0f);
        assert(in2[0] == 1.0f);
        assert(in2[1] == 2.0f);
        assert(idx == 4 % 4); // 0
    }

    return 0;
}
// The core algorithm is a circular buffer implementing a delay line. For each sample in the input block, we first read the output at the current write index (this is the sample from `delayTime` samples ago, because we always read before writing). Then we write into that same slot the current input sample plus `feedback * output` (if feedback is non-zero). Finally we replace the input sample with the output. The write index advances modulo `bufferSize`. Edge cases: if the delay in samples is zero (since we use integer truncation, e.g., delayMs=0.4 at 44.1kHz gives 17 samples, not zero; but if user passes delayMs=0 exactly, we must clamp to 1), the delay becomes 1 sample. Also, if `bufferSize` is 0 or 1, we must handle gracefully: with bufferSize 0 (should not happen per spec) we could just pass through, but we assume bufferSize>=2. The complexity is O(numSamples) time and O(1) extra space beyond the buffer. The function is designed to be called repeatedly on consecutive blocks; the `writeIndex` must be passed by reference so it persists across calls.
