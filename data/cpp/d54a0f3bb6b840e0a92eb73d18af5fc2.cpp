Write a C++ function `maximumActivities` that takes three parameters: an integer `N` (the number of activities), a vector of integers `startTimes` (length N), and a vector of integers `finishTimes` (length N), and returns the maximum number of non-overlapping activities that can be performed by a single machine or person. An activity is represented by a pair `(start, finish)`, and two activities are compatible if the finish time of one is less than or equal to the start time of the other. Assume all times are positive integers, and that both vectors are of size N. The function should handle up to 1000 activities per call and should be reusable in multiple test scenarios. The algorithm must choose activities greedily by sorting them by finish time, then selecting each activity whose start time is at least the finish time of the last selected one. The function should return the count of selected activities. Do not include a `main` function in your solution.
#include <cassert>
#include <vector>

// Declaration of the function under test
int maximumActivities(int N, const std::vector<int>& startTimes, const std::vector<int>& finishTimes);

int main() {
    // Example from the prompt: start = {1,3,0,5,8,5}, finish = {2,4,6,7,9,9} → 4
    assert(maximumActivities(6, {1,3,0,5,8,5}, {2,4,6,7,9,9}) == 4);

    // Single activity
    assert(maximumActivities(1, {5}, {10}) == 1);

    // All overlapping (same start, different finish) → only 1 can be chosen
    assert(maximumActivities(3, {1,1,1}, {2,3,4}) == 1);

    // Sequential non-overlapping activities
    assert(maximumActivities(3, {1,3,5}, {2,4,6}) == 3);

    // Activities that meet exactly at boundaries (finish == next start) are compatible
    assert(maximumActivities(2, {2,5}, {5,7}) == 2);

    // Zero-duration activities (start == finish) that do not overlap
    assert(maximumActivities(2, {3,4}, {3,4}) == 2);

    // Mixed: some overlapping, some not
    assert(maximumActivities(5, {0,1,2,3,4}, {5,3,4,6,6}) == 2); // picks (1,3) and (3,6) or (2,4) and (4,6)

    // Unsorted input should give same result as sorted
    assert(maximumActivities(4, {5,1,3,0}, {9,2,4,6}) == 3); // (0,6), (1,2), (3,4) → but (0,6) blocks others, so correct answer: (1,2),(3,4),(5,9) → 3

    // Duplicates and equal finish times
    assert(maximumActivities(4, {1,2,3,4}, {5,5,6,6}) == 2); // e.g., (1,5) and (3,6) or (2,5) and (4,6)

    // Large number of activities (performance test)
    std::vector<int> starts(1000), finishes(1000);
    for (int i = 0; i < 1000; ++i) {
        starts[i] = i;
        finishes[i] = i + 1;
    }
    assert(maximumActivities(1000, starts, finishes) == 1000);

    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum number of non-overlapping activities that can be performed.
// Activities are given as startTimes and finishTimes arrays of equal length N.
// The function sorts activities by finish time and greedily selects compatible ones.
int maximumActivities(int N, const std::vector<int>& startTimes, const std::vector<int>& finishTimes) {
    // Pair each start time with its corresponding finish time
    std::vector<std::pair<int, int>> activities;
    activities.reserve(N);
    for (int i = 0; i < N; ++i) {
        activities.emplace_back(startTimes[i], finishTimes[i]);
    }
    
    // Sort by finish time (ascending). If equal, order doesn't matter.
    std::sort(activities.begin(), activities.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.second < b.second;
              });
    
    int count = 0;
    int last_finish = 0; // assume all times are positive, so 0 is safe as initial
    for (const auto& act : activities) {
        if (act.first >= last_finish) {
            ++count;
            last_finish = act.second;
        }
    }
    return count;
}
// The optimal solution uses a classic greedy algorithm for the activity selection problem: sort all activities by their finish time in ascending order, then scan through the sorted list. Keep track of the finish time of the last selected activity (initially 0 or negative infinity). For each activity, if its start time is greater than or equal to the last selected finish time, select it, increment the count, and update the last finish time to this activity's finish time. This works because sorting by finish time ensures that we always consider the earliest-ending feasible activity next, leaving maximal room for subsequent activities. Edge cases: a single activity always returns 1; activities with equal finish times are handled naturally by the sorting (stable sort not required since any order among same finish times is fine); activities with zero duration (start == finish) are allowed, and if they fit sequentially, they can be selected; inputs with all activities overlapping will return 1 because only the earliest-finishing one is selected. Time complexity is O(N log N) due to sorting, and space complexity is O(N) for the pair vector (or O(1) if we sort indices). No special constraints on time values beyond being positive integers.
