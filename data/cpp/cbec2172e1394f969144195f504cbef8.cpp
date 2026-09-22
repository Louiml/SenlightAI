/*
Given a tree with n nodes (n ≤ 3000) rooted at node 1, write a C++ function `vector<long long> solveTree(int n, vector<pair<int,int>>& edges)` that returns an array `answer` of length n, where `answer[k]` (for 1 ≤ k ≤ n, using 1-indexed positions) represents the minimum possible diameter of the tree after performing exactly k operations. In each operation, you may choose any node and "center" the tree at that node: this creates a new rooted tree where the chosen node becomes the new root, and for every node, its distance to the new root is recomputed. The diameter of the resulting rooted tree is defined as the maximum distance between any node and the root. However, you are also allowed to "merge" any two nodes by adding an edge between them (the resulting graph remains a tree). After k operations, you must choose exactly k distinct nodes to center, and after all centers are applied, the diameter is computed as the maximum of: (a) the distance from any node to the new root, and (b) the maximum distance between any two nodes in the tree (the standard tree diameter after adding the extra edges). The objective is to minimize this maximum over all possible selections of k centers and k-1 added edges (since each operation adds one edge, but the first operation is just choosing the center without adding an edge). Actually, to simplify, the original problem reduces to: for each k from 1 to n, compute the minimum possible value of `max( maxDistanceFromRoot, maxPairDistance )` achievable by selecting any set of k distinct nodes as "special" nodes, where the root distance is computed from node 1 (the original root) but you can "shortcut" distances by moving through any special node as an intermediate hub. In effect, the final answer for each k is the minimum over all subsets S of size k of the maximum of: (the maximum distance from node 1 to any node j when you can jump from any special node to any other special node with zero cost? No, let me rephrase precisely.)

Actually, the given code computes for each i (1..n) the maximum distance from i to any other node in the original rooted tree (using only the directed edges from parent to child after rooting at 1). Then for each k, it tries to find a node j such that `k + mx[j]` is minimized, and then it computes something using that. The task is to understand that pattern and re-implement a clean solution that given the tree edges outputs the same sequence as the given code for all k from 1 to n.

Specifically, the given code performs the following: root the tree at 1, compute for each node i the maximum distance from i to any descendant (using the directed edges). Let `mx[i]` be that maximum distance. Then for each k from 1 to n, if `k >= mx[1]` then answer is `mx[1]` (the original height). Otherwise, find the minimum value of `k + mx[j]` over all j, call that `target`. Then count how many j achieve `target`. If exactly one j does, then the answer is the maximum of `target` and the maximum distance from node 1 to any node that is not that unique j (i.e., `dist[1][other]`). If more than one j achieves the minimum, answer is `mx[1]`. 

Your task is to write a standalone function that implements exactly this logic, given the tree edges, and returns a vector of n long long integers (the answers for k=1..n). The input tree is undirected, nodes numbered 1..n. The function should compute all values efficiently for n ≤ 3000.
*/

#include <bits/stdc++.h>

