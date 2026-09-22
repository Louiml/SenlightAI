// Given an undirected graph with \( N \) vertices (numbered \( 0 \) to \( N-1 \)) and \( M \) edges, each edge has an integer cost and a binary label string of length \( K \) (each label is a \( K \)-bit vector). Write a C++ function that, for each vertex \( v \) from \( 0 \) to \( N-2 \), computes the minimum total cost of a path from vertex \( v \) to vertex \( N-1 \) such that the XOR (bitwise exclusive OR) of all edge labels along the path equals the specific nonzero value represented by the label of the destination vertex (which is itself a \( K \)-bit vector). If no such path exists, the result for that vertex should be \( -1 \). The graph may contain parallel edges and self-loops. Costs are positive integers.

#include <cassert>
#include <vector>
#include <cstdint>

int main() {
    // Test 1: Simple graph, 3 vertices, K=1
    // Edges: 0-1 cost 1 label 1, 1-2 cost 2 label 1, 0-2 cost 5 label 0
    int N = 3;
    std::vector<int> from = {0,1,0};
    std::vector<int> to   = {1,2,2};
    std::vector<int> cost = {1,2,5};
    std::vector<uint32_t> label = {1,1,0};
    // Target=2, B=1 (nonzero)
    auto ans = minCostWithXor(N, from, to, cost, label, 2, 1);
    // From 0: path 0-1-2 xor=1^1=0, not B; path 0-2 xor=0 not B; so -1? Actually no path with xor=1? Wait 0-1-2 xor=1^1=0, 0-2 xor=0. So -1.
    assert(ans[0] == -1);
    // From 1: path 1-2 xor=1 => cost 2
    assert(ans[1] == 2);
    // From 2: target itself, must have path with xor=B? A path of length 0 has xor 0, not 1, and no loops. So -1.
    assert(ans[2] == -1);

    // Test 2: K=2, triangle with labels making xor required
    N = 3;
    from = {0,1,0};
    to   = {1,2,2};
    cost = {1,1,1};
    label = {1,2,3}; // 01, 10, 11
    // Target=2, B=3 (11). From 0: path 0-2 xor=3 cost 1 => 1. From 1: 1-2 xor=2 not 3, 1-0-2 xor=1^3=2 not 3, so -1. From 2: self-loop? no, so -1.
    ans = minCostWithXor(N, from, to, cost, label, 2, 3);
    assert(ans[0] == 1);
    assert(ans[1] == -1);
    assert(ans[2] == -1);

    // Test 3: Parallel edges with different labels
    N = 2;
    from = {0,0};
    to   = {1,1};
    cost = {3,1};
    label = {1,2};
    // Target=1, B=1. From 0: edge0 cost3 label1 => 3; edge1 label2 not 1. So ans[0]=3. From 1: no path? -1.
    ans = minCostWithXor(N, from, to, cost, label, 1, 1);
    assert(ans[0] == 3);
    assert(ans[1] == -1);

    // Test 4: Self-loops don't help achieve nonzero xor for target
    N = 1;
    from = {0};
    to   = {0};
    cost = {5};
    label = {1};
    // Target=0, B=1. A self-loop from 0 to 0 with label1 gives xor=1 after one loop? But that's a cycle, not a simple path? Usually a path cannot repeat vertices? In graph theory, a path may not repeat vertices, but in many MST/shortest path problems, cycles are allowed for paths? Typically shortest path allows revisiting? The original problem might allow cycles? The description says "path", which usually means a simple path (no repeated vertices) but in shortest path algorithms, cycles are not beneficial because costs positive. However a self-loop would create a cycle and increase cost, but if target is also source, you could start at target, take a self-loop and end at target with xor=1, cost 5. But is that allowed? A path from v to t typically cannot have repeated vertices, so a self-loop at t would repeat t, invalid. So for N=1, from 0 to 0 with a self-loop, a valid path would be length 0 only (xor=0). So no path with B=1. So ans[0] should be -1.
    ans = minCostWithXor(N, from, to, cost, label, 0, 1);
    assert(ans[0] == -1);

    // Test 5: Larger K, ensure it handles K=3
    N = 2;
    from = {0};
    to   = {1};
    cost = {7};
    label = {5}; // 101
    // Target=1, B=5 => path cost 7
    ans = minCostWithXor(N, from, to, cost, label, 1, 5);
    assert(ans[0] == 7);
    // B=0 (but specification says B nonzero, so we should not call with 0, but test anyway? The function might handle it incorrectly. We'll not test.

    return 0;
}

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <cstdint>

