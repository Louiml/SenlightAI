/*
Implement a C++ function `std::vector<std::vector<float>> compute_mel_spectrogram(const std::vector<float>& audio, int sample_rate, int n_fft, int hop_length, int n_mels, float fmin, float fmax)` that computes a log-scaled Mel spectrogram from raw audio samples. The function must perform the following steps: (1) optionally pad the audio signal symmetrically by `n_fft/2` zeros on both sides (assume padding is always applied), (2) apply a Hann (Hanning) window to each frame, (3) compute the STFT using a naive DFT implementation (do not rely on external FFT libraries), (4) convert the complex STFT to a power spectrogram (squared magnitude), (5) generate a Mel filter bank with triangular filters using the HTK Mel scale formula, (6) apply the filter bank to the power spectrogram to produce a Mel spectrogram, and (7) convert to dB scale using `10*log10(value)` with an `amin` floor of `1e-10` and no `top_db` clipping (skip the upper bound). The output should be a 2D vector with shape `(n_mels, num_frames)` where each column corresponds to a time frame and each row corresponds to a Mel frequency bin. The input audio is a 1D vector of float samples, and all parameters are positive integers or floats with `sample_rate > 0`, `n_fft > 1`, `hop_length > 0`, `n_mels > 0`, and `0 <= fmin < fmax <= sample_rate/2`. Ensure the function handles cases where the audio length is shorter than `n_fft` (after padding, produce at least one frame if possible) and correctly computes DFT bins for only the non-redundant half (0 to `n_fft/2` inclusive).
*/

#include <vector>
#include <cmath>
#include <complex>
#include <algorithm>

// Compute a log Mel spectrogram from raw audio samples.
// Parameters:
//   audio       - input signal as float samples
//   sample_rate - sampling rate in Hz
//   n_fft       - FFT size (window length)
//   hop_length  - frame shift in samples
//   n_mels      - number of Mel filter banks
//   fmin        - minimum frequency for filter bank
//   fmax        - maximum frequency for filter bank
// Returns:
//   A 2D vector with shape (n_mels, num_frames) containing dB-scaled Mel spectrogram.
std::vector<std::vector<float>> compute_mel_spectrogram(
    const std::vector<float>& audio,
    int sample_rate,
    int n_fft,
    int hop_length,
    int n_mels,
    float fmin,
    float fmax)
{
    const float amin = 1e-10f;

    // 1. Symmetric zero-padding
    int pad = n_fft / 2;
    std::vector<float> padded(pad + audio.size() + pad, 0.0f);
    std::copy(audio.begin(), audio.end(), padded.begin() + pad);

    int num_frames = 0;
    {
        int total = static_cast<int>(padded.size());
        // Ensure at least one frame if padded size >= n_fft
        if (total >= n_fft) {
            num_frames = (total - n_fft) / hop_length + 1;
        }
    }

    // 2. Hann window
    std::vector<float> window(n_fft);
    if (n_fft > 1) {
        for (int i = 0; i < n_fft; ++i) {
            window[i] = 0.5f - 0.5f * std::cos(2.0f * M_PI * i / (n_fft - 1));
        }
    } else {
        window[0] = 1.0f;
    }

    // 3. STFT using naive DFT, store only n_fft/2+1 bins
    int n_bins = n_fft / 2 + 1;
    std::vector<std::vector<std::complex<float>>> stft(
        num_frames, std::vector<std::complex<float>>(n_bins));

    for (int frame = 0; frame < num_frames; ++frame) {
        int start = frame * hop_length;
        for (int k = 0; k < n_bins; ++k) {
            float real = 0.0f, imag = 0.0f;
            for (int n = 0; n < n_fft; ++n) {
                float x = padded[start + n] * window[n];
                float angle = 2.0f * M_PI * k * n / n_fft;
                real += x * std::cos(angle);
                imag -= x * std::sin(angle);
            }
            stft[frame][k] = std::complex<float>(real, imag);
        }
    }

    // 4. Power spectrogram: shape (n_bins, num_frames)
    std::vector<std::vector<float>> power_spec(
        n_bins, std::vector<float>(num_frames, 0.0f));
    for (int t = 0; t < num_frames; ++t) {
        for (int k = 0; k < n_bins; ++k) {
            float r = stft[t][k].real();
            float i = stft[t][k].imag();
            power_spec[k][t] = r * r + i * i;
        }
    }

    // 5. Mel filter bank (HTK scale)
    auto hz_to_mel = [](float f) {
        return 2595.0f * std::log10(1.0f + f / 700.0f);
    };
    auto mel_to_hz = [](float m) {
        return 700.0f * (std::pow(10.0f, m / 2595.0f) - 1.0f);
    };

    float mel_min = hz_to_mel(fmin);
    float mel_max = hz_to_mel(fmax);

    std::vector<float> mel_points(n_mels + 2);
    for (int i = 0; i < n_mels + 2; ++i) {
        float mel = mel_min + (mel_max - mel_min) * i / (n_mels + 1);
        mel_points[i] = mel_to_hz(mel);
    }

    std::vector<int> bin_points(n_mels + 2);
    for (int i = 0; i < n_mels + 2; ++i) {
        int bin = static_cast<int>(std::floor((n_fft + 1) * mel_points[i] / sample_rate));
        bin = std::max(0, std::min(bin, n_fft / 2));
        bin_points[i] = bin;
    }

    std::vector<std::vector<float>> mel_filter_bank(
        n_mels, std::vector<float>(n_bins, 0.0f));
    for (int m = 1; m <= n_mels; ++m) {
        int left = bin_points[m - 1];
        int center = bin_points[m];
        int right = bin_points[m + 1];

        if (center == left || center == right) {
            continue; // degenerate triangle, skip
        }

        // Rising edge
        for (int j = left; j < center; ++j) {
            mel_filter_bank[m - 1][j] =
                static_cast<float>(j - left) / static_cast<float>(center - left);
        }
        // Falling edge
        for (int j = center; j <= right && j < n_bins; ++j) {
            mel_filter_bank[m - 1][j] =
                static_cast<float>(right - j) / static_cast<float>(right - center);
        }
    }

    // 6. Apply filter bank: mel_spec shape (n_mels, num_frames)
    std::vector<std::vector<float>> mel_spec(
        n_mels, std::vector<float>(num_frames, 0.0f));
    for (int m = 0; m < n_mels; ++m) {
        for (int t = 0; t < num_frames; ++t) {
            float sum = 0.0f;
            for (int k = 0; k < n_bins; ++k) {
                sum += mel_filter_bank[m][k] * power_spec[k][t];
            }
            mel_spec[m][t] = sum;
        }
    }

    // 7. Convert to dB scale (no top_db clipping)
    std::vector<std::vector<float>> log_mel_spec(
        n_mels, std::vector<float>(num_frames));
    for (int m = 0; m < n_mels; ++m) {
        for (int t = 0; t < num_frames; ++t) {
            float val = std::max(mel_spec[m][t], amin);
            log_mel_spec[m][t] = 10.0f * std::log10(val);
        }
    }

    return log_mel_spec;
}

