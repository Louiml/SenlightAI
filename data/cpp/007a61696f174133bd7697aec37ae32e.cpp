// Write a C++ function `int minSecondsToReachTarget(int start, int target)` that solves the following problem: Given a starting position `start` (0 ≤ start ≤ 100000) and a target position `target` (0 ≤ target ≤ 100000) on a number line, you can move from an integer `x` to either `x-1`, `x+1`, or `2*x` in exactly one second. Determine the minimum number of seconds required to reach the target from the start. If the start equals the target, return 0. The function must handle positions strictly within the range 0 to 100000 inclusive for intermediate moves (that is, you cannot move to a position outside this range, even if it would be `2*x`). The solution is expected to be efficient even when the start and target are far apart.
#include <cassert>

int main() {
    // Basic cases
    assert(minSecondsToReachTarget(0, 0) == 0);
    assert(minSecondsToReachTarget(5, 5) == 0);
    assert(minSecondsToReachTarget(1, 2) == 1); // 1 -> 2 (*2)
    assert(minSecondsToReachTarget(2, 1) == 1); // 2 -> 1 (-1)

    // Standard BFS example (similar to classic "Hide and Seek" problem)
    assert(minSecondsToReachTarget(5, 17) == 4); // 5->10->9->18->17 or 5->4->8->16->17
    assert(minSecondsToReachTarget(0, 1) == 1); // 0 -> 1 (+1)
    assert(minSecondsToReachTarget(100000, 0) == 17); // Known optimal: halve and adjust

    // Edge with large target via doubling and then removing
    assert(minSecondsToReachTarget(3, 10) == 3); // 3->6->7->10 or 3->4->8->10 (both 3 steps)

    // Target far above start, but doubling is efficient
    assert(minSecondsToReachTarget(1, 100000) == 24); // 1->2->4->...->65536 (16 doublings) then adjustments

    return 0;
}
#include <vector>
#include <queue>

// Returns the minimum number of seconds to reach 'target' from 'start'
// using moves x-1, x+1, or 2*x, with positions restricted to [0, 100000].
int minSecondsToReachTarget(int start, int target) {
    if (start == target) {
        return 0;
    }

    const int MAX_POS = 100000;
    std::vector<int> dist(MAX_POS + 1, -1); // -1 means unvisited
    std::queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        // Three possible moves: -1, +1, *2
        int nextPositions[3] = {cur - 1, cur + 1, cur * 2};

        for (int i = 0; i < 3; ++i) {
            int nxt = nextPositions[i];
            // Check bounds and whether unvisited
            if (nxt >= 0 && nxt <= MAX_POS && dist[nxt] == -1) {
                dist[nxt] = dist[cur] + 1;
                if (nxt == target) {
                    return dist[nxt];
                }
                q.push(nxt);
            }
        }
    }

    // According to problem constraints, target is always reachable
    return -1; // fallback (should never happen)
}
// This is a classic shortest-path problem on an unweighted graph where each state (a position) has up to 3 neighbors. Since all moves have equal cost (1 second), we can use Breadth-First Search (BFS) to find the minimum number of steps. We maintain a queue of positions to visit and a distance array `dist` (initialized to -1 for unvisited). We start by pushing the `start` position with distance 0. While the queue is not empty, we pop a position `cur`, and for each of the three possible next positions (`cur-1`, `cur+1`, `cur*2`), we check if it is within the valid range [0, 100000] and not yet visited. If valid and unvisited, we set its distance to `dist[cur] + 1`, push it into the queue, and if that position equals the target, we return that distance immediately. Since BFS expands level by level, the first time we encounter the target, we have the minimum seconds. Important edge cases: (1) If start == target, return 0 immediately. (2) The move `2*x` might exceed 100000, so we must check bounds before pushing; positions outside [0,100000] are invalid. (3) Negative positions from `cur-1` are invalid. (4) The solution correctly handles cases where the fastest path temporarily moves away from the target (e.g., start=5, target=17: 5→4→8→16→17 or 5→10→20→19→18→17? Actually BFS finds optimal). Time complexity is O(100001) since each position is visited at most once; space complexity is O(100001) for the distance array and queue.
