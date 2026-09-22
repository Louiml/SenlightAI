You are given an array `a` of `n` non-negative integers (the cost to solve each problem) and an array `b` of `n` integers in the range `[0, n-1]` (the problem you can jump to after solving problem `i`). You start at problem `0` and can always move from problem `i` to problem `i-1` (if `i > 0`) at zero cost. You can also teleport from problem `i` to problem `b[i]` at a cost equal to `a[i]`. You may use any sequence of zero-cost backward moves and paid teleports, possibly revisiting problems. For each problem `i`, let `d[i]` be the minimum total cost to reach problem `i` from problem `0`. Define `value[i] = (sum of a[0..i]) - d[i]`. Write a C++ function that, given `n`, `a`, and `b`, returns the maximum `value[i]` over all `i` from `0` to `n-1`. The function must handle up to `n = 200,000` efficiently.

This is a shortest-path problem on a directed graph with `n` nodes. Node `0` is the source. From node `i` there are two outgoing edges: one to `i-1` with weight `0` (if `i > 0`), and one to `b[i]` with weight `a[i]`. Since edges are non-negative (a[i] ≥ 0), we can use Dijkstra's algorithm with a priority queue (min-heap) to compute the minimum distance `d[i]` from node `0` to every other node. After computing all `d[i]`, compute prefix sums `pre[i] = pre[i-1] + a[i]` (with `pre[0] = a[0]`). Then the answer is `max(pre[i] - d[i])` for all `i`. Edge cases: when `n = 1`, the answer is `a[0] - 0 = a[0]`; when `b[i]` already points to `i` or `i-1`, the teleport edge may be redundant but harmless. The zero‑cost backward edges can cause cycles, but Dijkstra handles non‑negative weights correctly. Complexity: `O((n + m) log n)` with `m = 2n` edges, i.e., `O(n log n)` time and `O(n)` space.

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

// Computes the maximum value (prefix_sum[i] - min_distance_to_i) over all i.
long long maxProblemValue(const std::vector<long long>& a, const std::vector<int>& b) {
    int n = (int)a.size();
    if (n == 0) return 0;

    // Prefix sums of a
    std::vector<long long> pre(n, 0);
    pre[0] = a[0];
    for (int i = 1; i < n; ++i) {
        pre[i] = pre[i-1] + a[i];
    }

    // Dijkstra from node 0
    const long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    dist[0] = 0;
    using P = std::pair<long long, int>; // {distance, node}
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue; // stale entry

        // Edge to u-1 with weight 0
        if (u > 0 && dist[u-1] > d) {
            dist[u-1] = d;
            pq.push({d, u-1});
        }
        // Teleport edge to b[u] with weight a[u]
        int v = b[u];
        if (dist[v] > d + a[u]) {
            dist[v] = d + a[u];
            pq.push({dist[v], v});
        }
    }

    long long answer = 0;
    for (int i = 0; i < n; ++i) {
        answer = std::max(answer, pre[i] - dist[i]);
    }
    return answer;
}

#include <cassert>
#include <vector>

// The solution function as defined above
long long maxProblemValue(const std::vector<long long>& a, const std::vector<int>& b);

int main() {
    // Example 1: Simple chain, teleport to next or back
    {
        std::vector<long long> a = {5, 3, 2};
        std::vector<int> b = {0, 1, 2}; // no teleports actually
        // Distances: d[0]=0, d[1]=0 (back from 1 to 0 then back? actually from 0 cannot reach 1 directly, but from 1 we can go to 0 with cost 0, so d[1] is infinite? Let's compute: start at 0, only edge to b[0]=0, so stuck. So d[1]=INF, but that can't be allowed for negative? The original problem expects all reachable? Actually from 1 we can go to 0, but we need to reach 1 first. Since no edge into 1, d[1]=INF. So prefix - INF = -INF, but answer will just ignore it. The code returns max(5-0, 8-INF, 10-INF) = 5. Test accordingly.
        long long result = maxProblemValue(a, b);
        assert(result == 5);
    }

    // Example 2: Teleport from 0 to 2
    {
        std::vector<long long> a = {1, 100, 1};
        std::vector<int> b = {2, 0, 2}; // from 0 to 2 cost 1, from 1 to 0 cost 100, from 2 to 2 cost 1
        // Dist: d[0]=0, d[1]=INF? Actually from 0 teleport to 2 cost 1, d[2]=1. From 2 go back to 1 cost 0, d[1]=1. So d[1]=1, d[2]=1.
        // pre = [1, 101, 102]
        // values: 1-0=1, 101-1=100, 102-1=101 -> max 101
        long long result = maxProblemValue(a, b);
        assert(result == 101);
    }

    // Example 3: n=1
    {
        std::vector<long long> a = {7};
        std::vector<int> b = {0};
        assert(maxProblemValue(a, b) == 7);
    }

    // Example 4: Backward chain (like the original code snippet)
    {
        std::vector<long long> a = {10, 20, 30};
        std::vector<int> b = {0, 0, 0}; // all teleport to 0
        // Dist: d[0]=0, d[1] can be reached by teleport from 1? no, need to reach 1 first. But we can reach 2 by teleport from 0 cost 10, then back to 1 cost 0, then from 1 teleport to 0 cost 20. So d[2]=10, d[1]=10 (from 2 back to 1). Wait, from 0 teleport to b[0]=0, no. From 2? We need to start at 0. So how to reach 2? From 0 we can only go to b[0]=0, stuck. So d[2]=INF, d[1]=INF. So answer = max(10-0, 30-INF, 60-INF) = 10. But that seems unsatisfying. Actually the original snippet uses backward edges from i to i-1, so from 0 you cannot move forward except teleport. So indeed no way to reach 1 or 2 unless teleport from 0 directly to them. Here b[0]=0, so only node 0 reachable. Answer = 10.
        long long result = maxProblemValue(a, b);
        assert(result == 10);
    }

    // Example 5: Teleports enable forward progress
    {
        std::vector<long long> a = {2, 5, 3, 1};
        std::vector<int> b = {1, 2, 3, 3}; // chain
        // Dist: d[0]=0, d[1]=2, d[2]=2+5=7, d[3]=7+3=10
        // pre: [2,7,10,11]
        // values: 2, 7-2=5, 10-7=3, 11-10=1 -> max 5
        long long result = maxProblemValue(a, b);
        assert(result == 5);
    }

    return 0;
}
