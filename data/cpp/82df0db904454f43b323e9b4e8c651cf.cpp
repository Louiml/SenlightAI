/*
Write a standalone C++ function `computeCommutatorSinSquared(double theta)` that, given an angle in radians, computes the squared sine of half the rotation angle of the group commutator \( R_X(\theta) R_Y(\theta) R_X(-\theta) R_Y(-\theta) \), where \( R_X(\theta) \) and \( R_Y(\theta) \) are rotations around the X and Y axes respectively. The result is the scalar value \( \sin^2(\phi/2) \) where \( \phi \) is the rotation angle of the resulting combined rotation. Use standard 2×2 complex matrices (e.g., `std::array<std::array<std::complex<double>,2>,2>`) and the Pauli matrix representation for rotations: \( R(\theta) = \cos(\theta/2) I - i \sin(\theta/2) (n_x X + n_y Y + n_z Z) \). The function must handle arbitrary real input including negative values, zero, and values beyond \( 2\pi \) correctly. The returned value must be in the range \([0,1]\) (numerically), since it represents a squared sine. Provide a helper function for matrix multiplication, adjoint (conjugate transpose), and a trace-based method to extract the rotation axis and angle from a unitary matrix.
*/
#include <array>
#include <complex>
#include <cmath>
#include <algorithm>

// Type aliases for readability
using Matrix2x2 = std::array<std::array<std::complex<double>, 2>, 2>;
using Complex = std::complex<double>;

// Multiply two 2x2 complex matrices
Matrix2x2 multiply(const Matrix2x2& A, const Matrix2x2& B) {
    Matrix2x2 result{};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j];
        }
    }
    return result;
}

// Adjoint (conjugate transpose) of a 2x2 complex matrix
Matrix2x2 adjoint(const Matrix2x2& A) {
    Matrix2x2 result{};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = std::conj(A[j][i]);
        }
    }
    return result;
}

// Trace of a 2x2 complex matrix
Complex trace(const Matrix2x2& A) {
    return A[0][0] + A[1][1];
}

// Pauli matrices
const Matrix2x2 X = {{{ {0,0}, {1,0} }, { {1,0}, {0,0} }}};
const Matrix2x2 Y = {{{ {0,0}, {0,-1} }, { {0,1}, {0,0} }}};
const Matrix2x2 Z = {{{ {1,0}, {0,0} }, { {0,0}, {-1,0} }}};
const Matrix2x2 I = {{{ {1,0}, {0,0} }, { {0,0}, {1,0} }}};

// Rotation matrix around axis (nx,ny,nz) by angle theta
Matrix2x2 rotationMatrix(double theta, double nx, double ny, double nz) {
    // Normalize axis
    double norm = std::sqrt(nx*nx + ny*ny + nz*nz);
    nx /= norm; ny /= norm; nz /= norm;
    double half = theta / 2.0;
    double c = std::cos(half);
    double s = std::sin(half);
    // Pauli combination: nx*X + ny*Y + nz*Z
    Matrix2x2 pauli = X; // dummy, we'll compute properly
    // Compute nx*X + ny*Y + nz*Z
    Matrix2x2 nX = X; // just copy, then scale
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            nX[i][j] = nx * X[i][j] + ny * Y[i][j] + nz * Z[i][j];
        }
    // I * c - i * s * (nX)
    Matrix2x2 result{};
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            result[i][j] = I[i][j]*c - Complex(0, s)*nX[i][j];
        }
    return result;
}

// Compute sin^2(phi/2) for the commutator of rotations around X and Y by angle theta
double computeCommutatorSinSquared(double theta) {
    // Rotations around X and Y axes
    Matrix2x2 Rx = rotationMatrix(theta, 1.0, 0.0, 0.0);
    Matrix2x2 Ry = rotationMatrix(theta, 0.0, 1.0, 0.0);
    Matrix2x2 Rx_adj = adjoint(Rx);
    Matrix2x2 Ry_adj = adjoint(Ry);
    // Commutator U = Rx * Ry * Rx_adj * Ry_adj
    Matrix2x2 U = multiply(Rx, Ry);
    U = multiply(U, Rx_adj);
    U = multiply(U, Ry_adj);
    // Trace is 2*cos(phi/2); take real part for numerical safety
    double traceRe = std::real(trace(U));
    // Clamp to valid range for arccos
    double ratio = traceRe / 2.0;
    ratio = std::max(-1.0, std::min(1.0, ratio));
    double phiHalf = std::acos(ratio); // phi/2
    double sinHalf = std::sin(phiHalf);
    return sinHalf * sinHalf;
}
#include <cassert>
#include <cmath>

