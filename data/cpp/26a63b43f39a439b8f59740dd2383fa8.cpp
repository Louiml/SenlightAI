// Write a C++ function `applyBs2bCrossfeed` that takes a vector of floats representing interleaved stereo audio samples (left, right, left, right, ...), a sample rate, and a crossfeed level (an integer where 0=LowCLevel, 1=MiddleCLevel, 2=HighCLevel, 3=LowECLevel, 4=MiddleECLevel, 5=HighECLevel), and returns a new vector of floats of the same length and layout containing the processed audio. The function must implement the same crossfeed algorithm as shown in the snippet: for each sample pair, two IIR filters (a low-shelf and a high-shelf) are applied to each channel independently, and the filtered outputs of the left and right channels are summed with each other's filtered outputs (crossfeed). Specifically, for the left channel: `y0 = a0hi*x + z_hi; z_hi = a1hi*x + b1hi*y0; y1 = a0lo*x + z_lo; z_lo = b1lo*y1;` and store both `y0` and `y1`. For the right channel, apply the same but swap filter roles: `y0 = a0lo*x + z_lo; z_lo = b1lo*y0; y1 = a0hi*x + z_hi; z_hi = a1hi*x + b1hi*y1;` and then for each sample pair, the output left sample is `left_y0 + right_y1` and output right sample is `right_y0 + left_y1`. The filter coefficients depend on level and sample rate as described in the `init` function: use the provided constants and formulas, where `g = 1/(1 - G_hi + G_lo)`, `b1_lo = exp(-2*pi*Fc_lo/srate)`, `a0_lo = G_lo*(1 - b1_lo)*g`, `b1_hi = exp(-2*pi*Fc_hi/srate)`, `a0_hi = (1 - G_hi*(1 - b1_hi))*g`, `a1_hi = -b1_hi*g`. Preserve internal filter state (two sets of two floats: `z_lo` and `z_hi` for left and right channels) across calls if the function is called multiple times with the same level and sample rate; but for simplicity, you may require each call to start with zero history. Throw an exception if sample rate is less than 1 or if level is outside 0-5. The function must handle an empty vector gracefully (return empty). The input vector length must be even, otherwise throw `std::invalid_argument`.
#include <cassert>
#include <vector>
#include <cmath>
#include <stdexcept>

// Forward declaration of the solution function (already included above in real usage)
std::vector<float> applyBs2bCrossfeed(const std::vector<float>& input, int srate, int level);

