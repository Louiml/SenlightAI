Write a C++ function named `sumOfDiagonals` that takes a square matrix represented as a 2D array with a compile-time maximum size (`MAX_SIZE = 100`) and its actual dimension `n`, and returns the sum of the two main diagonals (the main diagonal from top-left to bottom-right and the anti-diagonal from top-right to bottom-left). The center element, if `n` is odd, appears in both diagonals and must be counted only once. The function should be `const`-correct (i.e., accept the matrix as `const int matrix[MAX_SIZE][MAX_SIZE]`), and it must not modify the input. The matrix is guaranteed to be square with `n` between 1 and `MAX_SIZE` inclusive.

#include <cassert>

int main() {
    // Test 1: 1x1 matrix
    int m1[100][100] = {{5}};
    assert(sumOfDiagonals(m1, 1) == 5);

    // Test 2: 2x2 matrix
    int m2[100][100] = {{1,2},{3,4}};
    assert(sumOfDiagonals(m2, 2) == 1 + 4 + 2 + 3); // 10

    // Test 3: 3x3 matrix, center counted once
    int m3[100][100] = {{1,2,3},{4,5,6},{7,8,9}};
    assert(sumOfDiagonals(m3, 3) == 1+5+9 + 3+5+7 - 5); // 25

    // Test 4: 4x4 matrix with zeros
    int m4[100][100] = {{0,1,2,3},{4,0,5,6},{7,8,0,9},{10,11,12,0}};
    assert(sumOfDiagonals(m4, 4) == 0 + 0 + 0 + 0 + 3 + 6 + 7 + 10); // 26

    // Test 5: 5x5 with all ones, center counted once
    int m5[100][100] = {};
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 5; ++j) m5[i][j] = 1;
    assert(sumOfDiagonals(m5, 5) == 10 - 1); // 9

    // Test 6: Large 100x100 with pattern, ensure no overflow (values small)
    int m6[100][100] = {};
    for (int i = 0; i < 100; ++i) for (int j = 0; j < 100; ++j) m6[i][j] = (i + j) % 2;
    int expected = 0;
    for (int i = 0; i < 100; ++i) expected += m6[i][i] + m6[i][99 - i];
    // n even, no subtraction
    assert(sumOfDiagonals(m6, 100) == expected);
}

#include <cstddef> // for std::size_t if needed, but not required

// Returns the sum of the two diagonals of a square matrix.
// The center element (when n is odd) is counted only once.
int sumOfDiagonals(const int matrix[100][100], int n) {
    int total = 0;
    for (int i = 0; i < n; ++i) {
        total += matrix[i][i];              // main diagonal
        total += matrix[i][n - i - 1];      // anti-diagonal
    }
    if (n % 2 == 1) {
        total -= matrix[n / 2][n / 2];      // subtract duplicate center
    }
    return total;
}

// The solution computes the sum of the main diagonal by iterating `i` from 0 to `n-1` and adding `matrix[i][i]`. For the anti-diagonal, iterate `i` from 0 to `n-1` and add `matrix[i][n - i - 1]`. However, when `n` is odd, the center element (at index `i = n/2`) is added twice because both diagonals intersect at that position (since `n - i - 1 = i` when `i = n/2`). To avoid double-counting, after summing both diagonals, if `n` is odd, subtract the center element once (i.e., `matrix[n/2][n/2]`). The algorithm runs in `O(n)` time, because it performs two passes of `n` iterations (or one combined pass), and uses `O(1)` auxiliary space. Edge cases include `n = 1`, where both diagonals are the same single element; the subtraction ensures the sum is that element exactly once. `n` is always positive and ≤ `MAX_SIZE`, so no out-of-bounds access occurs.
