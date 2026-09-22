// Write a C++ function named `segmentImage` that performs iterative image segmentation using a union-find (disjoint-set) approach. The function takes a vector of `Pixel` structs (with `uint8_t` members `r`, `g`, `b`), an integer width, and an integer height, and modifies the vector in-place so that each pixel's color is replaced by the color of its connected component's representative (the root of the union-find tree). Two adjacent pixels (horizontally or vertically, with diagonal connections also considered at certain scales) are merged if the Euclidean distance squared between their current representative colors is less than a fixed threshold (e.g., 30² = 900). The algorithm works in escalating "offset" levels (starting at 1, doubling each iteration) to compare pixels that are increasingly farther apart, merging components whose colors are similar. After all iterations, the function compresses the union-find paths and writes the representative color to each pixel. The function must be self-contained, include necessary headers, and handle edge cases like width or height being 1.

// The core idea is to treat each pixel as an independent node in a union-find (disjoint-set) structure, where the "root" represents a connected component of similar-colored pixels. Initially, each node is its own component with size 1. The algorithm performs multiple passes over the image at different scales (controlled by `offset`). At each scale, it examines neighboring pixels at specific displacements (e.g., horizontal neighbors at distance `offset`, vertical neighbors, and diagonal neighbors at half-offset steps). For each pair, it finds their current roots using path compression; if the roots differ and the Euclidean squared distance between the root colors is below the threshold, the two components are merged. Merging chooses the larger component's root as the new root (to keep trees shallow) and computes a new color as the weighted average of the two component colors by their sizes. The threshold is fixed at `t=30`, so the merge condition is `(r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2 < 900`. The iteration continues with `offset` doubling until it exceeds both width-1 and height-1. Finally, a path-compression step replaces each pixel's color with the color of its root. Edge cases: images with width or height 1 require careful handling to avoid out-of-bounds access; the loops must ensure indices stay within bounds (many loops use conditionals to adjust limits). Time complexity is roughly O(W*H*log(max(W,H))) due to the number of passes and union-find operations, but with path compression the amortized time per find is near O(α(n)). Space complexity is O(W*H) for the `next` and `size` matrices.

#include <vector>
#include <cstdint>
#include <cassert>

struct Pixel {
    uint8_t r, g, b;
};

// Threshold squared (since mergeCriterion uses < t*t with t=30)
static const int THRESHOLD_SQ = 900;

// Global width/height for use in find()
static int g_width, g_height;

// Union-find: find root with path compression
static int findRoot(std::vector<std::vector<int>>& next, int row, int col) {
    int r = row, c = col;
    while (true) {
        int idx = next[r][c];
        if (idx == -1) {
            int actualIdx = r * g_width + c;
            // Path compression: set current and original to root
            next[r][c] = actualIdx;
            next[row][col] = actualIdx;
            return actualIdx;
        }
        int nr = idx / g_width;
        int nc = idx % g_width;
        if (nr == r && nc == c) {
            // This is the root
            next[row][col] = idx;
            return idx;
        }
        r = nr;
        c = nc;
    }
}

// Merge two components if their colors are similar
static void verifyAndMerge(std::vector<Pixel>& pixels,
                           std::vector<std::vector<int>>& next,
                           std::vector<std::vector<int>>& size,
                           int col1, int row1, int col2, int row2) {
    assert(col1 >= 0 && col1 < g_width && row1 >= 0 && row1 < g_height);
    assert(col2 >= 0 && col2 < g_width && row2 >= 0 && row2 < g_height);

    int idxA = findRoot(next, row1, col1);
    int ar = idxA / g_width;
    int ac = idxA % g_width;

    int idxB = findRoot(next, row2, col2);
    int br = idxB / g_width;
    int bc = idxB % g_width;

    if (ar == br && ac == bc) return; // same component

    Pixel A = pixels[ar * g_width + ac];
    Pixel B = pixels[br * g_width + bc];

    int diffR = (int)A.r - (int)B.r;
    int diffG = (int)A.g - (int)B.g;
    int diffB = (int)A.b - (int)B.b;
    if (diffR*diffR + diffG*diffG + diffB*diffB < THRESHOLD_SQ) {
        int sizeA = size[ar][ac];
        int sizeB = size[br][bc];
        int totalSize = sizeA + sizeB;

        Pixel newP;
        newP.r = (uint8_t)(((int)A.r * sizeA + (int)B.r * sizeB) / totalSize);
        newP.g = (uint8_t)(((int)A.g * sizeA + (int)B.g * sizeB) / totalSize);
        newP.b = (uint8_t)(((int)A.b * sizeA + (int)B.b * sizeB) / totalSize);

        if (sizeA > sizeB) {
            pixels[ar * g_width + ac] = newP;
            next[br][bc] = idxA;
            size[ar][ac] += sizeB;
        } else {
            pixels[br * g_width + bc] = newP;
            next[ar][ac] = idxB;
            size[br][bc] += sizeA;
        }
    }
}

