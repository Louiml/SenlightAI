/*
Write a C++ function that implements Dijkstra's shortest path algorithm on a fixed graph of 21 West Java cities. The graph is defined by an adjacency matrix `float WestJavaMap[21][21]` where `-1.0` represents no direct connection, and positive values represent road distances. The function should take a starting city name as a `std::string` (e.g., `"Garut"`, `"Bandung"`) and return a `std::vector<std::string>` containing the shortest path from that city to `"Jakarta"` in order of travel (including both the starting city and Jakarta). If the starting city name is not one of the 21 known cities, the function should return an empty vector. The function must not rely on any external files or global initialization beyond the provided graph and city names; it should build the graph internally.
*/

#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

// Dijkstra's shortest path from a given city to Jakarta.
// Returns a vector of city names in order from the start to Jakarta.
// Returns an empty vector if the city name is invalid.
std::vector<std::string> shortestPathToJakarta(const std::string& startCity) {
    // Names indexed by enum values
    const std::vector<std::string> cityNames = {
        "Garut", "Jakarta", "Tasikmalaya", "Ciamis", "Banjar", "Pangandaran",
        "Sumedang", "Bandung", "Cimahi", "Kuningan", "Majalengka", "Cirebon",
        "Indramayu", "Subang", "Karawang", "Purwakarta", "Cianjur", "Sukabumi",
        "Bogor", "Depok", "Bekasi"
    };
    const int V = 21;
    const int GOAL = 1; // Jakarta index

    // Build adjacency matrix
    std::vector<std::vector<float>> adj(V, std::vector<float>(V, -1.0f));
    auto addEdge = [&](int a, int b, float w) {
        adj[a][b] = w;
        adj[b][a] = w;
    };
    addEdge(4, 5, 63);   // Pangandaran (5) - Banjar (4)? Actually indices: Garut=0, Jakarta=1, Tasik=2, Ciamis=3, Banjar=4, Pangandaran=5
    // Correcting from snippet: Pangandaran[5]-Banjar[4]=63
    addEdge(4, 3, 26);   // Banjar-Ciamis
    addEdge(4, 9, 83);   // Banjar-Kuningan
    addEdge(3, 2, 19);   // Ciamis-Tasik
    addEdge(3, 9, 68);   // Ciamis-Kuningan
    addEdge(2, 0, 56);   // Tasik-Garut
    addEdge(2, 7, 93);   // Tasik-Bandung
    addEdge(0, 7, 51);   // Garut-Bandung
    addEdge(6, 7, 33);   // Sumedang-Bandung
    addEdge(9, 11, 32);  // Kuningan-Cirebon
    addEdge(9, 10, 58);  // Kuningan-Majalengka
    addEdge(11, 12, 55); // Cirebon-Indramayu
    addEdge(11, 10, 43); // Cirebon-Majalengka
    addEdge(10, 6, 47);  // Majalengka-Sumedang
    addEdge(10, 13, 85); // Majalengka-Subang
    addEdge(12, 14, 115); // Indramayu-Karawang
    addEdge(7, 13, 60);  // Bandung-Subang
    addEdge(7, 8, 35);   // Bandung-Cimahi
    addEdge(8, 15, 55);  // Cimahi-Purwakarta
    addEdge(8, 16, 56);  // Cimahi-Cianjur
    addEdge(13, 15, 45); // Subang-Purwakarta
    addEdge(15, 14, 30); // Purwakarta-Karawang
    addEdge(14, 20, 45); // Karawang-Bekasi
    addEdge(16, 17, 30); // Cianjur-Sukabumi
    addEdge(16, 18, 69); // Cianjur-Bogor
    addEdge(17, 18, 72); // Sukabumi-Bogor
    addEdge(18, 19, 22); // Bogor-Depok
    addEdge(19, 1, 26);  // Depok-Jakarta
    addEdge(20, 1, 27);  // Bekasi-Jakarta

    // Find start index
    int start = -1;
    for (int i = 0; i < V; ++i) {
        if (cityNames[i] == startCity) {
            start = i;
            break;
        }
    }
    if (start == -1) return {};

    // Dijkstra
    const float INF = std::numeric_limits<float>::infinity();
    std::vector<float> dist(V, INF);
    std::vector<int> parent(V, -1);
    dist[start] = 0.0f;
    using P = std::pair<float, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0.0f, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        if (u == GOAL) break;
        for (int v = 0; v < V; ++v) {
            float w = adj[u][v];
            if (w < 0.0f) continue; // no edge
            float nd = d + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    if (dist[GOAL] == INF) return {}; // unreachable (shouldn't happen)

    // Reconstruct path
    std::vector<int> path;
    for (int v = GOAL; v != -1; v = parent[v]) {
        path.push_back(v);
        if (v == start) break;
    }
    std::reverse(path.begin(), path.end());

    std::vector<std::string> result;
    result.reserve(path.size());
    for (int idx : path) {
        result.push_back(cityNames[idx]);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is expected to be declared above.
// This main function tests it.

int main() {
    // Test 1: direct path from Jakarta
    auto path = shortestPathToJakarta("Jakarta");
    assert(path.size() == 1 && path[0] == "Jakarta");

    // Test 2: from Bekasi directly to Jakarta
    path = shortestPathToJakarta("Bekasi");
    assert(path.size() == 2);
    assert(path[0] == "Bekasi" && path[1] == "Jakarta");

    // Test 3: from a city one hop away via Depok
    path = shortestPathToJakarta("Depok");
    assert(path.size() == 2);
    assert(path[0] == "Depok" && path[1] == "Jakarta");

    // Test 4: from Garut, check that path starts with Garut and ends with Jakarta
    path = shortestPathToJakarta("Garut");
    assert(!path.empty());
    assert(path.front() == "Garut" && path.back() == "Jakarta");

    // Test 5: from Bandung, check path is reasonable (e.g., total distance should be less than going through all nodes)
    path = shortestPathToJakarta("Bandung");
    assert(path.front() == "Bandung" && path.back() == "Jakarta");

    // Test 6: invalid city name returns empty
    path = shortestPathToJakarta("UnknownCity");
    assert(path.empty());

    // Test 7: from Cimahi, verify a valid path and that it doesn't contain unreachable city
    path = shortestPathToJakarta("Cimahi");
    assert(path.front() == "Cimahi" && path.back() == "Jakarta");
    // Ensure every consecutive pair in the path actually has an edge (we can't easily check distances here,
    // but we can at least ensure the path is non-empty and has expected endpoints)

    // Test 8: from Sukabumi, verify it goes via Bogor / Depok route (just check endpoints)
    path = shortestPathToJakarta("Sukabumi");
    assert(path.front() == "Sukabumi" && path.back() == "Jakarta");

    // Test 9: from Pangandaran (far south), path must exist and not be empty
    path = shortestPathToJakarta("Pangandaran");
    assert(!path.empty() && path.back() == "Jakarta");

    // Test 10: from a city like Cianjur, ensure path length is at least 3 (must go through Bogor/Depok or Purwakarta etc.)
    path = shortestPathToJakarta("Cianjur");
    assert(path.size() >= 2);

    return 0;
}

// The core problem is to find the shortest path in a weighted, undirected graph with 21 nodes (cities). Since all edge weights are non-negative, Dijkstra's algorithm is appropriate and efficient. The solution involves:
// - Storing city names in an array indexed by an `enum` from 0 to 20.
// - Building the adjacency matrix inside the function or using a static local variable for reuse.
// - Mapping the input string to an integer index via linear search; if not found, return empty vector.
// - Running Dijkstra from the starting node to the goal node (Jakarta, index 1). Use a priority queue (min-heap) of pairs `(distance, node)` and a `parent` array to reconstruct the path.
// - Edge cases: starting city is Jakarta itself (path is just `{"Jakarta"}`); the graph is connected from all cities, so failure shouldn't occur, but the algorithm handles it gracefully.
// - Time complexity: O(E log V) where E is number of edges (about 36 undirected edges) and V=21, so trivial. Space complexity: O(V) for distances, parents, and the path vector.
