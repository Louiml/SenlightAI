/*
Write a C++ function named `diagonalCopy` that takes a constant reference to an `Eigen::Matrix3i` matrix and returns an `Eigen::Vector3i` containing the coefficients on the main diagonal (from top-left to bottom-right) in order. The function must work for any 3x3 integer matrix, including those with negative values, zeros, or repeated entries. Do not use any external Eigen functions like `.diagonal()`; manually extract the three diagonal elements using indexing.
*/
#include <Eigen/Core>

// Return a Vector3i containing the main diagonal of a 3x3 matrix.
Eigen::Vector3i diagonalCopy(const Eigen::Matrix3i& m) {
    return Eigen::Vector3i(m(0,0), m(1,1), m(2,2));
}
#include <cassert>
#include <Eigen/Core>

// The solution function (must be included in the same translation unit)
Eigen::Vector3i diagonalCopy(const Eigen::Matrix3i& m) {
    return Eigen::Vector3i(m(0,0), m(1,1), m(2,2));
}

int main() {
    // Test 1: Identity matrix
    Eigen::Matrix3i m1 = Eigen::Matrix3i::Identity();
    assert(diagonalCopy(m1) == Eigen::Vector3i(1, 1, 1));

    // Test 2: All zeros
    Eigen::Matrix3i m2 = Eigen::Matrix3i::Zero();
    assert(diagonalCopy(m2) == Eigen::Vector3i(0, 0, 0));

    // Test 3: All ones constant matrix
    Eigen::Matrix3i m3 = Eigen::Matrix3i::Constant(1);
    assert(diagonalCopy(m3) == Eigen::Vector3i(1, 1, 1));

    // Test 4: Random-like matrix with known diagonal
    Eigen::Matrix3i m4;
    m4 << 5, 1, 2,
          3, -4, 6,
          7, 8, 9;
    assert(diagonalCopy(m4) == Eigen::Vector3i(5, -4, 9));

    // Test 5: Negative diagonal values
    Eigen::Matrix3i m5;
    m5 << -1, 0, 0,
          0, -2, 0,
          0, 0, -3;
    assert(diagonalCopy(m5) == Eigen::Vector3i(-1, -2, -3));

    // Test 6: Diagonal elements not related to row/column sums (custom)
    Eigen::Matrix3i m6;
    m6 << 100, 2, 3,
          4, -50, 6,
          7, 8, 200;
    assert(diagonalCopy(m6) == Eigen::Vector3i(100, -50, 200));

    // Test 7: Mixed large and small values
    Eigen::Matrix3i m7;
    m7 << 1, 0, 0,
          0, 2, 0,
          0, 0, 3;
    assert(diagonalCopy(m7) == Eigen::Vector3i(1, 2, 3));

    // Test 8: Repeated diagonal values
    Eigen::Matrix3i m8;
    m8 << 7, 1, 1,
          1, 7, 1,
          1, 1, 7;
    assert(diagonalCopy(m8) == Eigen::Vector3i(7, 7, 7));

    return 0;
}
// The main diagonal of a 3x3 matrix consists of the elements at positions (0,0), (1,1), and (2,2). Since the matrix size is fixed, the algorithm is straightforward: access each of these three cells using the `operator()` or `operator[]` of the Eigen matrix and place them into a `Vector3i` in order. No special edge cases exist beyond handling any integer values; the function should be `const` correct because it does not modify the input matrix. Time complexity is O(1) because we only perform three constant-time accesses. Space complexity is O(1) for the returned vector, plus the input matrix itself which is not copied.
