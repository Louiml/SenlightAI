// Write a C++ function `int kthAncestor(int n, const std::vector<std::pair<int,int>>& queries, const std::vector<int>& parents, std::vector<int>& results)` where `n` is the number of nodes (labeled 1 to n), `parents` is a 0-indexed vector of size `n` where `parents[i]` is the parent of node `i+1` (for node 1, parent is 0 to indicate no parent), `queries` is a list of pairs `(x, k)` meaning "find the k-th ancestor of node x" (if such ancestor does not exist, return -1), and `results` is an output vector that must be filled with the answer for each query in order. The function returns 0 on success. The tree is rooted at node 1, and each node except the root has exactly one parent given. The input guarantees that the parent of each node is an integer between 0 and n, where 0 means no parent, and for any node x, its ancestors form a valid chain leading to the root. The function must handle up to n=2e5 nodes and up to 2e5 queries efficiently using binary lifting.

The core idea is to precompute a binary lifting table `up[node][j]` representing the 2^j-th ancestor of `node`, where `up[node][0]` is the direct parent (or 0 if none). First, initialize `up[node][0]` from the `parents` vector (note the input is 0-indexed but nodes are 1-indexed, so careful with mapping). Then for each `j` from 1 to 17 (since 2^17 > 2e5), set `up[node][j] = up[ up[node][j-1] ][j-1]`, with the convention that `up[0][*] = 0`. For each query `(x, k)`, decompose `k` into its binary representation; for each bit set at position `j`, update `x = up[x][j]`. If at any point `x` becomes 0, break and the result is -1; otherwise after processing all bits, output `x`. Edge cases: k=0 returns x itself (the loop does nothing). If x is 0 initially (invalid query, but not expected), return -1. Complexity: preprocessing O(n log n), each query O(log n), total O((n+q) log n) time, O(n log n) space.

#include <vector>
#include <cstddef>

// Compute k-th ancestor for each query using binary lifting.
// Returns 0 on success.
int kthAncestor(int n, const std::vector<int>& parents,
                const std::vector<std::pair<int,int>>& queries,
                std::vector<int>& results) {
    const int LOG = 18; // 2^17 > 2e5, use 18 for safety
    std::vector<std::vector<int>> up(n + 1, std::vector<int>(LOG, 0));

    // Set direct parents: parents[i] is for node i+1 (0-indexed input)
    for (int i = 0; i < n; ++i) {
        up[i + 1][0] = parents[i];
    }

    // Precompute binary lifting
    for (int j = 1; j < LOG; ++j) {
        for (int node = 1; node <= n; ++node) {
            up[node][j] = up[ up[node][j-1] ][j-1];
        }
    }

    results.clear();
    results.reserve(queries.size());

    for (const auto& q : queries) {
        int x = q.first;
        int k = q.second;
        for (int j = 0; j < LOG; ++j) {
            if (k & (1 << j)) {
                x = up[x][j];
                if (x == 0) {
                    break; // no ancestor exists beyond this point
                }
            }
        }
        results.push_back(x == 0 ? -1 : x);
    }
    return 0;
}

#include <cassert>
#include <vector>
#include <utility>

// The function declaration is as in the solution above.
int kthAncestor(int n, const std::vector<int>& parents,
                const std::vector<std::pair<int,int>>& queries,
                std::vector<int>& results);

int main() {
    // Example: n=5, tree: 1 root, 2 parent 1, 3 parent 1, 4 parent 2, 5 parent 3
    int n = 5;
    std::vector<int> parents = {0, 1, 1, 2, 3}; // for nodes 1..5
    std::vector<std::pair<int,int>> queries;
    std::vector<int> results;

    // Query: kth ancestor of node 4 with k=0 -> itself (4)
    queries = {{4, 0}};
    kthAncestor(n, parents, queries, results);
    assert(results.size() == 1 && results[0] == 4);

    // Query: k=1 for node 4 -> parent = 2
    queries = {{4, 1}};
    kthAncestor(n, parents, queries, results);
    assert(results.size() == 1 && results[0] == 2);

    // Query: k=2 for node 4 -> parent of 2 = 1
    queries = {{4, 2}};
    kthAncestor(n, parents, queries, results);
    assert(results[0] == 1);

    // Query: k=3 for node 4 -> beyond root -> -1
    queries = {{4, 3}};
    kthAncestor(n, parents, queries, results);
    assert(results[0] == -1);

    // Query: k=1 for node 1 (root) -> -1
    queries = {{1, 1}};
    kthAncestor(n, parents, queries, results);
    assert(results[0] == -1);

    // Mixed queries: node 3 (parent 1), node 5 (parent 3)
    queries = {{3, 1}, {5, 2}, {5, 1}, {5, 3}};
    kthAncestor(n, parents, queries, results);
    assert(results[0] == 1);      // 3's parent is 1
    assert(results[1] == 1);      // 5's 2nd ancestor: 5->3->1
    assert(results[2] == 3);      // 5's parent is 3
    assert(results[3] == -1);     // 5's 3rd ancestor does not exist

    // Large k test: k=100 should give -1 for any node in this small tree
    queries = {{4, 100}};
    kthAncestor(n, parents, queries, results);
    assert(results[0] == -1);

    return 0;
}
