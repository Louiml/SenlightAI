// You are given a notebook grid of size `n` rows by `m` columns, initially empty (all cells set to 0). There are `k` stickers, each with its own height and width, and each sticker is represented as a binary grid (1 = sticker cell, 0 = empty). Each sticker can be rotated by 90, 180, or 270 degrees clockwise, and when placed, the entire sticker must fit entirely inside the notebook without overlapping any already placed sticker cell. Stickers are processed in the given order; for each sticker, you must try all four rotations, and for each rotation, try all possible placements starting from the top-leftmost cell (row index 0, column index 0) scanning row by row then column by column. The first placement that fits (no overlap and within bounds) is used; if none fits, that sticker is discarded. After attempting all stickers, count the number of cells in the notebook that are occupied by at least one sticker. Write a C++ function `int countOccupiedCells(int n, int m, int k, const vector<vector<vector<vector<int>>>> &stickers)` where `stickers` is a `k`-element vector, each element is a `4`-element vector of rotations (rotation 0 = original, rotation 1 = 90° clockwise, rotation 2 = 180°, rotation 3 = 270°), and each rotation is a 2D vector of ints (height x width) containing the sticker pattern after that rotation. The function must return the total number of occupied cells in the notebook after processing all stickers in order. Assume `1 ≤ n, m ≤ 40`, `1 ≤ k ≤ 100`, each sticker’s dimensions are between 1 and 10, and all values in sticker patterns are 0 or 1.
#include <cassert>
#include <vector>

