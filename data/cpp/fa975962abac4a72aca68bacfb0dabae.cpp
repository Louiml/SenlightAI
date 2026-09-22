// Write a C++ function named `generatePascalTriangle` that takes a non-negative integer `numRows` and returns a vector of vectors of integers representing the first `numRows` rows of Pascal's triangle. Each row must be built using the rule that the first and last elements are `1`, and every interior element is the sum of the two elements directly above it from the previous row. The returned structure should be a `std::vector<std::vector<int>>` where the outer vector has exactly `numRows` inner vectors, and the `i`-th inner vector (0-indexed) has size `i+1`. If `numRows` is `0`, return an empty outer vector.
// The algorithm constructs the triangle row by row. For each row index `i` (starting from 0), we allocate a vector of length `i+1`. The first and last elements are set to `1`. For any interior position `j` (where `0 < j < i`), the value is obtained by summing the two elements from the previous row at indices `j-1` and `j` (i.e., `previousRow[j-1] + previousRow[j]`). The base case is `numRows = 0`, which returns an empty vector; `numRows = 1` returns `[[1]]`. Edge cases include very small inputs (0 or 1) and rows with only one element where the loop over `j` naturally sets the only element to `1` because both `j==0` and `j==x-1` hold. Time complexity is \(O(numRows^2)\) because the total number of elements across all rows is \(1+2+\cdots+numRows = numRows(numRows+1)/2\). Space complexity is \(O(numRows^2)\) for storing all rows, not counting the output itself. No extra significant auxiliary space is needed beyond the result container and temporary row vectors.
#include <vector>

// Generate the first numRows rows of Pascal's triangle.
// Each row is a vector<int>; the returned value is a vector of these rows.
std::vector<std::vector<int>> generatePascalTriangle(int numRows) {
    std::vector<std::vector<int>> triangle;
    triangle.reserve(numRows); // optional, improves performance for large inputs

    for (int rowIdx = 0; rowIdx < numRows; ++rowIdx) {
        const int cols = rowIdx + 1;
        std::vector<int> currentRow(cols);

        for (int colIdx = 0; colIdx < cols; ++colIdx) {
            if (colIdx == 0 || colIdx == cols - 1) {
                currentRow[colIdx] = 1;
            } else {
                // Elements from the previous row (rowIdx-1) at colIdx-1 and colIdx
                currentRow[colIdx] = triangle[rowIdx - 1][colIdx - 1] + triangle[rowIdx - 1][colIdx];
            }
        }

        triangle.push_back(currentRow);
    }

    return triangle;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // numRows = 0 -> empty
    assert(generatePascalTriangle(0).empty());

    // numRows = 1 -> [[1]]
    {
        auto triangle = generatePascalTriangle(1);
        assert(triangle.size() == 1);
        assert(triangle[0] == std::vector<int>({1}));
    }

    // numRows = 2 -> [[1], [1,1]]
    {
        auto triangle = generatePascalTriangle(2);
        assert(triangle.size() == 2);
        assert(triangle[0] == std::vector<int>({1}));
        assert(triangle[1] == std::vector<int>({1,1}));
    }

    // numRows = 3 -> [[1], [1,1], [1,2,1]]
    {
        auto triangle = generatePascalTriangle(3);
        assert(triangle.size() == 3);
        assert(triangle[0] == std::vector<int>({1}));
        assert(triangle[1] == std::vector<int>({1,1}));
        assert(triangle[2] == std::vector<int>({1,2,1}));
    }

    // numRows = 5 -> full correct triangle
    {
        auto triangle = generatePascalTriangle(5);
        assert(triangle.size() == 5);
        assert(triangle == std::vector<std::vector<int>>({
            {1},
            {1,1},
            {1,2,1},
            {1,3,3,1},
            {1,4,6,4,1}
        }));
    }

    // numRows = 10 -> check symmetry and specific interior values
    {
        auto triangle = generatePascalTriangle(10);
        assert(triangle[4] == std::vector<int>({1,4,6,4,1}));
        assert(triangle[9][5] == 126); // 10th row (index 9), 6th element = C(9,5) = 126
        // symmetric row check
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j <= i; ++j) {
                assert(triangle[i][j] == triangle[i][i - j]);
            }
        }
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
