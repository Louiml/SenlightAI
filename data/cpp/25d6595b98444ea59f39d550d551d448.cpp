Write a standalone C++ function `assignDomains` that processes a simplified event-driven simulation graph: the function receives a directed acyclic graph represented as a vector of vertices (each with a list of incoming source vertex indices) plus a vector of initial "domain" labels (integers) for some vertices (sentinel `-1` for vertices whose domain is not yet assigned). The function must assign a non-negative integer domain to every vertex using the rule: if a vertex already has a domain, keep it; otherwise, gather domains from all incoming neighbors (ignoring any incoming neighbor with domain `-1`), and if all gathered domains are equal and non-negative, assign that value; if no incoming neighbor has a non-negative domain, assign `-2` (meaning "deletable/untriggered"); if there are multiple distinct non-negative domains among incoming neighbors, assign a fresh new domain value that is the smallest positive integer not already used by any existing domain (including those already assigned to vertices and any domains created earlier in the same pass). The function must return a vector of assigned domains for all vertices in the original order. The graph may have vertices with no incoming edges, and a vertex's own initial domain must be preserved even if it conflicts with incoming neighbors. The function must be deterministic and handle cycles by ignoring back-edges (assume the graph is acyclic, but for safety you may detect and ignore any edge that would create a cycle during a topological traversal). You may assume the input vector of neighbor lists is non-empty and the number of vertices matches the size of the neighbor list.
#include <cassert>
#include <vector>

// The solution function is included here for completeness (or assume it's in a separate file).
// For brevity, we assume assignDomains is defined above.

int main() {
    // Test 1: Simple chain a->b->c, no pre-assigned
    {
        std::vector<std::vector<int>> neighbors = {
            {},      // a no incoming
            {0},     // b from a
            {1}      // c from b
        };
        std::vector<int> initial = {-1, -1, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == -2); // a has no incoming -> deletable
        assert(result[1] == -2); // b has only a's -2 ignored -> deletable
        assert(result[2] == -2);
    }

    // Test 2: Single pre-assigned source feeding two vertices
    {
        std::vector<std::vector<int>> neighbors = {
            {},      // 0 no incoming
            {0},     // 1 from 0
            {0}      // 2 from 0
        };
        std::vector<int> initial = {5, -1, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == 5);
        assert(result[1] == 5);
        assert(result[2] == 5);
    }

    // Test 3: Two distinct incoming domains -> fresh domain
    {
        std::vector<std::vector<int>> neighbors = {
            {},      // 0 no incoming
            {},      // 1 no incoming
            {0, 1}   // 2 from 0 and 1
        };
        std::vector<int> initial = {3, 7, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == 3);
        assert(result[1] == 7);
        assert(result[2] >= 0);
        assert(result[2] != 3 && result[2] != 7);
        assert(result[2] == 1); // smallest positive unused
    }

    // Test 4: Pre-assigned vertex ignores conflicting incoming domains
    {
        std::vector<std::vector<int>> neighbors = {
            {},      // 0
            {},      // 1
            {0, 1}   // 2 from both
        };
        std::vector<int> initial = {3, 7, 10}; // 2 pre-assigned to 10
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[2] == 10); // pre-assigned wins
        assert(result[0] == 3);
        assert(result[1] == 7);
    }

    // Test 5: Vertex with no incoming and pre-assigned stays as is
    {
        std::vector<std::vector<int>> neighbors = {{}, {}, {}};
        std::vector<int> initial = {9, -1, 4};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == 9);
        assert(result[1] == -2); // no incoming -> deletable
        assert(result[2] == 4);
    }

    // Test 6: Multiple vertices merging into same fresh domain
    {
        std::vector<std::vector<int>> neighbors = {
            {}, {}, {},     // 0,1,2 isolated
            {0, 1},         // 3 merges 0 and 1 -> fresh 1
            {2, 3}          // 4 merges 2 and 3 -> fresh 2 (since 1 used)
        };
        std::vector<int> initial = {5, 6, 7, -1, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[3] == 1);
        assert(result[4] == 2); // 7 and 1 mix, so new smallest unused is 2
    }

    // Test 7: Incoming domain -2 (deletable) is ignored
    {
        std::vector<std::vector<int>> neighbors = {
            {},      // 0 no incoming -> -2 by default
            {0}      // 1 from 0 (which is -2)
        };
        std::vector<int> initial = {-1, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == -2);
        assert(result[1] == -2); // only -2 incoming, so deletable
    }

    // Test 8: Complex DAG with preserved domain
    {
        std::vector<std::vector<int>> neighbors = {
            {},        // 0
            {0},       // 1
            {0},       // 2
            {1, 2},    // 3
            {3},       // 4
            {4, 0}     // 5
        };
        std::vector<int> initial = {0, -1, -1, -1, 100, -1};
        std::vector<int> result = assignDomains(neighbors, initial);
        assert(result[0] == 0);
        assert(result[1] == 0);
        assert(result[2] == 0);
        assert(result[3] == 0);   // both from 0
        assert(result[4] == 100); // pre-assigned
        assert(result[5] == 100); // from 4 (100) and 0 (0) -> but 4 is pre-assigned and stays, but for 5 we take both -> fresh
        // Actually 5 has incoming 4 (100) and 0 (0) -> distinct, so fresh 1
        // But we must compute properly: let's just assert it's a fresh domain not equal to 0 or 100
        assert(result[5] == 1);
    }

    return 0;
}
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

