// Write a C++ function that takes a fixed-size 3×3 matrix of double values (provided as a `std::array<std::array<double,3>,3>` or a simple 2D array) and returns a `std::array<double,3>` containing the Euclidean norm (square root of the sum of squares) of each row, in the same order as the rows. The function must compute the norm directly (not using Eigen or any external library) and handle any real-valued entries, including negatives and zero. The result should be computed with reasonable numerical accuracy (use `std::sqrt` and double arithmetic). The function must be `const`-correct: it must not modify the input matrix. If the matrix contains NaN or infinity, the behavior is undefined; you may assume inputs are finite.

#include <cassert>
#include <cmath>
#include <array>

// Function prototype from the solution
std::array<double, 3> rowNorms(const std::array<std::array<double, 3>, 3>& matrix);

int main() {
    // Test 1: Identity matrix – each row norm is 1.0
    std::array<std::array<double, 3>, 3> id = {{{1,0,0},{0,1,0},{0,0,1}}};
    auto norms = rowNorms(id);
    for (int i = 0; i < 3; ++i) {
        assert(std::fabs(norms[i] - 1.0) < 1e-12);
    }

    // Test 2: All negative values – squaring makes them positive
    std::array<std::array<double, 3>, 3> neg = {{{-3,-4,-0},{-0,-5,-12},{ -1,-2,-2}}};
    norms = rowNorms(neg);
    assert(std::fabs(norms[0] - 5.0) < 1e-12); // sqrt(9+16+0)=5
    assert(std::fabs(norms[1] - 13.0) < 1e-12); // sqrt(0+25+144)=13
    assert(std::fabs(norms[2] - 3.0) < 1e-12); // sqrt(1+4+4)=3

    // Test 3: Zero matrix – all norms are 0
    std::array<std::array<double, 3>, 3> zero = {{{0,0,0},{0,0,0},{0,0,0}}};
    norms = rowNorms(zero);
    for (int i = 0; i < 3; ++i) {
        assert(std::fabs(norms[i]) < 1e-12);
    }

    // Test 4: Mixed positive/negative and decimals
    std::array<std::array<double, 3>, 3> mixed = {{{1.5, -2.0, 0.5}, {0, 0, 3.0}, {-1, 2, -2}}};
    norms = rowNorms(mixed);
    assert(std::fabs(norms[0] - std::sqrt(1.5*1.5 + 4.0 + 0.25)) < 1e-12); // sqrt(6.5)
    assert(std::fabs(norms[1] - 3.0) < 1e-12);
    assert(std::fabs(norms[2] - 3.0) < 1e-12); // sqrt(1+4+4)

    // Test 5: Large numbers – check no overflow for reasonable values
    std::array<std::array<double, 3>, 3> large = {{{1e100, 1e100, 0}, {1e-100, 1e-100, 0}, {2e100, 0, 0}}};
    norms = rowNorms(large);
    assert(std::fabs(norms[0] - std::sqrt(2e200)) < 1e-12 * 1e100); // tolerance scaled
    assert(std::fabs(norms[1] - std::sqrt(2e-200)) < 1e-12);
    assert(std::fabs(norms[2] - 2e100) < 1e-12 * 1e100);

    return 0;
}

#include <array>
#include <cmath>
#include <cstddef>

// Compute the Euclidean norm (sqrt(sum of squares)) of each row of a 3x3 matrix.
// The input is a 3x3 array of doubles (row-major). Returns an array of 3 doubles.
std::array<double, 3> rowNorms(const std::array<std::array<double, 3>, 3>& matrix) {
    std::array<double, 3> norms{};
    for (std::size_t row = 0; row < 3; ++row) {
        double sum_of_squares = 0.0;
        for (std::size_t col = 0; col < 3; ++col) {
            const double value = matrix[row][col];
            sum_of_squares += value * value;
        }
        norms[row] = std::sqrt(sum_of_squares);
    }
    return norms;
}

// The solution iterates over each of the three rows. For each row, it sums the squares of the three elements, then takes the square root using `std::sqrt`. The main algorithm is straightforward: nested loops, outer loop over rows (0 to 2), inner loop over columns (0 to 2). Initialize the sum accumulator to `0.0` for each row, add `element * element` (or `element * element` with `std::pow` but multiplication is more efficient and avoids overflow for typical values). After finishing the inner loop, store `std::sqrt(sum)` in the result array at the corresponding row index. Edge cases: negative values become positive when squared; zero contributes nothing; very large or very small values may cause overflow of the squared sum, but for typical finite double inputs within reasonable range (e.g., up to ~1e154) this is fine; if overflow occurs, `sum` becomes `inf`, and `sqrt(inf)` is `inf`, which is acceptable for the task constraints. Time complexity is O(1) since matrix size is fixed (3×3 = 9 operations). Space complexity is O(1), only a result array of size 3 and a few local variables.
