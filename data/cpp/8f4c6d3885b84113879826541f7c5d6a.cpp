/*
You are given a circular array of non-negative integers `arr` of length `n` (1 ≤ n ≤ 2×10^5), and an initial array `initial` of non-negative integers of the same length. Write a C++ function `vector<long long> propagateCosts(const vector<int>& arr, const vector<int>& initial)` that returns the final array after repeatedly applying the following rule: for each position `i` (0-indexed), if the current value at `i` plus `arr[i]` is strictly less than the current value at the next position `(i+1)%n`, then update that next position to that smaller sum. This process is repeated until no more updates are possible (i.e., a stable state is reached). The input values fit in 32-bit signed integers, but the final values may exceed 32-bit, so use `long long` for the result.
*/
#include <vector>
#include <queue>
#include <tuple>

// Given circular array arr and initial costs, compute the minimal reachable cost at each node
// using relaxation along the cycle edge from i to (i+1)%n.
std::vector<long long> propagateCosts(const std::vector<int>& arr, const std::vector<int>& initial) {
    int n = static_cast<int>(arr.size());
    // Use long long to avoid overflow during additions.
    std::vector<long long> best(n);
    // Min-heap: store (cost, index) with cost negated to simulate min-heap using std::priority_queue.
    std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<>> pq;

    for (int i = 0; i < n; ++i) {
        best[i] = static_cast<long long>(initial[i]);
        pq.push({best[i], i});
    }

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // If this entry is outdated, skip it.
        if (d > best[u]) continue;

        int v = (u + 1) % n;
        long long candidate = d + static_cast<long long>(arr[u]);
        if (candidate < best[v]) {
            best[v] = candidate;
            pq.push({candidate, v});
        }
    }

    return best;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cycle of 3 nodes with trivial initial values.
    std::vector<int> arr1 = {1, 2, 3};
    std::vector<int> init1 = {10, 10, 10};
    std::vector<long long> res1 = propagateCosts(arr1, init1);
    assert((res1 == std::vector<long long>{10, 10, 10})); // No improvement because each step increases.

    // Cycle where improvements propagate around.
    std::vector<int> arr2 = {5, 1, 1, 1};
    std::vector<int> init2 = {0, 100, 100, 100};
    std::vector<long long> res2 = propagateCosts(arr2, init2);
    // Starting at node 0: cost 0 → node1 gets 0+5=5, node2 gets 5+1=6, node3 gets 6+1=7, node0 gets 7+1=8 but 8>0 so no change.
    // Starting at node1: 100→ node2 101, etc., no better. So final: {0,5,6,7}
    assert((res2 == std::vector<long long>{0, 5, 6, 7}));

    // Single node self-loop.
    std::vector<int> arr3 = {3};
    std::vector<int> init3 = {10};
    std::vector<long long> res3 = propagateCosts(arr3, init3);
    assert((res3 == std::vector<long long>{10})); // Candidate 10+3=13 not < 10.

    // Multiple sources with different initial costs.
    std::vector<int> arr4 = {2, 2, 2};
    std::vector<int> init4 = {5, 1, 5};
    std::vector<long long> res4 = propagateCosts(arr4, init4);
    // Node1 has 1 → node2 candidate 3 (<5) → node2=3, node2→node0 candidate 5 (<5? no strict, so 5 not improved) → final {5,1,3}
    assert((res4 == std::vector<long long>{5, 1, 3}));

    // Large numbers to test long long.
    std::vector<int> arr5 = {2000000000, 2000000000};
    std::vector<int> init5 = {0, 0};
    std::vector<long long> res5 = propagateCosts(arr5, init5);
    // Both start at 0, candidate 0+2e9 = 2e9 not < 0, so stays {0,0}
    assert((res5 == std::vector<long long>{0, 0}));

    // Chain that improves multiple times.
    std::vector<int> arr6 = {3, 4, 0, 1};
    std::vector<int> init6 = {100, 100, 100, 2};
    std::vector<long long> res6 = propagateCosts(arr6, init6);
    // Start node3 (cost2) → node0 gets 2+1=3, node0→node1 gets 3+3=6, node1→node2 gets 6+4=10, node2→node3 gets 10+0=10 >2 so no.
    // Final likely {3,6,10,2}
    assert((res6 == std::vector<long long>{3, 6, 10, 2}));

    return 0;
}
// The problem is essentially a relaxation process on a directed cycle graph: each node `i` has an outgoing edge to `(i+1)%n` with weight `arr[i]`. The initial values are upper bounds, and we want to compute the shortest possible distance to each node from any starting point, where starting at node `i` gives an initial distance of `initial[i]` (but we may also arrive from a neighbor with a smaller path). This is a single-source shortest path variant with multiple sources, solvable using a priority queue (Dijkstra-like) where we push all initial nodes with their initial costs. When we pop the smallest current known distance `d` for node `u`, if `d` is stale (greater than the current recorded `ans[u]`), we skip. Otherwise, we attempt to relax the neighbor `v = (u+1)%n` with new candidate `d + arr[u]`. If this candidate is strictly less than the current `ans[v]`, we update `ans[v]` and push it. Since all edge weights are non-negative, this greedy relaxation works and terminates because values only decrease and are bounded below by 0. Edge cases: when `n=1`, the self-loop (since `(0+1)%1=0`) is allowed; the candidate must be strictly less than current to update. The algorithm runs in `O(n log n)` time and `O(n)` space. The final results are the minimal achievable values.
