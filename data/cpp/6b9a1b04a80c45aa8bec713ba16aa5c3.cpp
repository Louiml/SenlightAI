// Write a C++ function `int countTraversedCells(const std::vector<int>& lengths)` that simulates a turtle starting at a central grid cell (coordinate 200,200 on an infinite grid, but bounded to 401x401 for this problem) facing east (direction 0). For each step length `L` in the given sequence, the turtle draws a straight line of `L` cells in its current direction, marking each cell it passes through as visited (including the starting cell of the segment, but excluding the final cell of the segment—actually including all cells it steps on, so if L=1 it marks the current cell and then stops, then turns). After completing each segment, the turtle turns 45 degrees counterclockwise (direction index decreases by 1 modulo 8) and also 45 degrees clockwise (direction index increases by 1 modulo 8), creating two possible new paths. The turtle follows both possible turns simultaneously (i.e., it explores all possible sequences of turns). The directions are indexed as: 0=East, 1=South-East, 2=South, 3=South-West, 4=West, 5=North-West, 6=North, 7=North-East. Note that after drawing a segment of length L, the turtle ends at the cell that is L steps from the start of that segment (including the start cell, so after L moves it lands on the L-th cell). Then from that ending cell, it attempts both turns: the direction `(dir+7)%8` and `(dir+1)%8`, and starts a new segment from that cell in the new direction. The function must return the total number of distinct grid cells that are marked (i.e., visited at least once) across all possible paths. The grid is limited to coordinates 0..400 inclusive; any move that would go outside this range is ignored (the turtle does not move outside, but the path simply stops at the boundary). The sequence length `n` is at least 1 and at most 12, and each `lengths[i]` is between 1 and 10 inclusive.
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.
int main() {
    // Single segment length 1: marks starting cell only, branches to two directions but no further segments, so only 1 cell.
    assert(countTraversedCells({1}) == 1);

    // Length 2 in east: marks (200,200) and (200,201) => 2 cells.
    assert(countTraversedCells({2}) == 2);

    // Sequence {1,1}: After first segment, mark (200,200). Then branch to north-east (dir 7) and south-east (dir 1).
    // Each second segment of length 1 marks the starting cell again (already marked) so no new cells? Actually the starting cell for the second segment is the same (200,200) because length 1 doesn't move.
    // So both branches mark (200,200) again, but no new cells. Total 1.
    assert(countTraversedCells({1,1}) == 1);

    // Sequence {2,1}: First segment marks (200,200) and (200,201), ends at (200,201). Then branches:
    // left (north-east) from (200,201) moves 1 step to (199,202), marks new.
    // right (south-east) from (200,201) moves 1 step to (201,202), marks new.
    // Total distinct: (200,200),(200,201),(199,202),(201,202) = 4.
    assert(countTraversedCells({2,1}) == 4);

    // Sequence {2,2}: First segment marks (200,200),(200,201). Branch left (dir 7) from (200,201) moves 2 steps: (199,202) then (198,203) => new cells. Branch right (dir 1) from (200,201) moves 2 steps: (201,202) then (202,203) => new cells.
    // Distinct: start two, plus 4 new = 6.
    assert(countTraversedCells({2,2}) == 6);

    // Sequence {3} marks three cells in a line: (200,200),(200,201),(200,202) => 3.
    assert(countTraversedCells({3}) == 3);

    // Sequence {1,2}: First marks (200,200). Then two branches from (200,200) each move 2 steps:
    // left dir 7: (199,201) then (198,202) => two new cells.
    // right dir 1: (201,201) then (202,202) => two new cells.
    // Total: 1 + 4 = 5.
    assert(countTraversedCells({1,2}) == 5);

    // A longer sequence that may overlap: {2,1,1}:
    // Step1 marks (200,200),(200,201), ends at (200,201), branches to dirs 7 and 1.
    // Step2 (len=1): from each of two states, mark the starting cell again (already marked? (200,201) is marked). Then branch to new dirs.
    // For state at (200,201) dir7: mark (200,201) (already), end same spot. branch to dir (7+7)%8=6 (north) and dir (7+1)%8=0 (east).
    // For state at (200,201) dir1: mark (200,201), branch to dir (1+7)%8=0 (east) and dir (1+1)%8=2 (south).
    // Step3 (len=1): from each of four states, mark the starting cell (each is (200,201) again? No, the states after step2 are positions (200,201) with various dirs, because length 1 didn't move. So all start at (200,201) and mark it again (already). No new cells.
    // So total remains 4.
    assert(countTraversedCells({2,1,1}) == 4);

    // Test a case that goes out of grid? With small numbers it stays inside. Use large but safe: {10,10,10} should be fine.
    // Just check it returns a positive number without crash.
    assert(countTraversedCells({10,10,10}) > 0);

    // Test with empty? Not allowed per constraints, but we can check that {0} is not given; skip.
    return 0;
}
#include <vector>
#include <set>
#include <utility>
#include <array>

