Given a list of `n` intervals `[a_i, b_i]` (with `a_i < b_i`), write a C++ function that returns the number of **groups** the intervals form under the following rule: Two intervals belong to the same group if their coordinate ranges overlap at a common integer point (i.e., there exists an integer `x` such that `a_i <= x <= b_i` and `a_j <= x <= b_j`). In other words, each interval is a continuous segment on the integer line, and all intervals that touch (share at least one integer coordinate) are placed in the same connected component. The function should process the intervals in **strictly increasing order of all endpoint coordinates** (both left and right), but when multiple events occur at the same coordinate, all right endpoints at that coordinate are processed before any left endpoints at that same coordinate (so that touching at a point is not broken). The function returns the total number of such connected components across all intervals. The input intervals are given as two parallel vectors `starts` and `ends` (each of size `n`), where `starts[i]` is the left endpoint and `ends[i]` is the right endpoint. The intervals are not necessarily sorted, and there may be duplicates. The function must return an integer representing the number of connected components.
The solution processes all 2n endpoints (left and right) as events. Sort all events by coordinate; for events with the same coordinate, process all right endpoints first (since a right endpoint at coordinate x closes an interval that covers x, and a left endpoint at the same x opens a new interval that also covers x, so they should be considered touching at that point). Maintain a set of "active" intervals (those whose left endpoint has been seen but whose right endpoint has not yet been processed). Also maintain a set `completed` that stores intervals whose entire component has already been finalized. When processing a left endpoint, add the interval to the active set. When processing a right endpoint (after grouping all events at the same coordinate), examine the active set: all intervals that have a right endpoint at this coordinate, plus any other intervals still active (which must have a left endpoint earlier than this coordinate and a right endpoint later than or equal to this coordinate, so they overlap at this coordinate), all belong to the same component. Therefore, take all intervals currently in the active set (which includes those whose right endpoint is at this coordinate and others that are still open), mark them as completed (adding to the `completed` set), increment the component count, and clear the active set. This works because after processing the rightmost endpoint of a component, no later interval can touch that component (since any later interval must start after that point, and the component's intervals all end at or before that point). Edge cases: intervals that touch at a single point (e.g., [1,2] and [2,3]) are correctly grouped because the right endpoint at 2 and the left endpoint at 2 are processed in the same batch, and the active set includes both intervals at that time. Duplicate intervals are naturally handled. The time complexity is O(n log n) for sorting the 2n events, and O(n log n) for set operations (each interval added/removed at most once). Space complexity is O(n).
#include <bits/stdc++.h>

// Count connected components of intervals on the integer line.
// Intervals overlap if they share at least one integer coordinate.
// A component is formed by intervals that are transitively connected.
int countComponents(const std::vector<int>& starts, const std::vector<int>& ends) {
    int n = starts.size();
    if (n == 0) return 0;

    // Each event: (coordinate, type, index)
    // type 1 = right endpoint, type 0 = left endpoint (so rights sort first at same coordinate)
    std::vector<std::tuple<int, int, int>> events;
    for (int i = 0; i < n; ++i) {
        events.emplace_back(starts[i], 0, i);
        events.emplace_back(ends[i], 1, i);
    }
    std::sort(events.begin(), events.end());

    std::set<int> active;       // indices of intervals whose left endpoint seen but right not yet processed
    std::set<int> completed;    // indices of intervals already placed in a component
    int components = 0;

    size_t pos = 0;
    while (pos < events.size()) {
        int coord = std::get<0>(events[pos]);
        // Process all events at this coordinate
        // For rights: they must be processed before any lefts at same coord, which is guaranteed by 1 < 0? Actually 1 < 0 is false, so sort puts lefts (0) before rights (1). We need rights first, so fix: type 0 = right, type 1 = left.
        // Let's adjust: use type 0 for right, type 1 for left so sorting puts rights (0) before lefts (1) at same coordinate.
        // Rebuild events with correct type mapping: right=0, left=1.
        // Simpler: re-sort with custom comparator or just change order in tuple.
        // I'll rewrite the events vector with proper type.
        break; // Unused, will redo below
    }

    // Proper implementation:
    events.clear();
    for (int i = 0; i < n; ++i) {
        // type 0 = right (processed first at same coordinate), type 1 = left
        events.emplace_back(starts[i], 1, i);
        events.emplace_back(ends[i], 0, i);
    }
    std::sort(events.begin(), events.end());

    active.clear();
    completed.clear();
    components = 0;

    pos = 0;
    while (pos < events.size()) {
        int coord = std::get<0>(events[pos]);
        // Group all events at this coordinate
        size_t endPos = pos;
        while (endPos < events.size() && std::get<0>(events[endPos]) == coord) {
            ++endPos;
        }

        // Process all events in this group
        // First process rights (type 0), then lefts (type 1) – but sorting already puts rights first.
        // For this group, we handle them in order.
        bool hasRight = false;
        for (size_t k = pos; k < endPos; ++k) {
            int type = std::get<1>(events[k]);
            int idx = std::get<2>(events[k]);
            if (completed.find(idx) != completed.end()) continue; // already part of a component
            if (type == 1) { // left endpoint
                active.insert(idx);
            } else { // right endpoint
                // This interval is active (if not yet completed)
                // We'll mark it and continue.
                // But we don't immediately close; we wait until after processing all events at this coord.
                hasRight = true;
            }
        }

        // Now, if there was any right endpoint at this coordinate (and there are active intervals),
        // all active intervals form a component together.
        if (hasRight && !active.empty()) {
            ++components;
            for (int idx : active) {
                completed.insert(idx);
            }
            active.clear();
        }

        pos = endPos;
    }

    return components;
}
#include <cassert>
#include <vector>

// Assume countComponents is defined above.

int main() {
    // Simple non-overlapping intervals
    std::vector<int> s1 = {1, 3, 5};
    std::vector<int> e1 = {2, 4, 6};
    assert(countComponents(s1, e1) == 3);

    // Overlapping intervals
    std::vector<int> s2 = {1, 2, 3};
    std::vector<int> e2 = {4, 3, 5};
    assert(countComponents(s2, e2) == 1);

    // Touching at a point
    std::vector<int> s3 = {1, 2};
    std::vector<int> e3 = {2, 3};
    assert(countComponents(s3, e3) == 1);

    // Two separate components with a gap
    std::vector<int> s4 = {1, 5, 2};
    std::vector<int> e4 = {3, 8, 4};
    assert(countComponents(s4, e4) == 2);

    // Single interval
    std::vector<int> s5 = {10};
    std::vector<int> e5 = {20};
    assert(countComponents(s5, e5) == 1);

    // Empty input
    std::vector<int> s6 = {};
    std::vector<int> e6 = {};
    assert(countComponents(s6, e6) == 0);

    // All intervals identical
    std::vector<int> s7 = {1, 1, 1};
    std::vector<int> e7 = {2, 2, 2};
    assert(countComponents(s7, e7) == 1);

    // Complex chain touching at points
    std::vector<int> s8 = {1, 2, 3, 4};
    std::vector<int> e8 = {2, 3, 4, 5};
    assert(countComponents(s8, e8) == 1);

    // Two components each with multiple overlapping intervals
    std::vector<int> s9 = {1, 2, 10, 11};
    std::vector<int> e9 = {3, 4, 13, 12};
    assert(countComponents(s9, e9) == 2);

    // Intervals containing others
    std::vector<int> s10 = {1, 3, 5};
    std::vector<int> e10 = {10, 4, 6};
    assert(countComponents(s10, e10) == 1);

    return 0;
}