std::vector<long long> minimumDiameterAfterKOps(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build undirected adjacency
    std::vector<std::vector<int>> undirected(n + 1);
    for (const auto& e : edges) {
        undirected[e.first].push_back(e.second);
        undirected[e.second].push_back(e.first);
    }
    
    // Root tree at 1 using DFS to get directed tree
    std::vector<std::vector<int>> directed(n + 1);
    std::vector<bool> visited(n + 1, false);
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : undirected[u]) {
            if (!visited[v]) {
                directed[u].push_back(v);
                dfs(v);
            }
        }
    };
    dfs(1);
    
    // Compute distances from each node to all reachable nodes in directed tree
    // dist[i][j] = -1 if j not reachable from i
    std::vector<std::vector<int>> dist(n + 1, std::vector<int>(n + 1, -1));
    std::vector<long long> mx(n + 1, 0); // maximum distance from i to any descendant
    
    for (int src = 1; src <= n; ++src) {
        std::queue<int> q;
        q.push(src);
        dist[src][src] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : directed[u]) {
                if (dist[src][v] == -1) {
                    dist[src][v] = dist[src][u] + 1;
                    mx[src] = std::max(mx[src], static_cast<long long>(dist[src][v]));
                    q.push(v);
                }
            }
        }
    }
    
    // Since tree is rooted at 1, dist[1][j] is defined for all j
    std::vector<long long> answer;
    answer.reserve(n);
    for (int k = 1; k <= n; ++k) {
        if (k >= mx[1]) {
            answer.push_back(mx[1]);
            continue;
        }
        long long target = LLONG_MAX;
        for (int j = 1; j <= n; ++j) {
            target = std::min(target, static_cast<long long>(k) + mx[j]);
        }
        int cnt = 0;
        long long dmax = 0;
        for (int j = 1; j <= n; ++j) {
            if (target == static_cast<long long>(k) + mx[j]) {
                cnt++;
                dmax = std::max(dmax, target);
            } else {
                dmax = std::max(dmax, static_cast<long long>(dist[1][j]));
            }
        }
        if (cnt == 1) {
            answer.push_back(dmax);
        } else {
            answer.push_back(mx[1]);
        }
    }
    return answer;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the function definition here or above

int main() {
    // Test 1: Single node
    {
        std::vector<std::pair<int,int>> edges;
        auto res = minimumDiameterAfterKOps(1, edges);
        assert(res.size() == 1);
        assert(res[0] == 0);
    }
    
    // Test 2: Simple chain of 3 nodes (1-2-3)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        auto res = minimumDiameterAfterKOps(3, edges);
        // Expected: k=1: mx[1]=2, k>=2 true? k=1<2 so we compute. Let's manually compute:
        // mx: node1 max dist to 2 and 3 = 2; node2 max dist to 3 =1; node3 max dist =0
        // k=1: target=min(1+mx[j]) = min(1+2,1+1,1+0)=1 -> only j=3 gives 1, cnt=1, dmax = max(1, dist[1][1], dist[1][2])? Actually for j=1,2 not achieving target, dist[1][1]=0, dist[1][2]=1, so dmax becomes max(1,0,1)=1. So answer[0]=1.
        // k=2: k>=mx[1] (2>=2) true -> answer=2.
        // k=3: answer=2.
        std::vector<long long> expected = {1, 2, 2};
        assert(res == expected);
    }
    
    // Test 3: Star with root 1 connected to leaves 2,3,4
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        auto res = minimumDiameterAfterKOps(4, edges);
        // mx[1]=1, mx[2]=mx[3]=mx[4]=0
        // k=1: k>=1 true -> answer=1
        // k=2,3,4: answer=1
        std::vector<long long> expected = {1, 1, 1, 1};
        assert(res == expected);
    }
    
    // Test 4: Chain of 5 nodes (1-2-3-4-5)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5}};
        auto res = minimumDiameterAfterKOps(5, edges);
        // Manual computation:
        // mx[1]=4, mx[2]=3, mx[3]=2, mx[4]=1, mx[5]=0
        // k=1: target = min(1+4=5,1+3=4,1+2=3,1+1=2,1+0=1)=1 (j=5) cnt=1 -> dmax = max(target=1, dist[1][1]=0,dist[1][2]=1,dist[1][3]=2,dist[1][4]=3) = 3
        // k=2: target = min(2+4=6,2+3=5,2+2=4,2+1=3,2+0=2)=2 (j=5) cnt=1 -> dmax=max(2,0,1,2,3)=3
        // k=3: target = min(3+4=7,3+3=6,3+2=5,3+1=4,3+0=3)=3 (j=5) cnt=1 -> dmax=max(3,0,1,2,3)=3
        // k=4: k>=mx[1]? 4>=4 true -> answer=4
        // k=5: answer=4
        std::vector<long long> expected = {3, 3, 3, 4, 4};
        assert(res == expected);
    }
    
    // Test 5: More complex tree with branching (1-2,1-3,2-4,2-5)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5}};
        auto res = minimumDiameterAfterKOps(5, edges);
        // Root at 1: directed edges: 1->2,1->3,2->4,2->5
        // mx[1]: distances to 2,3,4,5 = 1,1,2,2 -> max 2
        // mx[2]: distances to 4,5 = 1,1 -> max 1
        // mx[3]=0, mx[4]=0, mx[5]=0
        // dist[1][j] for all j: 0,1,1,2,2
        // k=1: k<2 so target = min(1+2=3,1+1=2,1+0=1,1+0=1,1+0=1)=1 (j=3,4,5 give 1) cnt=3 >1 -> answer=mx[1]=2
        // k=2: k>=2? 2>=2 true -> answer=2
        // k=3,4,5: answer=2
        std::vector<long long> expected = {2, 2, 2, 2, 2};
        assert(res == expected);
    }
    
    // Test 6: Larger chain to verify independence from order (reverse edges)
    {
        std::vector<std::pair<int,int>> edges = {{3,2},{2,1},{4,3},{5,4}};
        auto res = minimumDiameterAfterKOps(5, edges);
        // Same as chain 1-2-3-4-5 but edges listed in different order
        std::vector<long long> expected = {3, 3, 3, 4, 4};
        assert(res == expected);
    }
    
    return 0;
}

