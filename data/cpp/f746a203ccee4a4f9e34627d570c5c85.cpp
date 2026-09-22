/*
Write a C++ function named `extractCovarianceBlock` that takes a row-major covariance matrix stored as a `std::vector<double>` (of size `N*N` where `N = 6`, representing a 6×6 covariance matrix for a 3D pose and 3D orientation) and returns a `std::vector<double>` containing the 3×3 position covariance block extracted from the first three rows and columns. The input vector must have exactly 36 elements; if not, the function should throw a `std::invalid_argument` exception. The output should be a 9-element vector in row-major order, corresponding to the upper-left 3×3 submatrix of the 6×6 covariance matrix. The function must not modify the input, must be `const`-correct, and must use appropriate bounds checking. The solution should be standalone, using only standard C++ libraries (e.g., `<vector>`, `<stdexcept>`), and must not rely on any external dependencies or ROS-specific types.
*/
#include <vector>
#include <stdexcept>

/**
 * Extracts the 3x3 position covariance block from a 6x6 covariance matrix
 * stored as a flat row-major vector of 36 doubles.
 *
 * @param cov A vector of exactly 36 elements representing a 6x6 covariance matrix.
 * @return A vector of 9 doubles (3x3 row-major) containing the upper-left block.
 * @throws std::invalid_argument if cov.size() != 36.
 */
std::vector<double> extractCovarianceBlock(const std::vector<double>& cov) {
    if (cov.size() != 36) {
        throw std::invalid_argument("Covariance matrix must have exactly 36 elements (6x6).");
    }

    std::vector<double> block;
    block.reserve(9);

    // Extract rows 0..2 and columns 0..2 from the 6x6 matrix.
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            block.push_back(cov[r * 6 + c]);
        }
    }

    return block;
}
#include <cassert>
#include <vector>
#include <stdexcept>

// Function declaration (as per solution)
std::vector<double> extractCovarianceBlock(const std::vector<double>& cov);

int main() {
    // Test 1: Identity 6x6 matrix -> 3x3 identity block
    std::vector<double> identity(36, 0.0);
    for (int i = 0; i < 6; ++i) {
        identity[i * 6 + i] = 1.0;
    }
    std::vector<double> block = extractCovarianceBlock(identity);
    assert(block.size() == 9);
    for (int i = 0; i < 9; ++i) {
        if (i % 4 == 0) { // diagonal positions 0,4,8 in 3x3
            assert(block[i] == 1.0);
        } else {
            assert(block[i] == 0.0);
        }
    }

    // Test 2: Arbitrary values in the 6x6, verify positions are correctly copied
    std::vector<double> cov(36);
    for (int i = 0; i < 36; ++i) {
        cov[i] = static_cast<double>(i + 1); // Fill with 1..36
    }
    block = extractCovarianceBlock(cov);
    // Expected block: rows 0..2, cols 0..2 from cov (indices 0,1,2 / 6,7,8 / 12,13,14)
    assert(block[0] == 1.0);
    assert(block[1] == 2.0);
    assert(block[2] == 3.0);
    assert(block[3] == 7.0);
    assert(block[4] == 8.0);
    assert(block[5] == 9.0);
    assert(block[6] == 13.0);
    assert(block[7] == 14.0);
    assert(block[8] == 15.0);

    // Test 3: All zeros -> all zeros
    std::vector<double> zeros(36, 0.0);
    block = extractCovarianceBlock(zeros);
    for (double val : block) {
        assert(val == 0.0);
    }

    // Test 4: Incorrect size throws exception
    std::vector<double> wrong_size(35, 0.0);
    bool threw = false;
    try {
        extractCovarianceBlock(wrong_size);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 5: Non-symmetric matrix (should still extract upper-left)
    std::vector<double> non_sym(36, 5.0);
    non_sym[0] = -1.0; non_sym[1] = 2.0; non_sym[2] = 3.0;
    non_sym[6] = 4.0; non_sym[7] = -5.0; non_sym[8] = 6.0;
    non_sym[12] = 7.0; non_sym[13] = 8.0; non_sym[14] = -9.0;
    block = extractCovarianceBlock(non_sym);
    assert(block[0] == -1.0);
    assert(block[1] == 2.0);
    assert(block[2] == 3.0);
    assert(block[3] == 4.0);
    assert(block[4] == -5.0);
    assert(block[5] == 6.0);
    assert(block[6] == 7.0);
    assert(block[7] == 8.0);
    assert(block[8] == -9.0);

    return 0;
}
// The core task is straightforward: given a 6×6 matrix stored in a flat 36-element vector in row-major order (index `i*6 + j` for row `i`, column `j`), we need to extract the 3×3 submatrix formed by rows 0..2 and columns 0..2. The main challenge is to correctly index into the flat vector. For the extracted 3×3 output (index `r*3 + c`), we map to the input index `r*6 + c` where `r` and `c` range from 0 to 2. The algorithm runs in constant time: iterate `r` from 0 to 2 and `c` from 0 to 2, pushing `cov[r*6 + c]` into the result. Edge cases: if the input size is not exactly 36, throw an exception. Since the matrix is symmetric (covariance), we could also just copy the upper-left block without symmetry assumptions; the extraction is purely positional. No other special cases arise. Time complexity is O(1) (fixed 9 elements), space complexity is O(1) for the result (9 doubles), not counting the input.
