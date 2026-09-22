// Write a C++ function `vector<int> dfsOrder(int n, const vector<pair<int, int>>& edges, int start)` that accepts the number of nodes `n` (nodes are labeled 1 through `n`), an undirected graph represented as a list of edges, and a starting node. The function must perform an iterative depth-first search (DFS) traversal starting from the given node and return a `vector<int>` containing the nodes in the exact order they are first visited, as they would be printed by a stack-based DFS that pushes all unvisited neighbors of the current node onto the stack in the order they appear in the adjacency list, then pops them one by one. Nodes that are unreachable from the start must not appear in the result. Assume `n >= 1`, the input graph is undirected, and the starting node is valid (1 ≤ start ≤ n). The function must not modify the input edges.

The solution builds an adjacency list from the provided edge list, where each edge adds both directions because the graph is undirected. The DFS is implemented iteratively using a `std::stack<int>`. We initialize a boolean `visited` array of size `n+1` (1-indexed) and push the start node. The main loop pops the top node; if it has not been visited, we mark it visited and append it to the result vector. Then, we iterate over all its neighbors in the order they appear in the adjacency list and push any unvisited neighbor onto the stack. Because we only push neighbors that are not yet visited (at the time of the check), duplicate pushes can occur (a node may be pushed multiple times if it is a neighbor of several visited nodes before it is popped), but the `visited` flag ensures it is output only once. The order reflects stack LIFO behavior: the last neighbor pushed is popped first. Edge cases include: `n=1` with no edges (returns just `{start}`); an empty edge list (returns only the starting node); a disconnected graph (returns only the reachable component); and self-loops or multiple edges (handled naturally since visited checks prevent repeats). Time complexity is O(n + m) where `m` is the number of edges, because each node is pushed and popped at least once and each adjacency list entry is examined once. Space complexity is O(n + m) for the adjacency list, visited array, and stack.

#include <vector>
#include <stack>

// Perform iterative DFS on an undirected graph starting from `start`.
// Returns the nodes in the order they are first visited.
// `n` is the number of nodes (labels 1..n), `edges` is the edge list.
// The graph is treated as undirected. Nodes not reachable are omitted.
std::vector<int> dfsOrder(int n, const std::vector<std::pair<int, int>>& edges, int start) {
    // Build adjacency list (1-indexed)
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& edge : edges) {
        int u = edge.first;
        int v = edge.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<bool> visited(n + 1, false);
    std::vector<int> order;
    std::stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (!visited[current]) {
            visited[current] = true;
            order.push_back(current);
        }

        // Push all unvisited neighbors; the last pushed will be popped first.
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                s.push(neighbor);
            }
        }
    }

    return order;
}

#include <cassert>
#include <vector>

// Include the solution function here or link it.

int main() {
    // Test 1: Simple chain 1-2-3 starting from 1
    std::vector<std::pair<int, int>> edges1 = {{1,2}, {2,3}};
    assert(dfsOrder(3, edges1, 1) == std::vector<int>({1, 3, 2}));
    // Explanation: adjacency[1]=[2], push 2 -> pop 2, adjacency[2]=[1,3], 1 visited, push 3 -> pop 3 -> output 1,3,2

    // Test 2: Star graph center 1 connected to 2,3,4 starting from 1
    std::vector<std::pair<int, int>> edges2 = {{1,2}, {1,3}, {1,4}};
    assert(dfsOrder(4, edges2, 1) == std::vector<int>({1, 4, 3, 2}));
    // Neighbors pushed in order 2,3,4; last pushed (4) popped first.

    // Test 3: Single node with no edges
    std::vector<std::pair<int, int>> edges3;
    assert(dfsOrder(1, edges3, 1) == std::vector<int>({1}));

    // Test 4: Disconnected graph, starting from node in smaller component
    std::vector<std::pair<int, int>> edges4 = {{1,2}, {3,4}};
    assert(dfsOrder(4, edges4, 1) == std::vector<int>({1, 2}));

    // Test 5: Self-loop and duplicate edges
    std::vector<std::pair<int, int>> edges5 = {{1,1}, {1,2}, {2,1}, {2,3}};
    // Adjacency[1]=[1,2], first pop 1 -> output 1, push 2 (1 visited)
    // pop 2 -> output 2, push 3 (1 visited) -> pop 3 -> output 3
    assert(dfsOrder(3, edges5, 1) == std::vector<int>({1, 2, 3}));

    // Test 6: Start from a node with no edges, but others exist
    std::vector<std::pair<int, int>> edges6 = {{2,3}};
    assert(dfsOrder(3, edges6, 1) == std::vector<int>({1}));

    // Test 7: Larger tree where branching order matters
    // Edges: 1-2, 1-3, 2-4, 3-5
    std::vector<std::pair<int, int>> edges7 = {{1,2}, {1,3}, {2,4}, {3,5}};
    // adj[1]=[2,3] -> push 2,3 ; pop 3 -> output 3, push 5 ; pop 5 -> output 5 ; pop 2 -> output 2 push 4 ; pop 4 -> output 4
    // Result: 1,3,5,2,4
    assert(dfsOrder(5, edges7, 1) == std::vector<int>({1, 3, 5, 2, 4}));

    // Test 8: Cycle with four nodes, start at 2
    std::vector<std::pair<int, int>> edges8 = {{1,2}, {2,3}, {3,4}, {4,1}};
    // adj[2]=[1,3] -> push 1,3 -> pop 3 -> output 3 push 4 (1 not visited yet) but visited[3] now true; push 2? Wait: adj[3]=[2,4], 2 visited? No, 2 is current, but not marked yet? Actually 2 is popped and output, then marked visited, so 2 is visited. So push 4 only.
    // Let's trace: pop 2 (output 2), push 1,3. pop 3 (output 3), push 4 (since 2 visited). pop 4 (output 4), push 1 (since 3 visited). pop 1 (output 1). Result: 2,3,4,1
    assert(dfsOrder(4, edges8, 2) == std::vector<int>({2, 3, 4, 1}));

    return 0;
}
