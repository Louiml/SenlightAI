/*
Write a C++ function `std::vector<std::vector<int>> mergeOverlappingIntervals(std::vector<std::vector<int>> intervals)` that takes a vector of intervals, where each interval is a vector of exactly two integers `[start, end]` (with `start <= end`), and returns a new vector of disjoint merged intervals. The input intervals are not necessarily sorted and may appear in any order, may contain duplicates, and may be partially or fully overlapping. The output should be sorted in ascending order by start value, with each merged interval represented as `[start, end]` where `start` and `end` are the minimum start and maximum end among the original intervals that overlap (directly or transitively). If the input is empty, return an empty vector. Your function should not mutate the input; instead, return a new vector.
*/
#include <vector>
#include <algorithm>

// Merge overlapping intervals. Input is copied to allow sorting without mutating the caller's data.
std::vector<std::vector<int>> mergeOverlappingIntervals(std::vector<std::vector<int>> intervals) {
    if (intervals.empty()) {
        return {};
    }

    // Sort by start value (and secondarily by end, though that is irrelevant after merging).
    std::sort(intervals.begin(), intervals.end());

    std::vector<std::vector<int>> merged;
    merged.reserve(intervals.size());

    // Start with the first interval.
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i) {
        const auto& current = intervals[i];
        auto& last = merged.back();

        // If current starts before or at the last merged interval's end, they overlap.
        if (current[0] <= last[1]) {
            // Extend the merged interval's end if needed.
            last[1] = std::max(last[1], current[1]);
        } else {
            // No overlap – start a new merged interval.
            merged.push_back(current);
        }
    }

    return merged;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
std::vector<std::vector<int>> mergeOverlappingIntervals(std::vector<std::vector<int>> intervals);

int main() {
    // Example 1: Simple overlapping intervals.
    std::vector<std::vector<int>> input1 = {{1,3},{2,6},{8,10},{15,18}};
    std::vector<std::vector<int>> expected1 = {{1,6},{8,10},{15,18}};
    assert(mergeOverlappingIntervals(input1) == expected1);

    // Example 2: Touching intervals should merge.
    std::vector<std::vector<int>> input2 = {{1,2},{2,3},{4,5}};
    std::vector<std::vector<int>> expected2 = {{1,3},{4,5}};
    assert(mergeOverlappingIntervals(input2) == expected2);

    // Example 3: Nested intervals and duplicates.
    std::vector<std::vector<int>> input3 = {{1,10},{2,3},{5,8},{1,10}};
    std::vector<std::vector<int>> expected3 = {{1,10}};
    assert(mergeOverlappingIntervals(input3) == expected3);

    // Example 4: Empty input returns empty.
    std::vector<std::vector<int>> input4;
    assert(mergeOverlappingIntervals(input4).empty());

    // Example 5: Single interval.
    std::vector<std::vector<int>> input5 = {{5,7}};
    assert(mergeOverlappingIntervals(input5) == std::vector<std::vector<int>>({{5,7}}));

    // Example 6: Unsorted input with contiguous intervals spanning multiple overlaps.
    std::vector<std::vector<int>> input6 = {{4,5},{1,2},{2,4},{3,6}};
    std::vector<std::vector<int>> expected6 = {{1,6}};
    assert(mergeOverlappingIntervals(input6) == expected6);

    // Example 7: No overlaps at all.
    std::vector<std::vector<int>> input7 = {{1,2},{3,4},{5,6}};
    assert(mergeOverlappingIntervals(input7) == input7);

    return 0;
}
// The solution begins by sorting the input intervals by their start values (and automatically by end values as a secondary key, which is irrelevant after merging). Sorting is essential because it allows us to process intervals in order and merge overlapping ones in a single pass. After sorting, we iterate through each interval. We maintain a result vector `merged`. For each interval `[start, end]`:
// - If the result is empty or the current interval’s start is greater than the last merged interval’s end (i.e., no overlap), we push a new interval `{start, end}` into `merged`.
// - Otherwise, the current interval overlaps (or touches, since we treat `end >= next.start` as overlap) the last merged interval, so we extend the last merged interval’s end to `max(lastEnd, end)`.
// This greedy approach works because after sorting, any interval that overlaps with the last merged one cannot start before the last’s start, so only the end needs updating. Edge cases to consider: intervals that are fully contained within another (e.g., `[1,10]` and `[2,3]`), intervals that just touch (`[1,2]` and `[2,3]` should merge to `[1,3]`), duplicate intervals, and an empty input. Time complexity is O(n log n) due to sorting, and each interval is processed once, so overall O(n log n) time. Space complexity is O(n) for the result vector (plus the sort’s internal overhead), but no extra auxiliary structures are needed beyond the output.
