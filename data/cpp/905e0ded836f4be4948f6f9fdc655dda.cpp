// Write a C++ function that, given a positive even integer `n` that is divisible by 4, generates and prints an `n x n` doubly-even magic square (where the sum of every row, column, and main diagonal is equal). The function must implement the standard algorithm: first fill the matrix row-wise with consecutive integers from 1 to n², then complement (replace each element with `(n*n + 1) - element`) only those entries that fall inside four corner blocks of size `(n/4) × (n/4)` and the central block of size `(n/2) × (n/2)`. The function should print the square row by row, with each number separated by a space and each row on a new line. The function must handle only valid inputs (n is a positive integer multiple of 4). Do not include a main function in your solution; write only the free function and necessary headers.

#include <cassert>
#include <sstream>
#include <vector>

// Helper function to capture the printed output of printDoublyEvenMagicSquare.
std::vector<std::vector<int>> captureOutput(int n) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printDoublyEvenMagicSquare(n);
    std::cout.rdbuf(oldCout);

    std::vector<std::vector<int>> result(n, std::vector<int>(n));
    int value;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            buffer >> value;
            result[i][j] = value;
        }
    }
    return result;
}

// Helper to check magic square properties.
bool isMagic(const std::vector<std::vector<int>>& square) {
    int n = square.size();
    int sum = 0;
    for (int j = 0; j < n; ++j) sum += square[0][j];

    // Check all rows.
    for (int i = 1; i < n; ++i) {
        int rowSum = 0;
        for (int j = 0; j < n; ++j) rowSum += square[i][j];
        if (rowSum != sum) return false;
    }
    // Check all columns.
    for (int j = 0; j < n; ++j) {
        int colSum = 0;
        for (int i = 0; i < n; ++i) colSum += square[i][j];
        if (colSum != sum) return false;
    }
    // Check main diagonal.
    int diag1 = 0, diag2 = 0;
    for (int i = 0; i < n; ++i) {
        diag1 += square[i][i];
        diag2 += square[i][n - 1 - i];
    }
    return (diag1 == sum) && (diag2 == sum);
}

int main() {
    // n = 4
    auto sq4 = captureOutput(4);
    assert(isMagic(sq4));
    // Check each number 1..16 appears exactly once.
    std::vector<bool> seen(17, false);
    for (auto& row : sq4) {
        for (int v : row) {
            assert(v >= 1 && v <= 16);
            assert(!seen[v]);
            seen[v] = true;
        }
    }
    // Check the constant sum for n=4: n*(n^2+1)/2 = 4*17/2 = 34.
    for (int i = 0; i < 4; ++i) {
        int rowSum = 0;
        for (int j = 0; j < 4; ++j) rowSum += sq4[i][j];
        assert(rowSum == 34);
    }

    // n = 8
    auto sq8 = captureOutput(8);
    assert(isMagic(sq8));
    // Check constant sum for n=8: n*(n^2+1)/2 = 8*65/2 = 260.
    for (int j = 0; j < 8; ++j) {
        int colSum = 0;
        for (int i = 0; i < 8; ++i) colSum += sq8[i][j];
        assert(colSum == 260);
    }

    // n = 12
    auto sq12 = captureOutput(12);
    assert(isMagic(sq12));
    // Check constant sum for n=12: n*(n^2+1)/2 = 12*145/2 = 870.
    int diagSum = 0;
    for (int i = 0; i < 12; ++i) diagSum += sq12[i][i];
    assert(diagSum == 870);

    return 0;
}

#include <iostream>
#include <vector>

// Generate and print a doubly-even magic square of order n (n must be multiple of 4).
void printDoublyEvenMagicSquare(int n) {
    // Create and fill the matrix row-wise from 1 to n*n.
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = n * i + j + 1;
        }
    }

    // Complement the four corner blocks of size (n/4) x (n/4).
    for (int i = 0; i < n / 4; ++i) {
        for (int j = 0; j < n / 4; ++j) {
            matrix[i][j] = (n * n + 1) - matrix[i][j];                      // top-left
            matrix[i][n - 1 - j] = (n * n + 1) - matrix[i][n - 1 - j];      // top-right
            matrix[n - 1 - i][j] = (n * n + 1) - matrix[n - 1 - i][j];      // bottom-left
            matrix[n - 1 - i][n - 1 - j] = (n * n + 1) - matrix[n - 1 - i][n - 1 - j]; // bottom-right
        }
    }

    // Complement the central block of size (n/2) x (n/2).
    for (int i = n / 4; i < 3 * n / 4; ++i) {
        for (int j = n / 4; j < 3 * n / 4; ++j) {
            matrix[i][j] = (n * n + 1) - matrix[i][j];
        }
    }

    // Print the magic square.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// The solution follows the classic doubly-even magic square construction. First, create an `n × n` matrix and fill it sequentially: `arr[i][j] = n*i + j + 1`. After this, the matrix contains numbers 1 through n² in row-major order. The key property of a magic square with doubly-even order is that if you replace every element `x` by `n² + 1 - x`, the square remains magic (if originally magic) and the resulting square is also magic. In this construction, we only complement selected regions: the four corner blocks of size `(n/4) × (n/4)` (top-left, top-right, bottom-left, bottom-right) and the central block of size `(n/2) × (n/2)` (from row `n/4` to `3n/4 - 1`, and column `n/4` to `3n/4 - 1`). These five disjoint blocks together cover exactly half of the n² cells (since each block has size `(n/4)²` × 4 + `(n/2)²` = `n²/4` + `n²/4` = `n²/2`). Complementing exactly half the cells yields equal row, column, and diagonal sums because the original filled matrix has symmetric properties when complemented in this pattern. Time complexity is O(n²) for filling, complementing, and printing; space complexity is O(n²) for the matrix storage. Edge cases: only valid `n` (multiple of 4) are passed; for `n=4`, the blocks are all size 1 except the central 2×2, which works correctly. No special error handling is required for invalid inputs. The printing uses simple `cout` output.
