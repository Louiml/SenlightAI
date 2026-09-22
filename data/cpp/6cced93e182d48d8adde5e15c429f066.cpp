Implement a C++ function `int maximumWeightClosedSet(int n, const std::vector<std::vector<int>>& adj, const std::vector<int>& weights)` that solves the maximum weight closure problem on a bipartite-like directed graph. The input describes exactly `n` left vertices and `n` right vertices (both numbered 1..n) where `adj[i]` (0-indexed) gives the list of right-vertices reachable from left-vertex `i+1`. The `weights` array (size `n`, 0-indexed) gives the weight of each left vertex (can be positive, negative, or zero). The task is to choose a subset of left vertices and a subset of right vertices such that: (1) if a left vertex is chosen, then all right vertices it points to must also be chosen; (2) if a right vertex is chosen, then it must be pointed to by at least one chosen left vertex (i.e., no “dangling” right vertices). The objective is to maximize the sum of weights of chosen left vertices (right vertices have weight 0). The function should return the maximum possible total weight. Note: there is always at least one valid solution (choose the empty set, weight 0). The constraints guarantee that each left vertex has at least one outgoing edge (i.e., each `adj[i]` is non-empty), and the graph is such that the maximum weight can be found via a min-cut/max-flow approach. The solution must use Dinic’s max-flow algorithm. Do not modify the input; output only the integer answer.

#include <cassert>
#include <vector>
#include <iostream>

// Assume the function is defined above or included.

