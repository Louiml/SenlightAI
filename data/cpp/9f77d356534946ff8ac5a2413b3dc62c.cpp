/*
You are given `n` intervals, each described by a starting time `a_i` and an ending time `b_i` (with `a_i < b_i`). You must assign each interval to a meeting room. A room can host at most one interval at a time: two intervals can share a room only if they do not overlap (i.e., one ends strictly before the other starts). You may assign rooms in any order, but you must minimize the total number of distinct rooms used. Write a C++ function `std::vector<int> assignRooms(const std::vector<std::pair<int,int>>& intervals)` that, given a vector of `n` pairs `(a_i, b_i)`, returns a vector `room` of length `n` where `room[i]` is a positive integer room number assigned to the `i`-th interval (0-indexed). The rooms are numbered starting from 1, and the function must use the minimum possible number of rooms. The returned vector must use room numbers 1 through `k` for some `k` (no gaps), and every interval must be assigned a valid room number consistent with the non-overlap condition. If multiple valid assignments exist, any is acceptable as long as the number of rooms is minimal. Intervals are 0-indexed in the input order, and the output must preserve that order.
*/

#include <vector>
#include <set>
#include <algorithm>

// Assign rooms to intervals: intervals[i] = {start_i, end_i}
// Returns room numbers for each interval, 1-indexed, using minimum rooms.
std::vector<int> assignRooms(const std::vector<std::pair<int,int>>& intervals) {
    int n = (int)intervals.size();
    // events: (time, type) where type = 0 for start, 1 for end
    // For equal times, process end (type 1) before start (type 0)
    std::vector<std::pair<int,int>> events;
    events.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        events.push_back({intervals[i].first, 0});   // start
        events.push_back({intervals[i].second, 1});  // end
    }
    // sort by time, and for same time sort by type (end=1 before start=0)
    std::sort(events.begin(), events.end());

    std::set<int> availableRooms;
    for (int i = 1; i <= n; ++i) {
        availableRooms.insert(i);
    }

    std::vector<int> roomOf(n, 0);
    std::vector<int> roomForEnd(n, 0);  // to know which room to free when an interval ends

    int maxRoom = 0;
    for (const auto& ev : events) {
        int time = ev.first;
        int type = ev.second;
        // We need to associate each event with its interval index.
        // Since we stored just type, we need to recreate: but we lose the index.
        // To fix, store (time, type, index) as a triple.
        // Since the solution must be self-contained, we rewrite with triple below.
        // This is a placeholder; the correct implementation is given in the next block.
    }
    // The above is incomplete. Real implementation:
    // We'll use a vector of triples instead.

    // Correct implementation:
    struct Event {
        int time;
        int type;   // 0 = start, 1 = end
        int index;  // interval index
        bool operator<(const Event& other) const {
            if (time != other.time) return time < other.time;
            // end before start at same time
            return type > other.type;  // type 1 (end) comes before type 0 (start)
        }
    };

    std::vector<Event> evs;
    evs.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        evs.push_back({intervals[i].first, 0, i});
        evs.push_back({intervals[i].second, 1, i});
    }
    std::sort(evs.begin(), evs.end());

    std::set<int> rooms;
    for (int i = 1; i <= n; ++i) rooms.insert(i);

    std::vector<int> result(n, 0);
    int maxAssigned = 0;

    for (const auto& e : evs) {
        if (e.type == 0) {  // start
            int r = *rooms.begin();
            rooms.erase(r);
            result[e.index] = r;
            if (r > maxAssigned) maxAssigned = r;
        } else {  // end
            int r = result[e.index];
            rooms.insert(r);
        }
    }
    // result contains room numbers; the minimal number of rooms is maxAssigned
    // but we don't need to compress because we only used up to maxAssigned
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// Declare the solution function (defined elsewhere, but here for test)
std::vector<int> assignRooms(const std::vector<std::pair<int,int>>& intervals);

int main() {
    // Single interval
    std::vector<int> r = assignRooms({{1, 5}});
    assert(r.size() == 1 && r[0] == 1);

    // Two non-overlapping intervals: room 1 reused
    r = assignRooms({{1, 3}, {4, 6}});
    assert(r.size() == 2);
    assert(r[0] == 1 && r[1] == 1);

    // Two overlapping intervals: need 2 rooms
    r = assignRooms({{1, 4}, {2, 5}});
    assert(r.size() == 2);
    assert(r[0] != r[1]);
    assert(std::max(r[0], r[1]) == 2);

    // Three intervals: first and third non-overlap, second overlaps both
    r = assignRooms({{1, 2}, {1, 5}, {3, 6}});
    assert(r.size() == 3);
    assert(r[0] != r[1]);
    assert(r[1] != r[2]);
    assert(r[0] == r[2]); // exactly 2 rooms used
    assert(std::max(r[0], std::max(r[1], r[2])) == 2);

    // Intervals with same end and start (no overlap if end before start)
    r = assignRooms({{1, 3}, {3, 5}});
    assert(r.size() == 2);
    assert(r[0] == 1 && r[1] == 1);

    // All intervals overlap at a point -> n rooms
    r = assignRooms({{1, 10}, {2, 3}, {2, 4}, {1, 2}});
    assert(r.size() == 4);
    // All must have distinct rooms
    for (int i = 0; i < 4; ++i)
        for (int j = i+1; j < 4; ++j)
            assert(r[i] != r[j]);
    assert(*std::max_element(r.begin(), r.end()) == 4);

    // Large case: chain of non-overlapping intervals, room 1 for all
    std::vector<std::pair<int,int>> chain;
    for (int i = 0; i < 100; ++i) {
        chain.push_back({i*2, i*2+1});
    }
    r = assignRooms(chain);
    assert(r.size() == 100);
    for (int i = 0; i < 100; ++i) assert(r[i] == 1);

    // Empty input
    r = assignRooms({});
    assert(r.empty());
    
    return 0;
}

// The core problem is equivalent to finding the minimum number of resources (rooms) needed to schedule all intervals without overlap, which is the classic interval partitioning problem. The optimal greedy solution processes all start and end events in chronological order. Treat each interval as two events: a “start” event at time `a_i` and an “end” event at time `b_i`. Sort all events by time; when times are equal, process end events before start events to avoid assigning the same room to an interval that ends exactly when another starts. Maintain a set of available room numbers. Initially, the set contains room numbers 1 through `n` (though in practice we only need to add new room numbers on demand). When a start event occurs, assign the smallest available room from the set, remove it from the set, and record it for that interval. When an end event occurs, return the room number of the interval that ended back into the set. The maximum room number ever assigned gives the minimal number of rooms. This greedy is optimal because at any moment the number of active intervals equals the number of rooms currently in use, and the maximum simultaneous active intervals is a lower bound on the number of rooms needed; the greedy achieves exactly that lower bound. Edge cases: intervals that share an endpoint must use different rooms (because one ends at `b` and the other starts at `a` with `a == b` is considered overlapping? Actually if `a_i == b_j` for some other interval, they do not overlap because one ends exactly when the other starts, but a room can be reused only if the ending interval is returned before the starting one is assigned. To handle this correctly, process end events before start events at the same time). Also intervals with duplicate start times or duplicate end times are handled naturally. Time complexity: sorting events takes `O(n log n)`, and each event involves O(log n) set operations, so overall `O(n log n)`. Space complexity `O(n)` for events, the room array, and the set.
