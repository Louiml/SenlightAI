// Write a C++ function that takes a single integer `n` (the number of nodes in a rooted tree, where node 1 is the root), followed by `n-1` integers `parent[i]` for nodes `2` through `n` (each parent is an integer from 1 to `n-1`). The function must determine whether every internal node (a node with at least one child) has at least 3 leaf children (children that have no children of their own). Return `true` if this condition holds for all internal nodes, and `false` otherwise. If the root has no children, the tree is considered valid (vacuously true). The function should read the input from standard input in the same format as the snippet: first `n`, then the `n-1` parent values.
The problem is a tree validation problem. We build an adjacency list of children for each node from the given parent-child pairs. Then we perform a BFS (or DFS) traversal starting from the root. For each visited node, we inspect its children list. If a child has no children (i.e., its adjacency list is empty), it is a leaf, and we count it. If a node has any children, the number of leaf children must be at least 3; otherwise the condition fails. Note: The original snippet initializes `unvisited` incorrectly (only sets index 1), but the algorithm still works because it marks children as visited when exploring. We do not need the visited array at all if we assume the tree is a valid tree (no cycles, each node except root appears exactly once as a child). However, to be safe, we can track visited nodes to avoid processing the same node twice; but since the input is guaranteed to be a tree, BFS from root will visit each node exactly once. Important edge cases: `n=1` (no edges, root has no children → valid). A node with exactly 1 or 2 leaf children → invalid. A node with non‑leaf children plus at least 3 leaf children → valid. Time complexity is O(n) because we traverse each node and edge once. Space complexity is O(n) for the adjacency list and queue.
#include <vector>
#include <queue>

// Determine if every internal node has at least 3 leaf children.
// Reads n, then n-1 parent values from stdin.
bool hasAtLeastThreeLeafChildren(std::istream& input) {
    int n;
    if (!(input >> n)) return false; // no input
    
    std::vector<std::vector<int>> children(n + 1);
    for (int node = 2; node <= n; ++node) {
        int parent;
        input >> parent;
        children[parent].push_back(node);
    }
    
    // BFS from root (node 1)
    std::queue<int> q;
    q.push(1);
    std::vector<bool> visited(n + 1, false);
    visited[1] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        int leafChildren = 0;
        for (int child : children[u]) {
            if (children[child].empty()) {
                ++leafChildren;
            } else {
                if (!visited[child]) {
                    visited[child] = true;
                    q.push(child);
                }
            }
        }
        
        // If this node has children, it must have at least 3 leaf children.
        if (!children[u].empty() && leafChildren < 3) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <sstream>

// Declaration from the solution
bool hasAtLeastThreeLeafChildren(std::istream& input);

int main() {
    // Case 1: n=1, root only -> valid
    {
        std::istringstream in("1\n");
        assert(hasAtLeastThreeLeafChildren(in) == true);
    }
    // Case 2: root with 3 leaf children -> valid
    {
        std::istringstream in("4\n1\n1\n1\n");
        assert(hasAtLeastThreeLeafChildren(in) == true);
    }
    // Case 3: root with 2 leaf children -> invalid
    {
        std::istringstream in("3\n1\n1\n");
        assert(hasAtLeastThreeLeafChildren(in) == false);
    }
    // Case 4: root has one internal child (node2) with 3 leaf children, root itself has 0 leaf children -> invalid
    {
        std::istringstream in("6\n1\n2\n2\n2\n2\n"); // children: 1->2, 2->3,4,5,6
        assert(hasAtLeastThreeLeafChildren(in) == false);
    }
    // Case 5: root has 3 leaf children and an internal child with 3 leaf children -> valid
    {
        std::istringstream in("10\n1\n1\n1\n1\n2\n2\n2\n2\n2\n"); // 1->2,3,4; 2->5,6,7,8,9,10? Actually let's craft properly
        // Let's do: n=8, parents: 1,1,1,2,2,2,2 -> 1 has children 2,3,4 (3 leaf), 2 has children 5,6,7,8 (4 leaf) -> valid
        std::istringstream in2("8\n1\n1\n1\n2\n2\n2\n2\n");
        assert(hasAtLeastThreeLeafChildren(in2) == true);
    }
    // Case 6: root with 3 leaf children, one internal child with only 2 leaf children -> invalid
    {
        std::istringstream in("10\n1\n1\n1\n2\n2\n3\n3\n3\n3\n"); // 1->2,3,4; 2->5,6; 3->7,8,9,10 (3 leaf) -> node2 has only 2 leaf children -> invalid
        assert(hasAtLeastThreeLeafChildren(in) == false);
    }
    // Case 7: linear chain (1-2-3-4) -> every internal node has 1 leaf child (the next) -> invalid
    {
        std::istringstream in("4\n1\n2\n3\n");
        assert(hasAtLeastThreeLeafChildren(in) == false);
    }
    // Case 8: root with 4 leaf children -> valid
    {
        std::istringstream in("5\n1\n1\n1\n1\n");
        assert(hasAtLeastThreeLeafChildren(in) == true);
    }
    return 0;
}
