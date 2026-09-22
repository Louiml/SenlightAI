// Write a C++ function `bool isFibonacciTree(const std::vector<std::vector<int>>& adjacency, int root)` that determines whether an undirected tree with `n` vertices can be recursively decomposed into Fibonacci-number-sized subtrees. The input is an adjacency list (1-indexed vertices), and the tree is rooted at the given `root`. The function returns `true` if and every connected component at each recursion step has a size that is a Fibonacci number (from the sequence 1, 1, 2, 3, 5, ...) and can be split into exactly two subtrees whose sizes are also Fibonacci numbers, repeating until all components have size 1, 2, or 3. The decomposition is valid only if, at each step, there exists at least one edge whose removal splits the current component into two Fibonacci-sized parts. The function should be deterministic (no randomness) and must handle up to 200,000 vertices efficiently. Note that the tree may be disconnected in the input, but the function only needs to consider the component containing `root`. The function should assume the input is a valid tree (no cycles, correct adjacency symmetry).
The solution is a recursive divide-and-conquer algorithm. First, precompute a boolean lookup table `isFibSize` for all possible sizes up to `n` using the Fibonacci sequence. Then define a recursive function `dfs(u, parent)` that computes the subtree size of `u` and returns whether that subtree can be decomposed. The key is to find an edge (u, v) within the subtree such that removing it splits into two parts whose sizes are both Fibonacci numbers. To find such an edge efficiently, for each node `w` in the subtree (excluding the subtree root `u`), we check if `subtreeSize(w)` is Fibonacci and `currentComponentSize - subtreeSize(w)` is also Fibonacci. If found, we recursively check both resulting subtrees. To avoid recursion depth issues, we use an iterative stack-based approach or careful recursion with an explicit stack, but for typical limits a recursive DFS with depth up to `n` may hit stack overflow, so we should use an iterative DFS or increase stack size (but for a standalone function we can implement iterative). For simplicity in the solution, we use recursion but note that the tree is likely balanced in practice; however, to be safe, we implement an iterative post-order traversal to compute sizes and then an iterative decomposition with a stack of (componentRoot, componentSize) pairs.

Important edge cases: 
- If `n` is not a Fibonacci number, return false immediately.
- If `n <= 3`, return true (base case).
- For components of size > 3, there must be at least one valid split; if none, return false.
- Disconnected input: we only process the component containing `root`.
- The recursion must avoid revisiting parent nodes.

Time complexity: Each edge is considered at most once per decomposition level, and each level splits the component into smaller ones. In the worst case, we might scan all nodes in a component for each split, leading to O(n log n) or O(n^2) if naive. But we can optimize by storing subtree sizes and checking each node once per component; the total work across all recursion levels is O(n log n) due to halving sizes in the best case, but can be O(n^2) for unbalanced splits. For the problem constraints, we can assume the Fibonacci property ensures splits are roughly logarithmic, so O(n log n) average. Space complexity is O(n) for adjacency, sizes, and stack.
#include <vector>
#include <algorithm>
#include <unordered_map>

// Precompute Fibonacci sizes up to a given maximum
static std::vector<bool> fibonacciSizes(int maxN) {
    std::vector<int> fib = {1, 1};
    while (fib.back() < maxN) {
        fib.push_back(fib.back() + fib[fib.size() - 2]);
    }
    std::vector<bool> isFib(maxN + 1, false);
    for (int size : fib) {
        if (size <= maxN) isFib[size] = true;
    }
    return isFib;
}

// Check if a tree component can be decomposed into Fibonacci-sized subtrees
bool isFibonacciTree(const std::vector<std::vector<int>>& adjacency, int root) {
    int n = (int)adjacency.size() - 1; // assuming 1-indexed
    if (root < 1 || root > n) return false;
    
    // Build the component rooted at 'root' using iterative DFS to find all vertices in that component
    std::vector<int> parent(n + 1, 0);
    std::vector<int> order;
    std::vector<int> stack = {root};
    parent[root] = -1;
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : adjacency[u]) {
            if (v == parent[u]) continue;
            if (parent[v] == 0) { // not visited
                parent[v] = u;
                stack.push_back(v);
            }
        }
    }
    
    int compSize = (int)order.size();
    if (compSize <= 0) return false;
    
    auto isFib = fibonacciSizes(compSize);
    if (!isFib[compSize]) return false;
    if (compSize <= 3) return true;
    
    // Compute subtree sizes using post-order on the component order
    std::vector<int> subSize(n + 1, 0);
    for (int i = (int)order.size() - 1; i >= 0; --i) {
        int u = order[i];
        subSize[u] = 1;
        for (int v : adjacency[u]) {
            if (parent[v] == u) {
                subSize[u] += subSize[v];
            }
        }
    }
    
    // Stack of components to decompose: each is (root, size)
    std::vector<std::pair<int,int>> compStack;
    compStack.push_back({root, compSize});
    
    while (!compStack.empty()) {
        auto [compRoot, compSizeNow] = compStack.back();
        compStack.pop_back();
        
        if (compSizeNow <= 3) continue;
        if (!isFib[compSizeNow]) return false;
        
        // Find an edge that splits the component into two Fibonacci-sized parts
        bool foundSplit = false;
        for (int v : adjacency[compRoot]) {
            if (parent[v] == compRoot) {
                int sizeV = subSize[v];
                if (isFib[sizeV] && isFib[compSizeNow - sizeV]) {
                    compStack.push_back({v, sizeV});
                    compStack.push_back({compRoot, compSizeNow - sizeV});
                    foundSplit = true;
                    break;
                }
            }
        }
        
        // If not found from the root, search deeper nodes in the component
        if (!foundSplit) {
            for (int u : order) {
                if (u == compRoot) continue;
                for (int v : adjacency[u]) {
                    if (parent[v] == u) {
                        int sizeV = subSize[v];
                        if (isFib[sizeV] && isFib[compSizeNow - sizeV]) {
                            compStack.push_back({v, sizeV});
                            compStack.push_back({compRoot, compSizeNow - sizeV});
                            foundSplit = true;
                            break;
                        }
                    }
                }
                if (foundSplit) break;
            }
        }
        
        if (!foundSplit) return false;
    }
    
    return true;
}
#include <cassert>
#include <vector>

