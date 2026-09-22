/*
Write a C++ function that takes a vector of integer intervals, where each interval is represented as a vector of exactly two integers `[start, end]`, and returns a new vector of merged intervals. Intervals are considered overlapping if they share any common point (including touching at a boundary), and overlapping intervals must be combined into a single interval covering their union. The input intervals are not necessarily sorted, may appear in any order, and may contain duplicate intervals. The function should preserve the natural sorted order of the merged intervals (ascending by start value) in the output. Assume the input is non-empty and every interval satisfies `start <= end`.
*/

#include <vector>
#include <algorithm>

// Merge overlapping or touching intervals; returns a new sorted vector of merged intervals.
std::vector<std::vector<int>> mergeIntervals(const std::vector<std::vector<int>>& intervals) {
    if (intervals.empty()) return {};
    std::vector<std::vector<int>> sortedIntervals = intervals;
    std::sort(sortedIntervals.begin(), sortedIntervals.end());
    
    std::vector<std::vector<int>> result;
    result.push_back(sortedIntervals[0]);
    
    for (size_t i = 1; i < sortedIntervals.size(); ++i) {
        std::vector<int>& last = result.back();
        if (sortedIntervals[i][0] <= last[1]) {
            last[1] = std::max(last[1], sortedIntervals[i][1]);
        } else {
            result.push_back(sortedIntervals[i]);
        }
    }
    return result;
}

#include <cassert>

int main() {
    // Basic merge
    std::vector<std::vector<int>> in1 = {{1,3},{2,6},{8,10},{15,18}};
    std::vector<std::vector<int>> out1 = mergeIntervals(in1);
    assert(out1.size() == 3);
    assert(out1[0] == std::vector<int>({1,6}));
    assert(out1[1] == std::vector<int>({8,10}));
    assert(out1[2] == std::vector<int>({15,18}));
    
    // Already merged and unsorted input
    std::vector<std::vector<int>> in2 = {{5,7},{1,3},{2,4}};
    std::vector<std::vector<int>> out2 = mergeIntervals(in2);
    assert(out2.size() == 2);
    assert(out2[0] == std::vector<int>({1,4}));
    assert(out2[1] == std::vector<int>({5,7}));
    
    // Single interval
    std::vector<std::vector<int>> in3 = {{10,20}};
    std::vector<std::vector<int>> out3 = mergeIntervals(in3);
    assert(out3.size() == 1);
    assert(out3[0] == std::vector<int>({10,20}));
    
    // Touching intervals merge
    std::vector<std::vector<int>> in4 = {{1,2},{2,3},{3,4}};
    std::vector<std::vector<int>> out4 = mergeIntervals(in4);
    assert(out4.size() == 1);
    assert(out4[0] == std::vector<int>({1,4}));
    
    // Fully contained interval
    std::vector<std::vector<int>> in5 = {{1,10},{2,5},{6,8}};
    std::vector<std::vector<int>> out5 = mergeIntervals(in5);
    assert(out5.size() == 1);
    assert(out5[0] == std::vector<int>({1,10}));
    
    // Duplicate intervals
    std::vector<std::vector<int>> in6 = {{1,3},{1,3},{4,5}};
    std::vector<std::vector<int>> out6 = mergeIntervals(in6);
    assert(out6.size() == 2);
    assert(out6[0] == std::vector<int>({1,3}));
    assert(out6[1] == std::vector<int>({4,5}));
    
    // No overlap, unsorted
    std::vector<std::vector<int>> in7 = {{10,12},{1,2},{5,6}};
    std::vector<std::vector<int>> out7 = mergeIntervals(in7);
    assert(out7.size() == 3);
    assert(out7[0] == std::vector<int>({1,2}));
    assert(out7[1] == std::vector<int>({5,6}));
    assert(out7[2] == std::vector<int>({10,12}));
    
    // Empty interval list (edge case, though spec says non-empty, test for robustness)
    std::vector<std::vector<int>> in8 = {};
    std::vector<std::vector<int>> out8 = mergeIntervals(in8);
    assert(out8.empty());
    
    return 0;
}

// The solution sorts all intervals by their starting point (and secondarily by ending point, which the default sort on `vector<int>` handles). After sorting, any intervals that overlap must be adjacent in the sorted order, so a single pass suffices. Initialize the result with the first interval, then for each subsequent interval `[a, b]`:
// - If `a <= current_last_end`, the intervals overlap or touch; extend the current merged interval's end to `max(current_end, b)`.
// - Otherwise, the interval is disjoint from the current merged interval, so push it as a new merged interval.
// All intervals are processed exactly once after the sort. Edge cases include: a single interval (return it unchanged), identical intervals (they merge into one), adjacent intervals like `[1,2]` and `[2,3]` (they merge because `2 <= 2`), and intervals fully contained within another (the max update handles it). Time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the output vector (in-place modification of input is not used because the function takes `const` reference).
