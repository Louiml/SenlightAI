Given an even-length real-valued discrete signal and a frequency-domain representation produced by a real-input fast Fourier transform (FFT) that stores only the first \(N/2+1\) complex bins (where \(N\) is the full signal length), write a C++ function that reconstructs the original real time-domain signal. The function must implement the inverse real FFT using the split-radix/decimation-in-time approach: it should combine the Hermitian-symmetric frequency bins into the internal complex half-length FFT, apply appropriate twiddle factors, and then perform the inverse complex FFT. The input is provided as a vector of `std::complex<double>` representing the non-redundant frequency bins (indices 0 through \(N/2\) inclusive), and the output must be a vector of `double` of length \(N\). The input length \(N\) is guaranteed to be even and at least 2. The function should not rely on external FFT libraries; instead, it must implement the radix-2 Cooley-Tukey inverse complex FFT internally. The twiddle factors for the real-to-complex recombination are \(e^{-i\pi(k+0.5)/M}\) for the forward direction, where \(M = N/2\), and the conjugate for the inverse direction. The function must handle both real-valued DC (bin 0) and Nyquist (bin \(N/2\)) bins correctly, with the imaginary parts of both forced to zero in the output.

The solution follows the standard real FFT algorithm: for a real signal of length \(N\), pack the even-indexed samples into the real part and odd-indexed samples into the imaginary part of a complex signal of length \(M = N/2\). After performing a complex FFT on this packed signal, the frequency bins \(F[k]\) for \(0 \le k < M\) are obtained. The real FFT bins \(X[k]\) for \(0 \le k \le M\) are then reconstructed using the symmetry \(X[M-k] = \overline{X[M+k]}\), which yields formulas: \(X[0] = (F[0].r + F[0].i)/2\), \(X[M] = (F[0].r - F[0].i)/2\), and for \(1 \le k \le M/2\): let \(F_k = F[k]\), \(F_{M-k} = \overline{F[M-k]}\), then \(E_k = (F_k + F_{M-k})/2\), \(O_k = (F_k - F_{M-k})/2\), and \(X[k] = E_k + O_k \cdot e^{-i\pi(k)/M}\), \(X[M-k] = E_k - O_k \cdot e^{-i\pi(k)/M}\). For the inverse operation, we reverse this process: given the frequency bins \(X\), we reconstruct the packed signal \(F\) by: \(F[0] = X[0] + X[M]\) for real part and \(X[0] - X[M]\) for imaginary part (both divided by 2), \(F[k] = (X[k] + \overline{X[M-k]})/2 + (X[k] - \overline{X[M-k]})/2 \cdot e^{+i\pi k/M}\) for \(1 \le k \le M/2\), and \(F[M-k] = \) the conjugate of the corresponding value adjusted by the same twiddle but with negative imaginary sign. Then, run an inverse complex FFT on \(F\) to obtain the packed time-domain sequence, whose real parts are the even samples and imaginary parts are the odd samples of the original real signal. Edge cases: the DC and Nyquist bins must have their imaginary parts set to zero in the output (since the original signal is real). The complex FFT must be implemented recursively or iteratively; for simplicity, we use an iterative radix-2 inverse FFT with bit-reversal permutation, handling only lengths that are powers of two? The problem does not specify the length is a power of two, so we must handle arbitrary even lengths. However, the standard radix-2 FFT requires \(M\) to be a power of two. Since the problem statement is based on kissfft which supports arbitrary sizes via mixed-radix, but for simplicity and to keep the task self-contained, we assume \(N\) is a power of two (and thus \(M\) is a power of two). The task specification says "even length" but does not explicitly say power of two; however, to provide a correct reference solution, we restrict to power-of-two lengths and note this in comments. The solution implements an iterative in-place radix-2 inverse FFT for complex input of size \(M\), using precomputed twiddle factors. Time complexity is \(O(N \log N)\) and space is \(O(N)\) for the output and intermediate arrays.

#include <vector>
#include <complex>
#include <cmath>
#include <cassert>

