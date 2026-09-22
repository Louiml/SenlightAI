Write a standalone C++ function that takes a square sparse matrix in coordinate (COO) format (three vectors: row indices, column indices, and values, all of the same nonzero length) and a dense vector, and returns a new dense vector equal to the product of the sparse matrix and the dense vector. The matrix is assumed to be square (rows = cols). The function must handle empty input (zero nonzero elements) by returning a vector of zeros of the correct size, and must correctly accumulate contributions when multiple nonzero entries share the same row or column (which is common in COO input). The function must be `const`-correct and cannot modify the inputs.

// The main algorithm is straightforward sparse matrix-vector multiplication. For each nonzero entry `(row, col, value)` in the coordinate lists, we add `value * vector[col]` to `result[row]`. Because the input may contain duplicate `(row, col)` entries (i.e., multiple entries with the same row and column indices), we must accumulate all contributions rather than overwriting. Therefore, initialize the result vector with zeros of size equal to the number of rows (or columns since it's square), then loop over all nonzero entries and accumulate. Edge cases include: (1) empty input — return a zero vector of the given dimension; (2) a vector of wrong length — assume it's exactly the matrix column count; but for safety, the function could still index only within bounds, though the specification assumes correct lengths. Time complexity is O(nnz + n), where `nnz` is the number of nonzero entries and `n` is the matrix dimension (for allocation). Space complexity is O(n) for the result vector, plus O(1) auxiliary (excluding input storage). No sorting or rearranging of the input is needed.

#include <vector>
#include <cstddef>

// Multiply a square sparse matrix in COO format by a dense vector.
// Inputs:
//   rows, cols, values: coordinate lists (all same size nnz)
//   vector: dense vector of length equal to matrix dimension (rows == cols)
// Returns a new vector of length equal to the matrix dimension.
std::vector<double> sparseMatrixVectorProduct(
        const std::vector<std::size_t>& rows,
        const std::vector<std::size_t>& cols,
        const std::vector<double>& values,
        const std::vector<double>& vector) {
    // Determine matrix dimension. Since matrix is square, use rows.size() as dimension.
    // If there are no nonzero entries, we need to know the dimension.
    // The dimension is not explicitly given, so we infer it from the max row/col index +1.
    // If nnz == 0, we return an empty vector (cannot infer dimension).
    std::size_t nnz = rows.size();
    if (nnz == 0) {
        // If no nonzeros, we cannot know the dimension. Return empty vector.
        return std::vector<double>();
    }

    // Find maximum row and column index to infer dimension.
    std::size_t max_index = 0;
    for (std::size_t i = 0; i < nnz; ++i) {
        if (rows[i] > max_index) max_index = rows[i];
        if (cols[i] > max_index) max_index = cols[i];
    }
    std::size_t dim = max_index + 1;

    // Result initialized to zero.
    std::vector<double> result(dim, 0.0);

    // Accumulate contributions.
    for (std::size_t i = 0; i < nnz; ++i) {
        result[rows[i]] += values[i] * vector[cols[i]];
    }

    return result;
}

#include <cassert>
#include <vector>

// Declare the solution function (assume it is provided above).
std::vector<double> sparseMatrixVectorProduct(
        const std::vector<std::size_t>&,
        const std::vector<std::size_t>&,
        const std::vector<double>&,
        const std::vector<double>&);

int main() {
    // Test 1: simple 2x2 identity-like matrix
    std::vector<std::size_t> r1 = {0, 1};
    std::vector<std::size_t> c1 = {0, 1};
    std::vector<double> v1 = {2.0, 3.0};
    std::vector<double> vec1 = {5.0, 7.0};
    auto res1 = sparseMatrixVectorProduct(r1, c1, v1, vec1);
    assert(res1.size() == 2);
    assert(res1[0] == 10.0);
    assert(res1[1] == 21.0);

    // Test 2: duplicate coordinates (two entries at (0,0))
    std::vector<std::size_t> r2 = {0, 0};
    std::vector<std::size_t> c2 = {0, 0};
    std::vector<double> v2 = {1.0, 2.0};
    std::vector<double> vec2 = {4.0};
    auto res2 = sparseMatrixVectorProduct(r2, c2, v2, vec2);
    assert(res2.size() == 1);
    assert(res2[0] == 12.0); // (1+2)*4

    // Test 3: empty nonzero list returns empty
    std::vector<std::size_t> r3, c3;
    std::vector<double> v3;
    std::vector<double> vec3 = {1.0, 2.0};
    auto res3 = sparseMatrixVectorProduct(r3, c3, v3, vec3);
    assert(res3.empty());

    // Test 4: 3x3 matrix with a zero row (row 1 has no entries)
    std::vector<std::size_t> r4 = {0, 2};
    std::vector<std::size_t> c4 = {1, 2};
    std::vector<double> v4 = {1.5, -2.0};
    std::vector<double> vec4 = {10.0, 20.0, 30.0};
    auto res4 = sparseMatrixVectorProduct(r4, c4, v4, vec4);
    assert(res4.size() == 3);
    assert(res4[0] == 30.0); // row 0 col 1: 1.5 * 20 = 30
    assert(res4[1] == 0.0);  // row 1 has no entries
    assert(res4[2] == -60.0); // row 2 col 2: -2 * 30 = -60

    // Test 5: matrix with non-contiguous indices (e.g., only index 5)
    std::vector<std::size_t> r5 = {5};
    std::vector<std::size_t> c5 = {5};
    std::vector<double> v5 = {2.0};
    std::vector<double> vec5(6, 1.0); // size 6, all ones
    auto res5 = sparseMatrixVectorProduct(r5, c5, v5, vec5);
    assert(res5.size() == 6);
    assert(res5[5] == 2.0);
    for (int i = 0; i < 5; ++i) assert(res5[i] == 0.0);

    return 0;
}
