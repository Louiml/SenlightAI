Design a C++ function `CartWheelSummary` that, given a hub degree `hub_degree` and a vector of fixed neighbor degrees (each an integer between 5 and `max_degree` inclusive), constructs a wheel graph with a central hub connected to `hub_degree` vertices arranged in a cycle, then extends it into a "cartwheel" by adding a second neighbor layer for each adjacent pair of hub neighbors, and finally returns a string representation in the format `N E deg0 deg1 ... deg{N-1} u0 v0 u1 v1 ...` where `N` is the total number of vertices (hub + neighbors + second neighbors), `E` is the number of edges, each `deg_i` is the degree of vertex `i` (with `?` for vertices whose degree is not yet fixed), and each `u_j v_j` is an undirected edge. All hub neighbor degrees are fixed to the given input values, all second neighbors have degree 4 (initially unknown, so output `?`), and the hub has degree equal to `hub_degree`. The function must assume that no vertex has degree less than 5, and if `hub_degree` is 3 or 4, the wheel is simply the wheel without second neighbors (since the cycle already forms triangles). The output must list vertices in order: hub (index 0), then hub neighbors (indices 1..hub_degree), then second neighbors (indices hub_degree+1..N-1) in the order they are created, iterating over hub neighbors `v` from 1 to `hub_degree` and for each `v` adding a second neighbor between `v` and its next neighbor `(v % hub_degree) + 1`. Edge order should follow adjacency list construction, but any valid enumeration of all edges is acceptable.

The main algorithm: First, create a hub vertex 0 and `hub_degree` neighbor vertices 1..`hub_degree`. Connect hub to each neighbor and connect each neighbor `v` to its successor `(v % hub_degree) + 1` (cyclic). This forms a wheel. For each neighbor `v`, if `hub_degree` is at least 5 (so that the cycle has length ≥5), insert a new second neighbor vertex `w` connected to both `v` and its successor `u`, making a triangle `v-w-u`. For `hub_degree` 3 or 4, the wheel already has triangular faces everywhere (since a cycle of length 3 or 4 with a hub is a triangulation), so no second neighbors are added. Degrees: hub degree = `hub_degree` (fixed), neighbor degrees = given input (fixed), second neighbors have degree 2 (not 4) because they connect to exactly two vertices (`v` and `u`), but the original code's `fromWheel` function creates second neighbors as degree-unknown vertices with current connections to two vertices, so the degree is not fixed yet and we output `?`. Important edge cases: `hub_degree` must be at least 3 (a valid wheel), but the problem states we can assume valid input. If a neighbor degree is less than 5, the wheel would be invalid (since planar triangulation requires degree ≥3, but here we assume degrees ≥5 as per context). In the output string, vertices are numbered from 0; we must ensure the edge list includes all edges: hub-neighbor, neighbor-neighbor (cycle), and neighbor-second-neighbor edges. Time complexity: O(hub_degree) to build and output, space O(N) for adjacency storage.

#include <string>
#include <vector>
#include <set>
#include <sstream>

