Write a C++ function named `intersection` that takes two vectors of integers, `nums1` and `nums2`, and returns a vector containing the distinct elements that appear in both vectors. The result should not contain duplicates, and the order of elements in the returned vector does not matter. Assume both input vectors are non-empty and may contain duplicate values, negative numbers, and unsorted elements. The function should be efficient for large input sizes.

// The solution uses an `unordered_set` to achieve average O(1) lookups. First, insert all elements from `nums1` into an `unordered_set` to eliminate duplicates and allow fast membership testing. Then iterate through `nums2`, and for each element, check if it exists in the set from `nums1`. If it does, insert it into a second `unordered_set` (`resultSet`) to automatically remove any duplicates in `nums2`. Finally, copy all elements from `resultSet` into a vector and return it. Edge cases include when both vectors share no common elements (returns an empty vector), when duplicates exist in either vector (handled by sets), and when elements are negative or large (handled by integer storage). Time complexity is O(n + m) on average, where n and m are the sizes of the inputs, since each element is processed a constant number of times. Space complexity is O(n + k) in the worst case, where n is the size of `nums1` and k is the size of the result set.

#include <vector>
#include <unordered_set>

// Returns a vector of distinct integers that are present in both input vectors.
// The order of elements in the returned vector is unspecified.
std::vector<int> intersection(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    std::unordered_set<int> set1(nums1.begin(), nums1.end());
    std::unordered_set<int> resultSet;
    for (const int& num : nums2) {
        if (set1.find(num) != set1.end()) {
            resultSet.insert(num);
        }
    }
    return std::vector<int>(resultSet.begin(), resultSet.end());
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Helper to compare vectors ignoring order
    auto sameSet = [](std::vector<int> a, std::vector<int> b) {
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        return a == b;
    };

    // Basic case
    assert(sameSet(intersection({1,2,2,1}, {2,2}), {2}));

    // No common elements
    assert(sameSet(intersection({1,2,3}, {4,5}), {}));

    // All common but duplicates
    assert(sameSet(intersection({5,5,5}, {5,5}), {5}));

    // Mixed with negatives
    assert(sameSet(intersection({-1,0,3,-1}, {-1,-1,4}), {-1}));

    // Larger example
    assert(sameSet(intersection({4,9,5,9,4}, {9,4,9,8,4}), {9,4}));

    // Single element identical
    assert(sameSet(intersection({7}, {7}), {7}));

    // Single element not shared
    assert(sameSet(intersection({7}, {8}), {}));

    // Unsorted and duplicates
    assert(sameSet(intersection({3,1,3,2}, {2,2,3}), {3,2}));

    // Negative and positive common
    assert(sameSet(intersection({-2, -1, 0, 1}, {-2, 0, 2}), {-2, 0}));

    // Empty result when one is empty (though problem says non-empty, test anyway)
    assert(sameSet(intersection({1,2}, {}), {}));

    return 0;
}
