Write a C++ function that, given a sorted (but possibly rotated at an unknown pivot) vector of distinct integers and a target value, returns the index of the target in the vector if it exists, or -1 if it does not. The vector is guaranteed to be sorted in ascending order before rotation, meaning it may be shifted cyclically (e.g., [4,5,6,7,0,1,2]). The function must handle vectors of any size (including empty and single-element), must not use linear search (you must exploit the rotated-sorted structure with a binary-search-based approach), and must work in O(log n) time. Assume all integers are unique and fit in a standard `int`.
#include <cassert>

int main() {
    // Normal rotated arrays
    std::vector<int> v1 = {4, 5, 6, 7, 0, 1, 2};
    assert(searchRotated(v1, 0) == 4);
    assert(searchRotated(v1, 3) == -1);
    assert(searchRotated(v1, 4) == 0);
    assert(searchRotated(v1, 2) == 6);

    // Not rotated (still sorted)
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    assert(searchRotated(v2, 1) == 0);
    assert(searchRotated(v2, 5) == 4);
    assert(searchRotated(v2, 6) == -1);

    // Single element
    std::vector<int> v3 = {42};
    assert(searchRotated(v3, 42) == 0);
    assert(searchRotated(v3, 41) == -1);

    // Empty vector
    std::vector<int> v4;
    assert(searchRotated(v4, 1) == -1);

    // Two-element arrays (both rotation cases)
    std::vector<int> v5 = {2, 1};
    assert(searchRotated(v5, 2) == 0);
    assert(searchRotated(v5, 1) == 1);
    assert(searchRotated(v5, 0) == -1);

    std::vector<int> v6 = {1, 2};
    assert(searchRotated(v6, 1) == 0);
    assert(searchRotated(v6, 2) == 1);
    assert(searchRotated(v6, 3) == -1);

    // Larger rotation pivot in the middle
    std::vector<int> v7 = {7, 8, 9, 1, 2, 3, 4, 5, 6};
    assert(searchRotated(v7, 9) == 2);
    assert(searchRotated(v7, 1) == 3);
    assert(searchRotated(v7, 6) == 8);
    assert(searchRotated(v7, 0) == -1);

    return 0;
}
#include <vector>

// Search for target in a rotated sorted vector of distinct integers.
// Returns the index of target if found, otherwise -1.
int searchRotated(const std::vector<int>& nums, int target) {
    int low = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        // Left half is sorted
        if (nums[low] <= nums[mid]) {
            if (nums[low] <= target && target < nums[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        // Right half is sorted
        else {
            if (nums[mid] < target && target <= nums[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    return -1;
}
// The key insight is that a rotated sorted array can be split into two ascending segments whose boundary is the rotation pivot. At each step of binary search, we compute the middle index. If the middle element equals the target, we return it. Otherwise, we determine which of the two halves is guaranteed to be sorted by comparing `nums[low]` with `nums[mid]`. If `nums[low] <= nums[mid]`, then the left half is sorted; we check if the target lies within that sorted range and narrow accordingly. If the left half is not sorted, then the right half must be sorted (due to distinct elements), and we similarly check if the target lies in that sorted right half. This ensures we discard half the search space each iteration. Edge cases include an empty vector (return -1 immediately), a single-element vector (check that element), a vector that is not rotated at all (the algorithm naturally handles it because `nums[low] <= nums[mid]` holds everywhere), and a target that is not present. Time complexity is O(log n) due to halving the search space; space complexity is O(1) auxiliary.
