// Write a C++ function `int countCycles(const std::vector<int>& permutation)` that takes a permutation of the integers `0` through `n-1` (where `n` is the size of the vector), and returns the number of disjoint cycles in that permutation. A cycle is a sequence of indices `i0 -> i1 -> i2 -> ... -> i0` where `i_{k+1} = permutation[i_k]`. The permutation is guaranteed to be valid (each element appears exactly once, values are in range `[0, n-1]`). The function should be efficient for `n` up to 10^6, and must not modify the input vector.
#include <cassert>
#include <vector>

// Assume countCycles is defined as above.

int main() {
    // Empty permutation
    assert(countCycles({}) == 0);

    // Single element
    assert(countCycles({0}) == 1);

    // Identity permutation: each element maps to itself -> n cycles
    assert(countCycles({0, 1, 2, 3}) == 4);

    // One big cycle: 0->1, 1->2, 2->0
    assert(countCycles({1, 2, 0}) == 1);

    // Mixed cycles: [0->1, 1->0] and [2->2] and [3->4, 4->5, 5->3]
    assert(countCycles({1, 0, 2, 4, 5, 3}) == 3);

    // Two cycles of length 2 each
    assert(countCycles({1, 0, 3, 2}) == 2);

    // Reverse of 5 elements: 0->4, 1->3, 2->2, 3->1, 4->0 -> cycles: (0 4), (1 3), (2) => 3 cycles
    assert(countCycles({4, 3, 2, 1, 0}) == 3);

    // Large cycle of length 7
    assert(countCycles({1, 2, 3, 4, 5, 6, 0}) == 1);

    return 0;
}
#include <vector>
#include <cstddef>

// Count the number of disjoint cycles in a permutation of {0, ..., n-1}.
// The input vector 'permutation' must contain each index exactly once.
int countCycles(const std::vector<int>& permutation) {
    const std::size_t n = permutation.size();
    std::vector<bool> visited(n, false);
    int cycle_count = 0;

    for (std::size_t i = 0; i < n; ++i) {
        if (!visited[i]) {
            // Start a new cycle traversal
            std::size_t current = i;
            while (!visited[current]) {
                visited[current] = true;
                current = static_cast<std::size_t>(permutation[current]);
            }
            ++cycle_count;
        }
    }
    return cycle_count;
}
// The problem is equivalent to counting connected components in a functional graph where each node `i` has an outgoing edge to `permutation[i]`. Since the input is a permutation, every node has exactly one outgoing edge and exactly one incoming edge, so the graph consists only of disjoint cycles—no tails or branching. To count cycles, we can use a visited boolean array of size `n` (initialized to `false`). For each index `i` from `0` to `n-1`, if not visited, we start a traversal: follow `permutation[current]` while marking visited until we return to the starting node (or encounter an already visited node, which can only be the starting node because it's a permutation). Each such traversal counts one cycle. Since each node is visited exactly once across all traversals, the total time is `O(n)` and auxiliary space is `O(n)` for the visited array. Edge cases: `n=0` (empty permutation → 0 cycles), `n=1` (single fixed point → 1 cycle), and a permutation that is already sorted (each element maps to itself → `n` cycles). The algorithm does not require recursion, avoiding stack overflow for large `n`.
