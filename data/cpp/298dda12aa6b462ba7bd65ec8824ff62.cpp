// Write a C++ function `applySubrectangleUpdates` that accepts a 2D vector of integers (the rectangle) followed by a vector of update operations. Each update operation is a tuple `{row1, col1, row2, col2, newValue}` that sets all cells in the inclusive subrectangle from `(row1,col1)` to `(row2,col2)` to `newValue`. After applying all updates sequentially, the function should also accept a query operation `{row, col}` (encoded as a tuple with only two values, or as a separate vector of query pairs) and return the final integer at each queried position. For simplicity, the function signature should be: `std::vector<int> applySubrectangleUpdates(const std::vector<std::vector<int>>& rectangle, const std::vector<std::vector<int>>& operations, const std::vector<std::pair<int,int>>& queries);` where each operation in `operations` is either `{5, row1, col1, row2, col2, newValue}` (6 elements, update) or `{1, row, col, 0, 0, 0}` (6 elements, query, but queries are passed separately for clarity). Assume all rows have equal length, all indices are valid, and at least one query exists. The function must not modify the original rectangle (pass by const reference) and must return the results in the order of the queries.
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: Basic update and query.
    std::vector<std::vector<int>> rect1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> ops1 = {{5, 0, 0, 0, 1, 9}}; // update top row to 9
    std::vector<std::pair<int,int>> queries1 = {{1, 0}, {0, 0}};
    std::vector<int> result1 = applySubrectangleUpdates(rect1, ops1, queries1);
    assert((result1 == std::vector<int>{3, 9}));

    // Test 2: Two updates, second overrides part of first.
    std::vector<std::vector<int>> rect2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<std::vector<int>> ops2 = {
        {5, 0, 0, 2, 2, 0}, // entire matrix set to 0
        {5, 1, 1, 1, 1, 7}  // center cell set to 7
    };
    std::vector<std::pair<int,int>> queries2 = {{0,0}, {1,1}, {2,2}};
    std::vector<int> result2 = applySubrectangleUpdates(rect2, ops2, queries2);
    assert((result2 == std::vector<int>{0, 7, 0}));

    // Test 3: No updates, only queries.
    std::vector<std::vector<int>> rect3 = {{10, 20}, {30, 40}};
    std::vector<std::vector<int>> ops3;
    std::vector<std::pair<int,int>> queries3 = {{0,0}, {1,1}};
    std::vector<int> result3 = applySubrectangleUpdates(rect3, ops3, queries3);
    assert((result3 == std::vector<int>{10, 40}));

    // Test 4: Overlapping updates, and ensure original rectangle unchanged.
    std::vector<std::vector<int>> rect4 = {{1, 1, 1}, {1, 1, 1}};
    std::vector<std::vector<int>> ops4 = {
        {5, 0, 0, 0, 1, 5},
        {5, 0, 1, 1, 2, 8}
    };
    std::vector<std::pair<int,int>> queries4 = {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}};
    std::vector<int> result4 = applySubrectangleUpdates(rect4, ops4, queries4);
    assert((result4 == std::vector<int>{5, 8, 8, 1, 8, 8}));
    // Original rectangle must remain unchanged.
    assert((rect4 == std::vector<std::vector<int>>{{1,1,1},{1,1,1}}));

    // Test 5: All queries at same position after a full update.
    std::vector<std::vector<int>> rect5 = {{0, 0}, {0, 0}};
    std::vector<std::vector<int>> ops5 = {{5, 0, 0, 1, 1, 42}};
    std::vector<std::pair<int,int>> queries5 = {{1,1}, {1,1}, {1,1}};
    std::vector<int> result5 = applySubrectangleUpdates(rect5, ops5, queries5);
    assert((result5 == std::vector<int>{42, 42, 42}));

    return 0;
}
#include <vector>
#include <utility>

// Apply a sequence of subrectangle updates to a copy of the given rectangle,
// then evaluate the values at the given query positions.
std::vector<int> applySubrectangleUpdates(
    const std::vector<std::vector<int>>& rectangle,
    const std::vector<std::vector<int>>& operations,
    const std::vector<std::pair<int,int>>& queries) {
    
    // Make a mutable copy of the rectangle.
    std::vector<std::vector<int>> m = rectangle;
    
    // Process each operation.
    // Each operation is a vector<int> with 6 elements:
    // {type, row1, col1, row2, col2, newValue}
    // type == 5 means update, type == 1 means query (but queries are also passed separately).
    for (const auto& op : operations) {
        int type = op[0];
        if (type == 5) {
            int row1 = op[1];
            int col1 = op[2];
            int row2 = op[3];
            int col2 = op[4];
            int newValue = op[5];
            for (int i = row1; i <= row2; ++i) {
                for (int j = col1; j <= col2; ++j) {
                    m[i][j] = newValue;
                }
            }
        }
        // type == 1 is a query but ignored because queries are passed separately.
    }
    
    // Collect results for each query.
    std::vector<int> results;
    results.reserve(queries.size());
    for (const auto& q : queries) {
        results.push_back(m[q.first][q.second]);
    }
    return results;
}
// The solution mirrors the original `SubrectangleQueries` class behavior. We start by copying the input rectangle into a mutable internal 2D vector (since we must not modify the caller's data). Then we iterate through each operation in the operations list. For each update operation (identified by the type code, e.g., 5), we perform a nested loop over rows from `row1` to `row2` and columns from `col1` to `col2`, assigning `newValue` to each cell. This is a straightforward O(K) per update where K is the area of the subrectangle. For query operations, we simply read from the internal matrix at the given row and column. After processing all operations, we loop over the separate `queries` vector and push back the current value at each position. Important edge cases include: updates that fully overlap previously updated regions (later updates override earlier ones), zero-area updates (when row1 > row2 or col1 > col2, but the problem guarantees valid inclusive ranges, so we don't need to handle them), and ensure the internal copy is independent so that modifying it does not affect the original. Time complexity is O(U * A + Q) where U is the number of updates, A is the average area of an update, and Q is the number of queries. Space complexity is O(R*C) for the internal copy plus O(Q) for the result vector.
