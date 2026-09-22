Given an undirected tree with N vertices (2 ≤ N ≤ 2×10^5) and two distinct vertices U and V, write a C++ function that returns the maximum number of edges Takahiro (starting at U) can traverse before being caught by Aoki (starting at V). Both players move alternately: Aoki moves first, each move goes to an adjacent vertex, and both players may stay at their current vertex instead of moving. The game ends when Aoki reaches Takahiro's current vertex on Aoki's turn. Takahiro wants to maximize the number of edges he traverses (i.e., the number of his own moves), and Aoki minimizes it. The input tree is given as an adjacency list of size N, with vertices 0-indexed.

Root the tree at V and compute the depth `dep[x]` (distance from V) and parent array using a DFS from V. The initial distance between the players is `dep[U]`. If `dep[U] <= 1`, Aoki can catch Takahiro on his first move, so the answer is 0. Otherwise (dep[U] ≥ 2), Takahiro can safely move away from V along any path that does not go back towards V. During each full round (Aoki moves then Takahiro moves), both increase their depth by 1 if they move away, so the distance between them remains `dep[U]`, which is ≥ 2, ensuring Takahiro is never caught while moving forward. Thus Takahiro can reach any vertex in the subtree of U (when rooted at V), which is the set of vertices reachable from U without going through `par[U]`. He should choose the vertex with maximum depth in that subtree; let `maxDep` be that depth. After reaching that vertex, he stays there each turn, and the distance shrinks by 1 per full round. He can survive `maxDep - 1` rounds, giving that many moves. The answer is `maxDep - 1`. Edge case: if `dep[U] == 1`, answer is 0. Also, if `dep[U] == 0` (U=V) is not possible per constraints. Time complexity O(N) for two DFS traversals, space O(N).

#include <vector>
#include <algorithm>

// Returns the maximum number of moves Takahiro can make before being caught.
// n: number of vertices, u: Takahiro's start (0-indexed), v: Aoki's start (0-indexed),
// g: adjacency list of the tree.
int maxMoves(int n, int u, int v, const std::vector<std::vector<int>>& g) {
    // DFS from v to compute depths and parents relative to v.
    std::vector<int> dep(n, -1), par(n, -1);
    std::vector<int> stack;
    stack.push_back(v);
    dep[v] = 0;
    while (!stack.empty()) {
        int cur = stack.back();
        stack.pop_back();
        for (int nxt : g[cur]) {
            if (dep[nxt] != -1) continue;
            dep[nxt] = dep[cur] + 1;
            par[nxt] = cur;
            stack.push_back(nxt);
        }
    }

    // If Aoki is adjacent to U, he catches on the first move.
    if (dep[u] <= 1) {
        return 0;
    }

    // DFS from u, avoiding going back to par[u] (i.e., only into the subtree away from v).
    int maxDepth = dep[u];
    stack.clear();
    stack.push_back(u);
    // Use a visited array to avoid cycles (or rely on par, but par is not enough for siblings).
    std::vector<bool> visited(n, false);
    visited[u] = true;
    while (!stack.empty()) {
        int cur = stack.back();
        stack.pop_back();
        maxDepth = std::max(maxDepth, dep[cur]);
        for (int nxt : g[cur]) {
            if (visited[nxt]) continue;
            // Do not go back towards v (i.e., do not go to par[cur] for cur != u,
            // but for cur == u, par[u] is towards v, so skip it).
            if (cur == u && nxt == par[u]) continue;
            visited[nxt] = true;
            stack.push_back(nxt);
        }
    }

    return maxDepth - 1;
}

#include <cassert>
#include <vector>

int maxMoves(int n, int u, int v, const std::vector<std::vector<int>>& g);

int main() {
    // Test 1: Path 0-1-2, u=1, v=0, Aoki catches immediately.
    {
        int n = 3;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1}};
        assert(maxMoves(n, 1, 0, g) == 0);
    }
    // Test 2: Path 0-1-2-3, u=2, v=0.
    {
        int n = 4;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1,3}, {2}};
        assert(maxMoves(n, 2, 0, g) == 2);
    }
    // Test 3: Star: center 0, leaves 1,2,3, u=1, v=0.
    {
        int n = 4;
        std::vector<std::vector<int>> g = {{1,2,3}, {0}, {0}, {0}};
        assert(maxMoves(n, 1, 0, g) == 0);
    }
    // Test 4: Star: center 0, leaves 1,2,3, u=2, v=1 (distance 2).
    {
        int n = 4;
        std::vector<std::vector<int>> g = {{1,2,3}, {0}, {0}, {0}};
        assert(maxMoves(n, 2, 1, g) == 1); // u=2, v=1, maxDep in subtree of u (rooted at 1) is dep[2]=2? Actually dep[2]=2, dep[0]=1, so maxDep=2, answer=1.
    }
    // Test 5: Chain with branch: 0-1-2-3, and 2-4. v=0, u=2, leaves 3 and 4.
    {
        int n = 5;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1,3,4}, {2}, {2}};
        assert(maxMoves(n, 2, 0, g) == 2); // maxDep = 3 (leaf 3), answer = 2.
    }
    // Test 6: v=0, u=0? Not allowed (distinct). Skip.
    // Test 7: Large chain 0-1-2-3-4-5, u=5, v=0, depth=5, maxDep=5, answer=4.
    {
        int n = 6;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1,3}, {2,4}, {3,5}, {4}};
        assert(maxMoves(n, 5, 0, g) == 4);
    }
    // Test 8: u=1, v=2 (distance 1) on path 0-1-2, answer 0.
    {
        int n = 3;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1}};
        assert(maxMoves(n, 1, 2, g) == 0);
    }
    // Test 9: u=0, v=3 on path 0-1-2-3, distance 3, maxDep=0? Wait u=0, v=3, dep[0]=3, subtree of u (rooted at 3) includes 0,1,2? Actually root at 3, so subtree of u=0 includes 0 and 1 and 2? The parent of 0 is 1, so skipping parent, 0 has no other neighbors (only 1), so maxDepth=3, answer=2.
    {
        int n = 4;
        std::vector<std::vector<int>> g = {{1}, {0,2}, {1,3}, {2}};
        assert(maxMoves(n, 0, 3, g) == 2);
    }
    // Test 10: Branch where u has a child that leads to v? Not possible.
    return 0;
}