int main() {
    // Test 1: Single left, single right, positive weight -> choose both, answer=5
    {
        int n = 1;
        std::vector<std::vector<int>> adj = {{1}};
        std::vector<int> w = {5};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 5);
    }

    // Test 2: Two lefts, two rights, each left points to both rights, weights positive -> choose both lefts, both rights, total = 7
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1,2}, {1,2}};
        std::vector<int> w = {4, 3};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 7);
    }

    // Test 3: Same as above but one weight negative -> need to check if forced. 
    // left1 points to both rights, weight 10; left2 points to both rights, weight -100.
    // Can choose left1 only and right1 only? Need |L|=|R|=1, and there is a perfect matching: left1->right1, left2->right2? 
    // If we choose L={1}, R must have size 1 and contain all neighbors of 1 => R must contain 1 and 2, but size 1 impossible.
    // So we cannot choose left1 alone. If choose both lefts, size 2, R must be {1,2} (all neighbors), valid. Total = 10-100 = -90.
    // Empty set is valid with weight 0, so answer = 0.
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1,2}, {1,2}};
        std::vector<int> w = {10, -100};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 0);
    }

    // Test 4: Chain: left1->{1,2}, left2->{2}, weights: left1=5, left2=-3.
    // Can we choose left1 only? Need |L|=1, R size 1, but left1 neighbors are {1,2} so R must contain 1 and 2 -> impossible. So must choose both lefts, R = {1,2}, total = 2. Empty set gives 0, so answer=0.
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1,2}, {2}};
        std::vector<int> w = {5, -3};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 0);
    }

    // Test 5: Two lefts, each points to one distinct right, positive weights: choose both, answer = sum
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1}, {2}};
        std::vector<int> w = {4, 6};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 10);
    }

    // Test 6: Mixed: left1->{1}, left2->{1,2}, left3->{2}; weights: left1=3, left2=5, left3=-2.
    // Need equal size. Possible L={1,2}, R={1,2} (valid matching: left1-1, left2-2), left3 not selected. Total=8.
    // L={1,3}? left3 neighbor {2} not in R if R size 2 and contains 1? Need R include 2 too. R={1,2} but size 2, L={1,3} size 2, matching left1-1, left3-2 valid, but left2 not selected and its neighbor? Not required. But closure requires that for every selected left, all its neighbors are in R. left1 neighbor {1} in R, left3 neighbor {2} in R. So L={1,3} valid total=3-2=1. L={2,3}? left2 neighbors {1,2}, left3 {2} -> R must be {1,2} total=5-2=3. So max is 8.
    {
        int n = 3;
        std::vector<std::vector<int>> adj = {{1}, {1,2}, {2}};
        std::vector<int> w = {3, 5, -2};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 8);
    }

    // Test 7: All negative -> answer 0
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1}, {2}};
        std::vector<int> w = {-1, -2};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 0);
    }

    // Test 8: Larger: n=3, complete bipartite, weights 1,2,3 -> all positive, answer=6
    {
        int n = 3;
        std::vector<std::vector<int>> adj = {{1,2,3}, {1,2,3}, {1,2,3}};
        std::vector<int> w = {1,2,3};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 6);
    }

    // Test 9: n=2, left1->{1}, left2->{1,2}, weights: left1=10, left2=-1.
    // Can we choose left1 only? Need R size 1 but left1 neighbor {1} so R={1}, size 1, valid. Total=10.
    // Choose both? R={1,2}, total=9. So answer=10.
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1}, {1,2}};
        std::vector<int> w = {10, -1};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 10);
    }

    // Test 10: n=2, left1->{1}, left2->{2}, weights: left1=5, left2=-1. Can choose left1 alone, answer=5.
    {
        int n = 2;
        std::vector<std::vector<int>> adj = {{1}, {2}};
        std::vector<int> w = {5, -1};
        assert(maximumWeightBipartiteClosure(n, adj, w) == 5);
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
#include <limits>

// Maximum weight closure problem on a directed graph with n left vertices (weighted)
// and n right vertices (weight 0). Edges go from left to right.
// Returns the maximum total weight of a closure.
int maximumWeightClosedSet(int n, const std::vector<std::vector<int>>& adj, const std::vector<int>& weights) {
    // Node ids: source = 0, left vertices 1..n, right vertices n+1..2n, sink = 2n+1
    int V = 2 * n + 2;
    int s = 0, t = 2 * n + 1;
    const int INF = 1e9;

    // Dinic's max-flow implementation
    struct Edge {
        int to, cap, rev;
    };

    std::vector<std::vector<Edge>> graph(V);

    auto add_edge = [&](int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
    };

    // Left vertices are 1..n, right vertices are n+1..2n
    int total_positive = 0;
    // Add source/sink edges for left vertices based on weights
    for (int i = 0; i < n; ++i) {
        int w = weights[i];
        if (w > 0) {
            total_positive += w;
            add_edge(s, i + 1, w);
        } else if (w < 0) {
            add_edge(i + 1, t, -w);
        }
        // if w == 0, no edge needed
    }

    // Add infinite capacity edges from left to right vertices (all its out-neighbors)
    for (int i = 0; i < n; ++i) {
        for (int r : adj[i]) {
            // r is 1-indexed right vertex -> convert to node id n + r
            add_edge(i + 1, n + r, INF);
        }
    }

    // BFS to build level graph
    std::vector<int> level(V);
    auto bfs = [&]() -> bool {
        std::fill(level.begin(), level.end(), -1);
        std::queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (const auto& e : graph[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    };

    // DFS to send flow
    std::vector<int> iter(V);
    std::function<int(int, int)> dfs = [&](int v, int f) -> int {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)graph[v].size(); ++i) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, std::min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    };

    // Dinic main loop
    int flow = 0;
    while (bfs()) {
        std::fill(iter.begin(), iter.end(), 0);
        int f;
        while ((f = dfs(s, INF)) > 0) {
            flow += f;
        }
    }

    return total_positive - flow;
}

// This problem is a maximum weight closure problem on a directed graph where left vertices have weights and right vertices have weight 0, with edges from left to right. The closure constraint is: if you select a left vertex, you must also select all its out-neighbors. The additional condition “each right vertex selected must have at least one selected left predecessor” is automatically satisfied in the closure formulation because if you select a right vertex only because of a closure requirement, then at least one left predecessor is selected by definition. The maximum weight closure can be solved by constructing a flow network: create a source `s` and sink `t`. For every vertex in the original graph (here, only left vertices have weights; right vertices have weight 0), if weight is positive, add an edge `s -> v` with capacity equal to that weight; if weight is negative, add an edge `v -> t` with capacity equal to `-weight`. For every original directed edge `(u, v)`, add an infinite capacity edge `u -> v` (in the flow network). The maximum closure weight equals `sum of all positive weights - minCut(s, t)`. Here, the left vertices have weights `w[i]`; right vertices have weight 0, so we only process left vertices in the source/sink edges. We need to map the given adjacency (left->right lists) into flow edges. To enforce the problem’s additional condition about right vertices requiring a selected left predecessor, we must be careful: In a closure, if a right vertex is included, it must be because a left predecessor is included, otherwise it wouldn’t be in the closure. So the closure condition already ensures that. However, the original code snippet uses a bipartite matching trick to transform the problem into a min-cut on a graph constructed by matching left vertices to right vertices. That is because the original problem (CF 103E) had a different constraint: each left and right set have size n, and the condition is that the selected sets must have equal size, and every selected right must be covered by exactly one selected left, etc. But the given task statement simplifies it. Actually, re-reading: The task says "if a right vertex is chosen, then it must be pointed to by at least one chosen left vertex" – that is just the closure property, so it’s redundant. So the problem reduces to ordinary maximum weight closure, which can be solved with a standard max-flow construction. But wait, the code snippet given uses a Hungarian matching pre-processing step to construct the flow network with infinite edges from left i to matched right w[j] (where w[j] is the left vertex matched to right j). That is because the original problem had a matching condition: the selected left and right sets must be equal size, and each selected right must be matched to a distinct selected left. In our simplified task, we don’t have the equal-size or matching condition, so we can just directly add infinite edges from each left to all its out-neighbors. The function should implement that. The time complexity: building graph O(n + m) where m is total number of edges in adjacency, Dinic runs O(E * sqrt(V)) for bipartite but here general, worst-case O(V^2 E) but in practice fast. Space O(V + E). Edge cases: all weights negative -> answer 0 (empty set); positive weights -> sum of all positives if graph is acyclic? Actually, closure may force selecting negative nodes if they are reachable from positive ones. The min-cut handles that.
