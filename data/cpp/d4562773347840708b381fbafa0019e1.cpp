Write a C++ function that takes a constant reference to a 4x4 `Eigen::Matrix4f` and returns a `std::vector<float>` containing the eigenvalues of the matrix, sorted in ascending order. The input matrix is guaranteed to be real and symmetric (i.e., `A == A.transpose()`), but your function should verify this assumption and return an empty vector if the matrix is not symmetric within a small tolerance (e.g., `1e-5`). Use `Eigen::SelfAdjointEigenSolver` to compute the eigenvalues. Additionally, write a separate helper function that takes the same matrix and adds a 4x4 identity matrix to it before computing and returning the eigenvalues of `A + I`, also sorted ascending. Your main solution function must be `const`-correct and not modify the input.
// The core approach involves using Eigen’s `SelfAdjointEigenSolver`, which is designed for symmetric (self-adjoint) matrices and returns eigenvalues in ascending order by default. First, check symmetry by comparing each element `A(i,j)` with `A(j,i)`; use an absolute difference tolerance (e.g., `1e-5`) because floating-point random matrices may have tiny asymmetries. If the matrix is not symmetric, return an empty vector. Otherwise, declare a `SelfAdjointEigenSolver<Matrix4f>` object, call its `compute` method on the input matrix, and extract the eigenvalues via `.eigenvalues()` (a `Vector4f`). Convert this vector to a `std::vector<float>` by iterating over its elements. For the helper that adds the identity, simply create a temporary `Matrix4f B = A + Matrix4f::Identity()` and reuse the same solver object by calling `compute` again (Eigen allows reusing the solver instance). The eigenvalues of a real symmetric matrix are always real, so no complex handling is needed. Edge cases: if the input is exactly symmetric but has NaN or Inf, the solver may produce NaN eigenvalues, but the problem typically assumes valid numeric inputs. Time complexity: The symmetry check is O(16) constant time; the eigen-decomposition for a fixed 4x4 matrix is O(1) in practice (though theoretically O(n^3) with n=4). Space complexity is O(1) besides the output vector.
#include <Eigen/Dense>
#include <vector>
#include <cmath>

// Check if a 4x4 matrix is symmetric within a given tolerance.
bool isSymmetric(const Eigen::Matrix4f& mat, float tolerance = 1e-5f) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::fabs(mat(i, j) - mat(j, i)) > tolerance) {
                return false;
            }
        }
    }
    return true;
}

// Compute eigenvalues of a symmetric 4x4 matrix, sorted ascending.
// Returns an empty vector if the matrix is not symmetric.
std::vector<float> symmetricEigenvalues(const Eigen::Matrix4f& mat) {
    if (!isSymmetric(mat)) {
        return {};
    }
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix4f> solver;
    solver.compute(mat);
    // solver.eigenvalues() is already sorted ascending.
    Eigen::Vector4f evals = solver.eigenvalues();
    std::vector<float> result(evals.data(), evals.data() + evals.size());
    return result;
}

// Compute eigenvalues of (mat + Identity), sorted ascending.
// Returns an empty vector if the original matrix is not symmetric.
std::vector<float> eigenvaluesOfShiftedMatrix(const Eigen::Matrix4f& mat) {
    if (!isSymmetric(mat)) {
        return {};
    }
    Eigen::Matrix4f shifted = mat + Eigen::Matrix4f::Identity();
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix4f> solver;
    solver.compute(shifted);
    Eigen::Vector4f evals = solver.eigenvalues();
    std::vector<float> result(evals.data(), evals.data() + evals.size());
    return result;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <Eigen/Dense>

// Declarations from solution (for test compilation)
bool isSymmetric(const Eigen::Matrix4f& mat, float tolerance = 1e-5f);
std::vector<float> symmetricEigenvalues(const Eigen::Matrix4f& mat);
std::vector<float> eigenvaluesOfShiftedMatrix(const Eigen::Matrix4f& mat);

int main() {
    // Create a known symmetric matrix: [[2,1],[1,2]] embedded in 4x4 with zeros.
    Eigen::Matrix4f A = Eigen::Matrix4f::Zero();
    A(0,0) = 2.0f; A(0,1) = 1.0f;
    A(1,0) = 1.0f; A(1,1) = 2.0f;
    // Eigenvalues of the 2x2 block are 1 and 3; the other two eigenvalues are 0.
    std::vector<float> evals = symmetricEigenvalues(A);
    assert(evals.size() == 4);
    assert(std::fabs(evals[0] - 0.0f) < 1e-5f);
    assert(std::fabs(evals[1] - 0.0f) < 1e-5f);
    assert(std::fabs(evals[2] - 1.0f) < 1e-5f);
    assert(std::fabs(evals[3] - 3.0f) < 1e-5f);

    // For A+I: eigenvalues become 1,1,2,4.
    std::vector<float> shifted = eigenvaluesOfShiftedMatrix(A);
    assert(shifted.size() == 4);
    assert(std::fabs(shifted[0] - 1.0f) < 1e-5f);
    assert(std::fabs(shifted[1] - 1.0f) < 1e-5f);
    assert(std::fabs(shifted[2] - 2.0f) < 1e-5f);
    assert(std::fabs(shifted[3] - 4.0f) < 1e-5f);

    // Nonsymmetric matrix should yield empty vector.
    Eigen::Matrix4f B = Eigen::Matrix4f::Random();
    assert(symmetricEigenvalues(B).empty());
    assert(eigenvaluesOfShiftedMatrix(B).empty());

    // A random symmetric matrix: X + X.transpose() is symmetric.
    Eigen::Matrix4f X = Eigen::Matrix4f::Random();
    Eigen::Matrix4f Sym = X + X.transpose();
    std::vector<float> evalsSym = symmetricEigenvalues(Sym);
    assert(evalsSym.size() == 4);
    // Verify sorting: each element <= next (within tolerance).
    for (size_t i = 0; i + 1 < evalsSym.size(); ++i) {
        assert(evalsSym[i] <= evalsSym[i+1] + 1e-5f);
    }
    // Check that the sum of eigenvalues equals trace of matrix.
    float trace = Sym.trace();
    float sum = 0.0f;
    for (float v : evalsSym) sum += v;
    assert(std::fabs(sum - trace) < 1e-4f);

    return 0;
}
