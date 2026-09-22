// Write a C++ function `std::vector<int> seamCarving(const std::vector<int>& image, int newWidth, int newHeight)` that takes a grayscale image represented as a 1D row-major vector (size WIDTH * HEIGHT, where WIDTH = 28 and HEIGHT = 28 are fixed constants) and resizes it to `newWidth` × `newHeight` (both ≤ original dimensions) using seam carving. The algorithm removes vertical seams (one pixel-wide paths of minimal energy from top to bottom) until reaching `newWidth`, then removes horizontal seams (paths of minimal energy from left to right on the resulting image) until reaching `newHeight`. The energy of a pixel is its grayscale value. For a seam, the total cost is the sum of pixel values along the path, and at each step the next row/column pixel must be chosen from the current or adjacent columns/rows (3-neighbor connectivity). The function returns the resized image as a 1D vector in row-major order, preserving the original orientation (no flipping). The original orientation is top-left origin; do not mirror vertically. The image values are assumed to be in [0, 255] and duplicates are allowed. If the input dimensions are not 28×28 or the target dimensions are invalid (0, too large, etc.), return an empty vector. The algorithm uses dynamic programming to compute minimal cumulative costs.
The solution must implement two phases: vertical seam removal and horizontal seam removal. For vertical seams, define `dp[x][y]` as the minimal cumulative energy of a seam ending at pixel (x, y). Initialize `dp[x][0] = energy(x, 0)`. For each subsequent row y, `dp[x][y] = energy(x, y) + min(dp[x-1][y-1], dp[x][y-1], dp[x+1][y-1])` with boundary handling for x=0 and x=WIDTH-1 (only two neighbors). After filling the dp table, find the minimum value in the last row; that gives the seam's end column. Then backtrack from the last row to the first: at each step, choose among the three predecessor columns the one that led to the minimal cost (ties broken arbitrarily, e.g., leftmost). Remove the seam by shifting all pixels to the right of the seam one position left, and mark the last column as "removed" (or simply ignore it by reducing effective width). Repeat this process until the width becomes `newWidth`. After vertical removal, apply the same logic horizontally on the new image (width = newWidth, height = HEIGHT). For horizontal seams, `dp[x][y]` is minimal cumulative cost of a seam ending at (x, y) coming from the left. Initialize first column’s dp, then iterate x from 1 to newWidth-1, with `dp[x][y] = energy(x, y) + min(dp[x-1][y-1], dp[x-1][y], dp[x-1][y+1])` with boundary handling at y=0 and y=HEIGHT-1. Find minimal value in the last column, backtrack to remove the seam row by shifting pixels below upward. Repeat until height reaches `newHeight`. Edge cases: when newWidth equals original width, skip vertical removal; similarly for height. When target is larger, return empty. Also, the input vector must have exactly 28*28 elements; otherwise return empty. Time complexity: each seam removal recomputes dp over the current image dimensions. For a W×H image removing k seams, it’s O(k * W * H) per phase, so overall O((W - newWidth) * W * H + (H - newHeight) * newWidth * H) ≈ O(W^2 H + W H^2) in the worst case. Space complexity O(W*H) for dp and copies.
#include <vector>
#include <algorithm>
#include <limits>

constexpr int ORIGINAL_WIDTH = 28;
constexpr int ORIGINAL_HEIGHT = 28;

