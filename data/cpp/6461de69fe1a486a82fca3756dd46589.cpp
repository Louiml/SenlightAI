/*
Write a standalone C++ function that, given a 3D vector `v` represented as an `Eigen::Vector3d` and another 3D vector `w`, returns a `std::pair<double, Vector3d>` containing the dot product of `v` and `w` as the first element and the cross product `v × w` as the second element. The function must compute the dot product both directly using `.dot()` and via the matrix product `v.adjoint() * w`, verifying that both yield the exact same floating-point value (within machine epsilon). The function should handle any finite `double` inputs, including zero vectors, negative components, and very small or large magnitudes, without crashing or producing `NaN` results. The function must be `const`-correct, take arguments by `const` reference, and return the pair with the cross product as a `Vector3d`. No external libraries beyond Eigen and the standard library are allowed.
*/
#include <Eigen/Dense>
#include <utility>
#include <cmath>

// Compute dot (two ways) and cross product of two 3D vectors.
// Returns {dot_product, cross_product_vector}.
// The dot product is verified to be consistent between .dot() and adjoint()*w.
std::pair<double, Eigen::Vector3d> computeDotAndCross(const Eigen::Vector3d& v, const Eigen::Vector3d& w) {
    // Direct dot product
    double dot_direct = v.dot(w);
    
    // Matrix product approach: v.adjoint() is a 1x3 row vector, times w (3x1) gives 1x1 matrix.
    double dot_matrix = (v.adjoint() * w)(0, 0); // Convert to scalar
    
    // Ensure both methods produce practically the same value
    assert(std::abs(dot_direct - dot_matrix) < 1e-12); // Using assert for robust checking, but not required
    
    // Cross product
    Eigen::Vector3d cross = v.cross(w);
    
    return std::make_pair(dot_direct, cross);
}
#include <Eigen/Dense>
#include <cassert>
#include <cmath>

// Declaration of the function under test
std::pair<double, Eigen::Vector3d> computeDotAndCross(const Eigen::Vector3d& v, const Eigen::Vector3d& w);

int main() {
    // Test 1: Basic non-orthogonal vectors
    Eigen::Vector3d v1(1, 2, 3);
    Eigen::Vector3d w1(0, 1, 2);
    auto result1 = computeDotAndCross(v1, w1);
    assert(std::abs(result1.first - 8.0) < 1e-12); // dot = 1*0 + 2*1 + 3*2 = 8
    Eigen::Vector3d expected_cross1(1, -2, 1); // (2*2 -3*1, 3*0 -1*2, 1*1 -2*0)
    assert((result1.second - expected_cross1).norm() < 1e-12);

    // Test 2: Orthogonal vectors (dot = 0)
    Eigen::Vector3d v2(1, 0, 0);
    Eigen::Vector3d w2(0, 1, 0);
    auto result2 = computeDotAndCross(v2, w2);
    assert(std::abs(result2.first) < 1e-12);
    Eigen::Vector3d expected_cross2(0, 0, 1); // x-axis cross y-axis = z-axis
    assert((result2.second - expected_cross2).norm() < 1e-12);

    // Test 3: Parallel vectors (cross = zero)
    Eigen::Vector3d v3(2, 4, 6);
    Eigen::Vector3d w3(1, 2, 3);
    auto result3 = computeDotAndCross(v3, w3);
    assert(std::abs(result3.first - 28.0) < 1e-12); // 2*1 + 4*2 + 6*3 = 28
    assert(result3.second.norm() < 1e-12); // cross should be zero vector

    // Test 4: Zero vector
    Eigen::Vector3d v4(0, 0, 0);
    Eigen::Vector3d w4(5, -3, 2);
    auto result4 = computeDotAndCross(v4, w4);
    assert(std::abs(result4.first) < 1e-12);
    assert(result4.second.norm() < 1e-12);

    // Test 5: Negative components and anti-parallel vectors
    Eigen::Vector3d v5(-1, 2, -3);
    Eigen::Vector3d w5(2, -4, 6); // w5 = -2 * v5, so anti-parallel
    auto result5 = computeDotAndCross(v5, w5);
    double expected_dot5 = (-1*2) + (2*-4) + (-3*6) = -2 -8 -18 = -28;
    assert(std::abs(result5.first + 28.0) < 1e-12);
    assert(result5.second.norm() < 1e-12); // cross of anti-parallel is zero

    // Test 6: Large values (ensure no overflow in the dot comparison)
    Eigen::Vector3d v6(1e100, -2e100, 3e100);
    Eigen::Vector3d w6(-1e100, 0.5e100, -2e100);
    auto result6 = computeDotAndCross(v6, w6);
    // Expected dot: 1e100 * -1e100 + -2e100 * 0.5e100 + 3e100 * -2e100 = -1e200 -1e200 -6e200 = -8e200
    assert(std::abs(result6.first + 8e200) / 8e200 < 1e-12); // relative tolerance
    // Cross product components: v6×w6 = ((y*z2 - z*y2), (z*x2 - x*z2), (x*y2 - y*x2))
    // y*z2 = -2e100 * -2e100 = 4e200; z*y2 = 3e100 * 0.5e100 = 1.5e200 → 2.5e200
    // z*x2 = 3e100 * -1e100 = -3e200; x*z2 = 1e100 * -2e100 = -2e200 → -3e200 - (-2e200) = -1e200
    // x*y2 = 1e100 * 0.5e100 = 0.5e200; y*x2 = -2e100 * -1e100 = 2e200 → -1.5e200
    Eigen::Vector3d expected_cross6(2.5e200, -1e200, -1.5e200);
    for (int i = 0; i < 3; ++i) {
        assert(std::abs(result6.second[i] - expected_cross6[i]) / (std::abs(expected_cross6[i]) + 1e-300) < 1e-12);
    }

    return 0;
}
// The solution uses Eigen’s built-in operations for vectors. For the dot product, we compute it in two ways: `v.dot(w)` and `v.adjoint() * w`. The latter returns a 1×1 matrix; converting it to a `double` is necessary for direct comparison. The cross product is computed via `v.cross(w)`, which returns a `Vector3d` perpendicular to both inputs, with magnitude equal to the product of magnitudes times the sine of the angle between them — if either vector is zero or they are parallel, the cross product is the zero vector. Edge cases: zero vectors produce dot=0 and cross={0,0,0} (valid); parallel vectors produce cross={0,0,0}; very large or small magnitudes can result in overflow or underflow, but Eigen handles standard `double` arithmetic without special handling — we only need to compare the two dot products with a tolerance to account for rounding differences. Time complexity is O(1) since operations are fixed-size (3 components). Space usage is O(1) auxiliary, but the returned pair contains a `Vector3d` (which is a fixed-size 24-byte object, so still constant space).
