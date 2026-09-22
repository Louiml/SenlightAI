// Write a C++ function named `computeActivationStats` that takes a `double` input value `x` and returns a `struct ActivationStats` containing four fields: `logistic`, `logisticDerivative`, `tanh`, and `tanhDerivative`. The function must compute the logistic (sigmoid) function `1/(1+exp(-x))`, its derivative (using the logistic value itself: `l*(1-l)`), the hyperbolic tangent `(exp(x)-exp(-x))/(exp(x)+exp(-x))`, and its derivative (using the tanh value itself: `1 - t*t`). All computations must be performed using `std::exp` from `<cmath>`. The function should be `const`-correct (i.e., it can be called on const objects) and must not mutate any global state. Additionally, you must ensure that for very large positive or negative `x`, the function does not produce `NaN` or `inf` due to overflow; specifically, handle the logistic function by clamping `x` to avoid `exp(-x)` overflow (e.g., if `x > 700`, return 1.0; if `x < -700`, return 0.0). For tanh, the current formula is safe but you may simplify using `std::tanh` if desired, but the core requirement is to implement the formulas directly.

// The solution defines a `struct ActivationStats` with four `double` members. The main computational challenge is numeric stability for the logistic function. Directly computing `exp(-x)` overflows for very negative `x` (e.g., `x=-1000` gives `exp(1000)` which is `inf`, leading to `1/(1+inf)` = 0.0, which is actually fine, but for `x=1000`, `exp(-1000)` underflows to 0.0, giving `1/(1+0)` = 1.0, also fine. However, for `x` around `-745` or lower, `exp(-x)` may overflow to `inf` but 1/inf = 0.0, which is correct. The real vulnerability is intermediate overflow in `exp(-x)` when `x` is negative and large: `exp(800)` overflows to `inf`, denominator becomes `inf`, result is 0.0, correct. But to be safe and to avoid any undefined behavior (though `inf` is well-defined in IEEE), we clamp `x` to a range like `[-700, 700]` because `exp(700)` is about `1.0e304`, still finite, and `exp(-700)` is tiny but not zero. For values outside this range, the logistic function is effectively 0 or 1. This avoids any possibility of overflow. The derivative of logistic is computed as `l*(1-l)`, which is numerically stable. For tanh, the formula `(exp(x)-exp(-x))/(exp(x)+exp(-x))` can overflow for large `x` (e.g., `x=1000` gives `exp(1000)` overflow to inf, `exp(-1000)` underflows to 0, so numerator = inf, denominator = inf, giving `inf/inf` = NaN). To fix this, we can either use `std::tanh` from `<cmath>` (which is the cleanest) or clamp `x` similarly. The safest approach is to call `std::tanh(x)` directly, but the task says "implement the formulas directly" – however, using `std::tanh` is acceptable as it is the standard library implementation. To satisfy the requirement of implementing directly, we can clamp `x` to `[-700, 700]` for tanh as well, because for `|x| > 20`, tanh is already ±1 to within double precision, but for `x=1000`, `(exp(x)-exp(-x))/(exp(x)+exp(-x))` overflows. Clamping to `[-700,700]` ensures no overflow because `exp(700)` is finite. Both `exp(700)` and `exp(-700)` are finite, so division works. However, for `x=1000`, tanh should be 1.0, so we can clamp `x` to `[-700, 700]` and then compute, which will give approximately 1.0 or -1.0. That is fine. Time complexity is O(1) per call, space O(1). Edge cases: `x=0` gives logistic=0.5, derivative=0.25, tanh=0, derivative=1. Very large positive/negative `x` are handled by clamping. Also, the struct should be defined before the function, and the function returns by value.

#include <cmath>

struct ActivationStats {
    double logistic;
    double logisticDerivative;
    double tanh;
    double tanhDerivative;
};

// Compute logistic and tanh activations and their derivatives for a given input x.
// To avoid overflow in exp(), clamp x to a safe range [-700, 700].
ActivationStats computeActivationStats(const double x) {
    const double clampedX = (x > 700.0) ? 700.0 : ((x < -700.0) ? -700.0 : x);

    const double expNegX = std::exp(-clampedX);
    const double logistic = 1.0 / (1.0 + expNegX);
    const double logisticDerivative = logistic * (1.0 - logistic);

    const double expX = std::exp(clampedX);
    const double expNegX2 = std::exp(-clampedX);
    const double tanh = (expX - expNegX2) / (expX + expNegX2);
    const double tanhDerivative = 1.0 - tanh * tanh;

    return {logistic, logisticDerivative, tanh, tanhDerivative};
}

#include <cassert>
#include <cmath>

int main() {
    // Test at x=0
    ActivationStats s0 = computeActivationStats(0.0);
    assert(std::abs(s0.logistic - 0.5) < 1e-12);
    assert(std::abs(s0.logisticDerivative - 0.25) < 1e-12);
    assert(std::abs(s0.tanh - 0.0) < 1e-12);
    assert(std::abs(s0.tanhDerivative - 1.0) < 1e-12);

    // Test at x=1
    ActivationStats s1 = computeActivationStats(1.0);
    double expectedLogistic = 1.0 / (1.0 + std::exp(-1.0));
    double expectedTanh = std::tanh(1.0);
    assert(std::abs(s1.logistic - expectedLogistic) < 1e-12);
    assert(std::abs(s1.logisticDerivative - expectedLogistic * (1.0 - expectedLogistic)) < 1e-12);
    assert(std::abs(s1.tanh - expectedTanh) < 1e-12);
    assert(std::abs(s1.tanhDerivative - (1.0 - expectedTanh * expectedTanh)) < 1e-12);

    // Test at large positive x (no overflow, logistic ~ 1, tanh ~ 1)
    ActivationStats sBig = computeActivationStats(1000.0);
    assert(std::abs(sBig.logistic - 1.0) < 1e-12);
    assert(std::abs(sBig.logisticDerivative - 0.0) < 1e-12);
    assert(std::abs(sBig.tanh - 1.0) < 1e-12);
    assert(std::abs(sBig.tanhDerivative - 0.0) < 1e-12);

    // Test at large negative x (logistic ~ 0, tanh ~ -1)
    ActivationStats sNeg = computeActivationStats(-1000.0);
    assert(std::abs(sNeg.logistic - 0.0) < 1e-12);
    assert(std::abs(sNeg.logisticDerivative - 0.0) < 1e-12);
    assert(std::abs(sNeg.tanh - (-1.0)) < 1e-12);
    assert(std::abs(sNeg.tanhDerivative - 0.0) < 1e-12);

    // Test for a negative moderate value
    ActivationStats sNegMod = computeActivationStats(-2.0);
    double expectedLogisticNeg = 1.0 / (1.0 + std::exp(2.0));
    double expectedTanhNeg = std::tanh(-2.0);
    assert(std::abs(sNegMod.logistic - expectedLogisticNeg) < 1e-12);
    assert(std::abs(sNegMod.tanh - expectedTanhNeg) < 1e-12);
    assert(std::abs(sNegMod.tanhDerivative - (1.0 - expectedTanhNeg * expectedTanhNeg)) < 1e-12);
}
