Design a C++ function that solves a sliding-puzzle variant on an `n x n` grid. The grid contains lowercase letters (`a`–`m`) representing movable tokens, and their matching uppercase letters (`A`–`M`) representing fixed targets. When the player tilts the board in one of four directions (up, down, left, right), all lowercase tokens slide simultaneously as far as possible in that direction until they hit a wall, another token, or an uppercase target. If a lowercase token slides into an uppercase letter that matches it (same letter, case-insensitive), both disappear. If it hits a non-matching uppercase letter, the tilt is invalid and the move is not allowed. Walls are specified as blocked edges between adjacent cells; tokens cannot cross these edges. The goal is to reach a board configuration where all lowercase and uppercase letters have disappeared (all cells are `.`), using the minimum number of tilts. Write a function `int minTiltsToClear(const std::vector<std::vector<char>>& initial, int n, const std::vector<std::array<int,4>>& walls)` that returns the minimum number of moves, or `-1` if impossible. Assume `n` is at least 1, the grid is square, and all token pairs are unique (each letter appears exactly once as lowercase and once as uppercase). The input includes wall edges as four-integer tuples `(x1,y1,x2,y2)`.
The problem is a state-space search. Each board configuration is a state. The initial state is given, and the target state is a board fully filled with `'.'`. Since the grid can have up to, say, 5x5 cells and a handful of tokens, the state space is finite but potentially large, so we use BFS to guarantee the minimum number of tilts. For each state, we generate up to 4 successor states by simulating a tilt in each direction. The simulation for a given direction processes rows or columns independently: for a vertical tilt (up/down) each column is processed from top or bottom, and for horizontal tilt (left/right) each row is processed from left or right. For each lowercase token, we move it step by step in the tilt direction until it either hits a wall (blocked edge), hits the boundary, hits another token (lowercase or uppercase), or hits a matching uppercase token (in which case both are removed). If it hits a non-matching uppercase, the entire move is invalid and we discard that successor. A crucial detail: when a token moves, it may pass through cells that were originally empty, but it cannot pass through other tokens; also, after removal, the board changes for subsequent tokens in the same row/column during the same tilt, but the simulation processes tokens in order from the farthest in the direction of movement to the nearest, so earlier removals do not affect later tokens because removed tokens become empty and do not block. We must also handle the edge case where a token moves into a cell that was vacated by a previously removed pair; that is fine because it is empty. The BFS explores states, using a `std::map` or `std::unordered_map` with a custom hash (or `std::map` for simplicity) to store visited states and their distances. Time complexity is O(S * n^2) where S is the number of reachable states, and space complexity is O(S * n^2) to store the state set. For a 5x5 board with up to, say, 4 tokens, S is manageable.
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <array>
#include <algorithm>

// Convert board to a string key for map lookup
std::string boardToString(const std::vector<std::vector<char>>& board) {
    std::string key;
    for (const auto& row : board) {
        for (char c : row) {
            key.push_back(c);
        }
    }
    return key;
}

// Simulate a tilt in direction: 0=up,1=down,2=left,3=right
// Returns true if move is valid, false if any token hits a non-matching uppercase
// Modifies the board in place
bool tilt(std::vector<std::vector<char>>& board, int dir, int n,
          const std::vector<std::array<int,2>>& blocked) {
    // blocked is a set of directed edges represented as (from_x, from_y, to_x, to_y) but we'll store as a map for quick check
    // For simplicity, we'll just use a lambda to check if an edge is blocked
    // Since walls are given as undirected, we'll store pairs in a set
    // Use a local static set? Better pass as a reference to a set.
    // For simplicity, we'll precompute a 4D adjacency in the main function and pass it.
    // To keep this function self-contained, we'll accept a 4D bool array adj[n][n][4] as parameter.
    // But for the task, we'll just implement a simplified version assuming no walls? No, task includes walls.
    // So we'll modify signature to accept a 4D vector.
    // Let's do that.
}

// To keep the solution clean, we'll use a different approach: precompute blocked edges in a 4D bool array.
// The function below assumes that the caller provides adjacency information as a 3D vector adj[x][y][dir] where 0..3.
// We'll implement the solution function to build that from walls.

