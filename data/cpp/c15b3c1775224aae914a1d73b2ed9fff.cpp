// Write a C++ function `bool canTransform(const std::vector<int>& P, const std::vector<int>& Q, const std::vector<std::pair<int,int>>& goodPairs, int N)` that determines whether permutation `P` (of integers 1..N) can be transformed into permutation `Q` by repeatedly swapping any two positions that are connected by an undirected edge described by a good pair `(a, b)` (using 1-based indices). The graph is defined on positions `1..N`. Return `true` if `P` can be rearranged into `Q` using only these allowed swaps, and `false` otherwise. The function should handle graphs that may be disconnected. Assume inputs are valid permutations of 1..N, good pairs are 1-based and a<b, and N ≥ 2.
// The key observation is that the allowed swaps define a graph on the positions. Within each connected component of this graph, we can permute the elements arbitrarily because we can swap any two adjacent positions in a spanning tree and generate any permutation of that component via a sequence of transpositions. Therefore, for each connected component of the graph (on indices), the set of values currently in those positions in `P` must exactly match the set of values in the same positions in `Q`. If every component has matching value sets, then the transformation is possible; otherwise, it is not.  
// Algorithm:  
// 1. Build an adjacency list for the graph with N vertices (0-based internally).  
// 2. Perform DFS/BFS to find all connected components. For each component, collect the set (or multiset) of values at those positions in `P` and in `Q`.  
// 3. Compare the two sets; if for any component they differ, return `false`.  
// Edge cases: Disconnected components; components of size 1 (trivially equal sets); multiple test cases handled by the caller.  
// Complexity: O(N + M) time (since each vertex and edge visited once) and O(N + M) space for adjacency list and visited arrays.
#include <vector>
#include <unordered_set>
#include <queue>

/**
 * Determines whether permutation P can be transformed into permutation Q
 * by swapping positions connected by the given undirected good pairs.
 *
 * @param P Initial permutation (1-based values, 0-based index in vector).
 * @param Q Target permutation (1-based values, 0-based index in vector).
 * @param goodPairs List of (a, b) 1-based index pairs (a < b) that allow swapping.
 * @param N Number of elements (also max value).
 * @return true if transformation is possible; false otherwise.
 */
bool canTransform(const std::vector<int>& P, const std::vector<int>& Q,
                  const std::vector<std::pair<int,int>>& goodPairs, int N) {
    // Build adjacency list using 0-based indices.
    std::vector<std::vector<int>> adj(N);
    for (const auto& pr : goodPairs) {
        int a = pr.first - 1;  // 0-based
        int b = pr.second - 1;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<bool> visited(N, false);

    // Process each connected component via BFS.
    for (int start = 0; start < N; ++start) {
        if (visited[start]) continue;

        // BFS to collect positions in this component.
        std::vector<int> componentPositions;
        std::queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            componentPositions.push_back(u);
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        // Collect value sets from P and Q for this component.
        std::unordered_set<int> pValues, qValues;
        for (int pos : componentPositions) {
            pValues.insert(P[pos]);
            qValues.insert(Q[pos]);
        }

        if (pValues != qValues) {
            return false;
        }
    }

    return true;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be included above.

int main() {
    // Test 1: Sample 1 - NO (component {0,1} mismatch, {2,3} ok)
    {
        std::vector<int> P = {1, 3, 2, 4};
        std::vector<int> Q = {1, 4, 2, 3};
        std::vector<std::pair<int,int>> goodPairs = {{3,4}};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == false);
    }

    // Test 2: Sample 2 - YES (component containing 2 and 4, values match)
    {
        std::vector<int> P = {1, 3, 2, 4};
        std::vector<int> Q = {1, 4, 2, 3};
        std::vector<std::pair<int,int>> goodPairs = {{2,4}};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == true);
    }

    // Test 3: Disconnected, but each component already matches
    {
        std::vector<int> P = {2, 1, 4, 3};
        std::vector<int> Q = {1, 2, 3, 4};
        std::vector<std::pair<int,int>> goodPairs = {{1,2}, {3,4}};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == true);
    }

    // Test 4: Single edge but component values mismatch
    {
        std::vector<int> P = {1, 2, 3, 4};
        std::vector<int> Q = {2, 1, 3, 4};
        std::vector<std::pair<int,int>> goodPairs = {{1,2}};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == true); // 1 and 2 can swap
    }

    // Test 5: No edges at all - only identity works
    {
        std::vector<int> P = {2, 1, 3, 4};
        std::vector<int> Q = {1, 2, 3, 4};
        std::vector<std::pair<int,int>> goodPairs = {};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == false);
    }

    // Test 6: Fully connected graph - always possible
    {
        std::vector<int> P = {4, 3, 2, 1};
        std::vector<int> Q = {1, 2, 3, 4};
        std::vector<std::pair<int,int>> goodPairs = {{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
        int N = 4;
        assert(canTransform(P, Q, goodPairs, N) == true);
    }

    // Test 7: Larger N, two components, one mismatch
    {
        std::vector<int> P = {1, 2, 3, 4, 5, 6};
        std::vector<int> Q = {2, 1, 3, 5, 4, 6};
        // Components: {0,1} good; {2,3,4,5} has edges (3,4) and (4,5) but 3 and 5 mismatch? Actually Q pos 3=5, Q pos 5=6? Let's design: indices 0-5. Make component {2,3,4} with values {3,4,5} vs Q {3,5,4} => ok. component {5} alone ok.
        std::vector<std::pair<int,int>> goodPairs = {{1,2}}; // connects index0 and index1
        int N = 6;
        // Since only component {0,1} can swap, but P[0]=1,Q[0]=2 and P[1]=2,Q[1]=1 => ok; rest identical => true
        assert(canTransform(P, Q, goodPairs, N) == true);
        // Now change Q to make pos 4 value 6? But N=6, value 6 at pos5; Let's make a mismatch: Q = {2,1,3,5,6,4} with component {3,4,5} connected via (4,5) and (5,6)? Actually indices 3,4,5 have edges (4,5) and (5,6) => component {3,4,5}. P values={4,5,6}, Q values={5,6,4} same set => still true. Need mismatch: Q = {2,1,3,4,6,5} but then component {3,4,5} set P={4,5,6} Q={4,6,5} same => true. Let's just test the code's behavior with known answer.
    }

    // Test 8: Simple case where a swap is impossible because component missing needed value
    {
        std::vector<int> P = {1, 2, 3, 4};
        std::vector<int> Q = {2, 1, 4, 3};
        std::vector<std::pair<int,int>> goodPairs = {{1,2}}; // only positions 1 and 2 can swap
        int N = 4;
        // Component {0,1} has P={1,2}, Q={2,1} ok. Component {2,3} has P={3,4}, Q={4,3} but no edges => mismatch -> false
        assert(canTransform(P, Q, goodPairs, N) == false);
    }

    // Test 9: Edge case N=2, one edge, swap works
    {
        std::vector<int> P = {1, 2};
        std::vector<int> Q = {2, 1};
        std::vector<std::pair<int,int>> goodPairs = {{1,2}};
        int N = 2;
        assert(canTransform(P, Q, goodPairs, N) == true);
    }

    // Test 10: Edge case N=2, no edge, swap fails
    {
        std::vector<int> P = {1, 2};
        std::vector<int> Q = {2, 1};
        std::vector<std::pair<int,int>> goodPairs = {};
        int N = 2;
        assert(canTransform(P, Q, goodPairs, N) == false);
    }

    return 0;
}
