Write a standalone C++ function that takes a square matrix (represented as a 2D `std::vector<std::vector<double>>`) and returns a new matrix where each column is filled with values `3*i + j` (where `i` is the row index and `j` is the column index), following the exact pattern demonstrated in the given Boost uBLAS code snippet. The function must work for any square matrix size (including 1×1), must not modify the input matrix, and must return the result by value. The function should be named `fillColumnsFromPattern` and take a const reference to the input matrix, returning a new matrix of the same dimensions. Handle the edge case of an empty matrix gracefully by returning an empty matrix. Ensure that the result uses `double` precision and that row/column indices start at 0.

// The solution is straightforward: create a result matrix with the same dimensions as the input matrix. Iterate over each column index `j` from 0 to size-1, and for each row index `i` from 0 to size-1, set `result[i][j] = 3.0 * i + j`. The input matrix is only used to determine the dimensions, so we ignore its contents entirely. The main edge cases are: (1) an empty input matrix (size 0) — return an empty result; (2) a 1×1 matrix — result contains a single element `3*0+0 = 0`; (3) non-square input — although the task says square, we can still handle rectangular matrices by using `input.size()` for rows and `input[0].size()` for columns if non-empty, to be safe. Time complexity is O(n²) for an n×n matrix, and space complexity is O(n²) for the result (plus O(1) auxiliary). Implementation uses standard `std::vector` and `size_t` for indices to avoid signed/unsigned warnings.

#include <vector>
#include <cstddef>

// Fills each column j of the returned matrix with values 3*i + j,
// where i is the row index and j is the column index.
// The input matrix is read-only and only used to determine dimensions.
std::vector<std::vector<double>> fillColumnsFromPattern(
    const std::vector<std::vector<double>>& input) {
    // Handle empty matrix (no rows or no columns)
    if (input.empty() || input[0].empty()) {
        return {};
    }

    const std::size_t rows = input.size();
    const std::size_t cols = input[0].size();

    // Initialize result with zeros (or resize directly)
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));

    // Fill each cell according to the pattern
    for (std::size_t j = 0; j < cols; ++j) {
        for (std::size_t i = 0; i < rows; ++i) {
            result[i][j] = 3.0 * static_cast<double>(i) + static_cast<double>(j);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Include the solution function (or paste it here)

int main() {
    // Test 1: 3x3 matrix matching the original snippet pattern
    std::vector<std::vector<double>> input3(3, std::vector<double>(3, 0.0));
    auto result3 = fillColumnsFromPattern(input3);
    assert(result3.size() == 3);
    assert(result3[0].size() == 3);
    // Expected: column 0 = [0,3,6]; column 1 = [1,4,7]; column 2 = [2,5,8]
    assert(result3[0][0] == 0.0 && result3[0][1] == 1.0 && result3[0][2] == 2.0);
    assert(result3[1][0] == 3.0 && result3[1][1] == 4.0 && result3[1][2] == 5.0);
    assert(result3[2][0] == 6.0 && result3[2][1] == 7.0 && result3[2][2] == 8.0);

    // Test 2: 1x1 matrix
    std::vector<std::vector<double>> input1(1, std::vector<double>(1, 42.0));
    auto result1 = fillColumnsFromPattern(input1);
    assert(result1.size() == 1);
    assert(result1[0].size() == 1);
    assert(result1[0][0] == 0.0);

    // Test 3: Empty matrix (no rows)
    std::vector<std::vector<double>> inputEmpty;
    auto resultEmpty = fillColumnsFromPattern(inputEmpty);
    assert(resultEmpty.empty());

    // Test 4: Empty matrix (rows but no columns)
    std::vector<std::vector<double>> inputNoCols(2, std::vector<double>(0));
    auto resultNoCols = fillColumnsFromPattern(inputNoCols);
    assert(resultNoCols.empty());

    // Test 5: Rectangular matrix 2x3 (though task says square, function handles it)
    std::vector<std::vector<double>> inputRect(2, std::vector<double>(3, 5.0));
    auto resultRect = fillColumnsFromPattern(inputRect);
    assert(resultRect.size() == 2);
    assert(resultRect[0].size() == 3);
    assert(resultRect[0][0] == 0.0 && resultRect[0][1] == 1.0 && resultRect[0][2] == 2.0);
    assert(resultRect[1][0] == 3.0 && resultRect[1][1] == 4.0 && resultRect[1][2] == 5.0);

    return 0;
}
