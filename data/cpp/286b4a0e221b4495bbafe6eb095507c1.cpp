Write a C++ function `fftPowerOfTwoZeroPad` that takes a vector of real-valued samples (as `std::vector<double>`) and returns a new vector of real values representing the forward real FFT of the input after zero-padding the signal symmetrically to the next power-of-two length (at least 4). The output should contain the real and imaginary parts of the complex FFT bins interleaved as `[Re(bin0), Im(bin0), Re(bin1), Im(bin1), ...]`, where the FFT follows the convention: \( X[k] = \sum_{n=0}^{N-1} x[n] \cdot e^{-j 2\pi k n / N} \), with \( N \) being the padded length. No normalization is applied. If the input length is already a power of two and at least 4, no padding occurs. If the input is shorter than 4, pad symmetrically to length 4 (i.e., pad `(4 - n)/2` zeros on each side, with the left and right padding counts differing appropriately if `(4 - n)` is odd, placing the extra zero on the right). The function must perform an in-place radix-2 Cooley-Tukey FFT internally on a working copy, and must correctly handle edge cases like empty input (return empty vector) and single-element input (padded to length 4). You may not use external libraries beyond the C++ standard library.

// The core algorithm is a standard iterative radix-2 FFT, but adapted for real-valued input. Since the input is real, we still compute a full complex FFT of length \(N\) (where \(N\) is the next power of two ≥ max(4, original length)). The FFT itself uses the classic bit-reversal permutation followed by butterfly stages with precomputed twiddle factors for speed and correctness. The main steps are: (1) determine the padded length \(N\) by rounding up to the next power of two and ensuring at least 4; (2) create a working complex vector (real part from the input, imag part zero) with symmetric zero padding—the input is placed at the center, with zeros on both sides, and if the total padding is odd, the extra zero goes on the right; (3) apply the iterative FFT (decimation-in-time) using a standard implementation with cosine and sine tables; (4) extract the interleaved real/imag output. Edge cases: empty input returns empty; if input length is already power of two and ≥4, no padding; odd padding distribution must place the extra zero on the right. Time complexity: \(O(N \log N)\) for the FFT, with \(N\) being the padded length. Space complexity: \(O(N)\) for the working vector and twiddle tables.

#include <vector>
#include <cmath>
#include <cstddef>
#include <algorithm>

// Compute the next power of two >= n, but at least min_n (here 4).
static std::size_t nextPowerOfTwoAtLeast4(std::size_t n) {
    std::size_t p = 1;
    while (p < n) p <<= 1;
    if (p < 4) p = 4;
    return p;
}

// Iterative radix-2 FFT (in-place). The vector contains interleaved real/imag.
static void fftIterative(std::vector<double>& data, std::size_t N) {
    // Bit-reversal permutation
    for (std::size_t i = 1, j = 0; i < N; i++) {
        std::size_t bit = N >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            std::swap(data[2*i], data[2*j]);
            std::swap(data[2*i+1], data[2*j+1]);
        }
    }

    // Precompute twiddle factors per stage
    for (std::size_t len = 2; len <= N; len <<= 1) {
        double angle = -2.0 * M_PI / static_cast<double>(len);
        double wRe = std::cos(angle);
        double wIm = std::sin(angle);
        for (std::size_t i = 0; i < N; i += len) {
            double curRe = 1.0, curIm = 0.0;
            for (std::size_t k = 0; k < len/2; k++) {
                std::size_t a = i + k;
                std::size_t b = i + k + len/2;
                double xRe = data[2*a];
                double xIm = data[2*a+1];
                double yRe = data[2*b];
                double yIm = data[2*b+1];
                double tRe = curRe * yRe - curIm * yIm;
                double tIm = curRe * yIm + curIm * yRe;
                data[2*a]   = xRe + tRe;
                data[2*a+1] = xIm + tIm;
                data[2*b]   = xRe - tRe;
                data[2*b+1] = xIm - tIm;
                double nextRe = curRe * wRe - curIm * wIm;
                curIm = curRe * wIm + curIm * wRe;
                curRe = nextRe;
            }
        }
    }
}

