// Write a standalone C++ function named `queryMatrixElements` that takes two parameters: a vector of vectors of integers representing a jagged 2D array (where each inner vector may have a different length), and a vector of pairs of integers representing queries. Each query is an ordered pair `(row, column)`. The function should return a vector of integers where the `i`-th result is the element at the specified row and column for the `i`-th query. The function must validate that every queried row index is within `[0, rows-1]` and every queried column index is within `[0, size_of_that_row - 1]`. If any query is out of bounds, the function should throw an `std::out_of_range` exception with a descriptive message. The input rows may be empty (size zero), in which case no column index is valid. The function must not modify the input matrix.

#include <cassert>
#include <vector>
#include <utility>
#include <stdexcept>

// The solution function is declared here for the test (in practice, include the above solution).
std::vector<int> queryMatrixElements(
    const std::vector<std::vector<int>>& matrix,
    const std::vector<std::pair<int, int>>& queries);

int main() {
    // Basic jagged matrix test
    std::vector<std::vector<int>> mat = {{1, 2}, {3, 4, 5}, {6}};
    std::vector<std::pair<int, int>> q1 = {{0, 0}, {1, 2}, {2, 0}};
    std::vector<int> r1 = queryMatrixElements(mat, q1);
    assert((r1 == std::vector<int>{1, 5, 6}));

    // Single-row single-column
    std::vector<std::vector<int>> mat2 = {{42}};
    std::vector<std::pair<int, int>> q2 = {{0, 0}};
    assert(queryMatrixElements(mat2, q2) == std::vector<int>{42});

    // Empty rows (zero-length rows)
    std::vector<std::vector<int>> mat3 = {{}, {7, 8}};
    std::vector<std::pair<int, int>> q3 = {{1, 1}};
    assert((queryMatrixElements(mat3, q3) == std::vector<int>{8}));

    // Negative and out-of-range checks
    bool threw1 = false;
    try {
        queryMatrixElements(mat, {{0, 2}}); // col 2 invalid for row 0 (size 2)
    } catch (const std::out_of_range&) { threw1 = true; }
    assert(threw1);

    bool threw2 = false;
    try {
        queryMatrixElements(mat, {{-1, 0}}); // negative row
    } catch (const std::out_of_range&) { threw2 = true; }
    assert(threw2);

    bool threw3 = false;
    try {
        queryMatrixElements(mat, {{3, 0}}); // row 3 out of bounds
    } catch (const std::out_of_range&) { threw3 = true; }
    assert(threw3);

    bool threw4 = false;
    try {
        queryMatrixElements(mat3, {{0, 0}}); // row 0 is empty, col invalid
    } catch (const std::out_of_range&) { threw4 = true; }
    assert(threw4);

    // Boundary valid indices
    std::vector<std::pair<int, int>> q5 = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
    std::vector<int> r5 = queryMatrixElements(mat, q5);
    assert((r5 == std::vector<int>{2, 3, 4, 5}));

    // Const correctness: matrix not modified (compile-time check)
    const std::vector<std::vector<int>>& constMat = mat;
    const std::vector<std::pair<int, int>>& constQueries = q1;
    std::vector<int> r6 = queryMatrixElements(constMat, constQueries);
    assert((r6 == std::vector<int>{1, 5, 6}));
    
    return 0;
}

#include <vector>
#include <stdexcept>
#include <string>

// Return the elements of a jagged 2D vector at the given query positions.
// Throws std::out_of_range if any (row, column) is invalid.
std::vector<int> queryMatrixElements(
    const std::vector<std::vector<int>>& matrix,
    const std::vector<std::pair<int, int>>& queries) {
    
    std::vector<int> results;
    results.reserve(queries.size());
    
    for (const auto& query : queries) {
        int row = query.first;
        int col = query.second;
        
        // Validate row index
        if (row < 0 || row >= static_cast<int>(matrix.size())) {
            throw std::out_of_range("Row index out of bounds: " + std::to_string(row));
        }
        
        // Validate column index for that specific row
        const auto& currentRow = matrix[row];
        if (col < 0 || col >= static_cast<int>(currentRow.size())) {
            throw std::out_of_range("Column index out of bounds for row " + 
                                    std::to_string(row) + ": " + std::to_string(col));
        }
        
        results.push_back(currentRow[col]);
    }
    
    return results;
}

// The solution approach is straightforward: for each query, directly index into the matrix using the row and column values. Since the matrix is jagged, we must first check the row index against the total number of rows, then check the column index against the size of that specific row. We perform these checks before accessing to avoid undefined behavior. The main algorithm is a simple loop over all queries, performing constant-time access after validation. Edge cases include: empty matrix (zero rows), rows with zero length, queries with negative indices (since indices are ints but we should handle them as invalid), and queries exactly at the maximum valid indices. Time complexity is O(Q) where Q is the number of queries, because each query is processed in constant time (with validation also O(1)). Space complexity is O(Q) for the result vector, plus O(1) auxiliary space for loop variables. The function uses `const` references for the matrix and queries to avoid copying, and returns a vector by value.
