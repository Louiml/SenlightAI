/*
Write a C++ function that takes a square matrix `m` (represented as `Eigen::MatrixXi`) and returns an integer count of how many coefficients in `m` are equal to the corresponding coefficients of an identity matrix of the same size. The function must work for any square matrix size, including 0×0 (empty matrix, count = 0) and 1×1 (identity is [1]). You may assume the matrix is square; if not, throw `std::invalid_argument`. Do not modify the input matrix. The function signature should be `int countMatchesIdentity(const Eigen::MatrixXi& m);`. Use Eigen's coefficient-wise comparison and count method, not manual loops.
*/
#include <Eigen/Core>
#include <stdexcept>

// Returns the number of coefficients in square matrix m that match the
// coefficients of an identity matrix of the same size.
// Throws std::invalid_argument if m is not square.
// The input matrix is not modified.
int countMatchesIdentity(const Eigen::MatrixXi& m) {
    if (m.rows() != m.cols()) {
        throw std::invalid_argument("Matrix must be square");
    }

    // Use coefficient-wise comparison to identity matrix and count matches.
    return m.cwiseEqual(Eigen::MatrixXi::Identity(m.rows(), m.cols())).count();
}
#include <Eigen/Core>
#include <cassert>

// Declaration of the function under test
int countMatchesIdentity(const Eigen::MatrixXi& m);

int main() {
    // 2x2 identity matrix: all 4 coefficients match.
    Eigen::MatrixXi id2(2,2);
    id2 << 1, 0,
           0, 1;
    assert(countMatchesIdentity(id2) == 4);

    // 2x2 matrix from the snippet: only top-left coefficient matches (1 vs 1),
    // others fail: (0 vs 0) fails? Wait: (0 vs 0) is true, so actually:
    // Let's re-evaluate: matrix [[1,0],[1,1]] compared to identity [[1,0],[0,1]]:
    // (0,0):1==1 true, (0,1):0==0 true, (1,0):1==0 false, (1,1):1==1 true → count=3.
    Eigen::MatrixXi m(2,2);
    m << 1, 0,
         1, 1;
    assert(countMatchesIdentity(m) == 3);

    // All zeros matrix: only diagonal entries match? No, zeros vs identity:
    // (0,0):0==1 false, (0,1):0==0 true, (1,0):0==0 true, (1,1):0==1 false → count=2.
    Eigen::MatrixXi z(2,2);
    z << 0, 0,
         0, 0;
    assert(countMatchesIdentity(z) == 2);

    // 1x1 matrix with value 1: matches identity [1].
    Eigen::MatrixXi one(1,1);
    one << 1;
    assert(countMatchesIdentity(one) == 1);

    // 1x1 matrix with value 0: no match.
    Eigen::MatrixXi zero(1,1);
    zero << 0;
    assert(countMatchesIdentity(zero) == 0);

    // 3x3 identity matrix: all 9 match.
    Eigen::MatrixXi id3(3,3);
    id3 << 1,0,0,
           0,1,0,
           0,0,1;
    assert(countMatchesIdentity(id3) == 9);

    // 0x0 empty matrix: count is 0.
    Eigen::MatrixXi empty(0,0);
    assert(countMatchesIdentity(empty) == 0);

    // Non-square input throws.
    Eigen::MatrixXi nonSquare(2,3);
    nonSquare << 1,2,3,
                 4,5,6;
    bool threw = false;
    try {
        countMatchesIdentity(nonSquare);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
// The core idea is to use Eigen's built‑in coefficient‑wise comparison: `m.cwiseEqual(MatrixXi::Identity(m.rows(), m.cols()))` produces a boolean matrix with `true` where coefficients match (e.g., 1 must match 1, 0 must match 0). Then calling `.count()` returns the number of `true` entries. For an empty matrix (0×0), `MatrixXi::Identity(0,0)` is valid and produces an empty matrix; the count is zero. For any square matrix, the identity matrix has 1s on the diagonal and 0s elsewhere. Edge case: a square matrix of size 1×1 must have exactly one coefficient and the identity is `[1]`, so count is 1 if that coefficient is 1, else 0. Since the input is const‑ref, no modification is possible. Time complexity: O(n²) for an n×n matrix because the comparison and count traverse all n² coefficients; space complexity: O(n²) for the temporary boolean matrix (Eigen stores it dense). If the matrix is not square, we throw an exception. The solution uses no manual loops, relying on Eigen's vectorized operations for efficiency.
