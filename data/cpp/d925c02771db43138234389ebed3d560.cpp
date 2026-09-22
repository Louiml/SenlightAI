Given a vector of `Node` objects where each `Node` has an `int id` and a `vector<int> neighborIds`, write a C++ function `vector<vector<int>> groupConnectedComponents(const vector<Node>& nodes)` that returns the connected components of the undirected graph formed by these nodes, each component as a vector of node IDs, sorted by component size in descending order. Nodes without any neighbors form singleton components. IDs may be arbitrary (not necessarily contiguous), and the graph may contain self‑loops or duplicate neighbor entries, which must be handled without affecting connectivity. The function must not modify the input vector and must be implemented without external libraries (only standard C++).

// The problem reduces to finding connected components in an undirected graph. The main algorithm is depth‑first search (DFS) on an adjacency list. First, build a mapping from each node ID to its index in the input vector, and also create an adjacency list from the neighbor lists. Since the neighbor lists may contain duplicates and self‑loops, we can simply ignore duplicates during traversal (a visited set for nodes already processed handles this) and self‑loops are irrelevant because a node is already visited. Important edge cases: empty input (return empty vector), isolated nodes (each forms its own component), and multiple duplicate neighbor entries (should not create duplicate components or infinite loops). Time complexity is O(V + E) where V is the number of vertices and E is the total number of neighbor entries (since each edge is considered twice if symmetric, but we traverse each adjacency list once). Space complexity is O(V + E) for the adjacency list and visited array, plus O(V) for recursion stack in the worst case. Sorting components by size takes O(C log C) where C is the number of components, but C ≤ V, so overall it is dominated by O(V + E).

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

struct Node {
    int id;
    std::vector<int> neighborIds;
};

// Recursive DFS to collect all node IDs in one component.
void dfsComponent(int currentId,
                  const std::unordered_map<int, std::vector<int>>& adj,
                  std::unordered_set<int>& visited,
                  std::vector<int>& component) {
    visited.insert(currentId);
    component.push_back(currentId);
    const auto it = adj.find(currentId);
    if (it == adj.end()) return; // no neighbors
    for (int neighbor : it->second) {
        if (visited.find(neighbor) == visited.end()) {
            dfsComponent(neighbor, adj, visited, component);
        }
    }
}

// Return connected components as vectors of node IDs, sorted by size descending.
std::vector<std::vector<int>> groupConnectedComponents(const std::vector<Node>& nodes) {
    // Build adjacency list from neighborIds (duplicates and self-loops are fine).
    std::unordered_map<int, std::vector<int>> adj;
    for (const Node& node : nodes) {
        adj[node.id] = node.neighborIds; // copy; duplicates allowed
    }
    // Add all node IDs to adjacency map even if isolated (already done above).
    // Ensure every node has an entry even if no neighbors (already done).

    std::unordered_set<int> visited;
    std::vector<std::vector<int>> components;

    for (const Node& node : nodes) {
        if (visited.find(node.id) == visited.end()) {
            std::vector<int> comp;
            dfsComponent(node.id, adj, visited, comp);
            components.push_back(std::move(comp));
        }
    }

    // Sort components by size descending.
    std::sort(components.begin(), components.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a.size() > b.size();
              });
    return components;
}

#include <cassert>
#include <vector>

int main() {
    // Empty input
    {
        std::vector<Node> nodes;
        auto res = groupConnectedComponents(nodes);
        assert(res.empty());
    }

    // Isolated nodes
    {
        std::vector<Node> nodes = {{1, {}}, {2, {}}, {3, {}}};
        auto res = groupConnectedComponents(nodes);
        assert(res.size() == 3);
        // Each component size 1
        for (auto& comp : res) assert(comp.size() == 1);
    }

    // Single component with duplicates and self‑loop
    {
        std::vector<Node> nodes = {
            {1, {2, 2, 1}},   // self-loop + duplicate neighbor
            {2, {1, 3}},
            {3, {2, 3}}       // self-loop again
        };
        auto res = groupConnectedComponents(nodes);
        assert(res.size() == 1);
        assert(res[0].size() == 3);
        std::vector<int> expected = {1, 2, 3};
        std::sort(res[0].begin(), res[0].end());
        assert(res[0] == expected);
    }

    // Two components, larger first
    {
        std::vector<Node> nodes = {
            {10, {20, 30}},
            {20, {10}},
            {30, {10}},
            {40, {}},
            {50, {60}},
            {60, {50}}
        };
        auto res = groupConnectedComponents(nodes);
        assert(res.size() == 3);
        // First component should be {10,20,30} size 3
        assert(res[0].size() == 3);
        // Second should be size 2, third size 1
        assert(res[1].size() == 2);
        assert(res[2].size() == 1);
    }

    // Non‑contiguous IDs, chain
    {
        std::vector<Node> nodes = {
            {100, {200}},
            {200, {300}},
            {300, {}},
            {400, {}}
        };
        auto res = groupConnectedComponents(nodes);
        assert(res.size() == 2);
        assert(res[0].size() == 3);
        assert(res[1].size() == 1);
    }

    // Duplicate node IDs (should be treated as separate? Not specified, but we assume unique IDs)
    // The function assumes unique IDs per the task description.

    return 0;
}
