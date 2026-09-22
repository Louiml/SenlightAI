// Write a standalone C++ function named `cumulativeSum` that accepts a 2D matrix of numeric values (represented as a `std::vector<std::vector<double>>`) and an integer `dim` (1 for row-wise cumulative sum, 2 for column-wise cumulative sum). The function should return a new matrix of the same dimensions where each element at position `(i, j)` is the sum of all elements along the specified axis from the start up to and including that element. For `dim=1`, the cumulative sum runs along each column (i.e., for each column, each row accumulates the previous rows' values). For `dim=2`, the cumulative sum runs along each row (i.e., for each row, each column accumulates the previous columns' values). The input matrix must be non-empty (at least 1 row and 1 column). Handle any positive dimensions and return the result by value. The function must be `const`-correct, taking the input as a `const` reference.
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
// Test cases verifying both dims and edge cases.

int main() {
    // Test 1: Single element matrix.
    std::vector<std::vector<double>> m1 = {{5.0}};
    auto r1 = cumulativeSum(m1, 1);
    assert(r1.size() == 1 && r1[0].size() == 1 && r1[0][0] == 5.0);

    // Test 2: Column-wise cumulative sum (dim=1) for a 2x3 matrix.
    std::vector<std::vector<double>> m2 = {{1.0, 2.0, 3.0},
                                           {4.0, 5.0, 6.0}};
    auto r2 = cumulativeSum(m2, 1);
    assert(r2[0][0] == 1.0 && r2[1][0] == 5.0);
    assert(r2[0][1] == 2.0 && r2[1][1] == 7.0);
    assert(r2[0][2] == 3.0 && r2[1][2] == 9.0);

    // Test 3: Row-wise cumulative sum (dim=2) for the same matrix.
    auto r3 = cumulativeSum(m2, 2);
    assert(r3[0][0] == 1.0 && r3[0][1] == 3.0 && r3[0][2] == 6.0);
    assert(r3[1][0] == 4.0 && r3[1][1] == 9.0 && r3[1][2] == 15.0);

    // Test 4: Matrix with negative numbers.
    std::vector<std::vector<double>> m4 = {{-1.0, -2.0},
                                           {-3.0, -4.0}};
    auto r4 = cumulativeSum(m4, 1);
    assert(r4[0][0] == -1.0 && r4[1][0] == -4.0);
    assert(r4[0][1] == -2.0 && r4[1][1] == -6.0);

    // Test 5: Single row (1xN) with dim=1 (column-wise) should return same as input.
    std::vector<std::vector<double>> m5 = {{10.0, 20.0, 30.0}};
    auto r5 = cumulativeSum(m5, 1);
    assert(r5[0][0] == 10.0 && r5[0][1] == 20.0 && r5[0][2] == 30.0);

    // Test 6: Single column (Nx1) with dim=2 (row-wise) should return same as input.
    std::vector<std::vector<double>> m6 = {{10.0}, {20.0}, {30.0}};
    auto r6 = cumulativeSum(m6, 2);
    assert(r6[0][0] == 10.0 && r6[1][0] == 20.0 && r6[2][0] == 30.0);

    // Test 7: Larger matrix with mixed values.
    std::vector<std::vector<double>> m7 = {{1.0, 2.0, 3.0, 4.0},
                                           {5.0, 6.0, 7.0, 8.0},
                                           {9.0, 10.0, 11.0, 12.0}};
    auto r7 = cumulativeSum(m7, 2); // row-wise
    assert(r7[2][0] == 9.0);
    assert(r7[2][1] == 19.0);
    assert(r7[2][2] == 30.0);
    assert(r7[2][3] == 42.0);

    // Test 8: All zeros.
    std::vector<std::vector<double>> m8 = {{0.0, 0.0}, {0.0, 0.0}};
    auto r8 = cumulativeSum(m8, 1);
    assert(r8[0][0] == 0.0 && r8[1][1] == 0.0);

    return 0;
}
#include <vector>
#include <cstddef>

// Compute the cumulative sum along a specified axis of a 2D matrix.
// dim == 1: cumulative sum along each column (down the rows).
// dim == 2: cumulative sum along each row (across the columns).
// The input matrix must be non-empty and rectangular.
std::vector<std::vector<double>> cumulativeSum(
    const std::vector<std::vector<double>>& matrix,
    int dim) {
    const std::size_t rows = matrix.size();
    const std::size_t cols = matrix[0].size();
    
    // Initialize output with the same size as input.
    std::vector<std::vector<double>> result(rows, std::vector<double>(cols, 0.0));
    
    if (dim == 1) {
        // Column-wise cumulative sum.
        for (std::size_t c = 0; c < cols; ++c) {
            double running_sum = 0.0;
            for (std::size_t r = 0; r < rows; ++r) {
                running_sum += matrix[r][c];
                result[r][c] = running_sum;
            }
        }
    } else {
        // Row-wise cumulative sum (dim == 2).
        for (std::size_t r = 0; r < rows; ++r) {
            double running_sum = 0.0;
            for (std::size_t c = 0; c < cols; ++c) {
                running_sum += matrix[r][c];
                result[r][c] = running_sum;
            }
        }
    }
    
    return result;
}
// The solution iterates over the matrix in a straightforward manner, keyed by the specified axis. For `dim==1` (column-wise accumulation), we loop over each column (outer loop) and then over each row (inner loop), maintaining a running sum that starts at 0 and adds the current element, assigning the running sum to the output cell. For `dim==2` (row-wise accumulation), we loop over each row (outer loop) and then over each column (inner loop), similarly maintaining a running sum. Edge cases: empty input is not allowed per specification, and the matrix is guaranteed rectangular (all rows have the same column count). Behavior for invalid `dim` values is undefined per task, but we assume `dim` is 1 or 2. Time complexity is O(rows × columns) because each element is visited exactly once. Space complexity is O(rows × columns) for the output matrix (which is required), plus O(1) auxiliary space for the running sum and loop indices.
