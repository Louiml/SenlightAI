// Write a C++ function `float sinh_approx(float x)` that computes an approximation of the hyperbolic sine of `x` with high accuracy for normal `float` inputs, and handles special cases (NaN, infinity, overflow, underflow-to-zero) in a manner consistent with the IEEE 754 standard and typical `libm` behavior. For inputs with `|x| <= 0.078125`, the function must use a polynomial minimax approximation (degree 7 in `x`, i.e., odd powers up to 7) to achieve near-double-precision accuracy before casting back to `float`. For larger `|x| < 90`, use the exponential-based identity `sinh(x) = (e^x - e^(-x))/2` (you may implement a simplified `exp` for the task, but the reference solution will assume `std::exp` is available for simplicity). For `|x| >= 90`, return the correctly signed infinity (or `+/-FLT_MAX` under certain rounding modes) and set `errno` to `ERANGE` and raise `FE_OVERFLOW`. For NaN input, return a quiet NaN (e.g., `x + 1.0f` if `x` is a signaling NaN); for infinity, return the same infinity. The function must be `constexpr`-safe? No, but it should be free of side effects except for setting `errno`/floating-point exceptions as specified. Provide a self-contained implementation with suitable headers.
The main algorithm splits the input range into three parts:
1. **Tiny/very small |x| ≤ 0.078125**: For |x| ≤ 2^-26, return `x` (or `x + 0.25*x^3` for subnormal/zero to avoid underflow). For the rest of this range, use a degree-7 odd polynomial `x * P(x^2)` where `P(t) = 1 + t/6 + t^2/120 + t^3/5040` (coefficients from Taylor/Maximax), computed in `double` to reduce rounding error. For the specific value `|x| = 0x3a1285ff` (≈0.0005589), return `x` directly if rounding to nearest, because the polynomial would introduce a tiny error.
2. **Intermediate 0.078125 < |x| < 90**: Use `sinh(x) = (e^x - e^(-x))/2`. Since `e^x` can overflow for `x ~ 88`, but we cap at 90, we compute `e^x` and `e^(-x)` in `double` (which can handle up to ~709). For `x` near 90, `e^x` overflows `double`? No, `e^90 ≈ 1e39` which fits in `double`. So safe. Then compute `(exp(x) - exp(-x))/2.0` and cast to `float`. For `x` negative, the expression yields negative correctly.
3. **Large |x| ≥ 90**: Overflow condition. Return signed infinity with overflow exception, but under FE_DOWNWARD or FE_TOWARDZERO for positive `x`, return `FLT_MAX` instead; similarly for negative `x` under FE_UPWARD or FE_TOWARDZERO, return `-FLT_MAX`. Set `errno = ERANGE` and raise `FE_OVERFLOW`. Handle NaN and infinity separately (infinity returns itself, NaN returns quiet NaN).

Edge cases: `x = 0` returns 0 (including -0). Subnormal inputs are handled by tiny-range branch. The polynomial coefficients are taken from the provided snippet’s minimax design: `0x1.5555555556583p-3` (≈1/6), `0x1.111110d239f1fp-7` (≈1/120), `0x1.a02b5a284013cp-13` (≈1/5040). For simplicity, the solution can use the Taylor coefficients `1/6, 1/120, 1/5040` which are close enough for float precision, but the reference uses the minimax ones. Time complexity is O(1) for all paths; space O(1).
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <cfenv>
#include <limits>

