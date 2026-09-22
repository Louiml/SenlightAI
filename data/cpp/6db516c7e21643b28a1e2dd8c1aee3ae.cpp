// Write a C++ function named `generateNumberSquare` that takes an integer `n` (where `n >= 1`) and returns a `std::vector<std::vector<int>>` representing an `n x n` matrix. The matrix should be filled row by row with consecutive integers starting from 1, increasing by 1 each time. For example, if `n = 3`, the output should be `{{1,2,3}, {4,5,6}, {7,8,9}}`. The function must not print anything; it must return the matrix so the caller can use or assert on it. Handle the smallest valid input `n = 1` correctly, returning `{{1}}`. Assume `n` is positive and no other validation is required.
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    assert(generateNumberSquare(1) == std::vector<std::vector<int>>{{1}});
    assert(generateNumberSquare(2) == std::vector<std::vector<int>>{{1,2},{3,4}});
    assert(generateNumberSquare(3) == std::vector<std::vector<int>>{{1,2,3},{4,5,6},{7,8,9}});
    assert(generateNumberSquare(4) == std::vector<std::vector<int>>{{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}});
    assert(generateNumberSquare(5) == std::vector<std::vector<int>>{{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20},{21,22,23,24,25}});
}
#include <vector>

// Generate an n x n matrix filled row-wise with consecutive integers starting from 1.
std::vector<std::vector<int>> generateNumberSquare(int n) {
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    int count = 1;
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            matrix[row][col] = count++;
        }
    }
    return matrix;
}
// The core idea is to simulate the original loop-based number-filling process, but instead of printing, we store values into a 2D vector. Use two nested loops: an outer loop for the current row (from 0 to n-1), and an inner loop for the current column (also from 0 to n-1). Maintain a running integer counter that starts at 1 and increments after each assignment. For each cell, assign the current counter value to `matrix[row][col]`, then increment the counter. The matrix is initialized with `n` rows and `n` columns, all initially zero, which works fine since every cell is overwritten. The main edge case is `n = 1`, where only one iteration occurs, producing `{{1}}`. Since the matrix is exactly `n x n`, the total number of elements is `n²`, so we perform `n²` assignments. Time complexity is O(n²) because we visit each cell exactly once. Space complexity is O(n²) due to storing the matrix itself; no additional auxiliary space beyond a few integer variables and the loop indices is used.
