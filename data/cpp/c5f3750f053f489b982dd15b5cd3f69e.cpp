// Given an unweighted tree with \(n\) vertices (numbered \(1\) through \(n\)), write a C++ function `long long countValidPermutations(const std::vector<std::vector<int>>& adj)` that returns the number of valid permutations of the vertices when the tree is rooted at vertex \(1\). A permutation \(p_1, p_2, \dots, p_n\) is valid if for every vertex \(v\) (except the root), the parent of \(v\) appears before \(v\) in the permutation. Since the answer can be large, return it modulo \(998244353\). The input adjacency list `adj` is 0-indexed internally, but the tree vertices are numbered from \(1\) to \(n\); the function should adapt accordingly. The tree is connected and acyclic, and \(n \ge 1\).
// The problem asks for the number of linear extensions of a rooted tree where each node must appear after its parent. This is a classic combinatorial counting problem. For a rooted tree, the number of valid permutations is given by the formula:  
// \[
// \text{Answer} = n! \times \prod_{v=1}^{n} \frac{1}{size(v)}
// \]  
// where \(size(v)\) is the number of nodes in the subtree rooted at \(v\). This formula is derived from the fact that in any valid topological order, the relative order of nodes within each subtree is independent, and the root must come first. The derivation uses the multinomial coefficient: for each node, we choose positions for its subtree among the remaining slots. The product of subtree sizes appears in the denominator.  
//
// We compute this by a DFS from the root (vertex \(1\)). For each node, we compute its subtree size `sz[v]` recursively. Then we compute the denominator product: for every node \(v\), multiply `inv(sz[v])` (modular inverse modulo \(998244353\)) together. The final answer is `n! * product_inv_sizes mod MOD`.  
//
// Edge cases:  
// - \(n=1\): The only permutation is `[1]`, and the answer is \(1\). The formula gives \(1! / 1 = 1\).  
// - The tree is large; recursion depth may be up to \(n\), so we must consider potential stack overflow. For safety, we can either increase recursion limit or implement an iterative DFS. Since typical contest constraints are \(n \le 2\times 10^5\), we can use recursion with caution, or we can use an explicit stack to compute subtree sizes iteratively. For a high-quality solution, we implement an iterative post-order traversal to avoid recursion depth issues.
//
// Time complexity: \(O(n)\) for DFS and modular inverse precomputation (if using fast exponentiation per node, it would be \(O(n \log MOD)\), but we can precompute inverses of \(1\) to \(n\) in \(O(n)\) using linear inverse formula). Space complexity: \(O(n)\) for adjacency list, sizes, and factorial/inverse arrays.
//
// We precompute factorials up to \(n\) and modular inverses of all integers \(1\) to \(n\) using the linear inverse method: `inv[i] = MOD - MOD / i * inv[MOD % i] % MOD`.
#include <vector>
#include <cstdint>

// Count valid permutations of tree rooted at 1, modulo 998244353.
long long countValidPermutations(const std::vector<std::vector<int>>& adj) {
    const long long MOD = 998244353;
    int n = static_cast<int>(adj.size()); // adj is 0-indexed, n vertices

    // Factorials
    std::vector<long long> fact(n + 1, 1);
    for (int i = 1; i <= n; ++i) fact[i] = fact[i-1] * i % MOD;

    // Modular inverses of 1..n using linear formula
    std::vector<long long> inv(n + 1, 1);
    if (n >= 1) inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
        inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
    }

    // Subtree sizes
    std::vector<int> sz(n, 0);
    std::vector<int> parent(n, -1);
    // Iterative post-order DFS using stack
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack = {0}; // root is vertex 1 -> index 0
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : adj[u]) {
            if (v != parent[u]) {
                parent[v] = u;
                stack.push_back(v);
            }
        }
    }
    // Process in reverse order (post-order)
    for (int i = n-1; i >= 0; --i) {
        int u = order[i];
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != parent[u]) {
                sz[u] += sz[v];
            }
        }
    }

    // product of 1/sz[v]
    long long denom = 1;
    for (int i = 0; i < n; ++i) {
        denom = denom * inv[sz[i]] % MOD;
    }

    return fact[n] * denom % MOD;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (for brevity, assume it's above)

int main() {
    // n=1 single node
    std::vector<std::vector<int>> adj1 = {{}};
    assert(countValidPermutations(adj1) == 1);

    // n=2: edge 1-2
    std::vector<std::vector<int>> adj2 = {{1}, {0}};
    assert(countValidPermutations(adj2) == 1); // only [1,2]

    // n=3: star with root 1 connected to 2 and 3
    std::vector<std::vector<int>> adj3 = {{1,2}, {0}, {0}};
    // permutations: [1,2,3] and [1,3,2] => 2
    assert(countValidPermutations(adj3) == 2);

    // n=4: path 1-2-3-4
    std::vector<std::vector<int>> adj4 = {{1}, {0,2}, {1,3}, {2}};
    // Only [1,2,3,4] => 1
    assert(countValidPermutations(adj4) == 1);

    // n=4: root 1 with child 2, and 2 has child 3 and 4
    std::vector<std::vector<int>> adj5 = {{1}, {0,2,3}, {1}, {1}};
    // permutations: [1,2,3,4] and [1,2,4,3] => 2
    assert(countValidPermutations(adj5) == 2);

    // n=5: root 1 connected to 2,3; node 2 connected to 4,5
    std::vector<std::vector<int>> adj6 = {{1,2}, {0,3,4}, {0}, {1}, {1}};
    // Subtree sizes: root=5, node2=3, node3=1, node4=1, node5=1
    // formula: 120 / (5*3*1*1*1) = 8
    assert(countValidPermutations(adj6) == 8);

    // Large chain (n=200000) to ensure no crash (test with smaller for speed, but here test n=1000)
    int n = 1000;
    std::vector<std::vector<int>> adjLarge(n);
    for (int i = 0; i < n-1; ++i) {
        adjLarge[i].push_back(i+1);
        adjLarge[i+1].push_back(i);
    }
    // A chain has exactly 1 valid permutation
    assert(countValidPermutations(adjLarge) == 1);

    std::cout << "All tests passed!\n";
    return 0;
}
