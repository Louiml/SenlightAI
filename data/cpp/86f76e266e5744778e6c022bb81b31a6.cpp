Write a C++ function `findTargetRange` that takes a sorted vector of integers (`nums`) and an integer `target`, and returns a vector of two integers: the starting and ending positions of the target in the array. If the target is not present, return `{-1, -1}`. The function must run in O(log n) time, assume the input vector is non-empty and sorted in strictly non-decreasing order, and handle cases where the target appears once, multiple times, or not at all.

The solution uses two binary searches. The first finds the leftmost occurrence: we maintain a low/high window and when the middle element is less than the target, we move the left boundary past it; otherwise (middle >= target) we bring the right boundary down. After the loop, `left` points to the first index where `nums[left] >= target`. The second search finds the rightmost occurrence: here, when the middle element is less than or equal to the target, we move the left boundary up; otherwise we bring the right boundary down. After that loop, `right` points to the last index where `nums[right] <= target`. If `left <= right`, the target exists, and we return `{left, right}`; otherwise return `{-1, -1}`. Edge cases include target smaller than all elements (left ends at 0, right ends at -1), target larger than all (left ends at size, right ends at size-1), and target present exactly once (both searches converge on the same index). Time complexity is O(log n) for each search, so O(log n) total, and space is O(1) auxiliary.

#include <vector>

// Returns the first and last occurrence of target in sorted nums.
// If target is absent, returns {-1, -1}.
std::vector<int> findTargetRange(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    int first = -1;

    // Find leftmost occurrence.
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    first = left;

    left = 0;
    right = static_cast<int>(nums.size()) - 1;
    int last = -1;

    // Find rightmost occurrence.
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    last = right;

    if (first <= last) {
        return {first, last};
    }
    return {-1, -1};
}

#include <cassert>
#include <vector>

int main() {
    // Target present once.
    assert(findTargetRange({1, 3, 5, 7}, 3) == std::vector<int>({1, 1}));
    // Target present multiple times.
    assert(findTargetRange({1, 2, 2, 2, 3}, 2) == std::vector<int>({1, 3}));
    // Target at beginning.
    assert(findTargetRange({5, 5, 5, 6}, 5) == std::vector<int>({0, 2}));
    // Target at end.
    assert(findTargetRange({1, 2, 3, 4, 4}, 4) == std::vector<int>({3, 4}));
    // Target not present.
    assert(findTargetRange({1, 3, 5}, 4) == std::vector<int>({-1, -1}));
    // Target smaller than all.
    assert(findTargetRange({2, 3, 4}, 1) == std::vector<int>({-1, -1}));
    // Target larger than all.
    assert(findTargetRange({2, 3, 4}, 5) == std::vector<int>({-1, -1}));
    // Single element containing target.
    assert(findTargetRange({8}, 8) == std::vector<int>({0, 0}));
    // Single element not containing target.
    assert(findTargetRange({8}, 7) == std::vector<int>({-1, -1}));
    // All elements equal to target.
    assert(findTargetRange({9, 9, 9}, 9) == std::vector<int>({0, 2}));
}
