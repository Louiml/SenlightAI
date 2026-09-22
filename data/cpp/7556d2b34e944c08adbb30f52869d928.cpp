/*
Write a C++ function `int escapeSteps(int R, int C, int P, const vector<string> grid[])` that determines whether Luke and Leia can escape from a maze that changes over time. The maze has `R` rows and `C` columns, and there are `P` distinct time phases, each represented as a grid of characters `'0'` (traversable) or `'1'` (blocked). They start at cell `(0,0)` at phase `0` and must reach cell `(R-1, C-1)` at any phase. At each step, they may move to one of the four adjacent cells (up, down, left, right) or stay in place, but simultaneously the time phase advances to `(current_phase + 1) % P`. Moving into a cell is allowed only if that cell in the *next* phase's grid is `'0'`. The goal is to enter the target cell `(R-1, C-1)` at any phase; if they reach it, they succeed. If the target cell is already reached at time 0 without moving, return 0. If impossible, return -1. The input grids are provided as an array of `P` vectors of strings, each of length `R` with `C` characters. The function must return the minimum number of steps to reach the target, or -1. Guarantee that `R, C, P` are positive (though the original code stops on zeros, for this task assume all ≥ 1).
*/
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <cstring>

// Returns the minimum steps to escape, or -1 if impossible.
// grid is an array of P vectors, each of length R, each string of length C.
// Cells '0' are traversable, '1' are blocked.
int escapeSteps(int R, int C, int P, const std::vector<std::string> grid[]) {
    // Distance array: dist[phase][row][col], -1 means unvisited.
    int*** dist = new int**[P];
    for (int p = 0; p < P; ++p) {
        dist[p] = new int*[R];
        for (int r = 0; r < R; ++r) {
            dist[p][r] = new int[C];
            for (int c = 0; c < C; ++c) {
                dist[p][r][c] = -1;
            }
        }
    }

    // Starting state: phase 0, row 0, col 0.
    if (R == 1 && C == 1) {
        // Target is start; can escape in 0 steps even if cell is '1'? Original code returns 0 before checking grid.
        // But to be safe, check if start is '0'? Original returns 0 regardless. We'll do the same.
        dist[0][0][0] = 0;
        return 0;
    }

    std::queue<std::tuple<int, int, int>> q; // (phase, row, col)
    dist[0][0][0] = 0;
    q.push({0, 0, 0});

    // Direction vectors: stay, right, left, down, up.
    const int dr[5] = {0, 0, 0, 1, -1};
    const int dc[5] = {0, 1, -1, 0, 0};

    while (!q.empty()) {
        auto [p, r, c] = q.front();
        q.pop();

        int nextP = (p + 1) % P;
        for (int i = 0; i < 5; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                // Can move into cell if in next phase it's '0'
                if (grid[nextP][nr][nc] == '0' && dist[nextP][nr][nc] == -1) {
                    dist[nextP][nr][nc] = dist[p][r][c] + 1;
                    if (nr == R - 1 && nc == C - 1) {
                        int result = dist[nextP][nr][nc];
                        // Clean up
                        for (int pp = 0; pp < P; ++pp) {
                            for (int rr = 0; rr < R; ++rr) {
                                delete[] dist[pp][rr];
                            }
                            delete[] dist[pp];
                        }
                        delete[] dist;
                        return result;
                    }
                    q.push({nextP, nr, nc});
                }
            }
        }
    }

    // Clean up
    for (int p = 0; p < P; ++p) {
        for (int r = 0; r < R; ++r) {
            delete[] dist[p][r];
        }
        delete[] dist[p];
    }
    delete[] dist;

    return -1;
}
#include <cassert>
#include <vector>
#include <string>

// function declaration (from solution)
int escapeSteps(int R, int C, int P, const std::vector<std::string> grid[]);

