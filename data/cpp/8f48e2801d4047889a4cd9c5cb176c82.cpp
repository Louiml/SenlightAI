Write a C++ function that takes two arrays of integers, `arrival` and `departure`, each of size `n`, representing the arrival and departure times of `n` trains at a platform (times are in 24-hour format, e.g., 900 for 9:00 AM, 1730 for 5:30 PM). The function should return the minimum number of platforms required so that no train has to wait for another train to leave before it can arrive. A train can occupy a platform from its arrival time (inclusive) up to its departure time (inclusive). If an arrival time is less than or equal to the departure time of the previous train, they overlap and need different platforms. Use an efficient sorting-based approach.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> at1 = {900, 940, 950, 1100, 1500, 1800};
    std::vector<int> dt1 = {910, 1200, 1120, 1130, 1900, 2000};
    assert(minimumPlatforms(at1, dt1) == 3);

    // Single train
    assert(minimumPlatforms({1000}, {1100}) == 1);

    // Empty input
    assert(minimumPlatforms({}, {}) == 0);

    // No overlap (sequential)
    std::vector<int> at2 = {1000, 1100, 1200};
    std::vector<int> dt2 = {1030, 1130, 1230};
    assert(minimumPlatforms(at2, dt2) == 1);

    // All at same time (max overlap)
    std::vector<int> at3 = {900, 900, 900};
    std::vector<int> dt3 = {1000, 1000, 1000};
    assert(minimumPlatforms(at3, dt3) == 3);

    // Overlap at boundaries (arrival == departure of previous)
    std::vector<int> at4 = {1000, 1000};
    std::vector<int> dt4 = {1000, 1100};
    assert(minimumPlatforms(at4, dt4) == 2);

    // All same arrival but different departures
    std::vector<int> at5 = {900, 900, 900};
    std::vector<int> dt5 = {901, 902, 903};
    assert(minimumPlatforms(at5, dt5) == 3);

    // Random mix
    std::vector<int> at6 = {800, 830, 900, 930, 1000, 1030};
    std::vector<int> dt6 = {850, 910, 950, 1010, 1040, 1100};
    assert(minimumPlatforms(at6, dt6) == 2);

    return 0;
}
#include <algorithm>
#include <vector>

// Returns the minimum number of platforms required so that no train waits,
// given arrival and departure times (both inclusive). Works in O(n log n).
int minimumPlatforms(const std::vector<int>& arrival, const std::vector<int>& departure) {
    int n = arrival.size();
    if (n == 0) return 0;

    // Make local copies since we need to sort them
    std::vector<int> at = arrival;
    std::vector<int> dt = departure;
    std::sort(at.begin(), at.end());
    std::sort(dt.begin(), dt.end());

    int i = 1;      // pointer for arrivals (first train already counted)
    int j = 0;      // pointer for departures
    int count = 1;  // current number of platforms in use
    int ans = 1;    // final answer

    while (i < n && j < n) {
        if (at[i] <= dt[j]) {
            // New train arrives before or at the same time as a departure
            count++;
            i++;
        } else {
            // A train has departed before this arrival
            count--;
            j++;
        }
        ans = std::max(ans, count);
    }

    return ans;
}
// The classic greedy solution sorts both arrival and departure times separately in ascending order. We then use two-pointer technique: start with `i=1` for arrivals (the first train already occupies one platform) and `j=0` for departures. Maintain a running count of currently active trains. If the current arrival time is less than or equal to the current departure time (meaning the new train arrives before or exactly when the previous departs), we need an extra platform, so increment count and move `i`. Otherwise, a train has departed, so decrement count and move `j`. At each step, update the answer to the maximum count seen. This works because sorting independent arrays is sufficient—each arrival or departure event is processed in chronological order, and we only care about overlaps. Edge cases: `n=0` (return 0), `n=1` (return 1), duplicate times (e.g., arrival==departure counts as overlap, so we need separate platforms), and all trains arriving simultaneously but departing sequentially. Time complexity is O(n log n) due to sorting, space complexity is O(1) auxiliary (ignoring the input arrays). The function should not modify the original arrays, so we should copy them before sorting to maintain const correctness.