// Compute, for each vertex, the minimum cost of a path from that vertex to target
// such that the XOR of edge labels along the path equals a given nonzero value B.
// Graph is undirected, edges have positive costs.
// Returns a vector of length N, where result[v] = -1 if no such path exists.
std::vector<long long> minCostWithXor(
    int N,
    const std::vector<int>& from,
    const std::vector<int>& to,
    const std::vector<int>& cost,
    const std::vector<uint32_t>& label, // label[i] is the K-bit mask for edge i
    int target,
    uint32_t B // desired XOR, must be nonzero
) {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    int M = (int)from.size();
    int K = 0;
    // Determine K from labels (max bits used)
    for (uint32_t x : label) {
        if (x != 0) {
            int hb = 31 - __builtin_clz(x);
            K = std::max(K, hb + 1);
        }
    }
    // At least 1 bit if B nonzero
    if (B != 0) {
        int hb = 31 - __builtin_clz(B);
        K = std::max(K, hb + 1);
    }
    if (K == 0) K = 1; // ensure at least 1 bit
    int S = 1 << K; // total XOR states

    // Build adjacency list for reversed graph
    std::vector<std::vector<std::pair<int, std::pair<int, uint32_t>>>> rev(N);
    for (int i = 0; i < M; ++i) {
        int u = from[i], v = to[i];
        uint32_t x = label[i];
        // Reverse edge: v -> u with same cost and label
        rev[v].push_back({u, {cost[i], x}});
        rev[u].push_back({v, {cost[i], x}});
    }

    // Dijkstra on states (vertex, xor)
    // dist[u][x] = min distance from target to vertex u with xor x
    std::vector<std::vector<long long>> dist(N, std::vector<long long>(S, INF));
    using State = std::pair<long long, std::pair<int, int>>; // (dist, (vertex, xor))
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[target][0] = 0;
    pq.push({0, {target, 0}});

    while (!pq.empty()) {
        auto [d, sv] = pq.top(); pq.pop();
        int u = sv.first;
        int x = sv.second;
        if (d != dist[u][x]) continue;

        for (auto [v, ec] : rev[u]) {
            int c = ec.first;
            uint32_t lab = ec.second;
            int nx = x ^ lab; // xor on reversed path
            if (nx >= S) continue; // should not happen
            if (dist[v][nx] > d + c) {
                dist[v][nx] = d + c;
                pq.push({dist[v][nx], {v, nx}});
            }
        }
    }

    // For each vertex, find min distance among states with xor == B
    std::vector<long long> ans(N, -1);
    for (int v = 0; v < N; ++v) {
        long long best = INF;
        if ((uint32_t)B < S) {
            best = dist[v][B];
        }
        ans[v] = (best < INF) ? best : -1;
    }
    return ans;
}

// The problem is a shortest path problem with a group constraint: each edge has a cost and a label from the group \( \mathbb{Z}_2^K \) (XOR group). We need the minimum cost path from every source to a fixed target \( t = N-1 \), such that the total label XOR equals the label of the target vertex. Since the target is fixed, we can reverse the graph (or run Dijkstra on the reversed graph) and compute shortest paths from \( t \) to all vertices, tracking the current XOR value along the path. Because the group is Abelian and the labels are binary vectors, the state space is \( N \times 2^K \). However, \( 2^K \) could be large, but the original snippet uses a specialized algorithm for "nonzero group product shortest path", which likely leverages the fact that we only need paths with nonzero total label. In general, we can model this as a Dijkstra over states \( (u, x) \) where \( u \) is the vertex and \( x \) is the total XOR label so far. We want to find the minimum cost to reach any state \( (t, x) \) where \( x \) equals the label of \( t \) (which is given). Since we are computing from all sources to \( t \), we run Dijkstra on the reversed graph starting from target \( t \) with initial label 0, and for each state we record the minimum distance. Then for each source vertex \( v \), the answer is the minimum over all states \( (v, x) \) such that \( x \) equals the target label (which is known). But careful: the original code computes `nonzero_group_product_shortest_path` and returns distances that correspond to paths whose total label equals the label of the target? Actually, reading the original snippet: it calls `nonzero_group_product_shortest_path<ll, Mono>(G, label, N - 1)`, and then for each vertex prints the distance. The function likely returns the shortest distance from each vertex to the target where the product of labels along the path equals the identity? But the problem says "nonzero" so it probably excludes the identity. However, the exact semantics are unclear. For a standalone task, we should define precisely: Given a target vertex \( t \), for each vertex \( v \), find the minimum cost path from \( v \) to \( t \) such that the XOR of labels on the path equals a specific nonzero value \( B \) (which could be given as a parameter). Since the snippet uses the label of the destination? Actually the destination is \( N-1 \), and the label array is for edges, not vertices. The code passes `label` (edge labels) and `N-1` as target. The function likely computes shortest paths from each vertex to N-1 such that the path label XOR equals the identity? But they check `if (ans == infty<ll>) ans = -1;` so no special nonzero condition? The function name suggests "nonzero group product", meaning it may only return distances for nonzero products? To keep it simple and self-contained, I'll define: given a target vertex `t`, for each vertex `v`, find the minimum cost path from `v` to `t` such that the XOR of edge labels along the path equals a given nonzero value `B` (where `B` is provided). If no such path exists, return -1. We can implement a Dijkstra on states `(vertex, xor)` on the reversed graph (so we start from target `t` with xor=0 and follow edges backwards). For each state we record the minimal distance. Then for each source vertex `v`, we take the minimum distance among all states `(v, x)` where `x == B`. That gives the answer. Note that the path must have at least one edge, so the trivial path (length 0) that has xor 0 is not applicable because `B` is nonzero, so it's fine. If `B` were 0, we would exclude the zero-length path. But we specify `B` nonzero. The algorithm runs Dijkstra on a graph with `N * 2^K` states. Complexity is `O((M * 2^K) log(N * 2^K))` time and `O(N * 2^K)` space. Since `K` is small (given as input), this is feasible. Edge cases: parallel edges, self-loops, unreachable states. The graph is undirected, so we can just add edges in both directions when building reversed graph. All costs are positive, so Dijkstra is valid.
