/*
Write a C++ function `std::pair<int, int> findRange(const std::vector<int>& sortedArray, int target)` that, given a sorted vector of integers (possibly containing duplicates) and a target value, returns a pair of indices representing the first and last occurrence of the target in the vector. If the target is not present, return `{-1, -1}`. The vector is guaranteed to be sorted in non-decreasing order. The function must run in `O(log n)` time and use `O(1)` extra space. Use two binary searches: one to find the leftmost occurrence (first index ≥ target) and one to find the rightmost occurrence (last index ≤ target). Handle edge cases including an empty vector, target smaller than all elements, target larger than all elements, and single-element vectors.
*/

#include <vector>
#include <utility>

// Returns {first, last} indices of target in sortedArray, or {-1,-1} if absent.
std::pair<int, int> findRange(const std::vector<int>& sortedArray, int target) {
    int n = static_cast<int>(sortedArray.size());
    if (n == 0) {
        return {-1, -1};
    }

    // Leftmost occurrence: smallest index where sortedArray[i] >= target
    int low = 0, high = n - 1, leftIndex = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sortedArray[mid] >= target) {
            leftIndex = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    // If the element at leftIndex is not the target, target is absent
    if (sortedArray[leftIndex] != target) {
        return {-1, -1};
    }

    // Rightmost occurrence: largest index where sortedArray[i] <= target
    low = 0;
    high = n - 1;
    int rightIndex = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sortedArray[mid] <= target) {
            rightIndex = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return {leftIndex, rightIndex};
}

#include <cassert>
#include <vector>
#include <utility>

// The findRange function is assumed to be defined above.

int main() {
    // Basic cases
    std::vector<int> a1 = {1, 2, 2, 2, 3, 4};
    auto r1 = findRange(a1, 2);
    assert(r1.first == 1 && r1.second == 3);

    std::vector<int> a2 = {5, 5, 5, 5};
    auto r2 = findRange(a2, 5);
    assert(r2.first == 0 && r2.second == 3);

    // Single element cases
    std::vector<int> a3 = {7};
    assert(findRange(a3, 7) == std::make_pair(0, 0));
    assert(findRange(a3, 8) == std::make_pair(-1, -1));

    // Target not present
    std::vector<int> a4 = {1, 3, 5};
    assert(findRange(a4, 2) == std::make_pair(-1, -1));
    assert(findRange(a4, 0) == std::make_pair(-1, -1));
    assert(findRange(a4, 6) == std::make_pair(-1, -1));

    // Empty vector
    std::vector<int> a5;
    assert(findRange(a5, 1) == std::make_pair(-1, -1));

    // Negative numbers and duplicates
    std::vector<int> a6 = {-3, -3, -1, -1, -1, 0, 2, 2};
    auto r6 = findRange(a6, -1);
    assert(r6.first == 2 && r6.second == 4);
    auto r7 = findRange(a6, 2);
    assert(r7.first == 6 && r7.second == 7);
    assert(findRange(a6, -2) == std::make_pair(-1, -1));

    return 0;
}

// The solution uses two separate binary searches.  
// - **Left boundary:** Standard binary search that updates the answer index whenever `a[mid] >= target`, then narrows the search to the left half (`hh = mid - 1`). This finds the smallest index where the element equals or exceeds the target. If the value at that index is not equal to the target, the target does not exist, so return `{-1, -1}`.  
// - **Right boundary:** Another binary search that updates the answer index whenever `a[mid] <= target`, then searches the right half (`lw = mid + 1`). This finds the largest index where the element is ≤ target. Since the target exists, the value at this index is definitely equal to the target (because the array is sorted and we already know the target is present).  
// - **Edge cases:** Empty vector: return `{-1,-1}` immediately. Target smaller/larger than all elements: the left search will return index 0 or `n-1` (or some arbitrary index) but the value check will fail, so return `{-1,-1}`. Single-element vector: both searches return index 0 if that element equals target, else `{-1,-1}`.  
// - **Time complexity:** Two binary searches each run in `O(log n)`, so total `O(log n)`. Space complexity is `O(1)` (only a few integer variables). The solution avoids `std::lower_bound`/`upper_bound` to stay closer to the original binary search logic.
