Implement a C++ function that performs an incremental least-squares plane fit to a stream of 3D points. The function must maintain cumulative sums of coordinates and their pairwise products, allowing points to be processed one at a time and discarded. A second function finalizes the fit, computing the plane coefficients \(A\), \(B\), \(D\) such that the plane is \(z = A x + B y + D\). The finalization must return a boolean indicating success: `true` if the fit is valid (i.e., the denominator is numerically non-zero), and `false` if the input had no points or if the points are collinear in the XY-plane (resulting in a degenerate fit). The solution must use a simple struct to hold the state and provide a single public interface function that takes a vector of points and returns a `std::optional<Plane>` containing the coefficients if successful, or `std::nullopt` otherwise.

The algorithm is based on the method of least squares for fitting a plane \(z = A x + B y + D\) to \(N\) points. The core idea is to accumulate the following quantities incrementally: \(N\), \(\sum x\), \(\sum y\), \(\sum z\), \(\sum x^2\), \(\sum y^2\), \(\sum xy\), \(\sum yz\), \(\sum xz\), and also track the maximum absolute \(x\) and \(y\) values for numerical stability checks. After all points are added, the finalization computes the means: \(\bar{x} = (\sum x)/N\), etc., and then the centered second moments: \(x2bar = (\sum x^2)/N - \bar{x}^2\), \(y2bar = (\sum y^2)/N - \bar{y}^2\), \(xybar = (\sum xy)/N - \bar{x}\bar{y}\), and similarly for \(yzbar\) and \(xzbar\). The denominator \(DD = x2bar \cdot y2bar - xybar^2\) measures the variance of the XY distribution. If \(DD\) is near zero (compared to the scale of coordinates), the points are collinear in XY, and the fit is degenerate; return failure. Otherwise, solve the 2x2 linear system: \(A = (yzbar \cdot xybar - xzbar \cdot y2bar)/DD\), \(B = (xzbar \cdot xybar - yzbar \cdot x2bar)/DD\), and \(D = -(\bar{z} + A \bar{x} + B \bar{y})\). The incremental approach uses \(O(1)\) memory per point and \(O(N)\) total time. Edge cases: empty input (N=0) fails; very small coordinate magnitudes may cause division by zero; handle by checking the threshold with a scale factor. The solution uses `std::optional` to represent success/failure cleanly. Time complexity \(O(N)\), space complexity \(O(1)\) (excluding input storage).

#include <optional>
#include <vector>
#include <cmath>
#include <limits>

struct Point3D {
    float x, y, z;
};

struct Plane {
    float A, B, D;
};

// Incremental least-squares plane fit accumulator.
class LeastSquaresPlaneFitter {
public:
    LeastSquaresPlaneFitter() = default;

    void addPoint(const Point3D& p) {
        N += 1.0f;
        sum_x += p.x;
        sum_y += p.y;
        sum_z += p.z;
        sum_x2 += p.x * p.x;
        sum_y2 += p.y * p.y;
        sum_xy += p.x * p.y;
        sum_yz += p.y * p.z;
        sum_xz += p.x * p.z;
        max_abs_x = std::max(max_abs_x, std::abs(p.x));
        max_abs_y = std::max(max_abs_y, std::abs(p.y));
    }

    std::optional<Plane> fit() const {
        if (N == 0.0f) {
            return std::nullopt;
        }
        const float invN = 1.0f / N;
        const float xbar = sum_x * invN;
        const float ybar = sum_y * invN;
        const float zbar = sum_z * invN;
        const float x2bar = sum_x2 * invN - xbar * xbar;
        const float y2bar = sum_y2 * invN - ybar * ybar;
        const float xybar = sum_xy * invN - xbar * ybar;
        const float yzbar = sum_yz * invN - ybar * zbar;
        const float xzbar = sum_xz * invN - xbar * zbar;
        const float DD = x2bar * y2bar - xybar * xybar;

        // Check if the XY projection is degenerate (collinear points).
        const float scale = (max_abs_x + max_abs_y);
        if (scale > 0.0f && std::abs(DD) <= 1e-10f * scale) {
            return std::nullopt;
        }
        if (scale == 0.0f) {
            // All points have x=0 and y=0; the plane is vertical or undefined.
            return std::nullopt;
        }

        Plane result;
        result.A = (yzbar * xybar - xzbar * y2bar) / DD;
        result.B = (xzbar * xybar - yzbar * x2bar) / DD;
        result.D = -(zbar + result.A * xbar + result.B * ybar);
        return result;
    }

private:
    float N = 0.0f;
    float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;
    float sum_x2 = 0.0f, sum_y2 = 0.0f, sum_xy = 0.0f;
    float sum_yz = 0.0f, sum_xz = 0.0f;
    float max_abs_x = 0.0f, max_abs_y = 0.0f;
};

// Convenience wrapper: fits a plane to a vector of points.
std::optional<Plane> fitPlane(const std::vector<Point3D>& points) {
    LeastSquaresPlaneFitter fitter;
    for (const auto& p : points) {
        fitter.addPoint(p);
    }
    return fitter.fit();
}

#include <cassert>
#include <cmath>

int main() {
    // Perfect plane z = 2x + 3y + 1
    std::vector<Point3D> points = {
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 3.0f},
        {0.0f, 1.0f, 4.0f},
        {1.0f, 1.0f, 6.0f}
    };
    auto plane = fitPlane(points);
    assert(plane.has_value());
    assert(std::abs(plane->A - 2.0f) < 1e-4f);
    assert(std::abs(plane->B - 3.0f) < 1e-4f);
    assert(std::abs(plane->D - 1.0f) < 1e-4f);

    // Horizontal plane z = 5
    std::vector<Point3D> flat = {
        {0.0f, 0.0f, 5.0f},
        {2.0f, 0.0f, 5.0f},
        {0.0f, 3.0f, 5.0f},
        {2.0f, 3.0f, 5.0f}
    };
    auto flatPlane = fitPlane(flat);
    assert(flatPlane.has_value());
    assert(std::abs(flatPlane->A) < 1e-4f);
    assert(std::abs(flatPlane->B) < 1e-4f);
    assert(std::abs(flatPlane->D - 5.0f) < 1e-4f);

    // Degenerate: all points share same x and y (vertical line)
    std::vector<Point3D> degenerate = {
        {1.0f, 2.0f, 0.0f},
        {1.0f, 2.0f, 1.0f},
        {1.0f, 2.0f, 3.0f}
    };
    assert(!fitPlane(degenerate).has_value());

    // Empty input
    std::vector<Point3D> empty;
    assert(!fitPlane(empty).has_value());

    // Two points (still degenerate in XY)
    std::vector<Point3D> twoPoints = {
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 2.0f}
    };
    assert(!fitPlane(twoPoints).has_value());

    // Single point (denominator zero because no spread)
    std::vector<Point3D> single = {{0.0f, 0.0f, 0.0f}};
    assert(!fitPlane(single).has_value());

    // Noisy plane: points near z = x + y
    std::vector<Point3D> noisy = {
        {0.0f, 0.0f, 0.1f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.1f},
        {1.0f, 1.0f, 2.0f},
        {0.5f, 0.5f, 0.9f}
    };
    auto noisyPlane = fitPlane(noisy);
    assert(noisyPlane.has_value());
    assert(std::abs(noisyPlane->A - 1.0f) < 0.2f);
    assert(std::abs(noisyPlane->B - 1.0f) < 0.2f);
    assert(std::abs(noisyPlane->D) < 0.2f);

    return 0;
}
