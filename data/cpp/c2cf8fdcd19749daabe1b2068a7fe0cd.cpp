/*
You are given a sequence of integers, each representing the squarefree part of a number (i.e., the product of prime factors that appear to an odd power). Your task is to write a C++ function `int shortestPrimeCycle(const std::vector<int>& nums)` that returns the length of the shortest cycle in a graph constructed as follows: For each integer `x` in `nums`, factorize it into its distinct prime factors that appear to an odd power (i.e., the squarefree part). If the squarefree part is 1, the answer is immediately 1 (cycle of length 1). If it has exactly one prime factor `p`, add an edge between vertex `1` and vertex `p`. If it has exactly two prime factors `p` and `q` (with `p != q`), add an edge between `p` and `q`. If it has more than two distinct prime factors, ignore that number (it cannot contribute to a short cycle). After processing all numbers, the graph may contain parallel edges (i.e., a duplicate edge added twice), which form a 2-cycle. Otherwise, the graph is a simple undirected graph with vertices identified by prime numbers (and the special vertex `1`). Your function must return the length of the shortest cycle in this graph, or -1 if the graph is acyclic (i.e., no cycle exists). Note that the graph is undirected and unweighted.
*/
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// Returns the length of the shortest cycle in the graph constructed from the given numbers.
// Each number x contributes an edge between its distinct prime factors that appear to an odd power.
// Special case: if the squarefree part is 1, the answer is 1.
// If a number has more than two distinct odd-exponent primes, it is ignored.
// Returns -1 if the graph has no cycle.
int shortestPrimeCycle(const std::vector<int>& nums) {
    // Map for duplicate-edge detection and adjacency list
    set<pair<int,int>> edges;
    vector<vector<int>> g(2000000); // enough for primes up to 1e6? Actually we'll only use up to N
    const int N = 2000000; // safe upper bound for primes (since nums up to ~1e6, primes up to 1e6)
    // Actually we will use a smaller bound but this is fine.

    vector<pair<int,int>> edgelist;
    for (int x : nums) {
        int temp = x;
        vector<int> primes;
        for (int i = 2; i * i <= temp; ++i) {
            if (temp % i == 0) {
                int cnt = 0;
                while (temp % i == 0) {
                    temp /= i;
                    ++cnt;
                }
                if (cnt & 1) primes.push_back(i);
            }
        }
        if (temp > 1) primes.push_back(temp);
        if (primes.empty()) {
            // squarefree part is 1 -> cycle of length 1
            return 1;
        }
        else if (primes.size() == 1) {
            edgelist.push_back({1, primes[0]});
        }
        else if (primes.size() == 2) {
            edgelist.push_back({primes[0], primes[1]});
        }
        // if primes.size() > 2, ignore
    }

    // Build graph and detect duplicate edges (2-cycle)
    set<pair<int,int>> seen;
    for (auto& e : edgelist) {
        int u = e.first, v = e.second;
        if (u > v) swap(u, v);
        if (seen.count({u, v})) {
            // duplicate edge -> shortest cycle is 2
            return 2;
        }
        seen.insert({u, v});
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // BFS from each vertex that appears in edges to find shortest cycle
    // We'll use a smaller bound: we only need vertices up to max prime seen + 1
    int maxV = 0;
    for (auto& e : edgelist) {
        maxV = max(maxV, max(e.first, e.second));
    }
    // If no edges, no cycle
    if (edgelist.empty()) return -1;

    int ans = INF;
    vector<int> dist(N, INF), son(N, -1);

    // We'll run BFS from each vertex in the range 1..maxV plus 1 (since vertex 1 may be present)
    // but actually we can run from all vertices in the set that have degree > 0
    set<int> verts;
    for (auto& e : edgelist) {
        verts.insert(e.first);
        verts.insert(e.second);
    }

    for (int S : verts) {
        // Reset distances for this BFS (we use assignment to INF at the end)
        queue<int> q;
        q.push(S);
        dist[S] = 0;
        vector<int> visited;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            visited.push_back(u);
            for (int v : g[u]) {
                if (dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    if (u == S) {
                        son[v] = v; // root's neighbor is the representative
                    } else {
                        son[v] = son[u];
                    }
                    q.push(v);
                } else if (u != S && v != S && son[u] != son[v]) {
                    // This is a non-tree edge connecting two different branches
                    // But careful: because we have a simple graph (no parallel edges due to duplicate detection),
                    // this gives a cycle length
                    ans = min(ans, dist[u] + dist[v] + 1);
                }
            }
        }
        // Reset distances for next BFS
        for (int u : visited) {
            dist[u] = INF;
            son[u] = -1;
        }
    }

    if (ans == INF) return -1;
    return ans;
}
#include <bits/stdc++.h>
#include <assert.h>
using namespace std;

