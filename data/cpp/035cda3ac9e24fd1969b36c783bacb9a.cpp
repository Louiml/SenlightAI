Given a vector of sorted integer arrays, write a C++ function `int maxDistance(const std::vector<std::vector<int>>& arrays)` that returns the maximum absolute difference between any two integers that come from *different* arrays. Each inner array is non-empty and sorted in non-decreasing order. You may assume there are at least two arrays. The function should avoid comparing elements from the same array, and it must run efficiently without storing all elements together.

#include <cassert>
#include <vector>

int main() {
    // Basic example from problem
    std::vector<std::vector<int>> arrays1 = {{1, 2, 3}, {4, 5}, {1, 2, 3}};
    assert(maxDistance(arrays1) == 4); // 4-1=3 or 3-1=2? Actually max is |4-1|=3? Let's compute: globalMax=3,globalMin=1 after first; then array2: |3-4|=1, |1-5|=4 -> result=4; update globalMax=5, globalMin=1; array3: |5-1|=4, |1-3|=2 -> result=4. So correct.

    // Single-element arrays
    std::vector<std::vector<int>> arrays2 = {{5}, {10}, {1}};
    assert(maxDistance(arrays2) == 9); // |1-10|=9

    // Negative numbers
    std::vector<std::vector<int>> arrays3 = {{-10, -5}, {-1, 0}, {2}};
    assert(maxDistance(arrays3) == 12); // | -10 - 2 | = 12

    // Two arrays only
    std::vector<std::vector<int>> arrays4 = {{1, 2}, {3, 4}};
    assert(maxDistance(arrays4) == 3); // |1-4|=3 or |3-2|=1

    // Duplicates within arrays
    std::vector<std::vector<int>> arrays5 = {{1, 1, 1}, {2, 2}};
    assert(maxDistance(arrays5) == 1);

    // Large difference
    std::vector<std::vector<int>> arrays6 = {{0, 0}, {1000000, 1000000}};
    assert(maxDistance(arrays6) == 1000000);

    // All equal arrays
    std::vector<std::vector<int>> arrays7 = {{3, 3}, {3, 3}};
    assert(maxDistance(arrays7) == 0);

    // Unsorted inner? Not required, but test with sorted input
    std::vector<std::vector<int>> arrays8 = {{-5, -4, -3}, {-2, -1, 0}, {1, 2, 3}};
    assert(maxDistance(arrays8) == 8); // | -5 - 3 | = 8

    return 0;
}

#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns the maximum absolute difference between any two integers from different arrays.
// Each inner array is sorted in non-decreasing order and is non-empty.
int maxDistance(const std::vector<std::vector<int>>& arrays) {
    // Initialize global min/max from the first array
    int globalMin = arrays[0].front();
    int globalMax = arrays[0].back();
    int result = 0;

    // Process remaining arrays, comparing only with previous arrays' extremes
    for (size_t i = 1; i < arrays.size(); ++i) {
        // Current array's smallest and largest
        int currentMin = arrays[i].front();
        int currentMax = arrays[i].back();

        // Candidate distances using previous arrays' extremes
        result = std::max(result, std::abs(globalMax - currentMin));
        result = std::max(result, std::abs(globalMin - currentMax));

        // Update global extremes for future arrays
        globalMin = std::min(globalMin, currentMin);
        globalMax = std::max(globalMax, currentMax);
    }
    return result;
}

// The key insight is that the maximum distance between elements from different arrays will always be achieved by taking either the smallest element from one array and the largest element from another, or vice versa. Since each array is sorted, its smallest element is at index 0 and its largest is at the back. We can solve this in a single pass by keeping track of the minimum value and maximum value seen so far among all **previous** arrays. For each new array `i` starting from index 1, we compute two candidate distances:  
// - `abs(globalMax - arrays[i][0])` (using the current array's minimum with the global maximum from earlier arrays)  
// - `abs(globalMin - arrays[i].back())` (using the current array's maximum with the global minimum from earlier arrays)  
// We update the result with the maximum of these candidates. After that, we update `globalMax` and `globalMin` with the current array's maximum and minimum, so that future arrays can use them. This ensures that we never compare elements from the same array because at the time of computing the distance for array `i`, the global values only come from arrays `0` through `i-1`. Edge cases: if arrays have only one element, the minimum and maximum are the same. The algorithm runs in O(n) time where n is the number of arrays, and O(1) extra space (ignoring input storage). It also correctly handles negative numbers and duplicate values.
