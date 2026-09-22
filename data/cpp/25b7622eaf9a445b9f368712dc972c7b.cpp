Write a C++ function `int maxNonOverlappingActivities(const std::vector<std::pair<int,int>>& activities)` that takes a vector of activity time intervals, where each pair represents (start_time, end_time) with start_time < end_time, and returns the maximum number of activities that can be performed without overlapping, assuming an activity can start exactly when the previous one ends. The intervals are not necessarily sorted and may contain duplicates. The function should not modify the input vector.

// This is the classic "activity selection" problem. The greedy optimal strategy is to sort all activities by their end time (and for ties, by start time to maintain a deterministic order), then iterate through the sorted list, selecting an activity if its start time is greater than or equal to the end time of the last selected activity. The first activity in the sorted order is always selected. This works because choosing activities that finish earliest leaves the most room for subsequent activities. Edge cases include an empty vector (return 0) and vectors with a single activity (return 1). Duplicate intervals are harmless. Time complexity is O(n log n) due to sorting; space complexity is O(n) in the worst case due to the sorted copy (or O(1) if sorting in-place, but since we must not modify the input, we make a copy). The solution function should be `const` correct by taking the parameter by const reference.

#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum number of non-overlapping activities from the given intervals.
// A new activity can start exactly when the previous one ends.
int maxNonOverlappingActivities(const std::vector<std::pair<int,int>>& activities) {
    if (activities.empty()) {
        return 0;
    }

    // Copy to allow sorting without modifying the caller's vector.
    std::vector<std::pair<int,int>> sorted = activities;

    // Sort by end time, and for ties, by start time.
    std::sort(sorted.begin(), sorted.end(),
              [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
                  if (a.second != b.second) {
                      return a.second < b.second;
                  }
                  return a.first < b.first;
              });

    int count = 1;  // Always select the first activity in sorted order.
    int last_end = sorted[0].second;

    for (std::size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i].first >= last_end) {
            ++count;
            last_end = sorted[i].second;
        }
    }

    return count;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case
    std::vector<std::pair<int,int>> v1 = {{1,3}, {2,4}, {3,5}, {0,6}, {5,7}, {8,9}};
    assert(maxNonOverlappingActivities(v1) == 4);  // e.g., (1,3), (3,5), (5,7), (8,9)

    // Empty input
    std::vector<std::pair<int,int>> v2;
    assert(maxNonOverlappingActivities(v2) == 0);

    // Single element
    std::vector<std::pair<int,int>> v3 = {{5,10}};
    assert(maxNonOverlappingActivities(v3) == 1);

    // Exact boundary (one ends exactly when next starts)
    std::vector<std::pair<int,int>> v4 = {{1,2}, {2,3}, {3,4}};
    assert(maxNonOverlappingActivities(v4) == 3);

    // Duplicate intervals
    std::vector<std::pair<int,int>> v5 = {{1,5}, {1,5}, {2,3}, {4,6}};
    // Sorted: (2,3), (1,5), (1,5), (4,6). Select (2,3), then (4,6) -> 2
    assert(maxNonOverlappingActivities(v5) == 2);

    // All overlapping
    std::vector<std::pair<int,int>> v6 = {{1,4}, {2,5}, {3,6}};
    assert(maxNonOverlappingActivities(v6) == 1);

    // Unsorted input
    std::vector<std::pair<int,int>> v7 = {{4,7}, {1,2}, {3,5}, {6,8}};
    assert(maxNonOverlappingActivities(v7) == 3);  // (1,2), (3,5), (6,8)

    // Negative times (allowed if start < end)
    std::vector<std::pair<int,int>> v8 = {{-3,-1}, {-2,0}, {0,2}, {-1,1}};
    // Sorted by end: (-3,-1), (-2,0), (-1,1), (0,2)
    // Select (-3,-1), then (-2,0)? No because -2 < -1. Select (-1,1)? start -1 < -1? No.
    // Select (0,2) since start 0 >= -1 -> 2 total.
    assert(maxNonOverlappingActivities(v8) == 2);

    return 0;
}
