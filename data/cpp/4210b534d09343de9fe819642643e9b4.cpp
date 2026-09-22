// Write a standalone C++ function `buildComplexMatrix` that accepts two square Eigen matrices `A` and `B` of the same dimension and scalar type (e.g., `double`), and returns an Eigen matrix of `std::complex<Scalar>` where each element is formed by taking the real part from `A(i,j)` and the imaginary part from `B(i,j)`. The function must use Eigen’s `binaryExpr` with a custom binary functor (as shown in the snippet) to combine the two inputs element-wise. The input matrices must not be modified, and the function must work for any floating-point scalar type (e.g., `float`, `double`, `long double`). The returned matrix must have the same dimensions as the inputs. Edge cases include matrices of size 0×0 and non-square sizes (the function should still work if both inputs match in size and shape). You do not need to verify that both inputs have identical dimensions; you may assume they do. Provide the function with a descriptive name, proper `const` correctness, and necessary headers. Do not include a `main` function.
#include <cassert>
#include <complex>
#include <Eigen/Core>

// Assume solution code is included above.

int main() {
    // Test 1: 2x2 double matrices
    Eigen::Matrix2d A, B;
    A << 1, 2,
         3, 4;
    B << 5, 6,
         7, 8;
    auto C = buildComplexMatrix(A, B);
    assert(C.rows() == 2 && C.cols() == 2);
    assert(C(0,0) == std::complex<double>(1,5));
    assert(C(0,1) == std::complex<double>(2,6));
    assert(C(1,0) == std::complex<double>(3,7));
    assert(C(1,1) == std::complex<double>(4,8));

    // Test 2: Non-square matrix 2x3
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> D(2,3), E(2,3);
    D << 1, 2, 3,
         4, 5, 6;
    E << 7, 8, 9,
         10, 11, 12;
    auto F = buildComplexMatrix(D, E);
    assert(F.rows() == 2 && F.cols() == 3);
    assert(F(1,2) == std::complex<double>(6,12));

    // Test 3: Float matrices
    Eigen::Matrix2f G, H;
    G << 1.5f, 2.5f,
         3.5f, 4.5f;
    H << 5.5f, 6.5f,
         7.5f, 8.5f;
    auto I = buildComplexMatrix(G, H);
    assert(I(0,0) == std::complex<float>(1.5f, 5.5f));
    assert(I(1,1) == std::complex<float>(4.5f, 8.5f));

    // Test 4: Zero-size matrix
    Eigen::MatrixXd J(0,0), K(0,0);
    auto L = buildComplexMatrix(J, K);
    assert(L.rows() == 0 && L.cols() == 0);

    // Test 5: Large random matrix (check first and last elements)
    Eigen::MatrixXd M = Eigen::MatrixXd::Random(4,4);
    Eigen::MatrixXd N = Eigen::MatrixXd::Random(4,4);
    auto O = buildComplexMatrix(M, N);
    assert(O(0,0) == std::complex<double>(M(0,0), N(0,0)));
    assert(O(3,3) == std::complex<double>(M(3,3), N(3,3)));

    return 0;
}
#include <Eigen/Core>
#include <complex>

// Custom binary functor to combine real and imaginary parts.
template<typename Scalar>
struct MakeComplexOp {
    EIGEN_EMPTY_STRUCT_CTOR(MakeComplexOp)
    typedef std::complex<Scalar> result_type;
    std::complex<Scalar> operator()(const Scalar& real_part, const Scalar& imag_part) const {
        return std::complex<Scalar>(real_part, imag_part);
    }
};

// Build a complex matrix by combining real parts from A and imaginary parts from B.
template<typename Scalar>
Eigen::Matrix<std::complex<Scalar>, Eigen::Dynamic, Eigen::Dynamic>
buildComplexMatrix(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>& A,
                   const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>& B) {
    return A.binaryExpr(B, MakeComplexOp<Scalar>());
}
// The solution defines a templated binary functor `MakeComplexOp<Scalar>` whose `operator()` takes two scalar arguments (real and imaginary) and returns a `std::complex<Scalar>`. The main function `buildComplexMatrix` takes two `const` references to Eigen matrices (e.g., `const MatrixBase<Derived>&` for generality, but for simplicity use `const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>&`). It calls `.binaryExpr(B, MakeComplexOp<Scalar>())` on the first matrix, which applies the functor element-wise to pairs from `A` and `B`, yielding a matrix of `std::complex<Scalar>`. The return type is `Eigen::Matrix<std::complex<Scalar>, Eigen::Dynamic, Eigen::Dynamic>`. The functor must define `result_type` as `complex<Scalar>` to satisfy Eigen’s expression template requirements. No explicit loops are needed; binaryExpr handles the traversal. Edge cases: zero-sized matrices are handled naturally because binaryExpr returns an empty matrix (Eigen supports 0×0). Non-square matrices also work because the operation is element-wise and only requires matching dimensions. Time complexity is O(n*m) for an n×m matrix, and space complexity is O(n*m) for the result, as expected.