/**
 * Assigns a domain value to each vertex of a simplified graph.
 *
 * @param neighbors  For each vertex i, list of source vertex indices that have an edge to i.
 * @param initialDomains  Pre-assigned domains; use -1 for unassigned.
 * @return A vector of assigned domains for each vertex.
 *
 * Domain conventions:
 *   -1 : unassigned (input only)
 *   -2 : deletable (no triggering incoming domain)
 *   >=0 : normal domain identifier
 *
 * The algorithm processes vertices in topological order. For a vertex without a pre-assigned
 * domain, its domain is determined by its incoming neighbors' domains as described in the task.
 */
std::vector<int> assignDomains(const std::vector<std::vector<int>>& neighbors,
                               const std::vector<int>& initialDomains) {
    const int V = static_cast<int>(neighbors.size());
    // Sanity check: both vectors must have same size
    if ((int)initialDomains.size() != V) return {};

    // Compute in-degrees
    std::vector<int> inDegree(V, 0);
    for (int v = 0; v < V; ++v) {
        for (int src : neighbors[v]) {
            // Assume src is a valid vertex index (0 <= src < V)
            inDegree[v]++;
        }
    }

    // Initialize queue with all vertices that have in-degree zero
    std::queue<int> q;
    for (int v = 0; v < V; ++v) {
        if (inDegree[v] == 0) q.push(v);
    }

    // Result vector
    std::vector<int> result(V, -1);

    // Keep a set of all domain values already used (non-negative only)
    std::set<int> usedDomains;
    for (int d : initialDomains) {
        if (d >= 0) usedDomains.insert(d);
    }

    // Function to return the next unused positive domain (smallest positive not in set)
    auto nextFreshDomain = [&usedDomains]() -> int {
        int candidate = 1;
        while (usedDomains.find(candidate) != usedDomains.end()) {
            ++candidate;
        }
        usedDomains.insert(candidate);
        return candidate;
    };

    // Topological processing
    int processed = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        ++processed;

        // Determine domain for v
        if (initialDomains[v] >= 0) {
            // Pre-assigned
            result[v] = initialDomains[v];
        } else {
            // Collect domains from incoming neighbors (they are already processed)
            std::set<int> incomingDomains;
            for (int src : neighbors[v]) {
                int d = result[src];
                // Ignore -1 (shouldn't happen after processing) and -2 (deletable)
                if (d >= 0) {
                    incomingDomains.insert(d);
                }
            }

            if (incomingDomains.empty()) {
                result[v] = -2; // No triggering domain -> deletable
            } else if (incomingDomains.size() == 1) {
                result[v] = *incomingDomains.begin(); // Sole domain
            } else {
                // Multiple distinct domains -> new fresh domain
                result[v] = nextFreshDomain();
            }
        }

        // Note: we do not change the domain of a vertex even if it has a pre-assigned one.
        // Now decrement in-degrees of outgoing neighbors and enqueue if zero.
        // We don't have an adjacency list of outgoing edges, so we must find them by scanning all neighbors.
        for (int u = 0; u < V; ++u) {
            for (int src : neighbors[u]) {
                if (src == v) {
                    // Edge from v to u
                    --inDegree[u];
                    if (inDegree[u] == 0) {
                        q.push(u);
                    }
                }
            }
        }
    }

    // If the graph had cycles, some vertices may remain unprocessed. Assign them -2.
    for (int v = 0; v < V; ++v) {
        if (result[v] == -1) {
            result[v] = -2;
        }
    }

    return result;
}
// The main algorithm processes vertices in topological order so that by the time a vertex is visited, all its incoming neighbors have already been assigned a domain. To achieve this, compute an in-degree count for each vertex (number of incoming edges). Initialize a queue with all vertices whose in-degree is zero. While processing, for each neighbor outgoing from the current vertex, decrement its in-degree; when it becomes zero, push that neighbor into the queue. This ensures we only visit a vertex after all its predecessors have been processed. For each vertex, we first check if it has a pre-assigned domain (non-negative) – if so, we keep it. Otherwise, we scan its incoming neighbor list (which are all already processed in a DAG), collecting their assigned domains. We ignore any domain equal to `-1` or `-2` (unassigned or deletable). If no valid (non-negative) domains are found, assign `-2`. If exactly one unique domain value is found, assign that value. If more than one unique domain value is found, we need a new domain identifier. We maintain a set of all domain values already used (initialized from all pre-assigned domains and also adding new ones as we create them). The new domain is the smallest positive integer not present in that set. Add it to the set and assign it to the vertex. For edge cases: a vertex with pre-assigned domain ignores incoming domain conflicts (per specification). A vertex with a pre-assigned domain but also incoming neighbors is fine; we don’t change it. A vertex with no incoming edges and no pre-assigned domain gets `-2` because nothing triggers it. Time complexity is O(V + E) for topological traversal plus O(V*d) for domain scan where d is max in-degree, so overall O(V+E) if we sum over all vertices (each edge considered once). Space complexity is O(V) for the queue, in-degree array, and used-domain set. We must also handle the possibility of multiple edges between the same pair; we treat each incoming edge separately but duplication doesn't affect the domain logic beyond possibly duplicate values, which we handle by using a set or by checking uniqueness of collected values.
