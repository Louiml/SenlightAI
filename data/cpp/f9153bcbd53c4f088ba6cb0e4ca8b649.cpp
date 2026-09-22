// Write a C++ function `bool finish_incremental_LSF(LinearFitData& lsf)` that completes an incremental least-squares plane fit from accumulated sums of coordinates. The input structure contains running sums `N` (number of points), `xbar`, `ybar`, `zbar` (accumulated sums of x, y, z), `x2bar`, `y2bar`, `z2bar` (accumulated sums of squares), `xybar`, `yzbar`, `xzbar` (accumulated sums of products), and `max_absx`, `max_absy` (maximum absolute x and y values seen). After processing all points, the function normalizes these accumulations to compute means, variances, and covariances, then solves for the plane coefficients `A`, `B`, `D` of the equation `z = A*x + B*y + D`. It must handle the degenerate case where the x-y covariance matrix is singular (i.e., all points are collinear in the x-y plane) by returning `false`. The function returns `true` on success and `false` on failure. The input struct must be defined with `float` fields and the function must be `const`-correct where appropriate.
The algorithm is based on the standard least-squares normal equations for a plane. We maintain running sums incrementally: for each point `(x, y, z)`, we add to `N`, `xbar += x`, `ybar += y`, `zbar += z`, `x2bar += x*x`, `y2bar += y*y`, `z2bar += z*z`, `xybar += x*y`, `yzbar += y*z`, `xzbar += x*z`, and update `max_absx` and `max_absy`. After all points are accumulated, the function normalizes each sum by dividing by `N` to get the mean values (`xbar`, `ybar`, `zbar`). Then it computes variances and covariances: `x2bar = x2bar/N - xbar^2`, etc. The key is the 2x2 symmetric matrix `[[x2bar, xybar], [xybar, y2bar]]`; its determinant `DD = x2bar*y2bar - xybar^2` must be non-zero and also not too small relative to the scale of the data. If `DD` is zero or near zero, the points are collinear in the x-y plane, making the plane fit underdetermined; we return `false`. Otherwise, we solve for `A` and `B` via Cramer's rule: `A = (yzbar*xybar - xzbar*y2bar)/DD`, `B = (xzbar*xybar - yzbar*x2bar)/DD`, then compute `D = -(zbar + A*xbar + B*ybar)`. Edge cases: empty data (`N == 0`) returns `false`; a degenerate determinant (using a relative tolerance based on `max_absx + max_absy` to avoid false negatives from scaling) returns `false`. Time complexity is O(1) per call (the accumulation is O(1) per point, but the finish function is O(1)), and space complexity is O(1) beyond the struct.
#include <cmath>

// Structure to hold incremental sums for least-squares plane fitting.
struct LinearFitData {
    float N = 0.0f;        // number of points
    float xbar = 0.0f;     // sum of x
    float ybar = 0.0f;     // sum of y
    float zbar = 0.0f;     // sum of z
    float x2bar = 0.0f;    // sum of x^2
    float y2bar = 0.0f;    // sum of y^2
    float z2bar = 0.0f;    // sum of z^2
    float xybar = 0.0f;    // sum of x*y
    float yzbar = 0.0f;    // sum of y*z
    float xzbar = 0.0f;    // sum of x*z
    float max_absx = 0.0f; // max absolute x seen
    float max_absy = 0.0f; // max absolute y seen
    float A = 0.0f;        // plane coefficient for x
    float B = 0.0f;        // plane coefficient for y
    float D = 0.0f;        // plane constant

    // Add one point to the incremental sums.
    void addPoint(float x, float y, float z) {
        N += 1.0f;
        xbar += x;
        ybar += y;
        zbar += z;
        x2bar += x * x;
        y2bar += y * y;
        z2bar += z * z;
        xybar += x * y;
        yzbar += y * z;
        xzbar += x * z;
        const float ax = std::abs(x);
        const float ay = std::abs(y);
        if (ax > max_absx) max_absx = ax;
        if (ay > max_absy) max_absy = ay;
    }
};

/**
 * Finish the incremental least-squares plane fit.
 * Returns true on success, false if the fit is degenerate.
 * On success, sets A, B, D to satisfy z = A*x + B*y + D.
 */
