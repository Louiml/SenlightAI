/*
Write a C++ function `searchMatrix` that takes a 2D vector of integers (a matrix) sorted in non-decreasing order both row-wise and column-wise, where each row is sorted left-to-right and the first integer of each row is greater than the last integer of the previous row, and a target integer. The function must return `true` if the target exists in the matrix, otherwise `false`. The matrix may be empty (zero rows) or contain empty rows, in which case the function should return `false`. The solution must be efficient, running in better than linear time relative to the total number of elements.
*/

#include <vector>

// Searches for a target value in a matrix where each row is sorted from left to right
// and the first integer of each row is greater than the last integer of the previous row.
// Returns true if found, false otherwise. Handles empty matrices and empty rows.
bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) {
        return false;
    }

    const int n = static_cast<int>(matrix.size());
    const int m = static_cast<int>(matrix[0].size());

    int low = 0;
    int high = n * m - 1;

    while (low <= high) {
        const int mid = low + (high - low) / 2;
        const int row = mid / m;
        const int col = mid % m;
        const int value = matrix[row][col];

        if (value == target) {
            return true;
        } else if (value < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return false;
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test (included in the solution section)
bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target);

int main() {
    // Example 1 from problem statement
    std::vector<std::vector<int>> mat1 = {{1,3,5,7},{10,11,16,20},{23,30,34,60}};
    assert(searchMatrix(mat1, 3) == true);
    assert(searchMatrix(mat1, 13) == false);
    assert(searchMatrix(mat1, 60) == true);
    assert(searchMatrix(mat1, 0) == false);

    // Single element matrix
    std::vector<std::vector<int>> mat2 = {{5}};
    assert(searchMatrix(mat2, 5) == true);
    assert(searchMatrix(mat2, 6) == false);

    // Single row
    std::vector<std::vector<int>> mat3 = {{1,2,3,4}};
    assert(searchMatrix(mat3, 1) == true);
    assert(searchMatrix(mat3, 4) == true);
    assert(searchMatrix(mat3, 5) == false);

    // Single column
    std::vector<std::vector<int>> mat4 = {{1},{2},{3}};
    assert(searchMatrix(mat4, 2) == true);
    assert(searchMatrix(mat4, 0) == false);
    assert(searchMatrix(mat4, 3) == true);

    // Empty matrix (no rows)
    std::vector<std::vector<int>> mat5;
    assert(searchMatrix(mat5, 1) == false);

    // Matrix with zero columns (empty rows)
    std::vector<std::vector<int>> mat6 = {{}, {}};
    assert(searchMatrix(mat6, 1) == false);

    // Large values and negative numbers
    std::vector<std::vector<int>> mat7 = {{-10,-5},{0,5},{10,15}};
    assert(searchMatrix(mat7, -10) == true);
    assert(searchMatrix(mat7, 0) == true);
    assert(searchMatrix(mat7, 7) == false);
    assert(searchMatrix(mat7, 15) == true);

    // Two rows and two columns, target in first row end
    std::vector<std::vector<int>> mat8 = {{1,3},{5,7}};
    assert(searchMatrix(mat8, 3) == true);
    assert(searchMatrix(mat8, 5) == true);
    assert(searchMatrix(mat8, 4) == false);

    return 0;
}

// The key observation is that because the rows are sorted and the first element of each row is greater than the last element of the previous row, the entire matrix can be conceptually flattened into a single sorted array of length `n * m` (where `n` is the number of rows and `m` is the number of columns). This allows us to apply binary search on this virtual 1D array. To map a virtual index `mid` (ranging from 0 to `n*m - 1`) to actual matrix coordinates, we use `row = mid / m` and `col = mid % m`. We then compare `matrix[row][col]` with the target. If equal, return `true`; if less, move the lower bound up; if greater, move the upper bound down. Edge cases include an empty matrix (where `matrix.size() == 0`) or a matrix with zero columns (e.g., `matrix = {{}}`), both of which should immediately return `false` since there are no elements to search. Also, handle the case where `n*m` could be large by avoiding integer overflow in `mid` calculation using `low + (high - low) / 2`. Time complexity is O(log(n*m)) because each iteration halves the search space. Space complexity is O(1) extra space.