int main() {
    // Empty input
    assert(applyBs2bCrossfeed({}, 44100, 5).empty());

    // Single sample pair at a high sample rate where coefficients approach their limits
    std::vector<float> in = {1.0f, 0.0f};
    auto out = applyBs2bCrossfeed(in, 192000, 5);
    // At very high sample rate, b1_lo≈1, b1_hi≈1, a0_lo≈0, a0_hi≈g, a1_hi≈-g
    // Then left y0 ≈ g*1, left y1 ≈ 0; right y0 ≈ 0, right y1 ≈ g*0=0
    // Output L ≈ g, R ≈ 0
    float expected_g = 1.0f / (1.0f - 0.205671765275719f + 0.398107170553497f);
    assert(std::abs(out[0] - expected_g) < 1e-4f);
    assert(std::abs(out[1]) < 1e-6f);

    // Test odd-length input throws
    std::vector<float> odd = {1.0f, 2.0f, 3.0f};
    bool threw = false;
    try { applyBs2bCrossfeed(odd, 44100, 0); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);

    // Test invalid sample rate
    threw = false;
    try { applyBs2bCrossfeed({1.0f, 2.0f}, 0, 0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // Test invalid level
    threw = false;
    try { applyBs2bCrossfeed({1.0f, 2.0f}, 44100, 6); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);

    // Test that zero input remains zero (filter state starts at zero)
    std::vector<float> zeros(1024, 0.0f);
    auto outZeros = applyBs2bCrossfeed(zeros, 44100, 3);
    for (float v : outZeros) {
        assert(v == 0.0f);
    }

    // Test deterministic result: same input, same params -> same output
    std::vector<float> sig = {0.1f, -0.2f, 0.3f, 0.5f, -0.4f, 0.2f};
    auto o1 = applyBs2bCrossfeed(sig, 44100, 2);
    auto o2 = applyBs2bCrossfeed(sig, 44100, 2);
    assert(o1 == o2);
}
#include <vector>
#include <cmath>
#include <stdexcept>
#include <numbers>

/**
 * Apply BS2B crossfeed to interleaved stereo audio.
 * 
 * @param input   Interleaved stereo samples: L, R, L, R, ...
 * @param srate   Sample rate in Hz (must be >= 1)
 * @param level   Crossfeed level: 0=LowC, 1=MiddleC, 2=HighC, 3=LowEC, 4=MiddleEC, 5=HighEC
 * @return        Processed interleaved stereo samples of same length
 * @throws std::invalid_argument if input size is odd
 * @throws std::out_of_range if level is not in [0,5] or srate < 1
 */
std::vector<float> applyBs2bCrossfeed(const std::vector<float>& input, int srate, int level) {
    if (srate < 1) {
        throw std::out_of_range("Sample rate must be at least 1");
    }
    if (level < 0 || level > 5) {
        throw std::out_of_range("Crossfeed level must be 0-5");
    }
    if (input.size() % 2 != 0) {
        throw std::invalid_argument("Input size must be even");
    }
    if (input.empty()) {
        return {};
    }

    // ---- Coefficient setup (from init function) ----
    float Fc_lo, Fc_hi, G_lo, G_hi;
    switch (level) {
        case 0: // LowCLevel
            Fc_lo = 360.0f; Fc_hi = 501.0f;
            G_lo = 0.398107170553497f; G_hi = 0.205671765275719f;
            break;
        case 1: // MiddleCLevel
            Fc_lo = 500.0f; Fc_hi = 711.0f;
            G_lo = 0.459726988530872f; G_hi = 0.228208484414988f;
            break;
        case 2: // HighCLevel
            Fc_lo = 700.0f; Fc_hi = 1021.0f;
            G_lo = 0.530884444230988f; G_hi = 0.250105790667544f;
            break;
        case 3: // LowECLevel
            Fc_lo = 360.0f; Fc_hi = 494.0f;
            G_lo = 0.316227766016838f; G_hi = 0.168236228897329f;
            break;
        case 4: // MiddleECLevel
            Fc_lo = 500.0f; Fc_hi = 689.0f;
            G_lo = 0.354813389233575f; G_hi = 0.187169483835901f;
            break;
        case 5: // HighECLevel
            Fc_lo = 700.0f; Fc_hi = 975.0f;
            G_lo = 0.398107170553497f; G_hi = 0.205671765275719f;
            break;
        // No default; we already checked range
    }

    const float g = 1.0f / (1.0f - G_hi + G_lo);
    const float two_pi = 2.0f * std::numbers::pi_v<float>;

    const float b1_lo = std::exp(-two_pi * Fc_lo / static_cast<float>(srate));
    const float a0_lo = G_lo * (1.0f - b1_lo) * g;

    const float b1_hi = std::exp(-two_pi * Fc_hi / static_cast<float>(srate));
    const float a0_hi = (1.0f - G_hi * (1.0f - b1_hi)) * g;
    const float a1_hi = -b1_hi * g;

    // ---- Process ----
    std::vector<float> output(input.size());
    float z_lo_left = 0.0f, z_hi_left = 0.0f;
    float z_lo_right = 0.0f, z_hi_right = 0.0f;

    for (size_t i = 0; i < input.size(); i += 2) {
        const float xL = input[i];
        const float xR = input[i+1];

        // Left channel filter
        float y0L = a0_hi * xL + z_hi_left;
        z_hi_left = a1_hi * xL + b1_hi * y0L;
        float y1L = a0_lo * xL + z_lo_left;
        z_lo_left = b1_lo * y1L;

        // Right channel filter
        float y0R = a0_lo * xR + z_lo_right;
        z_lo_right = b1_lo * y0R;
        float y1R = a0_hi * xR + z_hi_right;
        z_hi_right = a1_hi * xR + b1_hi * y1R;

        // Crossfeed sum
        output[i] = y0L + y1R;   // Left gets own high + right's low
        output[i+1] = y0R + y1L; // Right gets own high + left's low
    }

    return output;
}
// The core algorithm is a biquad-like two-pole/two-zero IIR filter structure but implemented as two separate first-order filters per channel. The state variables `z_lo` and `z_hi` act as the memory of each first-order filter for the low and high frequency bands, respectively. For each input sample `x`, we compute two outputs: `y0` (high band) and `y1` (low band) using the recurrence relations described. The crossfeed is applied by linearly combining the filtered outputs of the left and right channels: the left output gets its own high-band plus the right's low-band, while the right output gets its own high-band plus the left's low-band. This creates the stereo widening effect. The coefficients are computed once per level/sample-rate combination as per the switch-case table; note the default maps any invalid level to HighECLevel in the original code, but our task requires throwing on invalid level. Edge cases: empty input returns empty; non-even length throws; sample rate < 1 throws. The history state must persist across calls if the function is stateful, but to keep it pure and simple, the task as stated allows each call to start fresh (which is acceptable for a standalone exercise). Time complexity is O(n) where n is the number of audio samples (each sample does constant work). Space complexity is O(n) for the output vector, plus O(1) for state and coefficients.