bool finish_incremental_LSF(LinearFitData& lsf) {
    const float N = lsf.N;
    if (N == 0.0f)
        return false;

    // Normalize sums to means.
    lsf.xbar /= N;
    lsf.ybar /= N;
    lsf.zbar /= N;
    lsf.x2bar = lsf.x2bar / N - lsf.xbar * lsf.xbar;
    lsf.y2bar = lsf.y2bar / N - lsf.ybar * lsf.ybar;
    lsf.z2bar = lsf.z2bar / N - lsf.zbar * lsf.zbar;
    lsf.xybar = lsf.xybar / N - lsf.xbar * lsf.ybar;
    lsf.yzbar = lsf.yzbar / N - lsf.ybar * lsf.zbar;
    lsf.xzbar = lsf.xzbar / N - lsf.xbar * lsf.zbar;

    const float DD = lsf.x2bar * lsf.y2bar - lsf.xybar * lsf.xybar;
    // Relative tolerance to avoid division by near-zero.
    const float scale = lsf.max_absx + lsf.max_absy;
    if (std::abs(DD) <= 1e-10f * scale)
        return false;

    lsf.A = (lsf.yzbar * lsf.xybar - lsf.xzbar * lsf.y2bar) / DD;
    lsf.B = (lsf.xzbar * lsf.xybar - lsf.yzbar * lsf.x2bar) / DD;
    lsf.D = -(lsf.zbar + lsf.A * lsf.xbar + lsf.B * lsf.ybar);
    return true;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple plane z = 2x + 3y + 1 for points (0,0,1), (1,0,3), (0,1,4), (1,1,6)
    {
        LinearFitData lsf;
        lsf.addPoint(0.0f, 0.0f, 1.0f);
        lsf.addPoint(1.0f, 0.0f, 3.0f);
        lsf.addPoint(0.0f, 1.0f, 4.0f);
        lsf.addPoint(1.0f, 1.0f, 6.0f);
        assert(finish_incremental_LSF(lsf) == true);
        assert(std::abs(lsf.A - 2.0f) < 1e-4f);
        assert(std::abs(lsf.B - 3.0f) < 1e-4f);
        assert(std::abs(lsf.D - 1.0f) < 1e-4f);
    }

    // Test 2: Empty data returns false
    {
        LinearFitData lsf;
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 3: All points collinear in x-y plane -> degenerate
    {
        LinearFitData lsf;
        lsf.addPoint(1.0f, 1.0f, 5.0f);
        lsf.addPoint(2.0f, 2.0f, 7.0f);
        lsf.addPoint(3.0f, 3.0f, 9.0f);
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 4: Horizontal plane z = constant (no x,y dependence)
    {
        LinearFitData lsf;
        lsf.addPoint(0.0f, 0.0f, 2.0f);
        lsf.addPoint(1.0f, 0.0f, 2.0f);
        lsf.addPoint(0.0f, 1.0f, 2.0f);
        lsf.addPoint(1.0f, 1.0f, 2.0f);
        assert(finish_incremental_LSF(lsf) == true);
        assert(std::abs(lsf.A) < 1e-4f);
        assert(std::abs(lsf.B) < 1e-4f);
        assert(std::abs(lsf.D - 2.0f) < 1e-4f);
    }

    // Test 5: Plane with negative coefficients and many points with noise-free fit
    {
        LinearFitData lsf;
        for (int i = 0; i <= 5; ++i) {
            for (int j = 0; j <= 5; ++j) {
                float x = static_cast<float>(i);
                float y = static_cast<float>(j);
                float z = -1.5f * x + 0.5f * y - 2.0f;
                lsf.addPoint(x, y, z);
            }
        }
        assert(finish_incremental_LSF(lsf) == true);
        assert(std::abs(lsf.A - (-1.5f)) < 1e-4f);
        assert(std::abs(lsf.B - 0.5f) < 1e-4f);
        assert(std::abs(lsf.D - (-2.0f)) < 1e-4f);
    }

    // Test 6: Single point is degenerate (no plane)
    {
        LinearFitData lsf;
        lsf.addPoint(3.0f, -2.0f, 1.0f);
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 7: Two points that are not collinear in x-y (e.g., differ in x and y) is still degenerate because a plane needs 3 non-collinear points in 3D, but x-y spread is fine -> still degenerate because N=2 gives zero covariance? Actually two points always make a line, determinant zero.
    {
        LinearFitData lsf;
        lsf.addPoint(0.0f, 0.0f, 1.0f);
        lsf.addPoint(1.0f, 1.0f, 2.0f);
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 8: Points with duplicated x and y but different z -> degenerate (no x-y spread)
    {
        LinearFitData lsf;
        lsf.addPoint(2.0f, 3.0f, 1.0f);
        lsf.addPoint(2.0f, 3.0f, 5.0f);
        lsf.addPoint(2.0f, 3.0f, -1.0f);
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 9: Points where x and y are perfectly correlated (line in x-y) -> determinant zero
    {
        LinearFitData lsf;
        lsf.addPoint(0.0f, 0.0f, 0.0f);
        lsf.addPoint(1.0f, 2.0f, 1.0f);
        lsf.addPoint(2.0f, 4.0f, 2.0f);
        lsf.addPoint(3.0f, 6.0f, 3.0f);
        assert(finish_incremental_LSF(lsf) == false);
    }

    // Test 10: Large offsets, plane with small coefficients, check relative tolerance
    {
        LinearFitData lsf;
        // Points around (1000, -1000) with plane z = 0.001x - 0.002y + 5
        for (int i = 0; i <= 2; ++i) {
            for (int j = 0; j <= 2; ++j) {
                float x = 1000.0f + i * 0.001f;
                float y = -1000.0f + j * 0.001f;
                float z = 0.001f * x - 0.002f * y + 5.0f;
                lsf.addPoint(x, y, z);
            }
        }
        assert(finish_incremental_LSF(lsf) == true);
        assert(std::abs(lsf.A - 0.001f) < 1e-3f);
        assert(std::abs(lsf.B - (-0.002f)) < 1e-3f);
        assert(std::abs(lsf.D - 5.0f) < 1e-3f);
    }

    return 0;
}
