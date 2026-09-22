Write a C++ function `canWalkExactly` that takes an integer `N` (the number of intersections in a town), a vector of integers `from` (starting points of directed one-way streets), and a vector of integers `to` (ending points of those streets). The town has intersections numbered 0 through N-1, and each pair `(from[i], to[i])` represents a directed road from `from[i]` to `to[i]`. A student wants to know if they can walk from intersection 0 to intersection 1 using any positive number of steps, and similarly from intersection 1 back to intersection 0, such that for every sufficiently large integer `k`, they can complete each journey in exactly `k` steps (each step traverses one road, and revisiting intersections and roads is allowed). If both `0→1` and `1→0` have this property, return the string `"Freedom"`; otherwise return `"Chores"`. The function should handle up to N = 2000 and up to 2000 roads, including cases with no roads, self-loops, parallel edges, and disconnected components. The graph may contain cycles of various lengths, and the property essentially asks whether the set of possible path lengths from 0 to 1 (and from 1 to 0) has gcd equal to 1, provided a path exists at all; if no path exists in either direction, the result is `"Chores"`.

The key observation is that in a directed graph, once you can reach a destination, the set of possible path lengths (allowing repeats) becomes eventually periodic. Specifically, if from a node `u` you can reach node `t`, and `u` lies on a cycle of length `L`, then you can add multiples of `L` to any path length from `u` to `t` by going around that cycle. The set of all possible path lengths from `a` to `b` has a gcd `g` (if at least one path exists). The property "for every sufficiently large `k`, there is a path of exactly `k` steps" holds if and only if there is at least one path from `a` to `b` and the gcd of all cycle lengths that are "relevant" (i.e., can be used while still reaching `b`) is 1. This is a classic result from Markov chain theory. The given code attempts to find, for each starting node `a` and target `b`, all cycle lengths that can appear in paths from `a` to `b`, then checks if their gcd is 1. The algorithm uses a DFS from `a` to discover the graph reachable from `a`, and for each back edge (or edge to a visited node), it detects the cycle length by a separate BFS/DFS that finds the distance from the target node back to the start of the cycle within that reachable subgraph. The `find_circle` function computes the length of a cycle reachable from a given node when the target is `t`. More concretely, for each node `u` in the DFS tree from `a`, and for each neighbor `v` that has already been assigned a discovery time (indicating a back edge), it performs a search from `v` to see if `t` is reachable; if so, it computes the cycle length as the depth at which `t` is found, and records that length in the array `w`. After processing all such back edges, it computes the gcd of all recorded cycle lengths. If `t` is reachable from `a` and the gcd is 1, then `freedom(a,b)` returns true. The solution must handle graphs with up to 2000 nodes; the DFS is O(N+M), and the cycle detection can be O(N) per back edge, but in the worst case it could be O(N*M) which is acceptable for 2000 nodes and 2000 edges (4 million operations). Edge cases include: no path at all (return false), self-loops (cycle length 1, which makes gcd 1 immediately), parallel edges (cycle length 2 possibly), and large gcds. Time complexity is O(N*(N+M)) in the worst case, space O(N+M). The implementation must be self-contained without using global mutable state; I will design a cleaner recursive function that computes the set of cycle lengths reachable from a start node while still being able to reach the target, using DFS with three colors and a post-order computation of reachability to the target.

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

