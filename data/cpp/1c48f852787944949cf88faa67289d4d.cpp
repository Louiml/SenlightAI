// Write a C++ function `applyLinearTransformToEquations` that, given a matrix `T` (represented as a vector of integer vectors, where each inner vector is a row) and a set of linear equations `rows` (each a vector of integers representing coefficients `a0*x0 + a1*x1 + ... + a_{n-1}*x_{n-1} = c`), returns a new set of equations where each original equation (excluding the constant term `c`) is multiplied on the left by `T`. That is, for each original row `r = [a0, a1, ..., a_{n-1}, c]`, the new row becomes `[ (T * [a0, a1, ..., a_{n-1}]^T)^T , c ]`, i.e., the coefficients are transformed by `T` (matrix-vector multiplication treating the coefficients as a column vector) and the constant term remains unchanged. The function must handle any number of rows (including zero) and any consistent matrix dimensions; assume `T` has `n` columns and `m` rows (where `m` may differ from `n`), and each input row has exactly `n+1` entries (n coefficients plus one constant). The output rows will have `m+1` entries. Ensure the function is const-correct and uses `std::vector<int>` for all data. Provide a standalone implementation with no external dependencies.

#include <cassert>
#include <vector>

// The function is assumed to be declared above.

int main() {
    // Test 1: Basic transformation with 2x2 matrix applied to one equation
    std::vector<std::vector<int>> T1 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> rows1 = {{5, 6, 7}}; // 5x + 6y = 7
    auto res1 = applyLinearTransformToEquations(T1, rows1);
    assert(res1.size() == 1);
    assert(res1[0] == std::vector<int>({1*5 + 2*6, 3*5 + 4*6, 7})); // {17, 39, 7}

    // Test 2: Non-square matrix (3 rows, 2 columns) applied to two equations
    std::vector<std::vector<int>> T2 = {{1, 0}, {0, 1}, {1, 1}};
    std::vector<std::vector<int>> rows2 = {{1, 2, 3}, {4, 5, 6}}; // 1x+2y=3, 4x+5y=6
    auto res2 = applyLinearTransformToEquations(T2, rows2);
    assert(res2.size() == 2);
    assert(res2[0] == std::vector<int>({1, 2, 3, 3})); // {1, 2, 1+2, 3}
    assert(res2[1] == std::vector<int>({4, 5, 9, 6})); // {4, 5, 4+5, 6}

    // Test 3: Zero rows input
    std::vector<std::vector<int>> T3 = {{1, 0}};
    std::vector<std::vector<int>> rows3 = {};
    auto res3 = applyLinearTransformToEquations(T3, rows3);
    assert(res3.empty());

    // Test 4: Matrix with zero columns (n=0), each row has only a constant
    std::vector<std::vector<int>> T4 = {{}, {}}; // 2 rows, 0 columns
    std::vector<std::vector<int>> rows4 = {{42}, {-7}}; // constant-only equations
    auto res4 = applyLinearTransformToEquations(T4, rows4);
    assert(res4.size() == 2);
    assert(res4[0] == std::vector<int>({0, 0, 42})); // coefficients zero, constant kept
    assert(res4[1] == std::vector<int>({0, 0, -7}));

    // Test 5: Matrix with zero rows (m=0), each output row has only constant
    std::vector<std::vector<int>> T5 = {}; // 0 rows
    std::vector<std::vector<int>> rows5 = {{1, 2, 3}}; // still n=2 from T? Actually n=0, but input row has 3 elements? This is invalid; skip.
    // Instead, use T with 0 columns but m=2? Not possible. For m=0, n is undefined; we handle by assuming T[0] doesn't exist.
    // Test a valid case: T5 = {} (m=0), rows have only a constant (n=0)
    std::vector<std::vector<int>> rows5b = {{10}, {20}};
    auto res5 = applyLinearTransformToEquations(T5, rows5b);
    assert(res5.size() == 2);
    assert(res5[0] == std::vector<int>({10})); // no coefficients, just constant
    assert(res5[1] == std::vector<int>({20}));

    // Test 6: Identity matrix
    std::vector<std::vector<int>> T6 = {{1, 0}, {0, 1}};
    std::vector<std::vector<int>> rows6 = {{1, -2, 5}, {0, 3, -1}};
    auto res6 = applyLinearTransformToEquations(T6, rows6);
    assert(res6 == rows6); // identity preserves

    // Test 7: Negative coefficients
    std::vector<std::vector<int>> T7 = {{-1, 0}, {0, -1}};
    std::vector<std::vector<int>> rows7 = {{2, 3, 4}};
    auto res7 = applyLinearTransformToEquations(T7, rows7);
    assert(res7[0] == std::vector<int>({-2, -3, 4}));

    return 0;
}

#include <vector>
#include <cstddef>

// Given a matrix T (rows x columns) and a list of equations (each row has n+1 elements:
// n coefficients followed by a constant), apply T to the coefficient part of each equation.
// Returns a list of transformed equations, each having m+1 elements (m coefficients + constant).
std::vector<std::vector<int>> applyLinearTransformToEquations(
    const std::vector<std::vector<int>>& T,
    const std::vector<std::vector<int>>& rows)
{
    const std::size_t m = T.size();          // number of rows in T
    const std::size_t n = (m == 0) ? 0 : T[0].size(); // number of columns in T

    std::vector<std::vector<int>> result;
    result.reserve(rows.size());

    for (const auto& row : rows) {
        // Assume row has size n+1 (n coefficients + 1 constant)
        const int constant = row.back();

        std::vector<int> newRow(m);
        for (std::size_t j = 0; j < m; ++j) {
            int sum = 0;
            for (std::size_t i = 0; i < n; ++i) {
                sum += T[j][i] * row[i];
            }
            newRow[j] = sum;
        }
        newRow.push_back(constant);
        result.push_back(std::move(newRow));
    }
    return result;
}

// The core operation is a linear transformation applied to the coefficient part of each equation. For each input row `r`, we separate the last element as the constant `c`, and take the first `n` elements as the coefficient vector `v`. The new coefficient vector `w` is computed as `w_j = sum_{i=0}^{n-1} T[j][i] * v[i]` for `j = 0..m-1`. Then we append `c` to `w` to form the new row. This is essentially multiplying the original coefficients (a column vector) by the matrix `T` on the left. If `m` is less than `n`, the transpose trick in the problem statement is not needed—here we simply compute the product directly.
//
// Edge cases include: zero rows (returns empty vector), `n=0` (each row has only a constant; then the new coefficients are all zero, but the constant remains), `m=0` (output rows have only a constant—the sum is empty). Time complexity is O(R * m * n) where R is the number of rows. Space complexity is O(R * m) for the output. We must ensure we treat `T` as a `const` reference and use `size_t` for indexing to avoid signed/unsigned warnings.
