// Write a standalone C++ function that takes a compile-time fixed matrix `Eigen::Matrix<double, 5, 3>` and returns a `std::tuple<Eigen::Matrix<double, 5, 5>, Eigen::Matrix<double, 5, 3>, Eigen::Matrix<int, 5, 1>, Eigen::Matrix<int, 3, 1>>` containing, respectively: the L part (a 5x5 lower-triangular matrix with ones on the diagonal, where the strictly lower part comes from the LU decomposition of the input), the U part (a 5x3 upper-triangular matrix), the inverse permutation vector for rows (as an index list of length 5, where element i gives the row index in the original matrix that should be placed at row i after applying the row permutation), and the inverse permutation vector for columns (as an index list of length 3, similarly defined). The function must reconstruct the original matrix exactly when multiplied as `P_inv * L * U * Q_inv` (where `P_inv` and `Q_inv` are permutation matrices built from the provided vectors). The function must use Eigen's `FullPivLU` decomposition. Assume the input matrix has full rank and is 5x3. Ensure the returned L and U are of the correct types and that the permutation vectors are of type `Eigen::Matrix<int, Dynamic, 1>` (or equivalent) but with sizes 5 and 3 respectively. The function must be `const`-correct and must not modify the input.
#include <cassert>
#include <Eigen/Dense>
#include <tuple>

// Declare the function to be tested (assume it's defined in the solution above)
// (In a real integration, include the solution header here.)

int main() {
    // Test case 1: Random full-rank matrix
    Eigen::Matrix<double, 5, 3> m1;
    m1 << 1, 0, 2,
          0, 3, 1,
          2, 1, 4,
          1, 2, 0,
          0, 1, 3;

    auto [L1, U1, Pv1, Qv1] = decompose_with_full_pivoting(m1);
    // Reconstruct and verify
    Eigen::Matrix<double, 5, 5> P1 = Eigen::PermutationMatrix<5>(Pv1).inverse().toDenseMatrix();
    Eigen::Matrix<double, 3, 3> Q1 = Eigen::PermutationMatrix<3>(Qv1).inverse().toDenseMatrix();
    Eigen::Matrix<double, 5, 3> recon1 = P1 * L1 * U1 * Q1;
    assert(recon1.isApprox(m1, 1e-12));

    // Test case 2: Simple matrix with clear structure
    Eigen::Matrix<double, 5, 3> m2;
    m2 << 1, 0, 0,
          0, 1, 0,
          0, 0, 1,
          0, 0, 0,
          0, 0, 0; // rank 3

    auto [L2, U2, Pv2, Qv2] = decompose_with_full_pivoting(m2);
    Eigen::Matrix<double, 5, 5> P2 = Eigen::PermutationMatrix<5>(Pv2).inverse().toDenseMatrix();
    Eigen::Matrix<double, 3, 3> Q2 = Eigen::PermutationMatrix<3>(Qv2).inverse().toDenseMatrix();
    Eigen::Matrix<double, 5, 3> recon2 = P2 * L2 * U2 * Q2;
    assert(recon2.isApprox(m2, 1e-12));

    // Test case 3: All ones matrix (rank 1, but still decomposable via FullPivLU)
    Eigen::Matrix<double, 5, 3> m3 = Eigen::Matrix<double, 5, 3>::Ones();
    auto [L3, U3, Pv3, Qv3] = decompose_with_full_pivoting(m3);
    Eigen::Matrix<double, 5, 5> P3 = Eigen::PermutationMatrix<5>(Pv3).inverse().toDenseMatrix();
    Eigen::Matrix<double, 3, 3> Q3 = Eigen::PermutationMatrix<3>(Qv3).inverse().toDenseMatrix();
    Eigen::Matrix<double, 5, 3> recon3 = P3 * L3 * U3 * Q3;
    assert(recon3.isApprox(m3, 1e-12));

    // Test case 4: Verify L has ones on diagonal and is lower-triangular
    auto [L4, U4, Pv4, Qv4] = decompose_with_full_pivoting(m1);
    for (int i = 0; i < 5; ++i) {
        assert(std::abs(L4(i, i) - 1.0) < 1e-12);
        for (int j = i + 1; j < 5; ++j) {
            assert(std::abs(L4(i, j)) < 1e-12);
        }
    }

    // Test case 5: Verify U is upper-triangular (zero below diagonal for the 3x3 part)
    for (int j = 0; j < 3; ++j) {
        for (int i = j + 1; i < 3; ++i) {
            assert(std::abs(U4(i, j)) < 1e-12);
        }
    }

    // Test case 6: Verify permutation vectors have correct sizes
    assert(Pv4.size() == 5);
    assert(Qv4.size() == 3);

    // Test case 7: Verify with a different random matrix
    Eigen::Matrix<double, 5, 3> m5 = Eigen::Matrix<double, 5, 3>::Random();
    auto [L5, U5, Pv5, Qv5] = decompose_with_full_pivoting(m5);
    Eigen::Matrix<double, 5, 5> P5 = Eigen::PermutationMatrix<5>(Pv5).inverse().toDenseMatrix();
    Eigen::Matrix<double, 3, 3> Q5 = Eigen::PermutationMatrix<3>(Qv5).inverse().toDenseMatrix();
    Eigen::Matrix<double, 5, 3> recon5 = P5 * L5 * U5 * Q5;
    assert(recon5.isApprox(m5, 1e-12));

    // Test case 8: Check that the input is not modified (pass by const ref)
    // (implicitly verified by the function signature)

    // Test case 9: Additional check on permutation vectors being valid permutations
    auto check_perm = [](const auto& vec, int size) {
        std::vector<int> seen(size, 0);
        for (int i = 0; i < vec.size(); ++i) {
            int val = vec[i];
            assert(val >= 0 && val < size);
            assert(seen[val] == 0);
            seen[val] = 1;
        }
    };
    check_perm(Pv4, 5);
    check_perm(Qv4, 3);

    // Test case 10: Compare with Eigen's own reconstruction via lu
    Eigen::FullPivLU<Eigen::Matrix<double, 5, 3>> lu_check(m1);
    auto [L10, U10, Pv10, Qv10] = decompose_with_full_pivoting(m1);
    Eigen::Matrix<double, 5, 5> P10 = Eigen::PermutationMatrix<5>(Pv10).inverse().toDenseMatrix();
    Eigen::Matrix<double, 3, 3> Q10 = Eigen::PermutationMatrix<3>(Qv10).inverse().toDenseMatrix();
    assert((P10 * L10 * U10 * Q10).isApprox(lu_check.permutationP().inverse() * L10 * U10 * lu_check.permutationQ().inverse(), 1e-12));

    return 0;
}
#include <Eigen/Dense>
#include <tuple>