int minTiltsToClear(const std::vector<std::vector<char>>& initial, int n,
                    const std::vector<std::array<int,4>>& walls) {
    // Build adjacency: adj[x][y][0]=up,1=down,2=left,3=right, true if move possible (no wall)
    bool adj[5][5][4] = {false};
    for (int x=0; x<n; ++x) {
        for (int y=0; y<n; ++y) {
            adj[x][y][0] = (x-1 >= 0);
            adj[x][y][1] = (x+1 < n);
            adj[x][y][2] = (y-1 >= 0);
            adj[x][y][3] = (y+1 < n);
        }
    }
    for (const auto& w : walls) {
        int x1=w[0], y1=w[1], x2=w[2], y2=w[3];
        // Remove edge from (x1,y1) to (x2,y2)
        if (x2 == x1-1) adj[x1][y1][0] = false;
        if (x2 == x1+1) adj[x1][y1][1] = false;
        if (y2 == y1-1) adj[x1][y1][2] = false;
        if (y2 == y1+1) adj[x1][y1][3] = false;
        // Also remove reverse
        if (x1 == x2-1) adj[x2][y2][0] = false;
        if (x1 == x2+1) adj[x2][y2][1] = false;
        if (y1 == y2-1) adj[x2][y2][2] = false;
        if (y1 == y2+1) adj[x2][y2][3] = false;
    }

    std::map<std::string, int> dist;
    std::queue<std::vector<std::vector<char>>> q;
    std::vector<std::vector<char>> target(n, std::vector<char>(n, '.'));
    dist[boardToString(initial)] = 0;
    q.push(initial);

    int dx[4] = {-1,1,0,0};
    int dy[4] = {0,0,-1,1};

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int d = dist[boardToString(cur)];
        if (cur == target) return d;

        for (int dir=0; dir<4; ++dir) {
            auto next = cur;
            bool valid = true;
            // Process rows or columns
            if (dir==0 || dir==1) { // up/down, process each column
                for (int y=0; y<n && valid; ++y) {
                    int start = (dir==0) ? 0 : n-1;
                    int step = (dir==0) ? 1 : -1;
                    for (int x=start; x>=0 && x<n; x+=step) {
                        if (!islower(next[x][y])) continue;
                        int cx = x;
                        while (true) {
                            int nx = cx + dx[dir];
                            if (nx<0 || nx>=n) break; // hit boundary
                            if (!adj[cx][y][dir]) break; // wall
                            if (isupper(next[nx][y])) {
                                if (toupper(next[cx][y]) != next[nx][y]) {
                                    valid = false;
                                } else {
                                    next[cx][y] = '.';
                                    next[nx][y] = '.';
                                }
                                break;
                            }
                            // If next cell is occupied by another lowercase or uppercase? Actually uppercase handled, so only lowercase or empty
                            // If occupied by lowercase, stop (can't pass through)
                            if (islower(next[nx][y])) break;
                            // else empty, move
                            std::swap(next[cx][y], next[nx][y]);
                            cx = nx;
                        }
                    }
                }
            } else { // left/right, process each row
                for (int x=0; x<n && valid; ++x) {
                    int start = (dir==2) ? 0 : n-1;
                    int step = (dir==2) ? 1 : -1;
                    for (int y=start; y>=0 && y<n; y+=step) {
                        if (!islower(next[x][y])) continue;
                        int cy = y;
                        while (true) {
                            int ny = cy + dy[dir];
                            if (ny<0 || ny>=n) break;
                            if (!adj[x][cy][dir]) break;
                            if (isupper(next[x][ny])) {
                                if (toupper(next[x][cy]) != next[x][ny]) {
                                    valid = false;
                                } else {
                                    next[x][cy] = '.';
                                    next[x][ny] = '.';
                                }
                                break;
                            }
                            if (islower(next[x][ny])) break;
                            std::swap(next[x][cy], next[x][ny]);
                            cy = ny;
                        }
                    }
                }
            }
            if (valid) {
                std::string key = boardToString(next);
                if (!dist.count(key)) {
                    dist[key] = d+1;
                    q.push(next);
                }
            }
        }
    }
    return -1;
}
#include <cassert>
#include <vector>
#include <array>

int main() {
    // Test 1: Simple 2x2 with one pair, no walls
    // a at (0,0), A at (1,1). Tilt right then down? Let's see: 
    // Initial:
    // a .
    // . A
    // Tilt right: a moves to (0,1) but no match there, so 'a.' -> '.a'? Actually tilt right on row 0: a moves to (0,1) because empty. Row 1: '.' and 'A' uppercase doesn't move. Result:
    // . a
    // . A
    // Tilt down: column 1: from bottom, token 'a' at (0,1) moves down to (1,1) which is 'A' -> match, both disappear. So 2 moves.
    std::vector<std::vector<char>> b1 = {{'a','.'},{'.','A'}};
    std::vector<std::array<int,4>> w1;
    assert(minTiltsToClear(b1, 2, w1) == 2);

    // Test 2: Already cleared
    std::vector<std::vector<char>> b2 = {{'.','.'},{'.','.'}};
    assert(minTiltsToClear(b2, 2, w1) == 0);

    // Test 3: Impossible due to wall blocking match
    // 2x2, a at (0,0), A at (1,1), but wall between (0,0)-(1,1) is impossible because walls are edge-based not diagonal. Let's make a wall that prevents moving.
    // Put a at (0,0), A at (0,1). Wall between (0,0) and (0,1) blocks right tilt. Also wall between (0,0) and (1,0) blocks down. So no moves possible -> impossible.
    std::vector<std::vector<char>> b3 = {{'a','A'},{'.','.'}};
    std::vector<std::array<int,4>> w3 = {{0,0,0,1}, {0,0,1,0}};
    assert(minTiltsToClear(b3, 2, w3) == -1);

    // Test 4: Multiple pairs, need serial moves
    // 3x3, a and b at bottom row, A and B at top row, but walls force separate moves? Let's simple: all tokens on same row move together.
    // Actually easier: a at (0,0), A at (0,2), b at (2,0), B at (2,2). No walls.
    // Tilt right: row0: a moves to (0,1) then (0,2) hits A -> both disappear. row2: b moves to (2,1) then (2,2) hits B -> both disappear. So 1 move.
    std::vector<std::vector<char>> b4 = {{'a','.','A'},{'.','.','.'},{'b','.','B'}};
    assert(minTiltsToClear(b4, 3, w1) == 1);

    // Test 5: Wall prevents simultaneous clearing, need two moves
    // Same as above but wall between (0,1) and (0,2) so a cannot reach A on first tilt right: a stops at (0,1). Then tilt right again? After first tilt: row0 becomes ". a ."? Actually initial: a . A. Tilt right: a moves to (0,1) then wall before (0,2), so stops at (0,1). Result ". a ." and A still at (0,2). Then tilt right again: a moves to (0,2) hits A -> clear. So 2 moves. But B is also there, we need to clear both. Actually row2: b moves to (2,1) then (2,2) hits B -> clear in first tilt. So after two tilts, clear all. So answer 2.
    std::vector<std::vector<char>> b5 = {{'a','.','A'},{'.','.','.'},{'b','.','B'}};
    std::vector<std::array<int,4>> w5 = {{0,1,0,2}};
    assert(minTiltsToClear(b5, 3, w5) == 2);

    return 0;
}
