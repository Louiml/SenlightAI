/*
Write a C++ function that accepts a square matrix represented as a vector of vectors of integers and returns a vector of integers containing the matrix elements in a zigzag diagonal order, starting from the top-left element and proceeding along anti-diagonals, alternating direction for each successive diagonal. The first diagonal (just the top-left element) is traversed from top-right to bottom-left, the second diagonal is traversed from bottom-left to top-right, and so on, with the pattern continuing until the bottom-right element is reached. The input matrix is guaranteed to be non-empty and square (number of rows equals number of columns). The function should handle matrices of size 1×1 correctly. The returned vector should have exactly n² elements for an n×n matrix.
*/

#include <vector>

// Returns the elements of a square matrix in zigzag diagonal order.
std::vector<int> matrixDiagonally(const std::vector<std::vector<int>>& mat) {
    int n = static_cast<int>(mat.size());
    std::vector<int> result;
    if (n == 0) return result;
    
    int i = 0, j = 0;
    result.push_back(mat[i][j]);
    
    while (true) {
        if (i == n - 1 && j == n - 1) break;
        
        // Move right (or down at right edge) and traverse down-left diagonal.
        if (j + 1 < n) {
            j++;
            while (j >= 0 && i < n) {
                result.push_back(mat[i][j]);
                i++;
                j--;
            }
            i--;
            j++;
        } else {
            i++;
            while (j >= 0 && i < n) {
                result.push_back(mat[i][j]);
                i++;
                j--;
            }
            i--;
            j++;
        }
        
        if (i == n - 1 && j == n - 1) break;
        
        // Move down (or right at bottom edge) and traverse up-right diagonal.
        if (i + 1 < n) {
            i++;
            while (i >= 0 && j < n) {
                result.push_back(mat[i][j]);
                i--;
                j++;
            }
            j--;
            i++;
        } else {
            j++;
            while (i >= 0 && j < n) {
                result.push_back(mat[i][j]);
                i--;
                j++;
            }
            j--;
            i++;
        }
    }
    
    return result;
}

#include <cassert>
#include <vector>

// Declaration of the function being tested (normally in a header)
std::vector<int> matrixDiagonally(const std::vector<std::vector<int>>& mat);

int main() {
    // Test 1: 1x1 matrix
    std::vector<std::vector<int>> m1 = {{7}};
    assert(matrixDiagonally(m1) == std::vector<int>({7}));
    
    // Test 2: 2x2 matrix
    std::vector<std::vector<int>> m2 = {{1, 2}, {3, 4}};
    assert(matrixDiagonally(m2) == std::vector<int>({1, 2, 3, 4}));
    
    // Test 3: 3x3 matrix
    std::vector<std::vector<int>> m3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    assert(matrixDiagonally(m3) == std::vector<int>({1, 2, 4, 7, 5, 3, 6, 8, 9}));
    
    // Test 4: 4x4 matrix with distinct values
    std::vector<std::vector<int>> m4 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    assert(matrixDiagonally(m4) == std::vector<int>({1, 2, 5, 9, 6, 3, 4, 7, 10, 13, 14, 11, 8, 12, 15, 16}));
    
    // Test 5: 5x5 matrix from the original snippet
    std::vector<std::vector<int>> m5 = {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}};
    assert(matrixDiagonally(m5) == std::vector<int>({1, 2, 1, 1, 2, 3, 4, 3, 2, 1, 1, 2, 3, 4, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 5}));
    
    return 0;
}

// The algorithm simulates the zigzag traversal by tracking a current position (row, column) and moving diagonally in alternating directions. Starting at (0,0), the movement pattern is: move one step right (or down if at the right edge) then traverse the descending anti-diagonal (moving down-left) until hitting the boundary; then move one step down (or right if at the bottom edge) and traverse the ascending anti-diagonal (moving up-right) until hitting the boundary. This alternates until the bottom-right cell is reached. The key edge cases are when the matrix is 1×1 (immediate return after adding the single element) and boundary conditions where a right step is unavailable but a down step is, and vice versa. The implementation uses two while loops inside a main loop, each handling one diagonal direction. After each diagonal traversal, the position is corrected to the next starting point. Time complexity is O(n²) because each of the n² elements is visited exactly once, and space complexity is O(n²) for the output vector, which is required to store the result.
