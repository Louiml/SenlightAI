/*
Write a C++ function named `insertInterval` that takes a sorted (in ascending order) vector of non-overlapping intervals, where each interval is a vector of two integers `[start, end]` with `start <= end`, and a new interval `newInterval` (also a vector of two integers) to insert. The function must merge `newInterval` with any existing intervals that overlap or touch (i.e., intervals where `end >= newInterval.start` and `start <= newInterval.end`), preserving sorted order and non-overlapping property, and return the resulting vector of intervals. The input intervals are guaranteed to be sorted by start time and non-overlapping. Handle edge cases such as inserting before all, after all, completely inside an existing interval, or spanning multiple intervals. The function should not modify the original input vector.
*/
#include <vector>
#include <algorithm>

// Insert a new interval into a sorted, non-overlapping list of intervals,
// merging overlaps, and return the resulting list.
std::vector<std::vector<int>> insertInterval(
    const std::vector<std::vector<int>>& intervals,
    const std::vector<int>& newInterval
) {
    std::vector<std::vector<int>> result;
    const int n = static_cast<int>(intervals.size());
    int idx = 0;

    // Add all intervals that end strictly before the new interval starts.
    while (idx < n && intervals[idx][1] < newInterval[0]) {
        result.push_back(intervals[idx]);
        ++idx;
    }

    // If we passed all intervals, append the new interval at the end.
    if (idx == n) {
        result.push_back(newInterval);
        return result;
    }

    // If the next interval starts after the new interval ends,
    // the new interval fits entirely before it.
    if (intervals[idx][0] > newInterval[1]) {
        result.push_back(newInterval);
        // Copy the rest of the intervals unchanged.
        for (; idx < n; ++idx) {
            result.push_back(intervals[idx]);
        }
        return result;
    }

    // Otherwise, start merging from the current interval.
    int mergedStart = std::min(intervals[idx][0], newInterval[0]);
    int mergedEnd   = std::max(intervals[idx][1], newInterval[1]);
    ++idx;

    // Merge all subsequent intervals that overlap with the current merged range.
    while (idx < n && intervals[idx][0] <= mergedEnd) {
        mergedEnd = std::max(mergedEnd, intervals[idx][1]);
        ++idx;
    }

    result.push_back({mergedStart, mergedEnd});

    // Append the remaining non-overlapping intervals.
    for (; idx < n; ++idx) {
        result.push_back(intervals[idx]);
    }

    return result;
}
#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // 1. Insert into empty list.
    std::vector<std::vector<int>> empty;
    std::vector<std::vector<int>> expected1 = {{2,5}};
    assert(insertInterval(empty, {2,5}) == expected1);

    // 2. Insert before all.
    std::vector<std::vector<int>> intervals2 = {{4,6},{8,10}};
    std::vector<std::vector<int>> expected2 = {{1,2},{4,6},{8,10}};
    assert(insertInterval(intervals2, {1,2}) == expected2);

    // 3. Insert after all.
    std::vector<std::vector<int>> intervals3 = {{1,3},{5,7}};
    std::vector<std::vector<int>> expected3 = {{1,3},{5,7},{9,11}};
    assert(insertInterval(intervals3, {9,11}) == expected3);

    // 4. Insert completely inside an existing interval (no change to merged end).
    std::vector<std::vector<int>> intervals4 = {{1,5},{8,10}};
    std::vector<std::vector<int>> expected4 = {{1,5},{8,10}};
    assert(insertInterval(intervals4, {2,4}) == expected4);

    // 5. Insert spanning multiple intervals and merging them.
    std::vector<std::vector<int>> intervals5 = {{1,2},{3,4},{6,8},{10,12}};
    std::vector<std::vector<int>> expected5 = {{1,2},{3,9},{10,12}};
    assert(insertInterval(intervals5, {5,9}) == expected5);

    // 6. Insert touching boundaries (end == next start).
    std::vector<std::vector<int>> intervals6 = {{1,3},{5,7}};
    std::vector<std::vector<int>> expected6 = {{1,7}};
    assert(insertInterval(intervals6, {3,5}) == expected6);

    // 7. Insert interval that covers everything.
    std::vector<std::vector<int>> intervals7 = {{1,2},{4,5}};
    std::vector<std::vector<int>> expected7 = {{1,10}};
    assert(insertInterval(intervals7, {0,10}) == expected7);

    // 8. Insert with new start exactly before first and new end exactly after first.
    std::vector<std::vector<int>> intervals8 = {{3,4}};
    std::vector<std::vector<int>> expected8 = {{2,5}};
    assert(insertInterval(intervals8, {2,5}) == expected8);

    return 0;
}
// The algorithm processes the intervals in three phases using a single loop over the original intervals. In the first phase, push all intervals that end strictly before the new interval starts (i.e., `intervals[i][1] < newInterval[0]`) directly into the answer, because they cannot overlap. Once the first interval that does not satisfy that condition is found, there are two subcases: if this interval starts after the new interval ends (`intervals[i][0] > newInterval[1]`), then the new interval fits entirely before it, so we push the new interval and then copy all remaining intervals. Otherwise, the new interval overlaps (or touches) this interval, so we start merging: set the merged interval’s start to the minimum of the current interval’s start and the new interval’s start, and its end to the maximum of the current interval’s end and the new interval’s end. Then continue scanning subsequent intervals: while an interval’s start is less than or equal to the current merged end, extend the merged end to the maximum of itself and that interval’s end. When a non-overlapping interval is found, push the merged interval and then copy all remaining intervals. If the merged interval reaches the end, push it last. Special cases include: the new interval is before all (then the loop breaks immediately and the new interval is pushed, followed by all originals), after all (then the loop ends without finding a break, and the new interval is appended at the end), or empty input (then the result is just the new interval). Time complexity is \(O(n)\) with \(O(1)\) extra space (excluding the output), as each interval is processed at most once.
