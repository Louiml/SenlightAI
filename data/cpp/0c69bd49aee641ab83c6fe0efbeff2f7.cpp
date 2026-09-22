/*
Write a C++ function that takes a rectangular matrix `b` of size `m × n` (where `1 ≤ m, n ≤ 1000` and each entry is either 0 or 1) and returns a pair consisting of: (1) a boolean indicating whether there exists a binary matrix `a` of the same size such that for each cell `(i, j)`, the value `b[i][j]` equals the logical OR of all entries in row `i` of `a` and all entries in column `j` of `a`; and (2) if such a matrix exists, the constructed matrix `a` (with all entries filled). If no such matrix exists, return an empty matrix. The matrix `a` must satisfy the property that if `b[i][j] == 0`, then every element in row `i` and every element in column `j` of `a` must be 0; otherwise, `a` can be arbitrary as long as the OR condition holds. Return the result as a `std::pair<bool, std::vector<std::vector<int>>>`.
*/
#include <vector>
#include <utility>

// Given a binary matrix b, return whether a matrix a exists satisfying the OR condition,
// and if so, the constructed matrix a. If not, return an empty a.
std::pair<bool, std::vector<std::vector<int>>> findMatrix(const std::vector<std::vector<int>>& b) {
    const int m = static_cast<int>(b.size());
    const int n = static_cast<int>(b[0].size());
    
    // Initialize a with all 1s.
    std::vector<std::vector<int>> a(m, std::vector<int>(n, 1));
    
    // For each zero in b, clear the corresponding row and column.
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (b[i][j] == 0) {
                for (int k = 0; k < n; ++k) a[i][k] = 0;
                for (int k = 0; k < m; ++k) a[k][j] = 0;
            }
        }
    }
    
    // Precompute row ORs and column ORs of a.
    std::vector<int> rowOR(m, 0);
    std::vector<int> colOR(n, 0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            rowOR[i] |= a[i][j];
            colOR[j] |= a[i][j];
        }
    }
    
    // Verify every cell.
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int val = rowOR[i] | colOR[j];
            if (val != b[i][j]) {
                return {false, {}};
            }
        }
    }
    
    return {true, a};
}
#include <cassert>
#include <vector>

// The solution function is defined above; here is the test main.
int main() {
    // Example 1: All zeros.
    std::vector<std::vector<int>> b1 = {{0, 0}, {0, 0}};
    auto r1 = findMatrix(b1);
    assert(r1.first == true);
    assert(r1.second == std::vector<std::vector<int>>{{0, 0}, {0, 0}});

    // Example 2: All ones.
    std::vector<std::vector<int>> b2 = {{1, 1}, {1, 1}};
    auto r2 = findMatrix(b2);
    assert(r2.first == true);
    assert(r2.second == std::vector<std::vector<int>>{{1, 1}, {1, 1}});

    // Example 3: A valid pattern with a mix.
    std::vector<std::vector<int>> b3 = {{1, 0}, {1, 0}};
    auto r3 = findMatrix(b3);
    assert(r3.first == true);
    // The constructed a must be all zeros because the second column is all zeros.
    assert(r3.second == std::vector<std::vector<int>>{{0, 0}, {0, 0}});

    // Example 4: Impossible pattern.
    std::vector<std::vector<int>> b4 = {{1, 1}, {0, 1}};
    auto r4 = findMatrix(b4);
    assert(r4.first == false);
    assert(r4.second.empty());

    // Example 5: Single element 1.
    std::vector<std::vector<int>> b5 = {{1}};
    auto r5 = findMatrix(b5);
    assert(r5.first == true);
    assert(r5.second == std::vector<std::vector<int>>{{1}});

    // Example 6: 1x3 with a pattern that forces row zero.
    std::vector<std::vector<int>> b6 = {{0, 1, 0}};
    auto r6 = findMatrix(b6);
    // Row must be all zeros because of the zeros at col 0 and col 2.
    assert(r6.first == true);
    assert(r6.second == std::vector<std::vector<int>>{{0, 0, 0}});

    // Example 7: 3x1 with all ones.
    std::vector<std::vector<int>> b7 = {{1}, {1}, {1}};
    auto r7 = findMatrix(b7);
    assert(r7.first == true);
    assert(r7.second == std::vector<std::vector<int>>{{1}, {1}, {1}});

    // Example 8: A larger valid case.
    std::vector<std::vector<int>> b8 = {{0, 1}, {1, 1}};
    auto r8 = findMatrix(b8);
    // Must be all zeros due to the first cell, causing a mismatch, so impossible.
    assert(r8.first == false);
    assert(r8.second.empty());
}
// The key observation is that if `b[i][j] == 0`, then the OR condition forces all entries in row `i` and column `j` of `a` to be 0. Therefore, we can initialize `a` with all 1s, and then for every zero in `b`, set the entire corresponding row and column of `a` to 0. After this construction, we must verify that the OR condition holds for every cell: for each `(i, j)`, compute the OR of row `i` of `a` and column `j` of `a` (which can be done by precomputing row ORs and column ORs), and check it matches `b[i][j]`. If any mismatch occurs, return false; otherwise, return true with the constructed `a`. 
//
// Edge cases: if `b` is all zeros, `a` becomes all zeros, and the OR of any row/column is 0, which matches, so it's valid. If `b` is all ones, `a` remains all ones, and the OR is 1, which matches. If `b` has a pattern like `b[0][0]=1` and `b[0][1]=0`, then row 0 of `a` becomes all 0 due to the second column, so the OR of row 0 is 0, but `b[0][0]=1`, causing a mismatch — correct, impossible. The time complexity is O(m*n + m*n) for constructing and verifying, plus O(m*n) for precomputing row/column ORs, so overall O(m*n) time and O(m*n) space for storing `a` and the OR arrays. Verification uses O(m + n) extra space.
