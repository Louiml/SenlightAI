Given an odd integer `k` (where `1 <= k <= 99`), write a C++ function that constructs and returns a graph represented as an adjacency list or edge list (you may choose) with the following properties: (1) every vertex has degree exactly `k` (a `k`-regular graph), (2) the graph is connected, and (3) the graph has no bridges (i.e., no edge whose removal disconnects the graph). If `k` is even, the function must return an empty structure (or a signal that no such graph exists). The output should be self-contained; you do not need to handle input/output, only the function. The function will be tested internally for correctness on various odd `k` values.

#include <cassert>
#include <vector>
#include <utility>
#include <set>
#include <map>

// Forward declaration of the solution function
std::vector<std::pair<int, int>> constructRegularBridgelessGraph(int k);

int main() {
    // Helper to check validity of graph for odd k
    auto verify = [](int k, const std::vector<std::pair<int,int>>& edges) {
        if (k % 2 == 1) {
            assert(!edges.empty());
            // Count degrees
            std::map<int,int> deg;
            for (auto& e : edges) {
                deg[e.first]++;
                deg[e.second]++;
            }
            // All degrees must be exactly k
            for (auto& d : deg) assert(d.second == k);
            // Check connectivity and no bridges using brute force (small k)
            int n = deg.size();
            // Check connectivity
            std::vector<std::vector<int>> adj(n+1);
            for (auto& e : edges) {
                adj[e.first].push_back(e.second);
                adj[e.second].push_back(e.first);
            }
            // BFS from vertex 1
            std::vector<bool> vis(n+1, false);
            std::vector<int> q = {1};
            vis[1] = true;
            int head = 0;
            while (head < (int)q.size()) {
                int u = q[head++];
                for (int v : adj[u]) if (!vis[v]) { vis[v] = true; q.push_back(v); }
            }
            for (int i = 1; i <= n; ++i) assert(vis[i]);

            // Check no bridges: for each edge, remove it and test if graph remains connected
            for (auto& e : edges) {
                std::vector<std::vector<int>> adj2(n+1);
                for (auto& f : edges) {
                    if (f == e) continue;
                    adj2[f.first].push_back(f.second);
                    adj2[f.second].push_back(f.first);
                }
                std::vector<bool> vis2(n+1, false);
                std::vector<int> q2 = {1};
                vis2[1] = true;
                int head2 = 0;
                while (head2 < (int)q2.size()) {
                    int u = q2[head2++];
                    for (int v : adj2[u]) if (!vis2[v]) { vis2[v] = true; q2.push_back(v); }
                }
                for (int i = 1; i <= n; ++i) assert(vis2[i]);
            }
        }
    };

    // Test even k: should return empty
    assert(constructRegularBridgelessGraph(2).empty());
    assert(constructRegularBridgelessGraph(4).empty());
    assert(constructRegularBridgelessGraph(10).empty());

    // Test odd k
    for (int k = 1; k <= 9; k += 2) {
        auto edges = constructRegularBridgelessGraph(k);
        verify(k, edges);
    }

    // Specifically test k=1: returns a single edge
    auto e1 = constructRegularBridgelessGraph(1);
    assert(e1.size() == 1);
    assert(e1[0] == std::make_pair(1,2));

    // Test k=3: should have 10 vertices and each degree 3
    auto e3 = constructRegularBridgelessGraph(3);
    std::map<int,int> deg;
    for (auto& e : e3) { deg[e.first]++; deg[e.second]++; }
    assert(deg.size() == 10);
    for (auto& d : deg) assert(d.second == 3);

    // Test k=5: should have 14 vertices and each degree 5
    auto e5 = constructRegularBridgelessGraph(5);
    std::map<int,int> deg5;
    for (auto& e : e5) { deg5[e.first]++; deg5[e.second]++; }
    assert(deg5.size() == 14);
    for (auto& d : deg5) assert(d.second == 5);

    return 0;
}

#include <vector>
#include <utility>

// Return a connected k-regular bridgeless graph for odd k, or an empty vector for even k.
// The graph is represented as a vector of edges (1-indexed vertices).
std::vector<std::pair<int, int>> constructRegularBridgelessGraph(int k) {
    if (k % 2 == 0) {
        return {}; // No such graph exists for even k
    }
    if (k == 1) {
        return {{1, 2}}; // A single edge is 1-regular and connected
    }

    // Build one component of size k+2, with vertices 1..k+2.
    // In this component, vertices 1 and k+2 have degree k-1; all others have degree k.
    // Then we duplicate the component with an offset of k+2 and connect vertex 1 to vertex k+3.
    std::vector<std::pair<int, int>> edges;

    // First component: vertices 1..k+2
    // Vertex 1 connects to vertices 2..k (k-1 edges)
    for (int v = 2; v <= k; ++v) {
        edges.emplace_back(1, v);
    }

    // For vertices 2..k+1 (excluding the special vertex 1 and the last special vertex k+2):
    // Connect to all later vertices except those in the same "pair group" to avoid bridges.
    for (int i = 2; i <= k+1; ++i) {
        // Connect i to all j > i, but skip j if they are in the same group (i-1)/2 == (j-1)/2
        for (int j = i+1; j <= k+2; ++j) {
            if ((i-1)/2 == (j-1)/2) continue;
            edges.emplace_back(i, j);
        }
    }

    // Now duplicate the component with offset k+2 (vertices k+3 .. 2k+4)
    int offset = k+2;
    for (int i = 1; i <= k+2; ++i) {
        for (int j = i+1; j <= k+2; ++j) {
            if ((i-1)/2 == (j-1)/2) continue;
            edges.emplace_back(i+offset, j+offset);
        }
    }
    // Add the connection between the two special vertices: component1 vertex 1 and component2 vertex 1 (which is 1+offset)
    edges.emplace_back(1, 1+offset);

    // Adjust the second component's special vertex 1: in duplication, we also have edges from vertex 1+offset to 2+offset..k+offset, which is correct.
    // But note: in the first component, vertex 1 has degree k-1; in second component, vertex 1+offset also has degree k-1; connecting them gives degree k.

    return edges;
}

// The core idea is that a `k`-regular connected bridgeless graph cannot exist when `k` is even because of a parity argument: the total degree sum in any connected component must be even, and removing a bridge would split the graph into two components where the degree sum becomes odd (since each component would have an odd number of odd-degree vertices). For odd `k`, we use a constructive approach: create two identical copies of a carefully designed component of `k+2` vertices, each vertex having degree `k` except for two special vertices that have degree `k-1` (so we can connect the two components with one edge between those special vertices, making all degrees exactly `k`). The component is built so that it is connected and has no bridges, and the single edge connecting the two components is also not a bridge because removing it leaves two identical components that are themselves connected and have no bridges—thus the graph remains connected. The construction handles `k=1` specially (a simple edge between two vertices). For even `k`, we return an empty result. Time complexity is O(k^2) per component (due to nested loops over ~k vertices), and space complexity is O(k^2) for the edge list. Edge cases: `k=1` returns a single edge; odd `k` works; even `k` returns no graph.
