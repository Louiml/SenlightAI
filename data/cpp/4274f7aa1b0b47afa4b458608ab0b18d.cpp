// Design a standalone C++ function `bfsParallel` that performs a breadth-first search on an undirected graph and returns the shortest distance from a given root node to every other node. The graph is represented using a struct `Graph` with fields `num_nodes`, `num_edges`, `outgoing_starts`, `outgoing_edges`, `incoming_starts`, and `incoming_edges` (where `outgoing_starts` and `incoming_starts` are arrays of size `num_nodes + 1`). The function must accept a pointer to the graph and a root node ID, and return a `std::vector<int>` of size `num_nodes` where each entry is the distance from the root (0 for the root itself) or -1 if the node is unreachable. Implement the BFS using a hybrid approach: for each level, choose between a top-down frontier expansion (processing the frontier’s outgoing edges) and a bottom-up scan (checking unvisited nodes’ incoming edges) based on a threshold: use top-down when the frontier size is ≤ `num_nodes / 120`, otherwise use bottom-up. The implementation must be thread-safe and parallelized with OpenMP where appropriate (e.g., parallelize loops over frontier vertices or node scanning). The function must not rely on any external BFS utilities—only standard C++ headers and OpenMP. Ensure memory correctness and avoid race conditions by using atomic operations or critical sections where needed, or by using per-thread buffers and merging them.
#include <cassert>
#include <vector>

// Assume Graph struct from solution is defined above.

int main() {
    // Test 1: Simple chain 0-1-2, root 0
    Graph g1;
    g1.num_nodes = 3;
    g1.num_edges = 2;
    g1.outgoing_starts = {0, 1, 2, 2};
    g1.outgoing_edges = {1, 2};
    g1.incoming_starts = {0, 0, 1, 2};
    g1.incoming_edges = {0, 1};
    auto d1 = bfsParallel(&g1, 0);
    assert(d1 == std::vector<int>({0, 1, 2}));

    // Test 2: Disconnected graph, root 0
    Graph g2;
    g2.num_nodes = 4;
    g2.num_edges = 1;
    g2.outgoing_starts = {0, 1, 1, 1, 1};
    g2.outgoing_edges = {2};
    g2.incoming_starts = {0, 0, 0, 1, 1};
    g2.incoming_edges = {0};
    auto d2 = bfsParallel(&g2, 0);
    assert(d2 == std::vector<int>({0, -1, 1, -1}));

    // Test 3: Single node
    Graph g3;
    g3.num_nodes = 1;
    g3.num_edges = 0;
    g3.outgoing_starts = {0, 0};
    g3.outgoing_edges = {};
    g3.incoming_starts = {0, 0};
    g3.incoming_edges = {};
    auto d3 = bfsParallel(&g3, 0);
    assert(d3 == std::vector<int>({0}));

    // Test 4: Complete graph K4, root 2
    Graph g4;
    g4.num_nodes = 4;
    g4.num_edges = 12;
    g4.outgoing_starts = {0, 3, 6, 9, 12};
    g4.outgoing_edges = {1,2,3, 0,2,3, 0,1,3, 0,1,2};
    g4.incoming_starts = {0, 3, 6, 9, 12};
    g4.incoming_edges = {1,2,3, 0,2,3, 0,1,3, 0,1,2};
    auto d4 = bfsParallel(&g4, 2);
    assert(d4 == std::vector<int>({1, 1, 0, 1}));

    // Test 5: Root not in graph (invalid, but if root is out of range? We assume valid input)

    // Test 6: Larger graph to trigger bottom-up (e.g., star with many nodes)
    int n = 1000;
    Graph g5;
    g5.num_nodes = n;
    g5.num_edges = 2*(n-1); // undirected edges from center to leaves
    g5.outgoing_starts.resize(n+1);
    g5.incoming_starts.resize(n+1);
    std::vector<int> out_edges(2*(n-1));
    std::vector<int> in_edges(2*(n-1));
    // Center = 0, edges to every other node
    g5.outgoing_starts[0] = 0;
    for (int i=1; i<n; i++) {
        g5.outgoing_starts[i] = 2*(i-1);
        g5.outgoing_edges[2*(i-1)] = i;
        g5.outgoing_edges[2*(i-1)+1] = 0;
    }
    g5.outgoing_starts[n] = 2*(n-1);
    // Incoming same as outgoing for undirected graph
    g5.incoming_starts = g5.outgoing_starts;
    g5.incoming_edges = g5.outgoing_edges;
    auto d5 = bfsParallel(&g5, 0);
    for (int i=0; i<n; i++) {
        assert(d5[i] == (i==0 ? 0 : 1));
    }

    // Test 7: Empty graph (no nodes) — expect empty vector
    Graph g6;
    g6.num_nodes = 0;
    g6.num_edges = 0;
    g6.outgoing_starts = {0};
    g6.outgoing_edges = {};
    g6.incoming_starts = {0};
    g6.incoming_edges = {};
    auto d6 = bfsParallel(&g6, 0);
    assert(d6.empty());

    return 0;
}
#include <vector>
#include <omp.h>
#include <atomic>

