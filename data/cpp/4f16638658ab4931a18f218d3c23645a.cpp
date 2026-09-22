// Write a C++ function named `inPlaceHartleyTransform` that takes a `std::vector<float>& signal` and applies the Discrete Hartley Transform (DHT) in-place, using the same output scaling as the forward transform in the provided snippet (i.e., divide by `n`). Your function must handle only power-of-two lengths (assert this precondition). The function should modify the input vector directly, and for a vector `v` with `n = v.size()`, after calling the function, `v` must satisfy `v[i] = (1/n) * sum_{k=0}^{n-1} original[k] * (cos(2πik/n) + sin(2πik/n))` for each `i`. Your implementation must not use any external FFT library; you must implement the recursive decimation-in-time algorithm directly, as in the reference snippet. Ensure that the transform is numerically stable for `n` up to at least 1024 with `float` precision, and that applying your forward transform followed by an inverse transform (which is the same function but without division by `n`) returns the original signal with tolerance `1e-3` for values in `[-100, 100]`.
#include <cassert>
#include <vector>
#include <cmath>
#include <iostream>

// Forward declaration of the solution function
void inPlaceHartleyTransform(std::vector<float>& signal);

// Inverse Hartley: same as forward but without scaling (scale=1)
void inverseHartleyTransform(std::vector<float>& signal) {
    int n = signal.size();
    std::vector<std::complex<float>> x(n);
    for (int i = 0; i < n; ++i) x[i] = std::complex<float>(signal[i], 0.0f);
    // We need the same internal FFT; easiest is to replicate the logic or call a helper.
    // For test simplicity, we'll just re-invoke the same forward logic with scale=1.
    // But since the solution only provides forward, we'll implement a small internal helper here.
    // For brevity, we'll use the same recursion as in the solution.
    static void fft(std::vector<std::complex<float>>& a, bool inv) {
        int n = a.size();
        if (n <= 1) return;
        std::vector<std::complex<float>> even(n/2), odd(n/2);
        for (int i = 0; i < n/2; ++i) {
            even[i] = a[2*i];
            odd[i] = a[2*i+1];
        }
        fft(even, inv);
        fft(odd, inv);
        float ang = 2 * M_PI / n * (inv ? 1.0f : -1.0f);
        std::complex<float> w(1,0), step(cos(ang), sin(ang));
        for (int k = 0; k < n/2; ++k) {
            std::complex<float> t = w * odd[k];
            a[k] = even[k] + t;
            a[k + n/2] = even[k] - t;
            w *= step;
        }
    }
    // Need forward FFT (no inverse) then convert without scaling.
    fft(x, false);
    for (int i = 0; i < n; ++i) {
        signal[i] = x[i].real() - x[i].imag();
    }
}

int main() {
    // Test 1: Single element
    std::vector<float> v1{5.0f};
    inPlaceHartleyTransform(v1);
    assert(std::fabs(v1[0] - 5.0f) < 1e-6);

    // Test 2: Two-element sequence {0,1}
    std::vector<float> v2{0.0f, 1.0f};
    inPlaceHartleyTransform(v2);
    // Expected: H[0] = (0+1)/2 = 0.5, H[1] = (0-1)/2 = -0.5
    assert(std::fabs(v2[0] - 0.5f) < 1e-6);
    assert(std::fabs(v2[1] + 0.5f) < 1e-6);

    // Test 3: Four-element sequence {1,2,3,4}
    std::vector<float> v3{1.0f, 2.0f, 3.0f, 4.0f};
    inPlaceHartleyTransform(v3);
    // Expected values computed manually (using DHT definition):
    // H[0] = (1+2+3+4)/4 = 2.5
    // H[1] = (1*cos0 + 2*cos(π/2)+3*cosπ+4*cos(3π/2) + 1*sin0+2*sin(π/2)+3*sinπ+4*sin(3π/2)) / 4
    //      = (1+0-3+0 + 0+2+0-4)/4 = (-4)/4 = -1
    // H[2] = (1*cos0 + 2*cosπ + 3*cos0 + 4*cosπ + 1*sin0+2*sinπ+3*sin0+4*sinπ)/4 = (1-2+3-4)/4 = -0.5
    // H[3] = similar to H[1] but with opposite signs? Let's compute: (1*cos0+2*cos(3π/2)+3*cosπ+4*cos(π/2) + 1*sin0+2*sin(3π/2)+3*sinπ+4*sin(π/2))/4
    //      = (1+0-3+0 + 0-2+0+4)/4 = 0/4 = 0
    assert(std::fabs(v3[0] - 2.5f) < 1e-5);
    assert(std::fabs(v3[1] + 1.0f) < 1e-5);
    assert(std::fabs(v3[2] + 0.5f) < 1e-5);
    assert(std::fabs(v3[3] - 0.0f) < 1e-5);

    // Test 4: Round-trip with random values of length 8
    std::vector<float> original{0.5f, -1.0f, 2.0f, 3.0f, -4.0f, 5.0f, 6.0f, -7.0f};
    std::vector<float> copy = original;
    inPlaceHartleyTransform(copy);
    inverseHartleyTransform(copy);
    for (size_t i = 0; i < original.size(); ++i) {
        assert(std::fabs(copy[i] - original[i]) < 1e-3);
    }

    // Test 5: All zeros
    std::vector<float> v5(16, 0.0f);
    inPlaceHartleyTransform(v5);
    for (float f : v5) assert(f == 0.0f);

    // Test 6: Impulse at index 0, length 16
    std::vector<float> v6(16, 0.0f);
    v6[0] = 16.0f;
    inPlaceHartleyTransform(v6);
    // The Hartley transform of an impulse at 0 is constant 1.0 for all indices (since scaled by 1/n)
    for (float f : v6) assert(std::fabs(f - 1.0f) < 1e-5);

    // Test 7: Length 32, compare with direct computation for a few indices
    std::vector<float> v7(32);
    for (int i = 0; i < 32; ++i) v7[i] = static_cast<float>(i - 16);
    std::vector<float> original7 = v7;
    inPlaceHartleyTransform(v7);
    // Direct compute H[0] and H[1]
    int n = 32;
    float direct0 = 0.0f, direct1 = 0.0f;
    for (int k = 0; k < n; ++k) {
        direct0 += original7[k];
        direct1 += original7[k] * (std::cos(2 * M_PI * 1 * k / n) + std::sin(2 * M_PI * 1 * k / n));
    }
    direct0 /= n;
    direct1 /= n;
    assert(std::fabs(v7[0] - direct0) < 1e-4);
    assert(std::fabs(v7[1] - direct1) < 1e-4);

    std::cout << "All tests passed.\n";
    return 0;
}
#include <vector>
#include <complex>
#include <cmath>
#include <cassert>

