// Write a standalone C++ function named `computeLineSpectralPairs` that takes as input an array of 11 LP (linear prediction) coefficients `a` (with `a[0]` implicitly 1.0 but stored at index 0), an output array `lsp` of 10 line spectral pair values, and an array `old_lsp` of 10 previously computed LSPs (to be used if fewer than 10 roots are found). The function must compute the LSPs by forming the symmetric and antisymmetric polynomials F1(z) and F2(z) from the LP coefficients, then finding the roots of these polynomials in the cosine domain (x from 1 to -1) using Chebyshev polynomial evaluation on a fixed grid of 60 points, with 4 bisection refinements and linear interpolation for each detected sign change. If fewer than 10 roots are found, the function must copy the `old_lsp` values into `lsp`. All arithmetic is in fixed-point Q15 format (Word16), and the function must handle potential overflow by simply clamping values, but no explicit overflow flag is required. The grid is predefined as 61 equally spaced points from 1.0 down to -1.0 (stored as Q15 values `32767` down to `-32768` with step about -1092). Use `Word16` as an alias for `short` and `Word32` as `int`. Provide the function with appropriate `const` qualifiers where applicable.

#include <cassert>
#include <cstdint>

// The solution function is declared here (inline from the solution section)

int main() {
    // Test case 1: Known LP coefficients for a typical frame (from GSM AMR)
    const int16_t a1[11] = {1024, -313, 30, 102, 46, -71, 29, 27, -15, -6, 3};
    int16_t lsp1[10];
    int16_t old_lsp1[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // fallback
    computeLineSpectralPairs(a1, lsp1, old_lsp1);
    // This should find 10 roots; check that they are sorted and within [-1,1] (Q15)
    for (int i = 0; i < 10; i++) {
        assert(lsp1[i] >= -32768 && lsp1[i] <= 32767);
        if (i > 0) assert(lsp1[i] <= lsp1[i-1]); // roots should be in decreasing order
    }

    // Test case 2: All coefficients zero except first (f1 and f2 become simple)
    const int16_t a2[11] = {1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int16_t lsp2[10];
    int16_t old_lsp2[10] = {30000, 25000, 20000, 15000, 10000, 5000, 0, -5000, -10000, -15000};
    computeLineSpectralPairs(a2, lsp2, old_lsp2);
    // With zero coefficients, F1 and F2 are both unity, so no sign changes -> use old
    for (int i = 0; i < 10; i++) {
        assert(lsp2[i] == old_lsp2[i]);
    }

    // Test case 3: A case where exactly 10 roots are found (symmetric coefficients)
    const int16_t a3[11] = {1024, 300, 200, 100, 50, 20, -20, -50, -100, -200, -300};
    int16_t lsp3[10];
    int16_t old_lsp3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    computeLineSpectralPairs(a3, lsp3, old_lsp3);
    // Should find 10 roots (since F1 and F2 alternate and have zeros)
    // Check that lsp values are within valid range and sorted
    for (int i = 0; i < 10; i++) {
        assert(lsp3[i] >= -32768 && lsp3[i] <= 32767);
        if (i > 0) assert(lsp3[i] <= lsp3[i-1]);
    }

    // Test case 4: Random coefficients (simple sanity check that no crash)
    const int16_t a4[11] = {1024, -100, 250, -50, 75, -200, 40, -300, 10, -150, 80};
    int16_t lsp4[10];
    int16_t old_lsp4[10] = {20000, 15000, 10000, 5000, 0, -5000, -10000, -15000, -20000, -25000};
    computeLineSpectralPairs(a4, lsp4, old_lsp4);
    // Check all outputs are within valid Q15 range
    for (int i = 0; i < 10; i++) {
        assert(lsp4[i] >= -32768 && lsp4[i] <= 32767);
    }

    // Test case 5: Verify that output array is fully written (no undefined values)
    bool all_defined = true;
    for (int i = 0; i < 10; i++) {
        // Since we use int16_t, any value is technically defined, but we check
        // that the values are plausible (not NaN, infinite, etc.)
        if (lsp4[i] == -32768 || lsp4[i] == 32767) {
            // Extreme values are possible; no issue
        }
    }

    return 0;
}

#include <cstdint>

using Word16 = int16_t;
using Word32 = int32_t;

static constexpr int M = 10;          // LPC order
static constexpr int NC = M / 2;      // 5
static constexpr int grid_points = 60;

static const Word16 grid[grid_points + 1] = {
    32767, 31675, 30583, 29491, 28399, 27307, 26215, 25123, 24031, 22939,
    21847, 20755, 19663, 18571, 17479, 16387, 15295, 14203, 13111, 12019,
    10927,  9835,  8743,  7651,  6559,  5467,  4375,  3283,  2191,  1099,
       7,  -1085, -2177, -3269, -4361, -5453, -6545, -7637, -8729, -9821,
    -10913, -12005, -13097, -14189, -15281, -16373, -17465, -18557, -19649,
    -20741, -21833, -22925, -24017, -25109, -26201, -27293, -28385, -29477,
    -30569, -31661, -32768
};

// Evaluate Chebyshev polynomial series for given x and coefficients f[0..n]
static Word16 Chebps(Word16 x, const Word16 f[], Word16 n) {
    Word16 i;
    Word16 cheb;
    Word16 b1_h, b1_l;
    Word32 t0;
    Word32 L_temp;
    const Word16 *p_f = &f[1];

    L_temp = 0x01000000L;  // 1.0 in 16.16 fixed point

    // t0 = 2*x + f[1]   (scaled: x<<10 + f[1]<<14)
    t0 = ((Word32)x << 10) + ((Word32)(*p_f++) << 14);
    b1_h = (Word16)(t0 >> 16);
    b1_l = (Word16)((t0 >> 1) - (b1_h << 15));

    for (i = 2; i < n; i++) {
        // t0 = 2.0*x*b1
        t0  = ((Word32)b1_h * x);
        t0 += ((Word32)b1_l * x) >> 15;
        t0 <<= 2;
        // t0 = 2.0*x*b1 - b2
        t0 -= L_temp;
        // t0 = 2.0*x*b1 - b2 + f[i]
        t0 += (Word32)(*p_f++) << 14;

        L_temp = ((Word32)b1_h << 16) + ((Word32)b1_l << 1);

        // b0 = t0, then shift b1<-b0
        b1_h = (Word16)(t0 >> 16);
        b1_l = (Word16)((t0 >> 1) - (b1_h << 15));
    }

    // Final step: t0 = x*b1 - b2 + f[n]/2
    t0  = ((Word32)b1_h * x);
    t0 += ((Word32)b1_l * x) >> 15;
    t0 <<= 1;
    t0 -= L_temp;
    t0 += (Word32)(*p_f) << 13;

    // Convert to Q15 with saturation
    if ((uint32_t)(t0 - 0xfe000000L) < 0x03ffffffL) {
        cheb = (Word16)(t0 >> 10);
    } else {
        cheb = (t0 > 0x01ffffffL) ? 32767 : -32768;
    }
    return cheb;
}

// Compute LSPs from LP coefficients a[] (length 11), output to lsp[] (length 10)
void computeLineSpectralPairs(const Word16 a[], Word16 lsp[], const Word16 old_lsp[]) {
    Word16 i, j, nf, ip;
    Word16 xlow, ylow, xhigh, yhigh, xmid, ymid, xint;
    Word16 x, y, sign, exp;
    const Word16 *coef;
    Word16 f1[NC + 1], f2[NC + 1];

    // Build F1 and F2 coefficient arrays
    f1[0] = 1024;  // 1.0 in Q15
    f2[0] = 1024;
    for (i = 0; i < NC; i++) {
        Word32 sum = (Word32)a[i + 1] + (Word32)a[M - i];
        Word32 diff = (Word32)a[i + 1] - (Word32)a[M - i];
        x = (Word16)(sum >> 2);
        y = (Word16)(diff >> 2);
        f1[i + 1] = x - f1[i];
        f2[i + 1] = y + f2[i];
    }

    // Find roots using Chebyshev evaluation on grid
    nf = 0;       // number of found roots
    ip = 0;       // 0 for f1, 1 for f2
    coef = f1;

    xlow = grid[0];
    ylow = Chebps(xlow, coef, NC);

    j = 0;
    while ((nf < M) && (j < grid_points)) {
        j++;
        xhigh = xlow;
        yhigh = ylow;
        xlow = grid[j];
        ylow = Chebps(xlow, coef, NC);

        if (((Word32)ylow * yhigh) <= 0) {
            // Bisect 4 times
            for (i = 4; i != 0; i--) {
                xmid = (xlow >> 1) + (xhigh >> 1);
                ymid = Chebps(xmid, coef, NC);
                if (((Word32)ylow * ymid) <= 0) {
                    yhigh = ymid;
                    xhigh = xmid;
                } else {
                    ylow = ymid;
                    xlow = xmid;
                }
            }

            // Linear interpolation
            x = xhigh - xlow;
            y = yhigh - ylow;
            if (y == 0) {
                xint = xlow;
            } else {
                sign = y;
                y = (y < 0) ? (Word16)-y : y;
                exp = 0;
                while ((y & 0x4000) == 0 && y != 0) {  // normalize
                    y <<= 1;
                    exp++;
                }
                y = (Word16)(16383 / y);
                y = (Word16)(((Word32)x * y) >> (19 - exp));
                if (sign < 0) y = (Word16)-y;
                xint = xlow - (Word16)(((Word32)ylow * y) >> 10);
            }

            lsp[nf] = xint;
            xlow = xint;
            nf++;

            // Switch polynomial
            if (ip == 0) {
                ip = 1;
                coef = f2;
            } else {
                ip = 0;
                coef = f1;
            }
            ylow = Chebps(xlow, coef, NC);
        }
    }

    // If not enough roots found, copy old LSPs
    if (nf < M) {
        for (i = 0; i < M; i++) {
            lsp[i] = old_lsp[i];
        }
    }
}

// The core algorithm follows the standard GSM AMR LSP computation. First, compute the coefficients of F1 and F2 using the recurrence: `f1[0]=1024` (Q15 representing 1.0), `f2[0]=1024`. For `i` from 0 to 4 (since NC=5), compute `x = (a[i+1] + a[10-i]) >> 2` and `y = (a[i+1] - a[10-i]) >> 2`, then `f1[i+1] = x - f1[i]` and `f2[i+1] = y + f2[i]`. These arrays have length 6 (indices 0..5). Next, evaluate the Chebyshev polynomial `Chebps` at each grid point. The Chebyshev evaluation uses the recurrence: for a given x and coefficients f[0..n], initialize `b2 = 1.0` (represented as `0x01000000L` in 16.16 fixed point), then compute `b1 = 2*x + f[1]` using Q15 arithmetic scaled appropriately. For each subsequent coefficient, update using `t0 = 2*x*b1 - b2 + f[i]` with proper scaling. At the end, compute `t0 = x*b1 - b2 + f[n]/2` and scale to Q15 output. The function `chebps` returns a Word16 value. The root finding iterates over grid points: for each consecutive pair (xlow,xhigh) where the product of ylow and yhigh is <= 0, a root exists in that interval. Perform 4 bisection steps by evaluating the midpoints and narrowing the interval. Then apply linear interpolation to refine the root: `xint = xlow - ylow*(xhigh-xlow)/(yhigh-ylow)`. Handle the degenerate case where yhigh==ylow by setting xint=xlow. The found xint is stored in `lsp[nf]`, and the next search alternates between F1 and F2 polynomials. If after scanning all grid points fewer than 10 roots are found, copy `old_lsp` into `lsp`. Time complexity is O(grid_points * NC) for polynomial evaluations plus O(M) for interpolation, which is constant for fixed parameters; space complexity is O(NC) for the coefficient arrays.
