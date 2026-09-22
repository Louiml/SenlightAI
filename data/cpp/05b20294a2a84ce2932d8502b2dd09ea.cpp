// Given a rooted tree with `n` nodes (numbered 1 through n, root = 1) and a list of parent pointers (for each node i from 2 to n, its parent is given), write a C++ function `int countOddDepthNodes(const std::vector<int>& parents)` that returns the number of nodes whose depth (distance from root, root depth = 0) has an odd count of nodes at that depth. In other words, for each depth `d`, count how many nodes are at that depth; if that count is odd, add 1 to the answer. The function takes the parent list as a vector where `parents[i-2]` is the parent of node `i` (for i from 2 to n). The root is always node 1, and parent indices are valid (1-based). The tree can be any shape (not necessarily binary). Return the total count of depths with an odd number of nodes.
// We first build an adjacency list from the parent pointers: for each node `v` (from 2 to n), we add `v` to the children list of its parent. Then we perform a depth-first search (DFS) from root 1, tracking depth. We maintain an array `depthCount` of size `n+1` (or use a vector) where `depthCount[d]` increments each time we visit a node at depth `d`. After DFS completes, we iterate over all depths 0..n and count how many have an odd value. The answer is that count. Edge cases: single-node tree (n=1) has depth 0 with one node, count is 1. Deep chains or wide trees work fine. Time complexity is O(n) for building the tree and DFS, plus O(n) for the final scan, so O(n) total. Space complexity is O(n) for adjacency list and depth count array (plus recursion stack up to O(n) in worst-case chain).
#include <vector>
#include <functional>

// Counts the number of depths that contain an odd number of nodes.
// parents[i-2] is the parent of node i (for i=2..n). Root is 1.
int countOddDepthNodes(const std::vector<int>& parents) {
    int n = parents.size() + 1; // total nodes
    std::vector<std::vector<int>> children(n + 1);
    for (int node = 2; node <= n; ++node) {
        int parent_node = parents[node - 2];
        children[parent_node].push_back(node);
    }

    std::vector<int> depthCount(n + 1, 0);

    // DFS to compute depth counts
    std::function<void(int, int)> dfs = [&](int node, int depth) {
        depthCount[depth]++;
        for (int child : children[node]) {
            dfs(child, depth + 1);
        }
    };
    dfs(1, 0);

    int answer = 0;
    for (int d = 0; d <= n; ++d) {
        if (depthCount[d] % 2 == 1) {
            answer++;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

// Function under test
int countOddDepthNodes(const std::vector<int>& parents);

int main() {
    // Single node tree: depth 0 has 1 node -> odd -> answer 1
    assert(countOddDepthNodes({}) == 1);

    // Simple chain: 1-2-3: depths: 0 has 1, 1 has 1, 2 has 1 -> all odd -> answer 3
    assert(countOddDepthNodes({1, 2}) == 3);

    // Star: root 1 with children 2,3,4: depth 0 has 1, depth 1 has 3 -> both odd -> 2
    assert(countOddDepthNodes({1, 1, 1}) == 2);

    // Balanced: 1 children 2,3; 2 children 4,5; 3 children 6,7: depth0:1, depth1:2, depth2:4 -> only depth0 odd -> 1
    assert(countOddDepthNodes({1, 1, 2, 2, 3, 3}) == 1);

    // Root + two children, one child has two children: depths: 0:1, 1:2, 2:2 -> only depth0 odd -> 1
    assert(countOddDepthNodes({1, 1, 2, 2}) == 1);

    // Chain of 4: 1-2-3-4: depths 0..3 each have 1 -> all odd -> 4
    assert(countOddDepthNodes({1, 2, 3}) == 4);

    // Tree with levels: 1 has 2,3; 2 has 4; 3 has 5,6: depth0:1, depth1:2, depth2:3 -> odd at depth0 and depth2 -> 2
    assert(countOddDepthNodes({1, 1, 2, 3, 3}) == 2);

    // Deep chain with a side leaf: 1-2-3-4, and 2 also has 5 : depths: 0:1, 1:1, 2:2, 3:1 -> odd at 0,1,3 -> 3
    assert(countOddDepthNodes({1, 2, 3, 2}) == 3);

    // Large star with 1000 children of root: depth0:1, depth1:1000 (even) -> answer 1
    std::vector<int> largeStar(999, 1);
    assert(countOddDepthNodes(largeStar) == 1);

    // Root with one child that has one child... n=5 chain: depths 0-4 all 1 -> all odd -> 5
    std::vector<int> chainParents = {1, 2, 3, 4};
    assert(countOddDepthNodes(chainParents) == 5);

    return 0;
}
