/*
Write a C++ function `int countRecentPings(const std::vector<int>& timestamps)` that processes a list of ping timestamps in non-decreasing order and returns the number of pings that occurred within the last 3000 milliseconds (inclusive) of the final timestamp. More specifically, you need to simulate a rate limiter that tracks how many requests were received in the sliding window `[lastPing - 3000, lastPing]`. For each timestamp, you must count how many of all previous and current pings fall within 3000 ms of that timestamp, but the function only needs to return the count for the *last* timestamp after processing all pings in chronological order. The input timestamps are guaranteed to be strictly increasing. For example, if `timestamps = {1, 2, 3001, 3002}`, then for the final timestamp 3002, the valid pings are those with time >= 2 (i.e., 2, 3001, 3002), so the answer is 3. If the input is empty, return 0. Implement the solution without modifying the input vector.
*/

#include <vector>
#include <queue>

// Returns the number of pings that fall within the 3000 ms window
// ending at the last timestamp in the input vector.
// Input timestamps are strictly increasing; returns 0 for empty input.
int countRecentPings(const std::vector<int>& timestamps) {
    std::queue<int> recentPings;
    int finalCount = 0;

    for (int t : timestamps) {
        // Define the lower bound for the sliding window.
        int lowerBound = t - 3000;
        // Remove pings that are older than the lower bound.
        while (!recentPings.empty() && recentPings.front() < lowerBound) {
            recentPings.pop();
        }
        // Add the current ping.
        recentPings.push(t);
        // Update the count for the current timestamp.
        finalCount = static_cast<int>(recentPings.size());
    }

    return finalCount;
}

#include <cassert>
#include <vector>

int main() {
    // Single ping
    assert(countRecentPings({1}) == 1);

    // Empty input
    assert(countRecentPings({}) == 0);

    // All pings within 3000 ms of the last
    assert(countRecentPings({1, 2, 3, 3000}) == 4);

    // Pings outside the window are removed
    assert(countRecentPings({1, 2, 3001, 3002}) == 3);

    // Larger gap clears the window
    assert(countRecentPings({100, 200, 3100, 3200}) == 2);

    // Multiple windows, final one has only last two
    assert(countRecentPings({0, 1000, 2000, 5000, 5001}) == 2);

    // Strictly increasing but with very large timestamps
    assert(countRecentPings({1000000, 1000500, 1003001}) == 3);

    // Boundary: exactly 3000 ms apart are included
    assert(countRecentPings({500, 3500}) == 2);

    // Boundary: exactly 3001 ms apart are excluded
    assert(countRecentPings({500, 3501}) == 1);

    // Many pings, only final window counts
    assert(countRecentPings({1, 2, 3, 4, 5, 4000, 4001, 4002}) == 3);

    return 0;
}

// The core algorithm is to maintain a sliding window over the timestamps. Since the timestamps are already sorted in strictly increasing order, we can use two pointers: one for the "left" boundary of the window and one for iterating through the timestamps. As we iterate through each timestamp, we first remove from the front any timestamp that is older than `currentTime - 3000` (i.e., strictly less than that lower bound). Then we include the current timestamp. The number of elements currently in the window is the count for that timestamp. For the final answer, we only need the count after processing the last timestamp. Alternatively, we can mimic the `RecentCounter` class by using a queue: push each timestamp and pop the front while it is less than `t - 3000`. The size of the queue after each push/pop gives the count for that timestamp, and we just return the size after the last one. Edge cases: empty input returns 0; a single timestamp always returns 1 (since it is within 3000 ms of itself); strictly increasing timestamps guarantee that the queue's front is always the oldest, so no sorting is needed. Time complexity is O(n) because each timestamp is pushed and popped at most once. Space complexity is O(n) in the worst case (e.g., if all timestamps are within a 3000 ms window), but the queue size is bounded by the number of timestamps. For the function signature, we avoid a class and use a plain function for simplicity.
