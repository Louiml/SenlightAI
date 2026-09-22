/*
Write a C++ function named `countPokemonOccurrences` that takes a non-empty square matrix of positive integers (represented as a `std::vector<std::vector<int>>`) and a target integer `target`, and returns an integer count of how many times `target` appears in the matrix. The input matrix is guaranteed to be square (rows == columns) and contain at least one element. The function must not modify the input matrix. The count should be a simple frequency count: each occurrence contributes to the total, even if the same value appears multiple times in the same row or column. For example, if the matrix is `{{1,2},{2,3}}` and `target = 2`, the function returns `2`. The function signature is: `int countPokemonOccurrences(const std::vector<std::vector<int>>& matrix, int target);` Note that the target may not exist in the matrix, in which case the return value must be `0`.
*/

#include <vector>

// Count occurrences of a target value in a square matrix.
// The matrix is read-only; no modifications are made.
// Returns the number of times 'target' appears in the matrix.
int countPokemonOccurrences(const std::vector<std::vector<int>>& matrix, int target) {
    int count = 0;
    for (const auto& row : matrix) {           // Iterate over each row
        for (int value : row) {                 // Iterate over each element in the row
            if (value == target) {
                ++count;                        // Increment counter on match
            }
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Declare the function to test (in a real setup, this would be in a header)
int countPokemonOccurrences(const std::vector<std::vector<int>>& matrix, int target);

int main() {
    // Test 1: Basic case with multiple matches
    std::vector<std::vector<int>> matrix1 = {{1, 2, 3}, {2, 2, 4}, {5, 2, 6}};
    assert(countPokemonOccurrences(matrix1, 2) == 4);

    // Test 2: Target not present
    std::vector<std::vector<int>> matrix2 = {{1, 2}, {3, 4}};
    assert(countPokemonOccurrences(matrix2, 9) == 0);

    // Test 3: Single element matrix
    std::vector<std::vector<int>> matrix3 = {{7}};
    assert(countPokemonOccurrences(matrix3, 7) == 1);
    assert(countPokemonOccurrences(matrix3, 3) == 0);

    // Test 4: All elements equal to target
    std::vector<std::vector<int>> matrix4 = {{5, 5}, {5, 5}};
    assert(countPokemonOccurrences(matrix4, 5) == 4);

    // Test 5: Negative numbers and zero
    std::vector<std::vector<int>> matrix5 = {{-1, 0, -1}, {0, -1, 0}, {3, 0, -1}};
    assert(countPokemonOccurrences(matrix5, -1) == 4);
    assert(countPokemonOccurrences(matrix5, 0) == 3);

    // Test 6: Matrix of size 3x3, target appears once
    std::vector<std::vector<int>> matrix6 = {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}};
    assert(countPokemonOccurrences(matrix6, 50) == 1);

    // Test 7: Ensure original matrix is not modified (const correctness check)
    std::vector<std::vector<int>> matrix7 = {{1, 1}, {1, 1}};
    countPokemonOccurrences(matrix7, 1);
    assert(matrix7[0][0] == 1 && matrix7[0][1] == 1 && matrix7[1][0] == 1 && matrix7[1][1] == 1);

    // Test 8: Large matrix with no matches (e.g., 4x4)
    std::vector<std::vector<int>> matrix8 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    assert(countPokemonOccurrences(matrix8, 17) == 0);

    return 0;
}

// The solution is straightforward: iterate through every element of the 2D vector using nested loops, comparing each element to the target, and incrementing a counter whenever they match. Since the matrix is square, we can assume it is non-empty and correctly sized; no need to validate dimensions. Edge cases include: the target not appearing at all (returns 0), the target appearing in every cell (returns total cells), and negative or zero values—the function handles these because it only checks equality, not positivity. Time complexity is \(O(n^2)\) where \(n\) is the number of rows (or columns), because we visit each element exactly once. Space complexity is \(O(1)\) since we only use a single integer counter, and we do not copy the matrix (the input is taken by const reference, ensuring no modification and no extra memory).
