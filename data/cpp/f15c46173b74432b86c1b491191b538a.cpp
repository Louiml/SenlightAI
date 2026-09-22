// Write a C++ function `containsOverlappingRange` that takes a vector of pairs of 64-bit unsigned integers, where each pair represents an inclusive range `[start, end]`, and a query range `[queryStart, queryEnd]`, and returns `true` if any range in the vector overlaps with the query range (i.e., they share at least one common value), otherwise returns `false`. The input ranges may be unsorted, may contain duplicates, and must have `start <= end`. The function should handle the full 64-bit unsigned integer domain (including values near the maximum), and should not modify the input vector. The implementation must be efficient for large numbers of ranges, using a balanced binary search tree approach to support range intersection queries. The function should return `false` if the vector is empty, and should treat a range that exactly touches the query at a single boundary point (e.g., `[5,5]` and `[5,10]`) as overlapping.
// The core problem is to determine if any stored range intersects a given query range. A brute-force linear scan over all ranges would take O(m) per query where m is the number of ranges. Instead, we can build a balanced BST (e.g., `std::map` or `std::set` of pairs) that stores ranges sorted by their start value. For each query, we need to find if there exists a stored range `[s, e]` such that `s <= queryEnd` and `e >= queryStart`. This is a classic interval stabbing query. Using `std::map<uint64_t, uint64_t>` mapping start to the maximum end seen so far for any range with that start (or overall), we can store ranges and for queries, we can use `lower_bound` to find the first range with start >= queryStart, then check both that range and the one before it (since a range with a smaller start could overlap at queryStart). However, to correctly handle cases where a range with a smaller start but a large end exists, we need to maintain the maximum end for each start and also consider ranges that start before queryStart. A robust approach: build a map from start to the maximum end among all ranges with that start. Then for a query, find the iterator to the first range with start > queryStart (using `upper_bound`), then check that range (if it exists and its start <= queryEnd) and also check the previous iterator (which has start <= queryStart) to see if its end >= queryStart. This works because any range that overlaps must either start at or before queryStart (in which case we need the maximum end among such ranges) or start after queryStart but at or before queryEnd (in which case we check the first such range). But we need to consider all ranges that start between queryStart and queryEnd. Since we are using a map keyed by start, we can use `lower_bound(queryStart)` to get the first range with start >= queryStart. But there could be multiple ranges with the same start and different ends; we store only the maximum end for each start, which is sufficient because if any range with that start overlaps, the one with the maximum end also overlaps. However, for ranges with start < queryStart, we need the maximum end among all such starts. So we can precompute a prefix maximum: for each start key in sorted order, store the maximum end seen from the beginning up to that key. Then for a query, we find the largest start key <= queryStart (using `upper_bound` and going back one) and check its prefix max end >= queryStart. Also, we find the first start key >= queryStart and check if that start (and possibly subsequent ones, but the first one suffices because if the first start > queryEnd, no later start can overlap) is <= queryEnd. So we only need to check at most two candidate ranges. This gives O(log m) per query after building the structure in O(m log m) time. Edge cases: empty vector, singleton ranges, ranges that touch at boundaries, query range smaller than a single integer, and overflow near UINT64_MAX. For the solution, we can build the map and prefix maxima on each call (or we could assume the function is called once with all ranges; the task says "takes a vector of pairs ... and a query range", so we build the structure inside the function). Time complexity O(m log m) for building and O(log m) for each query if multiple queries were done, but since we only have one query, it's O(m log m) build + O(log m) query. Space O(m).
#include <cstdint>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

