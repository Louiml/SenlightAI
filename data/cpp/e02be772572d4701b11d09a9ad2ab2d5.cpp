// You are given a tree with `n` vertices (numbered 1 to n) and an undirected graph where you need to find the number of vertices `v` such that if we remove `v` (and all edges incident to it), the remaining graph is a forest where every connected component has an even number of vertices. In other words, count the vertices whose removal makes the tree split into components each of even size. Write a C++ function `int count_good_vertices(int n, const vector<pair<int,int>>& edges)` that takes the number of vertices and a list of undirected edges (1-indexed endpoints) and returns the count. The function must be self-contained, not relying on global variables or I/O.

The problem is a classic tree rerooting DP (also known as "all-roots DP"). First, root the tree arbitrarily (say at vertex 1). Define `sub[v]` = size of the subtree rooted at `v` in this rooted tree. A vertex `v` is "good" if, after deleting it, every connected component has even size. The components after deletion are: (1) each child subtree of `v` (size = `sub[child]`), and (2) the "outside" part (everything not in `v`'s subtree, i.e., the rest of the tree) whose size = `n - sub[v]`. All these must be even.

We can compute this with rerooting: first do a DFS to compute `sub` and also compute for each vertex a value `dp[v]` that is 0 if all component sizes coming from its children are even, and 1 otherwise (or we can use a bitmask or boolean). Then a second DFS propagates information from parent to child: for a child `c`, we need to know the size of the "outside" part when considering `c` as new root, which is `n - sub[c]`, and also whether all the components from the parent side (excluding `c`) are even. This is standard rerooting. We maintain for each vertex a list of values from its neighbors in the rooted orientation. For each vertex `v`, we have a list of values (e.g., subtree sizes mod 2) from each child. After computing a prefix/suffix aggregate (here an AND or sum of parity checks), we can evaluate if `v` is good. Then pass the aggregate from the parent side to each child.

Time complexity: O(n) – each edge is processed constant times. Space complexity: O(n) for storing adjacency, subtree sizes, and reroot arrays. Edge cases: single vertex (n=1) is trivial – the answer is 1? Actually removing the only vertex leaves zero components, which is vacuously all even, so answer is 1. For n>1, need careful handling of root and leaves. The snippet provided uses a generic rerooting DP template with boolean AND and a flip (put_vertex returns 1-e). We can simplify: we only need subtree sizes and a parity check.

Implementation approach:
- Build adjacency list.
- First DFS (rooted at 0): compute `sub[v]`.
- Second DFS to compute for each vertex the parity of all child subtree sizes. Let `bad[v]` = number of children with `sub[child] % 2 == 1`. A vertex is good if `bad[v]==0` and `(n - sub[v]) % 2 == 0` (for root, outside size is 0 which is even).
- Rerooting: For each child `c`, we compute the "outside" bad count coming from the parent side. We need to propagate a combined parity from parent to child. Standard technique: for vertex `v` with children list, compute prefix and suffix counts of bad children. Then for child `c`, the outside component size is `n - sub[c]`, and the bad count from parent side is (`bad[v] - (sub[c]%2) + (outside_v % 2 == 1 ? 1 : 0)`), where `outside_v` is the size of the component above `v` when rooted at the original root. But this is a bit tricky; better to use the generic rerooting template from the snippet but simplified.

Actually, for the counting, we can do rerooting by computing for each vertex `v` two arrays: the list of component sizes (as parity) from each neighbor. Then for each vertex, we can check if all those parities are even. To reroot, we propagate from a parent to a child the aggregate of all neighbor parities except that child, plus the outside size parity. Since the tree is small enough, we can even do a naive O(n^2) but we want O(n). So we use prefix/suffix arrays.

Let's define: for each vertex v, we have a list of parities `par` for each neighbor (where `par` for a child is `sub[child] % 2`, and for parent side it's the size of the component above v). At the root, the parent side parity is 0 (since outside empty). At any vertex, if all parities in its list are 0, then it's good.

