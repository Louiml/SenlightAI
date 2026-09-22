Given a 2D grid `land` of non-negative integers representing ground heights, and two positive integers `P` and `Q` representing the cost to raise a block by one unit (`P`) and the cost to lower a block by one unit (`Q`), write a C++ function that returns the minimum total cost to make all cells in the grid have the same height. You may choose any target height (not necessarily an integer? The problem expects integer heights, and the optimal target will be between the minimum and maximum initial heights). The grid has at least one row and one column, and all row lengths are equal. The answer may exceed 32-bit integers, so return a `long long`.
// The key observation is that the cost function `f(h)` for making all cells equal to height `h` is convex (piecewise linear with a single minimum). Therefore, a binary search on the integer height `h` in the range `[minHeight, maxHeight]` can find the global minimum efficiently. For each candidate height `mid`, compute the total cost to raise (`P` per unit) or lower (`Q` per unit) every cell to `mid`. Because the optimal height could lie between two integers, we also evaluate `mid+1` and compare the two costs to decide which side of the search space to keep. Specifically, if `cost(mid) < cost(mid+1)`, the minimum lies at or below `mid`, so move `hi = mid-1`; if `cost(mid) > cost(mid+1)`, the minimum lies at or above `mid+1`, so move `lo = mid+1`; if equal, we can break. Keep track of the best answer seen. Important edge cases: `P` and `Q` can be large, and all heights may be identical (then `lo == hi` and cost is 0). The total cost can be up to `(maxHeight - minHeight) * gridCells * max(P,Q)`, so use `long long` for all arithmetic. Time complexity: `O(N * log(range))` where `N` is the number of cells and `range = maxHeight - minHeight + 1`; space complexity `O(1)` beyond input.
#include <vector>
#include <algorithm>
#include <cstdlib>

// Computes the minimum total cost to make all land cells equal height.
// land: 2D grid of non-negative integers (row-major, all rows same length).
// P: cost to raise one block by one unit (positive).
// Q: cost to lower one block by one unit (positive).
// Returns the minimum total cost as long long.
long long minCostToEqualize(const std::vector<std::vector<int>>& land, int P, int Q) {
    if (land.empty() || land[0].empty()) {
        return 0;
    }
    long long minH = land[0][0];
    long long maxH = land[0][0];
    const int rows = static_cast<int>(land.size());
    const int cols = static_cast<int>(land[0].size());
    
    // Find min and max heights in the grid.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            long long val = land[i][j];
            minH = std::min(minH, val);
            maxH = std::max(maxH, val);
        }
    }
    
    long long best = 9e18 + 1;  // large sentinel
    long long lo = minH;
    long long hi = maxH;
    
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        
        // Compute cost at 'mid' and 'mid+1'
        long long costMid = 0;
        long long costMidPlus = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                long long h = land[i][j];
                // Cost to bring h to 'mid'
                if (h < mid) {
                    costMid += (mid - h) * static_cast<long long>(P);
                } else if (h > mid) {
                    costMid += (h - mid) * static_cast<long long>(Q);
                }
                // Cost to bring h to 'mid+1'
                long long target2 = mid + 1;
                if (h < target2) {
                    costMidPlus += (target2 - h) * static_cast<long long>(P);
                } else if (h > target2) {
                    costMidPlus += (h - target2) * static_cast<long long>(Q);
                }
            }
        }
        
        long long currentBest = std::min(costMid, costMidPlus);
        if (currentBest < best) {
            best = currentBest;
        }
        
        if (costMid < costMidPlus) {
            hi = mid - 1;  // optimum is at or below mid
        } else if (costMid > costMidPlus) {
            lo = mid + 1;  // optimum is at or above mid+1
        } else {
            break;  // found a flat region; no need to search further
        }
    }
    
    return best;
}
#include <cassert>
#include <vector>

int main() {
    // Single cell, already equal.
    std::vector<std::vector<int>> grid1 = {{5}};
    assert(minCostToEqualize(grid1, 2, 3) == 0);
    
    // All equal, large grid.
    std::vector<std::vector<int>> grid2 = {{7, 7}, {7, 7}};
    assert(minCostToEqualize(grid2, 10, 10) == 0);
    
    // Simple case: two cells 1 and 3, P=1, Q=1 => best at height 2, cost 2.
    std::vector<std::vector<int>> grid3 = {{1, 3}};
    assert(minCostToEqualize(grid3, 1, 1) == 2);
    
    // P and Q differ: heights 0 and 10, P=1, Q=100 => raising from 0 costs 10*1, lowering from 10 to 0 costs 10*100 = 1000, so target 0 is better? Actually try target 0: raise 0->0 =0, lower 10->0 = 1000, total 1000. Target 10: raise 0->10=10, lower 10->10=0, total 10. So best is 10.
    std::vector<std::vector<int>> grid4 = {{0, 10}};
    assert(minCostToEqualize(grid4, 1, 100) == 10);
    
    // Asymmetric costs: P=10, Q=1, heights 0 and 10 => target 10 costs raise 0->10 =100, lower none; total 100; target 0 costs raise none, lower 10->0 =10; so best 10.
    std::vector<std::vector<int>> grid5 = {{0, 10}};
    assert(minCostToEqualize(grid5, 10, 1) == 10);
    
    // Larger grid: [[1,5],[9,13]] with P=1, Q=1. Best target around 7? Let's compute: target 7 => costs: |1-7|=6, |5-7|=2, |9-7|=2, |13-7|=6 total 16. Target 6 => 5+1+3+7=16. Target 8 => 7+3+1+5=16. So any integer from 6 to 8 gives 16. Test 16.
    std::vector<std::vector<int>> grid6 = {{1,5},{9,13}};
    assert(minCostToEqualize(grid6, 1, 1) == 16);
    
    // Edge case: very large values, ensure no overflow. Heights 0 and 1e9, P=1, Q=1, grid 1x2. Best at some middle, cost ~1e9 (exact: target any integer between 0 and 1e9, cost = h*1 + (1e9-h)*1 = 1e9). Test.
    std::vector<std::vector<int>> grid7 = {{0, 1000000000}};
    assert(minCostToEqualize(grid7, 1, 1) == 1000000000);
    
    // All zeros, P large, Q small: cost 0 always.
    std::vector<std::vector<int>> grid8 = {{0,0},{0,0}};
    assert(minCostToEqualize(grid8, 5, 1) == 0);
    
    return 0;
}
