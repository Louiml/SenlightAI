// You are given an `n x m` grid of single digits (0–9), where `n` and `m` are positive integers with `n, m ≤ 4`. The goal is to partition the grid into a set of "pieces" by drawing horizontal or vertical cuts along the grid lines between cells. The partition must satisfy: each piece is a contiguous straight segment (a 1×k or k×1 block of cells). For each piece, you concatenate its digits in order from left to right (for horizontal pieces) or top to bottom (for vertical pieces) to form a non-negative integer (leading zeros are allowed). The score of a partition is the sum of all such integers. Write a C++ function `int maxGridSum(const std::vector<std::string>& grid)` that returns the maximum possible score over all valid partitions. The grid is given as a vector of strings, each of length `m`, containing characters '0'–'9'. The function must handle any `n, m` with `1 ≤ n, m ≤ 4`, and the total number of cells is at most 16.

This problem is a classic exhaustive search over all possible cut patterns. Since the grid is tiny (at most 16 cells), we can enumerate every subset of vertical cut positions between adjacent columns for each row. However, a more natural enumeration is to iterate over all possible bitmasks for vertical cuts on the grid lines between cells in each row. But a cleaner model: there are `(m-1)*n` possible vertical cut segments (between column j and j+1 for each row i). For each such segment, we decide whether it is "open" (cut) or "closed" (connected). Once we fix which vertical segments are open, the grid is partitioned into horizontal blocks (runs of horizontally connected cells) and vertical blocks (runs of vertically connected cells that are not interrupted by horizontal blocks). However, an easier equivalent formulation: enumerate all possible masks of vertical cuts. Then, for each cell, determine its piece by moving left/right/up/down through connected cells. But the provided snippet uses a clever traversal: it processes cells in row-major order, and for each unvisited cell, it extends the piece either to the right (if the right vertical cut is open) or downward (if the vertical path is blocked by closed cuts). The key insight: every cell belongs to exactly one piece, and the piece shape is determined by which vertical cuts are open. The total number of masks is `2^(n*(m-1))`, which for n,m≤4 is at most `2^12 = 4096`. For each mask, we compute the sum by scanning the grid and for each unvisited cell, tracing the entire horizontal or vertical segment. The score for each piece is the integer formed by concatenating digits in the direction of the segment. The maximum over all masks is the answer. Edge cases: single row or single column, all pieces are horizontal or vertical. Leading zeros are allowed and `stoi` handles them correctly. Time complexity: O(2^(n(m-1)) * n*m * max(n,m)) worst-case, which is tiny. Space: O(n*m) for visited and nums.

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

// Compute the maximum possible sum over all valid partitionings.
int maxGridSum(const std::vector<std::string>& grid) {
    const int n = static_cast<int>(grid.size());
    if (n == 0) return 0;
    const int m = static_cast<int>(grid[0].size());

    // Convert grid to integer matrix for convenience.
    int nums[4][4];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            nums[i][j] = grid[i][j] - '0';

    bool opened[12];         // vertical cut between (row, col) and (row, col+1)
    bool visited[4][4];
    int best = 0;

    // Concatenate digits from a given start cell into a number.
    auto calSum = [&](int startRow, int startCol) -> int {
        std::vector<int> vec;
        vec.push_back(nums[startRow][startCol]);
        visited[startRow][startCol] = true;

        int row = startRow;
        int col = startCol;

        // If we're in the last column, piece must extend downward.
        if (col == m - 1) {
            row += 1;
            while (row < n && !opened[(m-1)*row + col]) {  // vertical cut to the left of this cell
                vec.push_back(nums[row][col]);
                visited[row][col] = true;
                row += 1;
            }
        }
        // If there is an open cut to the right, extend horizontally to the right.
        else if (opened[(m-1)*row + col]) {
            vec.push_back(nums[row][col+1]);
            visited[row][col+1] = true;
            col += 2;
            while (col < m && opened[(m-1)*row + col]) {
                vec.push_back(nums[row][col]);
                visited[row][col] = true;
                col += 1;
            }
        }
        // Otherwise, the path to the right is blocked, so extend downward.
        else {
            // For first column, only check right side; for others check both sides.
            row += 1;
            if (col == 0) {
                while (row < n && !opened[(m-1)*row + col]) {  // right cut of this cell
                    vec.push_back(nums[row][col]);
                    visited[row][col] = true;
                    row += 1;
                }
            } else {
                while (row < n && !opened[(m-1)*row + col] && !opened[(m-1)*row + col - 1]) {
                    vec.push_back(nums[row][col]);
                    visited[row][col] = true;
                    row += 1;
                }
            }
        }

        // Convert vector of digits to integer (concatenation).
        int num = 0;
        for (int digit : vec) {
            num = num * 10 + digit;
        }
        return num;
    };

    // Recursively enumerate all subsets of vertical cuts.
    // opened[i] corresponds to cut after row i/(m-1), col i%(m-1)?? Actually indexing: opened[(m-1)*row + col] cuts between (row,col) and (row,col+1).
    std::function<void(int)> split = [&](int curr) {
        if (curr == n * (m - 1)) {
            int currSum = 0;
            memset(visited, 0, sizeof(visited));
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (!visited[i][j]) {
                        currSum += calSum(i, j);
                    }
                }
            }
            best = std::max(best, currSum);
            return;
        }
        opened[curr] = true;
        split(curr + 1);
        opened[curr] = false;
        split(curr + 1);
    };

    split(0);
    return best;
}

#include <cassert>
#include <string>
#include <vector>

int maxGridSum(const std::vector<std::string>& grid); // forward declaration

int main() {
    // Single cell
    assert(maxGridSum({"5"}) == 5);
    // Single row
    assert(maxGridSum({"12"}) == 12 + 0); // best: concatenate both → 12, or split into 1+2=3 → max 12
    assert(maxGridSum({"123"}) == 123);
    assert(maxGridSum({"12" , "34"}) == 12 + 34); // horizontal rows give 12+34=46, vertical columns 13+24=37, max 46
    // 2x2 all zeros
    assert(maxGridSum({"00", "00"}) == 0);
    // 2x2 with all 9's: horizontal rows 99+99=198, vertical 99+99=198, single 9999 = 9999? Actually cannot parce all four into one piece because it's not a straight line. So max is 198.
    assert(maxGridSum({"99", "99"}) == 198);
    // 3x3 simple: each row "111" horizontally gives 111+111+111=333; vertical gives 111+111+111=333; mixed no better.
    assert(maxGridSum({"111", "111", "111"}) == 333);
    // 1x4: all one piece 1234
    assert(maxGridSum({"1234"}) == 1234);
    // 4x1: all one vertical piece 1234
    assert(maxGridSum({"1", "2", "3", "4"}) == 1234);
    // 2x3 with pattern: best is split into vertical "13" and "24" and "55"? Actually test known case: grid {"12","34","56"}? But n=3,m=2.
    // Simple test: 2x2 with 1,2,3,4 (as above) but assert maximum is 46.
    // Let's test a tricky one: 2x2 with "19","28" – horizontal rows: 19+28=47, vertical columns: 12+98=110? Actually col0: 1,2 → 12; col1: 9,8 → 98 sum=110, so max=110.
    assert(maxGridSum({"19", "28"}) == 110);
    // 2x2 with "91","82" – vertical: 98+12=110, horizontal:91+82=173, max=173.
    assert(maxGridSum({"91", "82"}) == 173);
    return 0;
}
