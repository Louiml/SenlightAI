Write a C++ function that, given an `N x M` grid of ground heights (where each height is an integer between 0 and 256), an initial inventory `B` of block units, and the grid dimensions `N` and `M`, determines the minimum time required to level the entire grid to a uniform target height. The operations allowed are: (1) removing one block from a cell that is taller than the target, which takes 2 seconds and adds the block to your inventory, and (2) placing one block into a cell that is shorter than the target, which takes 1 second and consumes one block from your inventory. You may only choose a target height between 0 and 256, and the operation is only feasible if the total number of blocks you have (`B` plus those removed) is at least the number needed to fill the shorter cells. The function should return a `std::pair<int, int>` containing the minimum time achievable and, among all target heights that achieve that minimum time, the **highest** such target height. The grid is guaranteed to have at least one cell, and `N` and `M` are positive.
The solution iterates over every possible target height from 0 to 256 (inclusive). For each target `h`, we compute the total number of blocks to remove (`del`) and the total number to place (`make`) by scanning every cell. If the cell height is greater than `h`, we add the difference to `del` (since we remove those blocks, gaining inventory). If the cell height is less than `h`, we add the difference to `make` (since we need to place that many blocks). The operation is feasible for target `h` if `del + B >= make`, because we start with `B` blocks and gain `del` more from removals; if the total available is at least `make`, then it is possible. The time for that target is `del * 2 + make` (removal takes 2 seconds per block, placement takes 1 second per block). We track the minimum time found so far; if the current target achieves a time **less than or equal** to the current minimum, we update the minimum time and set the answer height to `h`. Using `<=` ensures that when multiple target heights tie, we keep the **highest** one, because we iterate from 0 upward. Edge cases include grids where all cells are equal (then any height 0-256 may work) and cases where the initial `B` is insufficient for lower target heights but sufficient for higher ones (since higher targets require more `make`). Complexity: 257 iterations × `N*M` scans = O(257·N·M), which simplifies to O(N·M) since 257 is constant. Space is O(1) beyond the input grid storage, which is O(N·M) if the grid is stored, or O(1) if we read directly, but our function receives the grid as a vector of vectors.
#include <vector>
#include <utility>
#include <limits>

// Given grid heights, dimensions, and initial block inventory, return
// the minimum time to level the grid and the highest target height achieving it.
// The target height is clamped between 0 and 256 inclusive.
std::pair<int, int> minecraftLeveling(
    const std::vector<std::vector<int>>& ground,
    int B) {
    
    const int n = static_cast<int>(ground.size());
    const int m = static_cast<int>(ground[0].size());
    const int maxTarget = 256;
    
    int bestTime = std::numeric_limits<int>::max();
    int bestHeight = 0;
    
    for (int h = 0; h <= maxTarget; ++h) {
        int removeBlocks = 0;
        int placeBlocks = 0;
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                int diff = ground[i][j] - h; // positive if taller, negative if shorter
                if (diff > 0) {
                    removeBlocks += diff;
                } else {
                    placeBlocks -= diff; // diff is negative, so -diff is positive
                }
            }
        }
        
        // Feasibility: we start with B, gain removeBlocks, must cover placeBlocks
        if (removeBlocks + B >= placeBlocks) {
            int time = removeBlocks * 2 + placeBlocks;
            if (time <= bestTime) {
                bestTime = time;
                bestHeight = h; // Since we iterate upward, ties keep the highest
            }
        }
    }
    
    return {bestTime, bestHeight};
}
#include <cassert>
#include <vector>
#include <utility>

// Solution declaration from above (assumed included)

int main() {
    // Example 1: Simple single-cell grid
    std::vector<std::vector<int>> g1 = {{5}};
    auto r1 = minecraftLeveling(g1, 0);
    assert(r1.first == 0 && r1.second == 5); // already level at height 5, time 0

    // Example 2: 1x2 grid [0, 10], B=0
    // Target 5 requires remove 5 (10s) and place 5 (5s) total 15s
    // Target 0 requires remove 10 (20s) total 20s
    // Target 10 requires place 10 (10s) but B=0 insufficient
    // So best is height 5 with time 15
    std::vector<std::vector<int>> g2 = {{0, 10}};
    auto r2 = minecraftLeveling(g2, 0);
    assert(r2.first == 15 && r2.second == 5);

    // Example 3: 2x2 all 3's, B=1
    // Only height 3 is feasible (time 0) because other heights require blocks
    std::vector<std::vector<int>> g3 = {{3,3},{3,3}};
    auto r3 = minecraftLeveling(g3, 1);
    assert(r3.first == 0 && r3.second == 3);

    // Example 4: 1x1 grid height 0, B=100
    // Can place blocks to any height 1..100, but min time is 0 at height 0
    std::vector<std::vector<int>> g4 = {{0}};
    auto r4 = minecraftLeveling(g4, 100);
    assert(r4.first == 0 && r4.second == 0);

    // Example 5: large B allows high target even with many removals
    // 1x2 grid [0, 1], B=0
    // Target 0: remove 1 (2s) feasible (1>=0) time=2
    // Target 1: place 1 (1s) but B=0 insuff
    // So best time 2 at height 0
    std::vector<std::vector<int>> g5 = {{0, 1}};
    auto r5 = minecraftLeveling(g5, 0);
    assert(r5.first == 2 && r5.second == 0);

    // Example 6: Ties pick highest height
    // 1x1 grid height=10, B=100
    // Any height h gives time = 2*(10-h) for h<10 or (h-10) for h>10
    // Minimum time is 0 at h=10. Also h=10 yields time 0. No others.
    std::vector<std::vector<int>> g6 = {{10}};
    auto r6 = minecraftLeveling(g6, 100);
    assert(r6.first == 0 && r6.second == 10);

    // Example 7: Larger grid with tie on time
    // 2x1 grid [2,2], B=0
    // Height 2: time 0
    // Height 3: need 2 place (2s) but B=0 insuff
    // Height 1: remove 2 (4s) time=4
    // So best time 0 at height 2
    std::vector<std::vector<int>> g7 = {{2},{2}};
    auto r7 = minecraftLeveling(g7, 0);
    assert(r7.first == 0 && r7.second == 2);

    // Example 8: Check that extreme height 256 works
    // 1x1 grid [256], B=0
    // Height 256: time 0
    std::vector<std::vector<int>> g8 = {{256}};
    auto r8 = minecraftLeveling(g8, 0);
    assert(r8.first == 0 && r8.second == 256);

    // Example 9: Balanced grid
    // 2x2 grid [[1,3],[5,7]], B=2
    // Try target 4: del= (5-4)+(7-4)=1+3=4, make=(4-1)+(4-3)=3+1=4, total blocks needed 4 <= B+del=6, time=4*2+4=12
    // Target 5: del=2, make= (4+2)=6? compute: cells 1->5 add4, 3->5 add2, 5->5 add0,7->5 remove2 => del=2, make=6, feasible if 2+2>=6 false
    // Target 3: del=(5-3)+(7-3)=2+4=6, make=(3-1)+(3-3)=2+0=2, feasible 6+2>=2, time=6*2+2=14
    // So best is target 4 time 12.
    std::vector<std::vector<int>> g9 = {{1,3},{5,7}};
    auto r9 = minecraftLeveling(g9, 2);
    assert(r9.first == 12 && r9.second == 4);

    return 0;
}