To compute these parities for all vertices, we can do a two-pass DFS:
- First pass (post-order): compute `sub[v]` and also store for each child the parity `sub[child]%2`. Compute `cnt_odd[v] = sum over children of (sub[child]%2)`.
- For the root, `good[root] = (cnt_odd[root]==0) && ((n - sub[root]) % 2 == 0)` but n-sub[root]=0, so just cnt_odd==0.
- Second pass (pre-order): For each vertex v, we have a value `up_par` which is the parity of the component that includes the parent side (i.e., the size of the rest of the tree when v is considered root). For root, up_par=0. At vertex v, the full list of parities is: all child parities (from first pass) plus `up_par`. So `good[v] = (all these are 0)`. Then for each child c, we need to compute the up_par for c: it is the parity of (n - sub[c])? Actually the outside component size for c is `n - sub[c]`, and its parity is `(n - sub[c]) % 2`. But also we must check that all other components (siblings and parent side) are even. So for child c, we compute `odd_count_excluding_c = (up_par) + sum of child parities except c`. If this odd_count_excluding_c==0, then the parent side is fine; then the child c receives an up_par that is the parity of the outside component? Wait: For the child's perspective, the component that includes the old parent is the whole tree minus the child's subtree. Its size is `n - sub[c]`. So its parity is `(n - sub[c]) % 2`. That is what should be passed as `up_par` to the child. But we also need to know whether the other components (siblings) are even; that is used only to determine if the child is good, not to pass. Actually, to determine if child is good, we need the child's own child parities plus the up_par (which is parity of the big outside). So for child c, we compute `up_par_child = (n - sub[c]) % 2`. And also we need to know if the other components (siblings and the old parent side) are even, but that doesn't affect the child's up_par; it only affects whether child is good. So we must compute `good[child]` as: all child parities of child (from first pass) are even AND `up_par_child` is even (i.e., n - sub[c] even). That's it. So we don't need to propagate odd counts from parent; we just need each vertex's up_par.

Thus, the algorithm simplifies nicely:
- First pass: compute `sub[v]` and `child_odd[v]` = number of children with odd subtree size.
- For each vertex v, `good[v] = (child_odd[v]==0)` (since the outside component parity is handled separately). But wait: For a non-root vertex, the outside component size is `n - sub[v]`; we need that to be even. So `good[v] = (child_odd[v]==0) && ((n - sub[v]) % 2 == 0)`. For root, n-sub[root]=0 even, so same formula works.
- This seems too simple? Let's check: Is it sufficient? For a vertex v, after removal, components are: each child subtree (size sub[child]) and the outside (size n - sub[v]). All must be even. So yes, we only need to know that each child subtree size is even and n-sub[v] is even. That's it! No need for rerooting because the outside size is simply n - sub[v]. But wait: When we root the tree at arbitrary root, for a vertex v, the "outside" component is indeed the rest of the tree, which is connected? Since it's a tree, removing v splits into exactly the connected components: each child subtree (which are disconnected from each other) and the remaining part containing the parent (which is also connected). So it's correct. So the problem reduces to: compute subtree sizes with DFS, then for each v, check if all child subtree sizes are even and n-sub[v] is even.

But careful: What about the case where v is a leaf? Then child_odd=0, n-sub[v] = n-1. For n even, n-1 is odd, so leaf is not good except when n=1. That makes sense.

Edge cases: n=1: sub[1]=1, child_odd=0, n-sub[1]=0 even, so good. Answer 1.

So the solution is O(n). Great! But the original snippet uses full rerooting template, which is overkill. The task is to "create a task inspired by the snippet", so we can present a simpler version that uses the same underlying idea (subtree sizes) but not the full template. The analysis above is sufficient.

Implementation details:
- Build adjacency list as vector<vector<int>>.
- DFS iterative or recursive to compute subtree sizes. Use recursion (depth up to n, but fine for typical constraints).
- After computing sub, iterate over all vertices, for each check all neighbors: if neighbor is not parent in DFS tree, treat as child and check sub[neighbor] %2. Also compute n - sub[v] %2.
- Simpler: during DFS, after computing sub[v], we can compute `bad_children` count of children with odd sub. Then after DFS, for each vertex, check `bad_children[v]==0 && ((n-sub[v])%2==0)`.

To avoid recursion depth issues, we can use iterative stack. But for simplicity, we'll write recursive.

Complexity: O(n) time, O(n) space.

#include <vector>
#include <functional>

