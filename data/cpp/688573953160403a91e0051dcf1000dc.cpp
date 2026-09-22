Write a C++ function that computes the 1-norm (maximum absolute column sum) of a real symmetric tridiagonal matrix stored in compact form, given its diagonal entries `d[0..n-1]` and sub/super-diagonal entries `e[0..n-2]` (where `e[i]` is the off-diagonal between row/column `i` and `i+1`). The function should take `n` (matrix size), the diagonal array `d`, and the off-diagonal array `e` as parameters, and return the 1-norm as a `double`. Handle edge cases: `n <= 0` should return `0.0`, and `n == 1` should return `abs(d[0])`. For `n >= 2`, compute the sum of absolute values in each column: column 0 is `|d[0]| + |e[0]|`, column `i` (1 ≤ i ≤ n-2) is `|e[i-1]| + |d[i]| + |e[i]|`, and column n-1 is `|e[n-2]| + |d[n-1]|`. Return the maximum of these column sums.
// The 1-norm of a matrix is the maximum absolute column sum. Since the matrix is symmetric tridiagonal, column `j` contains at most three non-zero entries: the diagonal `d[j]`, and the off-diagonals `e[j-1]` (if j > 0) and `e[j]` (if j < n-1). For a tridiagonal symmetric matrix, the 1-norm equals the infinity-norm (because it's symmetric), but the task explicitly asks for the 1-norm, which here is implemented by summing absolute values per column. The algorithm iterates over columns: for the first column, only `d[0]` and `e[0]`; for the last column, `e[n-2]` and `d[n-1]`; for interior columns, `e[i-1] + d[i] + e[i]`. The maximum is tracked. Edge cases: if n <= 0, treat as empty matrix and return 0; if n == 1, only `d[0]` matters. The solution runs in O(n) time and O(1) auxiliary space. The implementation should use `std::abs` for absolute value, and carefully index the arrays to avoid out-of-bounds access. The function signature should be `double tridiagonal_1norm(int n, const std::vector<double>& d, const std::vector<double>& e)` to be safe with modern C++.
#include <vector>
#include <cmath>
#include <algorithm>

// Computes the 1-norm (maximum absolute column sum) of a real symmetric tridiagonal matrix.
// d: diagonal entries (length n), e: off-diagonal entries (length n-1).
// Returns 0.0 if n <= 0.
double tridiagonal_1norm(int n, const std::vector<double>& d, const std::vector<double>& e) {
    if (n <= 0) {
        return 0.0;
    }
    if (n == 1) {
        return std::abs(d[0]);
    }

    double max_col_sum = 0.0;

    // Column 0: |d[0]| + |e[0]|
    double col_sum = std::abs(d[0]) + std::abs(e[0]);
    max_col_sum = col_sum;

    // Interior columns 1..n-2: |e[i-1]| + |d[i]| + |e[i]|
    for (int i = 1; i < n - 1; ++i) {
        col_sum = std::abs(e[i - 1]) + std::abs(d[i]) + std::abs(e[i]);
        max_col_sum = std::max(max_col_sum, col_sum);
    }

    // Last column (n-1): |e[n-2]| + |d[n-1]|
    col_sum = std::abs(e[n - 2]) + std::abs(d[n - 1]);
    max_col_sum = std::max(max_col_sum, col_sum);

    return max_col_sum;
}
#include <cassert>
#include <vector>
#include <cmath>

// Declaration of the function under test (included here for completeness).
double tridiagonal_1norm(int n, const std::vector<double>& d, const std::vector<double>& e);

int main() {
    // Test 1: n=0 should return 0.0
    std::vector<double> d_empty;
    std::vector<double> e_empty;
    assert(std::fabs(tridiagonal_1norm(0, d_empty, e_empty) - 0.0) < 1e-12);

    // Test 2: n=1, single diagonal
    std::vector<double> d1 = {5.0};
    std::vector<double> e1;
    assert(std::fabs(tridiagonal_1norm(1, d1, e1) - 5.0) < 1e-12);

    // Test 3: n=2, simple 2x2 matrix [1, -2; -2, 3]
    // Columns: col0 = |1|+|-2| = 3, col1 = |-2|+|3| = 5 => max 5
    std::vector<double> d2 = {1.0, 3.0};
    std::vector<double> e2 = {-2.0};
    assert(std::fabs(tridiagonal_1norm(2, d2, e2) - 5.0) < 1e-12);

    // Test 4: n=3, matrix with diagonals [1, -2, 4] and off-diagonals [3, -1]
    // Columns: col0 = |1|+|3| = 4, col1 = |3|+|-2|+|-1| = 6, col2 = |-1|+|4| = 5 => max 6
    std::vector<double> d3 = {1.0, -2.0, 4.0};
    std::vector<double> e3 = {3.0, -1.0};
    assert(std::fabs(tridiagonal_1norm(3, d3, e3) - 6.0) < 1e-12);

    // Test 5: n=4, all positive, symmetric
    std::vector<double> d4 = {2.0, 3.0, 1.0, 5.0};
    std::vector<double> e4 = {1.0, 2.0, 4.0};
    // Columns: col0=2+1=3, col1=1+3+2=6, col2=2+1+4=7, col3=4+5=9 => max 9
    assert(std::fabs(tridiagonal_1norm(4, d4, e4) - 9.0) < 1e-12);

    // Test 6: n=4, with zeros and negatives
    std::vector<double> d5 = {0.0, -1.0, -2.0, 3.0};
    std::vector<double> e5 = {-3.0, 0.0, 0.5};
    // Columns: col0=0+3=3, col1=3+1+0=4, col2=0+2+0.5=2.5, col3=0.5+3=3.5 => max 4
    assert(std::fabs(tridiagonal_1norm(4, d5, e5) - 4.0) < 1e-12);

    // Test 7: large values, check precision
    std::vector<double> d6 = {1e8, -2e8, 3e8};
    std::vector<double> e6 = {4e8, -5e8};
    // Columns: col0=1e8+4e8=5e8, col1=4e8+2e8+5e8=11e8, col2=5e8+3e8=8e8 => max 1.1e9
    assert(std::fabs(tridiagonal_1norm(3, d6, e6) - 1.1e9) < 1e-3);

    return 0;
}