// Compute sinh(x) for float x, with special-case handling.
// Matches typical libm behavior for overflow, NaN, inf.
// For |x| <= 0.078125 uses polynomial, else uses exp identity.
float sinh_approx(float x) {
    using uint32_t = std::uint32_t;

    // Extract bits for fast checks.
    uint32_t bits;
    std::memcpy(&bits, &x, sizeof(bits));
    uint32_t x_abs = bits & 0x7fffffffU;
    constexpr uint32_t kLargeThreshold = 0x42b40000U; // 90.0f
    constexpr uint32_t kSmallThreshold = 0x3da00000U; // 0.078125f

    if (x_abs >= kLargeThreshold || x_abs <= kSmallThreshold) {
        // |x| <= 0.078125
        if (x_abs <= kSmallThreshold) {
            // Very tiny: |x| <= 2^-26
            if (x_abs <= 0x32800000U) {
                if (x_abs == 0U) return x;
                double xd = x;
                double x2 = xd * xd;
                return static_cast<float>(xd + 0.25 * xd * x2);
            }
            // Small but not tiny: use polynomial.
            double xd = x;
            double x2 = xd * xd;
            // Coefficients from minimax fit (Sollya) for sinh(x)/x.
            double pe = 1.0 +
                        x2 * (0x1.5555555556583p-3 +
                              x2 * (0x1.111110d239f1fp-7 +
                                    x2 * 0x1.a02b5a284013cp-13));
            return static_cast<float>(xd * pe);
        }

        // Handle NaN and infinity
        if (std::isnan(x)) {
            return x + 1.0f; // quiet NaN
        }
        if (std::isinf(x)) {
            return x;
        }

        // |x| >= 90: overflow.
        int rounding = std::fegetround();
        bool neg = (bits >> 31) != 0;
        if (neg) {
            if (rounding == FE_UPWARD || rounding == FE_TOWARDZERO) {
                return -std::numeric_limits<float>::max();
            }
        } else {
            if (rounding == FE_DOWNWARD || rounding == FE_TOWARDZERO) {
                return std::numeric_limits<float>::max();
            }
        }
        errno = ERANGE;
        std::feraiseexcept(FE_OVERFLOW);
        return std::copysign(std::numeric_limits<float>::infinity(), x);
    }

    // Intermediate range: use exp identity.
    double xd = x;
    double result = (std::exp(xd) - std::exp(-xd)) * 0.5;
    return static_cast<float>(result);
}
#include <cassert>
#include <cmath>
#include <cfenv>
#include <cerrno>
#include <cstring>
#include <limits>

// Forward declaration of the solution function (placed above in actual file)
float sinh_approx(float x);

int main() {
    // Basic values
    assert(sinh_approx(0.0f) == 0.0f);
    assert(sinh_approx(-0.0f) == -0.0f;
    assert(std::fabs(sinh_approx(1.0f) - std::sinh(1.0f)) < 1e-6f);
    assert(std::fabs(sinh_approx(-1.0f) - std::sinh(-1.0f)) < 1e-6f);
    assert(std::fabs(sinh_approx(0.5f) - std::sinh(0.5f)) < 1e-6f);

    // Tiny range
    assert(sinh_approx(1e-30f) == 1e-30f); // x + 0.25 x^3 ≈ x
    assert(sinh_approx(-1e-30f) == -1e-30f);

    // Small boundary (0.078125)
    float small = 0.078125f;
    assert(std::fabs(sinh_approx(small) - std::sinh(small)) < 1e-6f);

    // Intermediate
    assert(std::fabs(sinh_approx(10.0f) - std::sinh(10.0f)) < 1e-3f);
    assert(std::fabs(sinh_approx(-10.0f) - std::sinh(-10.0f)) < 1e-3f);

    // Large: overflow to inf
    errno = 0;
    std::feclearexcept(FE_ALL_EXCEPT);
    float big = sinh_approx(100.0f);
    assert(std::isinf(big) && big > 0);
    assert(errno == ERANGE);
    assert(std::fetestexcept(FE_OVERFLOW));

    // Negative large
    errno = 0;
    std::feclearexcept(FE_ALL_EXCEPT);
    float neg_big = sinh_approx(-100.0f);
    assert(std::isinf(neg_big) && neg_big < 0);
    assert(errno == ERANGE);
    assert(std::fetestexcept(FE_OVERFLOW));

    // NaN and Inf
    float nan_input = std::numeric_limits<float>::quiet_NaN();
    assert(std::isnan(sinh_approx(nan_input)));
    float inf_input = std::numeric_limits<float>::infinity();
    assert(std::isinf(sinh_approx(inf_input)) && sinh_approx(inf_input) > 0);

    // Rounding mode for overflow: FE_TOWARDZERO returns FLT_MAX
    std::fesetround(FE_TOWARDZERO);
    errno = 0;
    std::feclearexcept(FE_ALL_EXCEPT);
    float capped = sinh_approx(100.0f);
    assert(capped == std::numeric_limits<float>::max());
    assert(errno == ERANGE);
    std::fesetround(FE_TONEAREST);

    return 0;
}
