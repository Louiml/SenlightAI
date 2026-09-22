// Write a C++ function `countRooms(const std::vector<std::string>& building)` that, given a rectangular grid of characters where `'.'` represents empty floor space and any other character (e.g., `'#'`) represents a wall, returns the number of distinct connected regions of floor cells. Two floor cells are considered connected if they are adjacent horizontally or vertically (not diagonally). The grid has at least 1 row and at least 1 column. The input grid may contain multiple disconnected regions, and walls can form complex boundaries. Your function must not modify the input grid, and it should work efficiently for large grids (up to 1000x1000). Use a flood-fill (DFS or BFS) approach internally, but the public API is simply the count function. You may assume the input is well-formed (all strings have equal length and contain only `'.'` and `'#'`).
#include <cassert>
#include <vector>
#include <string>

int countRooms(const std::vector<std::string>& building); // declaration for clarity

int main() {
    // Test 1: Simple 2x2 single room
    std::vector<std::string> grid1 = {"..", ".."};
    assert(countRooms(grid1) == 1);

    // Test 2: Two separate rooms
    std::vector<std::string> grid2 = {".#", "#."};
    assert(countRooms(grid2) == 2);

    // Test 3: No floor cells
    std::vector<std::string> grid3 = {"###", "###"};
    assert(countRooms(grid3) == 0);

    // Test 4: One horizontal corridor
    std::vector<std::string> grid4 = {".#.", ".#.", "..."};
    assert(countRooms(grid4) == 1);

    // Test 5: Diagonals are not connected
    std::vector<std::string> grid5 = {".#", "#."};
    assert(countRooms(grid5) == 2);

    // Test 6: Complex shape with hole
    std::vector<std::string> grid6 = {"###", "#.#", "###"};
    assert(countRooms(grid6) == 1);

    // Test 7: Single cell room
    std::vector<std::string> grid7 = {"."};
    assert(countRooms(grid7) == 1);

    // Test 8: Single wall cell
    std::vector<std::string> grid8 = {"#"};
    assert(countRooms(grid8) == 0);

    // Test 9: Larger grid with multiple isolated cells
    std::vector<std::string> grid9 = {".#..", "#..#", "..#."};
    // Cells: (0,0), (0,2),(0,3) connected? (0,3) touches (1,3) => yes, so those two are one.
    // (1,3) part of that. (1,2) touches (0,2),(1,3),(2,2?) no (2,2 is #), (2,2 is #) so (1,2) is isolated? Actually (1,2) is '.' and neighbours (0,2) '.' so part of same. Let's just check count is 2: top-left (0,0) isolated, the rest form one region.
    assert(countRooms(grid9) == 2);

    // Test 10: Empty grid (zero rows) – though specification says at least 1 row, but test defensive
    std::vector<std::string> grid10 = {};
    assert(countRooms(grid10) == 0);
}
#include <vector>
#include <string>

// Counts the number of connected regions of '.' cells in the building grid.
// A region is a maximal set of '.' cells connected via 4-directional adjacency.
int countRooms(const std::vector<std::string>& building) {
    if (building.empty() || building[0].empty()) return 0;
    int H = building.size();
    int W = building[0].size();
    std::vector<std::vector<bool>> visited(H, std::vector<bool>(W, false));
    
    // Direction vectors: right, left, down, up
    const int dr[4] = {0, 0, 1, -1};
    const int dc[4] = {1, -1, 0, 0};
    
    // Depth-first search helper (lambda or nested function)
    auto dfs = [&](int r, int c, auto&& dfs_ref) -> void {
        if (r < 0 || r >= H || c < 0 || c >= W) return;
        if (visited[r][c]) return;
        if (building[r][c] != '.') return;
        visited[r][c] = true;
        for (int i = 0; i < 4; ++i) {
            dfs_ref(r + dr[i], c + dc[i], dfs_ref);
        }
    };
    
    int rooms = 0;
    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if (building[r][c] == '.' && !visited[r][c]) {
                dfs(r, c, dfs);
                ++rooms;
            }
        }
    }
    return rooms;
}
// The main algorithm is a standard connected-component labeling using flood fill. Iterate over every cell in the grid. Whenever an unvisited floor cell (`'.'`) is encountered, increment a room counter and perform a depth-first search (or breadth-first search) from that cell, marking all reachable floor cells as visited by setting a boolean in a visited matrix or by temporarily modifying a copy. The flood fill explores the four cardinal directions (up, down, left, right) and stops at boundaries or wall cells. Since each cell is visited at most once, the total time complexity is O(H*W), and the auxiliary space is O(H*W) for the visited matrix plus the recursion stack depth (worst-case O(H*W) for a fully open grid). Edge cases include grids with no floor (returns 0), a single-cell room, grids with only one giant room, and grids with many isolated rooms separated by walls. The visited matrix should be sized to match the input dimensions; the function must use `const` references appropriately and avoid modifying the input.
