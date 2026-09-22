/*
Write a C++ function `printSpiral` that takes a 2D integer matrix (fixed maximum 100 columns, but passed with actual row count `n` and column count `m`), and returns a `std::string` containing all matrix elements in spiral order, starting from the top-left corner, moving right, then down, then left, then up, repeating inward until all elements are visited. Each element should be followed by a single space (including after the last element, matching the typical output style). The function must handle rectangular matrices (including rows or columns of size 1) without duplication or missing elements, and must be `const`-correct (accept a `const int` matrix pointer or reference). Provide the implementation as a standalone free function, not including a `main` function.
*/

#include <string>
#include <sstream>
#include <vector>

// Given a 2D matrix with fixed max columns (100) but actual rows n and columns m,
// return a string with all elements in spiral order separated by a single space.
std::string printSpiral(const int matrix[][100], int n, int m) {
    std::ostringstream out;
    int top = 0, bottom = n - 1;
    int left = 0, right = m - 1;

    while (top <= bottom && left <= right) {
        // Traverse top row
        for (int j = left; j <= right; ++j) {
            out << matrix[top][j] << ' ';
        }
        // Traverse right column (if more than one row remains)
        if (top < bottom) {
            for (int i = top + 1; i <= bottom; ++i) {
                out << matrix[i][right] << ' ';
            }
        }
        // Traverse bottom row (if more than one row and more than one column)
        if (top < bottom && left < right) {
            for (int j = right - 1; j >= left; --j) {
                out << matrix[bottom][j] << ' ';
            }
        }
        // Traverse left column (if more than one column)
        if (left < right) {
            for (int i = bottom - 1; i >= top + 1; --i) {
                out << matrix[i][left] << ' ';
            }
        }
        ++top; --bottom;
        ++left; --right;
    }

    return out.str();
}

#include <cassert>
#include <string>

// Declaration of the function (from the solution)
std::string printSpiral(const int matrix[][100], int n, int m);

int main() {
    // Test 1: 3x4 matrix from the snippet
    int m1[3][100] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    assert(printSpiral(m1, 3, 4) == "1 2 3 4 8 12 11 10 9 5 6 7 ");

    // Test 2: Single row (1x5)
    int m2[1][100] = {1,2,3,4,5};
    assert(printSpiral(m2, 1, 5) == "1 2 3 4 5 ");

    // Test 3: Single column (5x1)
    int m3[5][100] = {{1},{2},{3},{4},{5}};
    assert(printSpiral(m3, 5, 1) == "1 2 3 4 5 ");

    // Test 4: 1x1 matrix
    int m4[1][100] = {42};
    assert(printSpiral(m4, 1, 1) == "42 ");

    // Test 5: 2x2 matrix
    int m5[2][100] = {{1,2},{3,4}};
    assert(printSpiral(m5, 2, 2) == "1 2 4 3 ");

    // Test 6: 2x3 matrix
    int m6[2][100] = {{1,2,3},{4,5,6}};
    assert(printSpiral(m6, 2, 3) == "1 2 3 6 5 4 ");

    // Test 7: 3x2 matrix
    int m7[3][100] = {{1,2},{3,4},{5,6}};
    assert(printSpiral(m7, 3, 2) == "1 2 4 6 5 3 ");

    // Test 8: All negative numbers
    int m8[2][100] = {{-1,-2},{-3,-4}};
    assert(printSpiral(m8, 2, 2) == "-1 -2 -4 -3 ");

    // Test 9: Larger 4x3 matrix
    int m9[4][100] = {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
    assert(printSpiral(m9, 4, 3) == "1 2 3 6 9 12 11 10 7 4 5 8 ");

    // Test 10: 5x5 identity-like pattern for more edges (just known result)
    int m10[5][100] = {{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20},{21,22,23,24,25}};
    assert(printSpiral(m10, 5, 5) == "1 2 3 4 5 10 15 20 25 24 23 22 21 16 11 6 7 8 9 14 19 18 17 12 13 ");

    return 0;
}

// The solution simulates the classic spiral traversal using four boundaries: `top`, `bottom`, `left`, `right`. Start with `top=0`, `bottom=n-1`, `left=0`, `right=m-1`. In a loop while `top<=bottom && left<=right`, traverse:
// 1. Top row: from `left` to `right` (always safe).
// 2. Right column: from `top+1` to `bottom` (only if `top <= bottom`; the existing check handles it, but we also guard against single-row duplicate by checking `if(top==bottom)` before this? Actually the boundary condition ensures if top==bottom, the bottom row loop will also run, so we need to avoid duplicate. The original snippet uses a break inside the bottom loop for single row, and inside left loop for single column. That works but we can make it cleaner: After top row, if `top < bottom`, traverse right column; after that, if `top < bottom` and `left < right`, traverse bottom row; after that, if `left < right`, traverse left column. This avoids any duplicate. Alternatively keep the original break checks. We'll implement with explicit condition checks for clarity.)
// Edge cases: single row (n=1), single column (m=1), 1x1 matrix. The algorithm ensures each element is appended exactly once. Time complexity O(n*m) because each cell is visited once. Space complexity O(n*m) for the returned string (or O(1) auxiliary if we output directly, but here we build a string). The `const` correctness: pass `const int matrix[][100]` or a pointer to const, and use `std::string` for accumulation.
