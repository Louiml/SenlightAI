// Write a C++ function `int minMovesInSlidingGrid(int n, const std::vector<std::string>& grid, int startX, int startY, int targetX, int targetY)` that, given an `n x n` grid of characters where `.` denotes a passable cell and `X` denotes a blocked cell, computes the minimum number of moves needed to reach the target cell from the start cell. A move consists of choosing one of the four cardinal directions (up, right, down, left) and then sliding continuously in that direction until immediately before hitting a blocked cell or the grid boundary; each such slide counts as exactly one move. You may stop on any passable cell along that slide. If the target is unreachable, return `-1`. The start and target coordinates are zero-based. The grid size `n` satisfies `1 <= n <= 1000`. The function must handle grids that are entirely open (no blocked cells) and cases where start and target are the same. You may assume the start and target cells are always passable.

// The problem is a variant of BFS on a grid where each state is a cell, and transitions are slides in four directions. Directly generating all intermediate cells along a slide would be too slow (up to O(n) per edge, leading to O(n^2) per cell). Instead, we observe that for each row and each column, we can precompute the nearest blocked cell (or boundary) to the left/right and up/down. Then, from a given cell, sliding in a direction goes to the furthest passable cell before a blocked cell or boundary. A key optimization: when expanding from a cell, we can move directly to that maximal cell, but we must be careful to not push redundant states—if a cell is reached from the same horizontal or vertical line with a shorter distance, we can still process it, but to avoid O(n^2) edges we can use a technique: for each direction, we only push the farthest cell, but we also need to mark all intermediate cells as visited? Actually, standard BFS would still be O(n^2) if we push every intermediate cell. Instead, we can use a trick: for each cell, we only consider pushing the farthest cell in each direction, and we mark that farthest cell as visited. But is that correct? Yes, because if we slide from A to B, any cell C between A and B is reachable in the same number of moves as B, but reaching B is at least as good because from B you have more options (you can always stop at C, but going to B gives you more flexibility). However, we must ensure that when we later process B, we don't lose possible moves through intermediate cells. Actually, we can use a BFS where each state is a pair (cell, direction) but that's overkill. The standard approach: For each cell, we compute the maximal reach in each direction. We push that maximal cell. But we must also allow stopping earlier—but since we can stop anywhere, the shortest path to an intermediate cell is at most the same as to the farthest cell. So we can just consider jumping to the farthest cell in each direction. To avoid processing the same cell multiple times from the same direction, we can keep a visited array of booleans. However, there's a subtle issue: If we jump from A to B (farthest right), and later from C (below B) we jump up to D, and D is the same as an intermediate cell on the slide from A? Actually, we only push farthest cells, not intermediates, so we don't lose because any path that goes to an intermediate cell can be simulated by going to the farthest and then sliding back? But sliding back counts as another move. So that doesn't work. Hence we need a different approach.
//
// The correct efficient approach: Use BFS, but for each cell, when moving in a direction, we query precomputed "next blocked" positions. We push all cells along that slide? That would be O(n) per cell, total O(n^3) worst-case. But we can do better: Use a queue and a `visited` matrix, and for each direction, we slide step by step but only push the first unreached cell? Actually, we can use a technique: when sliding in a direction, we can skip over already visited cells. Since each cell is visited at most once, the total number of edge relaxations across the whole BFS is O(n^2) if we use a pointer that moves monotonically. But simpler: The grid size is up to 1000, so O(n^2) is fine (1e6). A naive BFS that for each cell iterates in four directions and slides until blocked, but marks each intermediate cell as visited and pushes it? That would be O(n^3) if each cell slides across the entire grid. However, since we only push each cell once, the total number of slides across all cells is at most O(n^2) per direction? Actually, each slide from a cell visits a sequence of cells; if we push all intermediate cells, then each cell gets pushed once, and the total work is still O(n^2) because each cell is processed once and for each processed cell we slide to the boundary, which can be O(n) per cell, leading to O(n^3) worst-case (e.g., empty grid). So we need to avoid that.
//
// The standard solution for "Sliding Puzzle" type problems uses BFS with "jump to farthest" but also maintains a "visited" per cell, and we process each cell once. The key is that if we push the farthest cell, we don't push intermediate cells, but that's not correct because we might need to stop midway. However, we can think: the shortest path to any cell is at most to the farthest. If we only push farthest, we might miss a path that requires stopping at an intermediate cell to change direction. Example: grid with a wall in the middle; you slide right, then stop before the wall to go down. If we always slide to the farthest (the cell before the wall), that's exactly the correct stopping point. Actually, the farthest cell in a direction is the cell immediately before a blocked cell or boundary. Any intermediate cell is before that, so sliding all the way to the farthest gives you the same ability to turn (since you can also turn at intermediate cells, but sliding all the way is at least as good because you can't slide further). Wait, if you slide to farthest, you're at a cell that is further along the same line; you can still move perpendicular from there, but if you wanted to move perpendicular from an earlier cell, you could have stopped there. Since you only count one move for the entire slide, going to the farthest gives you more options (you can always "turn back" but that costs another move). So indeed, sliding to the farthest is always at least as good as stopping earlier, because any move you could make from an intermediate cell, you can make from the farthest cell by first sliding back? No, sliding back costs a move. So not equivalent.
//
// Consider a grid: 1D line, start at 0, target at 5, wall at 6. You can slide right one move to position 5, which is farthest. That's fine. Suppose there's a perpendicular path from position 3. If you slide to 5, you miss the path at 3. But you can slide to 5 and then slide left back to 3, costing 2 moves, while directly sliding to 3 would cost 1 move. So shortest path to 3 is 1, but we would get 2. So we need to consider intermediate cells. Therefore, we cannot just jump to farthest.
//
// The correct efficient method: Use BFS, but when sliding, we do not push every cell; instead, we process cells one by one, but we can skip cells that have already been visited. Since each cell is visited at most once, and for each direction we slide until we hit a visited cell, the total number of steps across all slides is O(n^2) because each time we move into a new cell, we either visit it or hit a wall. Actually, a common technique: For each cell, we maintain four "next" pointers (like in DFS) to the nearest unvisited cell in each direction. But simpler: Use BFS with a `visited` matrix, and for each direction, we loop from current cell outward, and if we hit an unvisited cell, we push it and continue; if we hit a visited cell or wall, we break. This way, each cell is pushed exactly once, and each edge (slide) is traversed at most twice per direction? The total number of iterations across all BFS loops is O(n^2) because each cell becomes visited once, and the sliding loop stops at the first visited cell. However, in the worst case, consider an empty grid: from start, we slide right and visit all cells to the right, then from each of those, we slide left to start (but start is visited, so we break immediately), and slide up/down similarly. The total work is O(n^2) because each slide visits new cells only once. More precisely, for each direction from each cell, we move until we hit a visited cell or wall; the number of moves per direction per cell is bounded by the number of cells that become visited in that direction, and since each cell becomes visited once, the total is O(n^2). So a simple BFS with that stopping condition works in O(n^2) time. We'll implement that.
//
// Edge cases: start equals target, return 0. Unreachable, return -1. The grid may contain only '.' cells, so sliding continues to boundary. BFS will visit all cells. Complexity: O(n^2) time, O(n^2) space for visited and grid (grid given as input, we can store copy). The number of moves can be up to n^2, so int is fine.

