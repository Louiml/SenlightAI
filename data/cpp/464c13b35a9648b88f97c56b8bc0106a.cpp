/*
Write a standalone C++ function that performs barycentric interpolation on a tetrahedron defined by four reference points in 3D space, mapping a query point to interpolated values. The function should accept arrays of 3D coordinates for the four tetrahedron vertices (in the order: black, white, blue, yellow), corresponding output values at each vertex (as a template type), and a query point. It should use Cramer's rule to compute barycentric coordinates, and if the query point is inside the tetrahedron (all weights non-negative and sum to 1), return the interpolated value as a weighted sum. If the determinant of the basis matrix is near zero (degenerate/co-planar tetrahedron) or the point is outside, fall back to inverse-distance weighting across all four vertices. The function must be templated so it works with numeric types like `float` or `double`, and return the interpolated value with proper normalization of weights in both cases.
*/

#include <cmath>
#include <array>
#include <algorithm>

// Compute the determinant of a 3x3 matrix given as a flat array (row-major).
template<typename T>
T determinant3x3(const T mat[3][3]) {
    return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
         - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
         + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

// Compute determinant of a 3x3 matrix with column `col` replaced by RHS vector.
template<typename T>
T determinantWithColumn(const T mat[3][3], const T rhs[3], int col) {
    T temp[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            temp[i][j] = (j == col) ? rhs[i] : mat[i][j];
        }
    }
    return determinant3x3(temp);
}

// Perform barycentric interpolation on a tetrahedron with fallback to inverse-distance weighting.
// vertices[0..3] are the tetrahedron corners (black, white, blue, yellow).
// values[0..3] are the output values at each corner.
// query is the point to interpolate.
// Returns the interpolated value.
template<typename T>
T tetrahedralInterpolate(
    const std::array<std::array<T,3>,4>& vertices,
    const std::array<T,4>& values,
    const std::array<T,3>& query
) {
    const T eps = static_cast<T>(1e-6);
    const T tiny = static_cast<T>(1e-3); // epsilon for distance fallback

    // Build matrix and RHS using vertices[3] (yellow) as the base.
    T mat[3][3] = {
        {vertices[0][0] - vertices[3][0], vertices[1][0] - vertices[3][0], vertices[2][0] - vertices[3][0]},
        {vertices[0][1] - vertices[3][1], vertices[1][1] - vertices[3][1], vertices[2][1] - vertices[3][1]},
        {vertices[0][2] - vertices[3][2], vertices[1][2] - vertices[3][2], vertices[2][2] - vertices[3][2]}
    };
    T rhs[3] = {
        query[0] - vertices[3][0],
        query[1] - vertices[3][1],
        query[2] - vertices[3][2]
    };

    T det = determinant3x3(mat);

    // If degenerate (co-planar or nearly so), fall back to inverse-distance.
    if (std::abs(det) < eps) {
        T weights[4];
        T sum = static_cast<T>(0);
        for (int i = 0; i < 4; ++i) {
            T dx = query[0] - vertices[i][0];
            T dy = query[1] - vertices[i][1];
            T dz = query[2] - vertices[i][2];
            T dist = std::sqrt(dx*dx + dy*dy + dz*dz);
            weights[i] = static_cast<T>(1) / (dist + tiny);
            sum += weights[i];
        }
        T result = static_cast<T>(0);
        for (int i = 0; i < 4; ++i) {
            result += (weights[i] / sum) * values[i];
        }
        return result;
    }

    // Compute barycentric weights using Cramer's rule.
    T w0 = determinantWithColumn(mat, rhs, 0) / det;
    T w1 = determinantWithColumn(mat, rhs, 1) / det;
    T w2 = determinantWithColumn(mat, rhs, 2) / det;
    T w3 = static_cast<T>(1) - w0 - w1 - w2;

    // If point is outside the tetrahedron, clamp weights and renormalize.
    // Alternatively, fall back to inverse-distance for any negative or >1 weight.
    if (w0 < -eps || w1 < -eps || w2 < -eps || w3 < -eps ||
        w0 > static_cast<T>(1)+eps || w1 > static_cast<T>(1)+eps ||
        w2 > static_cast<T>(1)+eps || w3 > static_cast<T>(1)+eps) {
        T weights[4] = {w0, w1, w2, w3};
        // Rather than complex clamping, use inverse-distance fallback for robustness.
        T fb_weights[4];
        T sum = static_cast<T>(0);
        for (int i = 0; i < 4; ++i) {
            T dx = query[0] - vertices[i][0];
            T dy = query[1] - vertices[i][1];
            T dz = query[2] - vertices[i][2];
            T dist = std::sqrt(dx*dx + dy*dy + dz*dz);
            fb_weights[i] = static_cast<T>(1) / (dist + tiny);
            sum += fb_weights[i];
        }
        T result = static_cast<T>(0);
        for (int i = 0; i < 4; ++i) {
            result += (fb_weights[i] / sum) * values[i];
        }
        return result;
    }

    // Point is inside; return weighted sum.
    return w0 * values[0] + w1 * values[1] + w2 * values[2] + w3 * values[3];
}

