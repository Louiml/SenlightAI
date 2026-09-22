// Write a C++ function `int maxDeletableEdges(int n, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `n` (vertices are labeled from 1 to n) and a list of undirected edges connecting these vertices, forming a tree. The function must determine the maximum number of edges that can be removed from the tree such that every connected component that remains has an **even** number of vertices. If the total number of vertices `n` is odd, the function should return `-1` because it is impossible to split an odd total into even components. The tree is guaranteed to be connected and acyclic. For example, for a path of 4 vertices (1-2-3-4), one edge can be removed (e.g., the edge between 2 and 3) to split into two components of size 2 each, so the answer is 1. For a star with center 1 and leaves 2,3,4,5,6 (n=6), you can remove edges to leaves 2 and 3 (each component size 1, which is even? No—size 1 is odd, so you cannot remove those edges directly; but you can remove edge 1-2 and 1-3? That leaves components {2}, {3}, and {1,4,5,6} which is size 4 even, so that works, giving 2 removals). The function should return the maximum possible number of removable edges.

#include <cassert>
#include <vector>

int main() {
    // n=4, path 1-2-3-4, remove edge 2-3 -> components {1,2} and {3,4}, size 2 each
    assert(maxDeletableEdges(4, {{1,2},{2,3},{3,4}}) == 1);
    
    // n=2, single edge, child subtree size 1 (odd) -> cannot remove, answer 0
    assert(maxDeletableEdges(2, {{1,2}}) == 0);
    
    // n=6 star: center 1, leaves 2,3,4,5,6
    // Remove edges to leaves 2 and 3: each leaf component size 1 but those become components of size 1, which is odd—wait, but the algorithm counts even child subtrees.
    // Let's check: root at 1, child 2 size=1 odd, not removable; child 3 size=1 odd, not; ... none. So answer should be 0? Actually think again:
    // In a star with 6 vertices, any removal separates a leaf (size 1) from the rest (size 5 odd). That fails. So answer is indeed 0.
    assert(maxDeletableEdges(6, {{1,2},{1,3},{1,4},{1,5},{1,6}}) == 0);
    
    // n=8: two stars connected by an edge, each star has 4 vertices (center and 3 leaves). Remove the connecting edge: each side has 4 even -> 1 removal. Then remove edges to leaves? Each leaf is size 1 odd, no. So answer 1.
    // Build: 1-2-3-4-5 (a path of 5) plus extra leaves? Let's do a simpler valid case:
    // n=8: tree where edges: 1-2, 2-3, 3-4, 4-5, 5-6, 6-7, 7-8 (a path of 8). Remove edges where child subtree even: root at 1, for each node compute sizes: node 8 size1 odd, node7 size2 even -> remove edge 6-7? Actually child subtree of node 7 (when rooted at 1) is size 2 (nodes 7,8) even -> count 1. Then node 5's subtree (5-8) size 4 even -> remove edge 4-5 -> count 2. Then node 3's subtree (3-8) size 6 even -> remove edge 2-3 -> count 3. Then node 1's whole tree size 8 even? That's root, not counted. So answer 3.
    assert(maxDeletableEdges(8, {{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,8}}) == 3);
    
    // n=5 odd -> -1
    assert(maxDeletableEdges(5, {{1,2},{2,3},{3,4},{4,5}}) == -1);
    
    // n=1 odd -> -1
    assert(maxDeletableEdges(1, {}) == -1);
    
    // n=4 star: center 1, leaves 2,3,4. Any removal leaves a leaf (size1) and rest (size3 odd) -> none, answer 0
    assert(maxDeletableEdges(4, {{1,2},{1,3},{1,4}}) == 0);
    
    // n=8, tree: 1-2, 2-3, 3-4, 1-5, 5-6, 6-7, 7-8 (like two paths attached to 1 and 4). Root at 1: child 2 subtree size? Nodes 2,3,4 -> size 3 odd; child 5 subtree size? Nodes 5,6,7,8 -> size 4 even -> remove edge 1-5 -> count 1. Also within child 5: node 6 subtree size? nodes 6,7,8 -> size 3 odd; node 7 subtree size? nodes 7,8 -> size 2 even -> remove edge 6-7 -> count 2. Also from node 5's perspective? Actually we count all edges where child subtree even. So total 2.
    assert(maxDeletableEdges(8, {{1,2},{2,3},{3,4},{1,5},{5,6},{6,7},{7,8}}) == 2);
    
    return 0;
}

#include <vector>
#include <functional>

// Returns the maximum number of edges that can be removed from a tree
// so that every resulting connected component has an even number of vertices.
// If n is odd, returns -1 because it is impossible.
int maxDeletableEdges(int n, const std::vector<std::pair<int,int>>& edges) {
    if (n % 2 != 0) {
        return -1;
    }
    
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
    
    int removable = 0;
    std::vector<int> subtreeSize(n + 1, 0);
    std::vector<bool> visited(n + 1, false);
    
    // DFS to compute subtree sizes and count even-sized subtrees
    std::function<int(int)> dfs = [&](int u) -> int {
        visited[u] = true;
        int sz = 1;
        for (int v : adj[u]) {
            if (!visited[v]) {
                int childSize = dfs(v);
                if (childSize % 2 == 0) {
                    removable++;
                }
                sz += childSize;
            }
        }
        subtreeSize[u] = sz;
        return sz;
    };
    
    dfs(1);
    return removable;
}

// The key observation is to process the tree using DFS (or any traversal) rooted at an arbitrary node (say 1). For each subtree rooted at node `u`, compute its size `sz[u]` as the number of vertices in that subtree. The idea is that an edge connecting a child subtree to its parent can be *safely removed* if the child subtree has an even number of vertices. Why? Because removing that edge separates that child subtree (size even) from the rest of the tree; the remaining part (the rest of the tree) will also have even size because the total `n` is even and subtracting an even number leaves an even number. This partition can be done independently for each child subtree, and the removals do not interact badly—you can remove all edges whose child subtree size is even, and each removal corresponds to exactly one edge. The maximum number of removable edges is thus the count of edges where the child-side subtree size is even. This works as long as `n` is even; if `n` is odd, no valid partition exists, return -1. Edge cases: for `n=1` (a single vertex), no edges exist, answer is 0 (even? 1 is odd, but actually a tree with 1 vertex has 0 edges and is trivially a single component of size 1 which is odd—but the problem says "if n is odd return -1", so for n=1 you return -1 because you cannot have even components). For n=2 (one edge), the child subtree size is 1 (odd), so you cannot remove that edge; answer is 0. The DFS runs in O(n) time and O(n) auxiliary space for the recursion stack and adjacency list, which is acceptable for typical constraints (e.g., up to 1e5 vertices). The algorithm uses a bottom-up traversal: compute subtree sizes, then count for each edge where the child's subtree size is even.
