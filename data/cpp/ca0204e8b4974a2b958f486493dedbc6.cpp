Write a C++ function `bipartiteCheck(int N, const std::vector<std::pair<int,int>>& edges)` that takes the number of vertices `N` (numbered 1 through N) and an edge list, then returns a `std::vector<int>` where each element is either 1 or 2 representing a valid bipartite coloring, or an empty vector if the graph is not bipartite. The graph is undirected, may be disconnected, and may contain up to 10^5 vertices and 2×10^5 edges. The solution must efficiently determine whether the graph can be colored with two colors such that no two adjacent vertices share the same color, and if possible, output any valid coloring.
// The problem is the classic bipartite graph check using BFS/DFS with coloring. For each connected component (since the graph may be disconnected), we start from an uncolored vertex, assign it color 1, then traverse all reachable vertices using a stack or queue. Each neighbor of a colored vertex must get the opposite color; if a neighbor already has the same color, the graph is not bipartite and we return an empty vector. We must handle multiple components by iterating over all vertices, skipping those already colored. Key edge cases include: single vertex (trivially bipartite with color 1), empty graph (all vertices isolated → all color 1), self-loops (would immediately cause conflict since neighbor==node with same color), and parallel edges (no issue since checking color equality works). Time complexity is O(N + M) since each edge is visited once and each vertex colored once; space complexity is O(N + M) for adjacency lists.
#include <vector>
#include <stack>

// Check if the graph is bipartite and return a valid 2-coloring (colors 1 and 2).
// Returns an empty vector if no valid coloring exists.
std::vector<int> bipartiteCheck(int N, const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> adj(N + 1);
    for (const auto& e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    std::vector<int> colors(N + 1, 0);
    
    for (int start = 1; start <= N; ++start) {
        if (colors[start] != 0) continue;
        
        colors[start] = 1;
        std::stack<int> st;
        st.push(start);
        
        while (!st.empty()) {
            int node = st.top();
            st.pop();
            
            for (int neighbor : adj[node]) {
                if (colors[neighbor] == 0) {
                    colors[neighbor] = (colors[node] == 1) ? 2 : 1;
                    st.push(neighbor);
                } else if (colors[neighbor] == colors[node]) {
                    return {};
                }
            }
        }
    }
    
    std::vector<int> result;
    for (int i = 1; i <= N; ++i) {
        result.push_back(colors[i]);
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is above. This is the test harness.
int main() {
    // Test 1: Simple bipartite graph (two connected nodes)
    std::vector<int> c1 = bipartiteCheck(2, {{1,2}});
    assert(c1.size() == 2 && c1[0] != c1[1]);

    // Test 2: Triangle is not bipartite
    std::vector<int> c2 = bipartiteCheck(3, {{1,2},{2,3},{3,1}});
    assert(c2.empty());

    // Test 3: Disconnected components, each bipartite
    std::vector<int> c3 = bipartiteCheck(4, {{1,2},{3,4}});
    assert(c3.size() == 4);
    assert(c3[0] != c3[1]);
    assert(c3[2] != c3[3]);

    // Test 4: Single vertex is trivially bipartite
    std::vector<int> c4 = bipartiteCheck(1, {});
    assert(c4.size() == 1 && c4[0] == 1);

    // Test 5: Self-loop is not bipartite
    std::vector<int> c5 = bipartiteCheck(2, {{1,1}});
    assert(c5.empty());

    // Test 6: Cycle of even length is bipartite
    std::vector<int> c6 = bipartiteCheck(4, {{1,2},{2,3},{3,4},{4,1}});
    assert(c6.size() == 4);
    for (int i = 0; i < 4; ++i) {
        int next = (i+1) % 4;
        assert(c6[i] != c6[next]);
    }

    // Test 7: Large path graph
    std::vector<std::pair<int,int>> edges7;
    for (int i = 1; i < 100; ++i) edges7.push_back({i, i+1});
    std::vector<int> c7 = bipartiteCheck(100, edges7);
    assert(c7.size() == 100);
    for (int i = 0; i < 99; ++i) assert(c7[i] != c7[i+1]);

    // Test 8: Star graph is bipartite
    std::vector<std::pair<int,int>> edges8;
    for (int i = 2; i <= 5; ++i) edges8.push_back({1, i});
    std::vector<int> c8 = bipartiteCheck(5, edges8);
    assert(c8.size() == 5);
    for (int i = 1; i < 5; ++i) assert(c8[i] != c8[0]);

    // Test 9: Empty graph with multiple vertices
    std::vector<int> c9 = bipartiteCheck(10, {});
    assert(c9.size() == 10);
    for (int v : c9) assert(v == 1 || v == 2);
}
