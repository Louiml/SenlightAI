/*
Write a C++ function `findRightInterval` that takes a vector of intervals, where each interval is represented as a vector of two integers `[start, end]`, and returns a vector of integers where the value at index `i` is the index of the interval with the smallest `start` that is greater than or equal to the `end` of interval `i`. If no such interval exists, the value should be `-1`. The indices returned correspond to the original order of intervals in the input. The function must handle empty input (returning an empty vector), intervals with equal starts, and negative values. You may assume that all interval starts are unique in the given input, but this uniqueness should not be relied upon—if there are duplicate starts, any valid index among them is acceptable (in practice, the map-based solution will return the last inserted index). The solution must be efficient for large inputs, so avoid an \(O(n^2)\) brute-force approach.
*/

#include <vector>
#include <map>
#include <algorithm> // not strictly needed but harmless

// Given intervals as [start, end], return for each interval the index of the interval
// with the smallest start >= end, or -1 if none. The returned indices refer to original input order.
std::vector<int> findRightInterval(const std::vector<std::vector<int>>& intervals) {
    int n = static_cast<int>(intervals.size());
    std::vector<int> result(n, -1);
    if (n == 0) return result;

    // Map from start value to original index.
    std::map<int, int> startToIndex;
    for (int i = 0; i < n; ++i) {
        startToIndex[intervals[i][0]] = i;
    }

    for (int i = 0; i < n; ++i) {
        auto it = startToIndex.lower_bound(intervals[i][1]);
        if (it != startToIndex.end()) {
            result[i] = it->second;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<std::vector<int>> intervals1 = {{1,2}};
    assert(findRightInterval(intervals1) == std::vector<int>{-1});

    // Example from problem
    std::vector<std::vector<int>> intervals2 = {{3,4},{2,3},{1,2}};
    assert(findRightInterval(intervals2) == std::vector<int>{-1,0,1});

    // Exact match case
    std::vector<std::vector<int>> intervals3 = {{1,4},{2,3},{3,4}};
    // Interval 0 end=4 -> no start >=4 -> -1; interval 1 end=3 -> start 3 at index2; interval 2 end=4 -> -1
    assert(findRightInterval(intervals3) == std::vector<int>{-1,2,-1});

    // Duplicate starts (last index wins)
    std::vector<std::vector<int>> intervals4 = {{1,2},{1,3},{2,3}};
    // For interval 0 end=2 -> lower_bound(2) finds start 2 at index2; interval 1 end=3 -> none; interval 2 end=3 -> none
    assert(findRightInterval(intervals4) == std::vector<int>{2,-1,-1});

    // Negative values
    std::vector<std::vector<int>> intervals5 = {{-3,-1},{-2,0},{0,2}};
    // Interval 0 end=-1 -> lower_bound(-1) finds start 0 at index2; interval 1 end=0 -> start 0 at index2; interval 2 end=2 -> none
    assert(findRightInterval(intervals5) == std::vector<int>{2,2,-1});

    // Empty input
    std::vector<std::vector<int>> intervals6 = {};
    assert(findRightInterval(intervals6) == std::vector<int>{});

    // Unsorted input
    std::vector<std::vector<int>> intervals7 = {{5,6},{1,2},{3,5}};
    // Interval 0 end=6 -> none; interval 1 end=2 -> start 3 at index2; interval 2 end=5 -> start 5 at index0
    assert(findRightInterval(intervals7) == std::vector<int>{-1,2,0});
}

// The core idea is to use a sorted map from each interval's `start` value to its original index. Since `std::map` maintains keys in sorted order and supports logarithmic lower-bound lookup, we can build the map in \(O(n \log n)\) time by inserting each start with its index. Then for each interval, we call `lower_bound(end)` on the map to find the first key that is greater than or equal to the interval's end. If such an iterator exists, we take its mapped index; otherwise, we return `-1`. The key edge cases include: empty input (return empty vector), intervals where the end is greater than all starts (result is `-1`), and intervals where the end exactly equals a start (the lower_bound finds that exact key, which is correct). Duplicate starts are handled naturally by the map overwriting, but since we iterate over the original order, the last inserted index for a duplicate start wins; this is acceptable per the task. Time complexity is \(O(n \log n)\) for insertion and \(O(n \log n)\) for the n lower_bound queries, total \(O(n \log n)\). Space complexity is \(O(n)\) for the map and result vector.
