Write a C++ function `int minRemovalsToNonOverlapping(std::vector<std::vector<int>>& intervals)` that takes a vector of intervals, each represented as `[start, end]` (inclusive on both ends), and returns the minimum number of intervals that must be removed so that the remaining intervals do not overlap. Two intervals overlap if they share any point, including endpoints (e.g., `[1,3]` and `[3,4]` overlap). The input intervals may be unsorted, contain duplicates, and the vector is guaranteed to be non-empty. You must implement the solution using a greedy approach based on sorting by end time, and the function must return the count of removed intervals.

// The problem asks for the minimum number of intervals to remove so that no two remaining intervals overlap. This is equivalent to finding the maximum size of a set of mutually non-overlapping intervals, and then subtracting that from the total count. The standard greedy strategy is: sort all intervals by their end time in ascending order. Then iterate through the sorted list, keeping track of the end time of the last interval that was kept. For each next interval, check if it overlaps with the last kept interval (i.e., if its start time is less than the last end time). If yes, we must remove it (increment removal counter). If no, we keep it and update the last end time to the current interval's end time. This greedy works because by always selecting the interval with the earliest finish time that does not overlap, we leave maximum room for subsequent intervals. Important edge cases: intervals sharing endpoints are considered overlapping; the input is non-empty so we can safely initialize with the first interval after sorting; sorting with a custom comparator that compares only the second element (end time) is sufficient, and if end times are equal, the order doesn't matter for correctness (though sorting by end only is fine). Time complexity is O(n log n) due to sorting, plus O(n) for the scan, so overall O(n log n). Space complexity is O(1) auxiliary, aside from the input vector (which can be modified in-place or copied, but we'll modify in-place by sorting the passed vector).

#include <vector>
#include <algorithm>

// Returns the minimum number of intervals to remove so that no two remaining intervals overlap.
// Intervals [start, end] overlap if they share any point, including endpoints.
int minRemovalsToNonOverlapping(std::vector<std::vector<int>>& intervals) {
    // Sort intervals by their end time in ascending order.
    std::sort(intervals.begin(), intervals.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[1] < b[1];
              });

    int removals = 0;
    // Track the end time of the last kept interval. Since input is non-empty, safe.
    int lastEnd = intervals[0][1];

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i][0] < lastEnd) {
            // Overlaps with the last kept interval, so we remove it.
            ++removals;
        } else {
            // No overlap, keep it and update the last end time.
            lastEnd = intervals[i][1];
        }
    }

    return removals;
}

#include <cassert>
#include <vector>

// The function under test; normally included from the solution header.
int minRemovalsToNonOverlapping(std::vector<std::vector<int>>& intervals);

int main() {
    // Example: [[1,2],[2,3],[3,4],[1,3]] -> remove [1,3] (since it overlaps with [1,2] and [2,3]).
    std::vector<std::vector<int>> test1 = {{1,2},{2,3},{3,4},{1,3}};
    assert(minRemovalsToNonOverlapping(test1) == 1);

    // Already non-overlapping intervals [1,2],[3,4],[5,6] -> no removals.
    std::vector<std::vector<int>> test2 = {{1,2},{3,4},{5,6}};
    assert(minRemovalsToNonOverlapping(test2) == 0);

    // All overlapping: [1,3],[1,3],[2,4] -> keep one, remove two.
    std::vector<std::vector<int>> test3 = {{1,3},{1,3},{2,4}};
    assert(minRemovalsToNonOverlapping(test3) == 2);

    // Single interval -> no removals.
    std::vector<std::vector<int>> test4 = {{5,10}};
    assert(minRemovalsToNonOverlapping(test4) == 0);

    // Endpoint overlap considered: [1,3] and [3,5] overlap -> remove one.
    std::vector<std::vector<int>> test5 = {{1,3},{3,5},{5,7}};
    assert(minRemovalsToNonOverlapping(test5) == 1);

    // Unsorted input with duplicates and equal end times.
    std::vector<std::vector<int>> test6 = {{0,2},{1,3},{2,4},{4,6},{0,2}};
    // Sorted by end: [0,2],[0,2],[1,3],[2,4],[4,6]
    // Keep [0,2], then [0,2] overlap? start=0 < end=2 -> remove.
    // [1,3] start=1 < 2 -> remove.
    // [2,4] start=2 not < 2 -> keep, lastEnd=4.
    // [4,6] start=4 not < 4 -> keep. Total removals = 2.
    assert(minRemovalsToNonOverlapping(test6) == 2);

    // All intervals overlap heavily.
    std::vector<std::vector<int>> test7 = {{1,2},{1,2},{1,2}};
    assert(minRemovalsToNonOverlapping(test7) == 2);

    // Non-overlapping but touching at a single point.
    std::vector<std::vector<int>> test8 = {{1,2},{2,3},{3,4}};
    // Overlap because share endpoints: [1,2] & [2,3] overlap -> remove? Actually [1,2] and [2,3] share 2; remove one.
    // Let's simulate: sorted as-is. lastEnd=2. Next [2,3]: start=2 not <2, keep, lastEnd=3. Next [3,4]: start=3 not <3, keep. Removals=0? Wait, share start = lastEnd is not overlap because we remove only if start < lastEnd, so if start == lastEnd, no overlap. So removals=0. Correct.
    assert(minRemovalsToNonOverlapping(test8) == 0);

    // Mixed case with negative numbers.
    std::vector<std::vector<int>> test9 = {{-5,-1},{-2,0},{1,3},{-1,1}};
    // Sorted by end: [-5,-1],[-2,0]? end -1,0,1,3 -> order: [-5,-1],[-2,0],[-1,1],[1,3]
    // lastEnd=-1, next [-2,0]: start=-2 < -1? false (-2 < -1 is true, so overlap -> remove), wait start=-2, lastEnd=-1, since -2 < -1, it's overlap. Remove one.
    // Next [-1,1]: start=-1 not < -1, keep, lastEnd=1.
    // Next [1,3]: start=1 not < 1, keep. Total removals=1.
    assert(minRemovalsToNonOverlapping(test9) == 1);

    return 0;
}
