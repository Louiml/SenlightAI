// Design a C++ class `NetworkRouter` that models routers in an undirected graph where every edge has a unit weight. The class must support: a default constructor that assigns a unique ID (starting at 0) in instantiation order; a method `connect(NetworkRouter& neighbor)` to add an undirected edge between two routers; a method `buildRoutingTable()` that computes and stores the complete routing table for every router in the network using Dijkstra’s algorithm (but simplified for unit weights) from each router to all others, where each table entry maps a destination router ID to the ID of the first hop on the shortest path from the source to that destination (or to itself if the destination is unreachable, in which case store -1). The routing table must be stored as `std::map<int, int>` inside each router. Your solution must be a standalone function `PairingResult computeRoutingTables(std::vector<NetworkRouter>& routers)` that takes a vector of pre-constructed and pre-connected routers (with IDs already set) and returns a `struct PairingResult` containing a single string `summary` that lists each router’s ID and its routing table in a readable format (e.g., `"Router 0: {1:3, 2:3, 3:3, 4:3, 5:4, 6:4}\n"` etc.). The function must handle disconnected components (unreachable destinations) and work correctly when routers have no neighbors (only self-loop entries). The solution must be correct for graphs with up to 100 routers and arbitrary connections (but no self-loops and no multi-edges; edges are undirected and added once from each side by the caller). Do not include a main function in the solution; provide only the free function and any helper struct definitions. Ensure const correctness where appropriate (e.g., the function should not modify the routers’ existing connections, but it may modify the routing tables).
#include <cassert>
#include <sstream>
#include <vector>
#include <string>

// Include the solution code here (or link it)

int main() {
    // Test 1: Simple two-node network
    {
        std::vector<NetworkRouter> routers(2);
        routers[0].neighbors.push_back(&routers[1]);
        routers[1].neighbors.push_back(&routers[0]);
        PairingResult res = computeRoutingTables(routers);
        std::string expected = "Router 0: {1:1}\nRouter 1: {0:0}\n";
        assert(res.summary == expected);
    }

    // Test 2: Line of three nodes 0-1-2
    {
        std::vector<NetworkRouter> routers(3);
        routers[0].neighbors.push_back(&routers[1]);
        routers[1].neighbors.push_back(&routers[0]);
        routers[1].neighbors.push_back(&routers[2]);
        routers[2].neighbors.push_back(&routers[1]);
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:1, 2:1}\n"
            "Router 1: {0:0, 2:2}\n"
            "Router 2: {0:1, 1:1}\n";
        assert(res.summary == expected);
    }

    // Test 3: Disconnected graph (two isolated nodes)
    {
        std::vector<NetworkRouter> routers(2);
        // No connections
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:-1}\n"
            "Router 1: {0:-1}\n";
        assert(res.summary == expected);
    }

    // Test 4: Triangle with 3 nodes fully connected
    {
        std::vector<NetworkRouter> routers(3);
        routers[0].neighbors = {&routers[1], &routers[2]};
        routers[1].neighbors = {&routers[0], &routers[2]};
        routers[2].neighbors = {&routers[0], &routers[1]};
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:1, 2:2}\n"
            "Router 1: {0:0, 2:2}\n"
            "Router 2: {0:0, 1:1}\n";
        assert(res.summary == expected);
    }

    // Test 5: Larger network with a bottleneck (star center 0)
    {
        std::vector<NetworkRouter> routers(5);
        for (int i = 1; i < 5; ++i) {
            routers[0].neighbors.push_back(&routers[i]);
            routers[i].neighbors.push_back(&routers[0]);
        }
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:1, 2:2, 3:3, 4:4}\n"
            "Router 1: {0:0, 2:0, 3:0, 4:0}\n"
            "Router 2: {0:0, 1:0, 3:0, 4:0}\n"
            "Router 3: {0:0, 1:0, 2:0, 4:0}\n"
            "Router 4: {0:0, 1:0, 2:0, 3:0}\n";
        assert(res.summary == expected);
    }

    // Test 6: Four-node cycle 0-1-2-3-0
    {
        std::vector<NetworkRouter> routers(4);
        routers[0].neighbors = {&routers[1], &routers[3]};
        routers[1].neighbors = {&routers[0], &routers[2]};
        routers[2].neighbors = {&routers[1], &routers[3]};
        routers[3].neighbors = {&routers[2], &routers[0]};
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:1, 2:1, 3:3}\n"
            "Router 1: {0:0, 2:2, 3:0}\n"
            "Router 2: {0:3, 1:1, 3:3}\n"
            "Router 3: {0:0, 1:2, 2:2}\n";
        assert(res.summary == expected);
    }

    // Test 7: Two separate components each with two nodes (0-1 and 2-3)
    {
        std::vector<NetworkRouter> routers(4);
        routers[0].neighbors.push_back(&routers[1]);
        routers[1].neighbors.push_back(&routers[0]);
        routers[2].neighbors.push_back(&routers[3]);
        routers[3].neighbors.push_back(&routers[2]);
        PairingResult res = computeRoutingTables(routers);
        std::string expected = 
            "Router 0: {1:1, 2:-1, 3:-1}\n"
            "Router 1: {0:0, 2:-1, 3:-1}\n"
            "Router 2: {0:-1, 1:-1, 3:3}\n"
            "Router 3: {0:-1, 1:-1, 2:2}\n";
        assert(res.summary == expected);
    }

    return 0;
}
#include <vector>
#include <map>
#include <queue>
#include <limits>
#include <sstream>
#include <string>