// The solution function is as provided above.

int main() {
    // Single number that is a perfect square (e.g., 4 = 2^2, squarefree part 1)
    {
        vector<int> nums = {4};
        assert(shortestPrimeCycle(nums) == 1);
    }
    // One number with one prime factor: 2 (squarefree part 2) -> edge (1,2), no cycle
    {
        vector<int> nums = {2};
        assert(shortestPrimeCycle(nums) == -1);
    }
    // Two numbers with same edge: 2 and 8 (8=2^3, squarefree part 2) -> duplicate edge (1,2) twice -> cycle 2
    {
        vector<int> nums = {2, 8};
        assert(shortestPrimeCycle(nums) == 2);
    }
    // Three numbers forming a triangle: 6 (2*3), 10 (2*5), 15 (3*5) -> edges (2,3), (2,5), (3,5) -> cycle length 3
    {
        vector<int> nums = {6, 10, 15};
        assert(shortestPrimeCycle(nums) == 3);
    }
    // Path: 2, 3, 5? Actually 2 and 3 gives edge (1,2) and (1,3) -> no cycle
    {
        vector<int> nums = {2, 3};
        assert(shortestPrimeCycle(nums) == -1);
    }
    // Cycle through 1: 2, 3, 6 gives edges (1,2), (1,3), (2,3) -> cycle (1-2-3-1) length 3
    {
        vector<int> nums = {2, 3, 6};
        assert(shortestPrimeCycle(nums) == 3);
    }
    // Cycle 2 via two numbers sharing same pair: 6 and 12? 12=2^2*3 -> squarefree part 3, so edge (1,3) with 6 gives edge (2,3) -> no cycle
    // Actually 6 gives (2,3), 12 gives (1,3) -> path 1-3-2, no cycle
    {
        vector<int> nums = {6, 12};
        assert(shortestPrimeCycle(nums) == -1);
    }
    // Larger cycle: 2*3, 3*5, 5*7, 7*2 -> edges (2,3),(3,5),(5,7),(7,2) -> cycle length 4
    {
        vector<int> nums = {6, 15, 35, 14}; // 14=2*7
        assert(shortestPrimeCycle(nums) == 4);
    }
    // Ignored number with three primes: 2*3*5=30, plus edges that form a triangle from other numbers
    {
        vector<int> nums = {30, 6, 10, 15}; // 30 ignored, 6->(2,3), 10->(2,5), 15->(3,5) -> triangle
        assert(shortestPrimeCycle(nums) == 3);
    }
    // No numbers at all: empty input -> no edges -> -1
    {
        vector<int> nums;
        assert(shortestPrimeCycle(nums) == -1);
    }
    cout << "All tests passed!" << endl;
    return 0;
}
// The problem reduces to detecting the girth (shortest cycle length) of an undirected graph built from the given numbers. Each number contributes either a direct 1-cycle (if squarefree part is 1), or an edge between two vertices (1 and p, or p and q). A duplicate edge (same undirected edge added twice) immediately gives a cycle of length 2, because two parallel edges form a 2-cycle in multigraph terms; in simple-graph terms this is also the shortest possible cycle (length 2 is allowed here because we count each edge as one unit, and two edges between the same pair of vertices make a cycle of length 2). After building the graph with an adjacency list, we run a BFS from each vertex (up to a limit, e.g., all vertices up to 1007, but in practice only vertices that appear in edges) to find the shortest cycle. The standard technique: for each source `s`, BFS tree, and when we encounter an edge `(u,v)` that is not a tree edge and connects two vertices in different branches (or connects a vertex to an ancestor not its parent), we compute `dist[u]+dist[v]+1` as a candidate cycle length. We must be careful to not count the parent edge. To handle the 2-cycle case (duplicate edges), we can detect it while adding edges: if the same undirected pair appears twice, return 2. Alternatively, during BFS, parallel edges will cause the same check to see `son[u] != son[v]` even when u and v are adjacent by two edges, giving a 2-cycle. However, the simplest is to detect duplicate edges upfront. Edge case: if any number's squarefree part is 1, return 1 immediately. Also, if a number has more than 2 distinct odd-exponent primes, it's ignored (does not add an edge, but it also cannot create any cycle by itself). The graph may have isolated vertices or vertices without any edges, but BFS from every vertex up to 1007 (or up to the maximum prime seen) is fine because the original solution limits to 1007; we can do the same but it's safe to only run BFS from vertices that appear in edges. Time complexity: For each source vertex, BFS over the graph with V vertices and E edges is O(V+E). If we run it from all V vertices, it's O(V*(V+E)). With V at most a few thousand (primes up to ~1e6 but we only consider those that appear), this is acceptable for the constraints (the original limits to 1007). Space complexity is O(V+E).