// Detect if from `start` to `target` there exists a path such that
// the set of all path lengths has gcd 1 (i.e., eventually all large
// integers are achievable).  Requires the graph to be directed.
bool freedom(int start, int target, const std::vector<std::vector<int>>& adj) {
    int n = (int)adj.size();
    // Colors: 0 = unvisited, 1 = in current DFS stack, 2 = fully processed.
    std::vector<int> color(n, 0);
    // Discovery time (depth in DFS tree).
    std::vector<int> depth(n, 0);
    // Reachability to target within the DFS traversal.  We'll fill this later.
    std::vector<bool> canReachTarget(n, false);
    // Store cycle lengths that are "useful" (i.e., the cycle is on a path
    // from start to target).
    std::vector<int> cycleLengths;

    // Helper to compute reachability to target using simple DFS (ignoring cycles).
    std::function<bool(int)> reach = [&](int u) -> bool {
        if (u == target) return true;
        color[u] = 1;
        for (int v : adj[u]) {
            if (color[v] == 0) {
                if (reach(v)) { color[u] = 2; return true; }
            } else if (color[v] == 1) {
                // back edge to current stack: ignore for reachability, but it could
                // still lead to target via other paths.
            }
        }
        color[u] = 2;
        return false;
    };
    // But we need a better approach: compute reachability explicitly with a separate DFS.
    std::vector<int> vis(n, 0);
    std::function<void(int)> dfsReach = [&](int u) {
        vis[u] = 1;
        if (u == target) { canReachTarget[u] = true; }
        for (int v : adj[u]) {
            if (!vis[v]) dfsReach(v);
            if (canReachTarget[v]) canReachTarget[u] = true;
        }
    };
    dfsReach(start);
    if (!canReachTarget[start]) return false;

    // Now DFS from start to find cycles.  
    // For each back edge u->v where v is gray (in current stack), the cycle length
    // is depth[u] - depth[v] + 1.  But we only count cycles that can lead to target:
    // we need to check if from v (the start of the cycle) we can reach target while
    // still going around the cycle.  Since canReachTarget[v] is true, we can go from
    // v to target after completing the cycle.  So any back edge within the DFS tree
    // where canReachTarget[v] is true gives a useful cycle.
    std::fill(color.begin(), color.end(), 0);
    std::function<void(int,int)> dfsCycles = [&](int u, int d) {
        color[u] = 1;
        depth[u] = d;
        for (int v : adj[u]) {
            if (color[v] == 0) {
                dfsCycles(v, d+1);
            } else if (color[v] == 1) {
                // back edge u->v, cycle length = d - depth[v] + 1
                if (canReachTarget[v]) {
                    cycleLengths.push_back(d - depth[v] + 1);
                }
            }
        }
        color[u] = 2;
    };
    dfsCycles(start, 0);

    if (cycleLengths.empty()) return false; // no cycles, only one path length (if any)
    int g = 0;
    for (int len : cycleLengths) {
        g = std::gcd(g, len);
    }
    return g == 1;
}

// Main function: given N intersections and directed roads from[] to[],
// return "Freedom" if both 0->1 and 1->0 have the property.
std::string canWalkExactly(int N, std::vector<int> from, std::vector<int> to) {
    std::vector<std::vector<int>> adj(N);
    for (size_t i = 0; i < from.size(); ++i) {
        adj[from[i]].push_back(to[i]);
    }
    bool f01 = freedom(0, 1, adj);
    bool f10 = freedom(1, 0, adj);
    return (f01 && f10) ? "Freedom" : "Chores";
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test 0 from problem: 2 nodes, edges 0->1 and 1->0. Single cycle of length 2.
    assert(canWalkExactly(2, {0,1}, {1,0}) == "Chores");
    
    // Test 1: 3 nodes, edges: 0->1,1->0,1->2,2->0. Cycle lengths: 2 and 3 -> gcd 1.
    assert(canWalkExactly(3, {0,1,1,2}, {1,0,2,0}) == "Freedom");
    
    // Test 2: 4 nodes, edges: 0->2,2->0,2->3,3->0,0->1. Only cycle length 3 from 0 to 1? Actually check.
    assert(canWalkExactly(4, {0,2,2,3,0}, {2,0,3,0,1}) == "Chores");
    
    // Test 5: N=2000, no edges. No path.
    assert(canWalkExactly(2000, {}, {}) == "Chores");
    
    // Self-loop at node 0, and edge 0->1. From 1 to 0 no path.
    assert(canWalkExactly(2, {0,0}, {0,1}) == "Chores");
    
    // Both 0->1 and 1->0 have a self-loop at both ends, plus direct edges.
    // Cycle length 1 at 0, and 1 at 1, so gcd 1.
    assert(canWalkExactly(2, {0,1,0,1}, {0,1,1,0}) == "Freedom");
    
    // A single edge 0->1 and 1->0, but with a parallel edge 0->1 (length 2 cycle).
    // gcd(2)=2 so not freedom.
    assert(canWalkExactly(2, {0,1,0}, {1,0,1}) == "Chores");
    
    // Disconnected but each side has cycles of length 2 and 3.
    // 0->1, 1->0 (cycle 2), 1->2,2->0 (cycle 3) gives gcd 1.
    assert(canWalkExactly(3, {0,1,1,2}, {1,0,2,0}) == "Freedom");
    
    // One-way path from 0 to 1 but no cycle, so no freedom.
    assert(canWalkExactly(3, {0,1}, {1,2}) == "Chores");
    
    // Complex: node 0 has cycle length 4, and also edge to 1; node 1 has cycle length 6.
    // gcd(4,6)=2 so not freedom.
    assert(canWalkExactly(4, {0,1,2,3,0,1}, {1,2,3,0,1,0}) == "Chores");
    
    // But with cycles lengths 4 and 6, plus a direct back edge giving length 3? 
    // Let's construct: 0->1->2->3->0 (length 4), and 0->2 (length 3), plus from 1 to 0 (length 1?) Actually we need both directions. Simpler: 0->1,1->0 (length2), and 0->2,2->3,3->0 (length3). gcd(2,3)=1.
    assert(canWalkExactly(4, {0,1,0,2,3}, {1,0,2,3,0}) == "Freedom");
    
    return 0;
}
