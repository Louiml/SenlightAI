// Write a standalone C++ function that, given a square matrix represented as `std::vector<std::vector<double>>`, returns a `std::vector<double>` containing the sum of each row. The input matrix is guaranteed to be non-empty and square (same number of rows and columns), but it may contain negative values, zeros, and any floating-point numbers. The function must preserve the order of rows, so the i-th element of the returned vector equals the sum of the elements in the i-th row of the input matrix. Do not modify the input matrix; the function should only read from it. Use `double` arithmetic and handle potential floating-point summation issues implicitly (no need for sophisticated stability techniques, just straightforward accumulation).
#include <cassert>
#include <vector>

// Include the solution function here (or link to it).
// For clarity, the function is assumed to be available.

int main() {
    // Test 1: 2x2 matrix with positive numbers
    std::vector<std::vector<double>> m1 = {{1.0, 2.0}, {3.0, 4.0}};
    assert(rowSums(m1) == std::vector<double>({3.0, 7.0}));

    // Test 2: 1x1 matrix
    std::vector<std::vector<double>> m2 = {{-5.0}};
    assert(rowSums(m2) == std::vector<double>({-5.0}));

    // Test 3: Matrix with negative values and zero
    std::vector<std::vector<double>> m3 = {{-1.0, 0.0, 2.0}, {3.0, -4.0, 5.0}, {0.0, 0.0, 0.0}};
    assert(rowSums(m3) == std::vector<double>({1.0, 4.0, 0.0}));

    // Test 4: 3x3 matrix with fractional values
    std::vector<std::vector<double>> m4 = {{0.5, 1.5, 2.0}, {-0.25, 0.25, 0.0}, {1e-3, -1e-3, 1.0}};
    assert(rowSums(m4) == std::vector<double>({4.0, 0.0, 1.0}));

    // Test 5: Check original matrix is unchanged (const correctness)
    std::vector<std::vector<double>> original = {{1.0, 2.0}, {3.0, 4.0}};
    rowSums(original);
    assert(original[0][0] == 1.0 && original[1][1] == 4.0);

    return 0;
}
#include <vector>

// Return a vector containing the sum of each row of a square matrix.
// The input matrix is not modified.
std::vector<double> rowSums(const std::vector<std::vector<double>>& matrix) {
    std::vector<double> sums;
    sums.reserve(matrix.size());

    for (const auto& row : matrix) {
        double rowSum = 0.0;
        for (double value : row) {
            rowSum += value;
        }
        sums.push_back(rowSum);
    }

    return sums;
}
// The solution iterates over each row of the matrix. For each row, we compute the sum by iterating over its columns and adding each element to an accumulator that starts at `0.0`. Because the matrix is square and non-empty, there is at least one row and one column. Edge cases include rows with a single element (where the sum is just that element), negative values (which must be added correctly), and all-zero rows (sum is `0.0`). The main algorithm is simple: for each row, reset the sum to `0.0`, then for each column value, add it to the sum, and finally push the sum into the result vector. Time complexity is \(O(n^2)\) for an \(n \times n\) matrix, since we visit every element exactly once. Space complexity is \(O(n)\) for the output vector (ignoring the input storage). The implementation uses `const` references to avoid copying the matrix, and `std::accumulate` can be used for clarity, but a manual loop is equally fine. Since the input is square, we can safely assume the row size is constant.
