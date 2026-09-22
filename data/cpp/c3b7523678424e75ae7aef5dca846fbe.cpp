Write a standalone C++ function `approximateSemicircleArcLength(int n)` that approximates the arc length of the upper semicircle \( y = \sqrt{1 - x^2} \) from \( x = -1 \) to \( x = 1 \). The function takes a positive integer `n` (the number of intended segments) and returns a `double` containing the approximated arc length. The approximation uses a constant step size `dx = 2.0 / n` and then, starting from \( x = -0.999999999999 \), iteratively moves along the curve by projecting each step onto the tangent line of the curve at the current point, ensuring the horizontal component of each tangent step is exactly `dx`. The process continues until reaching or exceeding \( x = 1 \); if the next tangent step would overshoot \( x = 1 \), it is clamped to exactly 1 and the corresponding segment length is computed via the slope. For each full tangent step, the horizontal displacement is exactly `dx` (so the segment length contribution is `dx`), and the loop terminates early if no numerical progress is made (i.e., the next `x` equals the current `x` due to floating-point precision). The function must handle `n` up to at least 6,000,000 efficiently and return the sum of all segment lengths. The task is purely computational; no input/output is required in the function itself, and the result should be a reasonable approximation of \( \pi \) (the true semicircle arc length).
The key idea is to approximate the curve length by moving along tangent lines instead of using the traditional chord length between points on the curve. At each current point \( (x, f(x)) \), compute the slope \( k = f'(x) = \frac{-x}{\sqrt{1-x^2}} \). The unit tangent vector is \( \left( \frac{1}{\sqrt{k^2+1}}, \frac{k}{\sqrt{k^2+1}} \right) \). To achieve a horizontal step of exactly `dx`, we multiply the tangent vector by `dx`, giving a new `x` as \( x_{\text{new}} = x + \frac{dx}{\sqrt{k^2+1}} \). Because the horizontal component of this projected step is exactly `dx`, the segment length contribution of a full step is exactly `dx` (from the horizontal component times the tangent's horizontal unit vector length). However, if the step would take `x` past 1, we clamp the final point to exactly 1 and compute the segment length using the slope: the vertical change is \( k \cdot (1 - x) \), and the segment length is \( \sqrt{(1-x)^2 + (k(1-x))^2} \). Starting from \( x = -0.999999999999 \) (slightly above -1 to avoid derivative singularity at the endpoints), we sum all contributions and count the number of steps. If the computed `nX` equals `cX` (no progress due to floating-point limits), we break to avoid infinite loops. The final result is the sum, approximating \( \pi \). Edge cases: `n` must be positive; if `n` is very large, the loop may become infinite if `dx` becomes too small relative to the starting `x` offset, so the break condition is essential. Time complexity is \( O(\text{effective number of steps}) \), which in practice is roughly proportional to `n` for reasonable values but can terminate earlier due to precision. Space complexity is \( O(1) \).
#include <cmath>

// Approximate the arc length of the upper semicircle y = sqrt(1 - x^2)
// from x = -1 to x = 1 using tangent-line stepping with horizontal step 2/n.
// Returns the sum of segment lengths, approximately pi.
double approximateSemicircleArcLength(int n) {
    if (n <= 0) {
        return 0.0;
    }

    const double dx = 2.0 / static_cast<double>(n);
    const double startX = -0.999999999999;
    double x = startX;
    double sum = 0.0;

    while (x < 1.0) {
        // f(x) = sqrt(1 - x^2)
        const double fx = std::sqrt(1.0 - x * x);
        // derivative f'(x) = -x / sqrt(1 - x^2)
        const double k = -x / fx;

        // Horizontal component of unit tangent is 1/sqrt(k^2 + 1)
        const double nextX = x + dx / std::sqrt(k * k + 1.0);

        // If no progress due to floating-point precision, break
        if (nextX == x) {
            break;
        }

        if (nextX > 1.0) {
            // Clamp final point to x = 1
            const double horizontal = 1.0 - x;
            const double vertical = k * horizontal;
            sum += std::sqrt(horizontal * horizontal + vertical * vertical);
            break;
        }

        // Full tangent step with horizontal component exactly dx
        sum += dx;
        x = nextX;
    }

    return sum;
}
#include <cmath>
#include <cassert>

int main() {
    // For small n, the approximation is rough but should be positive and < 4
    double result1 = approximateSemicircleArcLength(10);
    assert(result1 > 0.0 && result1 < 4.0);

    // For large n, the approximation should be close to pi (3.14159...)
    double result2 = approximateSemicircleArcLength(1000);
    assert(result2 > 3.0 && result2 < 3.3);

    double result3 = approximateSemicircleArcLength(100000);
    assert(std::abs(result3 - 3.141592653589793) < 0.001);

    // Edge case: n = 0 or negative should return 0
    assert(approximateSemicircleArcLength(0) == 0.0);
    assert(approximateSemicircleArcLength(-5) == 0.0);

    // Test with a very large n (e.g., 6,000,000) to ensure it doesn't hang
    // and returns a value very close to pi
    double result4 = approximateSemicircleArcLength(6000000);
    assert(std::abs(result4 - 3.141592653589793) < 0.0001);

    // Verify the function is deterministic: same input gives same output
    assert(approximateSemicircleArcLength(1234) == approximateSemicircleArcLength(1234));
}
