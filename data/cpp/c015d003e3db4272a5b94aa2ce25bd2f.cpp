Write a C++ function `buildTreeInfo(int n, const std::vector<std::vector<int>>& adjacency)` that, given the number of nodes `n` (0 to n-1) and an adjacency list where `adjacency[i]` contains the child node IDs of node `i` in a rooted tree (each node appears at most once as a child across the entire input), returns a `std::vector<std::string>` of `n` lines, one per node in order of node ID. Each line must have the exact format: `"node <i>: parent = <p>, depth = <d>, <type>, [<c1, c2, ...>]"` where `<p>` is the parent ID (or `-1` if root), `<d>` is the depth (root depth 0, children depth 1 more), `<type>` is either `root`, `internal node`, or `leaf`, and the brackets contain the child IDs separated by `", "` (an empty list prints as `[]`). The input tree is guaranteed to have exactly one root, and parent relationships are automatically derived from the given child lists (i.e., a node’s parent is the node that lists it as a child; if no node lists it, it is the root). The function must not modify the input and must handle up to `n=1000`. Provide the function only, no main.
// The problem is a straightforward tree traversal. First, initialize a structure for each node with `parent = -1`, `depth = -2` (sentinel), and `type = internal node` (but will be overridden). Then iterate over each node’s child list: for each child ID, set that child’s parent to the current node. Also, if a node has zero children, mark it as `leaf`; otherwise keep `internal node`. After processing all lists, find the unique node whose parent remains `-1` — that is the root. Set its depth to 0 and type to `root`, then perform a DFS from the root: for each child, set its depth to current depth + 1, and recurse. After DFS, all nodes have correct depth and type. Finally, build output strings by iterating nodes 0..n-1, formatting each with parent, depth, type string, and child list. Edge cases: single node tree (root and leaf at same time? The snippet treats a root with no children as type `root` but also leaf? In the snippet, type is set to `leaf` if no children before root override, but then root override sets it to `root` only for the root. So a single-node root becomes type `root`, not leaf — this matches typical definitions where root with no children is both root and leaf but the problem specifies exactly root for the root node. We follow that). Time complexity O(n + total children) = O(n) because each node appears exactly once as a child in the whole input (guaranteed tree). Space O(n) for storage.
#include <vector>
#include <string>
#include <sstream>

/**
 * Build formatted tree information for each node.
 * @param n number of nodes (0..n-1)
 * @param adjacency children list per node
 * @return vector of strings formatted as specified, one per node in order
 */
std::vector<std::string> buildTreeInfo(int n, const std::vector<std::vector<int>>& adjacency) {
    // Structure to hold tree metadata
    struct NodeInfo {
        int parent = -1;
        int depth = -2;
        int type = 1; // 0 root, 1 internal, 2 leaf
    };
    std::vector<NodeInfo> info(n);
    
    // Assign parents and determine leaf status based on child counts
    for (int i = 0; i < n; ++i) {
        if (adjacency[i].empty()) {
            info[i].type = 2; // leaf
        } else {
            for (int child : adjacency[i]) {
                info[child].parent = i;
            }
        }
    }
    
    // Find root (node with parent == -1)
    int root = -1;
    for (int i = 0; i < n; ++i) {
        if (info[i].parent == -1) {
            root = i;
            break;
        }
    }
    
    // DFS to set depths
    std::function<void(int, int)> dfs = [&](int idx, int depth) {
        info[idx].depth = depth;
        for (int child : adjacency[idx]) {
            dfs(child, depth + 1);
        }
    };
    if (root != -1) {
        info[root].type = 0; // root
        info[root].depth = 0;
        dfs(root, 0); // set depths for all nodes (root depth 0, children depth+1)
    }
    
    // Build output strings
    const char* typeNames[3] = {"root", "internal node", "leaf"};
    std::vector<std::string> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "node " << i << ": parent = " << info[i].parent
            << ", depth = " << info[i].depth << ", " << typeNames[info[i].type]
            << ", [";
        for (size_t j = 0; j < adjacency[i].size(); ++j) {
            if (j > 0) oss << ", ";
            oss << adjacency[i][j];
        }
        oss << "]";
        result.push_back(oss.str());
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// Include or paste the solution function here

int main() {
    // Test 1: Simple tree with root 0, children 1,2, node 3 child of 1
    std::vector<std::vector<int>> adj1(4);
    adj1[0] = {1, 2};
    adj1[1] = {3};
    adj1[2] = {};
    adj1[3] = {};
    auto res1 = buildTreeInfo(4, adj1);
    assert(res1.size() == 4);
    assert(res1[0] == "node 0: parent = -1, depth = 0, root, [1, 2]");
    assert(res1[1] == "node 1: parent = 0, depth = 1, internal node, [3]");
    assert(res1[2] == "node 2: parent = 0, depth = 1, leaf, []");
    assert(res1[3] == "node 3: parent = 1, depth = 2, leaf, []");

    // Test 2: Single node tree
    std::vector<std::vector<int>> adj2(1);
    adj2[0] = {};
    auto res2 = buildTreeInfo(1, adj2);
    assert(res2.size() == 1);
    assert(res2[0] == "node 0: parent = -1, depth = 0, root, []");

    // Test 3: Chain 0->1->2
    std::vector<std::vector<int>> adj3(3);
    adj3[0] = {1};
    adj3[1] = {2};
    adj3[2] = {};
    auto res3 = buildTreeInfo(3, adj3);
    assert(res3[0] == "node 0: parent = -1, depth = 0, root, [1]");
    assert(res3[1] == "node 1: parent = 0, depth = 1, internal node, [2]");
    assert(res3[2] == "node 2: parent = 1, depth = 2, leaf, []");

    // Test 4: Root with many leaves
    std::vector<std::vector<int>> adj4(5);
    adj4[0] = {1,2,3,4};
    adj4[1] = adj4[2] = adj4[3] = adj4[4] = {};
    auto res4 = buildTreeInfo(5, adj4);
    assert(res4[0] == "node 0: parent = -1, depth = 0, root, [1, 2, 3, 4]");
    assert(res4[1] == "node 1: parent = 0, depth = 1, leaf, []");
    assert(res4[4] == "node 4: parent = 0, depth = 1, leaf, []");

    // Test 5: Empty tree? Not applicable per spec (n>=1). 
    // But test small n=0? Not required.

    std::cout << "All tests passed." << std::endl;
    return 0;
}
