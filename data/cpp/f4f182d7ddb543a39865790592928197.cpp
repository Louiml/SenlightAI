Given an undirected tree with `n` nodes (numbered `0` to `n-1`) represented as an adjacency list, write a C++ function that computes and returns the parent of every node when the tree is rooted at a specified root node. The function must accept a vector of vectors `e` (where `e[i]` contains the neighbors of node `i`), an integer `root`, and return a `vector<int>` of length `n` such that `result[i]` is the parent of node `i` in the rooted tree. For the `root` node itself, set its parent to `-1`. The input tree is guaranteed to be connected and acyclic (i.e., a valid tree). The function must handle any root, including leaves, and nodes can be visited in any order as long as the parent relationship is correct.
// The core algorithm is a Depth-First Search (DFS) starting from the `root`. We maintain a `parent` vector initialized with some sentinel (e.g., `-1`) for all nodes. We call `dfs(x, p)` where `x` is the current node and `p` is its parent. For each neighbor `y` of `x`, if `y == p`, skip it to avoid revisiting the parent. Otherwise, set `parent[y] = x` and recurse into `dfs(y, x)`. Because the graph is a tree, this traversal visits each node exactly once and correctly assigns parents. Edge cases: if the tree has only one node, the DFS sets its parent to `-1` and returns. If the root is a leaf, the DFS visits only that node and all others via its single neighbor, correctly assigning parents. Time complexity is O(n) because each edge is explored twice (once from each endpoint), and space complexity is O(n) for the parent vector plus O(n) for the recursion stack in the worst case (a skewed tree). The solution uses `const` correctness by passing the adjacency list as `const vector<vector<int>>&`.
#include <vector>

// Compute parent of each node in a tree rooted at 'root'.
// e[u] contains neighbors of node u. The tree is undirected and connected.
// Returns vector of size e.size() where parent[i] is the parent of i,
// and parent[root] = -1.
std::vector<int> computeParents(const std::vector<std::vector<int>>& e, int root) {
    int n = static_cast<int>(e.size());
    std::vector<int> parent(n, -1);

    // Depth-first search to fill parents.
    // x = current node, p = parent of x in this recursion.
    std::function<void(int, int)> dfs = [&](int x, int p) {
        parent[x] = p;
        for (int y : e[x]) {
            if (y == p) continue;
            dfs(y, x);
        }
    };

    dfs(root, -1);
    return parent;
}
(Note: The above uses `std::function`, so we need to include `<functional>` as well. Alternatively, we can implement a helper recursive function. The code below is a complete version with proper includes.)

#include <vector>
#include <functional>

// Compute parent of each node in a tree rooted at 'root'.
// e[u] contains neighbors of node u. The tree is undirected and connected.
// Returns vector of size e.size() where parent[i] is the parent of i,
// and parent[root] = -1.
std::vector<int> computeParents(const std::vector<std::vector<int>>& e, int root) {
    int n = static_cast<int>(e.size());
    std::vector<int> parent(n, -1);

    // Depth-first search to fill parents.
    // x = current node, p = parent of x in this recursion.
    std::function<void(int, int)> dfs = [&](int x, int p) {
        parent[x] = p;
        for (int y : e[x]) {
            if (y == p) continue;
            dfs(y, x);
        }
    };

    dfs(root, -1);
    return parent;
}
#include <cassert>
#include <vector>
#include <functional>

// (Same solution function as above, placed here for completeness)
std::vector<int> computeParents(const std::vector<std::vector<int>>& e, int root) {
    int n = static_cast<int>(e.size());
    std::vector<int> parent(n, -1);
    std::function<void(int, int)> dfs = [&](int x, int p) {
        parent[x] = p;
        for (int y : e[x]) {
            if (y == p) continue;
            dfs(y, x);
        }
    };
    dfs(root, -1);
    return parent;
}

int main() {
    // Test 1: Simple tree 0-1-2, root=0
    std::vector<std::vector<int>> e1 = {{1}, {0,2}, {1}};
    std::vector<int> p1 = computeParents(e1, 0);
    assert(p1 == std::vector<int>({-1, 0, 1}));

    // Test 2: Same tree, root=2
    std::vector<int> p2 = computeParents(e1, 2);
    assert(p2 == std::vector<int>({1, 2, -1}));

    // Test 3: Star tree 0 connected to 1,2,3, root=0
    std::vector<std::vector<int>> e3 = {{1,2,3}, {0}, {0}, {0}};
    std::vector<int> p3 = computeParents(e3, 0);
    assert(p3 == std::vector<int>({-1, 0, 0, 0}));

    // Test 4: Single node
    std::vector<std::vector<int>> e4 = {{}};
    std::vector<int> p4 = computeParents(e4, 0);
    assert(p4 == std::vector<int>({-1}));

    // Test 5: Root is a leaf in a line 0-1-2-3, root=3
    std::vector<std::vector<int>> e5 = {{1}, {0,2}, {1,3}, {2}};
    std::vector<int> p5 = computeParents(e5, 3);
    assert(p5 == std::vector<int>({1, 2, 3, -1}));

    // Test 6: Root is an internal node in same line, root=1
    std::vector<int> p6 = computeParents(e5, 1);
    assert(p6 == std::vector<int>({1, -1, 1, 2}));

    return 0;
}
