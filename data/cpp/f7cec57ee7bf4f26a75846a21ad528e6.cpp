// Write a C++ function `int rank_with_threshold(const Eigen::MatrixXd& A, double threshold)` that computes the numerical rank of a matrix using a complete pivoting LU decomposition with an explicitly specified rank threshold. The function should accept any real-valued matrix (including non-square matrices), compute the rank by counting the number of singular values (or pivot magnitudes in the LU factorization) that exceed the given threshold, and return that integer rank. The threshold must be a positive value; if it is non-positive, the function should use the default behavior of treating values below machine epsilon as zero. This task mirrors the behavior shown in the snippet, where the rank of a nearly singular matrix changes when the threshold is adjusted. Ensure your implementation handles edge cases like zero matrices, rank-deficient matrices, and matrices with more rows than columns or vice versa.

// The solution uses Eigen's `FullPivLU` class, which performs a complete pivoting LU decomposition and provides a `rank()` method. However, `rank()` by default uses an internal threshold based on the machine epsilon times the largest pivot magnitude. To allow a custom threshold, we can either call `setThreshold()` on the LU object before calling `rank()`, or we can manually compute the rank by examining the vector of pivot magnitudes. The cleaner approach is to use `FullPivLU::setThreshold()` because it handles all underlying numerical details consistently. The algorithm: (1) construct a `FullPivLU` object from the input matrix; (2) if the given threshold is positive, call `lu.setThreshold(threshold)`; (3) call `lu.rank()` and return it. Important edge cases: a zero threshold or negative threshold should fall back to default behavior (we can just not call `setThreshold` in that case). The rank is always an integer between 0 and `min(rows, cols)`. Time complexity is \(O(m n \min(m,n))\) for the LU decomposition, and space complexity is \(O(m n)\) for storing the matrix and pivot data. The function returns an `int` and does not modify the input matrix, so we pass by const reference.

#include <Eigen/Dense>
#include <stdexcept>

/**
 * Compute the numerical rank of a matrix using FullPivLU with an optional custom threshold.
 * 
 * @param A         The input matrix (any size).
 * @param threshold Positive value for rank determination. If <= 0, Eigen's default threshold is used.
 * @return The integer rank of the matrix.
 */
int rank_with_threshold(const Eigen::MatrixXd& A, double threshold) {
    // Perform complete pivoting LU decomposition
    Eigen::FullPivLU<Eigen::MatrixXd> lu(A);
    
    // Apply custom threshold only if it's positive; otherwise keep default
    if (threshold > 0.0) {
        lu.setThreshold(threshold);
    }
    
    // Return the computed rank
    return lu.rank();
}

#include <cassert>
#include <Eigen/Dense>

// Declare the function under test
int rank_with_threshold(const Eigen::MatrixXd& A, double threshold);

int main() {
    // Case 1: Full-rank 2x2 matrix
    Eigen::MatrixXd A1(2,2);
    A1 << 1, 0, 0, 1;
    assert(rank_with_threshold(A1, 1e-9) == 2);

    // Case 2: Exactly singular 2x2 matrix
    Eigen::MatrixXd A2(2,2);
    A2 << 1, 1, 2, 2;
    assert(rank_with_threshold(A2, 1e-9) == 1);

    // Case 3: Nearly singular matrix (from the snippet), default threshold detects rank 2
    Eigen::MatrixXd A3(2,2);
    A3 << 2, 1,
          2, 0.9999999999;
    // With default threshold, rank is 2 because 0.9999999999 is considered non-zero
    assert(rank_with_threshold(A3, 0.0) == 2);

    // Case 4: Same matrix with a large threshold forces rank 1
    assert(rank_with_threshold(A3, 1e-5) == 1);

    // Case 5: Zero matrix has rank 0
    Eigen::MatrixXd A4(2,3);
    A4.setZero();
    assert(rank_with_threshold(A4, 1e-9) == 0);

    // Case 6: Non-square matrix (3x2) full rank
    Eigen::MatrixXd A5(3,2);
    A5 << 1, 0,
          0, 1,
          0, 0;
    assert(rank_with_threshold(A5, 1e-9) == 2);

    // Case 7: Large matrix identity
    Eigen::MatrixXd A6 = Eigen::MatrixXd::Identity(5,5);
    assert(rank_with_threshold(A6, 1e-9) == 5);

    // Case 8: Single element matrix
    Eigen::MatrixXd A7(1,1);
    A7 << 0.0;
    assert(rank_with_threshold(A7, 1e-9) == 0);

    return 0;
}
