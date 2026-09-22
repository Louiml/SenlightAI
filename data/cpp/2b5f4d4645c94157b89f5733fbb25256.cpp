// Write a C++ function named `generatePascalTriangle` that takes a non-negative integer `numRows` as input and returns a `std::vector<std::vector<int>>` representing the first `numRows` rows of Pascal's triangle. Each row is a vector of integers where the first and last elements are always `1`, and every interior element is the sum of the two elements directly above it from the previous row. If `numRows` is `0`, return an empty vector. The function must be efficient and handle all `numRows` from `0` upward, including `1` which returns only the row `[1]`.
// The solution builds Pascal's triangle row by row. Start with an empty result vector. If `numRows` is `0`, return immediately. Otherwise, create the first row as a vector containing a single `1`, push it into the result, and if `numRows == 1`, return. For each subsequent row `i` (from `1` to `numRows-1`), create a vector `cur` of size `i+1` initialized with all ones, then for each interior position `j` (from `1` to size-2), set `cur[j] = previousRow[j-1] + previousRow[j]`. Push `cur` into the result and update the previous row reference. Edge cases: `numRows == 0` returns empty; `numRows == 1` returns just `[1]`; larger values are built iteratively. Time complexity is O(numRows²) because the total number of elements across all rows is the sum of the first `numRows` integers, which is O(numRows²). Space complexity is O(numRows²) for storing the result, plus O(numRows) for the temporary previous row.
#include <vector>

// Generate the first numRows rows of Pascal's triangle.
// Returns an empty vector if numRows == 0.
std::vector<std::vector<int>> generatePascalTriangle(int numRows) {
    std::vector<std::vector<int>> result;
    if (numRows <= 0) return result;

    // First row always [1]
    result.push_back(std::vector<int>(1, 1));
    if (numRows == 1) return result;

    for (int i = 1; i < numRows; ++i) {
        // Current row size = i+1, all initially 1
        std::vector<int> currentRow(i + 1, 1);
        const std::vector<int>& previousRow = result[i - 1];
        for (int j = 1; j < static_cast<int>(currentRow.size()) - 1; ++j) {
            currentRow[j] = previousRow[j - 1] + previousRow[j];
        }
        result.push_back(currentRow);
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
// Place this main in a separate file or after the function definition.
int main() {
    // numRows = 0 -> empty
    assert(generatePascalTriangle(0).empty());

    // numRows = 1 -> [[1]]
    {
        std::vector<std::vector<int>> expected = {{1}};
        assert(generatePascalTriangle(1) == expected);
    }

    // numRows = 2 -> [[1], [1,1]]
    {
        std::vector<std::vector<int>> expected = {{1}, {1, 1}};
        assert(generatePascalTriangle(2) == expected);
    }

    // numRows = 3 -> [[1], [1,1], [1,2,1]]
    {
        std::vector<std::vector<int>> expected = {{1}, {1, 1}, {1, 2, 1}};
        assert(generatePascalTriangle(3) == expected);
    }

    // numRows = 5 -> standard Pascal triangle
    {
        std::vector<std::vector<int>> expected = {
            {1},
            {1, 1},
            {1, 2, 1},
            {1, 3, 3, 1},
            {1, 4, 6, 4, 1}
        };
        assert(generatePascalTriangle(5) == expected);
    }

    // Check symmetry and edge values for numRows = 10 (spot check)
    {
        auto triangle = generatePascalTriangle(10);
        assert(triangle.size() == 10);
        // Each row starts and ends with 1
        for (const auto& row : triangle) {
            assert(row.front() == 1);
            assert(row.back() == 1);
        }
        // Row 5 (index 5) = [1, 5, 10, 10, 5, 1]
        assert(triangle[5] == std::vector<int>({1, 5, 10, 10, 5, 1}));
        // Row 9 (index 9) = [1, 9, 36, 84, 126, 126, 84, 36, 9, 1]
        assert(triangle[9] == std::vector<int>({1, 9, 36, 84, 126, 126, 84, 36, 9, 1}));
    }

    return 0;
}
