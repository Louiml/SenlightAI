/*
Write a C++ function that takes two 4x4 transformation matrices (represented as `double m[4][4]` arrays, where the upper-left 3x3 is a rotation matrix and the fourth column is a translation vector, with the bottom row `{0,0,0,1}`) and returns a new 4x4 matrix representing the inverse of the composition of the two transformations in the order: first apply the second matrix's transformation, then apply the first matrix's transformation (i.e., compute the inverse of `first * second`). The result must be a valid rigid transformation (orthonormal rotation part, translation part, and bottom row `{0,0,0,1}`), and the function must not rely on external libraries beyond standard C++ headers. Ensure the function handles the case where the rotation matrices are not perfectly orthonormal by normalizing the result’s rotation columns to unit length and orthogonalizing them (e.g., using Gram–Schmidt on the rotation part of the computed inverse).
*/

#include <cmath>
#include <array>

// Helper to compute the inverse of a rigid transformation given as a 4x4 matrix.
// The matrix is assumed to have bottom row {0,0,0,1}. Rotation part is top-left 3x3.
// The result is orthogonalized and normalized to ensure a valid rotation.
std::array<std::array<double,4>,4> inverseRigidTransform(const double m[4][4]) {
    // Extract rotation part R (3x3) and translation t (3).
    // Compute transpose of R (this is the rotation part of the inverse).
    double Rinv[3][3];
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            Rinv[i][j] = m[j][i]; // transpose
        }
    }

    // Orthogonalize columns of Rinv using Gram-Schmidt.
    // We treat the three columns as vectors c1, c2, c3.
    double c1[3] = {Rinv[0][0], Rinv[1][0], Rinv[2][0]};
    double c2[3] = {Rinv[0][1], Rinv[1][1], Rinv[2][1]};
    double c3[3] = {Rinv[0][2], Rinv[1][2], Rinv[2][2]};

    // Normalize c1.
    double norm1 = std::sqrt(c1[0]*c1[0] + c1[1]*c1[1] + c1[2]*c1[2]);
    for (int i=0; i<3; ++i) c1[i] /= norm1;

    // Project c2 on c1 and subtract.
    double dot12 = c2[0]*c1[0] + c2[1]*c1[1] + c2[2]*c1[2];
    for (int i=0; i<3; ++i) c2[i] -= dot12 * c1[i];
    double norm2 = std::sqrt(c2[0]*c2[0] + c2[1]*c2[1] + c2[2]*c2[2]);
    for (int i=0; i<3; ++i) c2[i] /= norm2;

    // Project c3 on c1 and c2, subtract.
    double dot13 = c3[0]*c1[0] + c3[1]*c1[1] + c3[2]*c1[2];
    double dot23 = c3[0]*c2[0] + c3[1]*c2[1] + c3[2]*c2[2];
    for (int i=0; i<3; ++i) c3[i] -= dot13 * c1[i] + dot23 * c2[i];
    double norm3 = std::sqrt(c3[0]*c3[0] + c3[1]*c3[1] + c3[2]*c3[2]);
    for (int i=0; i<3; ++i) c3[i] /= norm3;

    // Build the orthogonalized Rinv from columns.
    double Rorth[3][3];
    for (int i=0; i<3; ++i) {
        Rorth[i][0] = c1[i];
        Rorth[i][1] = c2[i];
        Rorth[i][2] = c3[i];
    }

    // Compute translation part of inverse: t_inv = -Rinv * t.
    double t[3] = {m[0][3], m[1][3], m[2][3]};
    double t_inv[3] = {0,0,0};
    for (int i=0; i<3; ++i) {
        t_inv[i] = -(Rorth[i][0]*t[0] + Rorth[i][1]*t[1] + Rorth[i][2]*t[2]);
    }

    // Assemble result.
    std::array<std::array<double,4>,4> result = {};
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            result[i][j] = Rorth[i][j];
        }
        result[i][3] = t_inv[i];
    }
    result[3][0] = 0; result[3][1] = 0; result[3][2] = 0; result[3][3] = 1;
    return result;
}

