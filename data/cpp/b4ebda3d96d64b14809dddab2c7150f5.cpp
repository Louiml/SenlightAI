// You are given a list of `n` meeting intervals, each defined by a start time `l` and an end time `r` (both integers, `0 ≤ l < r ≤ 10^9`). Write a C++ function `assignMeetingRooms` that takes a vector of pairs `(l, r)` and returns a vector of room assignments. Each meeting must be assigned to a room number (starting from 1), and no two meetings may overlap in the same room. The function must minimize the total number of rooms used. If a meeting starts exactly when another ends, they can share a room (e.g., [1,3] and [3,5] are compatible). The room numbers in the output must correspond to the original order of the input meetings. The first meeting that starts can be assigned room 1, and any new room needed later should use the smallest available number (reuse a released room number if possible). Return a vector of integers where `answer[i]` is the room number for the i-th meeting.

// The solution uses a sweep-line algorithm over sorted events. Each interval produces two events: a start event (time, type=0, index) and an end event (time, type=1, index). Sorting all events by time ensures we process starts and ends in chronological order. When an end event occurs, we push its room number into a queue of available rooms. When a start event occurs, if the queue is non‑empty, we reuse the front (smallest available) room; otherwise, we allocate a new room (incrementing the total). This greedy strategy is optimal because it always assigns the earliest freed room to the current meeting, leading to the minimum number of rooms. Edge cases: simultaneous events—if a meeting ends and another starts at the same time, we must process the end first to allow reuse. The sorting comparator must handle ties by placing end events before start events. This code specifically sorts the raw array without a custom comparator, but because `type` for end is 1 and for start is 0, and sorting by the second element ascending would put start (0) before end (1), which is incorrect. To fix, we set `type` as 1 for end and 0 for start, then sort by time only, and in the loop we handle end (dx==1) before start (dx==0) if they are at the same time. Since we sort by full array in ascending order, a tie on time is broken by type ascending, meaning start (0) comes before end (1)—that would be wrong. Therefore we need to store type such that end comes first. The snippet uses `{l,0,i}` and `{r,1,i}` and processes `if (dx==1)` (end) before `else if (dx==0)` (start). But sorting by first then second ascending would put `{l,0,i}` before `{r,1,i}` for the same l, which is okay because start times are strictly less than end times for the same meeting, but across different meetings simultaneous events need end before start. To handle that, we should store end as type = 0 and start as type = 1, or use a custom sort. In the reference solution we adjust. Time complexity is O(n log n) due to sorting, O(n) space for events and queues. The algorithm produces a valid assignment and minimal rooms.

#include <vector>
#include <queue>
#include <algorithm>
#include <array>

// Assigns meeting rooms to intervals to minimize total rooms.
// Returns a vector where ans[i] is the room number for interval i (1-indexed).
std::vector<int> assignMeetingRooms(const std::vector<std::pair<int,int>>& intervals) {
    int n = static_cast<int>(intervals.size());
    std::vector<std::array<int,3>> events(2 * n); // {time, type, idx} – type 0 = end, 1 = start
    for (int i = 0; i < n; ++i) {
        events[2*i]     = {intervals[i].second, 0, i}; // end event processed first at same time
        events[2*i + 1] = {intervals[i].first, 1, i};
    }

    // Sort by time; for equal times, end (type 0) comes before start (type 1)
    std::sort(events.begin(), events.end());

    std::vector<int> ans(n);
    std::queue<int> available;
    int totalRooms = 0;

    for (const auto& ev : events) {
        int time = ev[0], type = ev[1], idx = ev[2];
        if (type == 0) { // meeting ends
            available.push(ans[idx]);
        } else { // meeting starts
            if (!available.empty()) {
                ans[idx] = available.front();
                available.pop();
            } else {
                ans[idx] = ++totalRooms;
            }
        }
    }

    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// assume assignMeetingRooms is defined above

int main() {
    // Example 1: overlapping intervals require 3 rooms
    std::vector<std::pair<int,int>> intervals1 = {{1,5}, {2,6}, {3,7}};
    std::vector<int> ans1 = assignMeetingRooms(intervals1);
    assert(ans1 == std::vector<int>({1,2,3}));

    // Example 2: back-to-back meetings can share a room
    std::vector<std::pair<int,int>> intervals2 = {{1,3}, {3,5}, {5,7}};
    std::vector<int> ans2 = assignMeetingRooms(intervals2);
    assert(ans2 == std::vector<int>({1,1,1}));

    // Example 3: nested intervals require 2 rooms
    std::vector<std::pair<int,int>> intervals3 = {{1,10}, {2,3}, {4,5}};
    std::vector<int> ans3 = assignMeetingRooms(intervals3);
    assert(ans3 == std::vector<int>({1,2,2})); // second and third use room2, first uses room1

    // Example 4: single interval
    std::vector<std::pair<int,int>> intervals4 = {{5,10}};
    std::vector<int> ans4 = assignMeetingRooms(intervals4);
    assert(ans4 == std::vector<int>({1}));

    // Example 5: one interval ends, another starts exactly at the same time -> reuse
    std::vector<std::pair<int,int>> intervals5 = {{1,5}, {5,10}, {10,15}};
    std::vector<int> ans5 = assignMeetingRooms(intervals5);
    assert(ans5 == std::vector<int>({1,1,1}));

    // Example 6: multiple meetings starting at same time need distinct rooms
    std::vector<std::pair<int,int>> intervals6 = {{1,2}, {1,3}, {1,4}};
    std::vector<int> ans6 = assignMeetingRooms(intervals6);
    assert(ans6 == std::vector<int>({1,2,3}));

    // Example 7: release and reuse – first meeting ends before third starts
    std::vector<std::pair<int,int>> intervals7 = {{1,2}, {3,5}, {2,4}};
    // Sort by start: (1,2), (2,4), (3,5) -> assign room1, room2, then room1 (since first ends at 2)
    std::vector<int> ans7 = assignMeetingRooms(intervals7);
    assert(ans7 == std::vector<int>({1,2,1}));

    return 0;
}