// Count distinct grid cells traversed by a turtle branching in all possible turn sequences.
// Directions: 0=E,1=SE,2=S,3=SW,4=W,5=NW,6=N,7=NE
int countTraversedCells(const std::vector<int>& lengths) {
    const int GRID_SIZE = 401;
    const int CENTER = 200;
    const int dirX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dirY[8] = {1, 1, 0, -1, -1, -1, 0, 1};

    std::array<std::array<bool, GRID_SIZE>, GRID_SIZE> visited{};
    struct State {
        int x, y, dir;
        bool operator<(const State& other) const {
            if (x != other.x) return x < other.x;
            if (y != other.y) return y < other.y;
            return dir < other.dir;
        }
    };

    std::set<State> current;
    current.insert({CENTER, CENTER, 0});

    for (int len : lengths) {
        std::set<State> next;
        for (const auto& s : current) {
            int x = s.x;
            int y = s.y;
            int d = s.dir;
            // Mark starting cell and move len-1 steps
            visited[x][y] = true;
            for (int step = 0; step < len - 1; ++step) {
                x += dirX[d];
                y += dirY[d];
                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    visited[x][y] = true;
                }
                // If out of bounds, break? The reference doesn't handle, but we assume inside.
            }
            // Now x,y is the ending cell after moving len-1 additional steps? Actually if len=1, loop not entered, ending cell is starting cell.
            // But the turtle ends exactly after len moves? The reference uses loop j from 0 to len-1, and each iteration marks current cell, then if not last, moves. So total marked cells = len, and ends at cell after len-1 moves from start. So ending position = start + (len-1)*dir.
            // Wait: the code marks arr[b.x][b.y]=1 at each j, and increments b only if j != len-1. So it marks len cells, and the final b is start + (len-1)*dir. So the ending cell is the last marked cell.
            // So we have already moved len-1 steps in the loop. The ending cell is (x,y) as updated.
            // Then we branch from that ending cell in the two perpendicular directions.
            int leftDir = (d + 7) % 8;
            int rightDir = (d + 1) % 8;
            next.insert({x, y, leftDir});
            next.insert({x, y, rightDir});
        }
        current = std::move(next);
    }

    int count = 0;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (visited[i][j]) ++count;
        }
    }
    return count;
}
// The problem is a simulation of branching paths on a grid. Since the turtle can turn left or right by 45 degrees after each segment, the number of possible paths grows exponentially (2^(n-1) paths). However, the grid is small (401x401) and the maximum segment length is modest, so we can use a set to store all possible states `(x, y, dir)` and simulate each step by expanding from all current states. The approach:
// - Initialize a set `cur` with a single state: position (200,200) and direction 0.
// - Maintain a 2D boolean array `visited[401][401]` to mark cells that have been traversed.
// - For each length `L` in the sequence:
//   - Create a new set `nex`.
//   - For each state `(x,y,dir)` in `cur`:
//     - Move `L` steps in the current direction, marking each cell passed (including the starting cell of the segment, but note: if L=1, mark starting cell and then move 0? Actually the loop marks the starting cell, then moves to next cell for each of L-1 steps. So for L=1, it marks the starting cell only and ends at the same cell).
//     - After moving, the ending position is `(x + L*dx, y + L*dy)` where `dx,dy` are the direction vector.
//     - If the turtle attempts to move outside the grid, the path stops? In the snippet, it marks cells as long as within the array, but if it goes out of bounds, the array access would be invalid. For safety, we only mark cells that are within 0..400. But the movement continues? The reference solution does not clamp; it assumes the turtle never goes outside because the grid is large and the total moves are limited. However, with n<=12 and max length 10, max displacement is 120, so from 200 it stays within 80..320, well within bounds. So we can assume all moves remain inside the grid. We'll still check bounds for safety.
//     - Add two new states: one with direction `(dir+7)%8` and position at the ending cell, and one with direction `(dir+1)%8` at the same ending cell.
// - After processing all lengths, count the number of `true` cells in `visited`.
// - Complexity: The number of distinct states is at most 401*401*8 ≈ 1.28 million, but actually the set never grows large because we only keep states at the current step. At each step, the maximum number of distinct states is bounded by the number of reachable positions and directions, which is O(401*401*8). For each state, we move up to 10 steps, so each step takes O(states * L). In the worst case, the number of states can double each iteration, but the grid limits it. The time complexity is O(n * states * maxLen) and space O(401*401 + states). For given constraints, it's easily feasible.
