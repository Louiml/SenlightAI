Write a C++ function `solveMatrixFree(const Eigen::SparseMatrix<double>& S, const Eigen::VectorXd& b)` that takes a symmetric positive-definite sparse matrix `S` and a right-hand side vector `b`, and returns the solution vector `x` obtained by solving the linear system `S x = b` using the Conjugate Gradient (CG) iterative solver in a matrix-free manner. The function must wrap `S` inside a custom matrix-free class that inherits from `Eigen::EigenBase<MatrixReplacement>`, implements the required traits and operator* methods, and provides a custom product implementation via `Eigen::internal::generic_product_impl`. The function should use the `Eigen::ConjugateGradient` solver with an `Eigen::IdentityPreconditioner` and the `Eigen::Lower|Eigen::Upper` option, and return the computed solution. Ensure the matrix-free wrapper has a method to attach the underlying sparse matrix, and the product operation `dst += alpha * lhs * rhs` is implemented by summing scaled columns of the sparse matrix when `alpha == 1`. The function should handle any dimension `n` of the square matrix, and the sparse matrix must be symmetric positive-definite for CG to converge.
// The solution involves creating a custom matrix-free class `MatrixReplacement` that mimics the interface of an `Eigen::SparseMatrix` but stores only a pointer to the actual sparse matrix. This class inherits from `Eigen::EigenBase<MatrixReplacement>` and defines required typedefs (`Scalar`, `RealScalar`, `StorageIndex`), constants (`ColsAtCompileTime`, `MaxColsAtCompileTime`, `IsRowMajor`), and methods (`rows()`, `cols()`). It also overloads `operator*` to return an `Eigen::Product` object, which triggers the custom product implementation. A specialization of `Eigen::internal::generic_product_impl` is provided for the matrix-vector product, where the `scaleAndAddTo` method implements `dst += alpha * lhs * rhs`. Since CG uses `alpha == 1`, we assert that and implement the product by iterating over each column `i` of the underlying sparse matrix and adding `rhs(i) * lhs.my_matrix().col(i)` to `dst`. The main function `solveMatrixFree` simply constructs a `MatrixReplacement` object, attaches the sparse matrix `S`, creates a `ConjugateGradient` solver with the specified options, computes the solution, and returns it. Edge cases include a zero-dimensional matrix (handled by returning an empty vector) and non-positive-definite matrices (CG may not converge, but that is outside the function's responsibility). The time complexity is dominated by the CG iterations, each of which performs one matrix-vector product with the sparse matrix in O(nnz) time, where `nnz` is the number of nonzeros. The space complexity is O(n) for the solution and temporary vectors, plus the storage of the sparse matrix itself.
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <cassert>

// Forward declaration of MatrixReplacement
class MatrixReplacement;

// Traits specialization to make MatrixReplacement look like a SparseMatrix
namespace Eigen {
namespace internal {
template<>
struct traits<MatrixReplacement> : public Eigen::internal::traits<Eigen::SparseMatrix<double>> {};
} // namespace internal
} // namespace Eigen

// Matrix-free wrapper class
class MatrixReplacement : public Eigen::EigenBase<MatrixReplacement> {
public:
    // Required typedefs
    typedef double Scalar;
    typedef double RealScalar;
    typedef int StorageIndex;
    enum {
        ColsAtCompileTime = Eigen::Dynamic,
        MaxColsAtCompileTime = Eigen::Dynamic,
        IsRowMajor = false
    };

    // Constructor
    MatrixReplacement() : mp_mat(nullptr) {}

    // Method to attach the underlying sparse matrix
    void attachMyMatrix(const Eigen::SparseMatrix<double>& mat) {
        mp_mat = &mat;
    }

    // Required methods
    Eigen::Index rows() const { return mp_mat ? mp_mat->rows() : 0; }
    Eigen::Index cols() const { return mp_mat ? mp_mat->cols() : 0; }

    // Access to the underlying matrix
    const Eigen::SparseMatrix<double>& my_matrix() const {
        assert(mp_mat != nullptr && "MatrixReplacement: no matrix attached");
        return *mp_mat;
    }

    // Overloaded operator* to enable Eigen's expression templates
    template<typename Rhs>
    Eigen::Product<MatrixReplacement, Rhs, Eigen::AliasFreeProduct> operator*(const Eigen::MatrixBase<Rhs>& x) const {
        return Eigen::Product<MatrixReplacement, Rhs, Eigen::AliasFreeProduct>(*this, x.derived());
    }

private:
    const Eigen::SparseMatrix<double>* mp_mat;
};

