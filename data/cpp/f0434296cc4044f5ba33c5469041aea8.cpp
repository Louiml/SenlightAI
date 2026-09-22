Write a C++ function `printAllPaths` that takes a 2D matrix represented as a flat 1D array (row-major order), its dimensions `m` and `n`, and prints all possible paths from the top-left cell `(0,0)` to the bottom-right cell `(m-1,n-1)`, where at each step you may only move either down or right. Each path should be printed on its own line, with the values of the cells visited in order, separated by spaces. The function must handle rectangular matrices with `m >= 1` and `n >= 1`. Paths may be printed in any order, but every distinct path must appear exactly once, and no duplicate paths are allowed. The function should not return anything; it should only output to `std::cout`. Use recursion with backtracking, accumulating the current path in a temporary array, and ensure that the output format matches the examples given in the original snippet (e.g., for a 2x2 matrix with values 1,2,3,4, outputs "1 2 4" and "1 3 4").
The main algorithm is a depth-first search (DFS) over the grid, starting from cell `(0,0)` and moving either down (increase row index `i` by 1) or right (increase column index `j` by 1) until reaching the bottom-right corner. We maintain a path array that stores the values of cells visited so far, along with a current length `pi`. At each recursive call, we add the current cell’s value to the path. If we reach the last row (`i == m-1`), we can only move right, so we append all remaining cells in that row to the path and print it. Similarly, if we reach the last column (`j == n-1`), we append all remaining cells in that column and print. Otherwise, we make two recursive calls: one going down, and one going right. Because both choices are explored, all possible monotonic routes are covered exactly once. The base cases handle the boundary where only one direction is possible, avoiding out-of-bounds access. Edge cases include a single-cell matrix (`m=1, n=1`), which should print that single value; and single-row or single-column matrices, where only one path exists. We use a flat array indexing `mat` as `*((mat + i*n) + j)` to access the value at row `i`, column `j`. The time complexity is O(2^(m+n)) in the worst case for the number of paths (specifically, the number of distinct paths is C(m+n-2, m-1)), and each path sums to O(m+n) for printing, so the total time is O((m+n) * number_of_paths). Space complexity is O(m+n) for the recursion stack and the path array, since at most we go m+n-1 steps deep.
#include <iostream>

// Print all paths from top-left to bottom-right moving only down or right.
// mat is a flat 1D array in row-major order of an m x n matrix.
void printAllPaths(const int* mat, int i, int j, int m, int n, int* path, int pi) {
    // Reached the bottom row: only move right.
    if (i == m - 1) {
        for (int k = j; k < n; ++k) {
            path[pi + k - j] = *((mat + i * n) + k);
        }
        for (int l = 0; l < pi + n - j; ++l) {
            std::cout << path[l] << " ";
        }
        std::cout << std::endl;
        return;
    }

    // Reached the rightmost column: only move down.
    if (j == n - 1) {
        for (int k = i; k < m; ++k) {
            path[pi + k - i] = *((mat + k * n) + j);
        }
        for (int l = 0; l < pi + m - i; ++l) {
            std::cout << path[l] << " ";
        }
        std::cout << std::endl;
        return;
    }

    // Add current cell to path.
    path[pi] = *((mat + i * n) + j);

    // Move down.
    printAllPaths(mat, i + 1, j, m, n, path, pi + 1);

    // Move right.
    printAllPaths(mat, i, j + 1, m, n, path, pi + 1);
}
#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output from printAllPaths.
std::string captureOutput(const int* mat, int m, int n) {
    int* path = new int[m + n];
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printAllPaths(mat, 0, 0, m, n, path, 0);
    std::cout.rdbuf(old);
    delete[] path;
    return buffer.str();
}

int main() {
    // 2x2 matrix: 1 2 / 3 4
    int mat2x2[] = {1, 2, 3, 4};
    std::string out2x2 = captureOutput(mat2x2, 2, 2);
    assert(out2x2 == "1 3 4 \n1 2 4 \n");

    // 2x3 matrix: 1 2 3 / 4 5 6
    int mat2x3[] = {1, 2, 3, 4, 5, 6};
    std::string out2x3 = captureOutput(mat2x3, 2, 3);
    assert(out2x3 == "1 4 5 6 \n1 2 5 6 \n1 2 3 6 \n");

    // 3x3 matrix: 1 2 3 / 4 5 6 / 7 8 9
    int mat3x3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::string out3x3 = captureOutput(mat3x3, 3, 3);
    assert(out3x3 == "1 4 7 8 9 \n1 4 5 8 9 \n1 4 5 6 9 \n1 2 5 8 9 \n1 2 5 6 9 \n1 2 3 6 9 \n");

    // Single cell 1x1
    int mat1x1[] = {42};
    std::string out1x1 = captureOutput(mat1x1, 1, 1);
    assert(out1x1 == "42 \n");

    // Single row 1x3
    int mat1x3[] = {7, 8, 9};
    std::string out1x3 = captureOutput(mat1x3, 1, 3);
    assert(out1x3 == "7 8 9 \n");

    // Single column 3x1
    int mat3x1[] = {5, 6, 7};
    std::string out3x1 = captureOutput(mat3x1, 3, 1);
    assert(out3x1 == "5 6 7 \n");

    return 0;
}
