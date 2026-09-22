/*
Write a standalone C++ function that concatenates two Eigen sparse matrices along a given dimension (1 for vertical concatenation of rows, 2 for horizontal concatenation of columns). The function must handle empty input matrices by returning the other matrix unchanged, assert that the dimensions are compatible (same number of columns for dim=1, same number of rows for dim=2), and produce a correctly sized compressed sparse result with all nonzero entries preserved in the correct order. The function should be templated on the scalar type (e.g., `double`, `int`) and must work with `Eigen::SparseMatrix<Scalar>`. The implementation must be efficient, avoiding dense intermediate representations, and must correctly handle matrices with duplicate coordinates (if any) by summing them during triplet insertion.
*/
#include <Eigen/SparseCore>
#include <vector>
#include <cassert>

// Concatenate two Eigen sparse matrices along dimension 1 (vertical) or 2 (horizontal).
// The result C is a new compressed sparse matrix.
// A and B must have compatible dimensions.
template <typename Scalar>
void concatSparse(
    const int dim,
    const Eigen::SparseMatrix<Scalar>& A,
    const Eigen::SparseMatrix<Scalar>& B,
    Eigen::SparseMatrix<Scalar>& C)
{
    assert(dim == 1 || dim == 2);

    // Handle empty input matrices
    if (A.size() == 0) {
        C = B;
        return;
    }
    if (B.size() == 0) {
        C = A;
        return;
    }

    // Determine output dimensions and check compatibility
    Eigen::Index rows_out, cols_out;
    if (dim == 1) {
        assert(A.cols() == B.cols());
        rows_out = A.rows() + B.rows();
        cols_out = A.cols();
    } else { // dim == 2
        assert(A.rows() == B.rows());
        rows_out = A.rows();
        cols_out = A.cols() + B.cols();
    }

    // Collect all nonzeros as triplets
    std::vector<Eigen::Triplet<Scalar>> triplets;
    triplets.reserve(A.nonZeros() + B.nonZeros());

    // Add A's nonzeros
    for (int k = 0; k < A.outerSize(); ++k) {
        for (typename Eigen::SparseMatrix<Scalar>::InnerIterator it(A, k); it; ++it) {
            triplets.emplace_back(it.row(), it.col(), it.value());
        }
    }

    // Add B's nonzeros with appropriate offset
    const Eigen::Index row_offset = (dim == 1) ? A.rows() : 0;
    const Eigen::Index col_offset = (dim == 2) ? A.cols() : 0;
    for (int k = 0; k < B.outerSize(); ++k) {
        for (typename Eigen::SparseMatrix<Scalar>::InnerIterator it(B, k); it; ++it) {
            triplets.emplace_back(it.row() + row_offset, it.col() + col_offset, it.value());
        }
    }

    // Build the output matrix from triplets
    C = Eigen::SparseMatrix<Scalar>(rows_out, cols_out);
    C.setFromTriplets(triplets.begin(), triplets.end());
}
#include <Eigen/SparseCore>
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (or included)
// Include the function definition here for completeness
// ... (paste the solution code here)

