/*
Given a rooted tree with `n` nodes (0-indexed, root at node 0), where each node has an integer value between 1 and 100000 inclusive, and a parent list (for each node `i` from 1 to n-1, its parent is given) so that the tree is directed from parent to child, write a C++ function `long long countGoodPairs(const std::vector<int>& values, const std::vector<int>& parents)` that returns the number of ordered pairs `(u, v)` such that `u` is a proper ancestor of `v` and `values[u]` divides `values[v]`. The function must run efficiently for `n` up to 100000. Note that a node is considered its own ancestor? No—only proper ancestors (strictly above in the tree). The root has no ancestors. The values are 1-indexed in range, but node IDs are 0-indexed. The input is a non-empty tree.
*/
#include <vector>
#include <array>
#include <numeric>

// Precompute divisor lists for all values up to 100000.
static const int MAX_VAL = 100000;
static std::vector<std::vector<int>> divisorCache;

void buildDivisors() {
    if (!divisorCache.empty()) return;
    divisorCache.resize(MAX_VAL + 1);
    for (int d = 1; d <= MAX_VAL; ++d) {
        for (int multiple = d; multiple <= MAX_VAL; multiple += d) {
            divisorCache[multiple].push_back(d);
        }
    }
}

long long countGoodPairs(const std::vector<int>& values, const std::vector<int>& parents) {
    const int n = static_cast<int>(values.size());
    if (n <= 1) return 0;

    // Build adjacency list of children.
    std::vector<std::vector<int>> children(n);
    for (int i = 1; i < n; ++i) {
        children[parents[i - 1]].push_back(i);
    }

    buildDivisors();

    long long answer = 0;
    std::vector<int> ancestorCount(MAX_VAL + 1, 0);

    // Recursive lambda for DFS.
    std::function<void(int)> dfs = [&](int node) {
        const int x = values[node];
        // Count how many ancestors have a value dividing x.
        for (int d : divisorCache[x]) {
            answer += ancestorCount[d];
        }
        // Add current node to ancestor count for descendants.
        ++ancestorCount[x];
        // Recurse into children.
        for (int child : children[node]) {
            dfs(child);
        }
        // Remove current node when backtracking.
        --ancestorCount[x];
    };

    // Root is node 0.
    dfs(0);
    return answer;
}
#include <cassert>
#include <vector>
#include <functional>

// Assume the solution function countGoodPairs is defined above.

int main() {
    // Test 1: single node.
    std::vector<int> v1 = {5};
    std::vector<int> p1 = {};
    assert(countGoodPairs(v1, p1) == 0);

    // Test 2: chain 1 -> 2 -> 3, values 2,4,8. Pairs: (0,1):2|4, (0,2):2|8, (1,2):4|8 => 3.
    std::vector<int> v2 = {2,4,8};
    std::vector<int> p2 = {0,1};
    assert(countGoodPairs(v2, p2) == 3);

    // Test 3: tree with root 6, children 3 and 12, and 12 has child 4.
    // Edges: 0->1 (val 6->3), 0->2 (6->12), 2->3 (12->4).
    // Pairs: (0,1): 6|3? no. (0,2):6|12 yes. (0,3):6|4? no. (2,3):12|4? no. => 1.
    std::vector<int> v3 = {6,3,12,4};
    std::vector<int> p3 = {0,0,2};
    assert(countGoodPairs(v3, p3) == 1);

    // Test 4: same values, all divide each other.
    // Chain: 0->1->2, values all 10. Pairs: (0,1), (0,2), (1,2) = 3.
    std::vector<int> v4 = {10,10,10};
    std::vector<int> p4 = {0,1};
    assert(countGoodPairs(v4, p4) == 3);

    // Test 5: values 1 and 2, root 1 child 2. 1 divides 2 => 1 pair.
    std::vector<int> v5 = {1,2};
    std::vector<int> p5 = {0};
    assert(countGoodPairs(v5, p5) == 1);

    // Test 6: values 2 and 3, root 2 child 3. 2 does not divide 3 => 0.
    std::vector<int> v6 = {2,3};
    std::vector<int> p6 = {0};
    assert(countGoodPairs(v6, p6) == 0);

    // Test 7: larger tree: root 2, children 1 and 4, child 4 has child 2.
    // Values: [2,1,4,2]. Edges: 0->1,0->2,2->3.
    // Pairs: (0,1):2|1? no. (0,2):2|4 yes. (0,3):2|2 yes. (2,3):4|2? no. => 2.
    std::vector<int> v7 = {2,1,4,2};
    std::vector<int> p7 = {0,0,2};
    assert(countGoodPairs(v7, p7) == 2);

    // Test 8: All nodes are leaves under root, root value 1, leaves values 1,2,3.
    // Pairs: (0,1):1|1 yes, (0,2):1|2 yes, (0,3):1|3 yes => 3.
    std::vector<int> v8 = {1,1,2,3};
    std::vector<int> p8 = {0,0,0};
    assert(countGoodPairs(v8, p8) == 3);

    return 0;
}
// We need to count for every pair of ancestor-descendant `(u, v)` (with `u` strictly above `v`), whether `values[u]` divides `values[v]`. Direct checking for all pairs would be O(n^2) and too slow. Instead, we do a depth-first search. For each node `cur`, we want to count how many of its ancestors have values that divide `values[cur]`. We maintain two global frequency arrays: `cnt1[d]` = number of ancestors of the current node (including the current node when we add it) whose value is exactly `d`? Actually the provided snippet uses `cnt1` for current path node count by value, and `cnt2` for divisors that have been added. Let's design a cleaner method.
//
// We'll maintain a global array `ancestorCount[value]` that counts how many nodes on the current path (from root to current node, excluding the current node itself) have exactly that value. During DFS, when we visit a node `u` with value `x`, we want to compute the contribution: for every divisor `d` of `x`, we add to the answer `ancestorCount[d]` because any ancestor with value `d` divides `x`. But careful: if `d == x` then `ancestorCount[x]` includes ancestors with value `x`, fine. So for each node, contribution = sum over all divisors `d` of `x` of `ancestorCount[d]`. Then we increment `ancestorCount[x]` by 1 (to count this node for its descendants) and recurse into children. After recursion, decrement `ancestorCount[x]` to restore state.
//
// Precompute for each possible value up to 100000 its list of divisors. Since values are ≤ 100000, total number of divisors across all values is O(V log V) where V=100000 (harmonic series). For each node, iterating over its divisors is O(number of divisors) which is small (max ~128 for numbers under 100000).
//
// Edge cases: n=1 (no pairs, returns 0), values repeated, root has no ancestors. Since the tree is given by parent list indicating for each node i from 1 to n-1 the parent, we build adjacency list. We need the answer to be of type `long long` because number of pairs can be up to n*(n-1)/2 ≈ 5e9, which fits in 64-bit.
//
// Time complexity: O(n * D) where D is the maximum number of divisors per value (small constant ~128), plus O(V log V) for precomputing divisors. Space: O(n + V) for tree and divisor lists.
