/*
You are given an undirected weighted graph where each edge (u, v) has two integer values: a base travel time `p` and a departure time `q` (with `q >= p`). A journey from a start node `s` to a target node `t` must satisfy the following constraint: if you are currently at node `x` and want to traverse an edge to node `y` with parameters `(p, q)`, you may only do so if your current total travel time is at most `q - p` if you have already used at least one "special" edge (i.e., you are in "special mode"), otherwise you may traverse it freely, but after traversing it you may optionally enter "special mode" for the rest of the journey. However, once you enter special mode, you cannot leave it. Your goal is to find the minimum total travel time from `s` to `t`, where the total is the sum of all `p` values of edges traversed, plus an additional penalty of `(q - p)` for exactly one edge (the edge that puts you into special mode, if any). If you never enter special mode, the total is simply the sum of `p` values, but the answer for every query is the minimum over all paths that do enter special mode (since the problem guarantees such a path exists). Write a C++ function `long long shortestSpecial(int n, const vector<tuple<int,int,int,int>>& edges, int s, int t)` that returns this minimum total. The graph has nodes numbered 1..n, edges are given as tuples `(u, v, p, q)`. Each query is independent; your function may be called multiple times with different `(s,t)` on the same pre-built adjacency list.
*/

#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

// State for Dijkstra: node, distance, threshold (for special mode), and mode flag
struct State {
    int node;
    long long dist;
    int threshold;
    int mode; // 0 = not special, 1 = special
    bool operator<(const State& other) const {
        if (dist != other.dist) return dist > other.dist;
        return threshold < other.threshold; // larger threshold first? actually priority_queue wants max at top; we'll use reversed
    }
};

// Computes minimum travel time from s to t that must enter special mode.
long long shortestSpecial(int n, const vector<tuple<int,int,int,int>>& edges, int s, int t) {
    // Build adjacency list: node, neighbor, p, (q-p)
    vector<vector<pair<int, pair<int,int>>>> adj(n + 1);
    for (const auto& e : edges) {
        int u, v, p, q;
        tie(u, v, p, q) = e;
        int extra = q - p;
        adj[u].push_back({v, {p, extra}});
        adj[v].push_back({u, {p, extra}});
    }

    // dist[node][mode] = minimum distance to reach this state
    vector<vector<long long>> dist(n + 1, vector<long long>(2, INF));
    // To handle varying thresholds, we also maintain a map of (node, threshold) -> best dist for state 1
    vector<unordered_map<int, long long>> bestSpecial(n + 1);
    dist[s][0] = 0;

    // priority_queue: use custom comparators: min-heap by distance, and for equal distance, larger threshold first (since stricter is better)
    priority_queue<State, vector<State>, function<bool(const State&, const State&)>> pq(
        [](const State& a, const State& b) {
            if (a.dist != b.dist) return a.dist > b.dist;
            return a.threshold < b.threshold; // larger threshold first
        }
    );
    pq.push({s, 0, 0, 0});

    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();

        // Check if this state is outdated
        if (cur.mode == 0) {
            if (cur.dist != dist[cur.node][0]) continue;
        } else {
            auto it = bestSpecial[cur.node].find(cur.threshold);
            if (it == bestSpecial[cur.node].end() || it->second != cur.dist) continue;
        }

        for (const auto& edge : adj[cur.node]) {
            int nxt = edge.first;
            int p = edge.second.first;
            int d = edge.second.second; // = q - p

            if (cur.mode == 0) {
                // Option 1: stay in mode 0, add only p
                long long newDist = cur.dist + p;
                if (newDist < dist[nxt][0]) {
                    dist[nxt][0] = newDist;
                    pq.push({nxt, newDist, 0, 0});
                }
                // Option 2: enter special mode on this edge, add p + d = q
                long long specialDist = cur.dist + p + d;
                // threshold becomes d
                auto it = bestSpecial[nxt].find(d);
                if (it == bestSpecial[nxt].end() || specialDist < it->second) {
                    bestSpecial[nxt][d] = specialDist;
                    if (specialDist < dist[nxt][1]) dist[nxt][1] = specialDist; // keep global minimum for state 1
                    pq.push({nxt, specialDist, d, 1});
                }
            } else {
                // In special mode, can only traverse if threshold >= d
                if (cur.threshold >= d) {
                    long long newDist = cur.dist + p;
                    int newThreshold = max(cur.threshold, d);
                    auto it = bestSpecial[nxt].find(newThreshold);
                    if (it == bestSpecial[nxt].end() || newDist < it->second) {
                        bestSpecial[nxt][newThreshold] = newDist;
                        if (newDist < dist[nxt][1]) dist[nxt][1] = newDist;
                        pq.push({nxt, newDist, newThreshold, 1});
                    }
                }
            }
        }
    }

    return dist[t][1] == INF ? -1 : dist[t][1];
}

#include <bits/stdc++.h>
using namespace std;

// Assume the solution function is defined above

