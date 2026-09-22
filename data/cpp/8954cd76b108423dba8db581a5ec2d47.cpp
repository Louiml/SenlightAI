Write a C++ function that takes a 2D integer matrix of dimensions `rows × cols` and returns a new matrix (as a 2D `std::vector<std::vector<int>>`) that is the transpose of the input, but with each element negated (i.e., the value at `out[col][row]` must equal `-input[row][col]`). Assume the input is non-empty and both `rows` and `cols` are at least 1. The function must not modify the input matrix, must preserve the order of the original dimensions (i.e., if input has `rows=3, cols=4`, output must have `rows=4, cols=3`), and must work correctly for any positive dimension sizes, including non-square matrices. Use only standard C++ (no OpenMP or parallel extensions) and handle edge cases like single-row or single-column matrices.

The solution requires iterating over every element of the input matrix exactly once. For each cell at `(i, j)`, read the value `v = input[i][j]`, negate it, and write `-v` to the output at position `(j, i)`. Since we need a new matrix, allocate an output vector with `cols` rows and `rows` columns, initialized to zeros. Then use nested loops: outer loop over `i` (rows of input), inner loop over `j` (cols of input), and assign `output[j][i] = -input[i][j]`. This is correct because the transpose swaps indices, and negation is applied per element. No special edge cases beyond ensuring dimensions are respected; for a 1×N input, output will be N×1 and vice versa. Time complexity is O(rows × cols) since every element is processed once. Space complexity is O(rows × cols) for the output matrix, plus O(1) auxiliary for loop counters and temporary variables.

#include <vector>
#include <cstddef>

// Transpose and negate a 2D matrix.
// input: rows x cols matrix, output: cols x rows matrix with out[j][i] = -input[i][j]
std::vector<std::vector<int>> transposeAndNegate(const std::vector<std::vector<int>>& input) {
    const std::size_t rows = input.size();
    const std::size_t cols = input[0].size();
    
    // Initialize output with 'rows' columns and 'cols' rows, all zeros.
    std::vector<std::vector<int>> output(cols, std::vector<int>(rows, 0));
    
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            output[j][i] = -input[i][j];
        }
    }
    
    return output;
}

#include <cassert>
#include <vector>

// Include the solution function here or via header.

int main() {
    // 2x3 matrix
    std::vector<std::vector<int>> mat1 = {{1, 2, 3}, {4, 5, 6}};
    auto res1 = transposeAndNegate(mat1);
    assert(res1.size() == 3);
    assert(res1[0].size() == 2);
    assert(res1[0][0] == -1 && res1[0][1] == -4);
    assert(res1[1][0] == -2 && res1[1][1] == -5);
    assert(res1[2][0] == -3 && res1[2][1] == -6);

    // 1x4 matrix
    std::vector<std::vector<int>> mat2 = {{7, -8, 0, 3}};
    auto res2 = transposeAndNegate(mat2);
    assert(res2.size() == 4);
    assert(res2[0].size() == 1);
    assert(res2[0][0] == -7);
    assert(res2[1][0] == 8);
    assert(res2[2][0] == 0);
    assert(res2[3][0] == -3);

    // 3x1 matrix
    std::vector<std::vector<int>> mat3 = {{-1}, {2}, {-3}};
    auto res3 = transposeAndNegate(mat3);
    assert(res3.size() == 1);
    assert(res3[0].size() == 3);
    assert(res3[0][0] == 1 && res3[0][1] == -2 && res3[0][2] == 3);

    // 1x1 matrix
    std::vector<std::vector<int>> mat4 = {{42}};
    auto res4 = transposeAndNegate(mat4);
    assert(res4.size() == 1 && res4[0].size() == 1);
    assert(res4[0][0] == -42);

    // Non-square wider than tall
    std::vector<std::vector<int>> mat5 = {{1, 2}, {3, 4}, {5, 6}};
    auto res5 = transposeAndNegate(mat5);
    assert(res5.size() == 2 && res5[0].size() == 3);
    assert(res5[0][0] == -1 && res5[0][1] == -3 && res5[0][2] == -5);
    assert(res5[1][0] == -2 && res5[1][1] == -4 && res5[1][2] == -6);

    return 0;
}