// Internal recursive FFT that computes the DFT in-place on a complex vector.
// The vector length must be a power of two.
static void fft_recursive(std::vector<std::complex<float>>& a, bool inverse) {
    int n = a.size();
    if (n <= 1) return;

    // Split into even and odd indices
    std::vector<std::complex<float>> even(n/2), odd(n/2);
    for (int i = 0; i < n/2; ++i) {
        even[i] = a[2*i];
        odd[i] = a[2*i + 1];
    }

    fft_recursive(even, inverse);
    fft_recursive(odd, inverse);

    float angle = 2 * M_PI / n * (inverse ? 1.0f : -1.0f);
    std::complex<float> w(1.0f, 0.0f);
    std::complex<float> step(std::cos(angle), std::sin(angle));

    for (int k = 0; k < n/2; ++k) {
        std::complex<float> t = w * odd[k];
        a[k] = even[k] + t;
        a[k + n/2] = even[k] - t;
        w *= step;
    }
}

// Applies the Discrete Hartley Transform (DHT) in-place on a vector of floats.
// The vector length must be a power of two. The transform is scaled by 1/n.
void inPlaceHartleyTransform(std::vector<float>& signal) {
    int n = signal.size();
    assert(n > 0 && (n & (n - 1)) == 0);

    // Prepare complex vector: real part from signal, imaginary part 0
    std::vector<std::complex<float>> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = std::complex<float>(signal[i], 0.0f);
    }

    // Compute FFT (forward: inverse = false)
    fft_recursive(x, false);

    // Convert FFT to Hartley, with scaling
    float scale = 1.0f / n;
    for (int i = 0; i < n; ++i) {
        float real_part = x[i].real() * scale;
        float imag_part = x[i].imag() * scale;
        signal[i] = real_part - imag_part;
    }
}
// The core algorithm is a recursive radix-2 Cooley–Tukey style FFT adapted to compute the Hartley transform. The provided `fft0` function computes an in-place FFT (with complex output), but the snippet uses it by placing real values as complex numbers with zero imaginary part, then combining the real and imaginary parts of the FFT output to obtain the Hartley transform: `H[i] = Re(F[i]) - Im(F[i])`, where `F` is the FFT of the input (with standard normalization). However, note that the snippet's `fft0` is unusual: it uses a stride-based recursion and swaps output buffers, and it applies twiddle factors `w_p = cos(θ) - i sin(θ)` on the combination `(a - b)`, which yields the standard FFT of the input (but with a sign convention that matches the Hartley transform after combining). The forward DHT divides by `n`, while the inverse does not.
//
// For our independent function, we will implement a clean recursive FFT (similar in spirit) that computes the complex DFT, then convert to Hartley output. To keep the function self-contained and avoid needing complex vectors, we can either use `std::complex<float>` (as in the snippet) or implement with pairs of floats. Using `std::complex` is simpler and matches the provided code. The recursion splits the sequence into even and odd indices, computes FFT of half-size, then combines with twiddle factors. The base case `n=1` (or `n=2`) is handled directly. We must be careful with the stride and output buffer swapping to produce correct in-place behavior. However, for our task, we can write a simpler recursive FFT that takes a complex vector and modifies it in place using the classic iterative or recursive structure, then convert to Hartley. But to honor the "in-place" requirement on the input `float` vector, we'll allocate temporary complex vectors internally, compute the FFT, then overwrite the input `float` vector with the Hartley values. This still satisfies "in-place" from the caller's perspective because the input vector is modified directly.
//
// Edge cases: `n=1` returns the same value (since Hartley of a single value is itself divided by 1). `n` must be a power of two; assert this. For `n=0`, assert `n>0`. Numerical stability: using `std::cos` and `std::sin` for each twiddle factor; we can precompute them in a loop. Time complexity is `O(n log n)` for the FFT, and space is `O(n)` for temporary arrays. The conversion loop is `O(n)`. The inverse transform is identical but without dividing by `n`, so we will provide an auxiliary function `inverseHartleyTransform` or add a parameter `scale` to handle both. The task specifically requires only the forward transform, so we'll implement `inPlaceHartleyTransform` that divides by `n`. For testing, we'll also implement an inverse version in the test code by calling the same internal function with scale=1, but the solution function itself must match the specification.
