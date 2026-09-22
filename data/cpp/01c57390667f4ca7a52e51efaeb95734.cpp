// Implement a C++ function named `detectBpm` that analyzes a mono audio signal stored as a `std::vector<float>` (samples normalized to [-1, 1]) and returns the estimated beats per minute (BPM) as a `float`. The function must simulate a simplified version of the provided BPM detection algorithm: it should decimate the input by averaging blocks of samples to approximately 1000 Hz, compute a smoothed amplitude envelope with RMS-based thresholding, then calculate an autocorrelation over a lag range corresponding to BPMs between 60 and 180. The peak of the autocorrelation (after removing a bias equal to the minimum value) determines the BPM, computed as `(60 * originalSampleRate) / decimationFactor / peakLag`. If no valid peak is found (e.g., peak lag is zero or data too short), return 0.0f. Assume the input sample rate is 44100 Hz and that the input may have any length (including zero). The function must be const-correct and avoid external dependencies beyond standard headers.
// The core idea is to reduce computational cost by decimating the signal to about 1000 Hz. With an original sample rate of 44100 Hz, a decimation factor of 44 (44100 / 1000) is used; each output sample is the average of 44 consecutive input samples (mono already). After decimation, we compute an envelope: for each decimated sample, we maintain a running RMS volume accumulator with exponential decay (`avgdecay = 0.99986`), update it with the squared absolute value, and if the sample's absolute value is below half the current RMS threshold, set it to zero; otherwise, smooth the envelope with a decay constant (`decay = 0.7`). The resulting envelope samples are accumulated into a buffer. When the buffer has more than `windowLen` samples (where `windowLen = (60 * 44100 / 44) / 60` ≈ 1002 for 60 BPM, and `windowStart = (60 * 44100 / 44) / 180` ≈ 334), we compute an autocorrelation for lags from `windowStart` to `windowLen-1` over the oldest `processLength` samples. The autocorrelation sums products of the envelope buffer at positions `i` and `i + offs`. After processing all input, we remove bias (subtract the minimum autocorrelation value) and find the lag with the maximum autocorrelation. The BPM is `60 * (44100 / 44) / peakLag`. Edge cases: if input length is too short to produce enough envelope samples (fewer than `windowLen`), return 0. If no peak found (peak position ≤ 0), return 0. Time complexity: decimation O(n), envelope O(n/decimation), autocorrelation O(windowLen * processLength) per block, but using a single pass for all data, total O(n * windowLen) in the worst case; typical audio lengths make this manageable with decimation reducing the sample count. Space complexity: O(windowLen) for the autocorrelation and O(windowLen) for the envelope buffer.
#include <vector>
#include <cmath>
#include <algorithm>