// Returns a string representation of the cartwheel constructed from a hub degree and fixed neighbor degrees.
// Format: "N E deg0 deg1 ... deg{N-1} u0 v0 u1 v1 ..."
// Degrees of unknown vertices are represented as '?'. Hub is vertex 0, neighbors 1..hub_degree, second neighbors follow.
std::string CartWheelSummary(int hub_degree, const std::vector<int>& neighbor_degrees) {
    // Validate input
    if (hub_degree < 3 || (int)neighbor_degrees.size() != hub_degree) {
        return "";
    }

    // Number of second neighbors
    int num_second = (hub_degree >= 5) ? hub_degree : 0;
    int N = 1 + hub_degree + num_second;
    
    // Build adjacency sets
    std::vector<std::set<int>> adj(N);
    
    // Hub connections
    for (int v = 1; v <= hub_degree; ++v) {
        adj[0].insert(v);
        adj[v].insert(0);
        // Cycle edges
        int u = (v == hub_degree) ? 1 : v + 1;
        adj[v].insert(u);
        adj[u].insert(v);
    }
    
    // Second neighbors
    if (num_second > 0) {
        for (int v = 1; v <= hub_degree; ++v) {
            int w = hub_degree + v; // second neighbor vertex id
            int u = (v == hub_degree) ? 1 : v + 1;
            adj[v].insert(w);
            adj[w].insert(v);
            adj[u].insert(w);
            adj[w].insert(u);
        }
    }
    
    // Collect all edges in a deterministic order
    std::vector<std::pair<int,int>> edges;
    for (int v = 0; v < N; ++v) {
        for (int u : adj[v]) {
            if (v < u) {
                edges.emplace_back(v, u);
            }
        }
    }
    
    // Build degree strings
    std::vector<std::string> deg_str(N, "?");
    deg_str[0] = std::to_string(hub_degree);
    for (int i = 0; i < hub_degree; ++i) {
        deg_str[i + 1] = std::to_string(neighbor_degrees[i]);
    }
    // Second neighbors remain "?" (degree not fixed yet)
    
    // Construct output
    std::ostringstream oss;
    oss << N << " " << edges.size();
    for (const auto& d : deg_str) {
        oss << " " << d;
    }
    for (const auto& e : edges) {
        oss << " " << e.first << " " << e.second;
    }
    return oss.str();
}

#include <cassert>
#include <string>
#include <vector>

std::string CartWheelSummary(int hub_degree, const std::vector<int>& neighbor_degrees);

int main() {
    // hub_degree 3: wheel with no second neighbors, all degrees known
    {
        std::string res = CartWheelSummary(3, {5, 5, 5});
        // N=4, E=6, degrees: hub=3, neighbors=5,5,5
        std::string expected = "4 6 3 5 5 5 0 1 0 2 0 3 1 2 2 3 3 1";
        assert(res == expected);
    }
    // hub_degree 4: wheel with no second neighbors
    {
        std::string res = CartWheelSummary(4, {5, 6, 5, 6});
        // N=5, E=8
        std::string expected = "5 8 4 5 6 5 6 0 1 0 2 0 3 0 4 1 2 2 3 3 4 4 1";
        assert(res == expected);
    }
    // hub_degree 5: one second neighbor per cycle edge
    {
        std::string res = CartWheelSummary(5, {5, 5, 5, 5, 5});
        // N=11, E=15 edges? Hub edges=5, cycle edges=5, second edges=10 -> total 20? Let's compute: 
        // hub-neighbor:5, cycle:5, second neighbor edges: each second connects to two neighbors -> 2*5=10, total=20
        // degrees: hub=5, neighbors=5, second neighbors "?"
        // Build expected by checking edge count and pattern
        std::string first_part = res.substr(0, res.find(' ', 3));
        // Extract N and E
        int space1 = res.find(' ');
        int space2 = res.find(' ', space1+1);
        assert(res.substr(0, space1) == "11");
        assert(res.substr(space1+1, space2-space1-1) == "20");
        // Check hub degree
        int space3 = res.find(' ', space2+1);
        assert(res.substr(space2+1, space3-space2-1) == "5");
    }
    // hub_degree 6: ensure all second neighbor degrees show '?'
    {
        std::string res = CartWheelSummary(6, {5, 6, 7, 5, 6, 7});
        int space_count = 0;
        for (char c : res) if (c == ' ') space_count++;
        // First two tokens are N and E, then N degrees, then 2E edge endpoints
        // We'll just verify that the count of '?' equals number of second neighbors (6)
        int question_count = 0;
        for (char c : res) if (c == '?') question_count++;
        assert(question_count == 6);
    }
    // Invalid input returns empty string
    {
        assert(CartWheelSummary(2, {5, 5}) == "");
        assert(CartWheelSummary(3, {5}) == "");
    }
    // Edge count for hub_degree 5 is exactly 20
    {
        std::string res = CartWheelSummary(5, {5, 5, 5, 5, 5});
        // Parse N and E
        int pos = res.find(' ');
        int N = std::stoi(res.substr(0, pos));
        int pos2 = res.find(' ', pos+1);
        int E = std::stoi(res.substr(pos+1, pos2-pos-1));
        assert(N == 11);
        assert(E == 20);
    }
    return 0;
}
