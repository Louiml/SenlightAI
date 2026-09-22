// Write a C++ function that accepts a 3x3 matrix of floating-point values (using `std::array<std::array<double,3>,3>` or a custom simple struct) and returns a `std::array<std::array<double,3>,3>` where each row is replaced by the maximum absolute value of the original row replicated across that row. In other words, for each row, find the element with the largest absolute value, and set every element in that row of the output to that value. You may assume the matrix contains only finite values (no NaN or infinity). The function must be `const`-correct, take the input by const reference, and not modify the input. Do not use external matrix libraries; implement all logic manually.
#include <cassert>
#include <cmath>

int main() {
    std::array<std::array<double, 3>, 3> m1 = {{{1.0, -2.0, 3.0},
                                                 {0.5, -0.5, 2.0},
                                                 {-4.0, 1.0, 1.0}}};
    auto r1 = maxAbsPerRowReplicated(m1);
    for (int row = 0; row < 3; ++row) {
        double expected = 0.0;
        for (int col = 0; col < 3; ++col) {
            expected = std::max(expected, std::abs(m1[row][col]));
        }
        for (int col = 0; col < 3; ++col) {
            assert(r1[row][col] == expected);
        }
    }

    std::array<std::array<double, 3>, 3> m2 = {{{0.0, 0.0, 0.0},
                                                 {-1.0, -2.0, -3.0},
                                                 {7.0, -7.0, 0.0}}};
    auto r2 = maxAbsPerRowReplicated(m2);
    assert(r2[0][0] == 0.0 && r2[0][1] == 0.0 && r2[0][2] == 0.0);
    assert(r2[1][0] == 3.0 && r2[1][1] == 3.0 && r2[1][2] == 3.0);
    assert(r2[2][0] == 7.0 && r2[2][1] == 7.0 && r2[2][2] == 7.0);

    std::array<std::array<double, 3>, 3> m3 = {{{-5.0, 4.0, 4.0},
                                                 {1.0, 2.0, 3.0},
                                                 {-10.0, -9.0, -8.0}}};
    auto r3 = maxAbsPerRowReplicated(m3);
    assert(r3[0][0] == 5.0 && r3[0][1] == 5.0 && r3[0][2] == 5.0);
    assert(r3[1][0] == 3.0 && r3[1][1] == 3.0 && r3[1][2] == 3.0);
    assert(r3[2][0] == 10.0 && r3[2][1] == 10.0 && r3[2][2] == 10.0);
}
#include <array>
#include <cmath>
#include <algorithm>

// Replace each row of a 3x3 matrix with the row's maximum absolute value replicated across that row.
std::array<std::array<double, 3>, 3> maxAbsPerRowReplicated(
    const std::array<std::array<double, 3>, 3>& input) {
    
    std::array<std::array<double, 3>, 3> result{};
    
    for (std::size_t row = 0; row < 3; ++row) {
        // Find maximum absolute value in current row.
        double max_abs = 0.0;
        for (std::size_t col = 0; col < 3; ++col) {
            max_abs = std::max(max_abs, std::abs(input[row][col]));
        }
        // Replicate the found value across the output row.
        result[row].fill(max_abs);
    }
    
    return result;
}
// The solution iterates over each of the three rows independently. For each row, we track the maximum absolute value by scanning the three elements, using `std::abs` for magnitude. Since we need to replicate the found value across the entire output row, we initialize an output row with that value (e.g., by filling a `std::array<double,3>`). The main algorithm is straightforward with a nested loop: outer loop over rows (3 iterations), inner loop over columns (3 iterations) to find max abs, then fill the output row. Edge cases: all values are positive/negative, zeros, equal absolute values (any is fine). Time complexity is O(3*3) = O(1) since the matrix size is fixed; space complexity is O(1) for the output matrix (excluding the input reference). No dynamic allocation is needed.
