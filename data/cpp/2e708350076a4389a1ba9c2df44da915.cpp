// Given a directed graph represented as an \( N \times N \) adjacency matrix where `1` indicates a direct edge from node `i` to node `j`, write a C++ function named `computeTransitiveClosure` that takes a `std::vector<std::vector<int>>` as input and returns a new matrix (same dimensions) where `result[i][j]` is `1` if there exists a path (direct or indirect) from node `i` to node `j`, otherwise `0`. The nodes are 0-indexed. The input is guaranteed to be a square matrix with entries only `0` or `1`. The diagonal may contain `0` (no self-loop) or `1` (self-loop). The function must not modify the input matrix. Apply `const` correctness and use the Floyd–Warshall algorithm variant for reachability.

// The core algorithm is an adaptation of Floyd–Warshall to compute the transitive closure: for every intermediate vertex `k`, we check for all pairs `(i, j)` whether there is an existing direct path `i → j` or whether `i` can reach `k` and `k` can reach `j`. If either condition holds, set `result[i][j] = 1`. This is an iterative improvement where after processing all `k`, every reachable pair is marked. Important edge cases include: (1) input matrix with all zeros (no edges), in which case the output remains all zeros; (2) self-loops if present in input are preserved; (3) multiple paths don't affect result, only existence; (4) a path of length zero (i == j) is considered reachable only if the original matrix has a `1` on the diagonal — the algorithm will not add self-loops incorrectly because it only uses existing adjacency values. Time complexity is \(O(N^3)\) due to three nested loops, and space complexity is \(O(N^2)\) for the output copy. We copy the input to avoid modifying it, then run the algorithm.

#include <vector>

// Compute the transitive closure of a directed graph given as an adjacency matrix.
// result[i][j] = 1 if there is a path from i to j (direct or indirect), else 0.
std::vector<std::vector<int>> computeTransitiveClosure(const std::vector<std::vector<int>>& graph) {
    const std::size_t n = graph.size();
    // Copy the input to preserve it.
    std::vector<std::vector<int>> reachable = graph; // deep copy

    // Floyd–Warshall style transitive closure.
    for (std::size_t k = 0; k < n; ++k) {
        for (std::size_t i = 0; i < n; ++i) {
            for (std::size_t j = 0; j < n; ++j) {
                if (reachable[i][k] == 1 && reachable[k][j] == 1) {
                    reachable[i][j] = 1;
                }
            }
        }
    }
    return reachable;
}

#include <cassert>
#include <vector>
#include <iostream>

// Forward declaration of the solution function.
std::vector<std::vector<int>> computeTransitiveClosure(const std::vector<std::vector<int>>& graph);

int main() {
    // Test 1: Simple 2-node graph with direct edge 0->1.
    std::vector<std::vector<int>> g1 = {{0,1},{0,0}};
    auto r1 = computeTransitiveClosure(g1);
    assert((r1 == std::vector<std::vector<int>>{{0,1},{0,0}}));

    // Test 2: 3-node chain 0->1->2 gives path 0->2.
    std::vector<std::vector<int>> g2 = {{0,1,0},{0,0,1},{0,0,0}};
    auto r2 = computeTransitiveClosure(g2);
    assert((r2 == std::vector<std::vector<int>>{{0,1,1},{0,0,1},{0,0,0}}));

    // Test 3: Self-loop present.
    std::vector<std::vector<int>> g3 = {{1,0},{0,0}};
    auto r3 = computeTransitiveClosure(g3);
    assert((r3 == std::vector<std::vector<int>>{{1,0},{0,0}}));

    // Test 4: Cycle 0<->1 makes both reachable.
    std::vector<std::vector<int>> g4 = {{0,1},{1,0}};
    auto r4 = computeTransitiveClosure(g4);
    assert((r4 == std::vector<std::vector<int>>{{1,1},{1,1}}));

    // Test 5: All zeros.
    std::vector<std::vector<int>> g5 = {{0,0,0},{0,0,0},{0,0,0}};
    auto r5 = computeTransitiveClosure(g5);
    assert((r5 == g5));

    // Test 6: 1-node graph with no self-loop.
    std::vector<std::vector<int>> g6 = {{0}};
    auto r6 = computeTransitiveClosure(g6);
    assert((r6 == std::vector<std::vector<int>>{{0}}));

    // Test 7: Complete graph without self-loops.
    std::vector<std::vector<int>> g7 = {{0,1,1},{1,0,1},{1,1,0}};
    auto r7 = computeTransitiveClosure(g7);
    assert((r7 == std::vector<std::vector<int>>{{1,1,1},{1,1,1},{1,1,1}}));

    // Test 8: Ensure input is not modified.
    std::vector<std::vector<int>> g8 = {{0,1},{0,0}};
    auto r8 = computeTransitiveClosure(g8);
    assert(g8[0][0] == 0 && g8[0][1] == 1 && g8[1][0] == 0 && g8[1][1] == 0);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
