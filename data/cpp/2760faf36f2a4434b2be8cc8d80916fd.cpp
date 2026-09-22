Design a C++ function `solveForReactions` that models the behavior of a simplified static equilibrium analysis system similar to the `EquiSolnAlgo` class. The function must take four pointers: a pointer to an integer representing the number of degrees of freedom (`numDOF`), a pointer to a double array of applied nodal forces (`appliedForces`, length = `numDOF`), a pointer to a 2D array representing the stiffness matrix stored as a flat double array in row-major order (`stiffnessMatrix`, size = `numDOF`×`numDOF`), and a pointer to a double array for the computed displacements (`displacements`, length = `numDOF`). The function solves the linear system `stiffnessMatrix * displacements = appliedForces` using Gaussian elimination with partial pivoting. It must return an integer status code: `0` on success, `-1` if any of the input pointers are null or `numDOF` is non-positive, and `-2` if the stiffness matrix is singular (i.e., pivot is zero after pivoting). The function must not allocate dynamic memory for temporary storage beyond a fixed-size workspace (use `std::vector<double>` or a stack-allocated array with a maximum size, e.g., 100). It must be robust for `numDOF` up to 100, handle zero or negative values in the stiffness matrix and forces, and preserve the original contents of the `stiffnessMatrix` and `appliedForces` arrays (i.e., make copies internally). The function must be `const`-correct: it should not modify the input arrays, and the output `displacements` array is the only non-const pointer.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: 2x2 simple system: 2x = 4, 3x = 6 => x = [2, 2]
    int n = 2;
    double K1[] = {2.0, 0.0, 0.0, 3.0};
    double F1[] = {4.0, 6.0};
    double U1[2] = {0.0, 0.0};
    assert(solveForReactions(n, F1, K1, U1) == 0);
    assert(std::fabs(U1[0] - 2.0) < 1e-10);
    assert(std::fabs(U1[1] - 2.0) < 1e-10);

    // Test 2: 3x3 with pivoting needed (swap rows)
    n = 3;
    double K2[] = {0.0, 1.0, 0.0,
                   1.0, 0.0, 0.0,
                   0.0, 0.0, 2.0};
    double F2[] = {3.0, 4.0, 6.0};
    double U2[3] = {0.0, 0.0, 0.0};
    assert(solveForReactions(n, F2, K2, U2) == 0);
    assert(std::fabs(U2[0] - 4.0) < 1e-10);
    assert(std::fabs(U2[1] - 3.0) < 1e-10);
    assert(std::fabs(U2[2] - 3.0) < 1e-10);

    // Test 3: Invalid input: null pointer
    double K3[] = {1.0};
    double F3[] = {1.0};
    double U3[1] = {0.0};
    assert(solveForReactions(0, F3, K3, U3) == -1);
    assert(solveForReactions(1, nullptr, K3, U3) == -1);
    assert(solveForReactions(1, F3, nullptr, U3) == -1);
    assert(solveForReactions(1, F3, K3, nullptr) == -1);

    // Test 4: Singular matrix
    double K4[] = {1.0, 2.0, 2.0, 4.0};
    double F4[] = {1.0, 2.0};
    double U4[2] = {0.0, 0.0};
    assert(solveForReactions(2, F4, K4, U4) == -2);

    // Test 5: Single DOF
    n = 1;
    double K5[] = {5.0};
    double F5[] = {15.0};
    double U5[1] = {0.0};
    assert(solveForReactions(n, F5, K5, U5) == 0);
    assert(std::fabs(U5[0] - 3.0) < 1e-10);

    // Test 6: Size limit (101 DOF)
    n = 101;
    std::vector<double> K6(n * n, 0.0), F6(n, 1.0), U6(n, 0.0);
    for (int i = 0; i < n; ++i) K6[i * n + i] = 2.0;
    assert(solveForReactions(n, F6.data(), K6.data(), U6.data()) == -3);

    // Test 7: Ensure input matrices are not modified
    n = 2;
    double K7[] = {1.0, 2.0, 3.0, 4.0};
    double F7[] = {5.0, 6.0};
    double K7_copy[4], F7_copy[2];
    std::copy(K7, K7 + 4, K7_copy);
    std::copy(F7, F7 + 2, F7_copy);
    double U7[2] = {0.0, 0.0};
    assert(solveForReactions(n, F7, K7, U7) == 0);
    for (int i = 0; i < 4; ++i) assert(K7[i] == K7_copy[i]);
    for (int i = 0; i < 2; ++i) assert(F7[i] == F7_copy[i]);

    // Test 8: Expected solution check for test 7: x = [?, ?] (solve manually: K*x=F)
    // 1*x + 2*y = 5
    // 3*x + 4*y = 6 => y = (6-3*x)/4 ; 1*x + 2*(6-3*x)/4 = 5 => x + 3 - 1.5x = 5 => -0.5x = 2 => x = -4, y = (6+12)/4 = 4.5
    assert(std::fabs(U7[0] - (-4.0)) < 1e-10);
    assert(std::fabs(U7[1] - 4.5) < 1e-10);

    return 0;
}

