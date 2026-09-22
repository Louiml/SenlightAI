Write a C++ function `std::vector<std::pair<int,int>> insertIntervals(const std::vector<std::pair<int,int>>& intervals, std::pair<int,int> newInterval)` that takes a vector of non-overlapping intervals sorted by their start values (each interval is a pair of integers `{start, end}` with `start <= end`) and a new interval, and returns a new vector of intervals after inserting `newInterval`, merging any overlapping intervals. The input intervals are guaranteed to be sorted and non-overlapping, but may be empty. The new interval may overlap with zero, one, or multiple existing intervals. The returned vector must also be sorted and contain only non-overlapping, merged intervals.
The standard approach is to iterate through the sorted intervals while building the result. We need to handle three phases: intervals that end before the new interval starts (copy them unchanged), intervals that overlap with the new interval (merge by updating the new interval's start and end to the min and max of the overlapping portions), and intervals that start after the new interval ends (copy them unchanged after inserting the merged new interval). Important edge cases: an empty input, the new interval being entirely before all existing intervals, entirely after, or completely containing one or more intervals. We can achieve a clean solution by first adding all intervals that come before the new interval, then merging all overlapping intervals while updating the new interval, then appending the merged new interval and the remaining intervals. Alternatively, a single pass can track an insertion index. The algorithm runs in O(n) time and uses O(n) extra space for the result, where n is the number of input intervals.
#include <vector>
#include <utility>
#include <algorithm>

// Insert a new interval into a sorted, non-overlapping vector of intervals, merging overlaps.
std::vector<std::pair<int,int>> insertIntervals(
    const std::vector<std::pair<int,int>>& intervals,
    std::pair<int,int> newInterval
) {
    std::vector<std::pair<int,int>> result;
    size_t i = 0;
    const size_t n = intervals.size();

    // Add all intervals that end before the new interval starts.
    while (i < n && intervals[i].second < newInterval.first) {
        result.push_back(intervals[i]);
        ++i;
    }

    // Merge all intervals that overlap with the new interval.
    while (i < n && intervals[i].first <= newInterval.second) {
        newInterval.first = std::min(newInterval.first, intervals[i].first);
        newInterval.second = std::max(newInterval.second, intervals[i].second);
        ++i;
    }

    // Insert the merged new interval.
    result.push_back(newInterval);

    // Append the remaining intervals.
    while (i < n) {
        result.push_back(intervals[i]);
        ++i;
    }

    return result;
}
#include <cassert>

int main() {
    // Basic overlap
    {
        std::vector<std::pair<int,int>> input = {{1,3},{6,9}};
        auto result = insertIntervals(input, {2,5});
        assert(result == std::vector<std::pair<int,int>>({{1,5},{6,9}}));
    }
    // Multiple overlaps
    {
        std::vector<std::pair<int,int>> input = {{1,2},{3,5},{6,7},{8,10},{12,16}};
        auto result = insertIntervals(input, {4,9});
        assert(result == std::vector<std::pair<int,int>>({{1,2},{3,10},{12,16}}));
    }
    // Empty input
    {
        std::vector<std::pair<int,int>> input = {};
        auto result = insertIntervals(input, {1,5});
        assert(result == std::vector<std::pair<int,int>>({{1,5}}));
    }
    // New interval completely inside an existing interval
    {
        std::vector<std::pair<int,int>> input = {{0,9}};
        auto result = insertIntervals(input, {1,5});
        assert(result == std::vector<std::pair<int,int>>({{0,9}}));
    }
    // New interval between intervals (no overlap)
    {
        std::vector<std::pair<int,int>> input = {{1,2},{8,9}};
        auto result = insertIntervals(input, {4,5});
        assert(result == std::vector<std::pair<int,int>>({{1,2},{4,5},{8,9}}));
    }
    // New interval after all intervals
    {
        std::vector<std::pair<int,int>> input = {{1,2},{8,9}};
        auto result = insertIntervals(input, {13,15});
        assert(result == std::vector<std::pair<int,int>>({{1,2},{8,9},{13,15}}));
    }
    // New interval before all intervals
    {
        std::vector<std::pair<int,int>> input = {{3,4},{6,7}};
        auto result = insertIntervals(input, {0,1});
        assert(result == std::vector<std::pair<int,int>>({{0,1},{3,4},{6,7}}));
    }
    // New interval covers multiple and merges boundaries exactly
    {
        std::vector<std::pair<int,int>> input = {{1,3},{4,6},{7,9}};
        auto result = insertIntervals(input, {3,7});
        assert(result == std::vector<std::pair<int,int>>({{1,9}}));
    }
    // Single existing interval, new interval disjoint to the right
    {
        std::vector<std::pair<int,int>> input = {{5,10}};
        auto result = insertIntervals(input, {1,3});
        assert(result == std::vector<std::pair<int,int>>({{1,3},{5,10}}));
    }
    // Duplicate boundary touches (end of new equals start of next)
    {
        std::vector<std::pair<int,int>> input = {{1,2},{3,4}};
        auto result = insertIntervals(input, {2,3});
        assert(result == std::vector<std::pair<int,int>>({{1,4}}));
    }
    return 0;
}
