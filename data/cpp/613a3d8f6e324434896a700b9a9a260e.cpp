/*
Write a standalone C++ function named `remapDartIndices` that takes a `std::vector<unsigned int>& oldnew` representing a mapping from old dart indices to new dart indices, and four `std::vector<unsigned int>&` arrays `phi1`, `phi_1`, `phi2`, `phi3` that store dart indices for each dart (0-based). The function must update each array so that for every element `i` (a dart index), if its stored target index `t` is not already equal to `oldnew[t]`, then the element is set to `oldnew[t]`. The transformation must be applied in-place to the four arrays, iterating over all indices from 0 to size-1. This mirrors the logic in the given code snippet's `compactTopoRelations`, but without any external topology classes—just plain vectors. The function should be `const`-correct (i.e., the mapping vector `oldnew` is read-only). Ensure the function handles empty arrays gracefully (no-op) and does not require any special includes beyond `<vector>`. The function must be a free function with the signature exactly: `void remapDartIndices(const std::vector<unsigned int>& oldnew, std::vector<unsigned int>& phi1, std::vector<unsigned int>& phi_1, std::vector<unsigned int>& phi2, std::vector<unsigned int>& phi3);`
*/
#include <vector>

// Remap dart indices in phi arrays according to oldnew mapping.
// For each dart i, if the stored target t satisfies t != oldnew[t],
// set it to oldnew[t]. Applied in-place to all four phi arrays.
void remapDartIndices(const std::vector<unsigned int>& oldnew,
                      std::vector<unsigned int>& phi1,
                      std::vector<unsigned int>& phi_1,
                      std::vector<unsigned int>& phi2,
                      std::vector<unsigned int>& phi3) {
    for (unsigned int i = 0; i < phi1.size(); ++i) {
        unsigned int d_index = phi1[i];
        if (d_index != oldnew[d_index])
            phi1[i] = oldnew[d_index];

        d_index = phi_1[i];
        if (d_index != oldnew[d_index])
            phi_1[i] = oldnew[d_index];

        d_index = phi2[i];
        if (d_index != oldnew[d_index])
            phi2[i] = oldnew[d_index];

        d_index = phi3[i];
        if (d_index != oldnew[d_index])
            phi3[i] = oldnew[d_index];
    }
}
#include <vector>
#include <cassert>

// Declaration of the function under test
void remapDartIndices(const std::vector<unsigned int>& oldnew,
                      std::vector<unsigned int>& phi1,
                      std::vector<unsigned int>& phi_1,
                      std::vector<unsigned int>& phi2,
                      std::vector<unsigned int>& phi3);

int main() {
    // Test 1: Identity mapping – no changes
    std::vector<unsigned int> oldnew_id = {0,1,2,3};
    std::vector<unsigned int> p1 = {1,2,3,0};
    std::vector<unsigned int> pm1 = {3,0,1,2};
    std::vector<unsigned int> p2 = {2,3,0,1};
    std::vector<unsigned int> p3 = {0,1,2,3};
    std::vector<unsigned int> orig1 = p1, origm1 = pm1, orig2 = p2, orig3 = p3;
    remapDartIndices(oldnew_id, p1, pm1, p2, p3);
    assert(p1 == orig1);
    assert(pm1 == origm1);
    assert(p2 == orig2);
    assert(p3 == orig3);

    // Test 2: Swap mapping 0<->1, 2<->3
    std::vector<unsigned int> oldnew_swap = {1,0,3,2};
    p1 = {1,2,3,0};
    pm1 = {3,0,1,2};
    p2 = {2,3,0,1};
    p3 = {0,1,2,3};
    remapDartIndices(oldnew_swap, p1, pm1, p2, p3);
    // Expected: each target t becomes oldnew_swap[t]
    assert(p1 == std::vector<unsigned int>({0,3,2,1}));
    assert(pm1 == std::vector<unsigned int>({2,1,0,3}));
    assert(p2 == std::vector<unsigned int>({3,2,1,0}));
    assert(p3 == std::vector<unsigned int>({1,0,3,2}));

    // Test 3: Mapping with some identities (0->0,1->2,2->1,3->3)
    std::vector<unsigned int> oldnew_mix = {0,2,1,3};
    p1 = {1,2,3,0};
    pm1 = {3,0,1,2};
    p2 = {2,3,0,1};
    p3 = {0,1,2,3};
    remapDartIndices(oldnew_mix, p1, pm1, p2, p3);
    assert(p1 == std::vector<unsigned int>({2,1,3,0}));
    assert(pm1 == std::vector<unsigned int>({3,0,2,1}));
    assert(p2 == std::vector<unsigned int>({1,3,0,2}));
    assert(p3 == std::vector<unsigned int>({0,2,1,3}));

    // Test 4: Empty vectors – should not crash
    std::vector<unsigned int> empty;
    std::vector<unsigned int> e1, em1, e2, e3;
    remapDartIndices(empty, e1, em1, e2, e3);
    assert(e1.empty() && em1.empty() && e2.empty() && e3.empty());

    // Test 5: All remap to a single index (e.g., all become 2)
    std::vector<unsigned int> oldnew_collapse = {2,2,2,2};
    p1 = {0,1,2,3};
    pm1 = {3,2,1,0};
    p2 = {1,0,3,2};
    p3 = {2,3,0,1};
    remapDartIndices(oldnew_collapse, p1, pm1, p2, p3);
    assert(p1 == std::vector<unsigned int>({2,2,2,2}));
    assert(pm1 == std::vector<unsigned int>({2,2,2,2}));
    assert(p2 == std::vector<unsigned int>({2,2,2,2}));
    assert(p3 == std::vector<unsigned int>({2,2,2,2}));

    return 0;
}
// The solution iterates over each dart index `i` from 0 to the size of the `phi1` array (which should match all arrays' sizes). For each of the four permutation arrays, it reads the stored value `d_index` at position `i`, and if `d_index` is not equal to `oldnew[d_index]`, it replaces the stored value with `oldnew[d_index]`. The key insight is the condition `if (d_index != oldnew[d_index])`—this avoids unnecessary writes when the mapping is identity for that index. The algorithm assumes that the mapping `oldnew` is valid for all indices that may appear as stored values; no bounds checking is performed (mirroring the original). Time complexity is O(N) where N is the number of darts (size of arrays), and space complexity is O(1) extra. Edge cases: empty vectors cause the loop to not execute; all arrays must have the same length (assumed); if `oldnew` maps an index to itself, no change occurs. The function processes all four arrays identically, so a helper lambda could be used but a simple loop is sufficient.