// Inverse real FFT for a real signal of length N (N even, N/2 is power of two).
// Input: freqData of length N/2+1, containing the non-redundant complex frequency bins.
// Output: vector<double> of length N, the reconstructed real time-domain signal.
std::vector<double> inverseRealFFT(const std::vector<std::complex<double>>& freqData) {
    const size_t N = (freqData.size() - 1) * 2;
    const size_t M = N / 2;
    assert(N % 2 == 0 && M > 0 && (M & (M - 1)) == 0); // M must be power of two

    // 1. Reconstruct the packed complex sequence F of length M
    std::vector<std::complex<double>> F(M);

    // DC and Nyquist bins
    std::complex<double> bin0 = freqData[0];
    std::complex<double> binNyq = freqData[M];
    // In a real signal, both DC and Nyquist have zero imaginary parts in the correct representation
    // F[0].r = (X[0].r + X[N/2].r) / 2, F[0].i = (X[0].r - X[N/2].r) / 2
    F[0] = std::complex<double>((bin0.real() + binNyq.real()) * 0.5,
                                (bin0.real() - binNyq.real()) * 0.5);

    // Reconstruct other bins using Hermitian symmetry
    for (size_t k = 1; k <= M / 2; ++k) {
        std::complex<double> Xk = freqData[k];
        std::complex<double> Xnk = std::conj(freqData[M - k]); // X[M-k] conjugate
        std::complex<double> E = (Xk + Xnk) * 0.5;
        std::complex<double> O = (Xk - Xnk) * 0.5;

        double angle = M_PI * static_cast<double>(k) / M; // note: e^{+i*pi*k/M} for inverse
        std::complex<double> twiddle(std::cos(angle), std::sin(angle));
        std::complex<double> Fk = E + O * twiddle;
        std::complex<double> Fmk = std::conj(E - O * twiddle); // F[M-k] is conjugate symmetric
        F[k] = Fk;
        F[M - k] = Fmk;
    }

    // 2. Perform inverse complex FFT of size M on F
    // Implement iterative radix-2 inverse FFT (result / M)
    // Bit-reversal permutation
    for (size_t i = 1, j = 0; i < M; ++i) {
        size_t bit = M >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(F[i], F[j]);
        }
    }

    // Butterfly stages
    for (size_t len = 2; len <= M; len <<= 1) {
        double angle = -2.0 * M_PI / static_cast<double>(len); // inverse has negative sign
        std::complex<double> wlen(std::cos(angle), std::sin(angle));
        for (size_t i = 0; i < M; i += len) {
            std::complex<double> w(1.0, 0.0);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = F[i + j];
                std::complex<double> v = F[i + j + len / 2] * w;
                F[i + j] = u + v;
                F[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // Scale by 1/M (inverse FFT)
    for (size_t i = 0; i < M; ++i) {
        F[i] /= static_cast<double>(M);
    }

    // 3. Extract real time-domain samples from F: even samples from real parts, odd from imag
    std::vector<double> timeData(N);
    for (size_t i = 0; i < M; ++i) {
        timeData[2 * i] = F[i].real();
        timeData[2 * i + 1] = F[i].imag();
    }

    return timeData;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <complex>

// Forward real FFT using the same packing technique (for testing purposes)
std::vector<std::complex<double>> forwardRealFFT(const std::vector<double>& timeData) {
    size_t N = timeData.size();
    size_t M = N / 2;
    // Pack even into real, odd into imag
    std::vector<std::complex<double>> F(M);
    for (size_t i = 0; i < M; ++i) {
        F[i] = std::complex<double>(timeData[2*i], timeData[2*i+1]);
    }
    // Forward complex FFT (iterative radix-2)
    for (size_t i = 1, j = 0; i < M; ++i) {
        size_t bit = M >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(F[i], F[j]);
        }
    }
    for (size_t len = 2; len <= M; len <<= 1) {
        double angle = 2.0 * M_PI / static_cast<double>(len);
        std::complex<double> wlen(std::cos(angle), std::sin(angle));
        for (size_t i = 0; i < M; i += len) {
            std::complex<double> w(1.0, 0.0);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = F[i + j];
                std::complex<double> v = F[i + j + len / 2] * w;
                F[i + j] = u + v;
                F[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    // Reconstruct real FFT bins
    std::vector<std::complex<double>> X(M + 1);
    X[0] = std::complex<double>((F[0].real() + F[0].imag()) * 0.5, 0.0);
    X[M] = std::complex<double>((F[0].real() - F[0].imag()) * 0.5, 0.0);
    for (size_t k = 1; k <= M / 2; ++k) {
        std::complex<double> fk = F[k];
        std::complex<double> fnk = std::conj(F[M - k]);
        std::complex<double> E = (fk + fnk) * 0.5;
        std::complex<double> O = (fk - fnk) * 0.5;
        double angle = -M_PI * static_cast<double>(k) / M;
        std::complex<double> twiddle(std::cos(angle), std::sin(angle));
        X[k] = E + O * twiddle;
        X[M - k] = std::conj(E - O * twiddle);
    }
    return X;
}

int main() {
    // Test 1: Simple sine wave
    size_t N = 8;
    std::vector<double> signal(N);
    for (size_t i = 0; i < N; ++i) {
        signal[i] = std::sin(2.0 * M_PI * 1.0 * i / N);
    }
    auto freq = forwardRealFFT(signal);
    auto reconstructed = inverseRealFFT(freq);
    for (size_t i = 0; i < N; ++i) {
        assert(std::abs(reconstructed[i] - signal[i]) < 1e-10);
    }

    // Test 2: DC + Nyquist
    N = 8;
    signal = std::vector<double>(N, 2.0);
    for (size_t i = 0; i < N; i += 2) signal[i] = 1.0; // not pure, but still real
    freq = forwardRealFFT(signal);
    reconstructed = inverseRealFFT(freq);
    for (size_t i = 0; i < N; ++i) {
        assert(std::abs(reconstructed[i] - signal[i]) < 1e-10);
    }

    // Test 3: Random values, N=16
    N = 16;
    signal.resize(N);
    for (size_t i = 0; i < N; ++i) signal[i] = (i * 37) % 10 - 5; // deterministic pattern
    freq = forwardRealFFT(signal);
    reconstructed = inverseRealFFT(freq);
    for (size_t i = 0; i < N; ++i) {
        assert(std::abs(reconstructed[i] - signal[i]) < 1e-10);
    }

    // Test 4: N=4, all zeros
    N = 4;
    signal = std::vector<double>(N, 0.0);
    freq = forwardRealFFT(signal);
    reconstructed = inverseRealFFT(freq);
    for (size_t i = 0; i < N; ++i) {
        assert(std::abs(reconstructed[i]) < 1e-10);
    }

    return 0;
}
