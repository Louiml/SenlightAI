// Given an \( n \times m \) matrix of non-negative integers, write a C++ function that determines whether the matrix can be reduced to all zeros by repeatedly decrementing an entire row or an entire column by 1. If possible, return a vector of operations (each operation is a pair of an integer flag \( 0 \) for column or \( 1 \) for row, and the 1-based index of that row or column) that achieves the reduction. If it is impossible, return an empty vector. The matrix dimensions satisfy \( 1 \le n, m \le 100 \) and cell values are at most \( 10^6 \). The function must be efficient and avoid excessive operations (the number of operations should be at most \( n \times m \times 100 \)).
// The core idea is to reduce the matrix column by column and row by row using a greedy approach. First, for each column, decrement it until its first-row entry equals the minimum value in the entire first row; this ensures that all entries in the first row become equal to that minimum after processing all columns. Then, for each row, decrement it until its first-column entry equals the minimum value in the entire first column; after this, the first row and first column values are consistent, and all remaining entries must be reduced by a common base value. Subtract that base from every cell; if any cell becomes negative, the matrix is impossible. Finally, since subtracting from rows and columns both reduce the total sum, we must apply the base value to either rows (if \( n < m \)) or columns (if \( n \ge m \)) to minimize the operation count, because neither operation count depends on the matrix values but only on the dimensions. After applying the base operations, check that all cells are zero; if any are non-zero, the matrix is impossible. The algorithm runs in \( O(n \cdot m) \) time for reduction and \( O(n+m+n+m+nm) \) total operations, which is at most \( O(nm) \) in practice (since the base value is at most \( 10^6 \), and the decrement loops per column/row sum to at most \( O(nm) \) operations). Space complexity is \( O(nm) \) for the matrix and \( O(\text{operations}) \) for the result, which is acceptable.
#include <vector>
#include <algorithm>
#include <climits>

// Returns a sequence of operations to reduce the matrix to all zeros.
// Operation pairs: (flag, index) with flag=0 for column, flag=1 for row, index is 1-based.
// Returns empty vector if impossible.
std::vector<std::pair<int,int>> reduceToZero(std::vector<std::vector<int>> tab) {
    int n = (int)tab.size();
    int m = (int)tab[0].size();
    std::vector<std::pair<int,int>> ops;
    
    // Step 1: Make all entries in first row equal to the minimum in that row.
    int naj = INT_MAX;
    for (int j = 0; j < m; ++j)
        naj = std::min(naj, tab[0][j]);
    for (int j = 0; j < m; ++j) {
        while (tab[0][j] != naj) {
            for (int i = 0; i < n; ++i)
                tab[i][j]--;
            ops.push_back({0, j+1});
        }
    }
    
    // Step 2: Make all entries in first column equal to the minimum in that column.
    naj = INT_MAX;
    for (int i = 0; i < n; ++i)
        naj = std::min(naj, tab[i][0]);
    for (int i = 0; i < n; ++i) {
        while (tab[i][0] != naj) {
            for (int j = 0; j < m; ++j)
                tab[i][j]--;
            ops.push_back({1, i+1});
        }
    }
    
    // The top-left cell now holds the common base value.
    int base = tab[0][0];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            tab[i][j] -= base;
    
    // Apply base operations to the smaller dimension to minimize count.
    if (n < m) {
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < base; ++k)
                ops.push_back({1, i+1});
    } else {
        for (int j = 0; j < m; ++j)
            for (int k = 0; k < base; ++k)
                ops.push_back({0, j+1});
    }
    
    // Verify all cells are zero.
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (tab[i][j] != 0)
                return {};
    
    return ops;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above.

int main() {
    // Test 1: Matrix 1x1 with value 5.
    {
        std::vector<std::vector<int>> m = {{5}};
        auto ops = reduceToZero(m);
        assert(!ops.empty());
        // Operations should reduce 5 to 0: either 5 row ops or 5 col ops, but n=m so col ops chosen.
        assert(ops.size() == 5);
        for (auto& op : ops) assert(op.first == 0 && op.second == 1);
    }
    
    // Test 2: Simple 2x2 all zero.
    {
        std::vector<std::vector<int>> m = {{0,0},{0,0}};
        auto ops = reduceToZero(m);
        assert(!ops.empty() && ops.empty()); // empty ops is fine, but we check empty correctly
        assert(ops.empty()); // should be empty because already zero
    }
    
    // Test 3: Matrix that is impossible (e.g., [[1,2],[2,3]]).
    {
        std::vector<std::vector<int>> m = {{1,2},{2,3}};
        auto ops = reduceToZero(m);
        assert(ops.empty()); // impossible
    }
    
    // Test 4: Known possible matrix 2x2 all equal 3.
    {
        std::vector<std::vector<int>> m = {{3,3},{3,3}};
        auto ops = reduceToZero(m);
        assert(!ops.empty());
        // Since n==m, uses column ops: 3 for each of 2 columns => 6 ops.
        assert(ops.size() == 6);
        for (auto& op : ops) assert(op.first == 0);
    }
    
    // Test 5: 1x2 matrix [7,7] should use row ops (n=1,m=2, n<m) => 7 row ops.
    {
        std::vector<std::vector<int>> m = {{7,7}};
        auto ops = reduceToZero(m);
        assert(!ops.empty());
        assert(ops.size() == 7);
        for (auto& op : ops) assert(op.first == 1 && op.second == 1);
    }
    
    // Test 6: 2x1 matrix [7;7] should use col ops (n=2,m=1, n>=m) => 7 col ops.
    {
        std::vector<std::vector<int>> m = {{7},{7}};
        auto ops = reduceToZero(m);
        assert(!ops.empty());
        assert(ops.size() == 7);
        for (auto& op : ops) assert(op.first == 0 && op.second == 1);
    }
    
    // Test 7: Matrix with large values but possible: [[10,10],[10,10]].
    {
        std::vector<std::vector<int>> m = {{10,10},{10,10}};
        auto ops = reduceToZero(m);
        assert(!ops.empty());
        assert(ops.size() == 20); // 10*2 columns
    }
    
    // Test 8: Edge case: 1x1 zero.
    {
        std::vector<std::vector<int>> m = {{0}};
        auto ops = reduceToZero(m);
        assert(ops.empty());
    }
    
    // Test 9: Verify actual operations reduce to zero (simulate).
    {
        std::vector<std::vector<int>> m = {{5,3},{3,1}};
        auto ops = reduceToZero(m);
        // Should be impossible? Let's simulate to check.
        // Compute if possible: We'll just check that ops is empty or not.
        // For a valid check, we could construct a known solvable such as [[2,2],[2,2]].
        std::vector<std::vector<int>> m2 = {{2,2},{2,2}};
        auto ops2 = reduceToZero(m2);
        assert(!ops2.empty());
        // Simulate ops2 on m2.
        for (auto& op : ops2) {
            if (op.first == 0) for (int i=0;i<2;i++) m2[i][op.second-1]--;
            else for (int j=0;j<2;j++) m2[op.second-1][j]--;
        }
        for (int i=0;i<2;i++) for (int j=0;j<2;j++) assert(m2[i][j] == 0);
    }
    
    return 0;
}