#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution function is declared above or included separately.
// For this test, include the implementation directly or via header.

int main() {
    // Test 1: Silence audio produces all negative infinity (actually log10(amin) = -100)
    {
        std::vector<float> audio(8000, 0.0f);
        auto spec = compute_mel_spectrogram(audio, 16000, 512, 128, 40, 0.0f, 8000.0f);
        assert(spec.size() == 40);
        // num_frames = (pad + len + pad - n_fft)/hop + 1 = (256+8000+256-512)/128+1 = 8000/128 + 1 = 63 (since 62*128=7936, 7936+512=8448 ≤ 8512, so 63 frames)
        int expected_frames = (256 + 8000 + 256 - 512) / 128 + 1; // check calculation
        assert(spec[0].size() == static_cast<size_t>(expected_frames));
        for (auto& row : spec) {
            for (float val : row) {
                assert(std::abs(val - 10.0f * std::log10(1e-10f)) < 1e-4f);
            }
        }
    }

    // Test 2: Single sine wave at known frequency, ensure output is finite and positive
    {
        int sr = 8000;
        int n_fft = 256;
        int hop = 64;
        int n_mels = 20;
        float freq = 1000.0f;
        std::vector<float> audio;
        for (int i = 0; i < sr; ++i) {
            audio.push_back(std::sin(2.0f * M_PI * freq * i / sr));
        }
        auto spec = compute_mel_spectrogram(audio, sr, n_fft, hop, n_mels, 0.0f, sr/2);
        assert(spec.size() == n_mels);
        // There should be at least one frame
        assert(spec[0].size() > 0);
        // Values should be finite and not all -100
        bool has_positive = false;
        for (auto& row : spec) {
            for (float val : row) {
                assert(std::isfinite(val));
                if (val > -99.0f) has_positive = true;
            }
        }
        assert(has_positive);
    }

    // Test 3: Check filter bank count and frame count with small audio
    {
        std::vector<float> audio(100, 0.5f); // 100 samples
        int sr = 1000, n_fft = 128, hop = 32, n_mels = 10;
        auto spec = compute_mel_spectrogram(audio, sr, n_fft, hop, n_mels, 0.0f, 500.0f);
        assert(spec.size() == 10);
        int padded_len = 2*(n_fft/2) + 100; // 128 + 100 = 228
        int frames = (228 - 128) / 32 + 1; // 100/32 +1 = 3+1 = 4 (since 3*32=96, 96+128=224 ≤ 228)
        assert(spec[0].size() == static_cast<size_t>(frames));
    }

    // Test 4: Edge case n_fft=1 (degenerate window and DFT)
    {
        std::vector<float> audio = {1.0f, 2.0f, 3.0f};
        auto spec = compute_mel_spectrogram(audio, 100, 1, 1, 5, 0.0f, 50.0f);
        assert(spec.size() == 5);
        // padded len = 0 + 3 + 0 = 3, frames = (3-1)+1 = 3
        assert(spec[0].size() == 3);
        for (auto& row : spec) {
            for (float val : row) {
                assert(std::isfinite(val));
            }
        }
    }

    // Test 5: Throw or handle invalid fmin >= fmax? Not required here, but we test typical valid case.
    {
        std::vector<float> audio(1000, 0.1f);
        auto spec = compute_mel_spectrogram(audio, 44100, 1024, 256, 32, 20.0f, 20000.0f);
        assert(spec.size() == 32);
        assert(spec[0].size() > 0);
    }

    return 0;
}

