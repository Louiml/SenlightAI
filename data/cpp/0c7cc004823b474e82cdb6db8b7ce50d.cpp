// Create a C++ function `sumAlternatingPattern` that takes a 2D square array (fixed size 5x5) of integers and returns the sum of all elements that are non-zero, but only counting each position once. The function should ignore all zero values and calculate the total sum of the non-zero entries in the matrix. Since the input is a fixed 5x5 matrix of integers, the function signature should be `int sumAlternatingPattern(const int matrix[5][5])`. The matrix may contain any integer values (positive, negative, or zero), and the function should handle all cases correctly, including matrices where every element is zero (returning 0) or where all elements are non-zero. Do not modify the input matrix.
The solution iterates through every cell of the 5x5 matrix using nested loops. For each cell, check if the value is non-zero; if it is, add it to a running total. The main algorithm is a simple nested loop traversal with a conditional addition. Edge cases include: an all-zero matrix (the sum stays 0), negative values (they contribute negatively to the sum), and the fact that the matrix is fixed at 5x5 so no dynamic resizing is needed. The time complexity is O(25) which is O(1) since the matrix size is constant, and the space complexity is O(1) as only a single integer accumulator is used. No special handling for duplicates or patterns is required—each cell is visited exactly once.
#include <cstddef>

// Return the sum of all non-zero elements in a fixed 5x5 integer matrix.
int sumAlternatingPattern(const int matrix[5][5]) {
    int total = 0;
    for (std::size_t i = 0; i < 5; ++i) {
        for (std::size_t j = 0; j < 5; ++j) {
            if (matrix[i][j] != 0) {
                total += matrix[i][j];
            }
        }
    }
    return total;
}
#include <cassert>

int main() {
    // Test 1: matrix from the original snippet (non-zero sum: 2+2+1+1+1+3+3+3+4+4+4+5+5 = 35)
    const int matrix1[5][5] = {
        {0, 2, 2, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 3, 3, 3, 0},
        {4, 4, 0, 0, 4},
        {5, 0, 0, 0, 5}
    };
    assert(sumAlternatingPattern(matrix1) == 35);

    // Test 2: all zeros
    const int matrix2[5][5] = {0};
    assert(sumAlternatingPattern(matrix2) == 0);

    // Test 3: all non-zero positive
    const int matrix3[5][5] = {
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15},
        {16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25}
    };
    assert(sumAlternatingPattern(matrix3) == 325); // sum 1..25

    // Test 4: negative values
    const int matrix4[5][5] = {
        {0, -1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, -2, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, -3}
    };
    assert(sumAlternatingPattern(matrix4) == -6);

    // Test 5: mixed positive and negative
    const int matrix5[5][5] = {
        {1, 0, -2, 0, 3},
        {0, 0, 0, 0, 0},
        {-4, 0, 5, 0, -6},
        {0, 0, 0, 0, 0},
        {7, 0, -8, 0, 9}
    };
    assert(sumAlternatingPattern(matrix5) == 5); // 1-2+3-4+5-6+7-8+9 = 5

    return 0;
}
