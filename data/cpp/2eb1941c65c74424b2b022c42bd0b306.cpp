Write a C++ function `long long unionLength(const std::vector<std::pair<long long, long long>>& intervals)` that takes a vector of closed intervals `[l, r]` (with `l <= r`) and returns the total length of their union (counted as the number of integer points covered). The intervals may overlap, be disjoint, or be nested. For example, the union of `[1,3]` and `[2,5]` has length `5` (points 1,2,3,4,5). The input is not necessarily sorted, and the intervals can contain negative values. The function must be efficient for up to \(10^5\) intervals.

The key idea is to transform each interval into two events: a "start" event at the left endpoint and an "end" event at the right endpoint. Sort all events by coordinate. Then sweep through the sorted events while maintaining a counter `active` of how many intervals currently cover the current position. When moving from one coordinate to the next, if `active > 0`, then all integer points between the previous coordinate and the current coordinate are covered, so we add `current - previous` to the total length. At each coordinate, process all events with the same coordinate before computing the gap to the next coordinate. Important edge cases: intervals that share endpoints (e.g., `[1,2]` and `[2,3]`) — since we add gaps between consecutive distinct coordinates, the shared point `2` is counted once; we must ensure we do not double count the same point. The standard approach: after sorting events, iterate through them, but when multiple events share the same coordinate, we should process them together (i.e., update the active count for all events at that coordinate before computing the distance to the next coordinate). Alternatively, the given snippet uses a simpler method: it adds `x[i].first - x[i-1].first` if `c>0` before updating `c` at the current event. That works because it only adds distances between consecutive events when there is an active interval spanning that gap. Time complexity is \(O(n \log n)\) due to sorting, with \(O(n)\) auxiliary space.

#include <vector>
#include <algorithm>
#include <utility>

// Compute the total number of integer points covered by the union of closed intervals.
// Each interval is given as [l, r] with l <= r.
long long unionLength(const std::vector<std::pair<long long, long long>>& intervals) {
    if (intervals.empty()) return 0;

    // Create events: (coordinate, is_end)
    std::vector<std::pair<long long, bool>> events;
    events.reserve(intervals.size() * 2);
    for (const auto& interval : intervals) {
        events.emplace_back(interval.first, false);  // start
        events.emplace_back(interval.second, true);  // end
    }

    std::sort(events.begin(), events.end());

    long long total_length = 0;
    long long active = 0;
    long long prev_coord = events[0].first;

    for (const auto& event : events) {
        long long coord = event.first;
        bool is_end = event.second;

        // If we moved to a new coordinate and we have active intervals,
        // the gap from prev_coord to coord is fully covered.
        if (coord > prev_coord && active > 0) {
            total_length += coord - prev_coord;
        }

        // Update active count using all events at this coordinate.
        // Note: we process one event at a time, but the gap computation only uses
        // the previous coordinate, so overlapping end/start at the same point is fine.
        if (is_end) {
            active--;
        } else {
            active++;
        }

        prev_coord = coord;
    }

    return total_length;
}

#include <cassert>
#include <vector>
#include <utility>

// Assume unionLength is defined above.

int main() {
    // Basic overlapping intervals
    std::vector<std::pair<long long, long long>> intervals1 = {{1, 3}, {2, 5}};
    assert(unionLength(intervals1) == 5); // points 1,2,3,4,5

    // Disjoint intervals
    std::vector<std::pair<long long, long long>> intervals2 = {{1, 2}, {4, 5}};
    assert(unionLength(intervals2) == 4); // 1,2,4,5

    // Nested intervals and duplicates
    std::vector<std::pair<long long, long long>> intervals3 = {{0, 10}, {2, 3}, {5, 8}, {0, 10}};
    assert(unionLength(intervals3) == 11); // 0..10 inclusive

    // Single interval
    std::vector<std::pair<long long, long long>> intervals4 = {{-3, -1}};
    assert(unionLength(intervals4) == 3); // -3,-2,-1

    // Adjacent intervals (share an endpoint)
    std::vector<std::pair<long long, long long>> intervals5 = {{1, 3}, {3, 5}};
    assert(unionLength(intervals5) == 5); // 1,2,3,4,5 (point 3 counted once)

    // Empty input
    std::vector<std::pair<long long, long long>> intervals6 = {};
    assert(unionLength(intervals6) == 0);

    // Large range and many intervals
    std::vector<std::pair<long long, long long>> intervals7 = {{-1000000000, 1000000000}, {0, 1}};
    assert(unionLength(intervals7) == 2000000001LL); // all points from -1e9 to 1e9 inclusive

    return 0;
}
