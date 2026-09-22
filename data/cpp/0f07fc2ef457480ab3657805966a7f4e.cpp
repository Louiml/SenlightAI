Given a positive integer `n` followed by a permutation of the integers `1` through `n`, write a C++ function `int countCycles(int n, const std::vector<int>& perm)` that returns the number of cycles in the permutation. The permutation is provided as a 0-indexed vector where `perm[i]` is the value (1-indexed) that element `i` maps to. For example, if `n = 4` and `perm = {2, 3, 1, 4}`, the mapping is `0→2, 1→3, 2→1, 3→4`, which forms cycles `(0,2,1)` and `(3)` — so the answer is `2`. The function must use a disjoint-set (union-find) structure to count the cycles by merging each element with its target, then counting the roots. The input will always be a valid permutation, and `n` is at least 1. If `n == 1`, the answer is always `1` because the single element is its own cycle.
#include <cassert>
#include <vector>
#include <functional>

// (Include the solution function here)

int main() {
    // Single element: one cycle
    assert(countCycles(1, {1}) == 1);

    // Two-element swap: one cycle (0→1, 1→0) becomes (0,1)
    assert(countCycles(2, {2, 1}) == 1);

    // Two elements fixed: two cycles
    assert(countCycles(2, {1, 2}) == 2);

    // Three elements: one 3-cycle (0→2→1→0)
    assert(countCycles(3, {2, 3, 1}) == 1);

    // Three elements: two cycles (0,1) and (2)
    assert(countCycles(3, {2, 1, 3}) == 2);

    // Four elements: one 2-cycle and two fixed points → 3 cycles
    assert(countCycles(4, {3, 2, 1, 4}) == 3);

    // Four elements: identity permutation → 4 cycles
    assert(countCycles(4, {1, 2, 3, 4}) == 4);

    // Five elements: (0,4,2,1) and (3) → 2 cycles
    assert(countCycles(5, {5, 3, 4, 4, 1}) == 2);

    // Invalid-ish? But given permutation, test a large cycle
    assert(countCycles(6, {6, 5, 4, 3, 2, 1}) == 3); // (0,6)(1,5)(2,4) and (3) → actually 4 cycles? Let's verify: 
    // For {6,5,4,3,2,1}: 0→5,1→4,2→3,3→2,4→1,5→0 => cycles (0,5) and (1,4) and (2,3) => 3 cycles
    // So the assertion should be 3, not 4. Let's fix.
    // (Removed incorrect assertion, replaced below)
    assert(countCycles(6, {6, 5, 4, 3, 2, 1}) == 3);

    return 0;
}
#include <vector>
#include <numeric>

// Count the number of cycles in a permutation represented by a 0-indexed vector.
// Each element perm[i] is a 1-indexed value to which i maps.
int countCycles(int n, const std::vector<int>& perm) {
    // Parent array for disjoint-set (union-find)
    std::vector<int> parent(n);
    std::iota(parent.begin(), parent.end(), 0);

    // Recursive find with path compression
    std::function<int(int)> find = [&](int u) -> int {
        if (parent[u] != u) {
            parent[u] = find(parent[u]);
        }
        return parent[u];
    };

    // Union each element with its target
    for (int i = 0; i < n; ++i) {
        int rootI = find(i);
        int rootTarget = find(perm[i] - 1); // convert to 0-based
        if (rootI != rootTarget) {
            parent[rootI] = rootTarget;
        }
    }

    // Count the number of roots (each root corresponds to one cycle)
    int cycles = 0;
    for (int i = 0; i < n; ++i) {
        if (find(i) == i) {
            ++cycles;
        }
    }
    return cycles;
}
// The problem is to count the number of cycles in a permutation using a union-find data structure. For each index `i` (0-based) and its mapped value `x = perm[i] - 1` (converting to 0-based), we union the set containing `i` with the set containing `x`. Initially, each element is its own parent (`f[i] = i` for all `i`). After processing all `n` elements, the number of cycles equals the number of distinct roots in the union-find forest, i.e., the number of indices `i` such that `find(i) == i`. Edge cases: when `n = 1`, the loop unions `0` with itself, resulting in one root, so the answer is `1`. Since the input is guaranteed to be a valid permutation, no element is missing or duplicated, so no special handling for invalid input is needed. The algorithm runs in nearly linear time: each union-find operation with path compression has amortized complexity \(O(\alpha(n))\), where \(\alpha\) is the inverse Ackermann function, and the total time is \(O(n \alpha(n))\), which is effectively \(O(n)\). Space complexity is \(O(n)\) for the parent array. Path compression is applied in `find` to keep the structure flat, ensuring efficient subsequent queries.