// Main segmentation function
void segmentImage(std::vector<Pixel>& pixels, int width, int height) {
    if (pixels.empty() || width <= 0 || height <= 0) return;
    g_width = width;
    g_height = height;

    std::vector<std::vector<int>> next(height, std::vector<int>(width, -1));
    std::vector<std::vector<int>> size(height, std::vector<int>(width, 1));

    int start = 0;
    int offset = 2; // initial offset (will use start=0 with offset=2 first)

    while (start < width - 1 || start < height - 1) {
        // Compare horizontally at distance `offset`
        for (int y = 0; y < height; y++) {
            for (int x = start; x <= width - offset; x += offset) {
                verifyAndMerge(pixels, next, size, x, y, x + 1, y);
            }
        }

        // Compare diagonally (somewhat complex pattern)
        for (int y = 0; y < height; y += offset / 2) {
            for (int x = start; x <= width - offset; x += offset) {
                int limit = offset / 2 - 1;
                if (y + limit > height - 1) limit = height - y - 1;
                for (int n = 0; n < limit; n++) {
                    verifyAndMerge(pixels, next, size, x, y + n, x + 1, y + n + 1);
                    verifyAndMerge(pixels, next, size, x + 1, y + n, x, y + n + 1);
                }
            }
        }

        // Compare vertically
        for (int y = start; y <= height - offset; y += offset) {
            for (int x = 0; x < width; x++) {
                verifyAndMerge(pixels, next, size, x, y, x, y + 1);
            }
        }

        // Additional diagonal comparisons
        for (int y = start; y <= height - offset; y += offset) {
            for (int x = 0; x <= width - offset; x += offset) {
                int limit = offset - 1;
                if (x + limit > width - 1) limit = width - x - 1;
                for (int n = 0; n < limit; n++) {
                    verifyAndMerge(pixels, next, size, x + n, y, x + n + 1, y + 1);
                    verifyAndMerge(pixels, next, size, x + n + 1, y, x + n, y + 1);
                }
            }
        }

        start = 2 * (start + 1) - 1;
        offset *= 2;
    }

    // Final pass: assign each pixel's color to its root's color
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = findRoot(next, i, j);
            int r = idx / g_width;
            int c = idx % g_width;
            pixels[i * width + j] = pixels[r * width + c];
        }
    }
}

#include <cassert>
#include <vector>
#include <cstdint>

// Assume the Pixel struct and segmentImage function are defined above.

int main() {
    // Test 1: Uniform image - all pixels should remain unchanged.
    std::vector<Pixel> img1(4, {100, 50, 200}); // 2x2 uniform
    std::vector<Pixel> expected1 = img1;
    segmentImage(img1, 2, 2);
    for (size_t i = 0; i < img1.size(); i++) {
        assert(img1[i].r == expected1[i].r);
        assert(img1[i].g == expected1[i].g);
        assert(img1[i].b == expected1[i].b);
    }

    // Test 2: Two very different adjacent pixels should not merge (distance > threshold).
    // Pixel A = (0,0,0), Pixel B = (255,255,255) → distance^2 = 3*255^2 > 900.
    std::vector<Pixel> img2 = {{0,0,0}, {255,255,255}}; // 1x2
    segmentImage(img2, 2, 1);
    assert(img2[0].r == 0 && img2[0].g == 0 && img2[0].b == 0);
    assert(img2[1].r == 255 && img2[1].g == 255 && img2[1].b == 255);

    // Test 3: Two similar adjacent pixels (distance < threshold) should merge to weighted average.
    // Pixel A = (10,20,30), Pixel B = (20,30,40) → dist^2 = 3*10^2 = 300 < 900.
    std::vector<Pixel> img3 = {{10,20,30}, {20,30,40}}; // 1x2
    segmentImage(img3, 2, 1);
    // Expected average: (15,25,35) exactly (integer division).
    assert(img3[0].r == 15 && img3[0].g == 25 && img3[0].b == 35);
    assert(img3[1].r == 15 && img3[1].g == 25 && img3[1].b == 35);

    // Test 4: Empty image should not crash.
    std::vector<Pixel> img4;
    segmentImage(img4, 0, 0);
    assert(img4.empty());

    // Test 5: 1x1 image - single pixel unchanged.
    std::vector<Pixel> img5 = {{50,60,70}};
    segmentImage(img5, 1, 1);
    assert(img5[0].r == 50 && img5[0].g == 60 && img5[0].b == 70);

    // Test 6: 3x1 image where left and right are similar to middle but not to each other directly.
    // Our algorithm may merge all if middle bridges them (threshold 900).
    // A=(0,0,0), B=(10,10,10), C=(20,20,20). A-B dist^2=300, B-C dist^2=300, A-C dist^2=1200.
    std::vector<Pixel> img6 = {{0,0,0}, {10,10,10}, {20,20,20}}; // 3x1
    segmentImage(img6, 3, 1);
    // All should merge to weighted average: (0+10+20)/3=10 each channel.
    assert(img6[0].r == 10 && img6[0].g == 10 && img6[0].b == 10);
    assert(img6[1].r == 10 && img6[1].g == 10 && img6[1].b == 10);
    assert(img6[2].r == 10 && img6[2].g == 10 && img6[2].b == 10);

    return 0;
}
