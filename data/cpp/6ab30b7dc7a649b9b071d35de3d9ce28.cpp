Write a C++ function `double sumDiagonals(const float matrix[6][6])` that takes a 6x6 matrix of floating-point values and returns the sum of the main diagonal (from top-left to bottom-right) plus the anti-diagonal (from top-right to bottom-left), with care to count the center element only once if the matrix were odd-sized; however, for a 6x6 matrix, no element lies on both diagonals. The original snippet incorrectly attempts to compute diagonal sums using nested loops that increment `i` inside the inner loop, leading to skipping elements and undefined behavior (reading out of bounds). Your function must compute both diagonal sums correctly by iterating along the diagonals directly. Return the total as a `double` (since the input is float). Do not print or read input; just return the computed value. The function should be `const`-correct and handle any floating-point values, including negative numbers or zeros.

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Identity-like matrix with 1 on main diagonal, 0 elsewhere.
    // Main sum = 6, anti sum = 0, total = 6.
    float mat1[6][6] = {};
    for (int i = 0; i < 6; ++i) mat1[i][i] = 1.0f;
    assert(std::fabs(sumDiagonals(mat1) - 6.0) < 1e-6);

    // Test 2: Matrix with 1 on anti-diagonal, 0 elsewhere.
    // Main sum = 0, anti sum = 6, total = 6.
    float mat2[6][6] = {};
    for (int i = 0; i < 6; ++i) mat2[i][5-i] = 1.0f;
    assert(std::fabs(sumDiagonals(mat2) - 6.0) < 1e-6);

    // Test 3: All zeros -> total = 0.
    float mat3[6][6] = {};
    assert(sumDiagonals(mat3) == 0.0);

    // Test 4: All ones -> main = 6, anti = 6, total = 12.
    float mat4[6][6];
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            mat4[i][j] = 1.0f;
    assert(std::fabs(sumDiagonals(mat4) - 12.0) < 1e-6);

    // Test 5: Mixed values, check manually.
    // Main: 1,2,3,4,5,6 sum=21. Anti: 7,8,9,10,11,12 sum=57. Total=78.
    float mat5[6][6] = {};
    // Fill main diagonal
    int vals[] = {1,2,3,4,5,6};
    for (int i = 0; i < 6; ++i) mat5[i][i] = vals[i];
    // Fill anti-diagonal (may overwrite some if overlapping? Not here)
    int anti[] = {7,8,9,10,11,12};
    for (int i = 0; i < 6; ++i) mat5[i][5-i] = anti[i];
    // Since no overlap, total should be 21+57=78.
    assert(std::fabs(sumDiagonals(mat5) - 78.0) < 1e-6);

    // Test 6: Negative values on diagonals.
    float mat6[6][6] = {};
    for (int i = 0; i < 6; ++i) mat6[i][i] = -1.0f;
    for (int i = 0; i < 6; ++i) mat6[i][5-i] = -2.0f;
    // Main sum = -6, anti sum = -12, total = -18.
    assert(std::fabs(sumDiagonals(mat6) + 18.0) < 1e-6);

    return 0;
}

#include <cstddef> // for size_t

// Computes the sum of the main diagonal (top-left to bottom-right) and the
// anti-diagonal (top-right to bottom-left) of a 6x6 matrix.
// Returns the total sum as a double.
double sumDiagonals(const float matrix[6][6]) {
    double total = 0.0;
    const std::size_t N = 6; // fixed size

    for (std::size_t i = 0; i < N; ++i) {
        total += matrix[i][i];        // main diagonal
        total += matrix[i][N - 1 - i]; // anti-diagonal
    }
    return total;
}

// The main diagonal of a 6x6 matrix consists of elements where row index equals column index: `matrix[0][0], matrix[1][1], ..., matrix[5][5]`. The anti-diagonal consists of elements where row index plus column index equals 5 (since N=6): `matrix[0][5], matrix[1][4], ..., matrix[5][0]`. For a 6x6 matrix, these diagonals are disjoint because 6 is even; no index pair satisfies both `i==j` and `i+j==5` simultaneously (that would require `i = i+5`, impossible). Therefore, we can safely sum both diagonals independently without worrying about double-counting. Iterate with a single loop variable `i` from 0 to 5, adding `matrix[i][i]` for the main diagonal and `matrix[i][5-i]` for the anti-diagonal. Sum both into a `double` accumulator. Edge cases: if the matrix contains negative numbers, the sum works normally; if the matrix is all zeros, the sum is zero. Time complexity is O(1) because the matrix size is fixed at 6x6 (constant 12 additions). Auxiliary space is O(1).