// Count vertices whose removal splits the tree into even-sized components.
int count_good_vertices(int n, const std::vector<std::pair<int,int>>& edges) {
    // 1-indexed input; convert to 0-indexed.
    std::vector<std::vector<int>> adj(n);
    for (auto& e : edges) {
        int u = e.first - 1;
        int v = e.second - 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    if (n == 1) return 1;

    std::vector<int> sub(n, 0);
    std::vector<int> odd_children(n, 0);
    std::vector<bool> visited(n, false);

    std::function<int(int)> dfs = [&](int v) -> int {
        visited[v] = true;
        int size = 1;
        for (int to : adj[v]) {
            if (!visited[to]) {
                int child_size = dfs(to);
                size += child_size;
                if (child_size % 2 == 1) {
                    odd_children[v]++;
                }
            }
        }
        sub[v] = size;
        return size;
    };

    dfs(0); // root at vertex 0 (which is vertex 1 in input)

    int answer = 0;
    for (int v = 0; v < n; ++v) {
        if (odd_children[v] == 0 && ((n - sub[v]) % 2 == 0)) {
            ++answer;
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// Test harness
int main() {
    // Test 1: single node tree
    {
        std::vector<std::pair<int,int>> edges;
        assert(count_good_vertices(1, edges) == 1);
    }

    // Test 2: path of 2 nodes
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(count_good_vertices(2, edges) == 0); // removing either leaves a single node (odd)
    }

    // Test 3: path of 4 nodes: 1-2-3-4
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4}};
        // Remove 2: splits into {1} (size1) and {3,4} (size2) -> not all even
        // Remove 3: splits into {1,2} (size2) and {4} (size1) -> not all even
        // Remove 1: leaves {2,3,4} size3 -> not good
        // Remove 4: leaves {1,2,3} size3 -> not good
        assert(count_good_vertices(4, edges) == 0);
    }

    // Test 4: star with center 1 and 4 leaves (n=5)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4},{1,5}};
        // Remove center 1: leaves 4 components of size1 (odd) -> not good
        // Remove a leaf: leaves component size4 (even) -> good only if that leaf's removal works? Actually removing leaf leaves rest of tree size4 even -> good. So 4 leaves are good, center not.
        assert(count_good_vertices(5, edges) == 4);
    }

    // Test 5: A tree where one vertex is good: path of 3 nodes (1-2-3)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3}};
        // n=3. Remove 2: leaves two singletons (odd) -> not good.
        // Remove 1: leaves {2,3} size2 even -> good -> 1 good.
        // Remove 3: leaves {1,2} size2 even -> good -> 3 good.
        assert(count_good_vertices(3, edges) == 2);
    }

    // Test 6: Complete binary tree with 7 nodes (root 1, children 2,3; 2 has 4,5; 3 has 6,7)
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{2,4},{2,5},{3,6},{3,7}};
        // n=7 (odd). Any removal leaves total remaining 6 which is even, but need each component even.
        // Remove root 1: two components size3 each (odd) -> not good.
        // Remove leaf 4: leaves component size6 even -> good. Similarly all leaves? But also internal nodes?
        // Remove node 2: its subtree size3 (nodes 2,4,5) removed? Wait removal of 2 splits: components {4},{5},{1,3,6,7} sizes 1,1,4 -> not all even.
        // So only the 4 leaves are good.
        assert(count_good_vertices(7, edges) == 4);
    }

    // Test 7: A balanced tree with 6 nodes: path 1-2-3-4-5-6
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{5,6}};
        // Check manually: removing 3 gives left {1,2} size2, right {4,5,6} size3 -> odd, not good.
        // removing 4 gives left {1,2,3} size3, right {5,6} size2 -> odd, not good.
        // removing 2 gives left {1} size1, right {3,4,5,6} size4 -> odd, not good.
        // removing 5 gives left {1,2,3,4} size4, right {6} size1 -> odd.
        // removing 1 gives rest size5 odd, not good.
        // So none good.
        assert(count_good_vertices(6, edges) == 0);
    }

    // Test 8: A tree with n=2 and edge, no good
    {
        std::vector<std::pair<int,int>> edges = {{1,2}};
        assert(count_good_vertices(2, edges) == 0);
    }

    // Test 9: A tree with n=4 star: center 1, leaves 2,3,4
    {
        std::vector<std::pair<int,int>> edges = {{1,2},{1,3},{1,4}};
        // Remove center: three components of size1 (odd) -> not good.
        // Remove a leaf: leaves component size3 (odd) -> not good.
        assert(count_good_vertices(4, edges) == 0);
    }

    // Test 10: A tree with n=8: two stars connected by a middle edge? Actually we can craft a specific tree: 
    // Center 1 connected to 2,3,4; and 4 connected to 5,6,7,8? Let's just test a known case. 
    // Use a path of length 4 (1-2-3-4-5) and add an extra leaf to 3: 3-6. n=6.
    { 
        std::vector<std::pair<int,int>> edges = {{1,2},{2,3},{3,4},{4,5},{3,6}};
        // Let's brute think: n=6. Good vertices? 
        // Remove 3: components: {1,2} size2, {4,5} size2, {6} size1 -> not all even.
        // Remove 2: {1} size1, rest {3,4,5,6} size4 -> odd -> not.
        // Remove 4: {1,2,3,6} size4, {5} size1 -> not.
        // Remove 1: rest size5 odd.
        // Remove 5: rest size5 odd.
        // Remove 6: rest size5 odd.
        // So none? Actually n=6 even, but no vertex works. So expected 0.
        assert(count_good_vertices(6, edges) == 0);
    }

    return 0;
}