#include <vector>
#include <cmath>
#include <algorithm>

// Solve linear system AK*x = F using Gaussian elimination with partial pivoting.
// Returns: 0 on success, -1 for invalid input, -2 for singular matrix, -3 if size > 100.
int solveForReactions(int numDOF,
                      const double* appliedForces,
                      const double* stiffnessMatrix,
                      double* displacements) {
    const int MAX_DOF = 100;
    
    // Input validation
    if (numDOF <= 0 || appliedForces == nullptr || stiffnessMatrix == nullptr || displacements == nullptr) {
        return -1;
    }
    if (numDOF > MAX_DOF) {
        return -3;
    }

    // Copy inputs into local vectors (row-major flattening)
    std::vector<double> A(stiffnessMatrix, stiffnessMatrix + numDOF * numDOF);
    std::vector<double> b(appliedForces, appliedForces + numDOF);
    std::vector<double> x(numDOF, 0.0);

    // Forward elimination with partial pivoting
    for (int col = 0; col < numDOF; ++col) {
        // Find pivot row (largest absolute value in column)
        int pivotRow = col;
        double maxVal = std::fabs(A[col * numDOF + col]);
        for (int row = col + 1; row < numDOF; ++row) {
            double val = std::fabs(A[row * numDOF + col]);
            if (val > maxVal) {
                maxVal = val;
                pivotRow = row;
            }
        }

        // Check for singular matrix
        if (maxVal < 1e-12) {
            return -2;
        }

        // Swap pivot row with current row if needed (both A and b)
        if (pivotRow != col) {
            for (int j = col; j < numDOF; ++j) {
                std::swap(A[col * numDOF + j], A[pivotRow * numDOF + j]);
            }
            std::swap(b[col], b[pivotRow]);
        }

        // Eliminate below
        double pivot = A[col * numDOF + col];
        for (int row = col + 1; row < numDOF; ++row) {
            double factor = A[row * numDOF + col] / pivot;
            for (int j = col; j < numDOF; ++j) {
                A[row * numDOF + j] -= factor * A[col * numDOF + j];
            }
            b[row] -= factor * b[col];
        }
    }

    // Back substitution
    for (int i = numDOF - 1; i >= 0; --i) {
        double sum = b[i];
        for (int j = i + 1; j < numDOF; ++j) {
            sum -= A[i * numDOF + j] * x[j];
        }
        x[i] = sum / A[i * numDOF + i];
    }

    // Copy solution to output
    for (int i = 0; i < numDOF; ++i) {
        displacements[i] = x[i];
    }
    return 0;
}

// The solution involves performing Gaussian elimination with partial pivoting on a copy of the stiffness matrix and a copy of the force vector. The main steps are: (1) validate input pointers and size (if any pointer is null or `numDOF<=0`, return `-1`; if `numDOF>100`, return `-3` handling max size); (2) copy the stiffness matrix and forces into local `std::vector<double>` containers (to avoid modifying original data); (3) perform forward elimination with row swapping to place the largest absolute pivot in the current column; if the maximum pivot is zero (or nearly zero, e.g., absolute value < 1e-12), return `-2`; (4) perform back substitution to compute displacements; (5) copy results to the output `displacements` array and return `0`. Edge cases include: `numDOF=1` (direct solve if pivot non-zero), non-zero forces but zero matrix (singular), multiple zero pivots, and large values that may cause floating-point errors. Time complexity is \(O(n^3)\) for elimination and \(O(n^2)\) for back substitution, space complexity is \(O(n^2)\) for the matrix copy and \(O(n)\) for vectors. Use `std::vector` with `reserve` to avoid reallocation.
