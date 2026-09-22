Write a C++ function that takes a non-negative integer `numRows` and returns a `std::vector<std::vector<int>>` representing the first `numRows` rows of Pascal's triangle. Each row must be a list of integers, where the first and last element are always 1, and each interior element is the sum of the two elements directly above it from the previous row. Rows must be ordered from the top (row 0, containing just `[1]`) to the bottom (row `numRows-1`). For `numRows == 0`, return an empty vector. For `numRows == 1`, return `{{1}}`. You may assume `numRows` fits within a 32-bit signed integer, but all individual values will fit within a 32-bit signed integer.
#include <cassert>
#include <vector>

// The solution function is declared above (or included via header).

int main() {
    // numRows = 0
    assert(generatePascalsTriangle(0) == std::vector<std::vector<int>>{});
    
    // numRows = 1
    assert(generatePascalsTriangle(1) == std::vector<std::vector<int>>{{1}});
    
    // numRows = 2
    assert(generatePascalsTriangle(2) == std::vector<std::vector<int>>{{1}, {1, 1}});
    
    // numRows = 3
    assert(generatePascalsTriangle(3) == std::vector<std::vector<int>>{{1}, {1, 1}, {1, 2, 1}});
    
    // numRows = 4
    assert(generatePascalsTriangle(4) == std::vector<std::vector<int>>{{1}, {1, 1}, {1, 2, 1}, {1, 3, 3, 1}});
    
    // numRows = 5 (classic example)
    assert(generatePascalsTriangle(5) == std::vector<std::vector<int>>{
        {1},
        {1, 1},
        {1, 2, 1},
        {1, 3, 3, 1},
        {1, 4, 6, 4, 1}
    });
    
    // numRows = 6 (larger triangle, verify a few values)
    auto tri6 = generatePascalsTriangle(6);
    assert(tri6.size() == 6);
    assert(tri6[5] == std::vector<int>({1, 5, 10, 10, 5, 1}));
    
    return 0;
}
#include <vector>

// Generate the first numRows rows of Pascal's triangle.
// Returns a vector of vectors, where each inner vector represents one row.
std::vector<std::vector<int>> generatePascalsTriangle(int numRows) {
    if (numRows == 0) {
        return {};
    }
    
    std::vector<std::vector<int>> triangle;
    
    for (int currentRow = 0; currentRow < numRows; ++currentRow) {
        // Initialize row with all 1s; size is currentRow + 1.
        std::vector<int> row(currentRow + 1, 1);
        
        // Fill interior elements using values from the previous row.
        for (int col = 1; col < currentRow; ++col) {
            row[col] = triangle[currentRow - 1][col - 1] + triangle[currentRow - 1][col];
        }
        
        triangle.push_back(row);
    }
    
    return triangle;
}
// The solution builds the triangle row by row. For the `i`-th row (0-indexed), the row has `i+1` elements. We start by initializing the row with all elements set to `1`, which automatically handles the first and last positions. Then, for interior positions `j` from 1 to `i-1` (inclusive), we overwrite the value with the sum of the two numbers from the previous row at indices `j-1` and `j`. The previous row is accessible via the already-built `res` vector at index `i-1`. Edge cases include `numRows == 0`, which returns an empty vector, and small rows (`1` or `2`) where the interior loop does not execute because there are no interior positions. The time complexity is \(O(\text{numRows}^2)\) because each row has up to `numRows` elements, and we process each element exactly once. The auxiliary space is \(O(1)\) excluding the output space, since we only use a single temporary row vector and the result container itself.
