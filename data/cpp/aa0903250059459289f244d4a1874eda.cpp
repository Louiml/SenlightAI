/*
Given a 5x5 matrix of integers provided through standard input (row by row), write a C++ function named `computeBoundarySum` that returns the sum of all integers located on the perimeter (i.e., the first row, last row, first column, and last column) of the matrix. The function should accept a `const std::vector<std::vector<int>>&` representing the matrix, and it must work for any 5x5 matrix. The function should not modify the input matrix, and it should handle the fact that corner elements belong to both a row and a column, so they must be counted exactly once. The program should read the 5x5 matrix from standard input, call the function, and print the resulting sum to standard output.
*/

#include <vector>

// Compute the sum of all elements on the perimeter of a 5x5 matrix.
int computeBoundarySum(const std::vector<std::vector<int>>& matrix) {
    int sum = 0;
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                sum += matrix[i][j];
            }
        }
    }
    return sum;
}

#include <iostream>
#include <vector>
#include <cassert>

// Include or paste the computeBoundarySum function here.

int main() {
    // Test 1: All ones, boundary sum = 16 (perimeter of 5x5 has 16 cells)
    std::vector<std::vector<int>> m1(5, std::vector<int>(5, 1));
    assert(computeBoundarySum(m1) == 16);

    // Test 2: Matrix with increasing values 0..24, boundary sum = 0+1+2+3+4 + 5+9+10+14+15+19+20+21+22+23+24? Compute manually.
    std::vector<std::vector<int>> m2(5, std::vector<int>(5));
    int val = 0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            m2[i][j] = val++;
    // Boundary: row0: 0..4 sum=10; row4: 20..24 sum=110; col0 excluding corners: 5,10,15 sum=30; col4 excluding corners: 9,14,19 sum=42; total=10+110+30+42=192
    assert(computeBoundarySum(m2) == 192);

    // Test 3: Matrix with negative numbers, sum should be computed correctly.
    std::vector<std::vector<int>> m3 = {
        {-1, -2, -3, -4, -5},
        {-6, 0, 0, 0, -7},
        {-8, 0, 0, 0, -9},
        {-10, 0, 0, 0, -11},
        {-12, -13, -14, -15, -16}
    };
    // Boundary sum: row0 sum=-15; row4 sum=-70; col0 excluding corners: -6-8-10=-24; col4 excluding corners: -7-9-11=-27; total=-15-70-24-27=-136
    assert(computeBoundarySum(m3) == -136);

    // Test 4: All zeros
    std::vector<std::vector<int>> m4(5, std::vector<int>(5, 0));
    assert(computeBoundarySum(m4) == 0);

    // Test 5: Large numbers
    std::vector<std::vector<int>> m5(5, std::vector<int>(5, 1000000));
    assert(computeBoundarySum(m5) == 16000000);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// The solution iterates over the matrix and adds elements that lie on the boundary. The boundary consists of the first row (index 0), last row (index 4), first column (index 0), and last column (index 4). A simple approach is to iterate over all cells and check if `i == 0 || i == 4 || j == 0 || j == 4`. If true, add the value to a running sum. This ensures each corner is added only once because the condition is a logical OR—each corner satisfies multiple conditions but is only processed once per loop iteration. Edge cases include a matrix with negative numbers (the sum should still be correct) and the constant size 5x5, so no need to handle variable dimensions. The time complexity is O(25) = O(1) since the matrix size is fixed, and space complexity is O(1) additional space for the sum.
