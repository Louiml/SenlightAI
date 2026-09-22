// Given an `N x N` binary matrix represented as a vector of strings (where `'1'` indicates a directed edge from row vertex `i` to column vertex `j`, and `'0'` indicates no edge), write a C++ function `std::vector<int> reconstructOrder(const std::vector<std::string>& adj)` that returns a permutation of the vertices labeled `1` through `N` (inclusive) such that for each vertex `i` (processed from `N-1` down to `0`), the position where label `i+1` is placed is the earliest (smallest index) zero‑entry in the result array that occurs after exactly `outdegree(i)` already‑placed labels have been encountered. More formally, the function must simulate the following reconstruction: maintain a result array of size `N` initialized to zeros. For `i` from `N-1` down to `0`, let `d` be the outdegree of vertex `i` (count of `'1'` in row `i`). Then find the first index `pos` such that the number of non‑zero entries before `pos` is exactly `d` and `result[pos] == 0`, set `result[pos] = i+1`. After placing all `N` labels, decrement the outdegrees of all vertices `j` where `adj[i][j] == '1'` (this step happens after placing label `i+1`). The input matrix is guaranteed to be such that a valid permutation always exists (i.e., the graph is a tournament that is acyclic in the reverse order, though you need not validate this). Return the resulting vector of integers. The function must handle `N` up to 5000.
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above.

int main() {
    // Example 1: N=1, no edges.
    std::vector<std::string> adj1 = {"0"};
    assert(reconstructOrder(adj1) == std::vector<int>({1}));

    // Example 2: N=2, no edges (both outdeg 0).
    std::vector<std::string> adj2 = {"00", "00"};
    assert(reconstructOrder(adj2) == std::vector<int>({2, 1}));

    // Example 3: N=2, edge from 0 to 1.
    std::vector<std::string> adj3 = {"01", "00"};
    // Process i=1: outdeg=0, place label 2 at index 0.
    // Then decrement outdeg of others? no edges from 1.
    // Process i=0: outdeg=1 (edge to 1), now index 1 is zero, seen==1? no,
    // Actually after placing 2 at index0, seen=1 when j=1, d=1, place label1 at index1.
    assert(reconstructOrder(adj3) == std::vector<int>({2, 1}));

    // Example 4: N=3, all edges go from lower index to higher (chain 0->1,0->2,1->2).
    std::vector<std::string> adj4 = {"011", "001", "000"};
    // Process i=2: outdeg=0 -> place 3 at index0.
    // Then decrement outdeg for others? no edges from 2.
    // Process i=1: outdeg=1 (edge to 2) -> after index0 filled, seen=1, index1 is zero, place 2.
    // Then decrement outdeg of vertex2? adj[1][2]=1 so outdeg[2] becomes -1 (ignored).
    // Process i=0: outdeg=2 (edges to 1,2) -> after index0 and index1 filled, seen=2, index2 zero, place 1.
    assert(reconstructOrder(adj4) == std::vector<int>({3, 2, 1}));

    // Example 5: N=4, reverse transitive tournament (i points to all larger j).
    std::vector<std::string> adj5 = {
        "0111",
        "0011",
        "0001",
        "0000"
    };
    // Expected order: place 4 at index0 (outdeg 0), then 3 at index1 (outdeg 1), etc.
    assert(reconstructOrder(adj5) == std::vector<int>({4, 3, 2, 1}));

    // Example 6: N=4, all zero matrix.
    std::vector<std::string> adj6(4, std::string(4, '0'));
    // All outdeg 0, so place labels in decreasing order from left to right.
    assert(reconstructOrder(adj6) == std::vector<int>({4, 3, 2, 1}));

    // Example 7: N=3, fully connected both ways? Not a tournament but test anyway.
    // If all '1', outdeg of i=2 is 2, but after placing labels we need to handle.
    // Since this is not a valid input, we don't test.

    // Example 8: N=5 with a specific pattern to verify logic.
    std::vector<std::string> adj8 = {
        "01000",
        "00100",
        "00010",
        "00001",
        "00000"
    };
    // This is a chain 0->1->2->3->4.
    // Process i=4: outdeg=0, place 5 at index0.
    // i=3: outdeg=1 (to 4), place 4 at index1 (seen=1).
    // i=2: outdeg=1 (to 3), place 3 at index2 (seen=2, d=1? actually after two filled, seen=2 when j=3? we need careful).
    // Since each vertex points only to the next, the order should be [5,4,3,2,1]? Let's compute:
    // After placing 5 at [0], i=3 has outdeg 1 (to 4), so scan: j=0 filled (seen=1), j=1 zero and seen==1? yes, place 4 at 1.
    // Then i=2: outdeg=1 (to 3), but now outdeg[3] was decremented when processing i=4? No, i=4 had no edges. Actually decrement happens after placing i=3 for edge to 4? Wait we decrement after placing i=3 for edges from i=3. So i=2: outdeg=1, scan: j=0 filled (seen=1), j=1 filled (seen=2), j=2 zero but seen==2 not equal d=1, continue, j=3 zero and seen==2? still not 1, so pos? Actually the algorithm's condition is seen==d when hitting a zero. So when d=1, we need the first zero after exactly one filled. That is index 2 (after index0 filled, index1 is also filled, so at index2 seen=2, not equal to 1, so skip, index3 seen=2, skip, index4 seen=2, then no zero? But there is index4? Actually after placing 5 at 0 and 4 at 1, zeros at 2,3,4. For d=1, we need a zero where the number of filled before it is exactly 1. That is index1, but index1 is already filled. So no such zero? But the input is valid? The chain 0->1->2->3->4 yields a valid permutation? Let's check: The algorithm requires for i=2, outdeg=1 (edge to 3). But after placing 4 and 5, the filled are at 0 and 1. The zero positions are 2,3,4. The number of filled before index2 is 2, before index3 is 2, before index4 is 2. None have exactly 1. So this input is not valid for the algorithm's guarantee. So we don't test it.

    return 0;
}
#include <vector>
#include <string>

