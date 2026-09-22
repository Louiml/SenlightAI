/*
Write a C++ function `makeCirculantMatrix` that accepts an `Eigen::MatrixBase<ArgType>` (a column vector) and returns a square matrix where each row is a cyclic shift of the input vector to the right, such that the first row equals the original vector and subsequent rows are shifted by one position. The function should work for vectors of arbitrary size (including size 1) and preserve the scalar type (e.g., `double`, `int`, `float`). The returned matrix must be square with dimensions `n x n` where `n` is the size of the input vector. The solution must be usable in Eigen expressions and avoid explicit loops in user code, leveraging `CwiseNullaryOp` and a custom functor.
*/
#include <Eigen/Core>
#include <cassert>

// Functor that returns a constant reference to an element of the original vector
// based on the cyclic index (col - row) mod size.
template<class ArgType>
class circulant_functor {
    const ArgType& m_vec;
public:
    circulant_functor(const ArgType& arg) : m_vec(arg) {}

    const typename ArgType::Scalar& operator() (Eigen::Index row, Eigen::Index col) const {
        // Compute cyclic index: (col - row) mod size, handling negative.
        Eigen::Index size = m_vec.size();
        Eigen::Index index = col - row;
        if (index < 0) index += size; // Since |index| < size, one addition suffices.
        // In rare case col-row could be >= size? No, because row,col < size, so col-row < size.
        return m_vec(index);
    }
};

// Helper to define the resulting matrix type as square and of the same scalar type.
template<class ArgType>
struct circulant_matrix_type {
    typedef Eigen::Matrix<typename ArgType::Scalar,
                          Eigen::Dynamic,
                          Eigen::Dynamic,
                          Eigen::ColMajor> type;
};

// Main function: returns a circulant matrix from a column vector.
template <class ArgType>
typename circulant_matrix_type<ArgType>::type
makeCirculantMatrix(const Eigen::MatrixBase<ArgType>& arg)
{
    typedef typename circulant_matrix_type<ArgType>::type MatrixType;
    // Ensure input is a vector (column or row).
    assert((arg.rows() == 1 || arg.cols() == 1) && "Input must be a vector");
    // Get the vector length: use arg.size() which works for both vectors.
    Eigen::Index n = arg.size();
    // Return nullary expression that lazily fills the matrix.
    return MatrixType::NullaryExpr(n, n, circulant_functor<ArgType>(arg.derived()));
}
#include <Eigen/Core>
#include <cassert>
#include <iostream>

// Assume the solution is placed above this test.

int main() {
    // Test 1: Basic double vector
    Eigen::VectorXd vec1(4);
    vec1 << 1.0, 2.0, 3.0, 4.0;
    Eigen::MatrixXd mat1 = makeCirculantMatrix(vec1);
    Eigen::MatrixXd expected1(4,4);
    expected1 << 1,2,3,4,
                 4,1,2,3,
                 3,4,1,2,
                 2,3,4,1;
    assert(mat1.isApprox(expected1));

    // Test 2: Single element vector (size 1)
    Eigen::VectorXd vec2(1);
    vec2 << 7.5;
    Eigen::MatrixXd mat2 = makeCirculantMatrix(vec2);
    assert(mat2.rows()==1 && mat2.cols()==1);
    assert(mat2(0,0) == 7.5);

    // Test 3: Integer vector
    Eigen::VectorXi vec3(3);
    vec3 << 5, -2, 0;
    Eigen::MatrixXi mat3 = makeCirculantMatrix(vec3);
    Eigen::MatrixXi expected3(3,3);
    expected3 << 5,-2,0,
                 0,5,-2,
                 -2,0,5;
    assert(mat3 == expected3);

    // Test 4: Row vector input (treated as vector)
    Eigen::RowVectorXd row(3);
    row << 1, 2, 3;
    Eigen::MatrixXd mat4 = makeCirculantMatrix(row);
    Eigen::MatrixXd expected4(3,3);
    expected4 << 1,2,3,
                 3,1,2,
                 2,3,1;
    assert(mat4.isApprox(expected4));

    // Test 5: Large size (100) to ensure no overflow
    int n = 100;
    Eigen::VectorXd big(n);
    for (int i=0; i<n; ++i) big(i) = i;
    Eigen::MatrixXd bigMat = makeCirculantMatrix(big);
    assert(bigMat.rows()==n && bigMat.cols()==n);
    // Check first row equals original
    for (int j=0; j<n; ++j) assert(bigMat(0,j) == big(j));
    // Check a cyclic shift property
    assert(bigMat(1,0) == big(n-1));

    // Test 6: Zero vector
    Eigen::VectorXd zero(3);
    zero.setZero();
    Eigen::MatrixXd matZero = makeCirculantMatrix(zero);
    assert(matZero.isZero());

    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The task is to generate a circulant matrix from a vector. A circulant matrix has the property that each row is a cyclic shift of the previous row. For an input vector `v` of length `n`, the element at position `(i, j)` is `v[(j - i) mod n]` (if zero-indexed). Alternatively, using row-major access, the element at `(row, col)` is `v[(col - row) mod n]`. Because Eigen supports nullary expressions, we can create a functor that, given `(row, col)`, computes the correct index into the original vector using modular arithmetic. The functor returns a constant reference to the scalar at that index. The main edge case is that if the input vector size is zero (though typically a vector is non-empty), we should ensure the functor does not cause division by zero; however, by assumption the vector has at least one element. The returned matrix type must match the scalar type and be dynamic-sized (or compile-time if the input is fixed-size). Time complexity is `O(1)` per element access, and total time to construct the matrix is `O(n^2)` because the matrix has `n^2` elements. Space complexity is `O(n)` for the underlying vector storage plus `O(1)` for the functor.
