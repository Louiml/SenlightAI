// Write a C++ function named `applyScalarOperations` that takes a 2x2 matrix of doubles and a vector of exactly 3 doubles, and returns a `std::pair<Eigen::Matrix2d, Eigen::Vector3d>` where the matrix is the original matrix multiplied by 2.5 (element-wise scalar multiplication), and the vector is the original vector multiplied by 0.1 and then also multiplied by 2 (i.e., the final vector equals the original vector scaled by 0.2). The function must not modify the input arguments. Use Eigen's dense matrix and vector types. Assume the inputs are correctly sized. The function should be `const`-correct: parameters passed by const reference, and the returned pair holds the results.

#include <cassert>
#include <cmath>
#include <Eigen/Dense>
#include <utility>

// The solution function (declared above) is assumed available.

int main() {
    Eigen::Matrix2d a;
    a << 1, 2,
         3, 4;
    Eigen::Vector3d v(1, 2, 3);

    auto result = applyScalarOperations(a, v);
    Eigen::Matrix2d expectedMat;
    expectedMat << 2.5, 5.0,
                   7.5, 10.0;
    Eigen::Vector3d expectedVec(0.2, 0.4, 0.6);

    assert(result.first.isApprox(expectedMat, 1e-12));
    assert(result.second.isApprox(expectedVec, 1e-12));

    // Ensure original inputs are unchanged.
    Eigen::Matrix2d originalA = a;
    Eigen::Vector3d originalV = v;
    assert(a.isApprox(originalA, 1e-12));
    assert(v.isApprox(originalV, 1e-12));

    // Test with negative values.
    Eigen::Matrix2d b;
    b << -1.0, 0.5,
         2.0, -3.0;
    Eigen::Vector3d w(-2.0, 4.0, -6.0);
    auto result2 = applyScalarOperations(b, w);
    Eigen::Matrix2d expectedMat2;
    expectedMat2 << -2.5, 1.25,
                    5.0, -7.5;
    Eigen::Vector3d expectedVec2(-0.4, 0.8, -1.2);
    assert(result2.first.isApprox(expectedMat2, 1e-12));
    assert(result2.second.isApprox(expectedVec2, 1e-12));

    // Test with zero values.
    Eigen::Matrix2d c = Eigen::Matrix2d::Zero();
    Eigen::Vector3d u = Eigen::Vector3d::Zero();
    auto result3 = applyScalarOperations(c, u);
    assert(result3.first.isZero(1e-12));
    assert(result3.second.isZero(1e-12));

    // Test with exact equality for known simple numbers.
    Eigen::Matrix2d d;
    d << 2, 4,
         6, 8;
    Eigen::Vector3d t(10, 20, 30);
    auto result4 = applyScalarOperations(d, t);
    Eigen::Matrix2d expectedMat4;
    expectedMat4 << 5, 10,
                    15, 20;
    Eigen::Vector3d expectedVec4(2, 4, 6);
    assert(result4.first == expectedMat4);
    assert(result4.second == expectedVec4);
}

#include <Eigen/Dense>
#include <utility>

// Apply scalar multiplications to a matrix and vector without modifying inputs.
// Returns a pair: (matrix * 2.5, (vector * 0.1) * 2).
std::pair<Eigen::Matrix2d, Eigen::Vector3d> applyScalarOperations(
    const Eigen::Matrix2d& mat,
    const Eigen::Vector3d& vec)
{
    Eigen::Matrix2d scaledMatrix = mat * 2.5;
    Eigen::Vector3d scaledVector = (vec * 0.1) * 2.0;
    return {scaledMatrix, scaledVector};
}

// The task requires performing scalar multiplication on Eigen types. Eigen's `operator*` supports scalar multiplication directly for both matrices and vectors, returning a new object without modifying the original. For the matrix, multiply by 2.5. For the vector, first multiply by 0.1, then multiply the result by 2 (equivalent to multiplying the original by 0.2). Since the vector is of fixed size `Vector3d` and matrix is `Matrix2d`, no dynamic allocation occurs. Edge cases: input values are finite doubles; if any are NaN or infinity, the result propagates naturally. Time complexity is O(1) because the matrix and vector sizes are fixed. Space complexity is O(1) for the output pair, plus temporary Eigen objects that are also constant size. No special handling for sizes is needed because Eigen's fixed-size types are used.
