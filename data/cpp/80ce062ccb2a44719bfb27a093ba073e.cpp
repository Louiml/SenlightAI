/*
Write a standalone C++ function that takes a 2D matrix (represented as `std::vector<std::vector<double>>`) and a 1D vector (represented as `std::vector<double>`) of the same length as the number of rows of the matrix, and returns a new matrix where the vector is added element-wise to every column of the original matrix. The function must not modify the input matrix or vector. Handle the edge case where the input matrix has zero columns (i.e., a matrix with rows but no columns) gracefully by returning an empty matrix. Assume the vector length always matches the matrix row count; if not, throw an `std::invalid_argument`. The function must be `const`-correct, using `const` references for inputs.
*/
#include <vector>
#include <stdexcept>

// Returns a new matrix where the given vector is added to each column.
std::vector<std::vector<double>> addVectorToEachColumn(
    const std::vector<std::vector<double>>& matrix,
    const std::vector<double>& vec) {
    
    // Validate dimensions.
    size_t rows = matrix.size();
    if (vec.size() != rows) {
        throw std::invalid_argument("Vector size must match number of rows");
    }
    
    // Handle empty matrix.
    if (rows == 0) {
        return {};
    }
    
    size_t cols = matrix[0].size();
    
    // Initialize result with same dimensions.
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    
    // Add vector to each column.
    for (size_t c = 0; c < cols; ++c) {
        for (size_t r = 0; r < rows; ++r) {
            result[r][c] = matrix[r][c] + vec[r];
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Basic 2x4 example from the snippet.
    std::vector<std::vector<double>> mat1 = {
        {1, 2, 6, 9},
        {3, 1, 7, 2}
    };
    std::vector<double> v1 = {0, 1};
    auto res1 = addVectorToEachColumn(mat1, v1);
    std::vector<std::vector<double>> expected1 = {
        {1, 2, 6, 9},
        {4, 2, 8, 3}
    };
    assert(res1 == expected1);

    // Single row matrix.
    std::vector<std::vector<double>> mat2 = {{5, 10, 15}};
    std::vector<double> v2 = {2};
    auto res2 = addVectorToEachColumn(mat2, v2);
    std::vector<std::vector<double>> expected2 = {{7, 12, 17}};
    assert(res2 == expected2);

    // Single column matrix.
    std::vector<std::vector<double>> mat3 = {{1}, {2}, {3}};
    std::vector<double> v3 = {10, 20, 30};
    auto res3 = addVectorToEachColumn(mat3, v3);
    std::vector<std::vector<double>> expected3 = {{11}, {22}, {33}};
    assert(res3 == expected3);

    // Matrix with zero columns (rows exist but empty inner vectors).
    std::vector<std::vector<double>> mat4 = {{}, {}};
    std::vector<double> v4 = {1, 2};
    auto res4 = addVectorToEachColumn(mat4, v4);
    assert(res4 == std::vector<std::vector<double>>({{}, {}}));

    // Empty matrix with empty vector.
    std::vector<std::vector<double>> mat5 = {};
    std::vector<double> v5 = {};
    auto res5 = addVectorToEachColumn(mat5, v5);
    assert(res5.empty());

    // Negative values.
    std::vector<std::vector<double>> mat6 = {{-1, -2}, {-3, -4}};
    std::vector<double> v6 = {1, -1};
    auto res6 = addVectorToEachColumn(mat6, v6);
    std::vector<std::vector<double>> expected6 = {{0, -1}, {-4, -5}};
    assert(res6 == expected6);

    // Dimension mismatch throws.
    bool threw = false;
    try {
        std::vector<std::vector<double>> mat7 = {{1, 2}, {3, 4}};
        std::vector<double> v7 = {1};
        addVectorToEachColumn(mat7, v7);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}
// The solution iterates over each column index and each row index, adding the corresponding vector element to the matrix element at that row and column. The main steps are: (1) validate that the vector size equals the number of rows; if not, throw `std::invalid_argument`. (2) Determine the number of columns as `matrix[0].size()` if rows > 0, else 0. (3) Create a result matrix with the same dimensions. (4) For each column `c` and row `r`, set `result[r][c] = matrix[r][c] + vec[r]`. This is a straightforward element-wise operation with no dependencies between columns, so it is naturally parallelizable but not required. Edge cases include: empty matrix (zero rows) — the vector size should be 0 to pass validation, and the result is empty; matrix with rows but zero columns — the result is a matrix with the same number of rows but empty inner vectors. Time complexity is \(O(R \times C)\) where \(R\) is rows and \(C\) is columns, and space complexity is \(O(R \times C)\) for the result (excluding input storage).
