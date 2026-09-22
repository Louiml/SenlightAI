Write a C++ function `std::vector<std::string> tileGrid(int H, int W)` that, given a grid of size H×W (1 ≤ H, W ≤ 10^6), determines whether it can be tiled with 1×2 and 2×1 dominoes such that each cell is covered exactly once and the number of dominoes placed horizontally is exactly equal to the number placed vertically. The function must return `{"No"}` if it's impossible, or `{"Yes"}` followed by H strings each of length W, where each string uses characters `'H'` for a cell covered by a horizontal domino and `'V'` for a cell covered by a vertical domino. The grid can be rotated (i.e., a solution for H×W is equivalent to one for W×H). The output must be a vector of strings representing the tiling. If multiple valid tilings exist, any one is acceptable. The function must handle all sizes up to 10^6, so it cannot allocate more than O(H×W) memory and must run in O(H×W) time.
// Since H and W are both even, the area H*W is divisible by 4. We can tile the grid by partitioning it into (H/2)*(W/2) disjoint 2×2 blocks. In each 2×2 block, place one horizontal domino covering the top two cells and one vertical domino covering the bottom two cells? That would give one horizontal and one vertical per block, but the vertical would cover cells in rows 1 and 2? Actually in a 2×2 block with coordinates (r,c), (r,c+1), (r+1,c), (r+1,c+1), place a horizontal domino on cells (r,c) and (r,c+1) (top row), and a vertical domino on cells (r+1,c) and (r+1,c+1) (bottom two cells of the block). That covers all four cells. Over all blocks, each block contributes exactly one horizontal and one vertical domino, so the totals are equal. This works for any even H and W. The algorithm iterates over rows in steps of 2 and columns in steps of 2, filling the output strings accordingly. Time complexity is O(H*W) because we fill each cell exactly once, and space complexity is O(H*W) for the output. Edge cases include H=2 or W=2, which still work. If H or W is odd, the function returns {"No"} as per the problem statement.
#include <vector>
#include <string>

std::vector<std::string> balancedDominoTiling(int H, int W) {
    // If either dimension is odd, no balanced tiling exists.
    if (H % 2 != 0 || W % 2 != 0) {
        return {"No"};
    }

    std::vector<std::string> grid(H, std::string(W, ' '));

    for (int r = 0; r < H; r += 2) {
        for (int c = 0; c < W; c += 2) {
            // Horizontal domino in the top row of the 2x2 block.
            grid[r][c] = 'H';
            grid[r][c+1] = 'H';
            // Vertical domino in the bottom row of the 2x2 block.
            grid[r+1][c] = 'V';
            grid[r+1][c+1] = 'V';
        }
    }

    return grid;
}
#include <cassert>
#include <vector>
#include <string>

// The function is declared here (include the solution above).
int main() {
    // Test 2x2: one horizontal and one vertical.
    auto t1 = balancedDominoTiling(2, 2);
    assert(t1.size() == 2);
    int hCount = 0, vCount = 0;
    for (const auto& row : t1) {
        for (char ch : row) {
            if (ch == 'H') ++hCount;
            else if (ch == 'V') ++vCount;
        }
    }
    assert(hCount == 2 && vCount == 2);

    // Test 4x4: four of each.
    auto t2 = balancedDominoTiling(4, 4);
    hCount = vCount = 0;
    for (const auto& row : t2) {
        for (char ch : row) {
            if (ch == 'H') ++hCount;
            else if (ch == 'V') ++vCount;
        }
    }
    assert(hCount == 8 && vCount == 8);

    // Test 2x6: six of each.
    auto t3 = balancedDominoTiling(2, 6);
    hCount = vCount = 0;
    for (const auto& row : t3) {
        for (char ch : row) {
            if (ch == 'H') ++hCount;
            else if (ch == 'V') ++vCount;
        }
    }
    assert(hCount == 6 && vCount == 6);

    // Test odd dimension returns "No".
    auto t4 = balancedDominoTiling(3, 4);
    assert(t4.size() == 1 && t4[0] == "No");

    // Test 6x2: six of each.
    auto t5 = balancedDominoTiling(6, 2);
    hCount = vCount = 0;
    for (const auto& row : t5) {
        for (char ch : row) {
            if (ch == 'H') ++hCount;
            else if (ch == 'V') ++vCount;
        }
    }
    assert(hCount == 6 && vCount == 6);
}
