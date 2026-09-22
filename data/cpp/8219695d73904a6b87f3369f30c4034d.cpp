// Write a C++ function that performs an in-place iterative Fast Fourier Transform (FFT) on a vector of complex numbers, where the complex type is represented by a simple struct with `double x` (real part) and double `y` (imaginary part). The function must accept two arguments: a reference to the vector (whose size must be a power of two) and an integer flag `f` that is either `1` for forward transform or `-1` for inverse transform. When `f == -1`, the function must scale each element by dividing the real part by the vector length (leave the imaginary part unchanged). The function should implement the radix-2 Cooley-Tukey algorithm using bit-reversal permutation and iterative butterfly operations, without using recursion or external libraries (except `<cmath>` for `cos`, `sin`, `pi`). The vector element type `Cp` is defined as a struct with public members `double x`, `double y`. Overload operators for addition, subtraction, and multiplication between two `Cp` values and between `Cp` and a scalar double (where scalar multiplies both real and imaginary parts). The function must handle size 1 correctly (returns unchanged). Do not assume any specific input values; the function must work for any vector of `Cp` with power-of-two length. Provide only the function and necessary type definitions/helpers; do not include a `main`.
#include <cassert>
#include <cmath>
#include <vector>

// Include the solution code here (struct Cp, operators, fft function)

// Helper to compare doubles with tolerance
bool approxEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

// Helper to compare two Cp vectors with tolerance
bool approxEqualCp(const std::vector<Cp>& a, const std::vector<Cp>& b, double eps = 1e-6) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!approxEqual(a[i].x, b[i].x, eps) || !approxEqual(a[i].y, b[i].y, eps)) {
            return false;
        }
    }
    return true;
}

int main() {
    // Test 1: identity for size 1
    std::vector<Cp> a1 = {Cp(2.0, 3.0)};
    std::vector<Cp> a1copy = a1;
    fft(a1, 1);
    assert(approxEqualCp(a1, a1copy));

    // Test 2: forward then inverse returns original (size 2)
    std::vector<Cp> a2 = {Cp(1.0, 0.0), Cp(2.0, 0.0)};
    std::vector<Cp> original2 = a2;
    fft(a2, 1);
    // Known forward of [1,2] -> [3, -1] (since e^{-2*pi*i*k/2} alternates)
    assert(approxEqual(a2[0].x, 3.0) && approxEqual(a2[0].y, 0.0));
    assert(approxEqual(a2[1].x, -1.0) && approxEqual(a2[1].y, 0.0));
    fft(a2, -1);
    assert(approxEqualCp(a2, original2));

    // Test 3: forward then inverse for size 4 with complex inputs
    std::vector<Cp> a4 = {Cp(1.0, 1.0), Cp(2.0, -1.0), Cp(3.0, 0.0), Cp(-1.0, 2.0)};
    std::vector<Cp> orig4 = a4;
    fft(a4, 1);
    fft(a4, -1);
    assert(approxEqualCp(a4, orig4));

    // Test 4: small impulse (size 8) forward then inverse
    std::vector<Cp> a8(8, Cp(0.0, 0.0));
    a8[3] = Cp(5.0, 0.0); // impulse at index 3
    std::vector<Cp> orig8 = a8;
    fft(a8, 1);
    // All frequency components should have magnitude 5 (real+imag)
    for (size_t i = 0; i < 8; ++i) {
        double mag = std::sqrt(a8[i].x*a8[i].x + a8[i].y*a8[i].y);
        assert(approxEqual(mag, 5.0));
    }
    fft(a8, -1);
    assert(approxEqualCp(a8, orig8));

    // Test 5: verify linearity for size 2: FFT of [0,0] is [0,0]
    std::vector<Cp> zeros(2, Cp(0.0, 0.0));
    fft(zeros, 1);
    assert(approxEqual(zeros[0].x, 0.0) && approxEqual(zeros[0].y, 0.0));
    assert(approxEqual(zeros[1].x, 0.0) && approxEqual(zeros[1].y, 0.0));

    return 0;
}
#include <vector>
#include <cmath>