// Returns L (5x5), U (5x3), row inverse permutation (vector of length 5), column inverse permutation (vector of length 3)
// such that input = P_inv * L * U * Q_inv, where P_inv and Q_inv are permutation matrices built from the returned vectors.
std::tuple<Eigen::Matrix<double, 5, 5>, Eigen::Matrix<double, 5, 3>, Eigen::Matrix<int, Eigen::Dynamic, 1>, Eigen::Matrix<int, Eigen::Dynamic, 1>>
decompose_with_full_pivoting(const Eigen::Matrix<double, 5, 3>& m) {
    using Matrix5x3 = Eigen::Matrix<double, 5, 3>;
    using Matrix5x5 = Eigen::Matrix<double, 5, 5>;

    Eigen::FullPivLU<Matrix5x3> lu(m);

    // Extract L: 5x5 identity with strictly lower part from matrixLU()
    Matrix5x5 l = Matrix5x5::Identity();
    l.block<5, 3>(0, 0).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();

    // Extract U: upper triangular view of matrixLU() (valid only for first 3 columns)
    Matrix5x3 u = lu.matrixLU().triangularView<Eigen::Upper>();

    // Inverse permutations as index vectors
    Eigen::PermutationMatrix<5> p_inv = lu.permutationP().inverse();
    Eigen::PermutationMatrix<3> q_inv = lu.permutationQ().inverse();
    Eigen::Matrix<int, Eigen::Dynamic, 1> p_vec = p_inv.indices().cast<int>();
    Eigen::Matrix<int, Eigen::Dynamic, 1> q_vec = q_inv.indices().cast<int>();

    return std::make_tuple(l, u, p_vec, q_vec);
}
// The solution uses Eigen's dense LU decomposition with full pivoting, which is appropriate for a rectangular matrix (5 rows, 3 columns). The `FullPivLU` object provides `matrixLU()`, which contains both L and U in a packed format: the strictly lower-triangular part of this matrix (rows 0-4, columns 0-2) is the L part without the diagonal, and the upper-triangular part (rows 0-2, columns 0-2) is the U part. We extract L as a 5x5 identity matrix and assign the strictly lower part using a `triangularView<StrictlyLower>()` with an assignment from the corresponding block of `matrixLU()`. U is extracted directly as a triangular view of `matrixLU()` restricted to the upper part (limited to the first 3 columns since the matrix is 5x3). For permutations, `FullPivLU` provides `permutationP()` and `permutationQ()` which are permutation matrices. To get the inverse permutation vectors, we can use the `.indices()` method (or `transpose().indices()` for inverse) — but the typical approach is to compute the permutation vectors from the permutation matrices by finding for each output row which input row maps to it. Alternatively, we can use `lu.permutationP().transpose().indices()` and `lu.permutationQ().transpose().indices()`. Since the task requires inverse permutations, we can directly call `lu.permutationP().inverse()` but that returns a permutation matrix; to get the index vector, we can do `Eigen::PermutationMatrix<5> p_inv = lu.permutationP().inverse();` then `p_inv.indices()`. The same for Q. Edge cases: the input matrix must be full rank, but `FullPivLU` will handle any matrix; however, for full rank 5x3, the LU decomposition is unique up to permutations. The algorithm operates in O(5*3*min(5,3)) = O(15) operations for the decomposition, which is constant. Space complexity is O(5*5 + 5*3) for the output matrices, also constant. The reconstruction verification would multiply the permutations and matrices back together.