int main() {
    // Example 1: simple path 1-2-3, each edge (p=2,q=3) so d=1
    // Optimal: traverse edge 1-2 in normal mode (cost 2), then edge 2-3 in special mode (cost 2+1=3) total=5
    // Alternative: enter special on first edge: cost 3, then second edge: cost 2+? threshold=1 so okay, total=5
    {
        int n = 3;
        vector<tuple<int,int,int,int>> edges = {{1,2,2,3},{2,3,2,3}};
        assert(shortestSpecial(n, edges, 1, 3) == 5);
    }

    // Example 2: direct edge with high q-p
    // Only option: enter special on that edge, cost q = 10
    {
        int n = 2;
        vector<tuple<int,int,int,int>> edges = {{1,2,4,10}};
        assert(shortestSpecial(n, edges, 1, 2) == 10);
    }

    // Example 3: two possible special edges, choose cheaper
    // Path 1: 1-2 (p=1,q=2) then 2-3 (p=1,q=100) => d=1, so can traverse second with cost 1+1=2 total=3? Wait cost: first special=2, second normal in special mode cost p=1, total=3
    // Path 2: 1-2 normal (cost=1), then 2-3 special (cost=1+99=100) total=101, so better is 3
    // But also path 1-2 normal, 2-3 normal would total=2 but doesn't enter special, invalid. So answer = 3
    {
        int n = 3;
        vector<tuple<int,int,int,int>> edges = {{1,2,1,2},{2,3,1,100}};
        assert(shortestSpecial(n, edges, 1, 3) == 3);
    }

    // Example 4: need to use special early to satisfy later constraint
    // Edge 1-2: p=1,q=2 (d=1); edge 2-3: p=5,q=100 (d=95)
    // If we enter special on first edge threshold=1, cannot traverse second because 1<95.
    // So we must enter special on second edge, but we cannot because we'd be in normal mode until then? Actually we can be normal on first edge (cost 1), then on second edge enter special (cost 5+95=100) total=101
    // Or we could enter special on first edge (cost 2) then fail on second. So answer 101.
    {
        int n = 3;
        vector<tuple<int,int,int,int>> edges = {{1,2,1,2},{2,3,5,100}};
        assert(shortestSpecial(n, edges, 1, 3) == 101);
    }

    // Example 5: multiple parallel edges with different d
    // From 1 to 2 there are two edges: (p=2,q=3,d=1) and (p=3,q=10,d=7)
    // Want to go from 1 to 3 via 2, with edge 2-3 (p=1,q=10,d=9)
    // If we take first edge normal cost 2 then need to enter special on 2-3 but d=9, okay, cost 1+9=10, total 12
    // If we take second edge special cost 10, then 2-3 in special mode cost 1, threshold=7 <9 fails. So take first edge normal, then 2-3 special: total 2+10=12
    // Alternatively take first edge special cost 3, threshold=1, then 2-3 fails. So answer 12.
    {
        int n = 3;
        vector<tuple<int,int,int,int>> edges = {{1,2,2,3},{1,2,3,10},{2,3,1,10}};
        assert(shortestSpecial(n, edges, 1, 3) == 12);
    }

    // Example 6: start == end, but still must enter special mode. There must be a cycle? Or maybe zero-length path? The definition requires entering special, so if s==t and no special edge used, not allowed. But if there is a self-loop, can enter special on it. Test a case with self-loop.
    {
        int n = 2;
        vector<tuple<int,int,int,int>> edges = {{1,1,3,5},{1,2,1,2}};
        // Option: take self-loop special? cost 5, then no need to go anywhere, but we are already at t. So answer could be 5.
        // Or go to 2 normal (cost1) and back to 1 special (cost2) total=3? Actually go to 2 normal cost 1, then from 2 to 1 special cost 2, total 3, threshold=1, no further constraints. So answer 3.
        assert(shortestSpecial(n, edges, 1, 1) == 3);
    }

    // Example 7: disconnected graph, no valid path -> -1
    {
        int n = 4;
        vector<tuple<int,int,int,int>> edges = {{1,2,1,1}};
        assert(shortestSpecial(n, edges, 1, 4) == -1);
    }

    return 0;
}

// This is a shortest-path problem with a state transition. We can model each node in two states: `state 0` = not yet used the special edge (still in "free" mode), and `state 1` = already used the special edge (in "strict" mode). In state 0, when traversing an edge `(p, q)`, we have two options: (a) stay in state 0, adding only `p` to the current distance, and not entering special mode; (b) enter special mode on this edge, adding `p + (q - p) = q` to the distance, and move to state 1 with the restriction that future edges must satisfy `d >= (q-p)` where `d` is the "remaining allowed time" for the strict mode (initially set to `q-p` of the edge that triggered the mode). In state 1, we can only traverse an edge if `d >= (q-p)`; if so, we add `p` to the distance and update `d = min(d, q-p)`? Actually the code uses `max(cur.d, nxt.d)` for the strict threshold, meaning the new `d` is the maximum of the previous `d` and the current edge's `(q-p)`. This is because to traverse in strict mode, we need `d >= (q-p)`, and after traversing, the new threshold becomes the maximum of the old threshold and the new edge's required time, to ensure future edges with larger requirements are still allowed. So the state is `(node, state)` with distance, and for state 1 we also track a threshold `d`. Implement Dijkstra with a priority queue that orders by distance, breaking ties by threshold. The initial state at `s` is `(s, state 0, dist=0, threshold=0)`. The answer for `(s,t)` is `dist[t][1]` (the minimum distance reaching `t` in state 1). If no such path exists, return -1 (but per problem constraints it always exists). Edge cases: self-loops are possible but not problematic; multiple edges between same nodes are fine. The graph is undirected, so add both directions. Complexity: O((n + m) log n) per query, because each state can be visited multiple times with different thresholds, but in practice the number of distinct thresholds per node is limited by the number of distinct `(q-p)` values. In the worst case, each node may have up to `degree` distinct thresholds, but with a priority queue we handle them fine; memory is O(n + m). For `tc` queries, total O(tc * (n + m) log n).
