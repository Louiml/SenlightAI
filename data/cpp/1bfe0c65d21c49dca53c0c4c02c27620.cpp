/*
You are given the number of train stops `n` (1 ≤ n ≤ 1500). At each stop `i` (0-indexed), a train records the number of passengers who exit (`exit[i]`) and the number who enter (`entry[i]`) at that stop. The train starts its journey before stop 0 with exactly 0 passengers on board. Your task is to write a C++ function that, given these two arrays, returns the maximum number of passengers on the train at any point after any stop (i.e., immediately after passengers exit and enter at that stop). You must handle the possibility that the train could have 0 passengers at some stops, but the maximum could be a positive number. The function should return the absolute value of that maximum count (which will always be a non-negative integer). For example, if the maximum passenger count at any stop is 34, return 34. The input arrays are of equal length `n`.
*/

#include <vector>
#include <algorithm>
#include <cstdlib> // for std::abs

// Returns the maximum absolute number of passengers on the train after any stop.
int maxPassengersAfterStops(const std::vector<int>& exitCounts, const std::vector<int>& entryCounts) {
    int n = static_cast<int>(exitCounts.size());
    int currentPassengers = 0;
    int maxSeen = 0; // train starts with 0 passengers

    for (int i = 0; i < n; ++i) {
        // Passengers after this stop = current - exits + entries
        currentPassengers = currentPassengers - exitCounts[i] + entryCounts[i];
        // Update maximum with the absolute value of the current count
        maxSeen = std::max(maxSeen, std::abs(currentPassengers));
    }
    return maxSeen;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Case 1: Simple increasing passengers
    std::vector<int> exit1 = {0, 0, 0};
    std::vector<int> entry1 = {5, 3, 2};
    assert(maxPassengersAfterStops(exit1, entry1) == 10);

    // Case 2: Passengers exit more than enter at a stop (count may go negative)
    std::vector<int> exit2 = {0, 10, 0};
    std::vector<int> entry2 = {5, 3, 0};
    // After stop 0: 5, after stop1: 5-10+3=-2, after stop2: -2-0+0=-2 → max abs = 5
    assert(maxPassengersAfterStops(exit2, entry2) == 5);

    // Case 3: Single stop with net negative
    std::vector<int> exit3 = {10};
    std::vector<int> entry3 = {4};
    assert(maxPassengersAfterStops(exit3, entry3) == 6);

    // Case 4: All zeros
    std::vector<int> exit4 = {0, 0, 0};
    std::vector<int> entry4 = {0, 0, 0};
    assert(maxPassengersAfterStops(exit4, entry4) == 0);

    // Case 5: Large values, alternating
    std::vector<int> exit5 = {3, 1, 4};
    std::vector<int> entry5 = {10, 2, 5};
    // after0:7, after1:8, after2:9 → max 9
    assert(maxPassengersAfterStops(exit5, entry5) == 9);

    // Case 6: Negative absolute maximum occurs
    std::vector<int> exit6 = {0, 100};
    std::vector<int> entry6 = {5, 5};
    // after0:5, after1:-90 → max abs = 90
    assert(maxPassengersAfterStops(exit6, entry6) == 90);

    // Case 7: Constant passengers
    std::vector<int> exit7 = {2, 2, 2};
    std::vector<int> entry7 = {5, 5, 5};
    // after0:3, after1:6, after2:9 → max 9
    assert(maxPassengersAfterStops(exit7, entry7) == 9);

    // Case 8: n=1, positive
    std::vector<int> exit8 = {1};
    std::vector<int> entry8 = {7};
    assert(maxPassengersAfterStops(exit8, entry8) == 6);

    // Case 9: n=1, zero net
    std::vector<int> exit9 = {4};
    std::vector<int> entry9 = {4};
    assert(maxPassengersAfterStops(exit9, entry9) == 0);

    // Case 10: Mixed with zero entries
    std::vector<int> exit10 = {2, 0, 0};
    std::vector<int> entry10 = {0, 5, 1};
    // after0:-2, after1:3, after2:4 → max abs = 4
    assert(maxPassengersAfterStops(exit10, entry10) == 4);

    return 0;
}

// The core idea is to simulate the passenger count as we process each stop sequentially. We maintain the current passenger count after processing each stop. Initially, before any stop, the count is 0. For stop `i`, we first subtract the number of passengers who exit (`exit[i]`), then add the number who enter (`entry[i]`). However, we must be careful: passengers who exit at stop `i` are from the count before that stop, and passengers who enter contribute to the new count. So the new count after stop `i` is `current = current - exit[i] + entry[i]`. But the given snippet uses an in-place modification of the `entry` array to store the cumulative count: `entry[i] = entry[i-1] - exit[i] + entry[i]` (when `i>0`), and for `i==0` it sets `entry[0] = entry[0]` (since initial passengers are 0, so after stop 0, count = 0 - exit[0] + entry[0] = entry[0] - exit[0]? Actually the snippet has a bug: it only updates for `i!=0`, and for `i==0` it doesn't set `temp` or update. But the intended logic is: after each stop, the new passenger count is `previous_count - exit[i] + entry[i]`. We track the maximum of all such counts, and because the problem statement (from the snippet) uses `abs(entry[i])` in the max check, we also consider the possibility of negative intermediate counts if more exit than enter, but since the train starts at 0 and we add after subtracting, the count can go negative only if exits exceed current passengers, which logically shouldn't happen in real trains but the snippet allows it. To be safe, we take the absolute value of the count at each stop when updating the maximum. Edge cases: n=1, the count after the only stop is `entry[0] - exit[0]` (since initial 0). If that is negative, we take abs. The maximum should be initialized to 0 because the train starts with 0 passengers, but the snippet initializes to -10000, then only updates inside the loop, so if n=1 and the count is positive, it works; if the count is negative, abs makes it positive, so it still works. Our solution will initialize max to 0 to be safe. Time complexity is O(n), space complexity is O(1) extra if we avoid modifying the input arrays (we'll use a separate variable for current count).
