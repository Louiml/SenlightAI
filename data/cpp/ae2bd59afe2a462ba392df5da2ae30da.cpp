/*
You are given an \(n \times n\) grid initially filled entirely with zeros. You will perform a sequence of \(k\) operations, where each operation selects a cell at row \(r\) and column \(c\) (both 1-indexed). After each operation, the entire row \(r\) and the entire column \(c\) are set to nonzero values (i.e., all cells in that row and column become nonzero). This means a cell becomes nonzero if its row OR its column has ever been chosen in any previous operation. Write a C++ function that takes the grid size \(n\), the number of operations \(k\), and a vector of \(k\) pairs (rows and columns) as input, and returns a vector of \(k\) long long integers, where the \(i\)-th element (0-indexed) is the number of cells in the grid that are still zero after performing the first \(i+1\) operations. Note that the same row or column may be chosen multiple times; repeated choices do not add any new nonzero cells.
*/
#include <vector>
#include <unordered_set>

// Given grid size n, number of operations k, and a list of (row, column) pairs,
// return for each prefix of operations the number of cells still zero.
std::vector<long long> countZeroCells(long long n, int k, const std::vector<std::pair<int,int>>& operations) {
    std::vector<long long> result;
    result.reserve(k);
    std::unordered_set<int> activatedRows;
    std::unordered_set<int> activatedCols;
    long long rowCount = 0;
    long long colCount = 0;

    for (int i = 0; i < k; ++i) {
        int r = operations[i].first;
        int c = operations[i].second;
        if (activatedRows.find(r) == activatedRows.end()) {
            activatedRows.insert(r);
            ++rowCount;
        }
        if (activatedCols.find(c) == activatedCols.end()) {
            activatedCols.insert(c);
            ++colCount;
        }
        long long zeroCells = n * n - rowCount * n - colCount * n + rowCount * colCount;
        result.push_back(zeroCells);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// The function declaration from the solution
std::vector<long long> countZeroCells(long long n, int k, const std::vector<std::pair<int,int>>& operations);

int main() {
    // Test 1: basic 3x3 grid, operations on different rows/columns
    {
        long long n = 3;
        int k = 2;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,2}};
        std::vector<long long> expected = {4, 1};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 2: repeated operations on same cell or same row
    {
        long long n = 5;
        int k = 3;
        std::vector<std::pair<int,int>> ops = {{2,3}, {2,3}, {2,4}};
        std::vector<long long> expected = {16, 16, 12};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 3: operations that cover all rows and columns eventually
    {
        long long n = 2;
        int k = 4;
        std::vector<std::pair<int,int>> ops = {{1,1}, {2,2}, {1,2}, {2,1}};
        std::vector<long long> expected = {1, 0, 0, 0};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 4: large grid, only one operation
    {
        long long n = 1000000;
        int k = 1;
        std::vector<std::pair<int,int>> ops = {{1000000, 1000000}};
        // After one operation, one row and one column are nonzero => (n-1)^2 zero cells
        long long expectedVal = (n-1) * (n-1);
        std::vector<long long> expected = {expectedVal};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 5: same row selected multiple times, different columns
    {
        long long n = 4;
        int k = 3;
        std::vector<std::pair<int,int>> ops = {{1,1}, {1,2}, {1,3}};
        // After first: rows=1, cols=1 => 16-4-4+1=9
        // After second: rows=1, cols=2 => 16-4-8+2=6
        // After third: rows=1, cols=3 => 16-4-12+3=3
        std::vector<long long> expected = {9, 6, 3};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 6: edge case where n=1
    {
        long long n = 1;
        int k = 1;
        std::vector<std::pair<int,int>> ops = {{1,1}};
        std::vector<long long> expected = {0};
        assert(countZeroCells(n, k, ops) == expected);
    }

    // Test 7: operations with zero-length (k=0) - return empty vector
    {
        long long n = 10;
        int k = 0;
        std::vector<std::pair<int,int>> ops = {};
        assert(countZeroCells(n, k, ops).empty());
    }

    return 0;
}
// The key insight is to track which rows and which columns have been "activated" so far. After processing a prefix of operations, let \(r\_count\) be the number of distinct rows activated, and \(c\_count\) be the number of distinct columns activated. The total number of cells that have become nonzero is the union of the activated rows and columns: each activated row contributes \(n\) cells, each activated column contributes \(n\) cells, but cells at the intersection of activated rows and columns are counted twice. Thus the number of nonzero cells is \(r\_count \cdot n + c\_count \cdot n - r\_count \cdot c\_count\). Therefore the number of remaining zero cells is \(n^2\) minus that expression: \(n^2 - r\_count \cdot n - c\_count \cdot n + r\_count \cdot c\_count\). We maintain two unordered sets (or boolean arrays) to track distinct rows and columns, incrementing the counts only when a new row or column is first seen. Edge cases: when \(k=0\) (though the problem implies \(k \ge 1\)), the result would be empty; also, repeated operations on the same row/column must not increase counts. Time complexity: \(O(k)\) on average using hash sets, or \(O(k)\) with boolean arrays of size \(n+1\). Space complexity: \(O(n)\) if using arrays, or \(O(k)\) for sets, but \(O(n)\) is typical if \(n\) is known.
