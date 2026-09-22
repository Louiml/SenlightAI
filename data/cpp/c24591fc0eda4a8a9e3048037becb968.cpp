Write a C++ function `vector<int> assignComponents(int n, const vector<pair<int,int>>& edges)` that takes the number of vertices `n` (vertices are numbered from 1 to `n`) and a list of undirected edges, and returns a vector `component` of size `n+1` (index 1..n) where `component[v]` is the identifier of the connected component containing vertex `v`. Components must be numbered consecutively starting from 1, in the order they are first discovered when iterating vertices from 1 to `n` and performing depth-first search. The graph may be disconnected, may contain multiple edges between the same pair of vertices, and may contain self-loops.

// The problem reduces to finding connected components in an undirected graph. The standard approach is to iterate through all vertices from 1 to `n`, and whenever a vertex has not been assigned a component yet, start a DFS from that vertex, labeling all reachable unvisited vertices with a new component number. This ensures components are discovered in increasing order of the smallest vertex in each component. Edge cases: self-loops are harmless (DFS revisits the same vertex but the visited check prevents re-labeling), multiple edges cause redundant neighbor checks but not incorrectness. Time complexity is O(n + m) because each vertex is visited once and each edge is examined twice during DFS (once from each endpoint). Space complexity is O(n + m) for the adjacency list and the component vector.

#include <vector>
#include <utility>

// Assign a component identifier to each vertex (1..n) using DFS.
// Returns a vector of size n+1 where index 0 is unused.
std::vector<int> assignComponents(int n, const std::vector<std::pair<int,int>>& edges) {
    // Build adjacency list
    std::vector<std::vector<int>> adj(n + 1);
    for (const auto& e : edges) {
        int u = e.first;
        int v = e.second;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    std::vector<int> component(n + 1, 0);
    int currentComponent = 0;

    // DFS helper lambda
    auto dfs = [&](auto&& self, int v, int compId) -> void {
        component[v] = compId;
        for (int neighbor : adj[v]) {
            if (component[neighbor] == 0) {
                self(self, neighbor, compId);
            }
        }
    };

    for (int v = 1; v <= n; ++v) {
        if (component[v] == 0) {
            ++currentComponent;
            dfs(dfs, v, currentComponent);
        }
    }

    return component;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be included above.
// (In a real test, include the solution code here.)

int main() {
    // Empty graph, 1 vertex
    {
        std::vector<int> comp = assignComponents(1, {});
        assert(comp.size() == 2);
        assert(comp[1] == 1);
    }

    // Single edge
    {
        std::vector<int> comp = assignComponents(3, {{1,2}});
        assert(comp[1] == 1);
        assert(comp[2] == 1);
        assert(comp[3] == 2);
    }

    // Disconnected with several components
    {
        std::vector<int> comp = assignComponents(5, {{1,2},{3,4}});
        assert(comp[1] == 1);
        assert(comp[2] == 1);
        assert(comp[3] == 2);
        assert(comp[4] == 2);
        assert(comp[5] == 3);
    }

    // Self-loop
    {
        std::vector<int> comp = assignComponents(2, {{1,1}});
        assert(comp[1] == 1);
        assert(comp[2] == 2);
    }

    // Multiple edges (duplicates)
    {
        std::vector<int> comp = assignComponents(3, {{1,2},{2,1},{2,3},{3,2}});
        assert(comp[1] == 1);
        assert(comp[2] == 1);
        assert(comp[3] == 1);
    }

    // Larger connected graph
    {
        std::vector<int> comp = assignComponents(6, {{1,2},{2,3},{3,1},{4,5}});
        assert(comp[1] == 1);
        assert(comp[2] == 1);
        assert(comp[3] == 1);
        assert(comp[4] == 2);
        assert(comp[5] == 2);
        assert(comp[6] == 3);
    }

    // All isolated vertices
    {
        std::vector<int> comp = assignComponents(4, {});
        assert(comp[1] == 1);
        assert(comp[2] == 2);
        assert(comp[3] == 3);
        assert(comp[4] == 4);
    }

    // n=0 should return size 1 (vector with only index 0)
    {
        std::vector<int> comp = assignComponents(0, {});
        assert(comp.size() == 1);
    }
}
