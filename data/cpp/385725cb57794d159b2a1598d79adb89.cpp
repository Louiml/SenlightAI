/*
Given a square grayscale image represented as a 2D vector of unsigned 8-bit pixel values (0-255) and a positive odd integer `blockSize`, write a C++ function that performs quadtree-based block splitting to reduce image complexity. The function should recursively check each square block of size `blockSize` (or larger, from powers of 2 starting at the full image size down to `blockSize`): if the block's variance (computed as the average squared deviation from the block mean) exceeds a specified threshold, split the block into four equal sub-blocks and recurse; otherwise, mark the entire block as homogeneous. The function should return a single integer count of the total number of leaf blocks (blocks that are not split) in the final quadtree decomposition. The image dimensions are guaranteed to be powers of two and at least as large as `blockSize`, so every split produces exact integer divisions. The variance threshold is a positive double. Note that for a 1×1 block, variance is always zero, so it is always a leaf. The function signature is: `int quadtreeLeafCount(const std::vector<std::vector<unsigned char>>& image, int blockSize, double threshold)`. Assume the image is square with size `n` where `n` is a power of two, and `blockSize` is an odd positive integer dividing `n` (e.g., 1, 3, 5, 7, etc.). Handle edge cases where `blockSize == n` (no splitting possible, only one leaf) and where variance is exactly equal to threshold (should not split; split only if variance > threshold). The recursion must consider only blocks aligned to multiples of their size starting at (0,0).
*/

#include <vector>
#include <cstdint>
#include <cmath>

// Recursively count leaf blocks in a quadtree decomposition of a grayscale image.
// image: square 2D vector of pixel values (0-255), dimensions are powers of two.
// blockSize: minimum block size (odd positive integer dividing image size).
// threshold: variance above which a block is split into four sub-blocks.
// Returns the number of leaf blocks after decomposition.
int quadtreeLeafCount(const std::vector<std::vector<unsigned char>>& image, int blockSize, double threshold) {
    const int n = static_cast<int>(image.size());
    
    // Recursive helper: processes a square block with top-left corner (row, col) and side length size.
    // Returns the leaf count for that block.
    auto countLeaves = [&](int row, int col, int size, auto&& self) -> int {
        // Compute mean and sum of squared deviations for this block.
        uint64_t sum = 0;
        uint64_t sumSq = 0;
        for (int r = row; r < row + size; ++r) {
            for (int c = col; c < col + size; ++c) {
                unsigned char pixel = image[r][c];
                sum += pixel;
                sumSq += static_cast<uint64_t>(pixel) * pixel;
            }
        }
        
        double mean = static_cast<double>(sum) / (size * size);
        double variance = (static_cast<double>(sumSq) / (size * size)) - (mean * mean);
        variance = std::abs(variance); // guard against slight negative due to floating-point rounding
        
        // If variance exceeds threshold and we can split further, split into quadrants.
        if (variance > threshold && size > blockSize) {
            int half = size / 2;
            int count = 0;
            count += self(row, col, half, self);
            count += self(row, col + half, half, self);
            count += self(row + half, col, half, self);
            count += self(row + half, col + half, half, self);
            return count;
        }
        
        // Otherwise, this is a leaf block.
        return 1;
    };
    
    return countLeaves(0, 0, n, countLeaves);
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: 4x4 image with uniform values, blockSize=1, threshold small -> all 16 blocks are leaves.
    std::vector<std::vector<unsigned char>> img1(4, std::vector<unsigned char>(4, 100));
    assert(quadtreeLeafCount(img1, 1, 0.5) == 16);

    // Test 2: 4x4 image with high variance, blockSize=1, large threshold -> entire image is one leaf.
    std::vector<std::vector<unsigned char>> img2 = {
        {0, 255, 0, 255},
        {255, 0, 255, 0},
        {0, 255, 0, 255},
        {255, 0, 255, 0}
    };
    assert(quadtreeLeafCount(img2, 1, 10000.0) == 1);

    // Test 3: 4x4 image, blockSize=2, threshold tiny -> split into four 2x2 leaves.
    // Each 2x2 block has variance 0, so no further split.
    std::vector<std::vector<unsigned char>> img3(4, std::vector<unsigned char>(4, 50));
    assert(quadtreeLeafCount(img3, 2, 0.01) == 4);

    // Test 4: 8x8 image with a single high-variance 4x4 quadrant, blockSize=2, threshold intermediate.
    // Top-left 4x4 has checkerboard pattern (variance high), other three 4x4 are uniform.
    std::vector<std::vector<unsigned char>> img4(8, std::vector<unsigned char>(8, 10));
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            img4[r][c] = ((r + c) % 2 == 0) ? 0 : 255;
    // The uniform 4x4 blocks have variance 0, so they remain leaves. The checkerboard 4x4 has variance > 10, splits into four 2x2 leaves.
    assert(quadtreeLeafCount(img4, 2, 10.0) == 3 + 4); // three uniform 4x4 leaves + four 2x2 leaves

    // Test 5: 2x2 image, blockSize=2, any threshold -> one leaf (no split possible).
    std::vector<std::vector<unsigned char>> img5 = {{1, 2}, {3, 4}};
    assert(quadtreeLeafCount(img5, 2, 0.0) == 1);

    // Test 6: 4x4 image with variance exactly equal to threshold should NOT split.
    // Construct a 2x2 block with variance exactly say 4 (pixels 0,2,2,4 -> mean 2, var = (4+0+0+4)/4=2, not 4).
    // Use a known case: 4x4 with all values 0 and one value 255, variance = (255^2)/16 ≈ 4064. Use threshold=4064 and blockSize=1.
    std::vector<std::vector<unsigned char>> img6(4, std::vector<unsigned char>(4, 0));
    img6[0][0] = 255;
    // Whole image variance = (255^2)/16 = 4064.0625, threshold = 4064.0625, should not split because variance > threshold is false (equal).
    // But actually variance > threshold is false, so one leaf.
    assert(quadtreeLeafCount(img6, 1, 4064.0625) == 1);
    // With slightly smaller threshold, it splits.
    assert(quadtreeLeafCount(img6, 1, 4064.0) > 1);

    // Test 7: 8x8 uniform image, blockSize=1, threshold=0.0 -> variance is 0, never > 0, so all leaves are size 8? No, because size > blockSize and variance > threshold is false, so the whole image is one leaf.
    std::vector<std::vector<unsigned char>> img7(8, std::vector<unsigned char>(8, 7));
    assert(quadtreeLeafCount(img7, 1, 0.0) == 1);

    // Test 8: 8x8 with alternating rows, blockSize=1, threshold=0 -> every 1x1 block has variance 0, so whole image is one leaf? Actually each 1x1 variance 0, but the root 8x8 variance >0 splits until 1x1, so leaves = 64.
    std::vector<std::vector<unsigned char>> img8(8, std::vector<unsigned char>(8));
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            img8[r][c] = (r % 2 == 0) ? 100 : 200;
    assert(quadtreeLeafCount(img8, 1, 0.0) == 64);

    return 0;
}

