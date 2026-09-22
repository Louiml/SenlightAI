/*
Write a C++ function `maxDistanceBetweenEqualElements` that takes a `std::vector<int>` (by const reference) and returns the maximum distance between two occurrences of the same element, where distance is defined as the absolute difference of their indices. If no element appears more than once, return 0. The input vector is non-empty. The function must be self-contained, with proper `const` correctness and include all necessary headers. You may assume the vector can contain duplicate values, negative numbers, and large sizes (e.g., up to 10^6 elements). The function should not modify the input and must handle the case where the maximum distance is between the first and last occurrence of a value that appears multiple times.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum distance between two occurrences of the same element.
// Distance = index difference (absolute). If no duplicates, returns 0.
int maxDistanceBetweenEqualElements(const std::vector<int>& arr) {
    std::unordered_map<int, int> firstIndex; // value -> first index seen
    int maxDist = 0;

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        auto it = firstIndex.find(arr[i]);
        if (it != firstIndex.end()) {
            int dist = i - it->second; // i >= first index, so absolute is implicit
            maxDist = std::max(maxDist, dist);
        } else {
            firstIndex[arr[i]] = i;
        }
    }

    return maxDist;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with duplicates
    std::vector<int> v1 = {1, 2, 3, 1, 4, 1}; // last 1 at index 5, first at 0 -> dist=5
    assert(maxDistanceBetweenEqualElements(v1) == 5);

    // No duplicates -> 0
    std::vector<int> v2 = {1, 2, 3, 4};
    assert(maxDistanceBetweenEqualElements(v2) == 0);

    // All same element
    std::vector<int> v3 = {7, 7, 7, 7};
    assert(maxDistanceBetweenEqualElements(v3) == 3);

    // Duplicates with negative numbers
    std::vector<int> v4 = {-1, -2, -1, -2, -1}; // -1: first 0, last 4 -> 4; -2: first 1, last 3 -> 2
    assert(maxDistanceBetweenEqualElements(v4) == 4);

    // Single element
    std::vector<int> v5 = {42};
    assert(maxDistanceBetweenEqualElements(v5) == 0);

    // Duplicates where maximum distance is not the last element overall
    std::vector<int> v6 = {5, 1, 5, 2, 5, 3}; // 5: first 0, last 4 -> 4; others single
    assert(maxDistanceBetweenEqualElements(v6) == 4);

    // Large vector with only one duplicate at far ends
    std::vector<int> v7 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0};
    assert(maxDistanceBetweenEqualElements(v7) == 9);

    // Duplicates adjacent
    std::vector<int> v8 = {1, 1, 2, 2, 2};
    assert(maxDistanceBetweenEqualElements(v8) == 2); // 2's first at 2, last at 4

    return 0;
}

// The key observation is that for each distinct value, the maximum distance between its occurrences is simply the difference between the last occurrence index and the first occurrence index. To find this efficiently, we can use a hash map (e.g., `std::unordered_map<int, int>`) that stores the first index where each value appears. We then iterate through the array once. For each element at index `i`, if we have seen it before, we compute `i - firstIndex[value]` and update the global maximum. If we haven't seen it, we record its first index as `i`. This works because the distance from the first occurrence to any later occurrence is maximized at the very last occurrence—so by always subtracting the first index, we capture the maximum possible distance for each value. Edge cases: (1) if the array has no duplicates, the map will never trigger an update, and max_dist remains 0; (2) if the same value appears many times, the first occurrence is fixed, and the largest distance is naturally found at the last occurrence; (3) negative numbers and zeros are handled because the map uses the value as key without any arithmetic restrictions. The algorithm runs in O(n) time and O(n) space for the hash map, where n is the size of the input vector.
