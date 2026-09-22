Write a C++ function that takes a vector of integers and an integer `k` as input, and returns `true` if there exist two distinct indices `i` and `j` in the vector such that the values at those indices are equal and the absolute difference between the indices is at most `k`; otherwise, return `false`. The vector may be empty, may contain negative numbers, may contain duplicates, and `k` may be zero or negative. The function should be efficient for large inputs and should not modify the input vector.

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> v1 = {1, 2, 3, 1};
    assert(containsNearbyDuplicate(v1, 3) == true);
    assert(containsNearbyDuplicate(v1, 2) == false);

    // Adjacent duplicates
    std::vector<int> v2 = {5, 5};
    assert(containsNearbyDuplicate(v2, 1) == true);
    assert(containsNearbyDuplicate(v2, 0) == false);

    // k is negative
    std::vector<int> v3 = {1, 2, 3, 1};
    assert(containsNearbyDuplicate(v3, -1) == false);

    // Empty and single element
    std::vector<int> v4;
    assert(containsNearbyDuplicate(v4, 5) == false);
    std::vector<int> v5 = {42};
    assert(containsNearbyDuplicate(v5, 1) == false);

    // Duplicate far apart
    std::vector<int> v6 = {1, 2, 3, 4, 1};
    assert(containsNearbyDuplicate(v6, 4) == true);
    assert(containsNearbyDuplicate(v6, 3) == false);

    // All same values, k small
    std::vector<int> v7 = {7, 7, 7, 7};
    assert(containsNearbyDuplicate(v7, 2) == true);
    assert(containsNearbyDuplicate(v7, 0) == false);

    // Negative numbers
    std::vector<int> v8 = {-1, 0, -1};
    assert(containsNearbyDuplicate(v8, 2) == true);
    assert(containsNearbyDuplicate(v8, 1) == false);

    return 0;
}

#include <vector>
#include <unordered_map>

// Returns true if any two distinct indices i and j (with |i - j| <= k) have equal values.
bool containsNearbyDuplicate(const std::vector<int>& nums, int k) {
    std::unordered_map<int, int> lastIndex; // value -> most recent index seen

    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        const int value = nums[i];
        auto it = lastIndex.find(value);
        if (it != lastIndex.end() && i - it->second <= k) {
            return true;
        }
        lastIndex[value] = i; // update with current index
    }
    return false;
}

// The solution uses an unordered map to store the most recent index of each value encountered while iterating through the vector from left to right. For each element at index `i`, we check if the same value was already seen at a previous index stored in the map, and if the difference `i - storedIndex` is less than or equal to `k`. If such a pair is found, return `true` immediately. Otherwise, update the map entry for that value to the current index. This works because we only need to check the most recent occurrence of each value—any earlier occurrence would have a larger index difference, so the latest occurrence gives the best chance to satisfy the condition. Edge cases include an empty vector (return `false`), all unique values (return `false`), and `k` being negative or zero (no pair of distinct indices can differ by a non-positive number, so return `false` unless the same index is counted, but since indices must be distinct, `k <= 0` always returns `false`). Time complexity is O(n) on average, where n is the number of elements, due to average O(1) operations for hash map lookups and insertions. Space complexity is O(n) in the worst case, as the map may store every unique value.
