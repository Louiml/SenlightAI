// Write a C++ function that takes an array of `std::complex<double>` values and its length `N` (where `N` is a positive power of 2), and applies the discrete Hilbert transform in-place using the following steps: (1) compute the forward FFT of the array, (2) multiply each frequency-domain coefficient by a "Heaviside‑like" filter where the DC component (index 0) and the Nyquist component (index `N/2`) are multiplied by 1, the positive frequency components (indices `1` to `N/2 - 1`) are multiplied by 2, and all negative frequency components (indices `N/2 + 1` to `N-1`) are multiplied by 0, and (3) compute the inverse FFT to return the result to the time domain. The function must operate in-place (modify the input array) and must not allocate dynamic memory of size depending on `N` beyond what standard FFT libraries need; assume an external `fft_complx` class is available with methods `fft_fwd(std::complex<double>*, int)` and `fft_bwd(std::complex<double>*, int)` that perform forward and inverse FFT in place (with scale factor 1/N on inverse). Provide a free function named `applyHilbertTransform` that takes a non-const pointer to the array and an integer `N`, and returns `void`. The implementation should be standalone (no `main`), but must include necessary headers (`<complex>`, `<fft_complx.h>`). Ensure that the function is correct for `N` = 1 (where the only component is both DC and Nyquist) and for all positive powers of 2.

// The main algorithm is straightforward: we use the FFT to convert the time-domain signal to its frequency-domain representation, apply a frequency-domain filter that removes negative-frequency components (while halving the amplitude of positive frequencies except for DC and Nyquist where the factor is 1, as the Hilbert transform's frequency response is `-j*sgn(f)` but here we implement the "analytic signal" version where negative frequencies are zeroed and positive ones doubled, with DC and Nyquist kept as is), and then inverse FFT back. The key edge case is `N = 1`: here the loop goes only once with `i==0` and `N/2 == 0`, so the condition `i==0||i==N/2` triggers and multiplies by 1, which is correct (no positive or negative frequencies exist). For `N > 1`, the filter correctly sets `hv[0]=1`, `hv[N/2]=1` (if `N/2` is distinct from 0, i.e., `N>=2`), `hv[i]=2` for `1 <= i < N/2`, and `hv[i]=0` for `i > N/2`. This matches the provided snippet. The time complexity is dominated by the two FFTs, each of which is `O(N log N)` for a typical FFT library. The auxiliary space used by the function itself is `O(N)` due to the `double hv[N]` array (which is a VLA and not standard C++ but works with some compilers; however, we can replace it with `std::vector<double>` or simply compute the multiplier on the fly without storing the whole array, reducing space to `O(1)`). To be safe and portable, we'll compute the multiplier for each `i` directly without an array. This gives `O(1)` extra space besides the input and the FFT library's internal buffers. Edge cases: `N` must be a positive power of 2; the function assumes this is true (no validation needed per the task). The inverse FFT must include the 1/N scaling, which the `fft_complx` library is assumed to handle.

#include <complex>
#include <fft_complx.h>

// Apply the discrete Hilbert transform (analytic signal version) in-place.
// Parameters:
//   x: pointer to an array of N complex numbers (modified in place)
//   N: length of the array, must be a positive power of 2
// Uses external fft_complx class with fft_fwd and fft_bwd methods.
void applyHilbertTransform(std::complex<double>* x, int N) {
    fft_complx myfft;
    myfft.fft_fwd(x, N);  // forward FFT in-place

    for (int i = 0; i < N; ++i) {
        double hv;
        if (i == 0 || i == N / 2) {
            hv = 1.0;          // DC and Nyquist components
        } else if (i > 0 && i < N / 2) {
            hv = 2.0;          // positive frequencies
        } else {
            hv = 0.0;          // negative frequencies
        }
        x[i] *= hv;            // apply filter
    }

    myfft.fft_bwd(x, N);  // inverse FFT in-place (includes 1/N scaling)
}

#include <cassert>
#include <complex>
#include <fft_complx.h>

// Declare the solution function (it would be in a separate header normally)
void applyHilbertTransform(std::complex<double>* x, int N);