// Main solution: compute inverse of (first * second) for two rigid transforms.
std::array<std::array<double,4>,4> inverseOfProduct(const double first[4][4], const double second[4][4]) {
    // Compute C = first * second.
    double C[4][4] = {};
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            C[i][j] = 0;
            for (int k=0; k<3; ++k) {
                C[i][j] += first[i][k] * second[k][j];
            }
        }
        C[i][3] = first[i][3];
        for (int k=0; k<3; ++k) {
            C[i][3] += first[i][k] * second[k][3];
        }
    }
    // Bottom row is {0,0,0,1}
    C[3][0] = 0; C[3][1] = 0; C[3][2] = 0; C[3][3] = 1;

    // Compute inverse of C.
    return inverseRigidTransform(C);
}

#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

// The solution functions are assumed to be declared above.
// (Place them before this main.)

int main() {
    // Identity matrices
    double I[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    auto res = inverseOfProduct(I, I);
    // Check result is identity
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            if (i==j) assert(std::fabs(res[i][j] - 1.0) < 1e-9);
            else assert(std::fabs(res[i][j]) < 1e-9);
        }
    }

    // Translation by (1,2,3)
    double T1[4][4] = {{1,0,0,1},{0,1,0,2},{0,0,1,3},{0,0,0,1}};
    // Translation by (-1,-2,-3) is inverse
    double T2[4][4] = {{1,0,0,-1},{0,1,0,-2},{0,0,1,-3},{0,0,0,1}};
    res = inverseOfProduct(T1, T2); // T1*T2 = identity, inverse is identity
    for (int i=0; i<3; ++i) assert(std::fabs(res[i][3]) < 1e-9);
    for (int i=0; i<3; ++i) for (int j=0; j<3; ++j) {
        if (i==j) assert(std::fabs(res[i][j] - 1.0) < 1e-9);
        else assert(std::fabs(res[i][j]) < 1e-9);
    }

    // Pure rotation 90 degrees around Z (cos=0, sin=1)
    // Rz(90) = [[0,-1,0],[1,0,0],[0,0,1]]
    double Rz[4][4] = {{0,-1,0,0},{1,0,0,0},{0,0,1,0},{0,0,0,1}};
    // Its inverse is Rz(-90) = [[0,1,0],[-1,0,0],[0,0,1]]
    res = inverseOfProduct(Rz, I); // inverse of Rz
    assert(std::fabs(res[0][0] - 0.0) < 1e-9);
    assert(std::fabs(res[0][1] - 1.0) < 1e-9);
    assert(std::fabs(res[1][0] - (-1.0)) < 1e-9);
    assert(std::fabs(res[1][1] - 0.0) < 1e-9);
    assert(std::fabs(res[2][2] - 1.0) < 1e-9);
    assert(std::fabs(res[0][3]) < 1e-9);
    assert(std::fabs(res[1][3]) < 1e-9);
    assert(std::fabs(res[2][3]) < 1e-9);

    // Product of two rotations: first = Rz(90), second = Rx(90) (around X)
    // Rx(90) = [[1,0,0],[0,0,-1],[0,1,0]]
    double Rx[4][4] = {{1,0,0,0},{0,0,-1,0},{0,1,0,0},{0,0,0,1}};
    // Compute inverse of (Rz * Rx). We can verify by multiplying forward.
    auto inv = inverseOfProduct(Rz, Rx);
    // Reconstruct product C = Rz*Rx
    double C[4][4] = {};
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            for (int k=0; k<3; ++k) C[i][j] += Rz[i][k]*Rx[k][j];
        }
        C[i][3] = 0;
    }
    C[3][3] = 1;
    // Check inv * C = identity (matrix multiplication)
    double prod[4][4] = {};
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            for (int k=0; k<4; ++k) prod[i][j] += inv[i][k] * C[k][j];
        }
    }
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            if (i==j) assert(std::fabs(prod[i][j] - 1.0) < 1e-9);
            else assert(std::fabs(prod[i][j]) < 1e-9);
        }
    }

    // Combined rotation and translation: first = Rz(90) + translation (1,0,0), second = Rx(90)
    double A[4][4] = {{0,-1,0,1},{1,0,0,0},{0,0,1,0},{0,0,0,1}};
    double B[4][4] = {{1,0,0,0},{0,0,-1,0},{0,1,0,0},{0,0,0,1}};
    inv = inverseOfProduct(A, B);
    // Compute actual C = A*B
    double C2[4][4] = {};
    for (int i=0; i<3; ++i) {
        for (int j=0; j<3; ++j) {
            for (int k=0; k<3; ++k) C2[i][j] += A[i][k]*B[k][j];
        }
        C2[i][3] = A[i][3];
        for (int k=0; k<3; ++k) C2[i][3] += A[i][k]*B[k][3];
    }
    C2[3][3] = 1;
    // Verify inv * C2 = identity
    double prod2[4][4] = {};
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            for (int k=0; k<4; ++k) prod2[i][j] += inv[i][k]*C2[k][j];
        }
    }
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            if (i==j) assert(std::fabs(prod2[i][j] - 1.0) < 1e-9);
            else assert(std::fabs(prod2[i][j]) < 1e-9);
        }
    }

    // Test with slightly non-orthonormal rotation to ensure orthogonalization works
    double S[4][4] = {{1.1,0.1,-0.2,5},{-0.1,0.9,0.3,-2},{0.2,-0.3,1.0,0.5},{0,0,0,1}}; // not orthonormal
    auto res2 = inverseOfProduct(S, I);
    // Result rotation should be orthonormal: check columns are unit length and orthogonal
    for (int col=0; col<3; ++col) {
        double norm = 0;
        for (int row=0; row<3; ++row) norm += res2[row][col]*res2[row][col];
        assert(std::fabs(std::sqrt(norm) - 1.0) < 1e-9);
    }
    // Dot products between columns
    for (int c1=0; c1<3; ++c1) {
        for (int c2=c1+1; c2<3; ++c2) {
            double dot = 0;
            for (int row=0; row<3; ++row) dot += res2[row][c1]*res2[row][c2];
            assert(std::fabs(dot) < 1e-9);
        }
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The task reduces to computing the inverse of a 4x4 rigid transformation. Given two rigid transformations A and B, their composition C = A * B is also rigid. The inverse of C is C^{-1} = B^{-1} * A^{-1}, but it might be simpler to compute C first, then invert C directly. For a rigid transformation represented as `[R | t]` (rotation matrix R and translation t), the inverse is `[R^T | -R^T * t]`. Steps:  
// 1. Multiply the two 4x4 matrices to get C: C = A * B. Since the bottom rows are `[0 0 0 1]`, the multiplication only involves the top 3 rows, giving rotation part R_C = R_A * R_B and translation t_C = t_A + R_A * t_B.  
// 2. Compute the inverse of C: rotation part R_inv = transpose(R_C) (since R_C is orthonormal if inputs are rigid; if not, we orthogonalize), and translation t_inv = -R_inv * t_C.  
// 3. To handle floating-point errors, orthogonalize R_inv using Gram–Schmidt on its columns (or rows). Then normalize each column to unit length. The result is still a rotation matrix.  
// 4. Set the bottom row to `{0,0,0,1}`.  
// Edge cases: If inputs are not rigid (e.g., rotation matrices with scaling), the inverse formula still works but the result may not be orthonormal; the orthogonalization corrects that. Also, if the determinant of the rotation part is negative (reflection), the inverse would have the same determinant; but we assume proper rotations. Time complexity is O(1) since matrix size is fixed (4x4). Space complexity O(1).
