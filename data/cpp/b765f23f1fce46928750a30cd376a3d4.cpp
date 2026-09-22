/*
Write a C++ function named `simpleCubicParameter` that computes the local simple cubic order parameter \( s_i \) for a central atom. The function takes a vector of `Vector3D` structures (each with `x`, `y`, `z` double components) representing the positions of neighboring atoms relative to the central atom (i.e., the vector from the central atom to each neighbor). It also takes a double `r0` and a double `d0`, and uses a switching function based on a rational form: \( \sigma(r) = \frac{1}{1 + \left(\frac{r - d_0}{r_0}\right)^6} \) for distances \( r \le d_0 + r_0 \) and \( \sigma(r) = 0 \) otherwise. The function returns the weighted average of \( \frac{x^4 + y^4 + z^4}{r^4} \) over all neighbors, where \( r^4 \) is \( (x^2+y^2+z^2)^2 \), and the weights are the switching function values. If the sum of weights is zero (i.e., no neighbors within the cutoff), return 0.0. The function must be `const`-correct, handle an empty input vector by returning 0.0, and avoid overflow by computing \( r^4 \) as the square of \( r^2 \). Provide a high-quality implementation with a descriptive free function and no `main`.
*/

#include <vector>
#include <cmath>

struct Vector3D {
    double x;
    double y;
    double z;
};

// Compute the simple cubic order parameter for a central atom given relative vectors to neighbors.
// r0: characteristic length, d0: offset in switching function.
// Switching function sigma(r) = 1 / (1 + ((r - d0)/r0)^6) for r <= d0+r0, else 0.
// Returns weighted average of (x^4+y^4+z^4)/r^4, or 0.0 if no neighbors within cutoff.
double simpleCubicParameter(const std::vector<Vector3D>& neighbors, double r0, double d0) {
    if (neighbors.empty()) {
        return 0.0;
    }

    double cutoff2 = (d0 + r0) * (d0 + r0);
    double numerator = 0.0;
    double denominator = 0.0;

    for (const auto& vec : neighbors) {
        double x2 = vec.x * vec.x;
        double y2 = vec.y * vec.y;
        double z2 = vec.z * vec.z;
        double d2 = x2 + y2 + z2;
        if (d2 <= cutoff2) {
            // Switching function value: sigma = 1 / (1 + ((sqrt(d2) - d0)/r0)^6)
            double arg = (std::sqrt(d2) - d0) / r0;
            double arg2 = arg * arg;
            double arg6 = arg2 * arg2 * arg2;
            double sigma = 1.0 / (1.0 + arg6);

            // Numerator term: (x^4 + y^4 + z^4) / r^4  = (x^4+y^4+z^4)/(d2^2)
            double x4 = x2 * x2;
            double y4 = y2 * y2;
            double z4 = z2 * z2;
            double r4 = d2 * d2;
            double harmonic = (x4 + y4 + z4) / r4;

            numerator += sigma * harmonic;
            denominator += sigma;
        }
    }

    if (denominator == 0.0) {
        return 0.0;
    }
    return numerator / denominator;
}

#include <cassert>
#include <cmath>