int main() {
    // Test 1: N=1, constant signal -> Hilbert transform should return same value (DC only)
    {
        std::complex<double> x[1] = { {3.0, 0.0} };
        applyHilbertTransform(x, 1);
        assert(std::abs(x[0].real() - 3.0) < 1e-9);
        assert(std::abs(x[0].imag()) < 1e-9);
    }

    // Test 2: N=2, signal [1, -1] -> analytic signal should be [1, -1] (since only Nyquist and DC)
    {
        std::complex<double> x[2] = { {1.0, 0.0}, {-1.0, 0.0} };
        applyHilbertTransform(x, 2);
        // The analytic signal of [1,-1] is itself (because it's composed of DC and Nyquist only)
        assert(std::abs(x[0].real() - 1.0) < 1e-9);
        assert(std::abs(x[0].imag()) < 1e-9);
        assert(std::abs(x[1].real() + 1.0) < 1e-9);
        assert(std::abs(x[1].imag()) < 1e-9);
    }

    // Test 3: N=4, impulse at time 0: [1,0,0,0] -> analytic signal has real part = [1,1,0,0]? Actually known result: analytic of delta is delta (all frequencies 1, but negative zeroed, positive doubled, inverse gives [1, 1, 0, 0]? Let's verify manually.
    // For N=4, delta gives all four freq components = 1. After filter: hv[0]=1, hv[1]=2, hv[2]=1, hv[3]=0 → spectrum = [1,2,1,0]. Inverse FFT (with scale 1/4) yields:
    // Real parts: (1+2+1+0)/4 = 1, (1+2cos(pi/2)+1cos(pi)+0)/4 = (1+0-1)/4=0, etc. Actually compute: x[0]= (1+2+1+0)/4=1, x[1]= (1+2*(0)+1*(-1)+0)/4=0, x[2]= (1+2*(-1)+1*1+0)/4=0, x[3]= (1+2*(0)+1*(-1)+0)/4=0. So real = [1,0,0,0]. Imag parts: for a real signal, the analytic signal's imaginary part is the Hilbert transform; for delta, it should be [0, 2/π, 0, -2/π]? But because we are using the analytic signal (not -j*sgn), the imaginary part is not simply Hilbert; for delta, real part remains delta and imaginary is zero? Actually the standard analytic signal of a real delta is delta itself (since it already contains only positive frequencies? No, delta has all frequencies, but the analytic signal removes negative ones, so the result is complex with real part being a "sinc-like" and imaginary part being the Hilbert transform). Let's not overcomplicate; we'll just check that the output is finite and that applying forward FFT on the output and comparing spectrum works. For simplicity, we'll test that the total energy (sum of squared magnitudes) is preserved? No, not. Let's just test a known case: for a cosine at positive frequency f, the analytic signal is e^{j2πft}. 
    // Instead, we'll test a simple ramp-like signal and verify the DC component remains unchanged.
    {
        std::complex<double> x[4] = { {1.0, 0.0}, {2.0, 0.0}, {3.0, 0.0}, {4.0, 0.0} };
        // Compute DC component before transform
        double dc_before = 0;
        for (int i = 0; i < 4; ++i) dc_before += x[i].real();
        applyHilbertTransform(x, 4);
        double dc_after = 0;
        for (int i = 0; i < 4; ++i) dc_after += x[i].real();
        // The DC component of the output should equal the original DC (since DC is multiplied by 1)
        assert(std::abs(dc_after - dc_before) < 1e-9);
        // Also check that the sum of imaginary parts is zero? For a real input, imaginary part is the Hilbert transform, which sums to zero (since its DC is zero). That holds.
        double imag_sum = 0;
        for (int i = 0; i < 4; ++i) imag_sum += x[i].imag();
        assert(std::abs(imag_sum) < 1e-9);
    }

    // Test 4: N=8, known property: applying Hilbert twice (with the analytic signal version) gives zero? Not exactly. We'll just test idempotence? No. We'll test that the function returns the same type of output for a pure sine wave and check the analytic signal's magnitude is constant.
    {
        const int N = 8;
        std::complex<double> x[N];
        for (int i = 0; i < N; ++i) x[i] = {std::cos(2.0 * M_PI * 1.0 * i / N), 0.0};
        applyHilbertTransform(x, N);
        // The analytic signal of cos(2πf t) is e^{j2πf t} = cos + j*sin. Check imaginary part equals sin.
        for (int i = 0; i < N; ++i) {
            double expected_imag = std::sin(2.0 * M_PI * 1.0 * i / N);
            assert(std::abs(x[i].imag() - expected_imag) < 1e-6);
        }
    }

    // Test 5: Edge case with N=1 and complex input
    {
        std::complex<double> x[1] = { {2.0, -3.0} };
        applyHilbertTransform(x, 1);
        assert(std::abs(x[0].real() - 2.0) < 1e-9);
        assert(std::abs(x[0].imag() + 3.0) < 1e-9);
    }

    return 0;
}
