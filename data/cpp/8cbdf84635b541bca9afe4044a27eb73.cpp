/*
Write a C++ function named `accumulateAdjusted` that implements a simplified version of the OpenCV `accumulate` operation. The function must take three parameters: a source matrix (`src`) of type `std::vector<std::vector<int>>`, a destination matrix (`dst`) of type `std::vector<std::vector<double>>` (same dimensions as `src`), and an optional mask matrix (`mask`) of type `std::vector<std::vector<unsigned char>>` (same dimensions, or empty to indicate no mask). The function must update `dst` in-place by adding each corresponding element of `src` (converted to `double`) to `dst`, but only where the mask element is non-zero. If the mask is empty, all elements are accumulated. The function must return `void` and should validate that all matrices have matching dimensions (throw `std::invalid_argument` on mismatch). The operation must be performed element-wise without any external libraries. Consider edge cases like empty matrices and masks containing only zeros.
*/
#include <vector>
#include <stdexcept>

void accumulateAdjusted(
    const std::vector<std::vector<int>>& src,
    std::vector<std::vector<double>>& dst,
    const std::vector<std::vector<unsigned char>>& mask = {}) {
    
    size_t rows = src.size();
    if (rows == 0) {
        if (dst.empty() && mask.empty()) return;
        throw std::invalid_argument("Source matrix is empty but destination or mask is not");
    }
    
    size_t cols = src[0].size();
    if (dst.size() != rows) throw std::invalid_argument("Destination row count mismatch");
    for (const auto& row : dst) {
        if (row.size() != cols) throw std::invalid_argument("Destination column count mismatch");
    }
    
    bool hasMask = !mask.empty();
    if (hasMask) {
        if (mask.size() != rows) throw std::invalid_argument("Mask row count mismatch");
        for (const auto& row : mask) {
            if (row.size() != cols) throw std::invalid_argument("Mask column count mismatch");
        }
    }
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (!hasMask || mask[i][j] != 0) {
                dst[i][j] += static_cast<double>(src[i][j]);
            }
        }
    }
}
#include <cassert>
#include <cmath>

int main() {
    // Basic accumulation without mask
    {
        std::vector<std::vector<int>> src = {{1, 2}, {3, 4}};
        std::vector<std::vector<double>> dst = {{10.0, 20.0}, {30.0, 40.0}};
        accumulateAdjusted(src, dst);
        assert(dst[0][0] == 11.0);
        assert(dst[0][1] == 22.0);
        assert(dst[1][0] == 33.0);
        assert(dst[1][1] == 44.0);
    }
    
    // With mask (only accumulate where mask is non-zero)
    {
        std::vector<std::vector<int>> src = {{1, 2}, {3, 4}};
        std::vector<std::vector<double>> dst = {{1.0, 1.0}, {1.0, 1.0}};
        std::vector<std::vector<unsigned char>> mask = {{1, 0}, {0, 1}};
        accumulateAdjusted(src, dst, mask);
        assert(dst[0][0] == 2.0);
        assert(dst[0][1] == 1.0);
        assert(dst[1][0] == 1.0);
        assert(dst[1][1] == 5.0);
    }
    
    // Empty mask (all zeros) leaves destination unchanged
    {
        std::vector<std::vector<int>> src = {{5, 6}};
        std::vector<std::vector<double>> dst = {{0.5, 0.5}};
        std::vector<std::vector<unsigned char>> mask = {{0, 0}};
        accumulateAdjusted(src, dst, mask);
        assert(dst[0][0] == 0.5);
        assert(dst[0][1] == 0.5);
    }
    
    // Empty matrices (all empty) should not throw
    {
        std::vector<std::vector<int>> src;
        std::vector<std::vector<double>> dst;
        accumulateAdjusted(src, dst);
    }
    
    // Dimension mismatch should throw
    {
        std::vector<std::vector<int>> src = {{1}};
        std::vector<std::vector<double>> dst = {{1.0, 2.0}};
        bool threw = false;
        try {
            accumulateAdjusted(src, dst);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }
    
    // Non-square matrix
    {
        std::vector<std::vector<int>> src = {{1, 2, 3}};
        std::vector<std::vector<double>> dst = {{0.0, 0.0, 0.0}};
        accumulateAdjusted(src, dst);
        assert(dst[0][0] == 1.0);
        assert(dst[0][1] == 2.0);
        assert(dst[0][2] == 3.0);
    }
    
    return 0;
}
// The solution iterates over every element of the matrices using nested loops. For each cell `(i, j)`, if the mask is empty or `mask[i][j] != 0`, then we add `static_cast<double>(src[i][j])` to `dst[i][j]`. The dimensions must match exactly; otherwise, throw an exception. The time complexity is \(O(R \times C)\) where \(R\) is the number of rows and \(C\) is the number of columns, since we must examine every element. The space complexity is \(O(1)\) beyond the input matrices because we only use loop counters and no additional storage. Edge cases include: empty matrices (the function returns immediately without error if dimensions are consistent—e.g., all empty), a mask that is empty (treated as all non-zero), and a mask with all zeros (resulting in `dst` unchanged). The function works for any size, including non-square matrices, as long as all three matrices (when mask is non-empty) share the same shape.
