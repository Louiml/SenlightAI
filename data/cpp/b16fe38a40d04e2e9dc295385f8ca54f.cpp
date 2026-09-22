/*
Given a fixed-size 3x3 matrix of double-precision floating-point values (represented as `std::array<std::array<double,3>,3>` or a simple nested C-array), write a C++ function that returns a `std::array<double,3>` containing the minimum value of each column, in column order (column 0, column 1, column 2). The function must handle arbitrary real values, including negative numbers, zeros, and duplicate values across or within columns. Do not use any external math or linear algebra libraries; implement the column-wise minimum logic yourself.
*/
#include <array>
#include <algorithm>  // For std::min

// Returns a std::array<double,3> where each element is the minimum of the corresponding column of the 3x3 input matrix.
// The input matrix is passed as a const reference to a 3x3 array of arrays.
std::array<double,3> columnWiseMin(const std::array<std::array<double,3>,3>& matrix) {
    std::array<double,3> result;
    
    // For each column index c from 0 to 2
    for (size_t c = 0; c < 3; ++c) {
        // Start with the first row value for this column
        double current_min = matrix[0][c];
        // Check rows 1 and 2
        for (size_t r = 1; r < 3; ++r) {
            current_min = std::min(current_min, matrix[r][c]);
        }
        result[c] = current_min;
    }
    
    return result;
}
#include <cassert>
#include <cmath>  // For fabs, to compare floating-point values with tolerance

int main() {
    // Test 1: Basic positive values
    {
        std::array<std::array<double,3>,3> m = {{{1.0, 2.0, 3.0},
                                                 {4.0, 5.0, 6.0},
                                                 {7.0, 8.0, 9.0}}};
        auto result = columnWiseMin(m);
        assert(result[0] == 1.0 && result[1] == 2.0 && result[2] == 3.0);
    }
    
    // Test 2: Negative values and zeros
    {
        std::array<std::array<double,3>,3> m = {{{-1.5, 0.0,  2.0},
                                                 { 3.0, -2.2, 1.0},
                                                 { 0.5,  0.1, -0.7}}};
        auto result = columnWiseMin(m);
        assert(fabs(result[0] - (-1.5)) < 1e-9);
        assert(fabs(result[1] - (-2.2)) < 1e-9);
        assert(fabs(result[2] - (-0.7)) < 1e-9);
    }
    
    // Test 3: All values identical
    {
        std::array<std::array<double,3>,3> m = {{{5.5, 5.5, 5.5},
                                                 {5.5, 5.5, 5.5},
                                                 {5.5, 5.5, 5.5}}};
        auto result = columnWiseMin(m);
        assert(result[0] == 5.5 && result[1] == 5.5 && result[2] == 5.5);
    }
    
    // Test 4: Minimum appears in different rows for different columns
    {
        std::array<std::array<double,3>,3> m = {{{8.0, 1.0, 4.0},
                                                 {3.0, 9.0, 2.0},
                                                 {6.0, 5.0, 7.0}}};
        auto result = columnWiseMin(m);
        assert(result[0] == 3.0 && result[1] == 1.0 && result[2] == 2.0);
    }
    
    // Test 5: Duplicate minimum in a column
    {
        std::array<std::array<double,3>,3> m = {{{2.0, 4.0, 7.0},
                                                 {2.0, 1.0, 7.0},
                                                 {9.0, 1.0, 7.0}}};
        auto result = columnWiseMin(m);
        assert(result[0] == 2.0 && result[1] == 1.0 && result[2] == 7.0);
    }
    
    // Test 6: Large positive and negative values
    {
        std::array<std::array<double,3>,3> m = {{{1e10, -1e10, 1e-10},
                                                 {-1e10, 1e10, -1e-10},
                                                 {0.0,  0.0,  0.0}}};
        auto result = columnWiseMin(m);
        assert(fabs(result[0] - (-1e10)) < 1e-5);
        assert(fabs(result[1] - (-1e10)) < 1e-5);
        assert(fabs(result[2] - (-1e-10)) < 1e-15);
    }
    
    return 0;
}
// The central algorithm is straightforward: iterate over each of the three columns, and for each column, scan the three rows to find the smallest element. Initialize each column's minimum to the element at row 0 (or `+infinity`), then compare and update with the remaining two rows. Since the matrix dimension is fixed at 3×3, the time complexity is constant \(O(3 \times 3) = O(1)\), and auxiliary space is constant \(O(1)\) for the output array. Edge cases to consider: negative values (a minimum can be negative), zero values, and duplicate values (e.g., all entries equal — the minimum is that value). The function must not modify the input matrix, so the parameter should be taken by `const&` (or by value if small, but `const&` is idiomatic for larger types). The output array should be returned by value, and the function should be marked `noexcept` if possible, though not required.