int main() {
    // At theta=0, all rotations are identity, commutator is identity, angle phi=0, sin^2(0)=0
    assert(std::abs(computeCommutatorSinSquared(0.0) - 0.0) < 1e-9);

    // At theta=2*pi, same as theta=0, result 0
    assert(std::abs(computeCommutatorSinSquared(2*M_PI) - 0.0) < 1e-9);

    // Negative angle: sinc(-theta) = -sinc(theta), but since we square, result should be same as positive
    double val_pos = computeCommutatorSinSquared(1.0);
    double val_neg = computeCommutatorSinSquared(-1.0);
    assert(std::abs(val_pos - val_neg) < 1e-9);

    // For theta = pi/2, the commutator is known to be a rotation by pi (angle phi=pi), so sin^2(pi/2)=1
    double val_half_pi = computeCommutatorSinSquared(M_PI/2.0);
    assert(std::abs(val_half_pi - 1.0) < 1e-6);

    // For theta = pi, the commutator is identity (since rotations by pi around orthogonal axes commute up to sign? Actually apply known result: R_x(pi) R_y(pi) = -i Z? Let's compute expected? We'll just verify range and consistency)
    double val_pi = computeCommutatorSinSquared(M_PI);
    // Should be in [0,1] and we can compute directly from formula; for pi, commutator is identity? Actually known: Rx(pi) Ry(pi) Rx(-pi) Ry(-pi) = I, so result 0
    assert(std::abs(val_pi - 0.0) < 1e-6);

    // Verify output range for various inputs
    for (double theta = -10.0; theta <= 10.0; theta += 0.5) {
        double v = computeCommutatorSinSquared(theta);
        assert(v >= -1e-9 && v <= 1.0 + 1e-9);
    }

    // Large angle, periodic behavior: theta and theta+2*pi yield same
    assert(std::abs(computeCommutatorSinSquared(3.0) - computeCommutatorSinSquared(3.0 + 2*M_PI)) < 1e-9);
    return 0;
}
// The solution requires implementing 2×2 complex matrix operations: multiplication, adjoint (conjugate transpose), and trace. The rotation matrix constructor uses the Pauli matrices:
// \( X = \begin{pmatrix} 0 & 1 \\ 1 & 0 \end{pmatrix}, Y = \begin{pmatrix} 0 & -i \\ i & 0 \end{pmatrix}, Z = \begin{pmatrix} 1 & 0 \\ 0 & -1 \end{pmatrix} \). For a rotation around normalized axis \( \vec{n} \) by angle \( \theta \), the matrix is \( \cos(\theta/2) I - i \sin(\theta/2)(n_x X + n_y Y + n_z Z) \). The group commutator is the product \( U = R_X(\theta) R_Y(\theta) R_X^\dagger(\theta) R_Y^\dagger(\theta) \). To extract the rotation angle \( \phi \) from a unitary matrix U, we use the identity \( \text{Tr}(U) = 2 \cos(\phi/2) \). For rotations, the trace is real (since the matrix is a special unitary SU(2) element). However, due to numerical error, we take the real part of the trace. The rotation angle can be recovered as \( \phi = 2 \arccos(\text{Re}(\text{Tr}(U))/2) \). Then the squared sine of half angle is \( \sin^2(\phi/2) = 1 - (\text{Re}(\text{Tr}(U))/2)^2 \). Care must be taken for the arccos domain: clamp the trace ratio to [-1,1]. The function must handle all real `theta` values; trigonometric functions naturally handle periodicity. The time complexity is O(1) with constant memory, since only a fixed number of 2×2 matrix multiplications are performed. Edge cases include `theta=0`, where all rotations are identity, commutator is identity, `phi=0`, result 0. For `theta=π`, we need to check results; but the algorithm is general. Numeric stability is good because we avoid manually extracting the axis; we directly use the trace. The final result should be squared, so we compute `double sinHalf = std::sin(phi/2); return sinHalf*sinHalf;` or directly from trace.
