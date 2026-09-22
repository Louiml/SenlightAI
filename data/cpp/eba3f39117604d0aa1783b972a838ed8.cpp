Write a C++ function `std::vector<int> findSaddlePoints(const std::vector<std::vector<int>>& matrix)` that takes a square matrix (equal number of rows and columns, with at least one element) and returns a vector containing all values that are simultaneously the maximum in their row and the minimum in their column. The matrix is guaranteed to be non-empty and square; values may be negative, zero, or positive, and duplicates are possible. A value qualifies if it is greater than or equal to every other element in its row AND less than or equal to every other element in its column. If no such element exists, return an empty vector. The returned vector must preserve the order in which the qualifying values appear when scanning rows from top to bottom, and for each row, from left to right.
// The solution processes the matrix row by row. For each row `i`, first find the maximum value in that row and note the column index `pos` where this maximum occurs. If multiple columns contain the same maximum in the same row, each such column must be checked independently because a value in a different column might also be a saddle point even if the row maximum is duplicated. Therefore, for each column `j` where `matrix[i][j]` equals the row maximum, check whether that value is less than or equal to every element in column `j`. If yes, append it to the result. This ensures correctness with duplicate row maximums. Edge cases include: a 1×1 matrix (the single element is trivially a saddle point), negative values (handled by standard comparisons), and matrices with no saddle points (returns empty vector). Time complexity is O(n³) in the worst case because for each row we scan the row (O(n)) and then for each potential column we scan the column (O(n)), leading to O(n) rows × O(n) columns × O(n) column scan = O(n³). Space complexity is O(n) for the result vector, plus O(n) auxiliary space if we store row maximums, but we can avoid that by recomputing the row maximum as needed, so auxiliary space is O(1) excluding the output.
#include <vector>

// Find all saddle points: values that are the max in their row and min in their column.
std::vector<int> findSaddlePoints(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> result;
    const int n = matrix.size();
    if (n == 0) return result;

    for (int i = 0; i < n; ++i) {
        // Find the maximum value in row i
        int rowMax = matrix[i][0];
        for (int j = 1; j < n; ++j) {
            if (matrix[i][j] > rowMax) {
                rowMax = matrix[i][j];
            }
        }

        // Check every column where the row maximum occurs
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] == rowMax) {
                // Verify it is the minimum in column j
                bool isMinInColumn = true;
                for (int k = 0; k < n; ++k) {
                    if (matrix[k][j] < matrix[i][j]) {
                        isMinInColumn = false;
                        break;
                    }
                }
                if (isMinInColumn) {
                    result.push_back(matrix[i][j]);
                }
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Solution function from above
std::vector<int> findSaddlePoints(const std::vector<std::vector<int>>&);

int main() {
    // 1x1 matrix
    std::vector<std::vector<int>> m1 = {{5}};
    assert(findSaddlePoints(m1) == std::vector<int>({5}));

    // Basic 3x3 with one saddle point at (0,2): value 7
    std::vector<std::vector<int>> m2 = {
        {1, 2, 7},
        {3, 4, 6},
        {5, 6, 7}
    };
    assert(findSaddlePoints(m2) == std::vector<int>({7}));

    // No saddle points
    std::vector<std::vector<int>> m3 = {
        {1, 2},
        {3, 4}
    };
    assert(findSaddlePoints(m3).empty());

    // Multiple saddle points (duplicate row max in same row)
    std::vector<std::vector<int>> m4 = {
        {5, 5, 1},
        {2, 3, 4},
        {6, 7, 8}
    };
    // Row 0 has maxima 5 at col0 and col1; both are min in their respective columns (col0 min=2, col1 min=3? No, col1 has 3, so 5 is not min. So only check: col0 has [5,2,6], min=2, so 5 not min. col1 has [5,3,7], min=3, so 5 not min. No saddle points. Actually correct.
    assert(findSaddlePoints(m4).empty());

    // Duplicate row max with saddle at both columns
    std::vector<std::vector<int>> m5 = {
        {4, 4, 0},
        {4, 1, 2},
        {5, 3, 3}
    };
    // Row0: max=4 at col0 and col1. col0=[4,4,5] min=4 -> qualifies. col1=[4,1,3] min=1 -> not. Row1: max=4 at col0, col0 min=4 -> qualifies (4 again). Row2: max=5 at col0, col0 min=4, so no. So result: [4,4]
    assert(findSaddlePoints(m5) == std::vector<int>({4, 4}));

    // Negative numbers
    std::vector<std::vector<int>> m6 = {
        {-1, -2},
        {-3, -4}
    };
    // Row0 max=-1 at col0, col0=[-1,-3] min=-3, so -1 not. Row1 max=-3 at col0, col0 min=-3 -> qualifies. Result: [-3]
    assert(findSaddlePoints(m6) == std::vector<int>({-3}));

    // All elements the same
    std::vector<std::vector<int>> m7 = {
        {2, 2},
        {2, 2}
    };
    // Every element qualifies, in row-major order: [2,2,2,2]
    assert(findSaddlePoints(m7) == std::vector<int>({2, 2, 2, 2}));

    // Larger 4x4 with saddle at center
    std::vector<std::vector<int>> m8 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    // Row0 max=4 at col3, col3=[4,8,12,16], min=4 -> qualifies. Row1 max=8 at col3, min=4 -> no. Row2 max=12 at col3, min=4 -> no. Row3 max=16 at col3, min=4 -> no. So result: [4]
    assert(findSaddlePoints(m8) == std::vector<int>({4}));

    return 0;
}