// Custom product implementation
namespace Eigen {
namespace internal {

template<typename Rhs>
struct generic_product_impl<MatrixReplacement, Rhs, SparseShape, DenseShape, GemvProduct>
    : generic_product_impl_base<MatrixReplacement, Rhs, generic_product_impl<MatrixReplacement, Rhs>> {
    typedef typename Product<MatrixReplacement, Rhs>::Scalar Scalar;

    template<typename Dest>
    static void scaleAndAddTo(Dest& dst, const MatrixReplacement& lhs, const Rhs& rhs, const Scalar& alpha) {
        // This implementation assumes alpha == 1 (as used by iterative solvers)
        assert(alpha == Scalar(1) && "scaling is not implemented");

        // Perform lhs * rhs by summing scaled columns of the sparse matrix
        const auto& mat = lhs.my_matrix();
        for (Eigen::Index i = 0; i < lhs.cols(); ++i) {
            dst += rhs(i) * mat.col(i);
        }
    }
};

} // namespace internal
} // namespace Eigen

// Main function to solve the system using matrix-free CG
Eigen::VectorXd solveMatrixFree(const Eigen::SparseMatrix<double>& S, const Eigen::VectorXd& b) {
    // Check dimensions
    assert(S.rows() == S.cols() && "Matrix must be square");
    assert(S.rows() == b.size() && "Dimension mismatch between matrix and vector");

    // Wrap the sparse matrix in the matrix-free wrapper
    MatrixReplacement A;
    A.attachMyMatrix(S);

    // Set up the Conjugate Gradient solver
    Eigen::ConjugateGradient<MatrixReplacement, Eigen::Lower | Eigen::Upper, Eigen::IdentityPreconditioner> cg;
    cg.compute(A);

    // Solve the system
    Eigen::VectorXd x = cg.solve(b);

    return x;
}
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <cassert>
#include <cmath>

// Declaration of the function under test
Eigen::VectorXd solveMatrixFree(const Eigen::SparseMatrix<double>& S, const Eigen::VectorXd& b);

int main() {
    // Test 1: Small 3x3 positive definite system
    {
        Eigen::SparseMatrix<double> S(3, 3);
        S.coeffRef(0, 0) = 4.0; S.coeffRef(0, 1) = 1.0; S.coeffRef(0, 2) = 0.0;
        S.coeffRef(1, 0) = 1.0; S.coeffRef(1, 1) = 3.0; S.coeffRef(1, 2) = 1.0;
        S.coeffRef(2, 0) = 0.0; S.coeffRef(2, 1) = 1.0; S.coeffRef(2, 2) = 2.0;
        S.makeCompressed();

        Eigen::VectorXd b(3);
        b << 1.0, 2.0, 3.0;

        Eigen::VectorXd x = solveMatrixFree(S, b);
        // Expected solution: (0.0, 1.0, 1.0) approximately
        assert(std::abs(x(0) - 0.0) < 1e-8);
        assert(std::abs(x(1) - 1.0) < 1e-8);
        assert(std::abs(x(2) - 1.0) < 1e-8);
    }

    // Test 2: Diagonal matrix
    {
        Eigen::SparseMatrix<double> S(2, 2);
        S.coeffRef(0, 0) = 5.0;
        S.coeffRef(1, 1) = 7.0;
        S.makeCompressed();

        Eigen::VectorXd b(2);
        b << 10.0, 14.0;

        Eigen::VectorXd x = solveMatrixFree(S, b);
        assert(std::abs(x(0) - 2.0) < 1e-8);
        assert(std::abs(x(1) - 2.0) < 1e-8);
    }

    // Test 3: Larger random symmetric positive definite matrix
    {
        int n = 10;
        Eigen::MatrixXd dense = Eigen::MatrixXd::Random(n, n);
        Eigen::MatrixXd spd = dense.transpose() * dense + Eigen::MatrixXd::Identity(n, n);
        Eigen::SparseMatrix<double> S = spd.sparseView();

        Eigen::VectorXd b = Eigen::VectorXd::Random(n);

        Eigen::VectorXd x = solveMatrixFree(S, b);
        // Verify residual: ||S*x - b|| should be small
        Eigen::VectorXd residual = S * x - b;
        assert(residual.norm() < 1e-6);
    }

    // Test 4: 1x1 matrix
    {
        Eigen::SparseMatrix<double> S(1, 1);
        S.coeffRef(0, 0) = 3.0;
        S.makeCompressed();

        Eigen::VectorXd b(1);
        b << 9.0;

        Eigen::VectorXd x = solveMatrixFree(S, b);
        assert(std::abs(x(0) - 3.0) < 1e-8);
    }

    return 0;
}