// We need to:
// 1. Build adjacency lists for an undirected tree.
// 2. Root the tree at node 1 using DFS to build a directed tree (parent to children). This gives `adj2`.
// 3. Compute distances from every node to every other node in the directed tree (only following parent-to-child edges). Since the tree is rooted, the distance from i to j is defined only if j is in the subtree of i. For pairs not in the subtree, we treat as unreachable. We'll compute `dist[i][j]` for all i,j using BFS from each i on the directed graph (or DFS). Since n ≤ 3000, O(n^2) BFS is acceptable (9 million operations).
// 4. For each node i, compute `mx[i]` = maximum `dist[i][j]` over all j reachable from i via directed edges. If no children, mx[i]=0.
// 5. Then for each k from 1 to n:
//    - If `k >= mx[1]`, answer[k] = mx[1].
//    - Else compute `target = min_{j=1..n} (k + mx[j])`. Note that j can be any node, including those with mx[j]=0.
//    - Count how many j achieve that minimum.
//    - If count == 1, let that unique node be `u`. Compute `dmax` = maximum over all j != u of `dist[1][j]` (note: dist[1][j] is defined only if j is in subtree of 1, which is all nodes since 1 is root). Also note that for j = u, we use `target` as its distance? The code uses `dmax` initially 0, then for each j: if (target == i + mx[j]) then we skip and set dmax = max(dmax, target) (this effectively sets dmax to target if that j is the unique one). Actually careful reading: In the code, for each j, if `target == i + mx[j]` then it increments cnt and sets dmax = max(dmax, target). Otherwise, it sets dmax = max(dmax, dist[1][j]). So after the loop, if cnt==1, dmax becomes max(target, max_{j≠u} dist[1][j]). Since target = k + mx[u] and mx[u] is max distance from u to its subtree. The answer is that dmax.
//    - If count > 1, answer[k] = mx[1].
// 6. Return vector of size n (index 0 for k=1, etc.) with long long values.
//
// Edge cases: n=1 tree (no edges). Then mx[1]=0. For k=1, condition k>=mx[1] (1>=0) true, answer=0. Output vector of size 1 with value 0. Also note that mx[1] is the height of the tree (maximum depth from root to leaf). Distances are 0-based.
//
// Time complexity: O(n^2) for building all-pairs distances (BFS from each node on directed tree, each BFS O(n) since tree has n-1 directed edges). Also O(n^2) for computing mx (we compute during BFS). Then for each k we iterate over all j to find minimum, that's O(n^2) total. So overall O(n^2) time, O(n^2) space for dist matrix. This is fine for n ≤ 3000.
//
// We must implement a clean function `std::vector<long long> minimumDiameterAfterKOps(int n, const std::vector<std::pair<int,int>>& edges)`.