// The solution involves a complete signal-processing pipeline implemented from scratch. The algorithm proceeds as follows:  
// 1. **Padding**: Create a new vector of size `2*(n_fft/2) + audio.size()` initialized to zeros, then copy the audio into the middle. This ensures the first frame starts at index 0 of the padded signal and each frame captures context at both ends. For a naive DFT, we iterate frames starting at index 0 with step `hop_length`, and stop when `frame + n_fft <= padded.size()`. If `audio.size()` is 0, no frames are produced (but padding still gives `n_fft` zeros; we can produce one frame for a zero signal).  
// 2. **Hann window**: For each frame, extract `n_fft` samples, multiply element-wise by `0.5 - 0.5*cos(2*pi*i/(n_fft-1))` for `i=0..n_fft-1`. Handle `n_fft==1` separately (window all ones) to avoid division by zero.  
// 3. **DFT**: For each frame, compute complex DFT for `k=0..n_fft/2` using `X[k] = sum_{n=0}^{n_fft-1} x[n] * (cos(2*pi*k*n/n_fft) - j*sin(2*pi*k*n/n_fft))`. This is O(n_fft^2) per frame, which is acceptable for educational tasks but noted in complexity. Store real and imaginary parts in a simple struct or use `std::complex<float>`.  
// 4. **Power spectrogram**: For each frame and each frequency bin `k`, compute `real^2 + imag^2`. Store as a 2D vector with shape `(n_fft/2+1, num_frames)` (frequency-major for easier filter bank application).  
// 5. **Mel filter bank**: Use HTK formula: `mel = 2595 * log10(1+f/700)`. Compute `mel_fmin` and `mel_fmax`, then generate `n_mels+2` equally spaced mel points inclusive of endpoints. Convert each mel point back to Hz using `f = 700 * (10^(mel/2595) - 1)`. Map Hz to FFT bin indices via `bin = floor((n_fft+1)*f/sample_rate)`. For each filter `i` (1..n_mels), create a triangular shape with rise from `bin[i-1]` to `bin[i]` and fall from `bin[i]` to `bin[i+1]`. Ensure indices are clamped to `[0, n_fft/2]` to avoid out-of-bounds.  
// 6. **Apply filter bank**: For each Mel filter and each frame, compute the dot product of the filter's weights with the power spectrogram column (or slice). Store result as `mel_spec[n_mels][num_frames]`.  
// 7. **Convert to dB**: For each element, take `max(value, amin)` (with `amin=1e-10`), compute `10*log10(max_val)`. No upper bound clipping.  
// 8. **Output**: Transpose the result so output shape is `(n_mels, num_frames)`.  
//
// Edge cases:  
// - If `audio` is empty, after padding the padded size is `n_fft` (since padding is symmetric anyway), so we produce at least one frame of zeros.  
// - If `n_fft` is even, `n_fft/2+1` is correct frequency bins.  
// - If `n_fft` is odd, we still use `n_fft/2+1` bins (floor division), which gives `(n_fft-1)/2+1` = `(n_fft+1)/2` bins, matching common STFT practice.  
// - The filter bank bin calculation uses `(n_fft+1)` to match typical librosa behavior; ensure indices don't exceed `n_fft/2` by clamping `min(bin, n_fft/2)`.  
// - If two consecutive bin points are equal (rare for small `n_fft` and high `n_mels`), avoid division by zero in triangle normalization; skip that bin or set triangle to zero.  
//
// Time complexity: For `F` frames, `N = n_fft`, `M = n_mels`, the DFT is `O(F * N^2)`, filter bank generation is `O(M * (N/2))`, and applying filters is `O(M * F * (N/2))`. Overall dominant term is `O(F * N^2)` for realistic parameters. Space complexity is `O(N*F + M*N/2 + M*F)` for the power spectrogram, filter bank, and mel spectrogram.