#include <vector>
#include <queue>
#include <string>

// Return minimum moves to reach target using sliding moves, or -1 if unreachable.
int minMovesInSlidingGrid(int n, const std::vector<std::string>& grid,
                          int startX, int startY, int targetX, int targetY) {
    if (startX == targetX && startY == targetY) return 0;

    std::vector<std::vector<bool>> visited(n, std::vector<bool>(n, false));
    const int dx[4] = {-1, 0, 1, 0};
    const int dy[4] = {0, 1, 0, -1};

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));

    visited[startX][startY] = true;
    dist[startX][startY] = 0;
    q.push({startX, startY});

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        int d = dist[x][y];

        for (int dir = 0; dir < 4; ++dir) {
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            // Slide in this direction until blocked or visited cell.
            while (nx >= 0 && nx < n && ny >= 0 && ny < n &&
                   grid[nx][ny] != 'X') {
                if (!visited[nx][ny]) {
                    visited[nx][ny] = true;
                    dist[nx][ny] = d + 1;
                    if (nx == targetX && ny == targetY) return d + 1;
                    q.push({nx, ny});
                } else {
                    // If we hit a visited cell, we cannot go further in this
                    // direction because all cells beyond are also visited
                    // (or that cell was reached with a shorter/equal distance).
                    break;
                }
                nx += dx[dir];
                ny += dy[dir];
            }
        }
    }
    return -1;
}

#include <cassert>
#include <vector>
#include <string>

int minMovesInSlidingGrid(int n, const std::vector<std::string>& grid,
                          int startX, int startY, int targetX, int targetY);

int main() {
    // Single cell, start equals target
    assert(minMovesInSlidingGrid(1, {"."}, 0, 0, 0, 0) == 0);

    // Simple 2x2 open grid
    assert(minMovesInSlidingGrid(2, {"..", ".."}, 0, 0, 1, 1) == 2);

    // Blocked center, need two moves
    assert(minMovesInSlidingGrid(3, {"...", ".X.", "..."}, 0, 0, 2, 2) == 2);

    // Unreachable due to wall surrounding
    assert(minMovesInSlidingGrid(3, {".X.", "X.X", ".X."}, 0, 0, 2, 2) == -1);

    // Slide across open row
    assert(minMovesInSlidingGrid(1, {"....."}, 0, 0, 0, 4) == 1);

    // Start and target separated by obstacle, need detour
    assert(minMovesInSlidingGrid(4, {"....", ".XX.", "....", "...."}, 0, 0, 3, 3) == 4);

    // Larger grid, target reachable in one slide vertically
    assert(minMovesInSlidingGrid(5, {".....", ".....", ".....", ".....", "....."}, 0, 0, 4, 0) == 1);

    // Wall blocking direct slide, need multiple moves
    assert(minMovesInSlidingGrid(3, {"...", "..X", "..."}, 0, 0, 2, 2) == 4);

    // Empty grid 1000x1000, from corner to opposite corner
    std::vector<std::string> big(1000, std::string(1000, '.'));
    assert(minMovesInSlidingGrid(1000, big, 0, 0, 999, 999) == 2); // slide right, then down

    // Ensure function works with const and returns -1 for isolated start
    assert(minMovesInSlidingGrid(3, {"X.X", ".X.", "X.X"}, 1, 0, 1, 2) == -1);
    return 0;
}