// Symmetrically zero-pad the input to the next power of two (at least 4) and return the interleaved real FFT bins.
std::vector<double> fftPowerOfTwoZeroPad(const std::vector<double>& input) {
    if (input.empty()) return {};

    std::size_t n = input.size();
    std::size_t N = nextPowerOfTwoAtLeast4(n);

    // Working vector interleaved: real, imag
    std::vector<double> x(2 * N, 0.0);

    // Compute symmetric padding
    std::size_t padTotal = N - n;
    std::size_t padLeft = padTotal / 2;
    std::size_t padRight = padTotal - padLeft; // extra zero goes to right if odd

    // Place input at center
    for (std::size_t i = 0; i < n; i++) {
        x[2 * (padLeft + i)] = input[i];
        x[2 * (padLeft + i) + 1] = 0.0; // already zero, but explicit for clarity
    }

    fftIterative(x, N);

    // Extract interleaved real and imaginary parts
    std::vector<double> output;
    output.reserve(2 * N);
    for (std::size_t i = 0; i < N; i++) {
        output.push_back(x[2*i]);
        output.push_back(x[2*i+1]);
    }
    return output;
}

#include <cassert>
#include <cmath>
#include <vector>

// The function is defined above; here we test it.
int main() {
    // Empty input
    std::vector<double> empty;
    assert(fftPowerOfTwoZeroPad(empty).empty());

    // Single sample: symmetric pad to 4 => [0, x, 0, 0]
    // FFT of [0, 1, 0, 0] gives [1, 0, -1, 0, 1, 0, -1, 0] (interleaved)
    std::vector<double> single = {1.0};
    auto r1 = fftPowerOfTwoZeroPad(single);
    assert(r1.size() == 8);
    assert(std::fabs(r1[0] - 1.0) < 1e-9);
    assert(std::fabs(r1[1] - 0.0) < 1e-9);
    assert(std::fabs(r1[2] - (-1.0)) < 1e-9);
    assert(std::fabs(r1[3] - 0.0) < 1e-9);
    assert(std::fabs(r1[4] - 1.0) < 1e-9);
    assert(std::fabs(r1[5] - 0.0) < 1e-9);
    assert(std::fabs(r1[6] - (-1.0)) < 1e-9);
    assert(std::fabs(r1[7] - 0.0) < 1e-9);

    // Length 4 (already power of two, no padding)
    std::vector<double> four = {1.0, 2.0, 3.0, 4.0};
    auto r4 = fftPowerOfTwoZeroPad(four);
    // Expected FFT (no normalization): bin0 = 10+0i, bin1 = -2+2i, bin2 = -2+0i, bin3 = -2-2i
    assert(r4.size() == 8);
    assert(std::fabs(r4[0] - 10.0) < 1e-9);
    assert(std::fabs(r4[1] - 0.0) < 1e-9);
    assert(std::fabs(r4[2] - (-2.0)) < 1e-9);
    assert(std::fabs(r4[3] - 2.0) < 1e-9);
    assert(std::fabs(r4[4] - (-2.0)) < 1e-9);
    assert(std::fabs(r4[5] - 0.0) < 1e-9);
    assert(std::fabs(r4[6] - (-2.0)) < 1e-9);
    assert(std::fabs(r4[7] - (-2.0)) < 1e-9);

    // Length 3: pad to 4 => [0, a, b, c] (padLeft=0, padRight=1)
    std::vector<double> three = {1.0, 0.0, 0.0};
    auto r3 = fftPowerOfTwoZeroPad(three);
    // FFT of [1,0,0,0] is [1,1,1,1] real, imag zero
    assert(r3.size() == 8);
    for (int i = 0; i < 8; i += 2) {
        assert(std::fabs(r3[i] - 1.0) < 1e-9);
        assert(std::fabs(r3[i+1] - 0.0) < 1e-9);
    }

    // Length 5: pad to 8 => [0,0, a,b,c,d,e,0] (padLeft=1, padRight=2)
    std::vector<double> five = {1.0, 2.0, 3.0, 4.0, 5.0};
    auto r5 = fftPowerOfTwoZeroPad(five);
    assert(r5.size() == 16);
    // Expected DC bin: sum = 15 real
    assert(std::fabs(r5[0] - 15.0) < 1e-9);
    assert(std::fabs(r5[1] - 0.0) < 1e-9);

    // Length 6: pad to 8 => padLeft=1, padRight=1
    std::vector<double> six = {1.0, -1.0, 1.0, -1.0, 1.0, -1.0};
    auto r6 = fftPowerOfTwoZeroPad(six);
    assert(r6.size() == 16);
    // DC bin = 0, bin4 (Nyquist) should be real sum of all with alternating sign = -6
    assert(std::fabs(r6[0] - 0.0) < 1e-9);
    assert(std::fabs(r6[8] - (-6.0)) < 1e-9);
    assert(std::fabs(r6[9] - 0.0) < 1e-9);

    return 0;
}
