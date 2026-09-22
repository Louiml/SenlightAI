Given an undirected graph with `n` vertices and `m` edges, where each edge has a positive integer capacity, write a C++ function that implements the Dinic's maximum flow algorithm. The function should take the number of vertices `n`, the source vertex `s`, the sink vertex `t`, and a vector of edges (each edge contains `u`, `v`, and capacity `c`), and return the maximum flow value from `s` to `t` as a `long long`. The graph may contain parallel edges (multiple edges between the same pair of vertices) and self-loops (edges where `u == v`). Edges are undirected, meaning flow can go in either direction, but the total flow in each direction is bounded by the capacity in that direction. You must implement the graph using adjacency lists with edge indices for efficient reverse-edge updates. The function should handle up to `n = 10^5` vertices and `m = 10^5` edges, with capacities up to `10^9`. Additionally, there may be multiple test cases; your function should be reusable and reset internal structures properly between calls.
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

// Assume the solution function is declared above.

int main() {
    // Test 1: Simple triangle graph with capacities 1 each, max flow from 0 to 2 is 1
    {
        vector<tuple<int,int,long long>> edges = {{0,1,1},{1,2,1},{0,2,1}};
        assert(undirected_max_flow(3, 0, 2, edges) == 1);
    }
    // Test 2: Two parallel edges between same vertices, capacity sum = 5
    {
        vector<tuple<int,int,long long>> edges = {{0,1,2},{0,1,3}};
        assert(undirected_max_flow(2, 0, 1, edges) == 5);
    }
    // Test 3: Self-loop should be ignored
    {
        vector<tuple<int,int,long long>> edges = {{0,0,100},{0,1,4}};
        assert(undirected_max_flow(2, 0, 1, edges) == 4);
    }
    // Test 4: Chain of 4 vertices, each edge capacity 10, max flow = 10
    {
        vector<tuple<int,int,long long>> edges = {{0,1,10},{1,2,10},{2,3,10}};
        assert(undirected_max_flow(4, 0, 3, edges) == 10);
    }
    // Test 5: Bottleneck: edges capacities 5, 100, 5, max flow = 5
    {
        vector<tuple<int,int,long long>> edges = {{0,1,5},{1,2,100},{2,3,5}};
        assert(undirected_max_flow(4, 0, 3, edges) == 5);
    }
    // Test 6: Undirected edge allows flow both ways independently
    {
        vector<tuple<int,int,long long>> edges = {{0,1,3}};
        assert(undirected_max_flow(2, 0, 1, edges) == 3);
        assert(undirected_max_flow(2, 1, 0, edges) == 3);
    }
    // Test 7: Disconnected graph, max flow 0
    {
        vector<tuple<int,int,long long>> edges = {{0,2,1},{1,3,1}};
        assert(undirected_max_flow(4, 0, 3, edges) == 0);
    }
    // Test 8: Larger example with multiple paths
    {
        vector<tuple<int,int,long long>> edges = {
            {0,1,2},{0,2,3},{1,3,2},{2,3,1},{1,2,1}
        };
        // Max flow from 0 to 3: path 0-1-3 gives 2, path 0-2-3 gives 1, total 3
        assert(undirected_max_flow(4, 0, 3, edges) == 3);
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

struct Dinic {
    struct Edge {
        int to;
        ll cap;
        int rev;
    };
    int n;
    vector<vector<Edge>> graph;
    vector<int> level, iter;

    Dinic(int n) : n(n), graph(n), level(n), iter(n) {}

    void add_edge(int from, int to, ll cap) {
        graph[from].push_back({to, cap, (int)graph[to].size()});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1});
    }

    void bfs(int s) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (const auto& e : graph[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }

    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)graph[v].size(); ++i) {
            Edge& e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) break;
            fill(iter.begin(), iter.end(), 0);
            ll f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
        return flow;
    }
};

// Compute maximum flow on an undirected graph with n vertices (0-indexed),
// source s, sink t, and edges as (u, v, capacity).
ll undirected_max_flow(int n, int s, int t, const vector<tuple<int,int,ll>>& edges) {
    Dinic dinic(n);
    for (const auto& [u, v, cap] : edges) {
        if (u == v) continue; // self-loop ignored
        dinic.add_edge(u, v, cap);
        dinic.add_edge(v, u, cap);
    }
    return dinic.max_flow(s, t);
}
// The solution uses Dinic's algorithm, which is a fast maximum flow algorithm for graphs with integer capacities. The algorithm works in phases: in each phase, it constructs a level graph using BFS from the source, assigning each vertex its shortest distance (in terms of number of edges) from the source. Then, it performs DFS from the source to the sink, sending flow along augmenting paths that only go through edges that move from a vertex to a next level (i.e., `dep[v] == dep[u] + 1`). To improve efficiency, a "current arc" pointer (`now[]`) is maintained per vertex, which skips edges that are already saturated, reducing the time per DFS. After each DFS phase, if no more augmenting paths exist (BFS fails to reach the sink), the algorithm terminates. Key edge cases: (1) Self-loops are ignored because they cannot carry flow from source to sink productively; adding them would create cycles that do not help. (2) Parallel edges are handled naturally because each is stored as a separate directed edge with a reverse edge; the algorithm will consider them independently. (3) Since edges are undirected, for every input edge (u,v,c), we add two directed edges: u→v with capacity c, and v→u with capacity c, each with its own reverse edge of capacity 0 initially. This correctly models the undirected capacity constraint. The algorithm's worst-case time complexity is O(V^2 E), but in practice it is much faster, often O(E√V) for certain graphs. The space complexity is O(V + E) for storing adjacency lists and edge arrays.