#include <cassert>
#include <cmath>
#include <array>

// Declaration of the solution function (as above).
template<typename T>
T tetrahedralInterpolate(
    const std::array<std::array<T,3>,4>& vertices,
    const std::array<T,4>& values,
    const std::array<T,3>& query
);

int main() {
    // Define a regular tetrahedron with known coordinates for easy math.
    // Vertices: (0,0,0), (1,0,0), (0,1,0), (0,0,1).
    std::array<std::array<double,3>,4> verts = {{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    }};
    // Values at vertices: black=0, white=10, blue=20, yellow=30.
    std::array<double,4> vals = {0.0, 10.0, 20.0, 30.0};

    // Test at vertex itself: query = black -> 0.
    double result = tetrahedralInterpolate(verts, vals, {0.0, 0.0, 0.0});
    assert(std::abs(result - 0.0) < 1e-9);

    // Query = white -> 10.
    result = tetrahedralInterpolate(verts, vals, {1.0, 0.0, 0.0});
    assert(std::abs(result - 10.0) < 1e-9);

    // Query = blue -> 20.
    result = tetrahedralInterpolate(verts, vals, {0.0, 1.0, 0.0});
    assert(std::abs(result - 20.0) < 1e-9);

    // Query = yellow -> 30.
    result = tetrahedralInterpolate(verts, vals, {0.0, 0.0, 1.0});
    assert(std::abs(result - 30.0) < 1e-9);

    // Test centroid of the tetrahedron: (0.25,0.25,0.25).
    // Barycentric weights are all 0.25 each -> (0+10+20+30)*0.25 = 15.
    result = tetrahedralInterpolate(verts, vals, {0.25, 0.25, 0.25});
    assert(std::abs(result - 15.0) < 1e-9);

    // Test midpoint along edge black-white: (0.5,0,0) -> weights: black=0.5, white=0.5, blue=0, yellow=0 -> 5.
    result = tetrahedralInterpolate(verts, vals, {0.5, 0.0, 0.0});
    assert(std::abs(result - 5.0) < 1e-9);

    // Test point outside: (2,0,0) should fall back to inverse-distance weighting.
    // Since it's far from all but white, the result should be close to 10 but not exact.
    result = tetrahedralInterpolate(verts, vals, {2.0, 0.0, 0.0});
    assert(result > 5.0 && result < 10.0); // Reasonable heuristic check.

    // Degenerate tetrahedron: all four points co-planar (z=0).
    std::array<std::array<double,3>,4> flatVerts = {{
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 1.0, 0.0}
    }};
    // Query point at the centroid of the square: (0.5,0.5,0) -> should get average-ish value.
    result = tetrahedralInterpolate(flatVerts, vals, {0.5, 0.5, 0.0});
    // Expect near (0+10+20+30)/4 = 15 (distance-weighted fallback gives a similar value).
    assert(std::abs(result - 15.0) < 5.0);

    // Test template with float precision.
    std::array<std::array<float,3>,4> fverts = {{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    }};
    std::array<float,4> fvals = {0.0f, 10.0f, 20.0f, 30.0f};
    float fresult = tetrahedralInterpolate(fverts, fvals, {0.25f, 0.25f, 0.25f});
    assert(std::abs(fresult - 15.0f) < 1e-4f);

    return 0;
}

// The core algorithm constructs a 3×3 matrix from the differences between three vertices (black, white, blue) and the fourth (yellow), and a right-hand side from the query point minus the fourth vertex. The determinant of this matrix gives the scaled volume of the tetrahedron (6× volume). Using Cramer's rule, the three barycentric weights (for black, white, blue) are obtained by replacing each column of the matrix with the RHS vector and dividing the resulting determinant by the main determinant. The weight for yellow is then `1 - (w_black + w_white + w_blue)`. If the main determinant is very small (e.g., absolute value < 1e-6), the tetrahedron is degenerate (co-planar or nearly so), so we fall back to inverse-distance weighting: each weight is `1/distance^2` (or simply `1/distance` with a small epsilon to avoid division by zero) to the four vertices, normalized to sum to 1. If the point lies outside the tetrahedron (any weight is negative or slightly exceeds 1 due to floating-point error), we also apply the same distance-based fallback. The interpolated value is the weighted sum of the vertex output values. Complexity is O(1) time and space, as operations are fixed-size 3×3 determinant calculations and a few arithmetic ops. Edge cases include zero-distance from query point to a vertex (inverse-distance would blow up; using `1/(distance + epsilon)` handles it), co-planar points causing determinant near zero, and points outside the tetrahedron where the fallback still gives reasonable interpolation.
