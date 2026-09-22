/*
Write a C++ function `maxAttendees(const std::vector<Meeting>& meetings)` that takes a vector of meetings, where each `Meeting` has an unsigned integer `start`, an unsigned integer `end` (with `start < end`), and an integer `people` (always positive), and returns the maximum total number of people that can be scheduled using non-overlapping meetings (a meeting can start exactly when another ends). The input vector may be empty, may be unsorted, and may contain duplicate meetings or meetings with the same end time. The function must not modify the input vector.
*/

#include <vector>
#include <algorithm>

struct Meeting {
    unsigned int start, end;
    int people;
};

// Compare meetings primarily by end time, then by start time.
bool compareByEnd(const Meeting& a, const Meeting& b) {
    if (a.end != b.end) return a.end < b.end;
    return a.start < b.start;
}

// Check if meeting l ends before or exactly when meeting r starts.
bool endsBeforeStart(const Meeting& l, const Meeting& r) {
    return l.end < r.start;
}

// Returns the maximum total number of people from non-overlapping meetings.
int maxAttendees(const std::vector<Meeting>& meetings) {
    if (meetings.empty()) return 0;

    // Work on a sorted copy to avoid modifying the input.
    std::vector<Meeting> sorted = meetings;
    std::sort(sorted.begin(), sorted.end(), compareByEnd);

    int n = sorted.size();
    std::vector<int> dp(n, 0);
    dp[0] = sorted[0].people;

    for (int i = 1; i < n; ++i) {
        // Find first meeting that conflicts (end >= start of current).
        int t = std::lower_bound(sorted.begin(), sorted.begin() + i,
                                 sorted[i], endsBeforeStart) - sorted.begin();
        int take = sorted[i].people + (t > 0 ? dp[t - 1] : 0);
        dp[i] = std::max(dp[i - 1], take);
    }
    return dp[n - 1];
}

#include <cassert>
#include <vector>

// Assume the Meeting struct and maxAttendees function from above are available.

int main() {
    // Empty input
    assert(maxAttendees({}) == 0);

    // Single meeting
    std::vector<Meeting> single = {{1, 3, 10}};
    assert(maxAttendees(single) == 10);

    // Two non-overlapping meetings (touching allowed)
    std::vector<Meeting> touch = {{1, 3, 5}, {3, 5, 7}};
    assert(maxAttendees(touch) == 12);

    // Two overlapping meetings - must pick the one with more people
    std::vector<Meeting> overlap = {{1, 5, 10}, {2, 4, 20}};
    assert(maxAttendees(overlap) == 20);

    // Chain that requires optimal selection
    std::vector<Meeting> chain = {{1, 3, 5}, {2, 5, 10}, {4, 6, 7}};
    // Best is {1,3,5} + {4,6,7} = 12, not {2,5,10} alone (10)
    assert(maxAttendees(chain) == 12);

    // Unsorted input with duplicate ends
    std::vector<Meeting> unsorted = {{9, 10, 3}, {1, 2, 4}, {2, 9, 100}, {1, 10, 50}, {2, 5, 6}};
    // Sorted by end: {1,2,4}, {2,5,6}, {2,9,100}, {1,10,50}, {9,10,3}
    // Best options: take {1,2,4} + {2,5,6} + {9,10,3} = 13, or {2,9,100} alone = 100, or {1,10,50} = 50
    assert(maxAttendees(unsorted) == 100);

    // All overlapping with equal people
    std::vector<Meeting> allOverlap = {{1, 3, 2}, {2, 4, 2}, {3, 5, 2}};
    // Can only take one, so answer 2
    assert(maxAttendees(allOverlap) == 2);

    // Large gaps, best to take all
    std::vector<Meeting> gaps = {{1, 2, 1}, {10, 11, 1}, {20, 21, 1}};
    assert(maxAttendees(gaps) == 3);

    // Very large values to ensure no overflow in int (but still positive)
    std::vector<Meeting> large = {{0, 1, 1000000}, {1, 2, 1000000}};
    assert(maxAttendees(large) == 2000000);

    return 0;
}

// This is a classic Weighted Interval Scheduling problem solvable via dynamic programming after sorting. First, sort all meetings by end time (and by start time as a tie-breaker to ensure a deterministic order). Create a DP array where `dp[i]` represents the maximum total people achievable using only the first `i+1` meetings (in sorted order). For each meeting `i`, find the last meeting `t` that ends before or at `meets[i].start` using binary search (`lower_bound` with a custom comparator that checks if `end < start`), which yields the index of the first meeting that conflicts with `meets[i]`. If `t` is exactly the index of the first conflicting meeting, then `t-1` is the last compatible one; if `t == 0`, no compatible meetings exist before it. The recurrence is `dp[i] = max(dp[i-1], (t > 0 ? dp[t-1] : 0) + meets[i].people)`, meaning either skip the current meeting (take the best from previous) or take it plus the best from all meetings that end before its start. Edge cases: empty input returns 0; a single meeting returns its people; meetings with identical end times are handled by sorting tie-break. Time complexity is `O(N log N)` due to sorting and binary search for each of the `N` meetings; space complexity is `O(N)` for the DP array and sorting copy.
