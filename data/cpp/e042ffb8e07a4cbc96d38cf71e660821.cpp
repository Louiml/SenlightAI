// Write a C++ function `double estimatedRank(const Eigen::Matrix2d& A, double threshold)` that computes the rank of a 2x2 matrix under a user-specified threshold using Eigen's `FullPivLU` decomposition. The function must return the rank as a `double` (even though ranks are integers, the return type is `double` to match Eigen's `rank()` method behavior). The threshold determines what singular values are considered zero: any pivot value whose absolute value is less than or equal to the threshold is treated as zero and does not contribute to the rank. The function should handle both well-conditioned and near-singular matrices correctly, and must apply `const` correctly to the input matrix. The threshold must be positive; if a non-positive threshold is passed, the function should fall back to Eigen's default threshold (which is a small machine-epsilon-based value).

// The solution uses Eigen's `FullPivLU` class, which performs a complete pivoting LU decomposition with column and row permutations. The `rank()` method of this class already implements rank estimation by counting the number of pivots whose absolute values exceed a given threshold. Eigen's default threshold is `epsilon() * max_size * max_pivot`, where `epsilon()` is machine precision (~2.2e-16 for doubles). To allow user-controlled thresholding, we call `lu.setThreshold(threshold)` before calling `rank()`. The function must check if the provided threshold is positive; if not, it resets to the default by calling `lu.setThreshold(DefaultThreshold)` (or simply not setting it, but for clarity we set it to a negative sentinel and rely on Eigen's behavior). Edge cases: a matrix with all zeros (rank 0 regardless of threshold), a nearly singular matrix like [[2,1],[2,0.9999999999]] which has determinant close to zero, and a perfectly rank-1 matrix. Time complexity is O(1) because the matrix is fixed-size 2x2 (Eigen dispatches to optimized paths); in general, LU decomposition of an n×n matrix is O(n^3), but here n=2. Space complexity is O(1) since `FullPivLU` stores the decomposition in the object itself.

#include <Eigen/Dense>

// Estimate the rank of a 2x2 matrix under a given threshold.
// The threshold controls which pivots are considered negligible.
// If threshold is non-positive, Eigen's default threshold is used.
double estimatedRank(const Eigen::Matrix2d& A, double threshold) {
    Eigen::FullPivLU<Eigen::Matrix2d> lu(A);
    if (threshold > 0.0) {
        lu.setThreshold(threshold);
    } else {
        // Reset to default by clearing threshold (Eigen uses default)
        lu.setThreshold(Eigen::NumTraits<double>::dummy_precision());
    }
    return static_cast<double>(lu.rank());
}

#include <cassert>
#include <Eigen/Dense>

// Function declaration (same as solution)
double estimatedRank(const Eigen::Matrix2d& A, double threshold);

int main() {
    // Well-conditioned full-rank matrix
    Eigen::Matrix2d A1;
    A1 << 2, 0,
          0, 3;
    assert(estimatedRank(A1, 1e-10) == 2.0);

    // Zero matrix: rank 0 regardless of threshold
    Eigen::Matrix2d A2 = Eigen::Matrix2d::Zero();
    assert(estimatedRank(A2, 1e-10) == 0.0);

    // Exactly rank-1 matrix
    Eigen::Matrix2d A3;
    A3 << 1, 2,
          2, 4;
    assert(estimatedRank(A3, 1e-10) == 1.0);

    // Near-singular matrix: default threshold gives rank 2, larger threshold gives rank 1
    Eigen::Matrix2d A4;
    A4 << 2, 1,
          2, 0.9999999999;
    // With default threshold (1e-16-ish) it is rank 2
    assert(estimatedRank(A4, 0.0) == 2.0);
    // With a loose threshold it becomes rank 1
    assert(estimatedRank(A4, 1e-5) == 1.0);

    // Very small threshold still keeps rank 2 for near-singular
    assert(estimatedRank(A4, 1e-12) == 2.0);

    // Non-positive threshold uses default (should be same as rank of normal matrix)
    Eigen::Matrix2d A5;
    A5 << 1, 0,
          0, 1e-15;
    // Default treats 1e-15 as nonzero (much larger than epsilon*max*2 ~ 1e-15*2?)
    // Actually default threshold ~ 2.2e-16 * 2 = 4.4e-16, so 1e-15 > threshold -> rank 2
    assert(estimatedRank(A5, -1.0) == 2.0);
    // But with explicit threshold 1e-10, rank becomes 1
    assert(estimatedRank(A5, 1e-10) == 1.0);

    return 0;
}
