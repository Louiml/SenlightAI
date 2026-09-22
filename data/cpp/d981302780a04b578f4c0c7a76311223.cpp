Write a C++ function named `solveTriangularSystem` that accepts two `Eigen::Matrix3d` objects, `A` and `B`, and returns a `std::pair<Eigen::Matrix3d, Eigen::Matrix3d>`. The function must treat `A` as an upper-triangular matrix (ignoring any entries below the diagonal) and compute the solutions `X` and `Y` to the equations `A * X = B` (left solve) and `Y * A = B` (right solve). The returned pair should contain `X` first and `Y` second. Use Eigen's `triangularView` with the appropriate mode and solve direction. The function must be `const`-correct, not modify its inputs, and work correctly even if `B` is not symmetric or `A` has zeros on its diagonal (in which case the solver may produce `NaN` or `inf`, but the function should not crash and should still call the appropriate Eigen method).
// The core algorithm leverages Eigen's built-in triangular solver. For the left solve `A * X = B`, when `A` is upper-triangular, we use `A.triangularView<Eigen::Upper>().solve(B)`, which performs forward or backward substitution internally (specifically backward substitution for upper-triangular). For the right solve `Y * A = B`, we prepend the template parameter `<Eigen::OnTheRight>` to the `solve` call because the unknown matrix appears on the right side of the multiplication. The function returns both results in a `std::pair`. Edge cases include matrices with zero diagonal entries, where Eigen's solver may produce `NaN` or `inf` values—this is acceptable behavior for a square matrix with a singular triangular part; we do not need to validate invertibility. Time complexity is \(O(n^3)\) for matrix multiplication-like operations (here \(n=3\), so constant small work), and space complexity is \(O(n^2)\) for the result matrices. The function is straightforward and relies on Eigen's optimized and robust triangular solvers.
#include <Eigen/Dense>
#include <utility>

// Solve A * X = B and Y * A = B where A is treated as upper-triangular.
// Returns a pair containing (X, Y).
std::pair<Eigen::Matrix3d, Eigen::Matrix3d> solveTriangularSystem(
    const Eigen::Matrix3d& A, const Eigen::Matrix3d& B) {
    // Left solve: A * X = B. Since A is upper-triangular, use Upper mode.
    Eigen::Matrix3d X = A.triangularView<Eigen::Upper>().solve(B);

    // Right solve: Y * A = B. Solver with OnTheRight direction.
    Eigen::Matrix3d Y = A.triangularView<Eigen::Upper>().solve<Eigen::OnTheRight>(B);

    return {X, Y};
}
#include <Eigen/Dense>
#include <cassert>
#include <utility>

// Declare the function from the solution (in practice this would be in a header).
std::pair<Eigen::Matrix3d, Eigen::Matrix3d> solveTriangularSystem(
    const Eigen::Matrix3d& A, const Eigen::Matrix3d& B);

int main() {
    // Case 1: Simple upper-triangular with identity B.
    Eigen::Matrix3d A1;
    A1 << 2, 1, 0,
          0, 3, 1,
          0, 0, 4;
    Eigen::Matrix3d B1 = Eigen::Matrix3d::Identity();
    auto result1 = solveTriangularSystem(A1, B1);
    // Expected X = A^{-1} (upper-triangular inverse).
    Eigen::Matrix3d expectedX1;
    expectedX1 << 0.5, -1.0/6.0, 1.0/24.0,
                  0,   1.0/3.0, -1.0/12.0,
                  0,      0,      0.25;
    assert((result1.first - expectedX1).norm() < 1e-10);
    // For Y solve Y = B * A^{-1} = A^{-1} since B=I, so same as X.
    assert((result1.second - expectedX1).norm() < 1e-10);

    // Case 2: Non-trivial B, verify by multiplying.
    Eigen::Matrix3d A2;
    A2 << 1, 2, 3,
          0, 4, 5,
          0, 0, 6;
    Eigen::Matrix3d B2;
    B2 << 7, 8, 9,
          10, 11, 12,
          13, 14, 15;
    auto result2 = solveTriangularSystem(A2, B2);
    // Verify A * X = B.
    Eigen::Matrix3d checkLeft = A2.triangularView<Eigen::Upper>() * result2.first;
    assert((checkLeft - B2).norm() < 1e-10);
    // Verify Y * A = B.
    Eigen::Matrix3d checkRight = result2.second * A2.triangularView<Eigen::Upper>();
    assert((checkRight - B2).norm() < 1e-10);

    // Case 3: Diagonal matrix (special case of upper-triangular).
    Eigen::Matrix3d A3 = Eigen::Matrix3d::Zero();
    A3.diagonal() << 2, -3, 5;
    Eigen::Matrix3d B3;
    B3 << 1, 2, 3,
          -1, 0, 4,
          2, -2, 1;
    auto result3 = solveTriangularSystem(A3, B3);
    // Expected X = A^{-1} * B (diagonal inverse).
    Eigen::Matrix3d expectedX3;
    expectedX3 << 0.5, 1.0, 1.5,
                  1.0/3.0, 0.0, -4.0/3.0,
                  0.4, -0.4, 0.2;
    assert((result3.first - expectedX3).norm() < 1e-10);
    // Expected Y = B * A^{-1} (diagonal inverse on right).
    Eigen::Matrix3d expectedY3;
    expectedY3 << 0.5, -2.0/3.0, 0.6,
                  -0.5, 0.0, 0.8,
                  1.0, 2.0/3.0, 0.2;
    assert((result3.second - expectedY3).norm() < 1e-10);

    // Case 4: B is zero matrix, result should be zero.
    Eigen::Matrix3d A4;
    A4 << 1, 1, 1,
          0, 2, 1,
          0, 0, 3;
    Eigen::Matrix3d B4 = Eigen::Matrix3d::Zero();
    auto result4 = solveTriangularSystem(A4, B4);
    assert(result4.first.isZero(1e-12));
    assert(result4.second.isZero(1e-12));

    return 0;
}
