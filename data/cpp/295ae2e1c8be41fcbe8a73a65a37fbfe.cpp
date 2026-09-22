/*
Given a set of intervals represented as pairs of integers `[start, end)` (where `start < end`), write a C++ function that takes a vector of these intervals and returns the total length of their union. Overlapping and adjacent intervals must be merged before computing the length. The input may contain duplicate intervals, intervals fully contained within others, and intervals in any order. The function should handle empty input by returning 0 and should not modify the input vector.
*/

#include <vector>
#include <algorithm>
#include <stdexcept>

// Struct representing a half-open interval [start, end).
struct Interval {
    int start;
    int end;
};

// Compute the total length of the union of a set of intervals.
// Intervals are given as pairs [start, end) with start < end.
// Returns 0 if the vector is empty.
int unionLength(const std::vector<Interval>& intervals) {
    if (intervals.empty()) {
        return 0;
    }

    // Copy to allow sorting without modifying the input.
    std::vector<Interval> sorted = intervals;
    std::sort(sorted.begin(), sorted.end(),
              [](const Interval& a, const Interval& b) {
                  if (a.start != b.start) return a.start < b.start;
                  return a.end < b.end;
              });

    int total = 0;
    int curStart = sorted[0].start;
    int curEnd = sorted[0].end;

    for (size_t i = 1; i < sorted.size(); ++i) {
        // If the current interval overlaps or is adjacent to the merged interval,
        // extend the merged end.
        if (sorted[i].start <= curEnd) {
            curEnd = std::max(curEnd, sorted[i].end);
        } else {
            // Otherwise, finalize the current merged interval.
            total += curEnd - curStart;
            curStart = sorted[i].start;
            curEnd = sorted[i].end;
        }
    }

    // Add the last merged interval.
    total += curEnd - curStart;
    return total;
}

#include <cassert>
#include <vector>

// (Interval struct and unionLength function are assumed to be included here.)

int main() {
    // Empty input.
    assert(unionLength({}) == 0);

    // Single interval.
    assert(unionLength({{0, 5}}) == 5);

    // Two disjoint intervals.
    assert(unionLength({{0, 2}, {5, 7}}) == 4);

    // Overlapping intervals.
    assert(unionLength({{1, 4}, {3, 6}}) == 5); // union is [1,6)

    // Adjacent intervals (should merge because [1,3) ∪ [3,5) = [1,5)).
    assert(unionLength({{1, 3}, {3, 5}}) == 4);

    // Nested intervals.
    assert(unionLength({{0, 10}, {2, 5}, {8, 12}}) == 12);

    // Unsorted input with duplicates.
    assert(unionLength({{5, 8}, {1, 3}, {5, 8}, {2, 4}}) == 6); // union is [1,4) ∪ [5,8)

    // Negative numbers.
    assert(unionLength({{-5, -1}, {-3, 2}}) == 7); // union is [-5,2)

    // One interval covering many others.
    assert(unionLength({{0, 20}, {1, 3}, {5, 10}}) == 20);

    // Many intervals forming a continuous line.
    assert(unionLength({{0, 1}, {1, 2}, {2, 3}, {3, 4}}) == 4);

    // Large gap between clusters.
    assert(unionLength({{0, 1}, {10, 12}, {20, 21}}) == 4);

    return 0;
}

// The solution sorts the intervals by their start value and, for ties, by their end value. Then it iterates through the sorted intervals while maintaining a current merged interval `[curStart, curEnd)`. For each interval, if it starts at or before the current end (i.e., `interval.start <= curEnd`), we extend the current end to the maximum of the two ends, thereby merging overlapping or adjacent intervals. Otherwise, we add the length of the current merged interval to the total and start a new merged interval. After the loop, we add the length of the last merged interval. The key edge cases are: empty input (return 0), a single interval (return its length), duplicate intervals (ignored after merging), and intervals that are nested (merged correctly by taking the max end). The time complexity is \(O(n \log n)\) due to sorting, and the space complexity is \(O(1)\) auxiliary (excluding the input vector) if we sort in place or \(O(n)\) if we make a copy; we will make a copy to avoid modifying the input.