// Reconstruct the vertex order from a binary adjacency matrix.
// adj[i][j] == '1' means a directed edge from i to j.
// Returns a permutation of labels 1..N such that when processing
// vertices from N-1 down to 0, the position of label i+1 is the
// earliest zero slot with exactly outdegree(i) already filled slots.
std::vector<int> reconstructOrder(const std::vector<std::string>& adj) {
    const int N = static_cast<int>(adj.size());
    std::vector<int> result(N, 0);
    std::vector<int> outdeg(N, 0);

    // Compute initial outdegrees.
    for (int i = 0; i < N; ++i) {
        int cnt = 0;
        for (char ch : adj[i]) {
            cnt += (ch == '1');
        }
        outdeg[i] = cnt;
    }

    // Process vertices from highest index to lowest.
    for (int i = N - 1; i >= 0; --i) {
        const int d = outdeg[i];
        int seen = 0;
        int pos = -1;
        // Find the first zero slot with exactly d filled before it.
        for (int j = 0; j < N; ++j) {
            if (result[j] != 0) {
                ++seen;
            } else {
                if (seen == d) {
                    pos = j;
                    break;
                }
            }
        }
        // The input guarantees a valid position exists.
        result[pos] = i + 1;

        // Remove edges from i to other vertices, decreasing their outdegree.
        for (int j = 0; j < N; ++j) {
            if (adj[i][j] == '1') {
                --outdeg[j];
            }
        }
    }

    return result;
}
// The algorithm processes vertices from `N-1` down to `0`. Maintain an array `result` of size `N` initially zero. For each vertex `i`, compute its current outdegree `d` (the number of remaining outgoing edges, i.e., count of `'1'` in its row). Then scan the result array from left to right, keeping a running count `seen` of how many positions are already filled (non‑zero). When `seen == d` and the current position is still zero, place `i+1` there. After placement, decrement the outdegree of every vertex `j` for which `adj[i][j] == '1'` because those edges are now “removed” (they point to vertices that will be processed later and have already had their labels placed). This greedy placement works because the matrix is guaranteed to represent a tournament that is transitively closed in reverse order; the chosen position is uniquely determined by the number of already‑placed vertices that this vertex points to. The algorithm runs in \(O(N^2)\) time because each vertex’s outdegree is computed initially in \(O(N)\) and we scan the result array for each vertex, also \(O(N)\). Space is \(O(N)\) for the result and outdegree array. Edge cases arise when a vertex has outdegree `0` (then place at the first zero position) or when the first zero position coincides with the count (which always succeeds given the input guarantee). The decrement of outdegrees after placement ensures future vertices see reduced counts.
