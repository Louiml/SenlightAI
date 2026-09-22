/*
Write a C++ function `int minTimeToReach(const vector<vector<int>>& moveTime)` that, given a non-empty grid `moveTime` of size `m x n` where each cell contains a non-negative integer representing the earliest time the cell becomes "available" (you cannot step into a cell before its availability time), returns the minimum time needed to travel from the top-left cell `(0,0)` to the bottom-right cell `(m-1, n-1)`. You start at time `0` at `(0,0)` (which is always available regardless of its value). From a cell at current time `t`, you may move in four cardinal directions (up, down, left, right) to an adjacent cell, and arriving at that neighboring cell takes exactly `1` unit of time after you are allowed to enter it. The arrival time at a neighboring cell is `max(t, moveTime[nrow][ncol]) + 1`. You may revisit cells, and if the destination is unreachable, return `-1`. Assume `1 <= m, n <= 100` and `0 <= moveTime[i][j] <= 10^5`.
*/
#include <vector>
#include <queue>
#include <climits>

// Returns the minimum time to reach bottom-right cell from top-left
// using Dijkstra's algorithm on the time-dependent grid.
int minTimeToReach(const std::vector<std::vector<int>>& moveTime) {
    const int m = static_cast<int>(moveTime.size());
    const int n = static_cast<int>(moveTime[0].size());

    // Min-heap: (time, (row, col))
    using State = std::pair<int, std::pair<int, int>>;
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;
    pq.push({0, {0, 0}});

    std::vector<std::vector<int>> dist(m, std::vector<int>(n, INT_MAX));
    dist[0][0] = 0;

    const int dir[5] = {-1, 0, 1, 0, -1};

    while (!pq.empty()) {
        auto [curr_time, pos] = pq.top();
        pq.pop();
        int row = pos.first;
        int col = pos.second;

        // If we already found a better time, skip (lazy deletion)
        if (curr_time > dist[row][col]) continue;

        // Destination reached
        if (row == m - 1 && col == n - 1) {
            return curr_time;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = row + dir[i];
            int nc = col + dir[i + 1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n) {
                int next_time = std::max(curr_time, moveTime[nr][nc]) + 1;
                if (next_time < dist[nr][nc]) {
                    dist[nr][nc] = next_time;
                    pq.push({next_time, {nr, nc}});
                }
            }
        }
    }

    return -1;
}
#include <cassert>
#include <vector>
#include <iostream>

// Assume minTimeToReach is defined above (included via paste or include).

int main() {
    // Single cell
    std::vector<std::vector<int>> g1 = {{5}};
    assert(minTimeToReach(g1) == 0);

    // Simple 2x2 with no restriction
    std::vector<std::vector<int>> g2 = {{0, 0}, {0, 0}};
    assert(minTimeToReach(g2) == 2);

    // Restricted middle cell
    std::vector<std::vector<int>> g3 = {{0, 10}, {0, 0}};
    // Path: (0,0)->(0,1) takes max(0,10)+1=11, then to (1,1) takes max(11,0)+1=12
    // Alternative via (1,0): start at 0 -> (1,0) = 1, then (1,1)=2 => better.
    assert(minTimeToReach(g3) == 2);

    // Larger restriction
    std::vector<std::vector<int>> g4 = {{0, 100}, {0, 0}};
    // Best path: down then right: (0,0)->(1,0) at time 1, then (1,1) at time 2.
    assert(minTimeToReach(g4) == 2);

    // Blocked-like high times
    std::vector<std::vector<int>> g5 = {{0, 1, 1}, {1, 100, 1}, {1, 1, 0}};
    // Expected? Let's compute: A B C / D E F / G H I. 
    // Optimal: 0->A(0) then B(1) then C(2) then F(3) then I(4) => total 4.
    // Or A->D->G->H->I => 1+2+3+4 ? Actually D time 1, G time 2, H time 3, I time 4.
    // So answer is 4.
    assert(minTimeToReach(g5) == 4);

    // High start cell value, but start always at 0
    std::vector<std::vector<int>> g6 = {{100, 0}, {0, 0}};
    assert(minTimeToReach(g6) == 2);

    // 1xN line
    std::vector<std::vector<int>> g7 = {{0, 5, 5}};
    // Path: start 0 -> (0,1) at max(0,5)+1=6 -> (0,2) at max(6,5)+1=7.
    assert(minTimeToReach(g7) == 7);

    // Nx1 column
    std::vector<std::vector<int>> g8 = {{0}, {3}, {0}};
    // start 0 -> (1,0) at 4 -> (2,0) at max(4,0)+1=5.
    assert(minTimeToReach(g8) == 5);

    // Unreachable? Actually grid is connected, but test fallback not needed.

    std::cout << "All tests passed!\n";
    return 0;
}
// This is a shortest-path problem on an unweighted grid where edge weights depend on the current time and the destination cell's availability. Since time is non-decreasing along any path, we can use Dijkstra's algorithm with a min-heap priority queue. The state is `(current_time, row, col)`. We maintain a distance matrix `dist` initialized to `INT_MAX`, with `dist[0][0] = 0`. For each popped state, if we reach the bottom-right cell, we return the current time as the answer. For each neighbor within bounds, we compute the earliest arrival time as `max(current_time, moveTime[nr][nc]) + 1`. If this time is less than the recorded `dist[nr][nc]`, we update it and push the new state. Because the priority queue always extracts the state with the smallest time, the first time we pop the destination we have the optimal result. Edge cases: grid of size 1x1 returns 0 immediately because we are already there; unreachable cells (though in a connected grid with non-negative times it's always reachable, but we still handle return -1 for safety). Time complexity is `O(m*n log(m*n))` due to each cell potentially being relaxed multiple times (but at most O(m*n) pushes), and space complexity is `O(m*n)` for the distance matrix and heap.
