Write a C++ function that, given two `Eigen::MatrixXf` matrices of identical dimensions, returns a new matrix representing the product of the two matrices, but using the `.noalias()` optimization when assigning the product to the result matrix to avoid a temporary and improve performance. The function must take const references to the two input matrices, ensure compatibility (throw a `std::invalid_argument` if dimensions don't match for multiplication), and return the result by value. The function should also demonstrate the difference between direct multiplication and the noalias version by computing both and returning a `std::pair<MatrixXf, MatrixXf>` where the first element is the plain multiplication result and the second is the noalias result, but both must be numerically identical. Use `Eigen`'s `MatrixXf` type and include necessary headers. The function must be const-correct and handle edge cases like empty matrices.

#include <Eigen/Dense>
#include <cassert>
#include <stdexcept>

int main() {
    // Test case 1: 2x2 matrix product.
    Eigen::MatrixXf A(2, 2);
    A << 2, 0, 0, 2;
    Eigen::MatrixXf B(2, 2);
    B << 1, 2, 3, 4;
    auto [plain1, noalias1] = multiplyWithAndWithoutNoalias(A, B);
    Eigen::MatrixXf expected1(2, 2);
    expected1 << 2, 4, 6, 8;
    assert(plain1 == expected1);
    assert(noalias1 == expected1);

    // Test case 2: Rectangular matrices (2x3 * 3x2).
    Eigen::MatrixXf C(2, 3);
    C << 1, 2, 3, 4, 5, 6;
    Eigen::MatrixXf D(3, 2);
    D << 7, 8, 9, 10, 11, 12;
    auto [plain2, noalias2] = multiplyWithAndWithoutNoalias(C, D);
    Eigen::MatrixXf expected2(2, 2);
    expected2 << 58, 64, 139, 154;
    assert(plain2 == expected2);
    assert(noalias2 == expected2);

    // Test case 3: Identity multiplication with a vector-like matrix.
    Eigen::MatrixXf I(3, 3);
    I.setIdentity();
    Eigen::MatrixXf V(3, 1);
    V << 1, 2, 3;
    auto [plain3, noalias3] = multiplyWithAndWithoutNoalias(I, V);
    assert(plain3 == V);
    assert(noalias3 == V);

    // Test case 4: Empty matrices (0x3 * 3x2 -> 0x2).
    Eigen::MatrixXf E1(0, 3);
    Eigen::MatrixXf E2(3, 2);
    E2 << 1, 2, 3, 4, 5, 6;
    auto [plain4, noalias4] = multiplyWithAndWithoutNoalias(E1, E2);
    assert(plain4.rows() == 0 && plain4.cols() == 2);
    assert(noalias4.rows() == 0 && noalias4.cols() == 2);

    // Test case 5: Dimension mismatch should throw.
    Eigen::MatrixXf F(2, 3);
    F.setZero();
    Eigen::MatrixXf G(4, 2);
    G.setZero();
    bool threw = false;
    try {
        multiplyWithAndWithoutNoalias(F, G);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test case 6: Large random matrix product (consistency check).
    Eigen::MatrixXf H = Eigen::MatrixXf::Random(5, 7);
    Eigen::MatrixXf K = Eigen::MatrixXf::Random(7, 4);
    auto [plain5, noalias5] = multiplyWithAndWithoutNoalias(H, K);
    assert(plain5.isApprox(noalias5, 1e-6));

    return 0;
}

#include <Eigen/Dense>
#include <stdexcept>
#include <utility>

// Compute the product of two Eigen matrices using both plain multiplication and
// the noalias() optimization, returning both results in a pair.
// The two returned matrices are numerically identical.
std::pair<Eigen::MatrixXf, Eigen::MatrixXf> multiplyWithAndWithoutNoalias(
    const Eigen::MatrixXf& A, const Eigen::MatrixXf& B) {
    // Validate dimensions: inner dimensions must match for multiplication.
    if (A.cols() != B.rows()) {
        throw std::invalid_argument("Matrix dimensions do not allow multiplication.");
    }

    // Plain multiplication: creates a temporary and copies.
    Eigen::MatrixXf plain_result = A * B;

    // noalias() version: writes directly into the result without a temporary.
    Eigen::MatrixXf noalias_result(A.rows(), B.cols());
    noalias_result.noalias() = A * B;

    return {plain_result, noalias_result};
}

// The task focuses on matrix multiplication using Eigen’s `MatrixXf` and specifically on the `noalias()` assignment optimization. The core algorithm is straightforward: for two matrices A (m×n) and B (n×p), the product C = A * B has dimensions m×p, where each element C(i,j) = Σ_{k=0}^{n-1} A(i,k)*B(k,j). The plain assignment `C = A * B` creates a temporary matrix for the product, then copies it into C, which is safe but slower. The `noalias()` version `C.noalias() = A * B` tells Eigen that the left-hand side does not alias the right-hand side, allowing Eigen to write directly into C without a temporary, but the user must ensure there is no alias (i.e., C is not a reference to A or B) to avoid data races. In our function, we create a fresh result matrix, so `noalias()` is safe. Edge cases: if either matrix is empty (0 rows or 0 columns), the product is also empty (0×0 or m×0 or 0×p) and should be handled gracefully. Also, dimension mismatch (A.cols() != B.rows()) must throw an exception. Time complexity is O(m*n*p) for standard multiplication, and space complexity is O(m*p) for the result (plus temporary if not using noalias). The function returns a pair of matrices; both must be equal numerically, which we can assert in tests.
