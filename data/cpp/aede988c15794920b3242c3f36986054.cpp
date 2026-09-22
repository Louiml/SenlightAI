// Write a C++ function that reads a sequence of square matrices (each of size \(y \times y\)) from standard input, where the first value `n` indicates the number of matrices to process. For each matrix, compute the sum of each column and output the results in the exact format: `Case #<index>: <sum1> <sum2> ... <sumy>` (with a single space between sums, no trailing space, and a newline after each case). The matrix dimensions can vary between cases, and all matrix entries are integers that may be negative or large (up to \(10^9\)). The function should not read from `std::cin` itself; instead, it should accept the number of matrices and an already-parsed vector of matrices, then return a vector of formatted strings, one per case.

#include <cassert>
#include <vector>
#include <string>

// Function declaration (must match solution)
std::vector<std::string> columnSumCases(
    const std::vector<std::vector<std::vector<long long>>>& matrices);

int main() {
    // Test 1: Single 1x1 matrix
    std::vector<std::vector<std::vector<long long>>> m1 = {{{5}}};
    auto r1 = columnSumCases(m1);
    assert(r1.size() == 1);
    assert(r1[0] == "Case #1: 5");

    // Test 2: Single 2x2 matrix with mixed signs
    std::vector<std::vector<std::vector<long long>>> m2 = {
        {{1, -2}, {3, 4}}
    };
    auto r2 = columnSumCases(m2);
    assert(r2.size() == 1);
    assert(r2[0] == "Case #1: 4 2");

    // Test 3: Two matrices, different sizes
    std::vector<std::vector<std::vector<long long>>> m3 = {
        {{1, 2}, {3, 4}},   // 2x2 -> column sums 4,6
        {{10, -5, 0}, {1, 2, 3}, {0, 0, 7}}  // 3x3 -> column sums 11,-3,10
    };
    auto r3 = columnSumCases(m3);
    assert(r3.size() == 2);
    assert(r3[0] == "Case #1: 4 6");
    assert(r3[1] == "Case #2: 11 -3 10");

    // Test 4: Large values to ensure long long
    std::vector<std::vector<std::vector<long long>>> m4 = {
        {{1000000000, 1000000000}, {1000000000, 1000000000}}
    };
    auto r4 = columnSumCases(m4);
    assert(r4.size() == 1);
    assert(r4[0] == "Case #1: 2000000000 2000000000");

    // Test 5: Single row matrix? (but per spec square, so valid y=1)
    std::vector<std::vector<std::vector<long long>>> m5 = {{{-7}}};
    auto r5 = columnSumCases(m5);
    assert(r5[0] == "Case #1: -7");

    return 0;
}

#include <string>
#include <vector>
#include <sstream>

// Compute column-wise sums for each matrix and return formatted case strings.
// Each matrix is a vector of rows, each row a vector of long long.
std::vector<std::string> columnSumCases(
    const std::vector<std::vector<std::vector<long long>>>& matrices) {
    std::vector<std::string> results;
    results.reserve(matrices.size());

    for (size_t case_idx = 0; case_idx < matrices.size(); ++case_idx) {
        const auto& matrix = matrices[case_idx];
        const size_t y = matrix.size();

        // Compute column sums
        std::vector<long long> col_sums(y, 0);
        for (size_t col = 0; col < y; ++col) {
            long long total = 0;
            for (size_t row = 0; row < y; ++row) {
                total += matrix[row][col];
            }
            col_sums[col] = total;
        }

        // Format output
        std::ostringstream oss;
        oss << "Case #" << (case_idx + 1) << ":";
        for (size_t j = 0; j < y; ++j) {
            oss << " " << col_sums[j];
        }
        results.push_back(oss.str());
    }
    return results;
}

// The solution processes each matrix independently. For a given \(y \times y\) matrix, the column sums are computed by iterating over each column index `j` and adding all `matrix[i][j]` for `i` from 0 to \(y-1\). Because column sums can exceed 32-bit integer range, store each sum in `long long`. A key edge case is \(y=1\), where the single column sum is just the element itself. Another edge case is zero or negative matrix sizes—but per the problem statement, \(y\) is positive, so we can assume \(y \ge 1\). For output formatting, we need to join sums with spaces and prefix each line with `Case #<case_number>: `. The time complexity is \(O(n \cdot y^2)\) because we process each of the \(n\) matrices with \(y^2\) elements. Space complexity is \(O(y)\) for the temporary sum array per matrix, plus the output vector of size \(n\). The solution avoids printing directly to stdout, making it more modular and testable.
