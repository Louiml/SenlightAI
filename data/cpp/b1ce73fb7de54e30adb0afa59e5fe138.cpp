// Write a C++ function `isPermutation(const std::vector<std::vector<int>>& matrix)` that determines whether a square matrix of size n×n contains exactly the integers from 1 to n², each appearing exactly once (i.e., it forms a valid permutation of numbers 1 through n²). The function should return `true` if the matrix is a valid permutation and `false` otherwise. Handle edge cases such as empty matrices, non-square matrices, numbers outside the valid range, and duplicate or missing values. Do not modify the input matrix, and use appropriate `const` correctness throughout.
// The solution validates the matrix in a single pass through all its elements. First, check that the matrix is non-empty and square; if not, return `false` immediately. Then, create a boolean array (or a `std::vector<bool>`) of size n²+1 (index 0 unused) to track which numbers have been seen. For each cell, read the value and verify it is within the range 1 to n²; if it is out of range, return `false`. If the value has already been marked as seen, there is a duplicate, so return `false`; otherwise, mark it as seen. After processing all cells, scan the tracking array from index 1 to n² to ensure every number was seen; if any is unmarked, return `false`. If all checks pass, return `true`. Time complexity is O(n²) for the matrix traversal plus O(n²) for the final verification, totaling O(n²). Space complexity is O(n²) for the tracking array. Edge cases include an empty matrix (size 0) — this technically has n²=0 numbers, but since the range 1 to 0 is empty, we define it as `false` to avoid ambiguity; non-square matrices are rejected because a permutation requires a square grid.
#include <vector>
#include <algorithm>

// Determines whether a square matrix contains exactly the integers 1..n^2, each once.
// Returns false for empty or non-square matrices.
bool isPermutation(const std::vector<std::vector<int>>& matrix) {
    const std::size_t n = matrix.size();
    if (n == 0) return false; // Empty matrix is not a valid permutation

    // Check that the matrix is square
    for (const auto& row : matrix) {
        if (row.size() != n) return false;
    }

    const std::size_t nSquared = n * n;
    std::vector<bool> seen(nSquared + 1, false); // indices 1..nSquared used

    for (const auto& row : matrix) {
        for (int value : row) {
            if (value < 1 || static_cast<std::size_t>(value) > nSquared) {
                return false; // Out of the required range
            }
            if (seen[static_cast<std::size_t>(value)]) {
                return false; // Duplicate found
            }
            seen[static_cast<std::size_t>(value)] = true;
        }
    }

    // Verify that all numbers 1..nSquared are present
    for (std::size_t i = 1; i <= nSquared; ++i) {
        if (!seen[i]) return false;
    }

    return true;
}
#include <cassert>
#include <vector>

int main() {
    // Valid 3x3 permutation
    std::vector<std::vector<int>> valid = {{1,2,3},{4,5,6},{7,8,9}};
    assert(isPermutation(valid) == true);

    // Duplicate number
    std::vector<std::vector<int>> duplicate = {{1,2,3},{4,5,5},{7,8,9}};
    assert(isPermutation(duplicate) == false);

    // Missing number (2 missing, 10 appears)
    std::vector<std::vector<int>> missing = {{1,3,4},{5,6,7},{8,9,10}};
    assert(isPermutation(missing) == false);

    // Number out of range (0)
    std::vector<std::vector<int>> zero = {{1,2},{3,0}};
    assert(isPermutation(zero) == false);

    // Number out of range (too large)
    std::vector<std::vector<int>> large = {{1,2},{3,5}};
    assert(isPermutation(large) == false);

    // Non-square matrix
    std::vector<std::vector<int>> nonSquare = {{1,2,3},{4,5,6}};
    assert(isPermutation(nonSquare) == false);

    // Empty matrix
    std::vector<std::vector<int>> empty;
    assert(isPermutation(empty) == false);

    // Valid 1x1 matrix
    std::vector<std::vector<int>> single = {{1}};
    assert(isPermutation(single) == true);

    // Valid 2x2 matrix (shuffled)
    std::vector<std::vector<int>> shuffled = {{4,2},{1,3}};
    assert(isPermutation(shuffled) == true);

    // Valid 4x4 matrix
    std::vector<std::vector<int>> four = {{16,2,3,13},{5,11,10,8},{9,7,6,12},{4,14,15,1}};
    assert(isPermutation(four) == true);

    return 0;
}
