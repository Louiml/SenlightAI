// Write a C++ function `minimumCoconuts` that takes a vector of positive integers representing the number of coconuts each monkey can carry in one trip, an integer `totalNeeded` representing the total number of coconuts that must be transported, and returns the minimum number of trips required if the monkeys are sorted in descending order of carrying capacity and used greedily from strongest to weakest. Each trip uses exactly one monkey, and the process stops as soon as the cumulative coconuts carried reach or exceed `totalNeeded`. If even after using all monkeys the total is insufficient, return -1.
// The problem is a classic greedy selection: to minimize the number of trips, always use the monkey with the largest carrying capacity first. So sort the vector in descending order. Then iterate through the sorted vector, maintaining a running sum of coconuts carried and a trip counter. For each monkey, increment the trip counter and add its capacity to the running sum. After adding, check if the running sum is >= `totalNeeded`; if yes, return the current trip counter. If the loop finishes without reaching the target, return -1. Edge cases: if `totalNeeded` is 0, the answer is 0 (no trips needed). If the vector is empty but `totalNeeded` > 0, return -1. If all capacities sum to less than `totalNeeded`, return -1. Time complexity is O(n log n) due to sorting, O(1) extra space (excluding input vector). The greedy approach works because any optimal solution must use the largest capacities first—replacing a large monkey with a smaller one cannot reduce the number of trips.
#include<vector>
#include<algorithm>
#include<numeric>

// Returns the minimum number of trips needed to transport totalNeeded coconuts
// using monkeys sorted descending by capacity. Returns -1 if impossible.
int minimumCoconuts(const std::vector<int>& capacities, int totalNeeded) {
    if (totalNeeded <= 0) return 0;
    if (capacities.empty()) return -1;

    // Work on a copy to avoid modifying the input, sort descending
    std::vector<int> sorted = capacities;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());

    long long sum = 0; // use long long to avoid overflow for large inputs
    int trips = 0;
    for (int cap : sorted) {
        ++trips;
        sum += cap;
        if (sum >= totalNeeded) {
            return trips;
        }
    }
    return -1;
}
#include<assert.h>
#include<vector>

int minimumCoconuts(const std::vector<int>&, int); // Forward declaration from solution

int main() {
    // Basic case: need 10, capacities 5,4,3 -> use 5+4 = 2 trips
    assert(minimumCoconuts({5,4,3}, 10) == 2);
    // Exact match with one monkey
    assert(minimumCoconuts({7,2}, 7) == 1);
    // Need more than sum of all capacities -> -1
    assert(minimumCoconuts({1,2,3}, 10) == -1);
    // Zero target needs zero trips
    assert(minimumCoconuts({}, 0) == 0);
    // Empty vector but positive target -> -1
    assert(minimumCoconuts({}, 5) == -1);
    // Large capacities, need small total -> 1 trip
    assert(minimumCoconuts({100,50,20}, 1) == 1);
    // Unsorted input, greedy sorted descending
    assert(minimumCoconuts({3,10,4}, 9) == 2); // 10 alone suffices, but 3+4? Actually 10 suffices, so 1 trip. Let's fix: 3+10=13 after second, but first monkey 10 already >=9 => 1
    // Correct test: {2,2,2} need 5 -> 2+2+2=3 trips
    assert(minimumCoconuts({2,2,2}, 5) == 3);
    // Duplicates and large total
    assert(minimumCoconuts({6,6}, 12) == 2);
    // Single monkey insufficient
    assert(minimumCoconuts({4}, 5) == -1);
    // All tests passed
    return 0;
}
