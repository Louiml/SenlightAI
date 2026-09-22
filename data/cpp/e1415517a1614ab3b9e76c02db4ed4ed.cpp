Write a C++ function that takes a non-negative integer `n` and returns a 2D vector (vector of vectors) of integers with `n` rows and 3 columns, where every element is set to the row index multiplied by 100 plus the column index (i.e., `value = row * 100 + col`). For example, for `n = 2`, the matrix should be:
```
0 1 2
100 101 102
```
The function must correctly handle the case where `n == 0` by returning an empty vector (i.e., a vector with 0 rows). Assume `n` is small enough that memory is not a concern. Your solution must demonstrate proper use of `const` correctness where applicable – specifically, the function should return by value, and the returned matrix (if non-empty) should have `const` applied to the outer loop variable when iterating over rows to avoid accidental modification. Provide the function definition only (no `main`). The function signature should be: `std::vector<std::vector<int>> buildMatrix(int n)`.
#include <cassert>
#include <vector>
// Assume buildMatrix is defined above.

int main() {
    // Test n = 0: empty matrix.
    std::vector<std::vector<int>> empty = buildMatrix(0);
    assert(empty.empty());

    // Test n = 1: single row with 0,1,2.
    std::vector<std::vector<int>> one = buildMatrix(1);
    assert(one.size() == 1);
    assert(one[0].size() == 3);
    assert(one[0][0] == 0);
    assert(one[0][1] == 1);
    assert(one[0][2] == 2);

    // Test n = 2: two rows.
    std::vector<std::vector<int>> two = buildMatrix(2);
    assert(two.size() == 2);
    assert(two[0][0] == 0 && two[0][1] == 1 && two[0][2] == 2);
    assert(two[1][0] == 100 && two[1][1] == 101 && two[1][2] == 102);

    // Test n = 5: check a few specific cells.
    std::vector<std::vector<int>> five = buildMatrix(5);
    assert(five.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(five[i].size() == 3);
        for (int j = 0; j < 3; ++j) {
            assert(five[i][j] == i * 100 + j);
        }
    }
}
#include <vector>

// Build a matrix of size n x 3 where element (row, col) = row * 100 + col.
std::vector<std::vector<int>> buildMatrix(int n) {
    // Initialize outer vector with n rows, each containing 3 default-initialized (0) integers.
    std::vector<std::vector<int>> matrix(n, std::vector<int>(3, 0));

    // Fill each cell with the desired value.
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < 3; ++col) {
            matrix[row][col] = row * 100 + col;
        }
    }

    return matrix;
}
// The solution involves constructing a 2D vector with `n` rows, each row being a vector of 3 integers. The core algorithm is straightforward: reserve or allocate the outer vector with `n` default-constructed rows, then loop over the rows and columns to assign each element according to the formula `row * 100 + col`. For edge cases: if `n == 0`, the constructor `std::vector<std::vector<int>>(0)` returns an empty vector, which is correct. For positive `n`, the inner vectors are initially empty, but since we know the size is 3, we can directly use `std::vector<int>(3)` for each row, or resize each row to 3 before assignment. However, a cleaner approach is to create the outer vector with `n` rows, each initialized to `std::vector<int>(3)` (all zeros), then fill in values via nested loops. Using `const` in a range-based for loop over the outer vector when iterating for printing (in the test) is good practice, but in the function itself, since we are modifying values, we cannot use `const` on non-const references; we only need to ensure we return by value. The time complexity is O(n) because each of the `n * 3` cells is assigned once (constant 3 columns). Space complexity is O(n) for the matrix itself, since each row has fixed length 3.
