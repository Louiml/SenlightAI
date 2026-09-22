// Write a C++ function `int minTeleportSteps(int start, int target)` that, given two non-negative integers `start` and `target` (both less than 100,001), returns the minimum number of moves needed to reach `target` from `start`. In one move, you may either move from your current position `x` to `x-1`, `x+1`, or `2*x`. Positions must always remain within the inclusive range `[0, 100,000]`. If `start == target`, return `0`. The function must use a breadth-first search (BFS) approach and must not rely on any global or static variables that persist between calls.

#include <cassert>

int main() {
    // Basic cases
    assert(minTeleportSteps(5, 17) == 4);  // 5→10→9→18→17 (or 5→4→8→16→17)
    assert(minTeleportSteps(0, 0) == 0);
    assert(minTeleportSteps(0, 1) == 1);   // 0→1
    assert(minTeleportSteps(1, 0) == 1);   // 1→0
    assert(minTeleportSteps(1, 2) == 1);   // 1→2 (2*1 = 2)
    assert(minTeleportSteps(2, 1) == 1);   // 2→1
    assert(minTeleportSteps(3, 3) == 0);

    // Boundary positions
    assert(minTeleportSteps(100000, 100000) == 0);
    assert(minTeleportSteps(100000, 99999) == 1);
    assert(minTeleportSteps(100000, 99998) == 2); // 100000→99999→99998
    assert(minTeleportSteps(0, 100000) == 22);    // Known minimum via doubling then subtracting

    // Larger target where doubling helps
    assert(minTeleportSteps(1, 100000) == 21); // Doubling 16 times to 65536, then adjust

    // From near the top, best is move left
    assert(minTeleportSteps(99999, 100000) == 1);
    assert(minTeleportSteps(99998, 100000) == 2); // 99998→99999→100000

    // Symmetric-ish case
    assert(minTeleportSteps(10, 10) == 0);
    assert(minTeleportSteps(10, 11) == 1);
    assert(minTeleportSteps(10, 9) == 1);

    // Test that revisiting is avoided (no infinite loop)
    assert(minTeleportSteps(7, 1) == 6); // 7→6→5→4→3→2→1 (or other path with 6 moves)

    return 0;
}

#include <queue>
#include <vector>
#include <utility>

const int MAX_POS = 100001;

// Returns the minimum number of moves to reach 'target' from 'start'
// using moves x-1, x+1, or 2*x, staying within [0, 100000].
int minTeleportSteps(int start, int target) {
    if (start == target) return 0;

    std::vector<bool> visited(MAX_POS, false);
    std::queue<std::pair<int, int>> bfsQueue; // {position, steps}

    visited[start] = true;
    bfsQueue.push({start, 0});

    while (!bfsQueue.empty()) {
        auto current = bfsQueue.front();
        bfsQueue.pop();

        int pos = current.first;
        int steps = current.second;

        int nextSteps = steps + 1;
        int candidates[3] = {pos - 1, pos + 1, pos * 2};

        for (int nextPos : candidates) {
            if (nextPos >= 0 && nextPos < MAX_POS && !visited[nextPos]) {
                if (nextPos == target) {
                    return nextSteps;
                }
                visited[nextPos] = true;
                bfsQueue.push({nextPos, nextSteps});
            }
        }
    }

    return -1; // Should never be reached because target is always reachable.
}

// The problem is a classic shortest-path search on an unweighted graph where each node is a position in `[0, 100000]` and edges connect `x` to `x-1`, `x+1`, and `2*x` (when those neighbors are in bounds). BFS guarantees the minimum number of moves because all edges have equal weight (1). We initialize a queue with `{start, 0}` and a visited boolean array of size 100001 (set to false initially). We mark `start` as visited and push it. While the queue is not empty, we pop the front element. If its position equals `target`, return its move count. Otherwise, for each of the three possible next positions (in order: `x-1`, `x+1`, `2*x`), we check that it is within `[0, 100000]` and not visited. If valid, we mark it visited and push `{nextPos, count+1}`. BFS explores all positions at distance `d` before any at distance `d+1`, so the first time we encounter `target` is the minimum. Edge cases: if `start == target`, BFS immediately returns 0 (the initial pop). If `start` is 0, `x-1` is invalid but `x+1` and `2*x` are valid; if `start` is 100000, `x+1` and `2*x` are invalid but `x-1` is valid. The visited array prevents revisiting nodes, which is essential because the graph has cycles (e.g., 2 → 1 → 2), and without it BFS could loop indefinitely. Time complexity is O(V + E) where V = 100001 and E ≤ 3*V, so O(V). Space complexity is O(V) for the visited array and queue.
