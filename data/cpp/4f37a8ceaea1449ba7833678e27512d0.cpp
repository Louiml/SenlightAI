Write a C++ function named `add2x2Matrices` that takes two constant references to 2D arrays of integers, each of size 2×2, and returns a 2D array of integers (also 2×2) containing the element-wise sum of the two input matrices. The function must not modify the inputs, must handle any integer values (including negative and zero), and must be reusable for arbitrary 2×2 matrix data. The returned matrix should be formed such that `result[i][j] = mat1[i][j] + mat2[i][j]` for all `i` and `j` in the range [0, 2). You may assume the inputs are always exactly 2×2 in size; no error handling for mismatched dimensions is required. The task is to practice multi-dimensional array manipulation, const-correctness, and returning array types via appropriate data structures (e.g., `std::array`).

#include <cassert>
#include <array>
#include "solution.h" // Assuming the above code is in solution.h

int main() {
    using Matrix = std::array<std::array<int, 2>, 2>;
    
    // Test case 1: Basic positive integers
    Matrix a1{{{{1, 2}, {3, 4}}}};
    Matrix b1{{{{5, 6}, {7, 8}}}};
    Matrix r1 = add2x2Matrices(a1, b1);
    assert(r1[0][0] == 6 && r1[0][1] == 8);
    assert(r1[1][0] == 10 && r1[1][1] == 12);
    
    // Test case 2: Negative numbers
    Matrix a2{{{{-1, -2}, {-3, -4}}}};
    Matrix b2{{{{1, 2}, {3, 4}}}};
    Matrix r2 = add2x2Matrices(a2, b2);
    assert(r2[0][0] == 0 && r2[0][1] == 0);
    assert(r2[1][0] == 0 && r2[1][1] == 0);
    
    // Test case 3: Mixed values
    Matrix a3{{{{10, -5}, {0, 7}}}};
    Matrix b3{{{{-3, 5}, {8, -2}}}};
    Matrix r3 = add2x2Matrices(a3, b3);
    assert(r3[0][0] == 7 && r3[0][1] == 0);
    assert(r3[1][0] == 8 && r3[1][1] == 5);
    
    // Test case 4: Zero matrices
    Matrix zero{{{{0, 0}, {0, 0}}}};
    Matrix r4 = add2x2Matrices(a1, zero);
    assert(r4[0][0] == 1 && r4[0][1] == 2);
    assert(r4[1][0] == 3 && r4[1][1] == 4);
    
    // Test case 5: Large integers (within int range)
    Matrix a5{{{{100000, 200000}, {300000, 400000}}}};
    Matrix b5{{{{-100000, 200000}, {700000, -400000}}}};
    Matrix r5 = add2x2Matrices(a5, b5);
    assert(r5[0][0] == 0 && r5[0][1] == 400000);
    assert(r5[1][0] == 1000000 && r5[1][1] == 0);
    
    return 0;
}

#include <array>

// Adds two 2x2 matrices element-wise and returns the result.
std::array<std::array<int, 2>, 2> add2x2Matrices(
    const std::array<std::array<int, 2>, 2>& mat1,
    const std::array<std::array<int, 2>, 2>& mat2) {
    
    std::array<std::array<int, 2>, 2> result{};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    return result;
}

// The solution uses `std::array<std::array<int, 2>, 2>` to represent a 2×2 matrix, which allows returning it by value and avoids C-style array decay issues. The algorithm iterates over rows `i` from 0 to 1 and columns `j` from 0 to 1, computing `result[i][j] = mat1[i][j] + mat2[i][j]`. This is a straightforward element-wise addition with no special edge cases beyond handling any integer values (including negatives), which addition handles naturally. Time complexity is O(4) = O(1) since the matrix size is fixed at 2×2, and space complexity is O(1) for the result matrix (plus the input references). The function parameters are `const` references to prevent accidental modification, and the return type is a fixed-size array that can be directly compared in tests.
