// You are given a bipartite graph encoded as adjacency lists from an input format where the first line contains the number of left-side vertices `n`, followed by `n` lines each starting with a vertex id, then `: (m)`, then `m` right-side neighbor ids. Write a C++ function `int minimumVertexCoverSize(int n, const std::vector<std::vector<int>>& edges)` that takes the number of left vertices `n` and a vector `edges` where `edges[i]` contains all right-side neighbors of left vertex `i`, and returns the size of a minimum vertex cover in this bipartite graph. The graph may have multiple edges (duplicates) and right-side vertices are numbered from `0` upward, but you may assume that the set of right vertices present in the adjacency lists is exactly all right vertices used (no isolated right vertices are given unless they appear as neighbors). Your function must compute the minimum vertex cover size using the König's theorem approach: find the maximum matching, then compute the minimum vertex cover from the alternating paths after the last augmenting search. The function should be self-contained and not rely on global variables.

The problem asks for the minimum vertex cover in a bipartite graph, which by König's theorem has size equal to the maximum matching size. However, we must return the *size* of the minimum vertex cover, not the matching size directly—though they are equal. The classical algorithm: perform a DFS-based Hungarian algorithm to find a maximum matching. Then, from the final unmatched left vertices, run a DFS/BFS that follows alternating paths: from a left vertex, go to all right neighbors not in the matching; from a right vertex, go to the left vertex that it is matched to (if any). The set of vertices reachable from unmatched left vertices via these alternating paths gives the minimum cover: left vertices *not* reachable plus right vertices *reachable* form a minimum vertex cover. The size of this cover equals the maximum matching size. Edge cases: duplicates in adjacency lists should be handled (either deduplicate or the algorithm naturally handles them, but we may keep them, as visiting the same neighbor twice is harmless). Isolated left vertices (with no edges) contribute nothing to matching and are trivially not in the cover. The algorithm runs in O(V * E) where V is the number of left vertices and E is the total number of edges, and uses O(V + E) auxiliary space for the matching, visited flags, and adjacency storage.

#include <vector>
#include <functional>

// Returns the size of a minimum vertex cover in a bipartite graph.
// 'edges[i]' contains the right-side neighbors of left vertex i.
// Right vertices are encoded as integers 0,1,... (implicitly the maximum index used).
int minimumVertexCoverSize(int n, const std::vector<std::vector<int>>& edges) {
    // Determine the maximum right vertex index
    int rightCount = 0;
    for (const auto& list : edges) {
        for (int v : list) {
            if (v >= rightCount) rightCount = v + 1;
        }
    }

    // Hungarian algorithm: find maximum matching
    std::vector<int> matchRight(rightCount, -1);  // -1 means unmatched
    std::vector<int> matchLeft(n, -1);            // optional, for constructing cover later

    std::vector<int> visitedRight;  // reused per DFS

    // DFS for augmenting path
    std::function<bool(int)> dfs = [&](int u) {
        for (int v : edges[u]) {
            if (visitedRight[v]) continue;
            visitedRight[v] = 1;
            if (matchRight[v] == -1 || dfs(matchRight[v])) {
                matchRight[v] = u;
                matchLeft[u] = v;
                return true;
            }
        }
        return false;
    };

    for (int u = 0; u < n; ++u) {
        visitedRight.assign(rightCount, 0);
        dfs(u);
    }

    // Now compute reachability in alternating graph starting from unmatched left vertices
    std::vector<int> reachableLeft(n, 0);
    std::vector<int> reachableRight(rightCount, 0);

    std::vector<int> leftStack;
    for (int u = 0; u < n; ++u) {
        if (matchLeft[u] == -1) {
            reachableLeft[u] = 1;
            leftStack.push_back(u);
        }
    }

    // Alternating traversal
    std::vector<int> rightStack;
    while (!leftStack.empty()) {
        int u = leftStack.back();
        leftStack.pop_back();
        for (int v : edges[u]) {
            if (reachableRight[v]) continue;
            reachableRight[v] = 1;
            rightStack.push_back(v);
        }
    }
    while (!rightStack.empty()) {
        int v = rightStack.back();
        rightStack.pop_back();
        int u = matchRight[v];
        if (u != -1 && !reachableLeft[u]) {
            reachableLeft[u] = 1;
            leftStack.push_back(u);
        }
    }

    // Minimum vertex cover: left vertices NOT reachable + right vertices reachable
    int coverSize = 0;
    for (int u = 0; u < n; ++u) if (!reachableLeft[u]) ++coverSize;
    for (int v = 0; v < rightCount; ++v) if (reachableRight[v]) ++coverSize;

    return coverSize;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include or paste it here.

int main() {
    // Test 1: Single edge, n=1, right vertex 0
    assert(minimumVertexCoverSize(1, {{0}}) == 1);

    // Test 2: Complete bipartite K2,2 (left 0,1; right 0,1 all edges)
    assert(minimumVertexCoverSize(2, {{0,1}, {0,1}}) == 2);

    // Test 3: Star: left 0 connects to right 0,1,2; no other left edges
    assert(minimumVertexCoverSize(3, {{0,1,2}, {}, {}}) == 1);

    // Test 4: Two disjoint edges: left0-r0, left1-r1
    assert(minimumVertexCoverSize(2, {{0}, {1}}) == 2);

    // Test 5: No edges
    assert(minimumVertexCoverSize(3, {{}, {}, {}}) == 0);

    // Test 6: Path of length 2: left0-r0, left1-r0 (both connect to same right)
    assert(minimumVertexCoverSize(2, {{0}, {0}}) == 1);

    // Test 7: More complex: left0-r0,r1; left1-r1,r2; left2-r2. Max matching size = 2? Actually edges: (0,0),(0,1),(1,1),(1,2),(2,2). Max matching can be (0,0),(1,1),(2,2) size 3? But left0,1,2 each distinct and right0,1,2 distinct -> perfect matching size 3. Min cover = 3.
    assert(minimumVertexCoverSize(3, {{0,1}, {1,2}, {2}}) == 3);

    // Test 8: Duplicate edges: left0-r0,r0,r1
    assert(minimumVertexCoverSize(1, {{0,0,1}}) == 1);

    // Test 9: Larger gap: left0-r5, left1-r5,r6; there is also right vertex 6
    assert(minimumVertexCoverSize(2, {{5}, {5,6}}) == 2);

    // Test 10: Isolated left vertex with others connected
    assert(minimumVertexCoverSize(4, {{0}, {}, {1}, {1}}) == 2);

    return 0;
}
