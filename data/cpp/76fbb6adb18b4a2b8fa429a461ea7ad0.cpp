Write a C++ function named `isValidMatrix` that takes a square 2D vector of integers (representing an \(n \times n\) matrix) and returns `true` if every row and every column contains all distinct integers from 1 to \(n\) inclusive (i.e., a valid permutation of numbers 1..n), and `false` otherwise. The input matrix is guaranteed to be non-empty and square (equal number of rows and columns). Handle duplicates, out-of-range values (e.g., 0 or > n), and unsorted order correctly. The function must be `const`-correct: it should accept the matrix as `const std::vector<std::vector<int>>&` and should not modify it. Assume the matrix may contain any integers, not necessarily positive, and you must validate that each row and column is a permutation of exactly the numbers 1 through n.

// The algorithm validates the matrix by checking each row and each column independently for being a permutation of 1..n. A straightforward approach uses a `std::set<int>` (or a boolean frequency array) for each row/column. For each row, insert all its elements into a set. If the set size is not equal to n, or any element is outside the range [1, n], then the row is invalid. Similarly, for each column, build a set from the column's elements and perform the same checks. Using a set automatically handles duplicates (since duplicates reduce the set size) and allows easy verification of unique values. For efficiency, you can also use a `vector<bool>` of size n+1 to track seen numbers, which avoids the overhead of a set. Edge cases include matrices of size 1x1 (must contain exactly 1), matrices with negative or zero values, values greater than n, and matrices with n=0 (though the problem guarantees non-empty, it's safe to handle). Time complexity is O(n^2) because each of the n² elements is processed once for rows and once for columns, giving O(2n²) = O(n²). Space complexity is O(n) for the set or boolean array used per row/column.

#include <vector>
#include <unordered_set>

bool isValidMatrix(const std::vector<std::vector<int>>& matrix) {
    const int n = static_cast<int>(matrix.size());
    
    // Validate each row.
    for (int i = 0; i < n; ++i) {
        std::unordered_set<int> seen;
        for (int j = 0; j < n; ++j) {
            int val = matrix[i][j];
            if (val < 1 || val > n) return false;
            if (!seen.insert(val).second) return false; // duplicate found
        }
        if (static_cast<int>(seen.size()) != n) return false;
    }
    
    // Validate each column.
    for (int j = 0; j < n; ++j) {
        std::unordered_set<int> seen;
        for (int i = 0; i < n; ++i) {
            int val = matrix[i][j];
            if (val < 1 || val > n) return false;
            if (!seen.insert(val).second) return false; // duplicate found
        }
        if (static_cast<int>(seen.size()) != n) return false;
    }
    
    return true;
}

#include <cassert>
#include <vector>

int main() {
    // Valid 1x1 matrix
    std::vector<std::vector<int>> m1 = {{1}};
    assert(isValidMatrix(m1) == true);

    // Valid 3x3 Latin square (permutation per row/col)
    std::vector<std::vector<int>> m2 = {{1,2,3}, {2,3,1}, {3,1,2}};
    assert(isValidMatrix(m2) == true);

    // Invalid: duplicate in first row (1 appears twice)
    std::vector<std::vector<int>> m3 = {{1,2,2}, {2,3,1}, {3,1,2}};
    assert(isValidMatrix(m3) == false);

    // Invalid: missing number 3 in second row
    std::vector<std::vector<int>> m4 = {{1,2,3}, {1,2,3}, {3,1,2}};
    assert(isValidMatrix(m4) == false);

    // Invalid: value out of range (0) in first column
    std::vector<std::vector<int>> m5 = {{0,2,3}, {2,3,1}, {3,1,2}};
    assert(isValidMatrix(m5) == false);

    // Invalid: value greater than n (4) in third column
    std::vector<std::vector<int>> m6 = {{1,2,4}, {2,3,1}, {3,1,2}};
    assert(isValidMatrix(m6) == false);

    // Valid 2x2 matrix
    std::vector<std::vector<int>> m7 = {{2,1}, {1,2}};
    assert(isValidMatrix(m7) == true);

    // Invalid: duplicate in column 0 (both rows have 1)
    std::vector<std::vector<int>> m8 = {{1,2}, {1,3}};
    assert(isValidMatrix(m8) == false);

    return 0;
}
