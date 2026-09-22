/*
Write a C++ function that accepts a `const Eigen::Matrix3i&` (a 3x3 integer matrix, as provided by the Eigen library) and returns a `std::pair<int,int>` containing the smallest and largest coefficient on the main diagonal (i.e., entries at positions (0,0), (1,1), (2,2)). If the matrix is not 3x3, the behavior is undefined, but you may assume it is always exactly 3x3. The function must be `const`-correct, meaning it must not modify the input matrix and it must be callable on constant matrices. The result pair should have the minimum as the first element and the maximum as the second element. If all diagonal entries are equal, both elements will be that same value.
*/
#include <Eigen/Dense>
#include <utility>
#include <algorithm>

// Returns the minimum and maximum coefficients on the main diagonal of a 3x3 integer matrix.
std::pair<int,int> diagonalMinMax(const Eigen::Matrix3i& m) {
    // Eigen's diagonal() returns a vector expression; assigning to a fixed-size vector is safe.
    Eigen::Vector3i diag = m.diagonal();

    int min_val = diag[0];
    int max_val = diag[0];

    // Only two remaining elements to check (indices 1 and 2).
    for (int i = 1; i < 3; ++i) {
        min_val = std::min(min_val, diag[i]);
        max_val = std::max(max_val, diag[i]);
    }

    return std::make_pair(min_val, max_val);
}
#include <cassert>
#include <Eigen/Dense>
#include <utility>

// The solution function is declared above; include it here for testing.
std::pair<int,int> diagonalMinMax(const Eigen::Matrix3i& m);

int main() {
    // Test 1: distinct diagonal values
    Eigen::Matrix3i m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    assert(diagonalMinMax(m1) == std::make_pair(1, 9));

    // Test 2: all same diagonal values
    Eigen::Matrix3i m2;
    m2 << 5, 0, 0,
          0, 5, 0,
          0, 0, 5;
    assert(diagonalMinMax(m2) == std::make_pair(5, 5));

    // Test 3: negative diagonal values
    Eigen::Matrix3i m3;
    m3 << -3, 1, 2,
          4, -7, 8,
          9, -1, 10;
    // Diagonal: -3, -7, 10 → min = -7, max = 10
    assert(diagonalMinMax(m3) == std::make_pair(-7, 10));

    // Test 4: zeros and positive numbers
    Eigen::Matrix3i m4;
    m4 << 0, 1, 2,
          3, 4, 5,
          6, 7, 8;
    assert(diagonalMinMax(m4) == std::make_pair(0, 8));

    // Test 5: all negative equal values
    Eigen::Matrix3i m5;
    m5 << -2, 0, 0,
          0, -2, 0,
          0, 0, -2;
    assert(diagonalMinMax(m5) == std::make_pair(-2, -2));

    // Test 6: const correctness – call on a const reference
    const Eigen::Matrix3i m6 = m1;
    assert(diagonalMinMax(m6) == std::make_pair(1, 9));

    // Test 7: ensure input matrix is not modified (check a non-diagonal entry)
    Eigen::Matrix3i m7;
    m7 << 1, 100, 2,
          3, 4, 5,
          6, 7, 8;
    Eigen::Matrix3i original = m7;
    (void)diagonalMinMax(m7);
    assert(m7 == original); // Matrix unchanged
    assert(diagonalMinMax(m7) == std::make_pair(1, 8));

    return 0;
}
// The main algorithm is straightforward: extract the three diagonal coefficients from the matrix using Eigen's `.diagonal()` method, which returns a vector expression of size 3, then iterate over that vector to find the minimum and maximum. Initialize both `min_val` and `max_val` to the first diagonal element, then compare with the remaining two elements, updating as needed. Edge cases: there are exactly three elements, so no empty input handling is needed; duplicates are naturally handled because comparisons use `<=`/`>=` and no special logic is required. Time complexity is O(1) because the matrix size is fixed at 3; space complexity is O(1) beyond the input reference. The solution must include `<Eigen/Dense>` header, and use `std::pair<int,int>` for the return. The function signature should be `std::pair<int,int> diagonalMinMax(const Eigen::Matrix3i& m)`.
