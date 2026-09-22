Write a C++ function `clampMatrixEntries` that takes an Eigen dense matrix of floating-point type and two scalar bounds (a lower bound and an upper bound), and returns a new matrix of the same dimensions where every entry is clamped to the closed interval `[lower, upper]`. If an entry is less than the lower bound, replace it with the lower bound; if greater than the upper bound, replace it with the upper bound; otherwise keep it unchanged. The function must work for any Eigen matrix expression (e.g., `MatrixXd`, `Matrix4f`, `ArrayXXd`) and preserve the original matrix. The bounds are guaranteed to satisfy `lower <= upper`. The solution must not modify the input matrix and must handle both row-major and column-major storage orders correctly. Return the resulting matrix by value.

The solution uses Eigen's `unaryExpr` mechanism with a custom functor that applies the clamping logic elementwise. The functor stores the lower and upper bounds as scalar values and implements `operator()` to clamp a single input value. Since `unaryExpr` creates a new matrix expression that is evaluated when assigned to a return value, the original matrix remains unmodified elementwise. The functor's `operator()` is declared `const` to allow use in const contexts. The implementation is generic over scalar type and matrix type using templates, so it works for `float`, `double`, and custom scalar types that support comparison operators. Edge cases: if `lower == upper`, all entries become that equal value; if all entries already lie within bounds, the returned matrix is a copy of the input. Time complexity is O(n) where n is the number of elements, and space complexity is O(n) for the returned matrix (plus O(1) for the functor). No special handling is needed for NaNs—comparisons with NaN are false, so NaN values remain unchanged, which is typically acceptable. The function uses `const` references for the input matrix and bounds to avoid copying.

#include <Eigen/Core>

// Custom unary functor for clamping a scalar value to [lower, upper].
template<typename Scalar>
struct ClampValue {
    ClampValue(const Scalar& lower, const Scalar& upper) : m_lower(lower), m_upper(upper) {}
    
    const Scalar operator()(const Scalar& x) const {
        return x < m_lower ? m_lower : (x > m_upper ? m_upper : x);
    }
    
    Scalar m_lower;
    Scalar m_upper;
};

// Clamp every entry of the input matrix to the closed interval [lower, upper].
// Returns a new matrix; the input matrix is left unmodified.
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar, Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>
clampMatrixEntries(const Eigen::MatrixBase<Derived>& mat,
                   typename Derived::Scalar lower,
                   typename Derived::Scalar upper) {
    // Use a static_assert to ensure the scalar type is floating-point.
    static_assert(std::is_floating_point<typename Derived::Scalar>::value,
                  "clampMatrixEntries requires a floating-point matrix");
    
    // unaryExpr applies the functor to each element and produces a new matrix.
    return mat.unaryExpr(ClampValue<typename Derived::Scalar>(lower, upper));
}

#include <Eigen/Core>
#include <cassert>

// Function declaration (assumed provided from the solution above).
template<typename Derived>
Eigen::Matrix<typename Derived::Scalar, Derived::RowsAtCompileTime, Derived::ColsAtCompileTime>
clampMatrixEntries(const Eigen::MatrixBase<Derived>& mat,
                   typename Derived::Scalar lower,
                   typename Derived::Scalar upper);

int main() {
    // Test 1: Basic clamping on a 2x2 double matrix.
    Eigen::Matrix2d m1;
    m1 << -1.0, 0.5, 0.8, 2.0;
    Eigen::Matrix2d result1 = clampMatrixEntries(m1, -0.5, 0.5);
    assert(result1(0,0) == -0.5);
    assert(result1(0,1) == 0.5);
    assert(result1(1,0) == 0.5);
    assert(result1(1,1) == 0.5);
    
    // Test 2: All entries within bounds -> unchanged.
    Eigen::Matrix2d m2;
    m2 << 0.1, 0.2, 0.3, 0.4;
    Eigen::Matrix2d result2 = clampMatrixEntries(m2, 0.0, 1.0);
    assert(result2 == m2);
    
    // Test 3: Lower == upper -> all entries become that value.
    Eigen::Matrix2d m3;
    m3 << -3.0, 7.0, 0.5, 1.5;
    Eigen::Matrix2d result3 = clampMatrixEntries(m3, 1.0, 1.0);
    assert(result3(0,0) == 1.0);
    assert(result3(0,1) == 1.0);
    assert(result3(1,0) == 1.0);
    assert(result3(1,1) == 1.0);
    
    // Test 4: Float matrix with row-major storage.
    Eigen::Matrix<float, 2, 3, Eigen::RowMajor> m4;
    m4 << -2.0f, 0.0f, 2.0f,
           1.0f, -1.0f, 3.0f;
    auto result4 = clampMatrixEntries(m4, -1.0f, 1.0f);
    assert(result4(0,0) == -1.0f);
    assert(result4(0,1) == 0.0f);
    assert(result4(0,2) == 1.0f);
    assert(result4(1,0) == 1.0f);
    assert(result4(1,1) == -1.0f);
    assert(result4(1,2) == 1.0f);
    
    // Test 5: Original matrix unchanged (input not modified).
    Eigen::Matrix2d m5;
    m5 << -2.0, -1.5, 1.5, 2.0;
    Eigen::Matrix2d original = m5;
    Eigen::Matrix2d result5 = clampMatrixEntries(m5, -1.0, 1.0);
    assert(m5 == original);
    assert(result5(0,0) == -1.0);
    assert(result5(0,1) == -1.0);
    assert(result5(1,0) == 1.0);
    assert(result5(1,1) == 1.0);
    
    return 0;
}
