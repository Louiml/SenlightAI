// Write a C++ function `mergeRegions` that takes a vector of disjoint integer intervals `[start, end]` (inclusive, with `start <= end`) and returns a vector of merged intervals, but with a twist: two intervals are merged only if the gap between them is less than or equal to a given threshold `maxGap`. The input vector may be unsorted and may contain intervals that overlap or are adjacent (gap 0). The output must be sorted by ascending start value, with each interval covering the union of all intervals and gaps that were merged together. Throwing an exception or returning empty for invalid input is acceptable, but you may assume all inputs are valid intervals. The function must be efficient and operate in-place if possible, but returning a new vector is also fine.
// The core algorithm is similar to the classic interval merge problem, but with a tolerance for gaps. First, sort the intervals by their start value (and if starts tie, by end value for stability). Then iterate through the sorted list, maintaining a "current merged interval" initialized from the first interval. For each subsequent interval `[s, e]`, check if `s - currentEnd <= maxGap + 1` (since the gap is the number of integers between the intervals; adjacency means gap 0, so `s <= currentEnd + 1` corresponds to gap 0, and the condition is `s <= currentEnd + maxGap + 1`). If true, merge by updating `currentEnd = max(currentEnd, e)`; otherwise, push the current merged interval into the result and start a new one. Edge cases include an empty input (return empty vector), a single interval, overlapping intervals (gap might be negative, which satisfies the condition), and intervals that are far apart. Time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the output (or `O(1)` extra if reusing the input vector after sorting). The main pitfall is correctly handling the gap threshold: ensure the condition uses `<=` not `<`, and account for the fact that the end is inclusive.
#include <vector>
#include <algorithm>

// Merge intervals if the gap between them is <= maxGap.
// Intervals are [start, end] inclusive. Output sorted by start.
std::vector<std::pair<int, int>> mergeRegions(
    std::vector<std::pair<int, int>> intervals, int maxGap) {
    if (intervals.empty()) return {};

    // Sort by start, then by end.
    std::sort(intervals.begin(), intervals.end());

    std::vector<std::pair<int, int>> result;
    int currentStart = intervals[0].first;
    int currentEnd = intervals[0].second;

    for (size_t i = 1; i < intervals.size(); ++i) {
        int s = intervals[i].first;
        int e = intervals[i].second;
        // Condition: gap = s - currentEnd - 1 (number of integers between).
        // Merge if gap <= maxGap, i.e., s - currentEnd - 1 <= maxGap
        // => s <= currentEnd + maxGap + 1.
        if (s <= currentEnd + maxGap + 1) {
            currentEnd = std::max(currentEnd, e);
        } else {
            result.emplace_back(currentStart, currentEnd);
            currentStart = s;
            currentEnd = e;
        }
    }
    result.emplace_back(currentStart, currentEnd);
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic merging with overlap and adjacency (gap 0).
    std::vector<std::pair<int, int>> input1 = {{1, 3}, {2, 5}};
    auto out1 = mergeRegions(input1, 0);
    assert(out1.size() == 1 && out1[0] == std::make_pair(1, 5));

    // Gap within threshold (gap 2 <= maxGap 2).
    std::vector<std::pair<int, int>> input2 = {{1, 2}, {5, 6}};
    auto out2 = mergeRegions(input2, 2);
    assert(out2.size() == 1 && out2[0] == std::make_pair(1, 6));

    // Gap exceeds threshold (gap 3 > maxGap 2).
    std::vector<std::pair<int, int>> input3 = {{1, 2}, {6, 7}};
    auto out3 = mergeRegions(input3, 2);
    assert(out3.size() == 2 && out3[0] == std::make_pair(1, 2) && out3[1] == std::make_pair(6, 7));

    // Unsorted input, multiple merges.
    std::vector<std::pair<int, int>> input4 = {{10, 12}, {1, 2}, {4, 5}, {8, 9}};
    auto out4 = mergeRegions(input4, 3);
    // Gap between 1-2 and 4-5 is 1, merge; gap between 5 and 8 is 2, merge; gap between 9 and 10 is 0, merge.
    assert(out4.size() == 1 && out4[0] == std::make_pair(1, 12));

    // Empty input.
    std::vector<std::pair<int, int>> input5;
    auto out5 = mergeRegions(input5, 0);
    assert(out5.empty());

    // Single interval.
    std::vector<std::pair<int, int>> input6 = {{7, 9}};
    auto out6 = mergeRegions(input6, 100);
    assert(out6.size() == 1 && out6[0] == std::make_pair(7, 9));

    // Overlapping and contained.
    std::vector<std::pair<int, int>> input7 = {{1, 10}, {2, 3}, {4, 5}};
    auto out7 = mergeRegions(input7, 1);
    assert(out7.size() == 1 && out7[0] == std::make_pair(1, 10));

    return 0;
}
