Given a directed graph with \(n\) vertices (numbered 1 to \(n\)) and \(m\) edges, write a C++ function `int shortestReachableInPowers(int n, const std::vector<std::pair<int,int>>& edges)` that returns the length of the shortest path from vertex 1 to vertex \(n\) where each step in the path must be a path of length exactly \(2^k\) for some non-negative integer \(k\) (i.e., you can jump from \(u\) to \(v\) in one move if there is a walk of length \(2^k\) from \(u\) to \(v\) in the original graph). If vertex \(n\) is unreachable, return \(-1\). The graph may contain multiple edges and self-loops; edge weights are all 1 per original edge. The function must handle \(n \leq 50\), \(m \leq 10000\).
// The key observation is that if a walk of length \(2^k\) exists between two vertices, it can be built by combining two walks of length \(2^{k-1}\). So we can precompute a boolean matrix `reach[k][u][v]` for \(k=0\) to 30 (since \(2^{30} > 10^9\) but \(n \leq 50\) so we only need up to about \(\log_2(50)=6\)? Actually we need up to 31 because the path may combine powers; however the largest useful power is bounded by the fact that any simple path has at most \(n-1\) edges, but because we allow jumps that are walks, the maximum jump length we need is the maximum possible shortest walk length, which is bounded by \(2^{\lceil\log_2(n)\rceil}\)? Let's think: If we have a path of length L in the original graph, we can represent L as a sum of distinct powers of two, but our move allows only a single power of two per move, so we need to be able to jump any power of two length walk. The maximum power we need is the maximum possible shortest-walk distance between any pair, which is at most \(n\) (or maybe \(n-1\)? Actually a walk can be longer, but if there is a walk of length L, there is a simple path of length ≤n-1 that is a sub-walk? Not necessarily for directed graphs. However the shortest walk distance in unweighted directed graph is at most \(n-1\) if reachable, otherwise infinite. So we only need powers up to \(2^{\lceil\log_2(n)\rceil}\)? But careful: the problem asks for shortest number of jumps, each jump of length exactly a power of two. The maximum jump length needed is at most the longest possible shortest walk distance? Actually we can use a jump of length larger than the shortest path length, but that might be wasteful. In the worst case, to reach a vertex, we might need a jump of length up to something like the diameter of the graph, which for directed is at most \(n-1\)? Yes, if reachable, there is a simple path of length ≤n-1. So we need powers up to 2^k where k is the smallest such that 2^k ≥ n-1, so for n≤50, that's k up to 6 (since 2^5=32, 2^6=64). But the original code uses 31 to be safe. We'll use 31 to be safe.
//
//   
// Algorithm:  
// 1. Initialize `reach[0][i][j] = 1` if there is an edge i->j.  
// 2. For k from 1 to 30:  
//    For all i,j, `reach[k][i][j] = OR over t of (reach[k-1][i][t] && reach[k-1][t][j])`.  
//    This is standard boolean matrix squaring.  
// 3. Build a new graph where there is an edge from i to j if any `reach[k][i][j]` is true for some k (0..30). Set weight 1 for such edges. Also initialize `dist[i][j]` as INF, set `dist[i][j]=1` if there is any such edge, else INF.  
// 4. Run Floyd-Warshall on `dist` to find shortest path in this new graph.  
// 5. Return `dist[1][n]` if not INF else -1.
//
//   
// Edge cases:  
// - If n=1, then vertex 1 and n are the same, so answer should be 0 (we are already at the target).  
// - If there is an edge from 1 to n directly, answer is 1.  
// - If unreachable, return -1.  
// - Self-loops do not help unless they allow reaching the target, but they are handled by the matrix multiplication.
//
//   
// Time complexity: O(31 * n^3) for precomputation (but we can optimize using bitsets or just O(31 * n^3) because n≤50). Plus O(n^3) for Floyd-Warshall. So overall O(31*n^3 + n^3) = O(n^3) with constant factor ~31, which for n=50 is trivial. Space: O(31*n^2) for reach matrices, plus O(n^2) for dist.
//
//   
// Correctness proof: Any path using jumps of powers of two corresponds to a walk in the original graph of length equal to sum of powers of two used. Conversely, any walk of length exactly a power of two is represented by the reach matrix. Then the shortest number of jumps is the shortest path in the auxiliary graph where each original walk of length any power of two is an edge. Floyd-Warshall gives the minimum number of such edges.
#include <vector>
#include <algorithm>
#include <limits>

const int INF = 0x3f3f3f3f;