// Simplified BPM detection for mono float audio (normalized to [-1, 1]).
// Returns estimated BPM or 0.0f if detection fails.
float detectBpm(const std::vector<float>& input) {
    const int originalSampleRate = 44100;
    const int decimationFactor = originalSampleRate / 1000; // = 44

    // Decimate to mono (already mono) by averaging blocks of decimationFactor samples.
    std::vector<float> decimated;
    decimated.reserve(input.size() / decimationFactor + 1);
    float sum = 0.0f;
    int count = 0;
    for (float sample : input) {
        sum += sample;
        count++;
        if (count == decimationFactor) {
            decimated.push_back(sum / decimationFactor);
            sum = 0.0f;
            count = 0;
        }
    }
    if (count > 0) {
        decimated.push_back(sum / count);
    }

    // Constants for envelope calculation.
    const float avgdecay = 0.99986f;
    const float avgnorm = (1.0f - avgdecay);
    const float decay = 0.7f;
    const float norm = (1.0f - decay);

    float RMSVolumeAccu = (0.045f * 0.045f) / avgnorm; // initial RMS level
    float envelopeAccu = 0.0f;

    std::vector<float> envelope;
    envelope.reserve(decimated.size());
    for (float sample : decimated) {
        RMSVolumeAccu *= avgdecay;
        float val = std::fabs(sample);
        RMSVolumeAccu += val * val;

        // Threshold: cut amplitudes below half of current RMS.
        if (val < 0.5f * std::sqrt(RMSVolumeAccu * avgnorm)) {
            val = 0.0f;
        }

        envelopeAccu *= decay;
        envelopeAccu += val;
        envelope.push_back(envelopeAccu * norm);
    }

    // Determine lag range for BPM 60-180.
    int windowLen = (60 * originalSampleRate) / (decimationFactor * 60); // ≈ 1002
    int windowStart = (60 * originalSampleRate) / (decimationFactor * 180); // ≈ 334
    windowLen = std::min(windowLen, static_cast<int>(envelope.size()));
    if (windowLen <= windowStart || windowLen < 1) {
        return 0.0f;
    }

    // Compute autocorrelation for lags in [windowStart, windowLen).
    std::vector<float> xcorr(windowLen, 0.0f);
    int processLength = static_cast<int>(envelope.size()) - windowLen;
    if (processLength <= 0) {
        processLength = 1;
    }

    for (int offs = windowStart; offs < windowLen; ++offs) {
        float sum = 0.0f;
        for (int i = 0; i < processLength && i + offs < static_cast<int>(envelope.size()); ++i) {
            sum += envelope[i] * envelope[i + offs];
        }
        xcorr[offs] = sum;
    }

    // Remove bias.
    float minval = *std::min_element(xcorr.begin() + windowStart, xcorr.end());
    for (int offs = windowStart; offs < windowLen; ++offs) {
        xcorr[offs] -= minval;
    }

    // Find peak lag.
    float maxval = -1e12f;
    int peakLag = -1;
    for (int offs = windowStart; offs < windowLen; ++offs) {
        if (xcorr[offs] > maxval) {
            maxval = xcorr[offs];
            peakLag = offs;
        }
    }

    if (peakLag <= 0 || maxval <= 0.0f) {
        return 0.0f;
    }

    float coeff = 60.0f * (static_cast<float>(originalSampleRate) / decimationFactor);
    return coeff / static_cast<float>(peakLag);
}
#include <cassert>
#include <vector>
#include <cmath>

// Function declaration (assumes the solution above is included).
float detectBpm(const std::vector<float>& input);

int main() {
    // Empty input returns 0.
    assert(detectBpm({}) == 0.0f);

    // Very short input (less than window) returns 0.
    std::vector<float> shortSignal(100, 0.5f);
    assert(detectBpm(shortSignal) == 0.0f);

    // Constant signal: envelope becomes constant, autocorrelation has peak at lag 0,
    // but windowStart > 0, so no peak found; should return 0.
    std::vector<float> constantSignal(5000, 0.1f);
    assert(detectBpm(constantSignal) == 0.0f);

    // Simulate a 120 BPM beat: a periodic pulse every 0.5 seconds (at 44100 Hz).
    // After decimation by 44, period is (44100/44)/2 ≈ 501 samples.
    // Generate a signal with impulses at those intervals.
    int sampleRate = 44100;
    float periodSamples = sampleRate / 2.0f; // 0.5 seconds for 120 BPM
    std::vector<float> beatSignal;
    beatSignal.reserve(sampleRate * 2); // 2 seconds
    for (int i = 0; i < sampleRate * 2; ++i) {
        float val = 0.0f;
        float pos = i / periodSamples;
        if (std::abs(pos - std::round(pos)) < 0.01f) {
            val = 1.0f;
        }
        beatSignal.push_back(val);
    }
    float bpm = detectBpm(beatSignal);
    assert(bpm > 110.0f && bpm < 130.0f);

    // Another test: 100 BPM, period = 0.6 seconds.
    periodSamples = sampleRate * 0.6f;
    std::vector<float> beatSignal2;
    beatSignal2.reserve(sampleRate * 3); // 3 seconds
    for (int i = 0; i < sampleRate * 3; ++i) {
        float val = 0.0f;
        float pos = i / periodSamples;
        if (std::abs(pos - std::round(pos)) < 0.01f) {
            val = 1.0f;
        }
        beatSignal2.push_back(val);
    }
    bpm = detectBpm(beatSignal2);
    assert(bpm > 90.0f && bpm < 110.0f);

    return 0;
}