struct NetworkRouter {
    int id;
    std::vector<NetworkRouter*> neighbors;
    std::map<int, int> routing_table; // dest ID -> next hop ID (or -1 if unreachable)
};

struct PairingResult {
    std::string summary;
};

// Helper to run Dijkstra from a given source and update its routing table.
void computeRouterTable(NetworkRouter& source, const std::vector<NetworkRouter>& allRouters) {
    int n = static_cast<int>(allRouters.size());
    const int INF = std::numeric_limits<int>::max();
    std::vector<int> dist(n, INF);
    std::vector<int> nextHop(n, -1);

    // Priority queue: (distance, node id)
    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    dist[source.id] = 0;
    pq.push({0, source.id});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        // Get actual router pointer
        const NetworkRouter& u_router = allRouters[u];
        for (const NetworkRouter* v_ptr : u_router.neighbors) {
            int v = v_ptr->id;
            int nd = d + 1;
            if (nd < dist[v]) {
                dist[v] = nd;
                // Set next hop: if u is the source, then next hop is v; else inherit from u's next hop
                if (u == source.id) {
                    nextHop[v] = v;
                } else {
                    nextHop[v] = nextHop[u];
                }
                pq.push({nd, v});
            }
        }
    }

    // Populate routing table for source (excluding self)
    source.routing_table.clear();
    for (int i = 0; i < n; ++i) {
        if (i != source.id) {
            source.routing_table[i] = nextHop[i]; // -1 if unreachable
        }
    }
}

// Main solution function.
PairingResult computeRoutingTables(std::vector<NetworkRouter>& routers) {
    // Ensure IDs are set based on vector index (they already are by construction)
    for (size_t i = 0; i < routers.size(); ++i) {
        routers[i].id = static_cast<int>(i);
    }

    // Compute table for each router
    for (auto& r : routers) {
        computeRouterTable(r, routers);
    }

    // Build summary string
    std::ostringstream oss;
    for (const auto& r : routers) {
        oss << "Router " << r.id << ": {";
        bool first = true;
        for (const auto& [dest, next] : r.routing_table) {
            if (!first) oss << ", ";
            oss << dest << ":" << next;
            first = false;
        }
        oss << "}\n";
    }
    return {oss.str()};
}
// The key challenge is to compute all-pairs shortest paths in an unweighted undirected graph. Since each edge has weight 1, we can run Dijkstra’s algorithm from each router using a priority queue (min-heap) with distance = number of hops. For each source router, we perform Dijkstra to find the shortest distances and also record the immediate next hop for each destination. To do this, during relaxation, when we improve the distance to a node `v` via `u`, we set the next hop for `v` to be the next hop stored for `u` (if `u` is the source, then next hop is `u`'s ID; otherwise it is the already stored next hop for `u`). After Dijkstra completes, for every router ID in the network, if the distance is finite, we store the next hop; if unreachable, store -1. This avoids backtracking through parent maps. Edge cases: (1) source has no neighbors – only itself shows in the table with next hop to itself? Actually, for the source itself we omit that entry (usually routing tables don't include self). The task says "for all others" – so we exclude the source itself from its own table. (2) Disconnected components: allocate distance = INF and next hop = -1 for those destinations. Time complexity: O(R * (V log V + E)) where R is number of routers, V=R, E=edges. For unweighted graph, we could use BFS, but the snippet uses Dijkstra so we keep that. Space: O(V^2) for all routing tables. Ensure we handle the fact that routers are passed by reference and we can read their neighbor lists; we assume the caller has already connected them bidirectionally (push_back both sides). The function will compute and store routing tables inside each router object.