struct Graph {
    int num_nodes;
    int num_edges;
    std::vector<int> outgoing_starts; // size num_nodes+1
    std::vector<int> outgoing_edges;  // size num_edges
    std::vector<int> incoming_starts; // size num_nodes+1
    std::vector<int> incoming_edges;  // size num_edges
};

// Perform parallel BFS with hybrid top-down/bottom-up approach.
// Returns vector of distances from root, -1 for unreachable.
std::vector<int> bfsParallel(const Graph* g, int root) {
    const int n = g->num_nodes;
    const int NOT_VISITED = -1;
    const double beta = 120.0;
    std::vector<int> distances(n, NOT_VISITED);
    distances[root] = 0;

    // Current and next frontiers
    std::vector<int> frontier;
    std::vector<int> next_frontier;
    frontier.reserve(n);
    next_frontier.reserve(n);
    frontier.push_back(root);

    int level = 0;
    while (!frontier.empty()) {
        next_frontier.clear();
        bool use_bottom_up = (frontier.size() > static_cast<size_t>(n / beta));

        if (!use_bottom_up) {
            // Top-down: process each frontier node's outgoing edges
            #pragma omp parallel
            {
                std::vector<int> local_next;
                local_next.reserve(frontier.size() * 2);
                #pragma omp for nowait
                for (int i = 0; i < static_cast<int>(frontier.size()); ++i) {
                    int node = frontier[i];
                    for (int e = g->outgoing_starts[node]; e < g->outgoing_starts[node+1]; ++e) {
                        int nb = g->outgoing_edges[e];
                        if (distances[nb] == NOT_VISITED) {
                            // Use atomic compare-and-swap to claim the node
                            int expected = NOT_VISITED;
                            if (__atomic_compare_exchange_n(&distances[nb], &expected, level+1, false, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED)) {
                                local_next.push_back(nb);
                            }
                        }
                    }
                }
                // Merge local results
                #pragma omp critical
                {
                    next_frontier.insert(next_frontier.end(), local_next.begin(), local_next.end());
                }
            }
        } else {
            // Bottom-up: scan all unvisited nodes, check incoming edges
            int num_threads = omp_get_max_threads();
            std::vector<std::vector<int>> local_lists(num_threads);
            #pragma omp parallel
            {
                int tid = omp_get_thread_num();
                local_lists[tid].reserve(n / num_threads + 1);
                #pragma omp for nowait
                for (int node = 0; node < n; ++node) {
                    if (distances[node] != NOT_VISITED) continue;
                    bool found = false;
                    for (int e = g->incoming_starts[node]; e < g->incoming_starts[node+1]; ++e) {
                        int nb = g->incoming_edges[e];
                        if (distances[nb] == level) {
                            distances[node] = level + 1;
                            local_lists[tid].push_back(node);
                            found = true;
                            break;
                        }
                    }
                }
            }
            for (auto& lst : local_lists) {
                next_frontier.insert(next_frontier.end(), lst.begin(), lst.end());
            }
        }
        frontier.swap(next_frontier);
        ++level;
    }
    return distances;
}
// The solution maintains a distances vector initialized to -1, with the root set to 0. We use two frontier vectors (current and next) to represent nodes at the current BFS level. The algorithm proceeds level by level: for each level, we decide between top-down and bottom-up. Top-down processes each node in the current frontier, iterates over its outgoing edges, and if a neighbor is unvisited, sets its distance and adds it to the next frontier. This is safe to parallelize across frontier nodes because each node’s outgoing edges are disjoint, but we must protect the shared next frontier index with an atomic increment or use per-thread buffers. Bottom-up scans all unvisited nodes; for each such node, we check its incoming edges to see if any neighbor has distance equal to the current level. If found, we set the node’s distance and add it to the next frontier. To avoid race conditions, we either use critical sections or per-thread buffers that are merged afterward. The threshold beta=120 means we switch to bottom-up when the frontier is large, typically efficient for dense graphs. Edge cases: root isolated, graph disconnected, graph with a single node, zero edges. Time complexity is O(V+E) for both variants per level in the worst case, but bottom-up can be more efficient when the frontier is large. With OpenMP parallelism, we achieve near-linear speedup in practice. Memory usage is O(V+E) for the graph and O(V) for distances and frontiers.