// Returns true if any range in `ranges` overlaps with [queryStart, queryEnd].
bool containsOverlappingRange(const std::vector<std::pair<uint64_t, uint64_t>>& ranges,
                              uint64_t queryStart, uint64_t queryEnd) {
    if (ranges.empty()) {
        return false;
    }

    // Map from start value to the maximum end among all ranges with that start.
    std::map<uint64_t, uint64_t> startToMaxEnd;
    for (const auto& rg : ranges) {
        // Assume rg.first <= rg.second.
        auto it = startToMaxEnd.find(rg.first);
        if (it == startToMaxEnd.end()) {
            startToMaxEnd[rg.first] = rg.second;
        } else {
            it->second = std::max(it->second, rg.second);
        }
    }

    // Build a prefix maximum array: for each start key, the maximum end from the
    // smallest start up to this start. We'll store this in a separate map for
    // efficient lower_bound/upper_bound queries.
    std::map<uint64_t, uint64_t> prefixMaxEnd;
    uint64_t currentMax = 0;
    for (const auto& kv : startToMaxEnd) {
        currentMax = std::max(currentMax, kv.second);
        prefixMaxEnd[kv.first] = currentMax;
    }

    // Case 1: A range with start <= queryStart. We need the maximum end among such ranges.
    auto itPrev = prefixMaxEnd.upper_bound(queryStart);
    if (itPrev != prefixMaxEnd.begin()) {
        --itPrev;
        if (itPrev->second >= queryStart) {
            return true;
        }
    }

    // Case 2: A range with start >= queryStart. Check if its start is within the query.
    auto itNext = prefixMaxEnd.lower_bound(queryStart);
    if (itNext != prefixMaxEnd.end() && itNext->first <= queryEnd) {
        return true;
    }

    // Also, there might be a range with start > queryStart but <= queryEnd that
    // is not the first such range? Actually the first one with start >= queryStart
    // is the smallest start in that set; if it > queryEnd, no larger start can be
    // <= queryEnd, so it's sufficient. Similarly, for case 1, if the max end from
    // all starts <= queryStart is < queryStart, no range starting earlier can overlap.
    return false;
}
#include <cassert>
#include <cstdint>
#include <vector>
#include <utility>

// Declaration of the function under test
bool containsOverlappingRange(const std::vector<std::pair<uint64_t, uint64_t>>& ranges,
                              uint64_t queryStart, uint64_t queryEnd);

int main() {
    // Basic overlap
    std::vector<std::pair<uint64_t, uint64_t>> r1 = {{10, 20}, {30, 40}};
    assert(containsOverlappingRange(r1, 15, 25) == true);
    assert(containsOverlappingRange(r1, 5, 9) == false);
    assert(containsOverlappingRange(r1, 25, 30) == true); // boundary touch at 30
    assert(containsOverlappingRange(r1, 21, 29) == false);

    // Empty vector
    std::vector<std::pair<uint64_t, uint64_t>> empty;
    assert(containsOverlappingRange(empty, 0, 100) == false);

    // Single range exact match
    std::vector<std::pair<uint64_t, uint64_t>> r2 = {{5, 5}};
    assert(containsOverlappingRange(r2, 5, 5) == true);
    assert(containsOverlappingRange(r2, 4, 4) == false);
    assert(containsOverlappingRange(r2, 4, 5) == true);

    // Range covering full 64-bit domain
    std::vector<std::pair<uint64_t, uint64_t>> r3 = {{0, UINT64_MAX}};
    assert(containsOverlappingRange(r3, 12345, 67890) == true);
    assert(containsOverlappingRange(r3, UINT64_MAX, UINT64_MAX) == true);

    // Overlap at the very end of range domain
    std::vector<std::pair<uint64_t, uint64_t>> r4 = {{UINT64_MAX - 10, UINT64_MAX}};
    assert(containsOverlappingRange(r4, UINT64_MAX - 5, UINT64_MAX) == true);
    assert(containsOverlappingRange(r4, UINT64_MAX - 20, UINT64_MAX - 11) == false);

    // Multiple ranges with same start
    std::vector<std::pair<uint64_t, uint64_t>> r5 = {{10, 15}, {10, 20}, {10, 12}};
    assert(containsOverlappingRange(r5, 18, 19) == true);
    assert(containsOverlappingRange(r5, 21, 22) == false);

    // Unsorted and duplicate ranges
    std::vector<std::pair<uint64_t, uint64_t>> r6 = {{30, 40}, {10, 20}, {10, 20}};
    assert(containsOverlappingRange(r6, 19, 19) == true);
    assert(containsOverlappingRange(r6, 41, 50) == false);

    // Query range larger than all ranges
    std::vector<std::pair<uint64_t, uint64_t>> r7 = {{100, 110}, {200, 210}};
    assert(containsOverlappingRange(r7, 0, 99) == false);
    assert(containsOverlappingRange(r7, 0, 100) == true);
    assert(containsOverlappingRange(r7, 211, UINT64_MAX) == false);
    assert(containsOverlappingRange(r7, 210, UINT64_MAX) == true);

    return 0;
}