// Returns the minimum number of jumps from vertex 1 to vertex n,
// where each jump must be a walk of length exactly 2^k for some k>=0.
// If unreachable, returns -1.
int shortestReachableInPowers(int n, const std::vector<std::pair<int,int>>& edges) {
    // Maximum power exponent; 2^30 is huge enough for any graph with n<=50.
    const int MAXK = 30;
    
    // reach[k][i][j] = true if there is a walk of length 2^k from i to j.
    // Use a 3D vector.
    std::vector<std::vector<std::vector<bool>>> reach(
        MAXK+1, std::vector<std::vector<bool>>(n+1, std::vector<bool>(n+1, false)));
    
    // Base case k=0: walks of length 1 are exactly the edges.
    for (const auto& e : edges) {
        int u = e.first, v = e.second;
        if (u >= 1 && u <= n && v >= 1 && v <= n) {
            reach[0][u][v] = true;
        }
    }
    
    // For k>0, a walk of length 2^k is a concatenation of two walks of length 2^(k-1).
    for (int k = 1; k <= MAXK; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int t = 1; t <= n; ++t) {
                if (reach[k-1][i][t]) {
                    for (int j = 1; j <= n; ++j) {
                        if (reach[k-1][t][j]) {
                            reach[k][i][j] = true;
                        }
                    }
                }
            }
        }
    }
    
    // Build a new graph where an edge (i,j) exists if any power-of-two walk from i to j exists.
    std::vector<std::vector<int>> dist(n+1, std::vector<int>(n+1, INF));
    for (int i = 1; i <= n; ++i) {
        dist[i][i] = 0; // distance to self is 0
    }
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (i != j) {
                bool hasEdge = false;
                for (int k = 0; k <= MAXK; ++k) {
                    if (reach[k][i][j]) {
                        hasEdge = true;
                        break;
                    }
                }
                if (hasEdge) {
                    dist[i][j] = 1;
                }
            }
        }
    }
    
    // Floyd-Warshall to find shortest number of jumps.
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    if (dist[1][n] == INF) {
        return -1;
    }
    return dist[1][n];
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test 1: single edge 1->2
    assert(shortestReachableInPowers(2, {{1,2}}) == 1);
    
    // Test 2: no path
    assert(shortestReachableInPowers(2, {{1,1}}) == -1); // only self-loop on 1, cannot reach 2
    
    // Test 3: need two jumps: 1->2->3, each step length 1 (2^0)
    assert(shortestReachableInPowers(3, {{1,2}, {2,3}}) == 2);
    
    // Test 4: can jump directly with a walk of length 2: edges 1->2, 2->1 (so 1->1 length 2), but that doesn't help. Better: 1->2, 2->3 gives 1->3 walk of length 2 directly, so answer 1.
    assert(shortestReachableInPowers(3, {{1,2}, {2,3}}) == 2? Wait, actually 1->2->3 is length 2, so there is a walk of length 2 from 1 to 3, so a direct jump of length 2 exists, so answer is 1. But my function should return 1. Let's check: reach[1][1][3] = true because reach[0][1][2] && reach[0][2][3]. So dist[1][3]=1. So answer 1.
    assert(shortestReachableInPowers(3, {{1,2}, {2,3}}) == 1);
    
    // Test 5: self-loop on 1 and edge 1->2. From 1 to 2 walk of length 1 exists, so answer 1.
    assert(shortestReachableInPowers(2, {{1,1}, {1,2}}) == 1);
    
    // Test 6: longer path requiring combination of powers.
    // Graph: 1->2, 2->3, 3->4, 4->5. Shortest path is 4 edges (length 4). But can we do it in fewer jumps? We can jump length 4 directly because there is a walk of length 4 from 1 to 5 (the path). So answer 1.
    assert(shortestReachableInPowers(5, {{1,2}, {2,3}, {3,4}, {4,5}}) == 1);
    
    // Test 7: a case where need 2 jumps because no single walk of power-of-two length reaches.
    // Graph: 1->2, 2->3, 3->4, 4->1 (cycle). Want to go 1 to 3. There is walk length 2 (1->2->3) so direct jump length 2 gives answer 1. Actually that's 1. Need a case where answer is 2. For example, graph: 1->2, 2->3, 3->4, 4->5, 5->6, 6->1 (cycle of length 6). To go from 1 to 5: walk lengths: 1->2->3->4->5 is length 4 (power of 2? 4=2^2, so direct jump length 4 gives answer 1). Try 1 to 4: length 3, not a power of two, so cannot jump directly. But can do 1->2->3 (length 2? that goes to 3) then from 3 to 4 is length 1, so two jumps: jump length 2 from 1 to 3, then jump length 1 from 3 to 4. So answer 2. Test:
    // edges: 1->2, 2->3, 3->4, 4->5, 5->6, 6->1. n=6. Shortest jumps from 1 to 4: 2. Check with function.
    std::vector<std::pair<int,int>> cycle = {{1,2},{2,3},{3,4},{4,5},{5,6},{6,1}};
    assert(shortestReachableInPowers(6, cycle) == 2);
    
    // Test 8: n=1, already at target, should return 0.
    assert(shortestReachableInPowers(1, {}) == 0);
    
    // Test 9: unreachable with a branch.
    assert(shortestReachableInPowers(4, {{1,2},{2,3},{4,4}}) == -1);
    
    // Test 10: multiple edges and self-loops, still reachable.
    assert(shortestReachableInPowers(3, {{1,1},{1,2},{2,2},{2,3}}) == 1); // direct walk length 2 via self-loops? Actually 1->2->3 is length 2, so answer 1.
    
    return 0;
}
