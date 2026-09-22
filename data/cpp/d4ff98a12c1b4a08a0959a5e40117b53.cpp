/*
Write a C++ function `rangeSumWithUpdates` that, given a 2D vector of integers (non-empty, at least 1 row and 1 column), and a sequence of operations (either `"update"` with row, col, newVal, or `"query"` with row1, col1, row2, col2), processes all operations in order and returns a vector of integers containing the results of all query operations. The function signature is: `std::vector<int> rangeSumWithUpdates(std::vector<std::vector<int>> matrix, const std::vector<std::string>& ops)`, where each string in `ops` is formatted as `"update r c v"` or `"query r1 c1 r2 c2"` (all 0-indexed). Guarantee that `0 <= r < rows`, `0 <= c < cols`, `0 <= r1 <= r2 < rows`, `0 <= c1 <= c2 < cols`, and new values are integers. The matrix is mutable only via update operations, and updates/query calls can be intermixed arbitrarily.
*/

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

// Process update and query operations on a mutable 2D matrix.
// Returns the results of all "query" operations in order.
std::vector<int> rangeSumWithUpdates(std::vector<std::vector<int>> matrix, const std::vector<std::string>& ops) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    
    // Column prefix sums: colSum[i][j] = sum of matrix[0..i][j]
    std::vector<std::vector<int>> colSum(rows, std::vector<int>(cols, 0));
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            colSum[i][j] = (i == 0) ? matrix[i][j] : colSum[i-1][j] + matrix[i][j];
        }
    }
    
    std::vector<int> results;
    for (const auto& op : ops) {
        std::istringstream iss(op);
        std::string type;
        iss >> type;
        if (type == "update") {
            int row, col, newVal;
            iss >> row >> col >> newVal;
            int diff = newVal - matrix[row][col];
            for (int i = row; i < rows; ++i) {
                colSum[i][col] += diff;
            }
            matrix[row][col] = newVal;
        } else if (type == "query") {
            int r1, c1, r2, c2;
            iss >> r1 >> c1 >> r2 >> c2;
            int total = 0;
            for (int c = c1; c <= c2; ++c) {
                total += (r1 == 0) ? colSum[r2][c] : colSum[r2][c] - colSum[r1 - 1][c];
            }
            results.push_back(total);
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

// Function declaration (declared above in solution)
std::vector<int> rangeSumWithUpdates(std::vector<std::vector<int>> matrix, const std::vector<std::string>& ops);

int main() {
    // Example from problem statement
    std::vector<std::vector<int>> m1 = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}
    };
    std::vector<std::string> ops1 = {
        "query 2 1 4 3",  // sum = 8
        "update 3 2 2",   // change [3][2] from 0 to 2
        "query 2 1 4 3"   // sum = 10
    };
    std::vector<int> res1 = rangeSumWithUpdates(m1, ops1);
    assert(res1.size() == 2);
    assert(res1[0] == 8);
    assert(res1[1] == 10);

    // Single-cell matrix
    std::vector<std::vector<int>> m2 = {{5}};
    std::vector<std::string> ops2 = {"query 0 0 0 0", "update 0 0 7", "query 0 0 0 0"};
    std::vector<int> res2 = rangeSumWithUpdates(m2, ops2);
    assert(res2.size() == 2);
    assert(res2[0] == 5);
    assert(res2[1] == 7);

    // All updates, no queries
    std::vector<std::vector<int>> m3 = {{1, 2}, {3, 4}};
    std::vector<std::string> ops3 = {"update 0 0 10", "update 1 1 -1"};
    std::vector<int> res3 = rangeSumWithUpdates(m3, ops3);
    assert(res3.empty());

    // Query whole matrix before and after multiple updates
    std::vector<std::vector<int>> m4 = {{1, 2}, {3, 4}};
    std::vector<std::string> ops4 = {
        "query 0 0 1 1",  // 1+2+3+4=10
        "update 0 1 5",   // becomes 1+5+3+4=13
        "update 1 0 0",   // becomes 1+5+0+4=10
        "query 0 0 1 1"   // 10
    };
    std::vector<int> res4 = rangeSumWithUpdates(m4, ops4);
    assert(res4.size() == 2);
    assert(res4[0] == 10);
    assert(res4[1] == 10);

    // Partial rectangle query after update
    std::vector<std::vector<int>> m5 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<std::string> ops5 = {
        "update 1 1 10",   // now row1 col1 =10
        "query 0 0 2 2",   // sum =1+2+3+4+10+6+7+8+9=50
        "query 1 1 2 2"    // sum=10+6+8+9=33
    };
    std::vector<int> res5 = rangeSumWithUpdates(m5, ops5);
    assert(res5.size() == 2);
    assert(res5[0] == 50);
    assert(res5[1] == 33);

    return 0;
}

// The core challenge is to support both point updates and rectangle sum queries efficiently. A naive approach recomputing the sum each query would be O(N) per query, but with many operations this becomes slow. A better approach uses a column-prefix-sum technique: maintain an auxiliary 2D array `colSum` where `colSum[i][j]` stores the sum of elements in column `j` from row 0 to row `i` inclusive. Initially, build `colSum` in O(rows*cols). For an update at `(row, col)` by changing the value by `diff`, update every entry in `colSum` from `row` to `rows-1` in that column, which takes O(rows) per update. For a query rectangle `(r1,c1)` to `(r2,c2)`, sum over columns from `c1` to `c2` the difference between `colSum[r2][col]` and (if `r1>0`) `colSum[r1-1][col]`, giving O(cols) per query. Since the number of rows and columns are typically modest, this is efficient. Edge cases: the matrix may be rectangular (not square), ensure bounds are correct, and handle the case where `r1 == 0` by not subtracting a `-1` row. Time complexity: initialization O(rows*cols), each update O(rows), each query O(cols). Space complexity: O(rows*cols) for `colSum` and the copy of the matrix.
