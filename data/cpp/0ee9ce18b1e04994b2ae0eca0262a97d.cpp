Given a directed graph with \(n\) vertices (numbered \(1\) to \(n\)) and \(m\) edges, write a C++ function that determines whether the graph contains a directed cycle. If a cycle exists, return the length of the shortest cycle that divides the length of the cycle found starting from vertex 1 (using a depth-first search that records the discovery time of each vertex). More precisely, let \(L\) be the length of the first cycle detected by a DFS that starts at vertex 1 and assigns each vertex a visit timestamp (starting at 0), and whenever it visits an already-visited vertex, the cycle length is the difference between the current timestamp and the stored timestamp of that vertex. If no cycle is reachable from vertex 1, the function should return \(-1\). The function must take the number of vertices \(n\), the number of edges \(m\), and two arrays (or a vector of pairs) describing the directed edges, and it must return a single integer: the smallest divisor \(d\) of \(L\) such that the graph can be colored with \(d\) colors (integers \(0\) to \(d-1\)) where every edge goes from a vertex colored \(c\) to a vertex colored \((c+1) \mod d\), with the coloring starting at vertex 1 with color 0. If no such divisor exists (including when no cycle is found), return \(-1\). The graph may have multiple edges and self-loops; each self-loop is a cycle of length 1.
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (included by the test compilation unit).
int main() {
    // Test 1: Simple triangle (1->2, 2->3, 3->1) => cycle length 3, smallest divisor 1? Actually with d=1, coloring all vertices 0 works because edges from 0 to 0 mod 1 = 0. But d=1 always works for any acyclic? For a cycle of length 3, d=1 works because all colors are 0, but d=3 also works. Smallest divisor is 1.
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,1}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 2: Self-loop at vertex 1 => cycle length 1, divisor 1 works.
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges = {{1,1}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 3: No cycle reachable from 1 (1->2, no edge back) => -1
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(findSmallestCycleDivisor(n, edges) == -1);
    }
    // Test 4: Cycle length 2 (1->2, 2->1) => divisors {1,2}. d=1 works (all colors 0), so answer 1.
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,1}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 5: Chain with a cycle later: 1->2, 2->3, 3->2 (cycle length 2) => answer 1 because d=1 always works.
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,2}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 6: A graph with two parallel edges from 1 to 1? Already tested.
    // Test 7: Cycle length 4: 1->2,2->3,3->4,4->1. d=1 works, answer 1.
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,1}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 8: A case where d=1 fails? Actually d=1 always works because all vertices get color 0 and any edge goes from 0 to (0+1)%1 = 0, so it's always valid. So the answer is always 1 for any reachable cycle. But the task might have been intended to find the smallest divisor > 1? The snippet prints the smallest divisor that works, and since 1 always works (provided a cycle exists), the answer will always be 1. However, the snippet's Check with mod=1 will always return true if all reachable vertices are colored 0. Indeed the coloring with d=1 assigns all vertices color 0, and (col+1)%1 = 0, so every edge is fine. Therefore for any cycle reachable from 1, the answer is 1. But to be safe, we test this: 
    // Test: cycle length 6, answer should be 1.
    {
        int n = 6;
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,6},{6,1}};
        assert(findSmallestCycleDivisor(n, edges) == 1);
    }
    // Test 9: If no cycle reachable => -1 already tested.
    // Test 10: Mixed graph with unreachable cycle? Actually we only care about reachable from 1, so if vertex 1 is isolated, no cycle reachable, answer -1.
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{2,3},{3,2}}; // vertex 1 has no edges
        assert(findSmallestCycleDivisor(n, edges) == -1);
    }
    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>

// Given a directed graph with n vertices (1..n) and edges given as vector of pairs (from, to),
// return the smallest divisor d of the first detected cycle length reachable from vertex 1
// such that the reachable subgraph can be colored with d colors following edges as +1 mod d.
// If no cycle is reachable, or no such d exists, return -1.
int findSmallestCycleDivisor(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list (1-indexed, but we use 0-indexed internally)
    std::vector<std::vector<int>> adj(n);
    for (const auto& e : edges) {
        int a = e.first - 1; // convert to 0-indexed
        int b = e.second - 1;
        adj[a].push_back(b);
    }

    // First DFS: find a cycle length using timestamps
    std::vector<int> timestamp(n, -1);
    int cycleLen = -1;
    // Recursive lambda for cycle detection
    std::function<int(int,int)> getCycle = [&](int v, int t) -> int {
        if (timestamp[v] != -1) return t - timestamp[v];
        timestamp[v] = t;
        for (int to : adj[v]) {
            int cand = getCycle(to, t + 1);
            if (cand != -1) return cand;
        }
        return -1;
    };
    cycleLen = getCycle(0, 0); // start at vertex 1 (index 0)

    if (cycleLen == -1) return -1;

    // Try divisors from 1 to cycleLen (inclusive) and find the smallest that works
    for (int d = 1; d <= cycleLen; ++d) {
        if (cycleLen % d != 0) continue;
        // Check if coloring with d colors works for reachable vertices from vertex 1
        std::vector<int> color(n, -1);
        bool ok = true;
        // Recursive lambda for coloring check
        std::function<bool(int,int)> check = [&](int v, int col) -> bool {
            if (color[v] != -1) return color[v] == col;
            color[v] = col;
            for (int to : adj[v]) {
                if (!check(to, (col + 1) % d)) return false;
            }
            return true;
        };
        if (check(0, 0)) {
            return d;
        }
    }
    return -1;
}
// The task combines cycle detection and modular graph coloring. First, we need to detect the length of a cycle reachable from vertex 1. The provided snippet uses a DFS with timestamps: each vertex is visited once, and when an edge leads to an already-visited vertex, the difference between the current time and the stored timestamp gives the length of the cycle. This procedure returns the first cycle found in the DFS recursion order, not necessarily the shortest cycle overall, but it is a valid cycle length \(L\). If no cycle is found from vertex 1, the answer is \(-1\). Otherwise, we try every divisor \(d\) of \(L\) (from largest to smallest, or simply iterate from 1 to \(L\) and pick the smallest that works) and check whether the entire graph (all vertices reachable from 1, and by symmetry all vertices? Actually the snippet only checks from vertex 1, so it only ensures that the coloring condition holds for all vertices reachable from vertex 1; vertices not reachable are left unvisited but are not required to satisfy anything. In the reference solution, we will follow the snippet's behavior: only vertices reachable from vertex 1 must be colored correctly; unvisited vertices are ignored). The check uses a DFS that assigns color 0 to the start vertex and each edge forces the next vertex to have color \((c+1) \mod d\). If a vertex is visited again with a different color, the coloring fails. If a valid coloring exists for a divisor \(d\), that \(d\) is the answer, and we can output it. Edge cases: self-loops are cycles of length 1; if \(L = 1\), the only divisor is 1 and the condition is that every vertex reachable from 1 has a self-loop? Actually with \(d=1\), the condition becomes each edge goes from color 0 to color 0, which is always true, so if a self-loop exists at vertex 1, \(L=1\) and \(d=1\) always works. If the graph has no cycle reachable from 1, return \(-1\). The algorithm runs DFS twice: the first to find \(L\) in \(O(n+m)\), and for each divisor (at most \(\sqrt{L}\) distinct, but in code we iterate all \(i\) up to \(L\) and check divisibility, giving \(O(L(n+m))\) in worst case), but since \(L\) could be up to \(n\), the worst-case complexity is \(O(n(n+m))\). However, in practice for typical constraints this is acceptable. Space complexity is \(O(n+m)\) for adjacency lists and timestamps.
