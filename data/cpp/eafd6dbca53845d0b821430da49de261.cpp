// Write a C++ function that takes a non-empty 2D vector of integers (matrix) where every row has the same length, and returns the maximum number of rows that can be made identical after performing any number of column flips, where a column flip toggles every value in that column between 0 and 1. The function should be named `maxEqualRowsAfterFlips` and accept the matrix by const reference. Note that the matrix values are guaranteed to be only 0 or 1.

The key observation is that two rows can be made identical through column flips if and only if for every column, the two rows either both equal their respective first elements or both differ from them. Equivalently, two rows are "flip-equivalent" if their bitwise pattern relative to their first element is identical. For each row, we can build a canonical signature string of the same length: for each column, if the current element equals the row's first element, place a `'1'`, otherwise place a `'0'`. Two rows with the same signature can be transformed into all `1`s (or all `0`s) after flipping appropriate columns, so they become identical. Thus, count the frequency of each signature using a hash map, and the maximum frequency is the answer. Edge cases: a single row always yields 1; all rows may already be identical (signature counts may be large); rows with all zeros produce all `'1'`s signature, and rows with alternating patterns produce distinct signatures. Time complexity is O(m * n) where m is number of rows and n is number of columns, since we process each matrix element once. Space complexity is O(m * n) in the worst case for storing signatures, but typically O(m * n) characters total.

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum number of rows that can become identical after any column flips.
int maxEqualRowsAfterFlips(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return 0;
    
    const int m = static_cast<int>(matrix.size());
    const int n = static_cast<int>(matrix[0].size());
    
    std::unordered_map<std::string, int> signatureCount;
    int maxCount = 0;
    
    for (int i = 0; i < m; ++i) {
        std::string signature;
        signature.reserve(n);
        const int first = matrix[i][0];
        for (int j = 0; j < n; ++j) {
            // If current equals first, mark '1'; otherwise mark '0'.
            signature.push_back(matrix[i][j] == first ? '1' : '0');
        }
        int& count = signatureCount[signature];
        ++count;
        maxCount = std::max(maxCount, count);
    }
    
    return maxCount;
}

#include <cassert>
#include <vector>

int maxEqualRowsAfterFlips(const std::vector<std::vector<int>>& matrix);

int main() {
    // Example 1: Each row can be flipped independently, but rows 0 and 2 become identical after flipping column 1.
    assert(maxEqualRowsAfterFlips({{0,1},{1,0},{0,1}}) == 2);
    
    // Example 2: All rows already identical (all zeros) -> all 3 rows.
    assert(maxEqualRowsAfterFlips({{0,0,0},{0,0,0},{0,0,0}}) == 3);
    
    // Example 3: Single row.
    assert(maxEqualRowsAfterFlips({{1,0,1}}) == 1);
    
    // Example 4: Two rows that are complements (flip all columns) become identical.
    assert(maxEqualRowsAfterFlips({{0,1,0},{1,0,1}}) == 2);
    
    // Example 5: Rows with different relative patterns.
    assert(maxEqualRowsAfterFlips({{0,0},{0,1},{1,0},{1,1}}) == 2);
    
    // Example 6: Large matrix with all rows identical after flips.
    assert(maxEqualRowsAfterFlips({{1,1,1},{0,0,0},{1,1,1},{0,0,0}}) == 4);
    
    // Example 7: Empty matrix? Not in spec but guard returns 0.
    assert(maxEqualRowsAfterFlips({}) == 0);
    
    return 0;
}
