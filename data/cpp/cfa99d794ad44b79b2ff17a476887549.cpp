// Write a C++ function `int sumPattern(const std::vector<std::vector<int>>& matrix)` that takes a square matrix of odd dimension `n` (where `n >= 1`) and returns the sum of specific elements arranged in a pattern: the entire middle row, the entire middle column (but the center element is counted only once), plus the four edge "arms" extending from the center to the corners — specifically, the top-left arm (row 0, columns 0 to n/2 - 1), the top-right arm (rows 0 to n/2 - 1, last column), the bottom-left arm (rows n/2 + 1 to n-1, column 0), and the bottom-right arm (last row, columns n/2 + 1 to n-1). The input is guaranteed to be square and odd-sized, but your function should not rely on that assumption; if the matrix is empty or even-sized, return 0. For example, for a 5×5 matrix with values increasing by row and column, the sum follows the described pattern. The function must be `const`-correct and handle any valid input without modification to the original matrix.

The solution involves summing a deterministic set of indices. The center of the matrix is at `(n/2, n/2)` where `n` is the dimension. The pattern consists of:  
1. All elements in the middle row: `matrix[n/2][col]` for `col` from 0 to n-1.  
2. All elements in the middle column: `matrix[row][n/2]` for `row` from 0 to n-1.  
3. The center element is counted twice in steps 1 and 2, so we subtract it once.  
4. Add the top-left arm: row 0, columns 0 to n/2 - 1 (excludes the middle column's top element, which is already counted).  
5. Add the top-right arm: rows 0 to n/2 - 1, last column (excludes the middle row's last element, already counted).  
6. Add the bottom-left arm: rows n/2 + 1 to n-1, column 0 (excludes the middle row's first element).  
7. Add the bottom-right arm: last row, columns n/2 + 1 to n-1 (excludes the middle column's last element).  

Important edge cases:  
- If `n == 1`, the pattern is just the single center element. Steps 1 and 2 both add it, we subtract it once, and all arms are empty, giving the correct sum = matrix[0][0].  
- If `n` is even or the matrix is empty, we return 0 per the specification.  
- The arms carefully avoid double-counting edge elements that are already part of the middle row/column. For example, the top-left arm excludes column `n/2` because that is already in the middle column.  

Time complexity: O(n) because we iterate over at most 4n elements (worst case for n×n matrix, but we only touch O(n) elements). Space complexity: O(1) extra, not counting the input matrix. The function uses `const` references and does not modify the input.

#include <vector>

// Computes the sum of the middle row, middle column (center counted once),
// and the four edge arms extending from the center to the corners.
// Returns 0 if the matrix is empty or has even dimension.
int sumPattern(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return 0;
    int n = static_cast<int>(matrix.size());
    if (n % 2 == 0) return 0;  // only odd dimensions supported

    int sum = 0;
    int center = n / 2;

    // Middle row
    for (int col = 0; col < n; ++col) {
        sum += matrix[center][col];
    }
    // Middle column (skip center to avoid double count, then add once more)
    for (int row = 0; row < n; ++row) {
        if (row != center) sum += matrix[row][center];
    }
    // Now the center is already counted once from the middle row.

    // Top-left arm: row 0, columns 0..center-1
    for (int col = 0; col < center; ++col) {
        sum += matrix[0][col];
    }
    // Top-right arm: rows 0..center-1, last column
    for (int row = 0; row < center; ++row) {
        sum += matrix[row][n - 1];
    }
    // Bottom-left arm: rows center+1..n-1, column 0
    for (int row = center + 1; row < n; ++row) {
        sum += matrix[row][0];
    }
    // Bottom-right arm: last row, columns center+1..n-1
    for (int col = center + 1; col < n; ++col) {
        sum += matrix[n - 1][col];
    }

    return sum;
}

#include <cassert>
#include <vector>

// Include the solution function above here.

int main() {
    // Test 1: 1x1 matrix
    std::vector<std::vector<int>> m1 = {{5}};
    assert(sumPattern(m1) == 5);

    // Test 2: 3x3 with simple values
    std::vector<std::vector<int>> m2 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    // Middle row sum = 3, middle column sum = 3 (center only once => 3+2=5)
    // Arms: top-left = 1, top-right = 1, bottom-left = 1, bottom-right = 1 => total 5+4=9
    assert(sumPattern(m2) == 9);

    // Test 3: 5x5 from problem sample (first sample)
    std::vector<std::vector<int>> m3 = {
        {1, 2, 3, 4, 5},
        {2, 3, 4, 1, 6},
        {3, 4, 9, 6, 7},
        {4, 2, 6, 7, 8},
        {5, 4, 3, 2, 1}
    };
    assert(sumPattern(m3) == 71);

    // Test 4: 7x7 all ones (second sample)
    std::vector<std::vector<int>> m4(7, std::vector<int>(7, 1));
    // Middle row: 7, middle column: 7 minus center = 6 → total 13
    // Arms: each arm has 3 elements (center-1 per side) -> 4*3 = 12 → total 25
    assert(sumPattern(m4) == 25);

    // Test 5: Even dimension returns 0
    std::vector<std::vector<int>> m5 = {{1, 2}, {3, 4}};
    assert(sumPattern(m5) == 0);

    // Test 6: Empty matrix returns 0
    std::vector<std::vector<int>> m6;
    assert(sumPattern(m6) == 0);

    return 0;
}
