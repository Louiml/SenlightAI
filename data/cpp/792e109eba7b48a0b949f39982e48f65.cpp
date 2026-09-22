// Write a self-contained C++ function named `computeSymmetricEigen3` that accepts a symmetric 3×3 matrix of type `Eigen::Matrix3d` and returns both its eigenvalues (sorted in ascending order) and corresponding orthonormal eigenvectors as a `std::pair<Eigen::Vector3d, Eigen::Matrix3d>`, where the first element is the eigenvalue vector and the second is a matrix whose columns are the eigenvectors. The function must compute the result analytically without calling Eigen’s `SelfAdjointEigenSolver`, using the closed-form cubic root method for the characteristic polynomial. Handle the case where the matrix has repeated eigenvalues by returning an identity eigenvector matrix. All computations must be done in double precision, the input matrix is guaranteed symmetric but may have entries with large magnitude (e.g., up to 1e8), and the function should be robust to numerical rounding by scaling the matrix internally before solving. The output eigenvectors must be normalized and pairwise orthogonal (within a tolerance of 1e-6). The function must not allocate dynamic memory beyond what Eigen containers already use, and must be declared `const`‑correct for the input.
#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include <cassert>
#include <cmath>

// Declare the tested function (assume it is in the same translation unit).
std::pair<Eigen::Vector3d, Eigen::Matrix3d> computeSymmetricEigen3(const Eigen::Matrix3d& mat);

int main() {
    // Test 1: Identity matrix.
    Eigen::Matrix3d A = Eigen::Matrix3d::Identity();
    auto [evals1, evecs1] = computeSymmetricEigen3(A);
    assert((evals1 - Eigen::Vector3d(1,1,1)).norm() < 1e-12);
    assert((evecs1 - Eigen::Matrix3d::Identity()).norm() < 1e-12);

    // Test 2: Diagonal matrix with distinct values.
    Eigen::Matrix3d B;
    B << 3, 0, 0,
         0, 1, 0,
         0, 0, 2;
    auto [evals2, evecs2] = computeSymmetricEigen3(B);
    assert((evals2 - Eigen::Vector3d(1,2,3)).norm() < 1e-12);
    // Eigenvectors should be permutations of standard basis.
    assert(std::abs(evecs2.col(0).dot(Eigen::Vector3d(0,1,0))) > 0.999);
    assert(std::abs(evecs2.col(1).dot(Eigen::Vector3d(0,0,1))) > 0.999);
    assert(std::abs(evecs2.col(2).dot(Eigen::Vector3d(1,0,0))) > 0.999);

    // Test 3: Random symmetric matrix, compare with Eigen's solver.
    Eigen::Matrix3d C = Eigen::Matrix3d::Random();
    C = C * C.transpose();  // symmetric positive semi-definite
    auto [evals3, evecs3] = computeSymmetricEigen3(C);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(C);
    Eigen::Vector3d eig_evals = solver.eigenvalues();
    Eigen::Matrix3d eig_evecs = solver.eigenvectors();
    assert((evals3 - eig_evals).norm() < 1e-6);
    // Ensure orthonormal columns.
    assert((evecs3.transpose() * evecs3 - Eigen::Matrix3d::Identity()).norm() < 1e-6);
    // Check the reconstructed matrix.
    assert((evecs3 * evals3.asDiagonal() * evecs3.transpose() - C).norm() < 1e-6);

    // Test 4: Matrix with two equal eigenvalues (e.g., rank-1 update of identity).
    Eigen::Matrix3d D;
    D << 2, 0, 0,
         0, 1, 1,
         0, 1, 1;  // eigenvalues: 2, 0, 0
    auto [evals4, evecs4] = computeSymmetricEigen3(D);
    assert((evals4 - Eigen::Vector3d(0,0,2)).norm() < 1e-12);
    assert((evecs4.transpose() * evecs4 - Eigen::Matrix3d::Identity()).norm() < 1e-6);

    // Test 5: Large scaling.
    Eigen::Matrix3d E;
    E << 1e8, 0, 0,
         0, -2e8, 0,
         0, 0, 3e8;
    auto [evals5, evecs5] = computeSymmetricEigen3(E);
    assert((evals5 - Eigen::Vector3d(-2e8,1e8,3e8)).norm() < 1e-6);

    return 0;
}
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>
#include <utility>

using Matrix3d = Eigen::Matrix3d;
using Vector3d = Eigen::Vector3d;

