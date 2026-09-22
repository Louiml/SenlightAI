Write a C++ function `uint64_t decodeFiducialInnerBits(const std::vector<std::vector<int>>& cellGrid, int innerBitsPerSide)` that takes a binary grid of size `(innerBitsPerSide+2) x (innerBitsPerSide+2)` representing a fiducial marker’s sampled cells (0 = black, 1 = white), where the outer border cells must all be 0 (black) for the marker to be valid. The function must extract the inner `innerBitsPerSide x innerBitsPerSide` bits in row-major order from the top-left inner cell to the bottom-right inner cell, and pack those bits into a 64-bit unsigned integer where the first extracted bit (top-left of inner region) becomes the least-significant bit (LSB) of the result, and subsequent bits fill upward so that the last extracted bit (bottom-right) becomes bit `innerBitsPerSide*innerBitsPerSide - 1`. If the outer border contains any non-zero cell, return 0 (which is also a valid packed code for all-zero inner bits, but in practice indicates invalid marker detection). Assume `innerBitsPerSide` is at least 1 and `innerBitsPerSide*innerBitsPerSide <= 64`. The function must not use any external libraries beyond standard C++.
#include <cassert>
#include <cstdint>
#include <vector>

// The solution function is declared above (include it here).

int main() {
    // Example 1: 2x2 inner bits with valid border, all inner bits zero.
    std::vector<std::vector<int>> grid1 = {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    assert(decodeFiducialInnerBits(grid1, 2) == 0);

    // Example 2: 2x2 inner bits, pattern: top-left=1, top-right=0, bottom-left=1, bottom-right=0.
    // Bits: index0=1, index1=0, index2=1, index3=0 => binary 0101 = 5
    std::vector<std::vector<int>> grid2 = {
        {0,0,0,0},
        {0,1,0,0},
        {0,1,0,0},
        {0,0,0,0}
    };
    assert(decodeFiducialInnerBits(grid2, 2) == 5);

    // Example 3: 1x1 inner bits with valid border and inner bit=1 => result 1
    std::vector<std::vector<int>> grid3 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    assert(decodeFiducialInnerBits(grid3, 1) == 1);

    // Example 4: Invalid border (non-zero at top-left) returns 0.
    std::vector<std::vector<int>> grid4 = {
        {1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,0}
    };
    assert(decodeFiducialInnerBits(grid4, 2) == 0);

    // Example 5: 3x3 inner bits, all ones => binary all 9 bits set = 511
    std::vector<std::vector<int>> grid5(5, std::vector<int>(5, 1));
    // Force border to zero
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c) {
            if (r==0 || r==4 || c==0 || c==4) grid5[r][c] = 0;
        }
    assert(decodeFiducialInnerBits(grid5, 3) == ((uint64_t{1} << 9) - 1));

    // Example 6: 1x1 inner bits with inner bit=0 => result 0 (also valid).
    std::vector<std::vector<int>> grid6 = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };
    assert(decodeFiducialInnerBits(grid6, 1) == 0);

    // Example 7: Check row-major ordering: bottom-left inner cell is bit index (innerBitsPerSide*1+0)
    // For 2x2: index2 = low-left. Set only that cell.
    std::vector<std::vector<int>> grid7 = {
        {0,0,0,0},
        {0,0,0,0},
        {0,1,0,0},
        {0,0,0,0}
    };
    // Bits: index0=0, index1=0, index2=1, index3=0 => 0100 = 4
    assert(decodeFiducialInnerBits(grid7, 2) == 4);

    // Example 8: Invalid border on right edge.
    std::vector<std::vector<int>> grid8 = {
        {0,0,0,0},
        {0,1,1,0},
        {0,0,0,1},
        {0,0,0,0}
    };
    assert(decodeFiducialInnerBits(grid8, 2) == 0);

    // Example 9: 2x2 inner pattern: top-left=0, top-right=1, bottom-left=0, bottom-right=1
    // binary 1010 = 10
    std::vector<std::vector<int>> grid9 = {
        {0,0,0,0},
        {0,0,1,0},
        {0,0,1,0},
        {0,0,0,0}
    };
    assert(decodeFiducialInnerBits(grid9, 2) == 10);

    // Example 10: Max bits for 8x8 inner (64 bits), set all to 1 => all bits set.
    std::vector<std::vector<int>> grid10(10, std::vector<int>(10, 1));
    for (int r = 0; r < 10; ++r)
        for (int c = 0; c < 10; ++c) {
            if (r==0 || r==9 || c==0 || c==9) grid10[r][c] = 0;
        }
    assert(decodeFiducialInnerBits(grid10, 8) == ~uint64_t{0});
}
#include <cstdint>
#include <vector>

/**
 * Extracts inner bits from a binary fiducial marker cell grid.
 * The grid is (innerBitsPerSide+2) x (innerBitsPerSide+2) where the outer border
 * must be all zeros (black). On success, returns the packed inner bits in row-major
 * order with the top-left inner cell as bit 0. Returns 0 if the border is invalid.
 */
uint64_t decodeFiducialInnerBits(const std::vector<std::vector<int>>& cellGrid,
                                 int innerBitsPerSide) {
    const int gridSize = innerBitsPerSide + 2;

    // Validate the outer border must be entirely zero (black).
    for (int r = 0; r < gridSize; ++r) {
        for (int c = 0; c < gridSize; ++c) {
            bool isBorder = (r == 0) || (r == gridSize - 1) ||
                            (c == 0) || (c == gridSize - 1);
            if (isBorder && cellGrid[r][c] != 0) {
                return 0;
            }
        }
    }

    // Extract inner bits and pack them row-major, first cell (top-left) as LSB.
    uint64_t result = 0;
    for (int r = 0; r < innerBitsPerSide; ++r) {
        for (int c = 0; c < innerBitsPerSide; ++c) {
            int bitValue = cellGrid[r + 1][c + 1];  // inner region is offset by 1 from border
            int bitIndex = r * innerBitsPerSide + c;  // row-major order
            if (bitValue != 0) {
                result |= (uint64_t{1} << bitIndex);
            }
        }
    }
    return result;
}
// The solution must first validate the border: for all cells where row index is 0, row index is the last row, column index is 0, or column index is the last column, check that the cell value is exactly 0. If any border cell is non-zero, immediately return 0. Then iterate over the inner region: for `r` from 0 to `innerBitsPerSide-1` and `c` from 0 to `innerBitsPerSide-1`, access `cellGrid[r+1][c+1]`, which gives the inner cell at that position. Build the 64-bit result by accumulating bits: for each extracted bit, place it at bit position `(r * innerBitsPerSide + c)` (since the first inner cell is (0,0) → bit 0, then (0,1) → bit 1, etc., row-major). Use a `uint64_t` accumulator and shift left by one each time, then OR the bit value; however, careful with bit order: the problem states the first extracted bit becomes LSB. So the simplest is to set `result |= (bitValue ? 1ULL : 0ULL) << (r * innerBitsPerSide + c)`. Edge cases: innerBitsPerSide=1 gives a single bit, valid if border all zeros; grid dimensions exactly match `innerBitsPerSide+2`; values are guaranteed to be only 0 or 1. Time complexity is O((n+2)^2) for border check and O(n^2) for inner extraction, so overall O(n^2) where n = innerBitsPerSide. Space complexity is O(1) besides the input grid.
