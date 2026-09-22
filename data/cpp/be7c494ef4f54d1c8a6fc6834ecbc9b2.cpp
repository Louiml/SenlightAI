/*
Write a C++ function `selectRowsColumns` that takes a dense Eigen matrix `A` of integer scalars, a row-index array `rowIndices`, and a column-index array `colIndices` (both Eigen dense arrays of integer type, e.g., `ArrayXi` or `Array3i`), and returns a new matrix `B` of the same scalar type where `B(i,j) = A(rowIndices(i), colIndices(j))`. The function must work for any sizes of row and column index arrays (including sizes that differ from the source matrix dimensions). Indices are assumed to be valid (no bounds checking required). The function must support both compile-time fixed-size and dynamic-size index arrays, and must be usable in expression contexts (i.e., it should return an Eigen expression that can be assigned to a `MatrixXi`). Do not use `Eigen::Map` or manual loops inside the function; instead, implement the indexing via a custom Eigen nullary functor similar to the provided snippet. Ensure the solution compiles with C++11 or later.
*/

#include <Eigen/Core>

// Custom functor for row/column indexing of a dense matrix.
template<typename ArgType, typename RowIndexType, typename ColIndexType>
class row_col_indexing_functor {
    const ArgType& m_arg;
    const RowIndexType& m_rowIndices;
    const ColIndexType& m_colIndices;

public:
    typedef Matrix<typename ArgType::Scalar,
                   RowIndexType::SizeAtCompileTime,
                   ColIndexType::SizeAtCompileTime,
                   ArgType::Flags & Eigen::RowMajorBit ? Eigen::RowMajor : Eigen::ColMajor,
                   RowIndexType::MaxSizeAtCompileTime,
                   ColIndexType::MaxSizeAtCompileTime> MatrixType;

    row_col_indexing_functor(const ArgType& arg,
                             const RowIndexType& row_indices,
                             const ColIndexType& col_indices)
        : m_arg(arg), m_rowIndices(row_indices), m_colIndices(col_indices) {}

    const typename ArgType::Scalar& operator()(Eigen::Index row, Eigen::Index col) const {
        return m_arg(m_rowIndices[row], m_colIndices[col]);
    }
};

// Returns a lazy expression that selects the given rows and columns from `arg`.
template<typename ArgType, typename RowIndexType, typename ColIndexType>
Eigen::CwiseNullaryOp<row_col_indexing_functor<ArgType, RowIndexType, ColIndexType>,
                      typename row_col_indexing_functor<ArgType, RowIndexType, ColIndexType>::MatrixType>
selectRowsColumns(const Eigen::MatrixBase<ArgType>& arg,
                  const RowIndexType& row_indices,
                  const ColIndexType& col_indices) {
    typedef row_col_indexing_functor<ArgType, RowIndexType, ColIndexType> Func;
    typedef typename Func::MatrixType MatrixType;
    return MatrixType::NullaryExpr(row_indices.size(),
                                   col_indices.size(),
                                   Func(arg.derived(), row_indices, col_indices));
}

#include <Eigen/Core>
#include <cassert>

// The solution function is assumed to be pasted above (omit main from solution).

int main() {
    Eigen::MatrixXi A(3, 4);
    A << 1, 2, 3, 4,
         5, 6, 7, 8,
         9, 10, 11, 12;

    // Fixed-size row indices, dynamic column indices
    Eigen::Array3i ri(2, 0, 1);
    Eigen::ArrayXi ci(5);
    ci << 3, 0, 2, 1, 0;
    Eigen::MatrixXi B = selectRowsColumns(A, ri, ci);
    assert(B.rows() == 3 && B.cols() == 5);
    assert(B(0,0) == A(2,3)); // 12
    assert(B(0,1) == A(2,0)); // 9
    assert(B(1,2) == A(0,2)); // 3
    assert(B(2,4) == A(1,0)); // 5

    // Dynamic row indices, fixed-size column indices
    Eigen::ArrayXi ri2(4);
    ri2 << 0, 2, 1, 0;
    Eigen::Array3i ci2(1, 2, 3);
    Eigen::MatrixXi C = selectRowsColumns(A, ri2, ci2);
    assert(C.rows() == 4 && C.cols() == 3);
    assert(C(0,0) == A(0,1)); // 2
    assert(C(1,1) == A(2,2)); // 11
    assert(C(3,2) == A(0,3)); // 4

    // Duplicate indices and size-1 selection
    Eigen::ArrayXi ri3(2);
    ri3 << 1, 1;
    Eigen::ArrayXi ci3(1);
    ci3 << 2;
    Eigen::MatrixXi D = selectRowsColumns(A, ri3, ci3);
    assert(D.rows() == 2 && D.cols() == 1);
    assert(D(0,0) == A(1,2)); // 7
    assert(D(1,0) == A(1,2)); // 7

    // Empty selection
    Eigen::ArrayXi ri_empty(0);
    Eigen::ArrayXi ci_empty(0);
    Eigen::MatrixXi E = selectRowsColumns(A, ri_empty, ci_empty);
    assert(E.rows() == 0 && E.cols() == 0);

    // Ensure it can be assigned to a dynamic matrix and used in expressions
    Eigen::MatrixXi F = (selectRowsColumns(A, Eigen::Array3i(0,1,2), Eigen::Array3i(0,1,2)) * 2);
    assert(F.rows() == 3 && F.cols() == 3);
    assert(F(2,2) == A(2,2) * 2); // 22
}

// The core idea is to create a lightweight functor that stores references to the source matrix and the two index arrays. The functor’s `operator()(Index row, Index col)` simply returns `m_arg(m_rowIndices[row], m_colIndices[col])`. The free function `selectRowsColumns` uses `MatrixType::NullaryExpr` to lazily generate the result matrix without materializing it until assignment. Key details: the functor must deduce the result matrix’s scalar type, compile-time sizes, and storage order from the provided index arrays and source matrix flags. We define the `MatrixType` alias inside the functor using `RowIndexType::SizeAtCompileTime`, `ColIndexType::SizeAtCompileTime`, and `MaxSizeAtCompileTime`. For the storage order, we respect the source matrix’s RowMajorBit flag. Edge cases: empty index arrays (size 0) produce a valid 0×0 matrix; duplicate indices are allowed; indices can be any integer type (we use `Index` for indexing the source). Time complexity is `O(N)` where `N` is the number of elements in the result (each output element is computed in constant time). Space complexity is `O(1)` extra for the functor (stores references only), plus the output matrix’s storage when assigned. The functor must be const-correct; all member references are `const`, and `operator()` is `const`. The solution must include headers `<Eigen/Core>` and use `Eigen::Index`. Since `NullaryExpr` expects a functor object, we return `CwiseNullaryOp` from the function.