// Compute eigenvalues (ascending) and orthonormal eigenvectors of a symmetric 3x3 matrix.
std::pair<Vector3d, Matrix3d> computeSymmetricEigen3(const Matrix3d& mat) {
    // Scale matrix to improve numerical stability.
    double scale = mat.cwiseAbs().maxCoeff();
    scale = std::max(scale, 1.0);
    Matrix3d scaledMat = mat / scale;

    // Coefficients of characteristic polynomial: x^3 - c2*x^2 + c1*x - c0 = 0
    double c0 = scaledMat(0,0)*scaledMat(1,1)*scaledMat(2,2)
              + 2.0*scaledMat(0,1)*scaledMat(0,2)*scaledMat(1,2)
              - scaledMat(0,0)*scaledMat(1,2)*scaledMat(1,2)
              - scaledMat(1,1)*scaledMat(0,2)*scaledMat(0,2)
              - scaledMat(2,2)*scaledMat(0,1)*scaledMat(0,1);
    double c1 = scaledMat(0,0)*scaledMat(1,1) - scaledMat(0,1)*scaledMat(0,1)
              + scaledMat(0,0)*scaledMat(2,2) - scaledMat(0,2)*scaledMat(0,2)
              + scaledMat(1,1)*scaledMat(2,2) - scaledMat(1,2)*scaledMat(1,2);
    double c2 = scaledMat(0,0) + scaledMat(1,1) + scaledMat(2,2);

    // Trigonometric solution for real roots.
    const double inv3 = 1.0 / 3.0;
    const double sqrt3 = std::sqrt(3.0);
    double c2_over_3 = c2 * inv3;
    double a_over_3 = (c1 - c2 * c2_over_3) * inv3;
    if (a_over_3 > 0.0) a_over_3 = 0.0;  // clamp due to rounding

    double half_b = 0.5 * (c0 + c2_over_3 * (2.0 * c2_over_3 * c2_over_3 - c1));
    double q = half_b * half_b + a_over_3 * a_over_3 * a_over_3;
    if (q > 0.0) q = 0.0;  // clamp due to rounding

    double rho = std::sqrt(-a_over_3);
    double theta = std::atan2(std::sqrt(-q), half_b) * inv3;
    double cos_theta = std::cos(theta);
    double sin_theta = std::sin(theta);

    Vector3d evals;
    evals(0) = c2_over_3 + 2.0 * rho * cos_theta;
    evals(1) = c2_over_3 - rho * (cos_theta + sqrt3 * sin_theta);
    evals(2) = c2_over_3 - rho * (cos_theta - sqrt3 * sin_theta);

    // Sort eigenvalues ascending.
    if (evals(0) >= evals(1)) std::swap(evals(0), evals(1));
    if (evals(1) >= evals(2)) {
        std::swap(evals(1), evals(2));
        if (evals(0) >= evals(1)) std::swap(evals(0), evals(1));
    }

    Matrix3d evecs;
    const double eps = Eigen::NumTraits<double>::epsilon();

    // Handle the case of all eigenvalues equal.
    if ((evals(2) - evals(0)) <= eps) {
        evecs.setIdentity();
    } else {
        // Eigenvector for largest eigenvalue.
        Matrix3d tmp = scaledMat;
        tmp.diagonal().array() -= evals(2);
        evecs.col(2) = tmp.row(0).cross(tmp.row(1)).normalized();

        // Eigenvector for middle eigenvalue.
        tmp = scaledMat;
        tmp.diagonal().array() -= evals(1);
        evecs.col(1) = tmp.row(0).cross(tmp.row(1));
        double n1 = evecs.col(1).norm();
        if (n1 <= eps) {
            // Repeated eigenvalue: use a vector orthogonal to evecs[2].
            evecs.col(1) = evecs.col(2).unitOrthogonal();
        } else {
            evecs.col(1) /= n1;
        }
        // Make evecs[1] exactly orthogonal to evecs[2].
        evecs.col(1) = evecs.col(2).cross(evecs.col(1).cross(evecs.col(2))).normalized();

        // Smallest eigenvector is cross product of the other two.
        evecs.col(0) = evecs.col(2).cross(evecs.col(1));
    }

    // Undo scaling.
    evals *= scale;
    return {evals, evecs};
}
// The solution follows the method from the provided snippet: first, scale the input matrix by its maximum absolute entry (or by 1 if all entries are within [-1,1]) to improve numerical stability during cubic-root solving. Then compute the coefficients of the characteristic polynomial `x^3 - c2*x^2 + c1*x - c0 = 0` from the scaled matrix entries. Use the trigonometric closed form for cubic roots, which is valid because all eigenvalues of a real symmetric matrix are real. After obtaining the three roots, sort them ascending. To compute eigenvectors, for each distinct eigenvalue, form the matrix `(scaledMat - eigenvalue*I)` and take the cross product of two rows (e.g., row 0 and row 1) to obtain a vector in the null space; normalize it. If two eigenvalues are equal (or nearly so), use a fallback: for the repeated eigenvalue, use `unitOrthogonal` of the already‑computed eigenvector for the largest eigenvalue, then orthogonalize via projection. The third eigenvector is the cross product of the other two. Finally, multiply the eigenvalues by the scale factor to undo scaling. Time complexity is O(1) with a small constant, and space complexity is O(1) beyond the input/output matrices. Edge cases: all eigenvalues equal (return identity), two eigenvalues equal (handle by orthogonalization), and extremely small or large entries (scale to stabilize).