int main() {
    // Helper to build a sticker set.
    using StickerSet = std::vector<std::vector<std::vector<std::vector<int>>>>;

    // Test 1: Single sticker 2x2 fits in 3x3 notebook.
    {
        StickerSet stickers(1, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(2, std::vector<int>(2, 0))));
        // Original pattern: [[1,0],[0,1]]
        stickers[0][0] = {{1,0},{0,1}};
        // 90° rotation: [[0,1],[1,0]]
        stickers[0][1] = {{0,1},{1,0}};
        // 180° rotation: [[1,0],[0,1]] (same as original)
        stickers[0][2] = {{1,0},{0,1}};
        // 270° rotation: [[0,1],[1,0]] (same as 90°)
        stickers[0][3] = {{0,1},{1,0}};
        assert(countOccupiedCells(3, 3, 1, stickers) == 2);
    }

    // Test 2: Sticker too big, discarded.
    {
        StickerSet stickers(1, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(5, std::vector<int>(5, 1))));
        for (int r = 0; r < 4; ++r) stickers[0][r] = std::vector<std::vector<int>>(5, std::vector<int>(5, 1));
        assert(countOccupiedCells(2, 2, 1, stickers) == 0);
    }

    // Test 3: Multiple stickers, second overlaps partially but first placement blocks.
    {
        StickerSet stickers(2, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(2, std::vector<int>(2, 0))));
        // Sticker 0: full 2x2 block, placed at top-left (0,0) in original rotation.
        stickers[0][0] = {{1,1},{1,1}};
        stickers[0][1] = {{1,1},{1,1}}; // same for all
        stickers[0][2] = {{1,1},{1,1}};
        stickers[0][3] = {{1,1},{1,1}};
        // Sticker 1: also 2x2 block, but only fits at (1,1) because (0,0) occupied.
        // But (1,1) is outside 2x2 notebook? Let's use 3x3 notebook.
        stickers[1][0] = {{1,1},{1,1}};
        stickers[1][1] = {{1,1},{1,1}};
        stickers[1][2] = {{1,1},{1,1}};
        stickers[1][3] = {{1,1},{1,1}};
        // Notebook 3x3: first sticker occupies (0,0),(0,1),(1,0),(1,1).
        // Second sticker tries (0,0) overlap, (0,1) overlap, (1,0) overlap, (1,1) overlap,
        // (2,0) fits? (2,0) to (3,0) row index out of bounds? No, 2+2=4>3, so all fail.
        // However (0,2) to (1,2) col index out of bounds? 2+2=4>3, fail. So second not placed.
        assert(countOccupiedCells(3, 3, 2, stickers) == 4);
    }

    // Test 4: Rotation allows fit.
    {
        StickerSet stickers(1, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(2, std::vector<int>(1, 0))));
        // Original is 2 rows x 1 col, all ones.
        stickers[0][0] = {{1},{1}}; // height 2, width 1
        stickers[0][1] = {{1,1}};   // 90° rotation: height 1, width 2
        stickers[0][2] = {{1},{1}}; // 180° same as original
        stickers[0][3] = {{1,1}};   // 270° same as 90°
        // Fit in 1x2 notebook: original doesn't fit (height 2>1), but 90° fits.
        assert(countOccupiedCells(1, 2, 1, stickers) == 2);
    }

    // Test 5: All stickers placed side by side.
    {
        StickerSet stickers(2, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(1, std::vector<int>(1, 0))));
        stickers[0][0] = {{1}}; stickers[0][1] = {{1}}; stickers[0][2] = {{1}}; stickers[0][3] = {{1}};
        stickers[1][0] = {{1}}; stickers[1][1] = {{1}}; stickers[1][2] = {{1}}; stickers[1][3] = {{1}};
        assert(countOccupiedCells(2, 2, 2, stickers) == 2);
    }

    // Test 6: Sticker with zeros and ones, partial placement.
    {
        StickerSet stickers(1, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(2, std::vector<int>(2, 0))));
        stickers[0][0] = {{1,0},{0,0}}; // only one cell
        stickers[0][1] = {{0,0},{1,0}}; // rotation
        stickers[0][2] = {{0,0},{0,1}}; // etc
        stickers[0][3] = {{0,1},{0,0}};
        // Place in 3x3, only one cell becomes occupied.
        assert(countOccupiedCells(3, 3, 1, stickers) == 1);
    }

    // Test 7: Overlap avoidance.
    {
        StickerSet stickers(2, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(1, std::vector<int>(2, 0))));
        // Sticker 0: [[1,1]] placed at (0,0) in 2x3 notebook.
        stickers[0][0] = {{1,1}}; stickers[0][1] = {{1},{1}}; stickers[0][2] = {{1,1}}; stickers[0][3] = {{1},{1}};
        // Sticker 1: [[1,1]] tries but all positions overlap? (0,1) overlaps (0,1) occupied? Actually (0,1) is occupied by first sticker at (0,1). So it must move to (1,0) which is empty, but width 2 doesn't fit at (1,0) because m=3? (1,0) has col 0 and 1, that's fine. So it will be placed at (1,0).
        stickers[1][0] = {{1,1}}; stickers[1][1] = {{1},{1}}; stickers[1][2] = {{1,1}}; stickers[1][3] = {{1},{1}};
        // Notebook 2x3: first sticker occupies (0,0),(0,1). Second sticker scans: (0,0) overlap, (0,1) overlap, (1,0) no overlap and fits, so placed at (1,0),(1,1). Total occupied 4 cells.
        assert(countOccupiedCells(2, 3, 2, stickers) == 4);
    }

    // Test 8: Sticker cannot fit due to overlap everywhere.
    {
        StickerSet stickers(2, std::vector<std::vector<std::vector<int>>>(
            4, std::vector<std::vector<int>>(1, std::vector<int>(1, 0))));
        stickers[0][0] = {{1}}; stickers[0][1] = {{1}}; stickers[0][2] = {{1}}; stickers[0][3] = {{1}};
        stickers[1][0] = {{1}}; stickers[1][1] = {{1}}; stickers[1][2] = {{1}}; stickers[1][3] = {{1}};
        // Notebook 1x1: first sticker occupies the only cell, second cannot place.
        assert(countOccupiedCells(1, 1, 2, stickers) == 1);
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Count occupied cells after placing stickers in order with rotation and position preferences.
int countOccupiedCells(int n, int m, int k,
                       const std::vector<std::vector<std::vector<std::vector<int>>>>& stickers) {
    // Notebook grid, 0 = empty, 1 = occupied.
    std::vector<std::vector<int>> notebook(n, std::vector<int>(m, 0));

    for (int idx = 0; idx < k; ++idx) {
        bool placed = false;

        // Try all four rotations.
        for (int rot = 0; rot < 4 && !placed; ++rot) {
            const auto& pattern = stickers[idx][rot];
            int h = static_cast<int>(pattern.size());
            int w = static_cast<int>(pattern[0].size());

            // Try all positions in row-major order.
            for (int startRow = 0; startRow + h <= n && !placed; ++startRow) {
                for (int startCol = 0; startCol + w <= m && !placed; ++startCol) {
                    // Check for any overlap with existing occupied cells.
                    bool overlap = false;
                    for (int i = 0; i < h && !overlap; ++i) {
                        for (int j = 0; j < w; ++j) {
                            if (notebook[startRow + i][startCol + j] == 1 && pattern[i][j] == 1) {
                                overlap = true;
                                break;
                            }
                        }
                    }

                    if (!overlap) {
                        // Place the sticker by OR-ing the pattern into the notebook.
                        for (int i = 0; i < h; ++i) {
                            for (int j = 0; j < w; ++j) {
                                if (pattern[i][j] == 1) {
                                    notebook[startRow + i][startCol + j] = 1;
                                }
                            }
                        }
                        placed = true;
                    }
                }
            }
        }
    }

    // Count occupied cells.
    int result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (notebook[i][j] == 1) {
                ++result;
            }
        }
    }
    return result;
}
// The solution must simulate the placement process exactly as described. For each sticker in order, iterate over rotations 0 to 3. For each rotation, get the rotated dimensions (height = original height if rotation is 0 or 2, else width; width = original width if rotation is 0 or 2, else height). Then for every possible top-left position `(startRow, startCol)` such that `startRow + height ≤ n` and `startCol + width ≤ m`, check if placing the sticker there causes any overlap with already occupied cells. If no overlap, place the sticker by setting all cells where the sticker has 1 to 1 in the notebook. Break out of all loops for this sticker once placed. If no rotation and position works, move to the next sticker without placing it. To check overlap efficiently, simply loop over the sticker’s cells and test `notebook[startRow+i][startCol+j] == 1 && stickerPattern[i][j] == 1`. After all stickers are processed, count the cells in the notebook that are 1. Important edge cases: when a sticker doesn’t fit in any rotation (e.g., larger than the notebook in all rotations), it is skipped; when a sticker fits but every possible position overlaps, it is also skipped. The order of trying placements is crucial: for each rotation, start at row 0 column 0, then row 0 column 1, … row 0, column m-1, then row 1 column 0, etc., and stop at the first valid position. Time complexity: For each sticker, at most 4 rotations, for each rotation at most n*m positions, and for each position checking at most 10*10 = 100 cells. With max values, this is 100 * 4 * 1600 * 100 = 64,000,000 operations, which is acceptable. Space complexity: O(n*m) for the notebook plus O(k * 4 * maxStickerArea) for storing all rotations; given the constraints, this is at most 40*40 + 100*4*100 = 1600 + 40000 = 41600 integers, well within limits.
