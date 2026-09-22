// Write a standalone C++ function named `computeDFT` that computes the discrete Fourier transform (DFT) of an input signal. The function must accept a vector of real samples (representing a real-valued signal) and return the real and imaginary parts of the DFT as two separate vectors of doubles. The DFT at frequency index `k` (for `k = 0, 1, ..., N-1`, where `N` is the signal length) is defined as \( F[k] = c \sum_{n=0}^{N-1} s[n] \cdot e^{-i 2\pi k n / N} \), with normalization constant `c = 1/sqrt(N)`. Specifically, for a real input `s`, compute `Re_F[k] = c * sum(s[n] * cos(2*pi*k*n/N))` and `Im_F[k] = -c * sum(s[n] * sin(2*pi*k*n/N))`. The function signature should be: `std::pair<std::vector<double>, std::vector<double>> computeDFT(const std::vector<double>& signal)`. Assume the input is non-empty (N ≥ 1). The function must be self-contained, use only standard headers, and avoid any external libraries.
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Include the solution function here (for brevity, assume it's defined above).

int main() {
    // Test 1: Single sample signal, DFT is the sample itself
    std::vector<double> s1 = {3.0};
    auto dft1 = computeDFT(s1);
    assert(dft1.first.size() == 1 && dft1.second.size() == 1);
    assert(std::abs(dft1.first[0] - 3.0) < 1e-9);
    assert(std::abs(dft1.second[0] - 0.0) < 1e-9);

    // Test 2: DC signal (all ones), only k=0 has non-zero value, value = sqrt(N)
    std::vector<double> s2 = {1.0, 1.0, 1.0, 1.0};
    auto dft2 = computeDFT(s2);
    assert(dft2.first.size() == 4 && dft2.second.size() == 4);
    assert(std::abs(dft2.first[0] - 2.0) < 1e-9); // sqrt(4)=2
    for (int k = 1; k < 4; ++k) {
        assert(std::abs(dft2.first[k]) < 1e-9);
        assert(std::abs(dft2.second[k]) < 1e-9);
    }

    // Test 3: A simple signal [1, 0, -1, 0] (a sine at k=1)
    std::vector<double> s3 = {1.0, 0.0, -1.0, 0.0};
    auto dft3 = computeDFT(s3);
    // Expected: Re_F[1] = 0, Im_F[1] = -1 (since c=0.5, sum = 1*sin(0) -1*sin(pi) + ... = -2? Let's compute manually)
    // Actually compute directly: N=4, c=0.5, for k=1: theta=pi/2
    // n=0: cos=1, sin=0 -> contributions: 1*1=1, 0
    // n=1: cos=0, sin=1 -> contributions: 0, 1*0=0
    // n=2: cos=-1, sin=0 -> contributions: (-1)*(-1)=1, 0
    // n=3: cos=0, sin=-1 -> contributions: 0, (-1)*(-1)=1? Actually sin(3*pi/2) = -1, so imagSum = 1*0 + 0*1 + (-1)*0 + 0*(-1)=0
    // Wait, better to use known property: For real s, Im_F[1] = -c*sum(s[n]*sin(theta*n)). Let's test with actual numbers.
    // We'll just check that the result is consistent with the inverse? For simplicity, check magnitude properties.
    // But we can compute by hand: For s=[1,0,-1,0], the DFT at k=0: Re=0.5*(1+0-1+0)=0, Im=0. k=1: theta=pi/2, contributions: n=0: 1*1=1, sin0=0; n=1:0; n=2: (-1)*cos(pi)=-1*(-1)=1, sin(pi)=0; n=3:0. So ReSum=2, ImSum=0 => Re_F=1, Im_F=0? But that seems odd. Actually the DFT of [1,0,-1,0] is [0,1,0,1]? No, that's for a different normalization. Let's trust the formula: Re_F[1]=1, Im_F[1]=0. Check with the code's logic: theta=pi/2, c=0.5, realSum = 1*cos(0) + 0 + (-1)*cos(pi) + 0 = 1 + 1 = 2, imagSum = 1*sin(0) + 0 + (-1)*sin(pi) + 0 = 0, so Re=1, Im=0. So we can assert that.
    assert(std::abs(dft3.first[1] - 1.0) < 1e-9);
    assert(std::abs(dft3.second[1] - 0.0) < 1e-9);
    // For k=2: theta=pi, cos(0)=1, cos(pi)=-1, cos(2pi)=1, cos(3pi)=-1 => realSum = 1 - (-1) + (-1)*(1) + 0 = 1+1-1=1? Actually compute: n=0:1*1=1; n=1:0; n=2: -1*cos(pi) = -1*(-1)=1; n=3:0 => realSum=2, imagSum=0 => Re_F=1, Im=0. So also 1.
    assert(std::abs(dft3.first[2] - 1.0) < 1e-9);
    assert(std::abs(dft3.second[2] - 0.0) < 1e-9);

    // Test 4: Check Parseval's theorem for a random signal: sum|s|^2 = sum|F|^2
    std::vector<double> s4 = {0.5, -1.2, 2.3, 0.7, -0.4};
    auto dft4 = computeDFT(s4);
    double sumSqInput = 0.0;
    for (double v : s4) sumSqInput += v * v;
    double sumSqOutput = 0.0;
    for (size_t i = 0; i < dft4.first.size(); ++i) {
        sumSqOutput += dft4.first[i]*dft4.first[i] + dft4.second[i]*dft4.second[i];
    }
    assert(std::abs(sumSqInput - sumSqOutput) < 1e-6);

    // Test 5: Odd length signal, ensure zero frequency is mean*sqrt(N)
    std::vector<double> s5 = {2.0, 4.0, 6.0};
    auto dft5 = computeDFT(s5);
    double mean = (2.0+4.0+6.0)/3.0 = 4.0;
    double expectedDC = mean * std::sqrt(3.0); // c = 1/sqrt(3), sum = 12, so c*sum = 12/sqrt(3) = 4*sqrt(3)
    assert(std::abs(dft5.first[0] - 4.0*std::sqrt(3.0)) < 1e-9);
    assert(std::abs(dft5.second[0]) < 1e-9);

    return 0;
}
#include <vector>
#include <cmath>
#include <utility>

// Compute the discrete Fourier transform of a real-valued signal.
// Returns a pair: first is the real part, second is the imaginary part.
// DFT definition: F[k] = c * sum_{n=0}^{N-1} s[n] * exp(-i*2*pi*k*n/N), c = 1/sqrt(N)
std::pair<std::vector<double>, std::vector<double>> computeDFT(const std::vector<double>& signal) {
    const size_t N = signal.size();
    const double c = 1.0 / std::sqrt(static_cast<double>(N));

    std::vector<double> realPart(N, 0.0);
    std::vector<double> imagPart(N, 0.0);

    for (size_t k = 0; k < N; ++k) {
        const double theta = 2.0 * M_PI * static_cast<double>(k) / static_cast<double>(N);
        double realSum = 0.0;
        double imagSum = 0.0;
        for (size_t n = 0; n < N; ++n) {
            const double angle = theta * static_cast<double>(n);
            realSum += signal[n] * std::cos(angle);
            imagSum += signal[n] * std::sin(angle);
        }
        realPart[k] = c * realSum;
        imagPart[k] = -c * imagSum; // Negative sign for imaginary component
    }

    return {realPart, imagPart};
}
// The solution directly implements the definition of the DFT using a double nested loop. For each frequency index `k`, compute the angle `theta = 2*pi*k/N`, then for each time sample `n`, accumulate `signal[n] * cos(theta * n)` into `realSum` and `signal[n] * sin(theta * n)` into `imagSum`. After the inner loop, multiply both by `c = 1/sqrt(N)` and store in the output vectors. The imaginary part is negated because the DFT uses `e^{-i theta n}`. Edge cases: N=1 yields a single frequency with `theta=0`, giving `Re_F[0] = signal[0]` (since c=1) and `Im_F[0]=0`. Floating-point precision is acceptable; no special handling is needed. Time complexity is O(N^2) because there are N frequencies and N samples per frequency. Space complexity is O(N) for the two output vectors, plus O(1) auxiliary space for loop variables. The function returns a `std::pair` of vectors for clarity.
