Write a C++ function named `approximate_integral` that, given a function identifier `function_id` (an integer from 1 to 4 inclusive), a lower bound `a` (float), an upper bound `b` (float, with `b > a`), a number of sample points `n` (positive int), and an integer `intensity` (used in the integrand), computes a midpoint Riemann sum approximation of the integral of the specified function over `[a, b]` using exactly `n` subintervals of equal width. The four integrands are defined as:
- `f1(x, intensity) = x^intensity`
- `f2(x, intensity) = (intensity*x - 3)^2 - 5`
- `f3(x, intensity) = ((intensity*x)^3)/5 - 2*(intensity*x)^2 + 3`
- `f4(x, intensity) = (x^4) / (x^2 + intensity)` (handle division by zero by returning `0.0` if the denominator is exactly `0`)

The function must return the computed sum (as a `float`), and it must not perform any input/output operations. It should be self-contained (only use standard headers like `<cmath>` and `<stdexcept>` if needed). Ensure the function is safe for any valid `function_id`, but you may return `0.0` for invalid IDs (or you may assert but that is not required).

The core algorithm is the midpoint rule for numerical integration: divide `[a, b]` into `n` equal subintervals of width `h = (b - a) / n`. For each subinterval `i` (from `0` to `n-1`), compute the midpoint `x_i = a + (i + 0.5) * h`. Evaluate the chosen function at `x_i` (using the provided `intensity`), sum all those values, and multiply the sum by `h` to get the approximation. This is exactly what the given snippet does in `numerical_integration`. Edge cases: if `b <= a`, the integral should be zero or negative? The problem states `b > a`, but for safety you can return `0.0` if `n <= 0` or `b <= a`. For `f4`, if `x^2 + intensity` equals zero (which only happens when `intensity` is negative and `x = sqrt(-intensity)`), return `0.0` for that term to avoid division by zero. Time complexity is `O(n)` because we loop `n` times. Space complexity is `O(1)` beyond a few local variables.

#include <cmath>

// Midpoint Riemann sum approximation of ∫_a^b f(x) dx using n subintervals.
// function_id: 1=f1, 2=f2, 3=f3, 4=f4. Returns 0.0 for invalid id, n<=0, or b<=a.
float approximate_integral(int function_id, float a, float b, int n, int intensity) {
    if (n <= 0 || b <= a) {
        return 0.0f;
    }

    const float h = (b - a) / static_cast<float>(n);
    float sum = 0.0f;

    for (int i = 0; i < n; ++i) {
        const float x = a + (static_cast<float>(i) + 0.5f) * h;

        switch (function_id) {
            case 1:
                sum += std::pow(x, intensity);
                break;
            case 2: {
                const float term = intensity * x - 3.0f;
                sum += term * term - 5.0f;
                break;
            }
            case 3: {
                const float t = intensity * x;
                const float t2 = t * t;
                sum += (t * t2) / 5.0f - 2.0f * t2 + 3.0f;
                break;
            }
            case 4: {
                const float denom = x * x + static_cast<float>(intensity);
                sum += (std::fabs(denom) < 1e-12f) ? 0.0f : (x * x * x * x) / denom;
                break;
            }
            default:
                return 0.0f; // invalid function id
        }
    }

    return sum * h;
}

#include <cassert>
#include <cmath>

// Assume approximate_integral is declared above.

int main() {
    // For f1(x)=x^2, integral from 0 to 2 is (8/3)≈2.666... with n=1000
    float r1 = approximate_integral(1, 0.0f, 2.0f, 1000, 2);
    assert(std::fabs(r1 - (8.0f/3.0f)) < 0.01f);

    // For f2(x)=(x-3)^2-5, integral from 1 to 4: antiderivative (x-3)^3/3 -5x from 1 to 4
    // = (1/3 -20) - (-8/3 -5) = (1/3 -20) + (8/3 +5) = (9/3) -15 = 3 -15 = -12
    float r2 = approximate_integral(2, 1.0f, 4.0f, 1000, 1);
    assert(std::fabs(r2 - (-12.0f)) < 0.01f);

    // For f3(x)= (x^3)/5 - 2x^2 +3 with intensity=1, integral from 0 to 1:
    // antiderivative: x^4/20 - (2/3)x^3 +3x from 0 to 1 = 0.05 - 0.6667 +3 = 2.38333...
    float r3 = approximate_integral(3, 0.0f, 1.0f, 1000, 1);
    assert(std::fabs(r3 - 2.38333f) < 0.01f);

    // For f4(x)=x^4/(x^2+1) with intensity=1, integral from 0 to 1:
    // numerical value approx 0.2795 (from known integral)
    float r4 = approximate_integral(4, 0.0f, 1.0f, 1000, 1);
    assert(std::fabs(r4 - 0.2795f) < 0.01f);

    // Edge cases: invalid function id returns 0
    assert(approximate_integral(5, 0.0f, 1.0f, 10, 1) == 0.0f);
    // n=0 returns 0
    assert(approximate_integral(1, 0.0f, 1.0f, 0, 1) == 0.0f);
    // b <= a returns 0
    assert(approximate_integral(1, 2.0f, 1.0f, 10, 1) == 0.0f);

    // Test division by zero in f4: x^2 + (-4) =0 when x=2, so integral from 1 to 3 with intensity=-4
    // should not crash; just ensure it returns a finite value
    float r5 = approximate_integral(4, 1.0f, 3.0f, 1000, -4);
    assert(std::isfinite(r5));

    return 0;
}
