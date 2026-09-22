/*
Write a C++ function named `sumEachRow` that takes a constant reference to a square 2D array of integers (represented as `std::array<std::array<int, N>, N>` or, more flexibly, a `std::vector<std::vector<int>>`) and returns a `std::vector<int>` containing the sum of each row, in the order the rows appear. The input matrix may have any size (including 0 rows), but if non-empty, every row must have the same length as the number of rows (i.e., it must be square). Duplicate values are allowed, and negative numbers may appear. The function should not modify the input matrix and should handle a matrix with zero rows by returning an empty vector. For each row, compute the sum of all its elements. The main challenge is to correctly traverse the 2D structure, accumulate each row's total, and store the results in a new vector, preserving row order. Edge cases include an empty matrix (no rows) and a matrix with a single row and a single element, which should produce a vector containing that element's value.
*/

#include <vector>
#include <cstddef> // for size_t

// Sum each row of a square 2D vector of ints.
// Returns a vector where result[i] = sum of row i.
// Handles empty input (returns empty vector).
std::vector<int> sumEachRow(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> rowSums;
    rowSums.reserve(matrix.size()); // optional optimization

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        int sum = 0;
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            sum += matrix[i][j];
        }
        rowSums.push_back(sum);
    }
    return rowSums;
}

#include <cassert>
#include <vector>

// (solution function above)

int main() {
    // Empty matrix
    std::vector<std::vector<int>> empty;
    assert(sumEachRow(empty) == std::vector<int>());

    // 1x1 matrix
    std::vector<std::vector<int>> single = {{7}};
    assert(sumEachRow(single) == std::vector<int>{7});

    // 2x2 matrix with mixed signs
    std::vector<std::vector<int>> small = {{1, -2}, {3, 4}};
    assert(sumEachRow(small) == (std::vector<int>{-1, 7}));

    // 3x3 matrix as in the original snippet
    std::vector<std::vector<int>> grade = {
        {100, 100, 100},
        {90, 50, 100},
        {60, 70, 80}
    };
    assert(sumEachRow(grade) == (std::vector<int>{300, 240, 210}));

    // Larger matrix with duplicates and negatives
    std::vector<std::vector<int>> large = {
        {0, 0, 0},
        {-5, -5, -5},
        {10, -10, 10}
    };
    assert(sumEachRow(large) == (std::vector<int>{0, -15, 10}));

    return 0;
}

// The solution involves iterating through each row of the matrix (outer loop) and, for each row, iterating through each element (inner loop) to accumulate the sum using a local integer variable initialized to 0. After the inner loop completes, push the row sum into a result vector. Since the matrix is square, the number of columns equals the number of rows, but the algorithm works for any rectangular matrix as long as we only access valid indices. Time complexity is O(n^2) for an n×n matrix, as we visit each element exactly once. Space complexity is O(n) for the result vector (plus the size of the input passed by reference, which is not counted). Edge cases: for an empty vector of rows, the outer loop runs zero times, returning an empty vector; for a matrix with one row and one column, the inner loop runs once, returning a vector with that single value. The function uses `const` reference to avoid copying the entire matrix, ensuring efficiency and correctness. We use `std::accumulate` or a manual loop; manual loop is clearer for teaching. No special handling is needed for negative or duplicate values, as they are simply added.
