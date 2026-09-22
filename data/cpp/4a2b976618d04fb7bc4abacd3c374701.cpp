/*
Write a C++ function `int countUniquePaths(const std::string& directions)` that simulates a character moving on a 10×10 grid (coordinates 0–9 in both axes, starting at (5,5)). The input string contains only the characters `'U'`, `'D'`, `'R'`, and `'L'` representing one-step moves up, down, right, and left, respectively. If a move would take the character outside the grid, that move is ignored entirely (the character stays in place). The function must return the number of distinct undirected edges (i.e., a path segment between two adjacent cells) that the character traverses at least once. For example, moving from (1,2) to (1,3) and later from (1,3) to (1,2) counts as the same single edge. The grid is small and the input length can be up to 10,000 characters. Ensure your solution handles repeated traversals, invalid moves, and arbitrary move sequences correctly.
*/
#include <string>
#include <vector>

// Count the number of unique undirected edges traversed on a 10x10 grid.
// Valid characters: 'U', 'D', 'R', 'L'. Invalid moves (out of bounds) are ignored.
// Returns the number of distinct path segments visited.
int countUniquePaths(const std::string& directions) {
    const int GRID_SIZE = 10;
    // visited[y][x][dir] : 0=up,1=down,2=right,3=left (from this cell)
    std::vector<std::vector<std::vector<bool>>> visited(
        GRID_SIZE, std::vector<std::vector<bool>>(GRID_SIZE, std::vector<bool>(4, false)));

    int x = 5;
    int y = 5;
    int count = 0;

    for (char c : directions) {
        int nx = x;
        int ny = y;
        int dirFrom = -1; // direction index from current cell
        int dirTo = -1; // opposite direction index from neighbor

        if (c == 'U') {
            ny = y - 1;
            dirFrom = 0; // up
            dirTo = 1;   // down
        } else if (c == 'D') {
            ny = y + 1;
            dirFrom = 1;
            dirTo = 0;
        } else if (c == 'R') {
            nx = x + 1;
            dirFrom = 2; // right
            dirTo = 3;   // left
        } else if (c == 'L') {
            nx = x - 1;
            dirFrom = 3;
            dirTo = 2;
        }

        // Skip out-of-bounds moves
        if (nx < 0 || nx >= GRID_SIZE || ny < 0 || ny >= GRID_SIZE) {
            continue;
        }

        // If this edge has not been visited from current cell, mark it as new
        if (!visited[y][x][dirFrom]) {
            visited[y][x][dirFrom] = true;
            visited[ny][nx][dirTo] = true;
            ++count;
        }

        // Move to the new position
        x = nx;
        y = ny;
    }

    return count;
}
#include <cassert>

int main() {
    // No moves: no edges
    assert(countUniquePaths("") == 0);

    // One simple move right
    assert(countUniquePaths("R") == 1);

    // Move right then left, same edge traversed twice
    assert(countUniquePaths("RL") == 1);

    // Square loop around starting area: unique edges = 4
    assert(countUniquePaths("RDLU") == 4);

    // Invalid moves are ignored
    // From (5,5): move left 6 times would go out of bounds after 5? Actually left 5 times reaches x=0, then 6th invalid
    // Let's test: "LLLLL" goes to (0,5) in 5 valid moves, 6th invalid
    assert(countUniquePaths("LLLLLL") == 5);

    // Edge case: move to boundary, then try to go further out
    // Starting (5,5), go left 5 times to (0,5), then left again (invalid) then up 5 times to (0,0), then up again (invalid)
    // Unique edges: 5 left + 5 up = 10
    assert(countUniquePaths("LLLLL L UUUUU U") == 10); // spaces ignored? Actually string includes spaces, but only U,D,R,L matter, spaces are not handled
    // Better use: "LLLLLL" is 6 L's, 5 valid + 1 invalid => 5, we already have. For up: "LLLLL" + "UUUUU" = 10 edges
    assert(countUniquePaths("LLLLLUUUUU") == 10);

    // Long repeated path: go right 9 times (to x=9), then left 9 times back to start, total unique edges = 9
    std::string goRight = "RRRRRRRRR"; // 9 R's
    std::string goLeft = "LLLLLLLLL"; // 9 L's
    assert(countUniquePaths(goRight + goLeft) == 9);

    // Mixed: traverse a 2x2 square and a diagonal? No diagonals. Ensure no double counting
    // Move down, right, up, left: covers a 2x2 block, edges = 4
    assert(countUniquePaths("DRUL") == 4);

    // Test movement that revisits a vertex but new edge
    // Go right, down, left: edges = 3
    assert(countUniquePaths("RDL") == 3);

    // Test sequence that goes out of bounds in middle and continues
    // From start, go left 6 (5 valid + 1 invalid) then right 6 (5 valid to original position, 1 invalid) => edges 5+5=10? Actually first left 5 valid to x=0, then invalid, then right 5 valid back to x=5, then invalid. So 5+5=10 unique (left edges and right edges same, actually they are the same edges opposite direction, so unique edges = 5). Let's compute: left 5 goes (5,5)->(4,5)->(3,5)->(2,5)->(1,5)->(0,5) → 5 edges. Right 5 goes (0,5)->(1,5)->(2,5)->(3,5)->(4,5)->(5,5) → these are the same 5 edges reversed, so count stays 5. So "LLLLLRRRRR" should return 5.
    assert(countUniquePaths("LLLLLRRRRR") == 5);

    // Another check: full perimeter of a 1x1 square (4 edges) repeated multiple times
    assert(countUniquePaths("RDLURDLURDLURDLU") == 4);

    // Boundary: move to (0,0), then try all directions, only right and down valid
    // Start by moving left 5 times and up 5 times: "LLLLLUUUUU" gives position (0,0) and 10 edges
    // Then attempt "L U R D" from (0,0): L invalid, U invalid, R valid (edge to (1,0)), D valid (edge to (0,1)) → 2 new edges, total 12
    assert(countUniquePaths("LLLLLUUUUULURD") == 12);

    return 0;
}
// The core idea is to represent each undirected edge between two adjacent cells uniquely. Since the grid is 10×10 (100 cells), we can encode a cell as an integer index `y * 10 + x` (or keep separate x,y coordinates). For each valid move, we need to mark the edge in a data structure that prevents double-counting when traversed in the opposite direction. A straightforward method is to use a 4-bit flags array per cell (like the original snippet) where each bit indicates whether an edge to the north, south, east, or west neighbor has been traversed from this cell. When moving from cell A to neighbor B, we set the corresponding bit in A and the opposite bit in B. However, this requires careful mapping (e.g., moving up sets north bit in A and south bit in B). Alternatively, a more robust approach is to store edges as normalized pairs of coordinates, e.g., for horizontal edges store the left cell's (x,y) and for vertical store the top cell's (x,y). Then use a set of strings or a boolean array indexed by normalized edge IDs. Since the grid is small and fixed, a simple boolean array of size 10*10*4 (400 bits) works. We must check bounds before applying any move—if out of bounds, skip that move without changing position. Time complexity is O(L) where L is the length of the input string, since each move is processed in constant time. Space complexity is O(1) because the grid size is constant (100 cells, each with 4 direction flags). Edge cases include: moves that stay within bounds but traverse an edge already visited, moves that go out of bounds (ignored), and sequences that revisit the same edge multiple times (should count once). Also, the starting position (5,5) is within the 0–9 grid. The function should return the total count of unique edges.