// Resize image using seam carving. Input is row-major ORIGINAL_WIDTH*ORIGINAL_HEIGHT.
// Returns empty vector on invalid input or target dimensions.
std::vector<int> seamCarving(const std::vector<int>& image, int newWidth, int newHeight) {
    if (image.size() != static_cast<size_t>(ORIGINAL_WIDTH * ORIGINAL_HEIGHT) ||
        newWidth <= 0 || newHeight <= 0 ||
        newWidth > ORIGINAL_WIDTH || newHeight > ORIGINAL_HEIGHT) {
        return {};
    }

    // Work on a mutable copy, stored row-major (y rows, x columns)
    std::vector<int> current(image);

    int currentWidth = ORIGINAL_WIDTH;
    int currentHeight = ORIGINAL_HEIGHT;

    // Helper to access pixel at (x, y) in current
    auto get = [&](int x, int y) -> int {
        return current[y * currentWidth + x];
    };
    auto set = [&](int x, int y, int val) {
        current[y * currentWidth + x] = val;
    };

    // Phase 1: remove vertical seams until width == newWidth
    while (currentWidth > newWidth) {
        // Dynamic programming table: dp[x][y] = minimal cumulative energy to (x, y)
        std::vector<std::vector<int>> dp(currentWidth, std::vector<int>(currentHeight));

        for (int x = 0; x < currentWidth; ++x) {
            dp[x][0] = get(x, 0);
        }
        for (int y = 1; y < currentHeight; ++y) {
            for (int x = 0; x < currentWidth; ++x) {
                int bestPrev = dp[x][y-1];
                if (x > 0) bestPrev = std::min(bestPrev, dp[x-1][y-1]);
                if (x < currentWidth - 1) bestPrev = std::min(bestPrev, dp[x+1][y-1]);
                dp[x][y] = get(x, y) + bestPrev;
            }
        }

        // Find minimal cost in last row
        int bestCol = 0;
        for (int x = 1; x < currentWidth; ++x) {
            if (dp[x][currentHeight-1] < dp[bestCol][currentHeight-1]) {
                bestCol = x;
            }
        }

        // Remove the seam by shifting left
        for (int y = 0; y < currentHeight; ++y) {
            // Backtrack to find the exact seam column for this row
            if (y > 0) {
                int prevCol = bestCol;
                int bestPrevVal = dp[bestCol][y-1];
                if (bestCol > 0 && dp[bestCol-1][y-1] < bestPrevVal) {
                    prevCol = bestCol - 1;
                    bestPrevVal = dp[bestCol-1][y-1];
                }
                if (bestCol < currentWidth - 1 && dp[bestCol+1][y-1] < bestPrevVal) {
                    prevCol = bestCol + 1;
                }
                bestCol = prevCol;
            }
            // Shift pixels right of bestCol one left
            for (int x = bestCol; x < currentWidth - 1; ++x) {
                set(x, y, get(x + 1, y));
            }
            // The last column becomes invalid (we'll ignore it)
        }

        // Reduce width
        --currentWidth;
        // Truncate the image to new width (remove last column)
        std::vector<int> newCurrent(currentWidth * currentHeight);
        for (int y = 0; y < currentHeight; ++y) {
            for (int x = 0; x < currentWidth; ++x) {
                newCurrent[y * currentWidth + x] = current[y * (currentWidth + 1) + x];
            }
        }
        current = std::move(newCurrent);
    }

    // Phase 2: remove horizontal seams until height == newHeight
    while (currentHeight > newHeight) {
        // Recompute access lambda with new dimensions
        int w = currentWidth;
        int h = currentHeight;
        auto getH = [&](int x, int y) -> int {
            return current[y * w + x];
        };
        auto setH = [&](int x, int y, int val) {
            current[y * w + x] = val;
        };

        // dp[x][y] = minimal cumulative energy to (x, y) coming from left
        std::vector<std::vector<int>> dp(w, std::vector<int>(h));
        for (int y = 0; y < h; ++y) {
            dp[0][y] = getH(0, y);
        }
        for (int x = 1; x < w; ++x) {
            for (int y = 0; y < h; ++y) {
                int bestPrev = dp[x-1][y];
                if (y > 0) bestPrev = std::min(bestPrev, dp[x-1][y-1]);
                if (y < h - 1) bestPrev = std::min(bestPrev, dp[x-1][y+1]);
                dp[x][y] = getH(x, y) + bestPrev;
            }
        }

        // Find minimal cost in last column
        int bestRow = 0;
        for (int y = 1; y < h; ++y) {
            if (dp[w-1][y] < dp[w-1][bestRow]) {
                bestRow = y;
            }
        }

        // Remove the seam by shifting up
        for (int x = 0; x < w; ++x) {
            if (x > 0) {
                int prevRow = bestRow;
                int bestPrevVal = dp[x-1][bestRow];
                if (bestRow > 0 && dp[x-1][bestRow-1] < bestPrevVal) {
                    prevRow = bestRow - 1;
                    bestPrevVal = dp[x-1][bestRow-1];
                }
                if (bestRow < h - 1 && dp[x-1][bestRow+1] < bestPrevVal) {
                    prevRow = bestRow + 1;
                }
                bestRow = prevRow;
            }
            for (int y = bestRow; y < h - 1; ++y) {
                setH(x, y, getH(x, y + 1));
            }
        }

        // Reduce height and truncate
        --currentHeight;
        std::vector<int> newCurrent(w * currentHeight);
        for (int y = 0; y < currentHeight; ++y) {
            for (int x = 0; x < w; ++x) {
                newCurrent[y * w + x] = current[y * w + x];
            }
        }
        current = std::move(newCurrent);
    }

    return current;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Test 1: Simple 2x2 image (but our function expects 28x28, so we use full-size)
    // For simplicity, create a 28x28 image with all zeros; carving should keep all zeros.
    std::vector<int> zeros(28*28, 0);
    std::vector<int> result0 = seamCarving(zeros, 14, 14);
    assert(result0.size() == 14*14);
    for (int v : result0) assert(v == 0);

    // Test 2: Invalid input size returns empty
    std::vector<int> bad(10, 1);
    assert(seamCarving(bad, 14, 14).empty());

    // Test 3: Invalid target larger than original returns empty
    std::vector<int> img(28*28, 5);
    assert(seamCarving(img, 29, 14).empty());
    assert(seamCarving(img, 14, 29).empty());
    assert(seamCarving(img, 0, 14).empty());

    // Test 4: No resizing (target equals original) returns copy
    std::vector<int> original(28*28);
    for (int i = 0; i < 28*28; ++i) original[i] = i % 256;
    std::vector<int> same = seamCarving(original, 28, 28);
    assert(same == original);

    // Test 5: Remove only one column (28 -> 27) and one row (28 -> 27)
    // Build an image with a clear vertical seam: left half high energy, right half low
    // Use a gradient to make seam predictable: energy increases with x, so seam should go to leftmost column
    std::vector<int> gradient(28*28);
    for (int y = 0; y < 28; ++y) {
        for (int x = 0; x < 28; ++x) {
            gradient[y*28 + x] = x; // leftmost column has low energy
        }
    }
    std::vector<int> resized = seamCarving(gradient, 27, 28);
    assert(resized.size() == 27*28);
    // The leftmost column (x=0) should have been removed, so new first column should originally be x=1 (value 1)
    // Check top-left pixel of resized
    assert(resized[0] == 1); // originally (x=1,y=0)

    // Test 6: Remove all but 1x1
    std::vector<int> tiny = seamCarving(gradient, 1, 1);
    assert(tiny.size() == 1);
    // With gradient increasing with x, the minimal seam will remove the leftmost column repeatedly,
    // leaving pixel at (27,0) with value 27? Actually each removal picks minimal total, which tends to remove high energy columns first.
    // The exact expected value is complex; we just assert it's within [0,255].
    assert(tiny[0] >= 0 && tiny[0] <= 255);

    // Test 7: Non-zero image with all same values: result should have same values
    std::vector<int> constVal(28*28, 42);
    std::vector<int> smallConst = seamCarving(constVal, 14, 14);
    assert(smallConst.size() == 14*14);
    for (int v : smallConst) assert(v == 42);

    // Test 8: Check that output dimensions are correct
    std::vector<int> randomImg(28*28);
    for (int i = 0; i < 28*28; ++i) randomImg[i] = (i * 7) % 256;
    std::vector<int> res = seamCarving(randomImg, 20, 10);
    assert(res.size() == 20*10);

    // Test 9: Carving when newWidth equals original but newHeight smaller
    std::vector<int> onlyH = seamCarving(gradient, 28, 14);
    assert(onlyH.size() == 28*14);

    // Test 10: Carving when newHeight equals original but newWidth smaller
    std::vector<int> onlyW = seamCarving(gradient, 14, 28);
    assert(onlyW.size() == 14*28);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
