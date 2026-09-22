/*
Write a standalone C++ function `countUniqueOffsets` that analyzes a set of "load" operations, where each load is represented as a simple struct containing a base pointer (as a `const void*`), an `int64_t` offset, and a boolean indicating whether the load has a tied input constraint. The function must simulate the core clustering logic from LLVM's `ClusterNeighboringLoads`: for each unique base pointer, it should collect all distinct offsets from loads sharing that base, sort the offsets in increasing order, and then count how many consecutive loads (starting from the smallest offset) can be clustered together, subject to the rule that loads with a tied input are never eligible to be clustered, and that the maximum allowed "distance" between consecutive offsets in the cluster is 16 (i.e., the absolute difference between any two consecutive offsets in the selected prefix must be ≤ 16). A base pointer with fewer than 2 eligible loads contributes 0 to the result. The function should return the total number of loads that can be clustered across all base pointers (counting each load once if it belongs to a valid cluster prefix for its base). If a base has eligible loads at offsets [0, 8, 24], then [0,8] cluster (difference 8 ≤ 16) but 24 is too far from 8 (difference 16 is allowed, actually ≤ 16 so it would be included; use > 16 as the break condition), so adjust: the cluster includes offsets 0, 8, and 24 if differences are 8 and 16, both allowed; a load at 41 would break. The function must handle empty input, duplicate offsets (only one load per offset per base), and multiple bases independently. Provide a standalone implementation with no external dependencies beyond standard headers.
*/
#include <vector>
#include <map>
#include <set>
#include <cstdint>
#include <cstddef>

// Simple representation of a load operation for this task.
struct Load {
    const void* base;  // memory base pointer
    int64_t offset;    // byte offset from base
    bool tied_input;   // true if this load has a tied input constraint
};

// Returns the total number of loads that can be clustered across all base pointers.
// Clustering rule: For each unique base, collect distinct offsets from eligible loads
// (loads without tied input). Sort offsets ascending. Starting from the smallest,
// include a consecutive offset if the difference from the previous included offset
// is <= 16. Stop at the first gap > 16. Count the number of included loads for that base.
int countUniqueOffsets(const std::vector<Load>& loads) {
    // Map from base pointer to a set of offsets (ensures uniqueness and sorting).
    std::map<const void*, std::set<int64_t>> baseOffsets;

    for (const auto& load : loads) {
        if (load.tied_input) {
            continue;  // skip loads with tied input; they cannot be clustered
        }
        // Insert the offset into the set for this base.
        baseOffsets[load.base].insert(load.offset);
    }

    int totalClustered = 0;
    for (const auto& entry : baseOffsets) {
        const auto& offsets = entry.second;
        // Need at least 2 offsets to form a cluster.
        if (offsets.size() < 2) {
            continue;
        }
        // Walk the sorted offsets, count the longest prefix where consecutive
        // differences are <= 16.
        int count = 1;  // always include the first offset
        auto it = offsets.begin();
        int64_t prev = *it;
        ++it;
        while (it != offsets.end()) {
            int64_t curr = *it;
            if (curr - prev <= 16) {
                ++count;
                prev = curr;
                ++it;
            } else {
                break;  // gap too large, stop.
            }
        }
        totalClustered += count;
    }
    return totalClustered;
}
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is assumed to be included above.
int main() {
    // Case 1: Single base with offsets 0, 6, 22, 40. Differences: 6, 16, 18.
    // Cluster includes 0, 6, 22 (6<=16, 22-6=16<=16), stops at 40 (18>16). Count = 3.
    intptr_t base1 = 0x1000;
    std::vector<Load> loads1 = {
        {reinterpret_cast<void*>(base1), 0, false},
        {reinterpret_cast<void*>(base1), 6, false},
        {reinterpret_cast<void*>(base1), 22, false},
        {reinterpret_cast<void*>(base1), 40, false}
    };
    assert(countUniqueOffsets(loads1) == 3);

    // Case 2: Tied inputs are excluded entirely.
    intptr_t base2 = 0x2000;
    std::vector<Load> loads2 = {
        {reinterpret_cast<void*>(base2), 0, false},
        {reinterpret_cast<void*>(base2), 8, true},   // tied, excluded
        {reinterpret_cast<void*>(base2), 16, false},
        {reinterpret_cast<void*>(base2), 32, false}
    };
    // Eligible offsets: 0,16,32. Differences: 16,16, all within limit. Count=3.
    assert(countUniqueOffsets(loads2) == 3);

    // Case 3: Duplicate offsets for same base are deduplicated.
    intptr_t base3 = 0x3000;
    std::vector<Load> loads3 = {
        {reinterpret_cast<void*>(base3), 5, false},
        {reinterpret_cast<void*>(base3), 5, false},  // duplicate
        {reinterpret_cast<void*>(base3), 5, false},  // duplicate
        {reinterpret_cast<void*>(base3), 21, false}
    };
    // Eligible distinct offsets: 5,21. Difference 16 <= 16, count=2.
    assert(countUniqueOffsets(loads3) == 2);

    // Case 4: Bases with only one eligible offset don't contribute.
    intptr_t base4 = 0x4000;
    intptr_t base5 = 0x5000;
    std::vector<Load> loads4 = {
        {reinterpret_cast<void*>(base4), 0, false},
        {reinterpret_cast<void*>(base5), 10, false}
    };
    // Each base has only 1 offset, so total = 0.
    assert(countUniqueOffsets(loads4) == 0);

    // Case 5: Multiple bases with clusters.
    intptr_t base6 = 0x6000;
    intptr_t base7 = 0x7000;
    std::vector<Load> loads5 = {
        {reinterpret_cast<void*>(base6), 0, false},
        {reinterpret_cast<void*>(base6), 12, false},
        {reinterpret_cast<void*>(base7), 100, false},
        {reinterpret_cast<void*>(base7), 115, false},
        {reinterpret_cast<void*>(base7), 130, false}
    };
    // base6: offsets 0,12 -> count=2. base7: offsets 100,115,130; differences 15,15 -> count=3. Total=5.
    assert(countUniqueOffsets(loads5) == 5);

    // Case 6: Empty input.
    std::vector<Load> loads6;
    assert(countUniqueOffsets(loads6) == 0);

    // Case 7: All loads have tied input, nothing clustered.
    intptr_t base8 = 0x8000;
    std::vector<Load> loads7 = {
        {reinterpret_cast<void*>(base8), 0, true},
        {reinterpret_cast<void*>(base8), 1, true}
    };
    assert(countUniqueOffsets(loads7) == 0);

    return 0;
}
// The solution iterates over all loads and groups them by base pointer. For each base, we collect unique offsets from loads that do not have a tied input. If a base has fewer than 2 distinct eligible offsets, skip it. Sort the offsets in increasing order. Then walk the sorted list starting from the first offset, and for each consecutive pair (current, next), check if `next - current <= 16`. If the difference exceeds 16, stop and count only the prefix up to and including the current offset; otherwise continue. The count for that base is the number of offsets in that prefix (since each offset maps to exactly one load, and we assumed uniqueness). Edge cases: duplicate offsets for the same base should be deduplicated (e.g., using a `std::set` or `std::unordered_map` with a set), and tied-input loads are excluded from the eligible set. If the base has exactly 1 eligible offset, no cluster is formed, so count 0. The total is the sum over all bases. Time complexity is O(N log N) overall due to sorting offsets per base, where N is the number of loads, and space complexity is O(N) for the grouping and storage of offsets.