// Complex number type for FFT operations.
struct Cp {
    double x; // real part
    double y; // imaginary part

    Cp(double real = 0.0, double imag = 0.0) : x(real), y(imag) {}

    // Compound assignment operators for efficiency
    Cp& operator+=(const Cp& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Cp& operator-=(const Cp& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Cp& operator*=(const Cp& other) {
        double nx = x * other.x - y * other.y;
        double ny = x * other.y + y * other.x;
        x = nx;
        y = ny;
        return *this;
    }
};

// Free operators
inline Cp operator+(Cp a, const Cp& b) { return a += b; }
inline Cp operator-(Cp a, const Cp& b) { return a -= b; }
inline Cp operator*(Cp a, const Cp& b) { return a *= b; }
inline Cp operator*(Cp a, double scalar) { a.x *= scalar; a.y *= scalar; return a; }

/**
 * Performs in-place iterative FFT on a vector of Cp.
 * f = 1 for forward transform, f = -1 for inverse transform (with scaling).
 * The size of the vector must be a power of two.
 */
void fft(std::vector<Cp>& A, int f) {
    const int n = static_cast<int>(A.size());
    if (n <= 1) return;

    // Bit-reversal permutation
    for (int i = 0, j = 0; i < n; ++i) {
        if (i < j) std::swap(A[i], A[j]);
        for (int k = n >> 1; (j ^= k) < k; k >>= 1) {}
    }

    // Iterative butterfly stages
    for (int len = 1; len < n; len <<= 1) {
        const double angle = (f == 1 ? 2.0 : -2.0) * M_PI / (2 * len);
        Cp w_base(std::cos(angle), std::sin(angle));

        for (int start = 0; start < n; start += (len << 1)) {
            Cp w(1.0, 0.0);
            for (int k = 0; k < len; ++k) {
                Cp u = A[start + k];
                Cp v = A[start + k + len] * w;
                A[start + k] = u + v;
                A[start + k + len] = u - v;
                w = w * w_base;
            }
        }
    }

    // Inverse transform scaling
    if (f == -1) {
        const double inv_n = 1.0 / static_cast<double>(n);
        for (int i = 0; i < n; ++i) {
            A[i].x *= inv_n;
            // imaginary part is left unchanged per the original snippet's behavior
        }
    }
}
// The core algorithm is the standard iterative FFT. First, we perform bit-reversal permutation: for each index `i`, compute its bit-reversed index `j` and swap `A[i]` with `A[j]` if `i < j` to avoid unnecessary swaps. The iterative loop processes stages from length `1` up to `n/2`. For each stage, compute the twiddle factor `w = e^(2*pi*i*f / (2*len))` where `len` is the current sub-transform length, and `f` is the transform direction. For each block of size `2*len`, apply butterflies: for each offset `k` in `[0, len)`, compute `u = A[j+k]`, `v = A[j+k+len] * w^k`, then set `A[j+k] = u + v` and `A[j+k+len] = u - v`. After all stages, if `f == -1`, divide each real part by `n` (the total size). Edge cases: vector size 1 should return immediately (no loops). The vector size must be a power of two; assume the caller ensures that. The complex multiplication must handle real and imaginary parts correctly: `(a+bi)*(c+di) = (ac-bd) + (ad+bc)i`. Applying `const` correctness: the function takes `std::vector<Cp>&` (non-const because it mutates in place) and an `int` flag by value. Time complexity is O(n log n) for FFT, O(n) for bit reversal, so total O(n log n). Space complexity is O(1) extra (only a few local variables). The twiddle factor is computed incrementally via multiplication to avoid repeated trig calls inside the inner loop, but for clarity we may compute a base `w` per stage and then update `w0` by multiplying by `w` each iteration.
