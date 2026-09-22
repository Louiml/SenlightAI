Given an array `v` of `n` complex numbers stored in the fixed-point format where each 32-bit integer packs the real part in the high 16 bits and the imaginary part in the low 16 bits (using two's complement for negative parts), write a C++ function `void fixed_fft_custom(int n, int32_t *v)` that performs an in-place radix-2 Cooley-Tukey Fast Fourier Transform. The function must handle power-of-two sizes up to 1024, use bit-reversal reordering first, then combine butterfly stages, preserving the fixed-point layout throughout. Scaling must be applied via arithmetic right shifts (or equivalent) to avoid overflow, and the twiddle factors must be obtained by indexing into a provided constant table (which stores only the first quadrant of the unit circle) using a computed index that may require handling negative indices via conditional addition and bitwise inversion. The transform must be self-contained, not rely on any external library, and work correctly for both positive and negative values in either component of the input complex numbers.

// The core algorithm is the iterative radix-2 FFT. First, perform bit-reversal permutation of the input array so that subsequent butterfly operations operate in-place with contiguous index pairs. The number of stages is `log2(n)`, and at each stage, we process `n/(2*p)` groups, each containing a butterfly between indices `i` and `i+p`. For the first butterfly in each group (`r=0`), the twiddle factor is 1, so we can compute directly with simple addition/subtraction after halving both inputs to prevent overflow. For subsequent butterflies (`r>0`), we need a complex multiplication: given twiddle `w = (wr, wi)` and input `b = (br, bi)`, the product `(wr*br - wi*bi, wr*bi + wi*br)` must be computed with 16-bit signed components. Use a helper function `mult` that performs this using 32-bit arithmetic and masks to extract the high and low 16-bit halves. The twiddle table stores only angles from 0 to 90 degrees in steps of `2*pi/1024`. For a given stage and `r`, the required angle is `-2*pi*r / (2*p)`; index into the table using `(MAX_SIZE/4 - (r << scale))`, where `scale` decreases by 1 each stage starting from `LOG_FFT_SIZE`. Negative indices are handled by masking and conditional negation. Edge cases include `n=1` (no work), `n=2` (single stage, no twiddles), and extreme values that require halving at each butterfly to avoid 32-bit overflow. Time complexity is O(n log n), space complexity O(1) beyond input array.

#include <stdint.h>

// Fixed-point FFT for power-of-two sizes up to 1024.
// Each complex is packed as (real<<16) | (imag) with signed 16-bit halves.
// Uses in-place processing, bit-reversal, and a precomputed twiddle table.
// Scaling is applied via arithmetic right shifts to prevent overflow.

#define LOG_FFT_SIZE_MAX 10
#define MAX_FFT_SIZE_MAX (1 << LOG_FFT_SIZE_MAX)

// Twiddle table: stores cos/sin for angles 0..90 degrees
// in steps of 2*pi/1024, packed as (cos<<16)|sin.
static const uint32_t twiddle_fixed[MAX_FFT_SIZE_MAX / 4] = {
    0x00008000, 0xff378001, 0xfe6e8002, 0xfda58006, 0xfcdc800a, 0xfc13800f,
    0xfb4a8016, 0xfa81801e, 0xf9b88027, 0xf8ef8032, 0xf827803e, 0xf75e804b,
    0xf6958059, 0xf5cd8068, 0xf5058079, 0xf43c808b, 0xf374809e, 0xf2ac80b2,
    0xf1e480c8, 0xf11c80de, 0xf05580f6, 0xef8d8110, 0xeec6812a, 0xedff8146,
    0xed388163, 0xec718181, 0xebab81a0, 0xeae481c1, 0xea1e81e2, 0xe9588205,
    0xe892822a, 0xe7cd824f, 0xe7078276, 0xe642829d, 0xe57d82c6, 0xe4b982f1,
    0xe3f4831c, 0xe3308349, 0xe26d8377, 0xe1a983a6, 0xe0e683d6, 0xe0238407,
    0xdf61843a, 0xde9e846e, 0xdddc84a3, 0xdd1b84d9, 0xdc598511, 0xdb998549,
    0xdad88583, 0xda1885be, 0xd95885fa, 0xd8988637, 0xd7d98676, 0xd71b86b6,
    0xd65c86f6, 0xd59e8738, 0xd4e1877b, 0xd42487c0, 0xd3678805, 0xd2ab884c,
    0xd1ef8894, 0xd13488dd, 0xd0798927, 0xcfbe8972, 0xcf0489be, 0xce4b8a0c,
    0xcd928a5a, 0xccd98aaa, 0xcc218afb, 0xcb698b4d, 0xcab28ba0, 0xc9fc8bf5,
    0xc9468c4a, 0xc8908ca1, 0xc7db8cf8, 0xc7278d51, 0xc6738dab, 0xc5c08e06,
    0xc50d8e62, 0xc45b8ebf, 0xc3a98f1d, 0xc2f88f7d, 0xc2488fdd, 0xc198903e,
    0xc0e990a1, 0xc03a9105, 0xbf8c9169, 0xbedf91cf, 0xbe329236, 0xbd86929e,
    0xbcda9307, 0xbc2f9371, 0xbb8593dc, 0xbadc9448, 0xba3394b5, 0xb98b9523,
    0xb8e39592, 0xb83c9603, 0xb7969674, 0xb6f196e6, 0xb64c9759, 0xb5a897ce,
    0xb5059843, 0xb46298b9, 0xb3c09930, 0xb31f99a9, 0xb27f9a22, 0xb1df9a9c,
    0xb1409b17, 0xb0a29b94, 0xb0059c11, 0xaf689c8f, 0xaecc9d0e, 0xae319d8e,
    0xad979e0f, 0xacfd9e91, 0xac659f14, 0xabcd9f98, 0xab36a01c, 0xaaa0a0a2,
    0xaa0aa129, 0xa976a1b0, 0xa8e2a238, 0xa84fa2c2, 0xa7bda34c, 0xa72ca3d7,
    0xa69ca463, 0xa60ca4f0, 0xa57ea57e, 0xa4f0a60c, 0xa463a69c, 0xa3d7a72c,
    0xa34ca7bd, 0xa2c2a84f, 0xa238a8e2, 0xa1b0a976, 0xa129aa0a, 0xa0a2aaa0,
    0xa01cab36, 0x9f98abcd, 0x9f14ac65, 0x9e91acfd, 0x9e0fad97, 0x9d8eae31,
    0x9d0eaecc, 0x9c8faf68, 0x9c11b005, 0x9b94b0a2, 0x9b17b140, 0x9a9cb1df,
    0x9a22b27f, 0x99a9b31f, 0x9930b3c0, 0x98b9b462, 0x9843b505, 0x97ceb5a8,
    0x9759b64c, 0x96e6b6f1, 0x9674b796, 0x9603b83c, 0x9592b8e3, 0x9523b98b,
    0x94b5ba33, 0x9448badc, 0x93dcbb85, 0x9371bc2f, 0x9307bcda, 0x929ebd86,
    0x9236be32, 0x91cfbedf, 0x9169bf8c, 0x9105c03a, 0x90a1c0e9, 0x903ec198,
    0x8fddc248, 0x8f7dc2f8, 0x8f1dc3a9, 0x8ebfc45b, 0x8e62c50d, 0x8e06c5c0,
    0x8dabc673, 0x8d51c727, 0x8cf8c7db, 0x8ca1c890, 0x8c4ac946, 0x8bf5c9fc,
    0x8ba0cab2, 0x8b4dcb69, 0x8afbcc21, 0x8aaaccd9, 0x8a5acd92, 0x8a0cce4b,
    0x89becf04, 0x8972cfbe, 0x8927d079, 0x88ddd134, 0x8894d1ef, 0x884cd2ab,
    0x8805d367, 0x87c0d424, 0x877bd4e1, 0x8738d59e, 0x86f6d65c, 0x86b6d71b,
    0x8676d7d9, 0x8637d898, 0x85fad958, 0x85beda18, 0x8583dad8, 0x8549db99,
    0x8511dc59, 0x84d9dd1b, 0x84a3dddc, 0x846ede9e, 0x843adf61, 0x8407e023,
    0x83d6e0e6, 0x83a6e1a9, 0x8377e26d, 0x8349e330, 0x831ce3f4, 0x82f1e4b9,
    0x82c6e57d, 0x829de642, 0x8276e707, 0x824fe7cd, 0x822ae892, 0x8205e958,
    0x81e2ea1e, 0x81c1eae4, 0x81a0ebab, 0x8181ec71, 0x8163ed38, 0x8146edff,
    0x812aeec6, 0x8110ef8d, 0x80f6f055, 0x80def11c, 0x80c8f1e4, 0x80b2f2ac,
    0x809ef374, 0x808bf43c, 0x8079f505, 0x8068f5cd, 0x8059f695, 0x804bf75e,
    0x803ef827, 0x8032f8ef, 0x8027f9b8, 0x801efa81, 0x8016fb4a, 0x800ffc13,
    0x800afcdc, 0x8006fda5, 0x8002fe6e, 0x8001ff37,
};

// Multiply two complex numbers a and b, returning (conj(a)*b) in fixed-point.
// Each complex is 32-bit with real in high 16, imag in low 16.
static inline int32_t mult_fixed(int32_t a, int32_t b) {
    int32_t ar = a >> 16;
    int32_t ai = (int16_t)a;
    int32_t br = b >> 16;
    int32_t bi = (int16_t)b;

    int32_t real = (ar * br + ai * bi) & ~0xFFFF;  // keep high 16 bits
    int32_t imag = ((ar * bi - ai * br) >> 16) & 0xFFFF; // keep low 16 bits
    return real | imag;
}

// Half each 16-bit component, rounding toward zero, preserving sign bits.
static inline int32_t half_fixed(int32_t a) {
    int32_t ar = a >> 1;
    int32_t ai = (int16_t)a >> 1;
    // Preserve original sign bits in high and low halves.
    int32_t mask = 0x80008000;
    return (ar & ~mask) | (a & mask) | (ai & 0x0000FFFF);
}

// Perform in-place fixed-point FFT for n complex samples.
// n must be a power of two between 1 and 1024.
void fixed_fft_custom(int n, int32_t *v) {
    if (n <= 1) return;

    int scale = LOG_FFT_SIZE_MAX;
    int i, p, r;

    // Bit-reversal permutation
    for (r = 0, i = 1; i < n; ++i) {
        for (p = n; !(p & r); p >>= 1, r ^= p);
        if (i < r) {
            int32_t t = v[i];
            v[i] = v[r];
            v[r] = t;
        }
    }

    // Butterfly stages
    for (p = 1; p < n; p <<= 1) {
        --scale;

        // First butterfly in each group (twiddle = 1)
        for (i = 0; i < n; i += p << 1) {
            int32_t x = half_fixed(v[i]);
            int32_t y = half_fixed(v[i + p]);
            v[i] = x + y;
            v[i + p] = x - y;
        }

        // Remaining butterflies with twiddle factors
        for (r = 1; r < p; ++r) {
            int32_t w_index = MAX_FFT_SIZE_MAX / 4 - (r << scale);
            // Handle negative indices by wrapping into table with sign inversion
            int32_t sign_adjust = w_index >> 31;
            w_index = (w_index ^ sign_adjust) - sign_adjust; // abs
            int32_t w = (int32_t)twiddle_fixed[w_index];
            // Negate imaginary part if original index was negative
            if (sign_adjust) {
                int32_t real = w & 0xFFFF0000;
                int32_t imag = (int16_t)w;
                imag = -imag;
                w = real | (imag & 0xFFFF);
            }

            for (i = r; i < n; i += p << 1) {
                int32_t x = half_fixed(v[i]);
                int32_t y = mult_fixed(w, v[i + p]);
                v[i] = x - y;
                v[i + p] = x + y;
            }
        }
    }
}

#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

// Helper to pack real and imag (each -32768..32767) into int32
static int32_t pack(int16_t real, int16_t imag) {
    return ((int32_t)real << 16) | (uint16_t)imag;
}

// Reference DFT for small sizes
static void reference_dft(int n, int32_t *input, int32_t *output) {
    for (int k = 0; k < n; ++k) {
        double real_sum = 0, imag_sum = 0;
        for (int t = 0; t < n; ++t) {
            int16_t real_in = (int16_t)(input[t] >> 16);
            int16_t imag_in = (int16_t)(input[t] & 0xFFFF);
            double angle = -2.0 * M_PI * k * t / n;
            double c = cos(angle), s = sin(angle);
            real_sum += real_in * c - imag_in * s;
            imag_sum += real_in * s + imag_in * c;
        }
        // Scale down by n/2? Actually reference DFT is unscaled; FFT uses halving per stage.
        // We'll compare after scaling reference by (1/2^log2(n)) to match our halving.
        int scale_factor = 1 << (int)(log(n)/log(2));
        int16_t real_out = (int16_t)lround(real_sum / scale_factor);
        int16_t imag_out = (int16_t)lround(imag_sum / scale_factor);
        output[k] = pack(real_out, imag_out);
    }
}

int main() {
    // Test with n=4 and simple sequence
    {
        int32_t v[4] = {
            pack(1, 0),
            pack(0, 1),
            pack(-1, 0),
            pack(0, -1)
        };
        int32_t expected[4];
        int32_t input_copy[4];
        for (int i=0;i<4;i++) input_copy[i] = v[i];
        reference_dft(4, input_copy, expected);
        fixed_fft_custom(4, v);
        // Allow small rounding differences of +/-1 in each component
        for (int i=0;i<4;i++) {
            int16_t re = (int16_t)(v[i] >> 16);
            int16_t im = (int16_t)(v[i] & 0xFFFF);
            int16_t re_exp = (int16_t)(expected[i] >> 16);
            int16_t im_exp = (int16_t)(expected[i] & 0xFFFF);
            assert(abs(re - re_exp) <= 1);
            assert(abs(im - im_exp) <= 1);
        }
    }

    // Test n=2 with real-only input
    {
        int32_t v[2] = { pack(100, 0), pack(200, 0) };
        // FFT of [100,200] with halving: stage1: x=50, y=100 -> [150, -50]
        assert(v[0] == pack(150, 0));
        assert(v[1] == pack(-50, 0));
    }

    // Test n=1 (should do nothing)
    {
        int32_t v[1] = { pack(7, -3) };
        fixed_fft_custom(1, v);
        assert(v[0] == pack(7, -3));
    }

    // Test n=8 with randomish values
    {
        int32_t v[8];
        for (int i=0;i<8;i++) {
            int16_t r = (int16_t)(i*10 - 20); // -20, -10, 0, 10, 20, 30, 40, 50
            int16_t im = (int16_t)(i*3 - 7);  // -7, -4, -1, 2, 5, 8, 11, 14
            v[i] = pack(r, im);
        }
        int32_t input_copy[8];
        for (int i=0;i<8;i++) input_copy[i] = v[i];
        int32_t expected[8];
        reference_dft(8, input_copy, expected);
        fixed_fft_custom(8, v);
        for (int i=0;i<8;i++) {
            int16_t re = (int16_t)(v[i] >> 16);
            int16_t im = (int16_t)(v[i] & 0xFFFF);
            int16_t re_exp = (int16_t)(expected[i] >> 16);
            int16_t im_exp = (int16_t)(expected[i] & 0xFFFF);
            assert(abs(re - re_exp) <= 2); // allow slightly larger error for larger n
            assert(abs(im - im_exp) <= 2);
        }
    }

    // Test n=16 with DC-only signal
    {
        int32_t v[16];
        for (int i=0;i<16;i++) v[i] = pack(5, -2);
        fixed_fft_custom(16, v);
        // DC component should be (5*16)/(2^4)=5, imag=(-2*16)/(16)=-2
        assert(v[0] == pack(5, -2));
        // All others should be near zero
        for (int i=1;i<16;i++) {
            int16_t re = (int16_t)(v[i] >> 16);
            int16_t im = (int16_t)(v[i] & 0xFFFF);
            assert(re == 0 && im == 0);
        }
    }

    return 0;
}