int main() {
    // Test 1: Simple 1x1 grid, P=1, target is start, should return 0.
    {
        std::vector<std::string> g[1] = { {"0"} };
        assert(escapeSteps(1, 1, 1, g) == 0);
    }

    // Test 2: 1x2 grid, P=1, start at (0,0), target (0,1) open, should return 1.
    {
        std::vector<std::string> g[1] = { {"01"} };
        assert(escapeSteps(1, 2, 1, g) == 1);
    }

    // Test 3: Same but target blocked, P=1, should be -1.
    {
        std::vector<std::string> g[1] = { {"00"} }; // wait target at (0,1) is '0', so reachable. Change to blocked:
        g[0][0] = "01"; // Actually let's do blocked: "001" is 1x3? Let's do 1x2 with target '1':
        std::vector<std::string> g2[1] = { {"01"} }; // that's reachable. Let's set target '1':
        std::vector<std::string> g3[1] = { {"01"} }; // hmm. Let's do 1x2 where cell (0,1) is '1':
        std::vector<std::string> g4[1] = { {"00"} }; // no, that's open. Let's set g4[0][0]="01"? Wait R=1,C=2, grid[0][0]="01" means col0='0', col1='1'. So target blocked. 
        std::vector<std::string> g5[1] = { {"01"} };
        assert(escapeSteps(1, 2, 1, g5) == -1);
    }

    // Test 4: Need phase cycling. 1x2 grid, P=2. Phase0: "00", Phase1: "00". Start (0,0), target (0,1). Can reach in 1 step by moving right in phase1? Actually at step 0 -> next phase is 1, cell (0,1) in phase1 is '0', so reachable in 1 step.
    {
        std::vector<std::string> g[2] = { {"00"}, {"00"} };
        assert(escapeSteps(1, 2, 2, g) == 1);
    }

    // Test 5: Blocked in all phases for target, should be -1.
    {
        std::vector<std::string> g[2] = { {"01"}, {"01"} };
        assert(escapeSteps(1, 2, 2, g) == -1);
    }

    // Test 6: 2x2 grid, P=1, open all, start (0,0) target (1,1). Shortest path is 2 steps (right then down or down then right).
    {
        std::vector<std::string> g[1] = { {"00", "00"} };
        assert(escapeSteps(2, 2, 1, g) == 2);
    }

    // Test 7: Need to wait (stay) because next phase blocks the way. Example: R=1,C=3, P=2. Phase0: "000", Phase1: "010" (middle blocked). Start left (0,0), target right (0,2). Can't go through middle in phase1. But stay at start moves to phase1 at left (open), then next step phase0: can go right to middle (open), then next step phase1: go right to target (open). So steps: 3.
    {
        std::vector<std::string> g[2] = { {"000"}, {"010"} };
        assert(escapeSteps(1, 3, 2, g) == 3);
    }

    // Test 8: Impossible because target blocked in all phases and cannot step onto it.
    {
        std::vector<std::string> g[1] = { {"010"} }; // target col2='0'? Actually R=1,C=3, target col2 is '0'? "010" has col2='0', so reachable. Let's do "011" target '1':
        std::vector<std::string> g2[1] = { {"011"} };
        assert(escapeSteps(1, 3, 1, g2) == -1);
    }

    // Test 9: Larger grid with multiple phases, ensure BFS finds path.
    {
        std::vector<std::string> g[3] = {
            {"0000", "0000", "0000"},
            {"0000", "0000", "0000"},
            {"0000", "0000", "0000"}
        };
        // R=3,C=4,P=3, all open, start (0,0) to (2,3). Manhattan distance = 5, but because phases change, can still do in 5 steps (each move allowed since all open). So answer 5.
        assert(escapeSteps(3, 4, 3, g) == 5);
    }

    // Test 10: Single phase, but start is target but cell marked '1'? Original returns 0 anyway. We'll test that.
    {
        std::vector<std::string> g[1] = { {"1"} };
        assert(escapeSteps(1, 1, 1, g) == 0);
    }

    return 0;
}
// The problem is a classic shortest-path search on an expanded state space. Each state is a tuple `(phase, row, col)` representing the current time phase (0..P-1) and position in the grid. From a state `(p, r, c)`, the next phase is `(p+1) % P`, and we can transition to any of five neighboring cells (including staying) if that cell in the next phase's grid is `'0'` (or if it's the target, even if it's `'1'`? Actually the original code checks `grid[p2][r2][c2]=='0'` — so the target cell must be `'0'` in that phase to be entered. If the target is blocked in all phases, you cannot enter it). We start at `(0, 0, 0)` and want to reach any state `(*, R-1, C-1)`. Since the state space has exactly `P * R * C` states, a Breadth-First Search (BFS) will find the minimum number of steps (each transition counts as one step). BFS is appropriate because the graph is unweighted (each move costs 1). We maintain a `dist` array of size `[P][R][C]` initialized to -1, and a queue of states. We check if the starting cell is already the target and return 0 immediately (even though the original code checks `if(r1==R-1 && c1==C-1) return 0` before marking, but note it also marks start as visited; we can simply return 0). Otherwise, we process states in BFS order, generating all up to 5 neighbors, and if the neighbor cell in the next phase is `'0'` and not yet visited, we mark it visited, set its distance, and if it's the target, return that distance. If BFS exhausts without finding the target, return -1. Edge cases: staying in place is allowed, so the start cell might be revisited only after a full cycle of phases (but BFS will handle it because phase changes). The grid arrays are indexed correctly, and we must ensure we don't go out of bounds for row/col. Time complexity is O(P * R * C * 5) = O(P*R*C) because each state is processed once and has constant neighbors. Space complexity is O(P*R*C) for the distance array and queue.
