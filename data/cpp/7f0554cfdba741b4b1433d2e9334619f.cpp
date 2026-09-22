Create a C++ function named `minimumPlatforms` that takes two arrays of integers representing the arrival times and departure times of trains at a station, along with the number of trains `n`, and returns the minimum number of platforms required so that no train waits. Arrival times are sorted in non-decreasing order and departure times are sorted in non-decreasing order independently (the arrays are not necessarily paired). The function must handle cases where trains arrive and depart at exactly the same time: treat a departure as occurring before an arrival at the same time. Assume `n >= 1` and all times are non-negative integers.

// The optimal solution uses a two-pointer technique after sorting both arrays independently. Sort the arrival array and the departure array in ascending order. Initialize two indices: `i` for arrivals and `j` for departures, both starting at 0. Maintain a counter `currentPlatforms` and a variable `maxPlatforms` initialized to 0. While `i < n`, compare `arr[i]` and `dep[j]`. Because a departure at the same time as an arrival is processed first (as per the problem statement), we check if `arr[i] > dep[j]`: if true, a train departs before the next arrival, so decrement the counter and increment `j`. Otherwise, a new train arrives (including the equal-time case), so increment the counter and increment `i`. After each step, update `maxPlatforms` with the maximum of itself and `currentPlatforms`. The loop continues until all arrivals are processed. Edge cases: if all trains arrive before any departure, the counter grows monotonically. If arrivals are all after departures, the counter never exceeds 1. Time complexity is O(n log n) due to sorting, and space complexity is O(1) auxiliary (ignoring sort overhead).

#include <algorithm>
#include <vector>

// Returns the minimum number of platforms needed so no train waits.
// Departures at the same time as arrivals are processed first.
int minimumPlatforms(const std::vector<int>& arrivals, const std::vector<int>& departures) {
    int n = static_cast<int>(arrivals.size());
    std::vector<int> arr = arrivals;
    std::vector<int> dep = departures;

    std::sort(arr.begin(), arr.end());
    std::sort(dep.begin(), dep.end());

    int i = 0; // index for arrivals
    int j = 0; // index for departures
    int currentPlatforms = 0;
    int maxPlatforms = 0;

    while (i < n) {
        if (arr[i] > dep[j]) {
            // A train departs before the next arrival
            --currentPlatforms;
            ++j;
        } else {
            // A train arrives (including same-time case, departure handled first)
            ++currentPlatforms;
            ++i;
        }
        maxPlatforms = std::max(maxPlatforms, currentPlatforms);
    }

    return maxPlatforms;
}

#include <cassert>
#include <vector>

int minimumPlatforms(const std::vector<int>& arrivals, const std::vector<int>& departures);

int main() {
    // Basic examples
    assert(minimumPlatforms({900, 940, 950, 1100, 1500, 1800},
                             {910, 1200, 1120, 1130, 1900, 2000}) == 3);
    assert(minimumPlatforms({900, 1100, 1235},
                             {1000, 1200, 1240}) == 1);
    assert(minimumPlatforms({1000, 1000, 1100},
                             {1000, 1100, 1200}) == 2); // same-time departure first

    // Single train
    assert(minimumPlatforms({500}, {600}) == 1);

    // All arrivals before any departure
    assert(minimumPlatforms({1, 2, 3}, {10, 11, 12}) == 3);

    // All departures before next arrival
    assert(minimumPlatforms({10, 20, 30}, {11, 21, 31}) == 1);

    // Equal times with many trains
    assert(minimumPlatforms({5, 5, 5}, {5, 5, 5}) == 3); // all depart at same time as arrivals, each arrival adds one

    // Non-contiguous times with duplicates
    assert(minimumPlatforms({1, 1, 2, 2}, {2, 2, 3, 3}) == 2);

    // Large times
    assert(minimumPlatforms({0, 1000000}, {1, 1000001}) == 1);

    // Already sorted but unsorted input arrays (function sorts internally)
    assert(minimumPlatforms({300, 100, 200}, {150, 250, 350}) == 2);

    return 0;
}
