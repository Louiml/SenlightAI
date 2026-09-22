// Given a list of intervals represented by their start and end coordinates (both integers), write a C++ function `findPeakInterval` that takes a vector of pairs `(start, end)` and returns a pair `(coordinate, maximumOverlap)` where `coordinate` is the smallest coordinate at which the maximum number of intervals overlap, and `maximumOverlap` is that maximum number. Overlap is counted inclusively: if an interval is `[a, b]`, then a point `c` is covered if `a <= c <= b`. The coordinate space is unbounded (may include negative values). If there are multiple coordinates with the same maximum overlap, choose the smallest such coordinate. The input may contain duplicate intervals, and intervals may have start > end (treat them as empty and ignore them). The function must handle an empty input by returning `{0, 0}`.

// The problem asks for the coordinate with the maximum number of active intervals, where an interval is active at a point if the point lies within `[start, end]`. A classic sweep-line algorithm works: collect all unique coordinates (both starts and ends) into a sorted set. Then traverse them in ascending order. Maintain a running counter `current_overlap`. At each coordinate, first add all intervals that start at that coordinate (increment `current_overlap`), then subtract all intervals that end at that coordinate (decrement `current_overlap`). After updating, if `current_overlap` exceeds the best seen so far, record this coordinate as the answer (since we traverse in ascending order, the first time we see a new maximum we get the smallest coordinate). Important edge cases: (1) At a coordinate where both a start and an end occur, we must add starts before subtracting ends, because an interval that ends at `c` and another that starts at `c` both overlap at `c`, so the correct count at `c` includes both. (2) Intervals with `start > end` are invalid and must be ignored. (3) Duplicate intervals are simply counted multiple times. (4) Empty input returns `{0,0}`. Time complexity is `O(U log U)` where `U` is the number of unique coordinates (at most 2n), due to sorting/set operations; space is `O(U)` for the set and maps.

#include <vector>
#include <set>
#include <unordered_map>
#include <utility>

// Given a vector of intervals (start, end), return {coordinate, maxOverlap}
// where coordinate is the smallest point with maximum inclusive overlap.
std::pair<int, int> findPeakInterval(const std::vector<std::pair<int, int>>& intervals) {
    if (intervals.empty()) return {0, 0};

    std::set<int> coordinates;
    std::unordered_map<int, int> start_count;
    std::unordered_map<int, int> end_count;

    for (const auto& [start, end] : intervals) {
        if (start > end) continue; // ignore invalid intervals
        coordinates.insert(start);
        coordinates.insert(end);
        start_count[start]++;
        end_count[end]++;
    }

    int current_overlap = 0;
    int max_overlap = 0;
    int best_coord = 0;

    for (int coord : coordinates) {
        // Add all starts at this coordinate first
        if (start_count.count(coord)) current_overlap += start_count[coord];
        // Then subtract all ends at this coordinate
        if (end_count.count(coord)) current_overlap -= end_count[coord];

        // Update best if we found a new maximum
        if (current_overlap > max_overlap) {
            max_overlap = current_overlap;
            best_coord = coord;
        }
    }

    return {best_coord, max_overlap};
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (implementation above)
std::pair<int, int> findPeakInterval(const std::vector<std::pair<int, int>>& intervals);

int main() {
    // Basic case: intervals [1,3], [2,4] overlap at 2 and 3 with count 2
    assert(findPeakInterval({{1,3},{2,4}}) == std::make_pair(2, 2));

    // Single interval: peak at its start
    assert(findPeakInterval({{5,10}}) == std::make_pair(5, 1));

    // Empty input
    assert(findPeakInterval({}) == std::make_pair(0, 0));

    // Invalid interval (start > end) ignored
    assert(findPeakInterval({{7,2}}) == std::make_pair(0, 0));

    // Duplicate intervals count multiple times
    assert(findPeakInterval({{1,3},{1,3}}) == std::make_pair(1, 2));

    // Overlap inclusive at endpoints: [1,2] and [2,3] both include 2
    assert(findPeakInterval({{1,2},{2,3}}) == std::make_pair(2, 2));

    // Negative coordinates
    assert(findPeakInterval({{-5,-1},{-3,0}}) == std::make_pair(-3, 2));

    // Tie broken by smallest coordinate
    assert(findPeakInterval({{1,10},{2,3},{4,5}}) == std::make_pair(1, 1));

    // Mixed valid and invalid, with max at a coordinate where start and end coincide
    // Intervals: [1,4], [2,3], [3,5], invalid [6,1]
    // At coord 3: starts from [3,5] add 1, ends from [2,3] subtract 1, current = (1+1) -1? Let's compute manually:
    // coord1: +1 => cur1, max1, best1
    // coord2: +1 => cur2, max2, best2
    // coord3: +1 (from [3,5]) then -1 (from [2,3]) => cur2, not > max2, best stays 2
    // coord4: -1 => cur1
    // coord5: -1 => cur0
    // result: (2,2)
    assert(findPeakInterval({{1,4},{2,3},{3,5},{6,1}}) == std::make_pair(2, 2));

    // Large interval coverage uniform
    assert(findPeakInterval({{-100,100},{0,0}}) == std::make_pair(0, 2));

    return 0;
}
