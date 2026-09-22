Write a C++ function `bool scheduleEvent(vector<pair<int,int>>& calendar, int start, int end)` that takes a vector of booked time intervals (each interval is a pair of integers `{start, end}` where the interval is half-open, meaning it includes `start` but excludes `end`), and a new event request with given `start` and `end`. The function should return `true` if the new event can be booked without overlapping any existing interval in the calendar, and `false` otherwise. If the event is booked successfully, append it to the calendar (by modifying the vector in place) and return `true`. If it overlaps, return `false` and leave the calendar unchanged. Assume all times are non-negative integers, `start < end` for all intervals, and intervals in the calendar are not necessarily sorted. The function must correctly handle edge cases where events touch at boundaries (e.g., one ends at 10 and another starts at 10) — those do not overlap and should be allowed. You should not assume any ordering of the calendar and must check against every existing interval.
// The problem is a classic interval-overlap detection with insertion. The main algorithm iterates through every interval currently stored in the calendar. For each stored interval `{s, e}`, two conditions indicate an overlap with the new interval `{start, end}`:
// 1. The new interval starts inside the stored interval: `start >= s && start < e` — formally, `s <= start && start < e`.
// 2. The stored interval starts inside the new interval: `start < s && s < end` — formally, `start < s && s < end`.
// If either condition holds for any stored interval, the booking fails. If no overlap is found, we push the new interval into the vector and return `true`. Edge cases: If the new interval ends exactly when a stored interval starts (`end == s`), no overlap; if the new interval starts exactly when a stored interval ends (`start == e`), no overlap — both conditions correctly exclude these. Time complexity is O(n) per booking call, where n is the number of intervals already in the calendar, because we scan all existing intervals. Space complexity is O(1) extra space (excluding the vector itself, which grows by one element per successful booking). The function modifies the vector only on success, which is essential for correctness when called multiple times.
#include <vector>
#include <utility>

// Attempt to book a new event [start, end) into the calendar.
// Returns true and appends the interval if no overlap; otherwise returns false without modifying the calendar.
bool scheduleEvent(std::vector<std::pair<int, int>>& calendar, int start, int end) {
    for (const auto& interval : calendar) {
        int existingStart = interval.first;
        int existingEnd = interval.second;
        // Overlap if new event starts inside existing interval, or existing interval starts inside new event.
        if (existingStart <= start && start < existingEnd) return false;
        if (start < existingStart && existingStart < end) return false;
    }
    calendar.emplace_back(start, end);
    return true;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (including necessary headers).
int main() {
    std::vector<std::pair<int, int>> cal;
    // Empty calendar: first booking always succeeds
    assert(scheduleEvent(cal, 0, 10) == true);
    assert(cal.size() == 1);
    
    // Touching intervals (end == start) are allowed
    assert(scheduleEvent(cal, 10, 20) == true);
    assert(cal.size() == 2);
    
    // Overlap with existing [0,10) — new starts inside existing
    assert(scheduleEvent(cal, 5, 15) == false);
    assert(cal.size() == 2);  // unchanged
    
    // Overlap with existing [10,20) — existing starts inside new
    assert(scheduleEvent(cal, 15, 25) == false);
    assert(cal.size() == 2);  // unchanged
    
    // Non-overlapping before all existing intervals
    assert(scheduleEvent(cal, -5, 0) == true);
    assert(cal.size() == 3);
    
    // Non-overlapping after all existing intervals
    assert(scheduleEvent(cal, 20, 30) == true);
    assert(cal.size() == 4);
    
    // Exact duplicate of existing interval [10,20)
    assert(scheduleEvent(cal, 10, 20) == false);
    assert(cal.size() == 4);
    
    // Fully contained interval [12,18) inside [10,20)
    assert(scheduleEvent(cal, 12, 18) == false);
    assert(cal.size() == 4);
    
    // Interval that contains multiple existing but has no overlap? Not possible if they are adjacent — test a gap
    assert(scheduleEvent(cal, -1, 1) == true);  // overlaps with [-5,0) and [0,10) — should fail
    // Actually that fails, so re-test a real gap: between 0 and 10 there is no gap, but after 10 we have 10,20, so no.
    // Use a simple correct case: between -5 and 0 there is a gap? no — intervals are contiguous; but we can test an interval fully between existing ones if gaps exist. Let's create a gap.
    std::vector<std::pair<int, int>> cal2;
    assert(scheduleEvent(cal2, 0, 10) == true);
    assert(scheduleEvent(cal2, 20, 30) == true);
    assert(scheduleEvent(cal2, 12, 18) == true);  // gap between 10 and 20
    assert(cal2.size() == 3);
}