// The function is declared above; include its definition or link appropriately.

int main() {
    // Test 1: Simple tree of 5 nodes (fibonacci number)
    // Star: 1 connected to 2,3,4,5; sizes: leaf size 1, root subtree size 5.
    // Splitting? Removing edge (1,2) gives 1 and 4 (4 not fib). No valid split -> false
    std::vector<std::vector<int>> adj1(6);
    adj1[1] = {2,3,4,5};
    adj1[2] = {1}; adj1[3] = {1}; adj1[4] = {1}; adj1[5] = {1};
    assert(isFibonacciTree(adj1, 1) == false);

    // Test 2: Path of 3 nodes (fib) -> true
    std::vector<std::vector<int>> adj2(4);
    adj2[1] = {2}; adj2[2] = {1,3}; adj2[3] = {2};
    assert(isFibonacciTree(adj2, 1) == true);

    // Test 3: Two separate components, root is in a 1-node component -> true
    std::vector<std::vector<int>> adj3(3);
    adj3[1] = {}; adj3[2] = {};
    assert(isFibonacciTree(adj3, 1) == true);

    // Test 4: A 13-node tree that is a complete binary tree? Not necessarily fib-decomposable
    // We'll construct a 13-node path: 1-2-...-13. Splitting? At each step, we need a Fibonacci split.
    // Path of 13: can split at edge (1,2) giving 1 and 12 (12 not fib) -> false
    std::vector<std::vector<int>> adj4(14);
    for (int i = 1; i <= 13; ++i) {
        if (i < 13) adj4[i].push_back(i+1);
        if (i > 1) adj4[i].push_back(i-1);
    }
    assert(isFibonacciTree(adj4, 1) == false);

    // Test 5: A tree that is a chain of two 5-node Fibonacci trees connected by an edge -> 10 nodes, not fib -> false
    // But if total is 10 (not a Fibonacci number), the initial check fails.
    std::vector<std::vector<int>> adj5(11);
    for (int i = 1; i <= 5; ++i) {
        if (i < 5) adj5[i].push_back(i+1);
        if (i > 1) adj5[i].push_back(i-1);
    }
    for (int i = 6; i <= 10; ++i) {
        if (i < 10) adj5[i].push_back(i+1);
        if (i > 6) adj5[i].push_back(i-1);
    }
    adj5[5].push_back(6); adj5[6].push_back(5);
    assert(isFibonacciTree(adj5, 1) == false);

    // Test 6: A 8-node chain (8 is fib) that can be split properly?
    // Path of 8: possible split at edge (3,4) giving 3 and 5 (both fib). Then 3 can stop, 5-node chain can split into 2 and 3.
    std::vector<std::vector<int>> adj6(9);
    for (int i = 1; i <= 8; ++i) {
        if (i < 8) adj6[i].push_back(i+1);
        if (i > 1) adj6[i].push_back(i-1);
    }
    assert(isFibonacciTree(adj6, 1) == true);

    // Test 7: Disconnected, root not in main component -> only component of root
    std::vector<std::vector<int>> adj7(10);
    adj7[1] = {2}; adj7[2] = {1}; // component size 2 (fib)
    // other component of 8 nodes (not fib) but we ignore it
    assert(isFibonacciTree(adj7, 1) == true);

    // Test 8: Invalid root
    std::vector<std::vector<int>> adj8(3);
    adj8[1] = {2}; adj8[2] = {1};
    assert(isFibonacciTree(adj8, 5) == false);

    return 0;
}
