/*
Write a C++ function `maxShopsVisited` that takes two vectors of integers `start` and `end` (both of the same size, representing the opening and closing times of shops on a single day) and an integer `k` (the number of shopping trips available). Each trip can visit any number of shops sequentially, but a trip cannot visit a shop that has already closed when the trip arrives, and a trip must finish visiting a shop before it closes. The function should return the maximum number of shops that can be visited across all `k` trips combined. Assume `k >= 1`, all times are non-negative, and each shop has `start[i] < end[i]`. You may reorder shops arbitrarily and assign them to trips in any order.
*/
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum number of shops that can be visited with k trips.
// Each shop i has opening time start[i] and closing time end[i].
// A trip can visit a shop only if the trip is free at or before start[i],
// and after visiting, the trip becomes busy until end[i].
int maxShopsVisited(const std::vector<int>& start, const std::vector<int>& end, int k) {
    int n = static_cast<int>(start.size());
    std::vector<std::pair<int, int>> shops;
    shops.reserve(n);
    for (int i = 0; i < n; ++i) {
        shops.emplace_back(end[i], start[i]); // sort by end time
    }
    std::sort(shops.begin(), shops.end());

    // endTimes[j] = the earliest time trip j is free (i.e., its last assigned shop's end time)
    std::vector<int> endTimes(k, 0);
    int count = 0;

    for (int i = 0; i < n; ++i) {
        int shopStart = shops[i].second;
        int shopEnd = shops[i].first;
        // Find a trip that is free at shopStart (endTime <= shopStart)
        // Greedily assign to the trip with the smallest endTime that fits.
        int bestTrip = -1;
        int bestEnd = -1;
        for (int j = 0; j < k; ++j) {
            if (endTimes[j] <= shopStart) {
                if (bestTrip == -1 || endTimes[j] < bestEnd) {
                    bestTrip = j;
                    bestEnd = endTimes[j];
                }
            }
        }
        if (bestTrip != -1) {
            endTimes[bestTrip] = shopEnd;
            ++count;
        }
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Example from the snippet
    std::vector<int> S1 = {1, 8, 3, 2, 6};
    std::vector<int> E1 = {5, 10, 6, 5, 9};
    assert(maxShopsVisited(S1, E1, 2) == 4);

    // Simple test: each trip can visit one shop
    std::vector<int> S2 = {1, 2, 3};
    std::vector<int> E2 = {3, 4, 5};
    assert(maxShopsVisited(S2, E2, 2) == 2);

    // All shops can fit in one trip if sorted by end
    std::vector<int> S3 = {1, 2, 3};
    std::vector<int> E3 = {2, 3, 4};
    assert(maxShopsVisited(S3, E3, 1) == 3);

    // Overlapping shops force skipping
    std::vector<int> S4 = {1, 1, 1};
    std::vector<int> E4 = {2, 2, 2};
    assert(maxShopsVisited(S4, E4, 2) == 2);

    // More trips than shops
    std::vector<int> S5 = {5, 6};
    std::vector<int> E5 = {7, 8};
    assert(maxShopsVisited(S5, E5, 10) == 2);

    // Non-overlapping but not sorted input
    std::vector<int> S6 = {10, 0, 5};
    std::vector<int> E6 = {11, 1, 6};
    assert(maxShopsVisited(S6, E6, 1) == 3);

    // Zero-time windows? Not allowed but ensure no crash if endpoints equal
    std::vector<int> S7 = {1, 2};
    std::vector<int> E7 = {1, 2}; // technically invalid, but test robustness
    assert(maxShopsVisited(S7, E7, 1) == 1); // only one fits because end==start for first

    // k=1 with many overlapping
    std::vector<int> S8 = {0, 1, 2, 3};
    std::vector<int> E8 = {10, 11, 12, 13};
    assert(maxShopsVisited(S8, E8, 1) == 4);

    // Single shop
    std::vector<int> S9 = {0};
    std::vector<int> E9 = {1};
    assert(maxShopsVisited(S9, E9, 1) == 1);

    return 0;
}
// This is a classic interval scheduling problem extended to multiple resources (trips). The key observation is that to maximize the total number of shops visited, we should greedily assign shops to trips in order of their ending times. Sort all shops by their end time (ascending). Maintain an array of `k` end times, one per trip, initialized to 0. For each shop in sorted order, try to assign it to the trip that has the smallest end time that is less than or equal to the shop's start time; if such a trip exists, assign the shop to that trip, update that trip's end time to the shop's end time, and increment the count. This greedy works because sorting by end time ensures we consider the most urgent shops first, and assigning to the earliest available trip frees up other trips for future shops. Edge cases: multiple shops with the same end time are handled naturally by the sorting order; if no trip is free, skip the shop; if `k` is larger than the number of shops, all shops can be visited. Time complexity is O(n log n) for sorting plus O(n * k) for the assignment loop, so O(n * k) overall if `k` is considered variable; if `k` is small relative to `n`, it's effectively O(n log n). Space complexity is O(n + k) for storing the sorted pairs and the trip end times.
