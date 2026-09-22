// Implement a C++ function that, given two 0-indexed arrays (using 1-based vertex numbering) representing a directed graph's edges and a separate precomputed "finishing time" order (an array of vertex IDs in decreasing order of their finishing times from a prior DFS pass), computes the sizes of the five largest strongly connected components (SCCs) in the graph, returning them in descending order as a `std::vector<long>`. The graph is provided via two parallel vectors: `tails` (source vertex of each edge) and `heads` (destination vertex of each edge). The function must build an adjacency list from the edges, perform a DFS on the graph in the given order (process vertices in the exact sequence of the `finishingOrder` array, treating all unvisited vertices in that sequence), count the number of vertices in each SCC, and return the top five sizes (including zeros if fewer than five SCCs exist). The graph has at most 875,714 vertices, but the function must be general and work for any positive `numVertices`. Assume vertices are labeled from 1 to `numVertices`. The function must not use recursion (to avoid stack overflow on large inputs) and must handle isolated vertices (no edges) correctly.
#include <cassert>
#include <vector>

// Include the solution function here (or rely on linking)

int main() {
    // Test 1: Simple graph with two SCCs: {1,2} and {3} (isolated)
    // Edges: 1->2, 2->1
    std::vector<long> tails1 = {1, 2};
    std::vector<long> heads1 = {2, 1};
    // Finishing order: e.g., vertex 2 then 1 then 3 (any order works for this test)
    std::vector<long> order1 = {2, 1, 3};
    std::vector<long> res1 = fiveLargestSCCs(3, tails1, heads1, order1);
    assert(res1.size() == 5);
    assert(res1[0] == 2 && res1[1] == 1 && res1[2] == 0 && res1[3] == 0 && res1[4] == 0);

    // Test 2: Disconnected vertices, all isolated
    std::vector<long> tails2 = {};
    std::vector<long> heads2 = {};
    std::vector<long> order2 = {5, 4, 3, 2, 1};
    std::vector<long> res2 = fiveLargestSCCs(5, tails2, heads2, order2);
    assert(res2[0] == 1 && res2[1] == 1 && res2[2] == 1 && res2[3] == 1 && res2[4] == 1);

    // Test 3: Single self-loop
    std::vector<long> tails3 = {1};
    std::vector<long> heads3 = {1};
    std::vector<long> order3 = {1};
    std::vector<long> res3 = fiveLargestSCCs(1, tails3, heads3, order3);
    assert(res3[0] == 1 && res3[1] == 0 && res3[2] == 0 && res3[3] == 0 && res3[4] == 0);

    // Test 4: Chain 1->2->3->4->5, each vertex its own SCC
    std::vector<long> tails4 = {1, 2, 3, 4};
    std::vector<long> heads4 = {2, 3, 4, 5};
    std::vector<long> order4 = {5, 4, 3, 2, 1};
    std::vector<long> res4 = fiveLargestSCCs(5, tails4, heads4, order4);
    assert(res4[0] == 1 && res4[1] == 1 && res4[2] == 1 && res4[3] == 1 && res4[4] == 1);

    // Test 5: Larger SCC: 1->2, 2->3, 3->1, plus vertex 4 alone
    std::vector<long> tails5 = {1, 2, 3};
    std::vector<long> heads5 = {2, 3, 1};
    std::vector<long> order5 = {3, 2, 1, 4};
    std::vector<long> res5 = fiveLargestSCCs(4, tails5, heads5, order5);
    assert(res5[0] == 3 && res5[1] == 1 && res5[2] == 0 && res5[3] == 0 && res5[4] == 0);

    // Test 6: Duplicate edges and out-of-range in finishing order
    std::vector<long> tails6 = {1, 1, 2};
    std::vector<long> heads6 = {2, 2, 3};
    std::vector<long> order6 = {3, 2, 1, 99, -1};
    std::vector<long> res6 = fiveLargestSCCs(3, tails6, heads6, order6);
    // SCCs: {1}->{2}->{3}, each single, so top three are 1,1,1
    assert(res6[0] == 1 && res6[1] == 1 && res6[2] == 1 && res6[3] == 0 && res6[4] == 0);

    return 0;
}
#include <vector>
#include <stack>
#include <algorithm>

// Compute the sizes of the five largest strongly connected components (SCCs) in a directed graph.
// The graph is given by edges: each edge from tails[i] to heads[i] (1-based vertex numbers).
// finishingOrder provides vertices in the order of a prior finishing-time pass (decreasing finish times).
// Returns a vector of exactly 5 long values, sorted descending, with zeros for missing SCCs.
std::vector<long> fiveLargestSCCs(
    int numVertices,
    const std::vector<long>& tails,
    const std::vector<long>& heads,
    const std::vector<long>& finishingOrder
) {
    // Build adjacency list (1-based indexing, allocate size numVertices+1)
    std::vector<std::vector<long>> adj(numVertices + 1);
    for (size_t i = 0; i < tails.size(); ++i) {
        long u = tails[i];
        long v = heads[i];
        if (u >= 1 && u <= numVertices && v >= 1 && v <= numVertices) {
            adj[u].push_back(v);
        }
    }

    // Visited markers
    std::vector<bool> visited(numVertices + 1, false);
    std::vector<long> sccSizes;

    // Process vertices in the given finishing order
    for (long v : finishingOrder) {
        if (v < 1 || v > numVertices) continue;  // ignore out-of-range
        if (visited[v]) continue;

        // Iterative DFS from vertex v to count the SCC
        long count = 0;
        std::stack<long> stk;
        stk.push(v);
        visited[v] = true;
        while (!stk.empty()) {
            long cur = stk.top();
            stk.pop();
            ++count;
            for (long next : adj[cur]) {
                if (!visited[next]) {
                    visited[next] = true;
                    stk.push(next);
                }
            }
        }
        sccSizes.push_back(count);
    }

    // Sort sizes descending and take top five (or fill with zeros)
    std::sort(sccSizes.begin(), sccSizes.end(), std::greater<long>());
    std::vector<long> result(5, 0);
    for (int i = 0; i < 5 && i < (int)sccSizes.size(); ++i) {
        result[i] = sccSizes[i];
    }
    return result;
}
// The core problem is a classic Kosaraju-style second pass, but here the finishing order is provided externally, so we only need to perform one DFS pass. The main steps: (1) Build an adjacency list from the given edge arrays, storing for each vertex a vector of its outgoing neighbors. (2) Initialize a `visited` vector of booleans to false. (3) Iterate through the vertices in the exact order given by `finishingOrder` (which contains each vertex ID from 1 to `numVertices` exactly once, though we can safely treat any values, but for correctness we assume it’s a permutation). For each vertex in that order, if not visited, start an iterative DFS (using an explicit stack to avoid recursion depth issues) that counts the number of vertices reachable from that start vertex. That count is the size of one SCC. (4) Collect all SCC sizes into a list, sort in descending order, and take the top five. If fewer than five SCCs, fill the remainder with zeros. Edge cases: The graph may have self-loops, parallel edges, and isolated vertices. The finishing order array may not be a perfect permutation in practice, so we must skip out-of-range indices and repeated visits (the visited array handles duplicates). The iterative DFS must correctly push neighbors and mark visited when popping (or when pushing, but careful to avoid double counting; either is fine as long as we mark when we actually process). Complexity: Building adjacency list takes O(E) time and O(V+E) memory. The DFS visits each vertex and edge once, so O(V+E) time, with O(V) auxiliary space for stacks and visited arrays. The sorting step for all SCC sizes takes O(V log V) in the worst case, but since we only need top five, we could optimize, but simple sorting is acceptable.