int main() {
    // Test 1: Basic vertical concatenation of two 2x2 sparse matrices
    Eigen::SparseMatrix<double> A(2, 2);
    std::vector<Eigen::Triplet<double>> tripsA;
    tripsA.emplace_back(0, 0, 1.0);
    tripsA.emplace_back(1, 1, 2.0);
    A.setFromTriplets(tripsA.begin(), tripsA.end());

    Eigen::SparseMatrix<double> B(2, 2);
    std::vector<Eigen::Triplet<double>> tripsB;
    tripsB.emplace_back(0, 1, 3.0);
    tripsB.emplace_back(1, 0, 4.0);
    B.setFromTriplets(tripsB.begin(), tripsB.end());

    Eigen::SparseMatrix<double> C1;
    concatSparse(1, A, B, C1);
    assert(C1.rows() == 4 && C1.cols() == 2);
    assert(C1.nonZeros() == 4);
    assert(C1.coeff(0, 0) == 1.0);
    assert(C1.coeff(1, 1) == 2.0);
    assert(C1.coeff(2, 1) == 3.0);
    assert(C1.coeff(3, 0) == 4.0);

    // Test 2: Horizontal concatenation of two 2x2 matrices
    Eigen::SparseMatrix<double> C2;
    concatSparse(2, A, B, C2);
    assert(C2.rows() == 2 && C2.cols() == 4);
    assert(C2.nonZeros() == 4);
    assert(C2.coeff(0, 0) == 1.0);
    assert(C2.coeff(1, 1) == 2.0);
    assert(C2.coeff(0, 3) == 3.0);  // B's (0,1) goes to col 2+1=3
    assert(C2.coeff(1, 2) == 4.0);  // B's (1,0) goes to col 2+0=2

    // Test 3: Empty A (returns B)
    Eigen::SparseMatrix<double> Empty(0, 2);
    Eigen::SparseMatrix<double> C3;
    concatSparse(1, Empty, A, C3);
    assert(C3.rows() == 2 && C3.cols() == 2);
    assert(C3.nonZeros() == 2);
    assert(C3.coeff(0, 0) == 1.0);
    assert(C3.coeff(1, 1) == 2.0);

    // Test 4: Empty B (returns A)
    Eigen::SparseMatrix<double> Empty2(2, 0);
    Eigen::SparseMatrix<double> C4;
    concatSparse(2, A, Empty2, C4);
    assert(C4.rows() == 2 && C4.cols() == 2);
    assert(C4.nonZeros() == 2);
    assert(C4.coeff(0, 0) == 1.0);
    assert(C4.coeff(1, 1) == 2.0);

    // Test 5: Duplicate entries across A and B at the same position after concatenation
    Eigen::SparseMatrix<double> D(1, 2);
    std::vector<Eigen::Triplet<double>> tripsD;
    tripsD.emplace_back(0, 0, 5.0);
    tripsD.emplace_back(0, 1, 6.0);
    D.setFromTriplets(tripsD.begin(), tripsD.end());

    Eigen::SparseMatrix<double> E(1, 2);
    std::vector<Eigen::Triplet<double>> tripsE;
    tripsE.emplace_back(0, 0, 7.0);
    tripsE.emplace_back(0, 1, 8.0);
    E.setFromTriplets(tripsE.begin(), tripsE.end());

    Eigen::SparseMatrix<double> C5;
    concatSparse(1, D, E, C5);
    // After vertical concatenation, coordinates are (0,0), (0,1), (1,0), (1,1) — no duplicates
    assert(C5.rows() == 2 && C5.cols() == 2);
    assert(C5.nonZeros() == 4);
    assert(C5.coeff(0, 0) == 5.0);
    assert(C5.coeff(0, 1) == 6.0);
    assert(C5.coeff(1, 0) == 7.0);
    assert(C5.coeff(1, 1) == 8.0);

    // Test 6: Non-square matrices (3x2 and 3x4 horizontal)
    Eigen::SparseMatrix<int> F(3, 2);
    std::vector<Eigen::Triplet<int>> tripsF;
    tripsF.emplace_back(0, 0, 1);
    tripsF.emplace_back(2, 1, 2);
    F.setFromTriplets(tripsF.begin(), tripsF.end());

    Eigen::SparseMatrix<int> G(3, 4);
    std::vector<Eigen::Triplet<int>> tripsG;
    tripsG.emplace_back(1, 2, 3);
    tripsG.emplace_back(2, 3, 4);
    G.setFromTriplets(tripsG.begin(), tripsG.end());

    Eigen::SparseMatrix<int> C6;
    concatSparse(2, F, G, C6);
    assert(C6.rows() == 3 && C6.cols() == 6);
    assert(C6.nonZeros() == 4);
    assert(C6.coeff(0, 0) == 1);
    assert(C6.coeff(2, 1) == 2);
    assert(C6.coeff(1, 4) == 3);  // G's (1,2) -> col 2+2=4
    assert(C6.coeff(2, 5) == 4);  // G's (2,3) -> col 3+3=5

    return 0;
}
// The solution approach uses the triplet insertion method, which is robust and efficient for sparse matrices. The main algorithm is:
// 1. If `dim` is not 1 or 2, assert or handle error (we'll use `assert`).
// 2. Handle empty matrices: if `A.size() == 0`, set `C = B`; if `B.size() == 0`, set `C = A`. Note that a matrix with zero rows or zero columns has `size() == 0`.
// 3. Determine the output dimensions: for `dim == 1`, `C` has `A.rows() + B.rows()` rows and `A.cols()` columns (and assert `A.cols() == B.cols()`); for `dim == 2`, `C` has `A.rows()` rows and `A.cols() + B.cols()` columns (and assert `A.rows() == B.rows()`).
// 4. Collect all nonzeros from `A` into triplets with their original `(row, col)` coordinates. For `B`, offset the coordinates: if `dim == 1`, add `A.rows()` to the row index; if `dim == 2`, add `A.cols()` to the column index.
// 5. Reserve capacity for the expected number of nonzeros (`A.nonZeros() + B.nonZeros()`), then construct `C` via `setFromTriplets`, which automatically sums duplicate entries.
// 6. The output is automatically in compressed column-major format (Eigen's default).
//
// Edge cases:
// - Empty matrices: handled before any iteration.
// - Matrices with zero rows or columns but nonzero `size()`? Actually `size()` is `rows()*cols()`, so if any dimension is zero, `size()` is zero, so handled.
// - Duplicate entries within `A` or `B` (or across both) are summed by `setFromTriplets`, which is correct behavior.
// - Non-square matrices: handled by dimension checks.
// - Scalars of any numeric type: templated.
//
// Time complexity: \(O(\text{nnz}(A) + \text{nnz}(B))\) to iterate over nonzeros and build triplets, plus \(O(\text{nnz}(A) + \text{nnz}(B))\) for `setFromTriplets` (which sorts internally, making it \(O(n \log n)\) in the worst case with `nnz` total nonzeros, but typically efficient). Space complexity: \(O(\text{nnz}(A) + \text{nnz}(B))\) for the triplet vector and output matrix.
