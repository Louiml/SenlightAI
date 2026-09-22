/*
Write a C++ function named `canAttendAllMeetings` that takes a vector of meeting intervals, where each interval is represented as a vector of two integers `[start, end]` (with `start < end`), and returns a boolean indicating whether a person can attend all meetings without overlapping. Two meetings overlap if one meeting's end time is strictly greater than another meeting's start time. The function should handle an empty or single-interval input correctly. After writing the function, include test code using `assert` statements to verify the function against several cases, including overlapping intervals, non-overlapping intervals, adjacent intervals (where one ends exactly when another starts), empty input, and a single interval.
*/

#include <vector>
#include <algorithm>

// Returns true if a person can attend all meetings without overlapping.
// Each interval is a vector<int> with exactly two elements: [start, end].
bool canAttendAllMeetings(std::vector<std::vector<int>>& intervals) {
    // Sort intervals by start time
    std::sort(intervals.begin(), intervals.end(),
              [](const std::vector<int>& a, const std::vector<int>& b) {
                  return a[0] < b[0];
              });
    
    // Check for overlap between consecutive meetings
    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i - 1][1] > intervals[i][0]) {
            // Previous meeting ends after next starts -> overlap
            return false;
        }
    }
    return true; // No overlaps found
}

#include <cassert>
#include <vector>

int main() {
    std::vector<std::vector<int>> empty;
    assert(canAttendAllMeetings(empty) == true);

    std::vector<std::vector<int>> single = {{1, 5}};
    assert(canAttendAllMeetings(single) == true);

    std::vector<std::vector<int>> nonOverlap = {{0, 30}, {30, 45}, {45, 60}};
    assert(canAttendAllMeetings(nonOverlap) == true);

    std::vector<std::vector<int>> overlap = {{0, 30}, {5, 10}, {15, 20}};
    assert(canAttendAllMeetings(overlap) == false);

    std::vector<std::vector<int>> unsortedNoOverlap = {{7, 10}, {2, 4}};
    assert(canAttendAllMeetings(unsortedNoOverlap) == true);

    std::vector<std::vector<int>> unsortedOverlap = {{5, 15}, {10, 20}, {1, 4}};
    assert(canAttendAllMeetings(unsortedOverlap) == false);

    std::vector<std::vector<int>> adjacentOnly = {{1, 2}, {2, 3}, {3, 4}};
    assert(canAttendAllMeetings(adjacentOnly) == true);

    std::vector<std::vector<int>> exactSameStart = {{1, 3}, {1, 4}};
    assert(canAttendAllMeetings(exactSameStart) == false);

    return 0;
}

// The solution sorts all intervals by their start time in ascending order. Once sorted, any overlap can be detected by comparing each interval's end time with the next interval's start time: if the current interval's end is greater than the next interval's start, they overlap. This works because sorting ensures that any overlapping intervals will be adjacent in the sorted order, so checking consecutive pairs is sufficient. Edge cases include an empty list (returns true) and a single interval (returns true since no pair to compare). Adjacent intervals like `[1,2]` and `[2,3]` do not overlap because the condition uses strict greater-than (`>`), not `>=`. The time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(1)\) if the sort is in-place (ignoring the input vector itself).
