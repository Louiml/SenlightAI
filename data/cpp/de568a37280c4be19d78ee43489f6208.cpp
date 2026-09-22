A hotel has `n` guests arriving throughout the day. Each guest is described by an arrival time and a departure time (both inclusive of the time units, meaning a guest occupying a room from time `a` to `b` must leave after time `b`). The hotel has an unlimited number of rooms, but it wants to minimize the total number of rooms used, assigning each guest to a room such that no two guests occupying the same room overlap (i.e., if guest A departs at time 5 and guest B arrives at time 5, they cannot share a room because departure and arrival times are inclusive). Write a C++ function that takes a vector of pairs `(arrival, departure)` and returns a pair containing the minimum number of rooms required and a vector `assignment` where `assignment[i]` tells which room number (starting from 1) the `i`-th guest (in the order given in the input) should be assigned to. If there are multiple valid assignments with the minimum room count, any is acceptable. Your function should be named `assign_rooms` and must not modify the input.
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above (assume it's included here).
int main() {
    // Example 1: Simple cases
    {
        auto [rooms, assign] = assign_rooms({{1, 3}, {2, 4}, {3, 5}});
        assert(rooms == 2);
        // Since assignment can vary, check that it's a valid coloring with 2 rooms.
        assert(assign[0] >= 1 && assign[0] <= 2);
        assert(assign[1] >= 1 && assign[1] <= 2);
        assert(assign[2] >= 1 && assign[2] <= 2);
        // No two overlapping guests share a room: (0,1) overlap, (1,2) overlap, (0,2) not overlap.
        assert(assign[0] != assign[1]);
        assert(assign[1] != assign[2]);
    }

    // Example 2: Zero-length stays (arrival == departure) cannot share with arrival at same time.
    {
        auto [rooms, assign] = assign_rooms({{5, 5}, {5, 6}, {6, 7}});
        assert(rooms == 2); // Guest0 needs a room alone at time 5, guest1 and guest2 can share? No: guest1 departs at 6, guest2 arrives at 6, so they share? Actually guest1 (5-6) and guest2 (6-7) don't overlap because 6 > 6 is false, they overlap at 6, so need separate. Thus rooms=2 total: room1 for guest0, room2 for guest1, then guest2 can reuse room1? Wait: compute carefully. All three need distinct? Guest0 (5-5), guest1 (5-6) overlap at 5, so distinct. Guest2 (6-7) overlaps with guest1 at 6, so distinct. Guest0 (5) and guest2 (6) don't overlap, so they can share. So rooms=2.
        assert(rooms == 2);
        assert(assign[0] != assign[1]);
        assert(assign[1] != assign[2]);
        // guest0 and guest2 can be same or different, but must not share with others.
    }

    // Example 3: All overlapping
    {
        auto [rooms, assign] = assign_rooms({{1, 10}, {2, 9}, {3, 8}});
        assert(rooms == 3);
        assert(assign[0] != assign[1] && assign[0] != assign[2] && assign[1] != assign[2]);
    }

    // Example 4: No overlap, all can share one room
    {
        auto [rooms, assign] = assign_rooms({{1, 2}, {3, 4}, {5, 6}});
        assert(rooms == 1);
        for (int r : assign) assert(r == 1);
    }

    // Example 5: Already sorted? Mixed order
    {
        auto [rooms, assign] = assign_rooms({{5, 6}, {1, 2}, {3, 4}});
        assert(rooms == 1);
        for (int r : assign) assert(r == 1);
    }

    // Example 6: Large non-overlapping with gaps
    {
        auto [rooms, assign] = assign_rooms({{0, 0}, {1, 1}, {2, 2}});
        assert(rooms == 1);
    }

    // Example 7: Complex overlap requiring 3 rooms
    {
        auto [rooms, assign] = assign_rooms({{1, 4}, {2, 3}, {3, 5}, {6, 8}});
        assert(rooms == 3);
        // Verify valid coloring: Check each guest's room and ensure no conflicts.
        for (size_t i = 0; i < 4; ++i) {
            assert(assign[i] >= 1 && assign[i] <= rooms);
            for (size_t j = i + 1; j < 4; ++j) {
                // Overlap condition: not (dep_i < arr_j or dep_j < arr_i)
                bool overlap = !( (4 < assign_j_arr) ); // Let's just check directly:
                // We need the original data to check properly. Use a lambda.
            }
        }
        // Simpler: just assert the three overlapping guests (0,1,2) have distinct rooms.
        assert(assign[0] != assign[1]);
        assert(assign[0] != assign[2]);
        assert(assign[1] != assign[2]);
        // Guest3 can reuse a room if free, but at least verification is ok.
        assert(assign[3] >= 1 && assign[3] <= 3);
    }

    // Example 8: Single guest
    {
        auto [rooms, assign] = assign_rooms({{10, 20}});
        assert(rooms == 1);
        assert(assign[0] == 1);
    }

    // Example 9: Empty input
    {
        auto [rooms, assign] = assign_rooms({});
        assert(rooms == 0);
        assert(assign.empty());
    }

    // Example 10: Strict inequality edge: arrival equal to previous departure
    {
        auto [rooms, assign] = assign_rooms({{1, 3}, {3, 5}});
        assert(rooms == 2); // because at time 3 both departure and arrival, cannot share.
        assert(assign[0] != assign[1]);
    }

    return 0;
}
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

// Assign rooms to guests to minimize total rooms used, given (arrival, departure) pairs.
// Returns {minimum_rooms, assignment_vector} where assignment[i] is the room number (1-based) for guest i.
std::pair<int, std::vector<int>> assign_rooms(const std::vector<std::pair<int, int>>& guests) {
    const size_t n = guests.size();
    std::vector<int> assignment(n, 0);

    // Sort guests by arrival time, keeping track of original index.
    std::vector<std::pair<std::pair<int, int>, int>> sorted_guests;
    sorted_guests.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        sorted_guests.push_back({{guests[i].first, guests[i].second}, static_cast<int>(i)});
    }
    std::sort(sorted_guests.begin(), sorted_guests.end(),
              [](const auto& a, const auto& b) { return a.first.first < b.first.first; });

    // Min-heap of (departure_time, room_number). Ordered by departure time.
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> occupied_rooms;

    int room_count = 0;

    for (const auto& guest : sorted_guests) {
        int arrival = guest.first.first;
        int departure = guest.first.second;
        int original_index = guest.second;

        if (!occupied_rooms.empty() && occupied_rooms.top().first < arrival) {
            // Reuse the room that frees up earliest.
            int reusable_room = occupied_rooms.top().second;
            occupied_rooms.pop();
            occupied_rooms.push({departure, reusable_room});
            assignment[original_index] = reusable_room;
        } else {
            // Need a new room.
            ++room_count;
            occupied_rooms.push({departure, room_count});
            assignment[original_index] = room_count;
        }
    }

    return {room_count, assignment};
}
// The problem is a classic interval graph coloring problem, equivalent to scheduling meetings in a minimal number of rooms. Sort all guests by arrival time. Maintain a min-heap of currently occupied rooms, where the heap is ordered by departure time, and also stores the room number. For each guest in sorted order, if the room with the earliest departure is free (i.e., this guest's arrival time is strictly greater than that room's departure time), then that room can be reused; pop the heap entry, push the current guest's departure time with that same room number, and assign that room to the guest. Otherwise, if the earliest departure is greater than or equal to the current arrival (or heap is empty), allocate a new room (increment room count), push it, and assign it. This greedy algorithm works because assigning a new room only when necessary ensures minimal room usage; reusing the earliest-free room is optimal since all later-arriving guests have even later arrival times. Edge cases: guests with zero length (arrival == departure) still occupy a room for that single time unit, so they cannot share with anyone whose stay overlaps that exact time; the condition `>` handles this correctly (a guest departing at time 5 cannot share with an arrival at 5, so reuse only when `arrival > earliest_departure`). Time complexity is O(n log n) due to sorting and heap operations; space complexity is O(n) for the heap and assignment vector.
