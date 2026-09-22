Write a standalone C++ function named `horizontalMirrorFilter` that takes a 2D array represented as a vector of vectors of integers (with all rows having the same length) and returns a new 2D vector where each row is transformed by replacing each element with the maximum of the element and the element at the mirrored position (i.e., at index `(cols - 1 - j)` for column `j`). The transformation must operate independently on each row and preserve the original input (do not modify the input vector). The function should handle empty input (a vector with zero rows or zero columns) by returning an empty vector. The result must be computed only using integer arithmetic, and the function must be `const`-correct: the input parameter should be passed as `const std::vector<std::vector<int>>&`. The function signature should be `std::vector<std::vector<int>> horizontalMirrorFilter(const std::vector<std::vector<int>>& matrix)`.

#include <cassert>
#include <vector>

// The solution function is declared above. This test harness calls it.
int main() {
    // Test 1: Simple 2x3 matrix
    std::vector<std::vector<int>> m1 = {{1, 2, 3}, {4, 5, 6}};
    std::vector<std::vector<int>> r1 = horizontalMirrorFilter(m1);
    std::vector<std::vector<int>> expected1 = {{3, 2, 3}, {6, 5, 6}};
    assert(r1 == expected1);

    // Test 2: Odd number of columns, center unchanged
    std::vector<std::vector<int>> m2 = {{7, 8, 9, 10, 11}};
    std::vector<std::vector<int>> r2 = horizontalMirrorFilter(m2);
    std::vector<std::vector<int>> expected2 = {{11, 10, 9, 10, 11}};
    assert(r2 == expected2);

    // Test 3: All elements equal
    std::vector<std::vector<int>> m3 = {{5, 5}, {5, 5}};
    std::vector<std::vector<int>> r3 = horizontalMirrorFilter(m3);
    assert(r3 == m3);

    // Test 4: Single row, single column
    std::vector<std::vector<int>> m4 = {{42}};
    std::vector<std::vector<int>> r4 = horizontalMirrorFilter(m4);
    assert(r4 == std::vector<std::vector<int>>{{42}});

    // Test 5: Empty matrix (zero rows)
    std::vector<std::vector<int>> m5;
    assert(horizontalMirrorFilter(m5).empty());

    // Test 6: Matrix with zero columns
    std::vector<std::vector<int>> m6 = {{}, {}};
    assert(horizontalMirrorFilter(m6).empty());

    // Test 7: Negative values
    std::vector<std::vector<int>> m7 = {{-1, -2, -3}, {-4, -5, -6}};
    std::vector<std::vector<int>> r7 = horizontalMirrorFilter(m7);
    std::vector<std::vector<int>> expected7 = {{-1, -2, -1}, {-4, -5, -4}};
    assert(r7 == expected7);

    // Test 8: Original input is not modified
    std::vector<std::vector<int>> m8 = {{1, 2}, {3, 4}};
    std::vector<std::vector<int>> original = m8;
    auto r8 = horizontalMirrorFilter(m8);
    assert(m8 == original);
    assert(r8 == std::vector<std::vector<int>>{{2, 2}, {4, 4}});

    // Test 9: Large row length with symmetric pattern
    std::vector<std::vector<int>> m9 = {{1, 5, 3, 9, 2}};
    std::vector<std::vector<int>> r9 = horizontalMirrorFilter(m9);
    assert(r9 == std::vector<std::vector<int>>{{2, 9, 3, 9, 2}});

    // Test 10: Multiple rows with different patterns
    std::vector<std::vector<int>> m10 = {{10, 20}, {30, 40}, {50, 60}};
    std::vector<std::vector<int>> r10 = horizontalMirrorFilter(m10);
    assert(r10 == std::vector<std::vector<int>>{{20, 20}, {40, 40}, {60, 60}});

    return 0;
}

#include <vector>
#include <algorithm>

// Returns a new matrix where each element at (row, col) is the maximum
// of the original element and the element at the mirrored column position.
// Input is not modified; empty input returns empty output.
std::vector<std::vector<int>> horizontalMirrorFilter(const std::vector<std::vector<int>>& matrix) {
    if (matrix.empty() || matrix[0].empty()) {
        return {};
    }

    const size_t rows = matrix.size();
    const size_t cols = matrix[0].size();

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            size_t mirror = cols - 1 - j;
            result[i][j] = std::max(matrix[i][j], matrix[i][mirror]);
        }
    }

    return result;
}

// The main algorithm is straightforward: iterate over each row of the input matrix, and for each column index `j`, compute the mirrored column index `mirror = cols - 1 - j`. The output value at `(row, j)` is `max(matrix[row][j], matrix[row][mirror])`. Since the operation is symmetric (when `j` and `mirror` are swapped, the max is identical), the result is symmetric across the vertical center line. For each row, we can iterate over `j` from 0 to `cols-1`, compute the max, and store it. We do not need to handle odd or even column counts differently; when `cols` is odd, the center element's mirror is itself, so the max is just that element. Edge cases: empty matrix (zero rows) or rows with zero columns — handle by returning an empty vector. If rows have inconsistent lengths, the problem statement guarantees all rows have the same length, so we can assume that; but to be safe, we could assert that but the task says all rows have same length, so no special handling needed. Time complexity: \(O(R \times C)\) where \(R\) is number of rows and \(C\) is columns. Space complexity: \(O(R \times C)\) for the output, ignoring input. This is optimal because we must produce the output.
