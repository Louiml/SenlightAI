/*
Write a C++ function named `findMinimumPlatforms` that takes two `std::vector<int>` references: `arrivals` (train arrival times) and `departures` (train departure times), both measured in minutes since midnight and sorted in non-decreasing order relative to each train (i.e., the i-th arrival corresponds to the i-th departure, but the vectors themselves are not necessarily sorted). The function must return the minimum number of railway platforms needed so that no train waits, meaning that at any moment the number of trains present (arrived but not yet departed) never exceeds the number of platforms. Trains can share a platform if one departs before or exactly at the same time another arrives (a departure at time `t` frees the platform for an arrival at exactly time `t`). The vectors are non-empty and of equal length, with all times being non-negative integers and each departure time strictly greater than its corresponding arrival time. You may modify the input vectors if needed (e.g., by sorting copies). Provide a function with `const` correctness where appropriate, and do not include any `main` function in your solution.
*/

#include <vector>
#include <algorithm>

// Function to find the minimum number of platforms required so that no train waits.
// arr[i] is the arrival time of train i, dep[i] is the departure time of train i.
// Times are in minutes since midnight. dep[i] > arr[i] for all i.
int findMinimumPlatforms(std::vector<int>& arrivals, std::vector<int>& departures) {
    // Sort both arrays independently to process events in time order.
    std::sort(arrivals.begin(), arrivals.end());
    std::sort(departures.begin(), departures.end());

    int n = arrivals.size();
    int platformsNeeded = 0;
    int maxPlatforms = 0;
    int i = 0; // index for arrivals
    int j = 0; // index for departures

    while (i < n) {
        // If the next arrival is strictly before the next departure, we need a new platform.
        // If they are equal, we should process the departure first (free a platform), so we go to the else branch.
        if (i < n && (j == n || arrivals[i] < departures[j])) {
            platformsNeeded++;
            if (platformsNeeded > maxPlatforms) {
                maxPlatforms = platformsNeeded;
            }
            i++;
        } else {
            // A departure occurs (either because no more arrivals, or next departure is <= next arrival)
            platformsNeeded--;
            j++;
        }
    }
    return maxPlatforms;
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple case with overlapping trains
    std::vector<int> arr1 = {900, 940, 950, 1100, 1500, 1800};
    std::vector<int> dep1 = {910, 1200, 1120, 1130, 1900, 2000};
    assert(findMinimumPlatforms(arr1, dep1) == 3);

    // Test 2: No overlap (sequential trains)
    std::vector<int> arr2 = {100, 200, 300};
    std::vector<int> dep2 = {150, 250, 350};
    assert(findMinimumPlatforms(arr2, dep2) == 1);

    // Test 3: All trains overlap maximally
    std::vector<int> arr3 = {0, 1, 2, 3};
    std::vector<int> dep3 = {10, 11, 12, 13};
    assert(findMinimumPlatforms(arr3, dep3) == 4);

    // Test 4: Arrival exactly at departure time (should not need an extra platform)
    std::vector<int> arr4 = {100, 100, 200};
    std::vector<int> dep4 = {100, 150, 200};
    // Train 1 arrives at 100, departs at 100 (but dep > arr is violated in this test) – but we assume valid input. Use valid:
    std::vector<int> arr5 = {100, 200};
    std::vector<int> dep5 = {200, 300};
    assert(findMinimumPlatforms(arr5, dep5) == 1); // because train 1 departs at 200, train 2 arrives at 200

    // Test 5: Single train
    std::vector<int> arr6 = {500};
    std::vector<int> dep6 = {600};
    assert(findMinimumPlatforms(arr6, dep6) == 1);

    // Test 6: Trains with same arrival and departure times but different pairs
    std::vector<int> arr7 = {1, 2, 3};
    std::vector<int> dep7 = {2, 3, 4};
    assert(findMinimumPlatforms(arr7, dep7) == 1); // each departs before next arrives? Wait: 1-2, 2-3, 3-4 -> at time 2, one departs and another arrives -> no overlap? Actually at time 2, one departs (train1) and another arrives (train2) – they don't overlap because departure at 2 and arrival at 2: with our rule, departure processed first, so only 1 platform needed. Correct.

    // Test 7: A more complex case with ties
    std::vector<int> arr8 = {1, 2, 2, 3};
    std::vector<int> dep8 = {3, 4, 5, 6};
    // Train1: 1-3, Train2: 2-4, Train3: 2-5, Train4: 3-6. At t=2, two arrivals, one train (train1) still present? At t=2, train1 present, train2 and train3 arrive -> 3 platforms. At t=3, train1 departs and train4 arrives -> still 3. So answer = 3.
    assert(findMinimumPlatforms(arr8, dep8) == 3);

    return 0;
}

// The classic greedy approach is to sort the arrival and departure times independently into two sorted arrays, then use a two-pointer technique. Since the minimum number of platforms is determined solely by the maximum overlap of trains at any time, sorting both lists allows us to simulate time by advancing through arrivals and departures in chronological order. Initialize `platformsNeeded = 0` and `maxPlatforms = 0`. Use two indices `i` and `j` pointing to the earliest unsorted arrival and departure. While `i < n`: if the next arrival time is less than or equal to the next departure time (i.e., `arrivals[i] <= departures[j]`), it means a train arrives before or exactly when the earliest departure happens, so we increment `platformsNeeded`, update `maxPlatforms`, and move `i` forward. Otherwise, a train has departed, so we decrement `platformsNeeded` and move `j` forward. This works because sorting separately loses the original pairing but preserves the event order; we don't need the pairing—only the times matter. Edge cases: if `arrivals[i] == departures[j]`, the departure should be processed first (i.e., we should treat it as a departure event before an arrival), but the condition `arrivals[i] <= departures[j]` counts the arrival first, which would overcount by one. To handle this correctly, we use `<` instead of `<=` for the arrival check; if they are equal, we advance `j` first (departure) to free a platform. Alternatively, we can use the common approach: check `if (arrivals[i] <= departures[j])` but then process departures before arrivals when equal by using a `while` loop that first checks departures. In the standard solution, we often use `if (arrivals[i] <= departures[j])` and then increment i, else increment j—this counts trains that arrive before or exactly at the same time as a departure, which incorrectly adds a platform when they are equal. The correct method is to check `arrivals[i] < departures[j]` for arrival, and if equal, treat it as departure first. The reference solution below uses the corrected logic. Time complexity: O(n log n) due to sorting, where n is the number of trains. Space complexity: O(1) auxiliary if we sort in-place (but the function receives non-const references, so we can sort them directly), plus O(n) for the input vectors themselves.
