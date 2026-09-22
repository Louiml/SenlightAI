Write a C++ function that takes a fixed-size integer matrix (represented as an `Eigen::Matrix<int, 3, 3>`) and returns a new `Eigen::Matrix<int, 3, 3>` that is the symmetric matrix formed by reflecting the strictly upper triangular part (entries above the main diagonal) to the lower part, leaving the diagonal unchanged. The function must use Eigen's `selfadjointView<Upper>()` feature to extract the symmetric part, but also ensure the output is a standalone dense matrix (not a view). The function should be named `makeSymmetricFromUpper` and must be `const`-correct with respect to its input parameter. Handle edge cases like negative entries or zeros naturally (they are already handled by the matrix operations). Provide the implementation in a header-only style (no `main` function, but include necessary Eigen headers).
The main algorithm relies on Eigen's built-in `selfadjointView` functionality. A self-adjoint (symmetric) view of a matrix using `<Upper>` treats the upper triangular part (including the diagonal) as the defining data, and when converted back to a dense matrix via the `Matrix3i(...)` constructor, it fills the lower triangular part by mirroring the upper part's strictly upper entries across the diagonal. The function simply creates a const reference to the input matrix, calls `m.selfadjointView<Upper>()`, and constructs a new `Matrix3i` from that view. Edge cases such as negative numbers, zeros, or repeated values require no special handling because the operation is purely algebraic: it copies the upper triangle (diagonal and above) and reflects the above-diagonal entries to the below-diagonal positions. The diagonal entries remain unchanged. Time complexity is \(O(1)\) because the matrix size is fixed (3×3), and space complexity is also \(O(1)\) for the output matrix (plus constant temporary storage). No dynamic allocation beyond the returned matrix is needed.
#include <Eigen/Core>

// Return a symmetric matrix obtained by reflecting the strictly upper triangular
// part of the input matrix to the lower part. The diagonal remains unchanged.
Eigen::Matrix3i makeSymmetricFromUpper(const Eigen::Matrix3i& m) {
    // Use Eigen's selfadjointView<Upper> to treat the upper part as defining
    // a symmetric matrix, then materialize it into a dense matrix.
    return Eigen::Matrix3i(m.selfadjointView<Eigen::Upper>());
}
#include <Eigen/Core>
#include <cassert>

// Declaration of the function under test (already provided above)
Eigen::Matrix3i makeSymmetricFromUpper(const Eigen::Matrix3i& m);

int main() {
    // Test 1: Simple positive entries
    Eigen::Matrix3i m1;
    m1 << 1, 2, 3,
          0, 4, 5,
          0, 0, 6;
    Eigen::Matrix3i expected1;
    expected1 << 1, 2, 3,
                  2, 4, 5,
                  3, 5, 6;
    assert(makeSymmetricFromUpper(m1) == expected1);

    // Test 2: Negative and zero entries
    Eigen::Matrix3i m2;
    m2 << -1, 0, -2,
           0,  0,  4,
           0,  0, -3;
    Eigen::Matrix3i expected2;
    expected2 << -1,  0, -2,
                  0,  0,  4,
                 -2,  4, -3;
    assert(makeSymmetricFromUpper(m2) == expected2);

    // Test 3: All zeros (trivial symmetric)
    Eigen::Matrix3i m3 = Eigen::Matrix3i::Zero();
    assert(makeSymmetricFromUpper(m3) == Eigen::Matrix3i::Zero());

    // Test 4: Random matrix, verify symmetry and that upper part matches original
    Eigen::Matrix3i m4 = Eigen::Matrix3i::Random();
    Eigen::Matrix3i result4 = makeSymmetricFromUpper(m4);
    // Check symmetry
    assert(result4 == result4.transpose());
    // Check that the upper part (including diagonal) matches the original
    for (int i = 0; i < 3; ++i)
        for (int j = i; j < 3; ++j)
            assert(result4(i,j) == m4(i,j));

    // Test 5: Diagonal-only matrix (already symmetric)
    Eigen::Matrix3i m5;
    m5 << 1, 0, 0,
          0, 2, 0,
          0, 0, 3;
    assert(makeSymmetricFromUpper(m5) == m5);

    return 0;
}