// The problem requires a recursive quadtree decomposition: starting from the full image, check if a block's variance exceeds the threshold. Variance is computed as the average over all pixels of `(pixel - mean)^2`, where mean is the arithmetic mean of pixel values in that block. Since pixels are unsigned 8-bit, compute sums using wider types (e.g., `uint64_t` for sum and sum of squares to avoid overflow, then convert to double for variance). If variance > threshold and the block size is larger than `blockSize`, split into four quadrants and recurse; otherwise, count this block as a leaf. Because the image size is a power of two and `blockSize` divides it, every recursive split produces exact integer halves. The base case is when the block size equals `blockSize`, at which point variance (possibly zero) is always ≤ threshold if threshold > 0, so it's a leaf. For a block of size `s`, compute variance in \(O(s^2)\) time. The recursion visits each leaf block exactly once, and each split adds four children. The total number of nodes processed is proportional to the number of leaves times a constant factor (since each split creates four children, the total nodes is at most 5/4 times the leaves for a full quadtree). Thus, the time complexity is \(O(n^2)\) for the whole image, as every pixel is examined at each level it belongs to; in the worst case (all leaves at minimum size), each pixel appears in \(O(\log_2(n/blockSize))\) levels, giving \(O(n^2 \log(n/blockSize))\). Auxiliary space is \(O(\log(n/blockSize))\) for the recursion stack. Important edge cases: variance exactly equal to threshold should not split; block size 1 always leaf; if the threshold is extremely small, all blocks may split until minimum size; if the threshold is huge, the whole image is one leaf.
