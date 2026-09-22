/*
Given an \( n \times m \) matrix of integers, write a C++ function that computes, for each cell \( (i,j) \), the value \( ans[i][j] = 1 + \max(r_1, r_2) + \max(c_1, c_2) \), where \( r_1 \) is the number of distinct values strictly smaller than \( a[i][j] \) in row \( i \), \( r_2 \) is the number of distinct values strictly larger than \( a[i][j] \) in row \( i \), \( c_1 \) is the number of distinct values strictly smaller in column \( j \), and \( c_2 \) is the number strictly larger in column \( j \). The function should return a vector of vectors of integers of the same dimensions, with all entries computed correctly. You may assume \( 1 \le n, m \le 100 \) and values are 32-bit integers.
*/
#include <vector>
#include <algorithm>

// Compute ans[i][j] = 1 + max(rowSmaller, rowLarger) + max(colSmaller, colLarger)
// rowSmaller = number of distinct values in row i strictly less than a[i][j]
// rowLarger  = number of distinct values in row i strictly greater than a[i][j]
// colSmaller, colLarger similarly for column j.
std::vector<std::vector<int>> computeMatrixAns(const std::vector<std::vector<int>>& a) {
    int n = (int)a.size();
    int m = (int)a[0].size();

    // rowSmaller[i][j], rowLarger[i][j]
    std::vector<std::vector<int>> rowSmaller(n, std::vector<int>(m, 0));
    std::vector<std::vector<int>> rowLarger(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        std::vector<std::pair<int,int>> tp(m);
        for (int j = 0; j < m; ++j) tp[j] = {a[i][j], j};
        std::sort(tp.begin(), tp.end());
        int cnt = 0;
        int j = 0;
        while (j < m) {
            int last = j;
            while (last < m && tp[last].first == tp[j].first) ++last;
            for (int k = j; k < last; ++k) rowSmaller[i][tp[k].second] = cnt;
            ++cnt;
            j = last;
        }
        cnt = 0;
        j = m - 1;
        while (j >= 0) {
            int last = j;
            while (last >= 0 && tp[last].first == tp[j].first) --last;
            for (int k = last + 1; k <= j; ++k) rowLarger[i][tp[k].second] = cnt;
            ++cnt;
            j = last;
        }
    }

    // colSmaller[i][j], colLarger[i][j]
    std::vector<std::vector<int>> colSmaller(n, std::vector<int>(m, 0));
    std::vector<std::vector<int>> colLarger(n, std::vector<int>(m, 0));

    for (int j = 0; j < m; ++j) {
        std::vector<std::pair<int,int>> tp(n);
        for (int i = 0; i < n; ++i) tp[i] = {a[i][j], i};
        std::sort(tp.begin(), tp.end());
        int cnt = 0;
        int i = 0;
        while (i < n) {
            int last = i;
            while (last < n && tp[last].first == tp[i].first) ++last;
            for (int k = i; k < last; ++k) colSmaller[tp[k].second][j] = cnt;
            ++cnt;
            i = last;
        }
        cnt = 0;
        i = n - 1;
        while (i >= 0) {
            int last = i;
            while (last >= 0 && tp[last].first == tp[i].first) --last;
            for (int k = last + 1; k <= i; ++k) colLarger[tp[k].second][j] = cnt;
            ++cnt;
            i = last;
        }
    }

    std::vector<std::vector<int>> ans(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            ans[i][j] = 1 + std::max(rowSmaller[i][j], rowLarger[i][j])
                          + std::max(colSmaller[i][j], colLarger[i][j]);
        }
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <iostream>

// Assume the solution function is declared above
// std::vector<std::vector<int>> computeMatrixAns(const std::vector<std::vector<int>>& a);

int main() {
    // 1x1 matrix
    {
        std::vector<std::vector<int>> a = {{5}};
        auto ans = computeMatrixAns(a);
        assert(ans.size() == 1 && ans[0].size() == 1);
        assert(ans[0][0] == 1); // 1 + max(0,0) + max(0,0) = 1
    }

    // 1x3 all same
    {
        std::vector<std::vector<int>> a = {{7,7,7}};
        auto ans = computeMatrixAns(a);
        std::vector<std::vector<int>> expected = {{1,1,1}};
        assert(ans == expected);
    }

    // 3x1 all distinct
    {
        std::vector<std::vector<int>> a = {{1},{2},{3}};
        auto ans = computeMatrixAns(a);
        // Row: each row has 1 element -> rowSmaller=0, rowLarger=0.
        // Column: 1-> smaller=0, larger=2; 2-> smaller=1, larger=1; 3-> smaller=2, larger=0
        // So max(colSmaller,colLarger) = max(0,2)=2 for 1, max(1,1)=1 for 2, max(2,0)=2 for 3
        // ans = 1 + 0 + that
        std::vector<std::vector<int>> expected = {{3},{2},{3}};
        assert(ans == expected);
    }

    // 2x2 matrix with duplicates
    {
        std::vector<std::vector<int>> a = {{10, 20}, {10, 20}};
        auto ans = computeMatrixAns(a);
        // Row 0: [10,20]: 10 smaller=0, larger=1; 20 smaller=1, larger=0
        // Row 1: same
        // Col 0: [10,10]: both smaller=0, larger=0
        // Col 1: [20,20]: both smaller=0, larger=0
        // For (0,0): 1 + max(0,1) + max(0,0) = 1+1+0=2
        // For (0,1): 1 + max(1,0) + max(0,0) = 1+1+0=2
        // For (1,0): 1 + max(0,1) + max(0,0) = 2
        // For (1,1): 1 + max(1,0) + max(0,0) = 2
        std::vector<std::vector<int>> expected = {{2,2},{2,2}};
        assert(ans == expected);
    }

    // 3x3 all distinct, both rows and columns have increasing order
    {
        std::vector<std::vector<int>> a = {
            {1,2,3},
            {4,5,6},
            {7,8,9}
        };
        auto ans = computeMatrixAns(a);
        // For (1,1) value 5:
        // Row 1: [4,5,6] -> smaller distinct =1 (value 4), larger =1 (value 6) -> max=1
        // Col 1: [2,5,8] -> smaller distinct =1 (2), larger =1 (8) -> max=1
        // ans = 1+1+1=3
        // For (0,0) value 1:
        // Row: smaller=0, larger=2 -> max=2
        // Col: smaller=0, larger=2 -> max=2
        // ans = 1+2+2=5
        std::vector<std::vector<int>> expected = {
            {5,4,5},
            {4,3,4},
            {5,4,5}
        };
        assert(ans == expected);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// For each row and each column, we need to know, for every element, how many distinct values are smaller and how many are larger within that row/column. We can solve this by processing each row independently and each column independently. For a given row (or column), create an array of pairs (value, original index), sort the array by value. After sorting, equal values are contiguous. We can then assign for each element the number of strictly smaller distinct values: that is simply the count of distinct groups before its group. For strictly larger values, that is the number of distinct groups after its group. To avoid double counting when processing rows and columns, we store separate arrays: one for the "smaller count" per row, and one for the "larger count" per row, and likewise per column. However, we can combine them: for each cell, we need \( \max(\text{rowSmaller}, \text{rowLarger}) \) and \( \max(\text{colSmaller}, \text{colLarger}) \). Actually the task says take max of row smaller and row larger? Wait, the code uses max(mx[0][i][j], mx[1][i][j]) where mx[0] is the count of smaller distinct values in the row (or column depending on flag), and mx[1] is the count of larger distinct values in that row/column? Let's read the code: In constructt(flag), for each row/column, they sort. They compute cnt=0 for increasing order, assign mx[flag][...] = cnt, then cnt increments for each distinct group. That gives "number of distinct values strictly smaller" for each element. Then decreasing loop computes mpp[flag][...] = cnt, which is "number of distinct values strictly larger". So the final ans = 1 + max(rowSmaller, rowLarger) + max(colSmaller, colLarger). That seems odd, but for the task we will implement exactly that. Edge cases include duplicate values in a row/column: they all get the same smaller and larger counts. For a row all equal, each element has smaller=0, larger=0. Complexity: For each row we sort O(m log m), for each column sort O(n log n). Total O(n m log(max(n,m))) time, O(n m) space.
