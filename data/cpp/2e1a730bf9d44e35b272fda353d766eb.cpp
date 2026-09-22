/*
Write a C++ function that takes two vectors of integers, `nums1` and `nums2`, and returns a vector containing the intersection of the two arrays (i.e., the set of distinct elements that appear in both arrays). The result must contain no duplicates, and the order of the returned elements does not matter. The input vectors may be unsorted, may contain duplicates, and may be empty. Your solution should be efficient for large inputs and should not use extra library functions beyond the standard algorithms and containers.
*/
#include <vector>
#include <set>
#include <algorithm>

// Return the intersection of two integer vectors as a vector of unique elements.
std::vector<int> findIntersection(std::vector<int> nums1, std::vector<int> nums2) {
    // Sort both vectors to enable a linear merge-style traversal.
    std::sort(nums1.begin(), nums1.end());
    std::sort(nums2.begin(), nums2.end());

    std::set<int> common;
    std::size_t i = 0, j = 0;
    while (i < nums1.size() && j < nums2.size()) {
        if (nums1[i] < nums2[j]) {
            ++i;
        } else if (nums1[i] > nums2[j]) {
            ++j;
        } else {
            // Both elements are equal; insert to eliminate duplicates.
            common.insert(nums1[i]);
            ++i;
            ++j;
        }
    }

    // Convert the set to a vector to match the return type.
    return std::vector<int>(common.begin(), common.end());
}
#include <cassert>
#include <vector>

// Declaration of the function under test (assumes the solution is included above).
std::vector<int> findIntersection(std::vector<int> nums1, std::vector<int> nums2);

int main() {
    // Basic case with common elements.
    assert(findIntersection({1, 2, 2, 1}, {2, 2}) == std::vector<int>({2}));

    // No common elements.
    assert(findIntersection({1, 3, 5}, {2, 4, 6}) == std::vector<int>());

    // One empty vector.
    assert(findIntersection({}, {1, 2}) == std::vector<int>());
    assert(findIntersection({1, 2}, {}) == std::vector<int>());

    // Both empty.
    assert(findIntersection({}, {}) == std::vector<int>());

    // All elements common, but duplicates in both.
    assert(findIntersection({4, 9, 5}, {9, 4, 9, 8, 4}) == std::vector<int>({4, 9}));

    // Arrays already intersected exactly once each.
    assert(findIntersection({0, 1, 2}, {2, 1, 0}) == std::vector<int>({0, 1, 2}));

    // Duplicate common values are reduced to one.
    assert(findIntersection({1, 1, 1}, {1, 1}) == std::vector<int>({1}));

    // Larger negative and positive values.
    assert(findIntersection({-3, -1, 0, 2, 5}, {-5, -1, 3, 5, 10}) == std::vector<int>({-1, 5}));

    // Unsorted input with mixed overlap.
    assert(findIntersection({3, 5, 7, 9}, {7, 3, 1}) == std::vector<int>({3, 7}));

    return 0;
}
// The core idea is to sort both input vectors so that we can merge them in a single pass using two pointers. After sorting, we maintain one index `i` for `nums1` and one index `j` for `nums2`. While both indices are within bounds, we compare the current elements: if `nums1[i]` is smaller, we advance `i`; if `nums2[j]` is smaller, we advance `j`; otherwise, they are equal, so we insert that common value into a `std::set` to automatically remove duplicates, then advance both indices. Finally, we copy the distinct common values from the set into a result vector. Edge cases include empty inputs (returns empty vector), one array fully contained in the other, all equal values, and duplicates within each array. The sorting step dominates the time complexity: \(O(n \log n + m \log m)\) for sizes \(n\) and \(m\), while the merge pass is \(O(n+m)\). The auxiliary space is \(O(\min(n,m))\) for the set in the worst case when all elements are common.
