// Write a C++ function `int diagonalDifference(int** matrix, int n)` that takes a dynamically allocated square matrix of size `n x n` (where `n >= 1`) and returns the absolute difference between the sum of its main diagonal (top-left to bottom-right) and the sum of its anti-diagonal (top-right to bottom-left). The matrix is guaranteed to be valid and accessible via `matrix[i][j]` for `0 <= i, j < n`. Your function must not modify the matrix and must handle any integer values, including negative numbers.
#include <cassert>

int main() {
    // Test 1: 1x1 matrix
    int** a = new int*[1];
    a[0] = new int[1];
    a[0][0] = 5;
    assert(diagonalDifference(a, 1) == 0);
    delete[] a[0];
    delete[] a;

    // Test 2: 2x2 matrix
    int** b = new int*[2];
    for (int i = 0; i < 2; ++i) b[i] = new int[2];
    int vals2[2][2] = {{1, 2}, {3, 4}};
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) b[i][j] = vals2[i][j];
    // main sum = 1+4=5, anti sum = 2+3=5, diff = 0
    assert(diagonalDifference(b, 2) == 0);
    for (int i = 0; i < 2; ++i) delete[] b[i];
    delete[] b;

    // Test 3: 3x3 matrix with positive values
    int** c = new int*[3];
    for (int i = 0; i < 3; ++i) c[i] = new int[3];
    int vals3[3][3] = {{1, 2, 3}, {4, 5, 6}, {9, 8, 9}};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) c[i][j] = vals3[i][j];
    // main = 1+5+9=15, anti = 3+5+9=17, diff = 2
    assert(diagonalDifference(c, 3) == 2);
    for (int i = 0; i < 3; ++i) delete[] c[i];
    delete[] c;

    // Test 4: 3x3 with negative numbers
    int** d = new int*[3];
    for (int i = 0; i < 3; ++i) d[i] = new int[3];
    int vals4[3][3] = {{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) d[i][j] = vals4[i][j];
    // main = -1-5-9=-15, anti = -3-5-9=-17, diff = 2
    assert(diagonalDifference(d, 3) == 2);
    for (int i = 0; i < 3; ++i) delete[] d[i];
    delete[] d;

    return 0;
}
#include <cstdlib> // for std::abs

// Return the absolute difference between main and anti-diagonal sums.
// matrix is a square n x n dynamic 2D array, n >= 1.
int diagonalDifference(int** matrix, int n) {
    int mainSum = 0;
    int antiSum = 0;
    for (int i = 0; i < n; ++i) {
        mainSum += matrix[i][i];          // main diagonal
        antiSum += matrix[i][n - 1 - i];  // anti-diagonal
    }
    return std::abs(mainSum - antiSum);
}
// The solution simply iterates through each row index `i` from 0 to `n-1`, adding `matrix[i][i]` to the main diagonal sum and `matrix[i][n-1-i]` to the anti-diagonal sum. After the loop, compute the absolute difference between the two sums. Edge cases: when `n == 1`, both diagonals reference the same element, so both sums are equal and the difference is 0. Negative values are handled naturally by integer arithmetic. Time complexity is O(n) for a single pass through the diagonal indices, and space complexity is O(1) beyond the input matrix.
