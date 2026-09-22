/*
Implement a C++ function that takes a 2D vector (represented as a pair of floating-point values) and returns a 2x2 rotation matrix (as an array of four doubles in row-major order) that zeroes out the second component of the vector when the matrix's transpose is applied to it. Specifically, the returned matrix G must satisfy: if v = (x, y), then G^T * v = (r, 0) for some non-negative r. Use the standard Givens rotation construction: when y = 0, the rotation should be the identity (angle 0); when x = 0 and y ≠ 0, the rotation should be a 90-degree rotation (cos = 0, sin = 1) so that G^T * (0, y) = (|y|, 0). For general nonzero vectors, compute c = x / sqrt(x²+y²) and s = y / sqrt(x²+y²) such that applying G^T yields (sqrt(x²+y²), 0). The matrix G is defined as [[c, s], [-s, c]] and its transpose G^T is [[c, -s], [s, c]]. The function must handle all floating-point inputs, including very large and very small magnitudes, and must return the matrix as a std::array<double, 4> in row-major order (indices 0,1 for first row, 2,3 for second row).
*/

#include <array>
#include <cmath>
#include <utility>

// Returns a Givens rotation matrix G (as a 2x2 row-major array) such that
// applying G^T to the input vector (x, y) yields (r, 0) where r = sqrt(x^2+y^2).
// If both x and y are zero, the identity matrix is returned.
std::array<double, 4> makeGivensRotation(double x, double y) {
    const double r = std::hypot(x, y);
    // Handle the zero vector case: no rotation can produce a nonzero result.
    if (r == 0.0) {
        return {1.0, 0.0, 0.0, 1.0}; // identity
    }
    const double c = x / r; // cosine
    const double s = y / r; // sine
    // G = [[c, s], [-s, c]]
    return {c, s, -s, c};
}

#include <cassert>
#include <array>
#include <cmath>

// Forward declaration of the tested function
std::array<double, 4> makeGivensRotation(double x, double y);

int main() {
    // Test 1: General nonzero vector (3, 4) -> G^T * v = (5, 0)
    {
        auto G = makeGivensRotation(3.0, 4.0);
        double x = 3.0, y = 4.0;
        double new_x = G[0]*x + G[2]*y; // using G^T = [[c, -s], [s, c]]
        double new_y = G[1]*x + G[3]*y;
        assert(std::abs(new_x - 5.0) < 1e-12);
        assert(std::abs(new_y) < 1e-12);
    }
    // Test 2: Negative y value (1, -2) -> G^T * v = (sqrt(5), 0)
    {
        auto G = makeGivensRotation(1.0, -2.0);
        double new_x = G[0]*1.0 + G[2]*(-2.0);
        double new_y = G[1]*1.0 + G[3]*(-2.0);
        assert(std::abs(new_x - std::sqrt(5.0)) < 1e-12);
        assert(std::abs(new_y) < 1e-12);
    }
    // Test 3: y = 0 -> identity matrix
    {
        auto G = makeGivensRotation(5.0, 0.0);
        assert(G[0] == 1.0 && G[1] == 0.0 && G[2] == 0.0 && G[3] == 1.0);
    }
    // Test 4: x = 0, y = 3 -> G^T * (0,3) = (3,0) with c=0, s=1
    {
        auto G = makeGivensRotation(0.0, 3.0);
        double new_x = G[0]*0.0 + G[2]*3.0;
        double new_y = G[1]*0.0 + G[3]*3.0;
        assert(std::abs(new_x - 3.0) < 1e-12);
        assert(std::abs(new_y) < 1e-12);
    }
    // Test 5: Zero vector -> identity
    {
        auto G = makeGivensRotation(0.0, 0.0);
        assert(G[0] == 1.0 && G[1] == 0.0 && G[2] == 0.0 && G[3] == 1.0);
    }
    // Test 6: Very large values (1e200, 1e200) -> no overflow, r ≈ 1.414e200
    {
        auto G = makeGivensRotation(1e200, 1e200);
        double new_x = G[0]*1e200 + G[2]*1e200;
        double new_y = G[1]*1e200 + G[3]*1e200;
        assert(std::abs(new_x - 1e200*std::sqrt(2.0)) < 1e-12);
        assert(std::abs(new_y) < 1e-12);
    }
    // Test 7: Very small values (1e-200, -1e-200) -> no underflow
    {
        auto G = makeGivensRotation(1e-200, -1e-200);
        double new_x = G[0]*1e-200 + G[2]*(-1e-200);
        double new_y = G[1]*1e-200 + G[3]*(-1e-200);
        double expected_r = 1e-200 * std::sqrt(2.0);
        assert(std::abs(new_x - expected_r) < 1e-212);
        assert(std::abs(new_y) < 1e-212);
    }
    // Test 8: Random validation (e.g., 0.5, -0.25) -> direct computation
    {
        double x = 0.5, y = -0.25;
        auto G = makeGivensRotation(x, y);
        double new_x = G[0]*x + G[2]*y;
        double new_y = G[1]*x + G[3]*y;
        double expected_r = std::sqrt(x*x + y*y);
        assert(std::abs(new_x - expected_r) < 1e-12);
        assert(std::abs(new_y) < 1e-12);
    }
    // Test 9: Orthogonality check (G^T * G should be identity)
    {
        auto G = makeGivensRotation(3.0, 4.0);
        // G^T * G = [[c, -s], [s, c]] * [[c, s], [-s, c]] = [[1,0],[0,1]]
        double result00 = G[0]*G[0] + G[2]*G[2];
        double result01 = G[0]*G[1] + G[2]*G[3];
        double result10 = G[1]*G[0] + G[3]*G[2];
        double result11 = G[1]*G[1] + G[3]*G[3];
        assert(std::abs(result00 - 1.0) < 1e-12);
        assert(std::abs(result01) < 1e-12);
        assert(std::abs(result10) < 1e-12);
        assert(std::abs(result11 - 1.0) < 1e-12);
    }
    return 0;
}

// The solution directly implements the Givens rotation construction. For any input vector (x, y), we compute the norm r = sqrt(x²+y²). The key insight is that the matrix G constructed as [[c, s], [-s, c]] with c = x/r and s = y/r (when r > 0) has the property that G^T * v = [[c, -s], [s, c]] * (x, y) = (c*x - s*y, s*x + c*y). Substituting c and s gives first component = (x² + y²)/r = r, and second component = (x*y - x*y)/r = 0, which matches the requirement. Edge cases must be handled explicitly: if both x and y are zero, a zero vector cannot be rotated to a nonzero vector; the convention here is to return the identity matrix (though mathematically any rotation works, but the task specifies that when y=0 the identity is used, and this extends naturally to the zero case). When r is zero, we must avoid division by zero, so we return the identity. When computing r, using std::hypot(x, y) provides better numerical stability against overflow/underflow compared to sqrt(x*x + y*y) for extreme magnitudes. Time complexity is O(1) and space complexity is O(1) since only a few scalar constants are used.
