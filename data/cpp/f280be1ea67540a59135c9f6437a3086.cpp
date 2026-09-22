/*
Write a C++ function `int rotOranges(std::vector<std::vector<int>>& grid)` that simulates the rotting of fresh oranges in a rectangular grid. Each cell contains either: `0` for an empty cell, `1` for a fresh orange, or `2` for a rotten orange. In each time unit, any fresh orange that is orthogonally adjacent (up, down, left, right) to a rotten orange becomes rotten. The function must return the minimum number of time units until all fresh oranges have rotted. If it is impossible for all fresh oranges to rot (because some fresh oranges are isolated by empty cells or grid boundaries), return `-1`. The grid dimensions are not fixed; the function should accept any non-empty rectangular grid. The input grid is mutated during the process, so the function should work on a copy or modify in-place safely. Assume the grid is always non-empty and rectangular.
*/
#include <vector>

// Rot all reachable fresh oranges level by level.
// Returns the number of time units needed, or -1 if some oranges cannot rot.
int rotOranges(std::vector<std::vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) return 0;
    
    const int rows = static_cast<int>(grid.size());
    const int cols = static_cast<int>(grid[0].size());
    
    auto isSafe = [rows, cols](int i, int j) {
        return i >= 0 && i < rows && j >= 0 && j < cols;
    };
    
    bool changed = false;
    int no = 2;  // initial rotten value marker; each wave increments this
    
    while (true) {
        changed = false;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == no) {
                    // Check four orthogonal neighbors
                    if (isSafe(i + 1, j) && grid[i + 1][j] == 1) {
                        grid[i + 1][j] = no + 1;
                        changed = true;
                    }
                    if (isSafe(i - 1, j) && grid[i - 1][j] == 1) {
                        grid[i - 1][j] = no + 1;
                        changed = true;
                    }
                    if (isSafe(i, j + 1) && grid[i][j + 1] == 1) {
                        grid[i][j + 1] = no + 1;
                        changed = true;
                    }
                    if (isSafe(i, j - 1) && grid[i][j - 1] == 1) {
                        grid[i][j - 1] = no + 1;
                        changed = true;
                    }
                }
            }
        }
        
        if (!changed) break;
        ++no;
    }
    
    // Check for any remaining fresh oranges
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) return -1;
        }
    }
    
    // no started at 2, so time = no - 2
    return no - 2;
}
#include <cassert>
#include <vector>

// Function declaration (the actual implementation is provided above; this is for the test)
int rotOranges(std::vector<std::vector<int>>& grid);

int main() {
    // Test 1: Example from snippet
    std::vector<std::vector<int>> grid1 = {
        {2, 1, 0, 2, 1},
        {1, 0, 1, 2, 1},
        {1, 0, 0, 2, 1}
    };
    assert(rotOranges(grid1) == 2);

    // Test 2: All already rotten or empty -> 0
    std::vector<std::vector<int>> grid2 = {
        {2, 0, 2},
        {0, 0, 2}
    };
    assert(rotOranges(grid2) == 0);

    // Test 3: Impossible to rot all
    std::vector<std::vector<int>> grid3 = {
        {2, 1, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    assert(rotOranges(grid3) == -1);

    // Test 4: Single fresh orange next to rotten
    std::vector<std::vector<int>> grid4 = {{2, 1}};
    assert(rotOranges(grid4) == 1);

    // Test 5: Single fresh orange isolated
    std::vector<std::vector<int>> grid5 = {{1}};
    assert(rotOranges(grid5) == -1);

    // Test 6: Grid with no fresh (only empty and rotten) -> 0
    std::vector<std::vector<int>> grid6 = {{0, 0}, {0, 0}};
    assert(rotOranges(grid6) == 0);

    // Test 7: Large chain needing multiple steps
    std::vector<std::vector<int>> grid7 = {
        {2, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    // Orange at (0,0) rots (0,1) in step1; (0,2) in step2; (1,0) step1; etc. Max distance from (0,0) is 4 to (2,2)
    assert(rotOranges(grid7) == 4);

    // Test 8: Non-square grid
    std::vector<std::vector<int>> grid8 = {
        {2, 1, 1, 1},
        {0, 1, 0, 1}
    };
    // Step1: (0,1) rots; (0,2) rots? No, (0,2) is adjacent to (0,1) which becomes rotten after step1, so it rots step2; (1,3) is adjacent to (0,3) which rots step2; (1,1) is isolated by 0s -> unreachable? Let's compute: (0,0) rotten -> (0,1) rot step1. (0,1) -> (0,2) step2. (0,2) -> (0,3) step3. (0,3) -> (1,3) step4. (1,1) is surrounded by 0s and a rotten? Actually (1,1) neighbors: (0,1) rot step1, (1,0)=0, (1,2)=0, (2,1) out. So it rots step2. So all rot in 4 steps.
    assert(rotOranges(grid8) == 4);

    // Test 9: Already all rotten
    std::vector<std::vector<int>> grid9 = {{2, 2}, {2, 2}};
    assert(rotOranges(grid9) == 0);

    // Test 10: One rotten, many fresh in a line
    std::vector<std::vector<int>> grid10 = {{2, 1, 1, 1}};
    assert(rotOranges(grid10) == 3);

    return 0;
}
// The algorithm is a classic multi-source BFS (breadth-first search) or iterative simulation. The given snippet uses a level-by-level expansion technique: it initializes a variable `no = 2` representing the current "age" label of rotten oranges (starting from 2). It repeatedly scans the entire grid, and for every cell containing the current aged rotten orange (`v[i][j] == no`), it checks its four orthogonal neighbors. If a neighbor is fresh (`==1`), it becomes rotten and is labeled with `no + 1` (meaning it will rot further in the next time step). A `changed` flag tracks whether any transformation happened. After a full scan, if no changes occurred, the process stops; otherwise `no` is incremented and the scan repeats. This approach effectively processes one "wave" per time unit. After the loop, it scans the grid again for any remaining `1`; if found, returns `-1`. Otherwise it returns `no - 2`, which equals the number of time units (since `no` started at 2 and increments once per complete wave). Important edge cases: (1) an already entirely rotten or empty grid (no fresh oranges) should return `0`; the algorithm does because `no` stays at 2 and `no-2=0`. (2) Fresh oranges that are unreachable due to empty-cell barriers correctly cause `-1`. (3) The input grid must not be `const` because it is mutated; but a robust solution can take by value or make a copy. Time complexity is \(O(R*C*T)\) where \(T\) is the number of time units (at most \(R*C\)), so worst-case \(O((R*C)^2)\). Space complexity is \(O(1)\) auxiliary besides the input grid. A more efficient BFS queue solution would be \(O(R*C)\) time and \(O(R*C)\) space, but the iterative approach is simpler and matches the snippet.