int main() {
    // Empty input
    assert(simpleCubicParameter({}, 1.0, 0.0) == 0.0);

    // Single neighbor at perfect simple cubic position: (1,0,0)
    // r=1, d0=0, r0=1 => sigma = 1/(1+((1-0)/1)^6)=1/(1+1)=0.5
    // harmonic = (1+0+0)/1 = 1
    // result = 0.5*1 / 0.5 = 1.0
    {
        std::vector<Vector3D> v = {{1.0, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0);
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    // Single neighbor at (1,1,0): r^2=2, r^4=4, x^4+y^4+z^4=1+1+0=2 => harmonic=0.5
    // sigma = 1/(1+((sqrt(2)-0)/1)^6) = 1/(1+8)=1/9
    // result = (1/9*0.5)/(1/9) = 0.5
    {
        std::vector<Vector3D> v = {{1.0, 1.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0);
        assert(std::fabs(val - 0.5) < 1e-12);
    }

    // Two neighbors: one at (1,0,0) and one at (0,1,0). Both have harmonic=1, sigma=0.5 each.
    // numerator = 0.5*1 + 0.5*1 = 1.0, denominator = 1.0, result=1.0
    {
        std::vector<Vector3D> v = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0);
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    // Neighbor exactly at cutoff: r = d0 + r0 = 1.5, r0=1, d0=0.5 => arg=1.0, sigma=0.5
    // harmonic computed for (1.5,0,0): r^2=2.25, r^4=5.0625, x^4=5.0625 => harmonic=1
    // result = 0.5*1/0.5 = 1
    {
        std::vector<Vector3D> v = {{1.5, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.5);
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    // Neighbor beyond cutoff: r=2.0, d0=0.5, r0=1.0 => cutoff=1.5, so sigma=0 => denominator=0 => 0.0
    {
        std::vector<Vector3D> v = {{2.0, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.5);
        assert(val == 0.0);
    }

    // Mixed: one neighbor at (1,0,0) weight 0.5, another at (2,0,0) weight 0 (beyond cutoff)
    // result = 0.5*1 / 0.5 = 1
    {
        std::vector<Vector3D> v = {{1.0, 0.0, 0.0}, {2.0, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0); // cutoff = 1.0, second neighbor at 2.0 > 1.0 => ignored
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    // Symmetry check: neighbor at (-1,0,0) same as (1,0,0)
    {
        std::vector<Vector3D> v = {{-1.0, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0);
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    // Large r0 makes sigma approach 1 for small distances; e.g., r0=100, d0=0, neighbor at (1,0,0)
    // arg = (1-0)/100 = 0.01, sigma = 1/(1+1e-12) ≈ 1.0, harmonic=1, result≈1
    {
        std::vector<Vector3D> v = {{1.0, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 100.0, 0.0);
        assert(std::fabs(val - 1.0) < 1e-9);
    }

    // Check with two neighbors where one is closer, result should be weighted average
    // Neighbor A at (1,0,0): sigma_A = 0.5 (d0=0,r0=1), harmonic=1
    // Neighbor B at (0,0,0)? But r=0 leads to division by zero in harmonic; we avoid that by not using zero distance.
    // Instead use B at (0.5,0,0): r=0.5, arg=0.5, sigma=1/(1+0.015625)=0.984615..., harmonic=1
    // numerator=0.5*1 + 0.984615*1=1.484615, denominator=1.484615, result=1
    {
        std::vector<Vector3D> v = {{1.0, 0.0, 0.0}, {0.5, 0.0, 0.0}};
        double val = simpleCubicParameter(v, 1.0, 0.0);
        assert(std::fabs(val - 1.0) < 1e-12);
    }

    return 0;
}

// The solution iterates over all neighbor vectors. For each neighbor, compute the squared distance \( d2 = x^2 + y^2 + z^2 \). The switching function is \( \sigma = \frac{1}{1 + \left(\frac{\sqrt{d2} - d_0}{r_0}\right)^6} \), but to avoid a costly square root we can compute the condition \( \sqrt{d2} \le d_0 + r_0 \) as \( d2 \le (d_0 + r_0)^2 \). If that condition holds, compute \( \sigma \) using a helper that evaluates the rational function with a power of six. To avoid overflow, compute the sixth power as the cube of the square of the argument. Then accumulate the weighted numerator as \( \sigma \cdot \frac{x^4 + y^4 + z^4}{d2^2} \) and the denominator as \( \sigma \). The final result is numerator divided by denominator, or 0.0 if denominator is zero. Edge cases: empty input returns 0.0; all weights zero returns 0.0; a single neighbor with weight 1 returns its harmonic value. Time complexity is \( O(n) \) for \( n \) neighbors, and space complexity is \( O(1) \) beyond the input vector. Use `std::vector` and a simple struct for 3D points.
