Write a C++ function `float adaptiveCompressor(float input, float sampleRate, float &state, float &gain, float kneeThreshold, float alphaAttack, float alphaRelease, float slope)` that implements a single-sample dynamic range compressor using a decoupled attack/release envelope follower in the log domain. The function must accept a mono audio sample, update the internal state (envelope follower value in dB) and the adaptive gain multiplier, apply the gain to the input sample, and hard-clip the result to ±32767.0. The compressor uses a hard knee: if the absolute input level (in dB, using natural log of max(abs(x), 1e-6)) is above the knee threshold, the amount of overshoot is rectified, multiplied by `slope` (which is negative for compression), and smoothed with attack/release smoothing coefficients. The gain is updated multiplicatively using `exp(state - prevState)`. The function must return the processed sample. All parameters except `state` and `gain` are passed by value; `state` and `gain` are passed by reference so they persist across calls. The input may be positive, negative, or near zero; you must avoid taking the log of zero by using the provided minimum absolute value constant.

#include <cassert>
#include <cmath>

// Declaration of the function under test.
float adaptiveCompressor(float input, float sampleRate,
                         float &state, float &gain,
                         float kneeThreshold, float alphaAttack,
                         float alphaRelease, float slope);

int main() {
    // Test 1: Near-zero input should not produce NaN and should stay near zero.
    {
        float state = 0.0f;
        float gain = 1.0f;
        float out = adaptiveCompressor(0.0f, 48000.0f, state, gain,
                                       -8.0f, 0.9f, 0.5f, -0.857142857f);
        assert(out == 0.0f);
    }

    // Test 2: Constant large input should be reduced (compressed) below clip limit.
    {
        float state = 0.0f;
        float gain = 1.0f;
        float out = adaptiveCompressor(30000.0f, 48000.0f, state, gain,
                                       -8.0f, 0.001f, 0.015f, -0.857142857f);
        // After a single sample, the gain should be less than 1 (compression).
        assert(gain < 1.0f);
        assert(out < 30000.0f);
        assert(out >= -32767.0f && out <= 32767.0f);
    }

    // Test 3: Very small negative input near zero is handled without log(0).
    {
        float state = 0.0f;
        float gain = 1.0f;
        float out = adaptiveCompressor(-0.0000005f, 44100.0f, state, gain,
                                       -8.0f, 0.9f, 0.5f, -0.857142857f);
        assert(out >= -32767.0f && out <= 32767.0f);
        assert(std::isfinite(out));
    }

    // Test 4: Attack vs release asymmetry: rising envelope uses release coefficient.
    {
        float state = 0.0f;
        float gain = 1.0f;
        // Force a large cv (overshoot > 0) to trigger release branch (cv > state).
        // First call with huge input increases state.
        adaptiveCompressor(1000.0f, 48000.0f, state, gain,
                           -8.0f, 0.1f, 0.9f, -1.0f);
        float stateAfterFirst = state;
        assert(stateAfterFirst > 0.0f);
        // Now a lower input (still above threshold) should decrease state via attack branch.
        float prevState = state;
        adaptiveCompressor(500.0f, 48000.0f, state, gain,
                           -8.0f, 0.1f, 0.9f, -1.0f);
        // Since cv for 500 is smaller than previous state (likely), attack branch used.
        assert(state < prevState);
    }

    // Test 5: Hard clipping at upper limit.
    {
        float state = 0.0f;
        float gain = 100000.0f; // Huge gain to force clipping.
        float out = adaptiveCompressor(1.0f, 48000.0f, state, gain,
                                       -8.0f, 0.9f, 0.5f, -1.0f);
        assert(out == 32767.0f);
    }

    // Test 6: Hard clipping at lower limit.
    {
        float state = 0.0f;
        float gain = 100000.0f;
        float out = adaptiveCompressor(-1.0f, 48000.0f, state, gain,
                                       -8.0f, 0.9f, 0.5f, -1.0f);
        assert(out == -32767.0f);
    }

    // Test 7: No compression when input is below threshold (overshoot=0).
    {
        float state = 0.0f;
        float gain = 1.0f;
        // Input amplitude exp(-20) ~ 2e-9, well below threshold -8.
        float out = adaptiveCompressor(0.000001f, 48000.0f, state, gain,
                                       -8.0f, 0.9f, 0.5f, -1.0f);
        // Since overshoot <=0, cv = 0, state stays 0, gain stays 1.
        assert(gain == 1.0f);
        assert(out == 0.000001f);
    }

    return 0;
}

#include <cmath>
#include <algorithm>

// Single-sample adaptive dynamic range compressor.
// Parameters:
//   input          - the audio sample (can be any float)
//   sampleRate     - sampling rate in Hz (not directly used but kept for signature consistency)
//   state          - envelope follower state in natural-log dB (persisted across calls)
//   gain           - adaptive gain multiplier (persisted across calls)
//   kneeThreshold  - threshold in natural-log dB (e.g., -8.0)
//   alphaAttack    - smoothing coefficient for attack (0..1)
//   alphaRelease   - smoothing coefficient for release (0..1)
//   slope          - compression slope (e.g., 1/ratio - 1, negative for compression)
// Returns the processed sample, hard-clipped to [-32767.0, 32767.0].
float adaptiveCompressor(float input, float sampleRate,
                         float &state, float &gain,
                         float kneeThreshold, float alphaAttack,
                         float alphaRelease, float slope) {
    (void)sampleRate; // Not used in per-sample processing; kept for interface clarity.
    const float kMinAbs = 0.000001f;
    const float kFixedPointLimit = 32767.0f;

    // Log-domain amplitude (natural log of absolute value).
    const float maxAbs = std::max(std::fabs(input), kMinAbs);
    const float levelDB = std::log(maxAbs);

    // Overshoot above threshold, half-wave rectified.
    const float overshoot = levelDB - kneeThreshold;
    const float rect = std::max(overshoot, 0.0f);
    const float cv = rect * slope;

    // Smooth the control voltage with attack/release filtering.
    const float prevState = state;
    if (cv <= state) {
        state = alphaAttack * state + (1.0f - alphaAttack) * cv;
    } else {
        state = alphaRelease * state + (1.0f - alphaRelease) * cv;
    }

    // Update gain multiplicatively.
    gain *= std::exp(state - prevState);

    // Apply gain and clip.
    float output = input * gain;
    if (output > kFixedPointLimit) {
        output = kFixedPointLimit;
    } else if (output < -kFixedPointLimit) {
        output = -kFixedPointLimit;
    }
    return output;
}

// The algorithm processes one sample as follows:
// 1. Compute `maxAbs = max(abs(input), 1e-6f)` to avoid log(0).
// 2. Convert to natural-log dB: `levelDB = log(maxAbs)` (this is a natural log scale, not base-10).
// 3. Compute overshoot = `levelDB - kneeThreshold`.
// 4. Apply half-wave rectification: `rect = max(overshoot, 0.0f)`.
// 5. Compute control voltage `cv = rect * slope`.
// 6. Smooth with attack/release: if `cv <= state`, use attack coefficient (falling envelope); else use release coefficient (rising envelope): `state = alpha * state + (1-alpha)*cv`.
// 7. Compute gain multiplier as `exp(state - prevState)` and multiply the existing gain.
// 8. Apply gain to the input sample, then hard clip to ±32767.0.
// Edge cases: very small inputs near zero are clamped for log calculation; large inputs may exceed the clip limit and are saturated; state and gain must persist across calls via reference parameters. Time complexity is O(1) per sample, space complexity O(1).
