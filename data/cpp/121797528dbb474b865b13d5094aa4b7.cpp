Write a C++ function `int findInsertPosition(const std::vector<int>& nums, int target)` that returns the index at which `target` should be inserted into a sorted vector of distinct integers (in ascending order) to maintain sorted order. If `target` already exists in the vector, return that index. The function must handle an empty vector and must not modify the input vector.
// The sorted vector allows a direct linear scan, but since it is sorted we can use binary search for efficiency. The algorithm initializes `low = 0` and `high = nums.size()` (not size-1, because the insertion position can be at the end). While `low < high`, compute `mid = low + (high - low) / 2`. If `nums[mid] < target`, then the target’s insertion point is to the right, so set `low = mid + 1`. Otherwise, set `high = mid`. This invariant ensures that after the loop, `low` is the first index where `nums[low] >= target`, which is exactly the required result for both existing and missing targets. Edge cases: empty vector returns 0; target smaller than all elements returns 0; target larger than all elements returns `nums.size()`. Time complexity is O(log n) due to binary search; space complexity is O(1).
#include <vector>

// Returns the insertion index for target in a sorted vector of distinct integers.
// If target is present, returns its index; otherwise, returns the position where
// it should be inserted to keep the vector sorted in ascending order.
int findInsertPosition(const std::vector<int>& nums, int target) {
    int low = 0;
    int high = static_cast<int>(nums.size());
    
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    
    return low;
}
#include <cassert>
#include <vector>

int findInsertPosition(const std::vector<int>&, int); // declaration for clarity

int main() {
    std::vector<int> empty;
    assert(findInsertPosition(empty, 5) == 0);
    
    std::vector<int> nums1 = {1, 3, 5, 6};
    assert(findInsertPosition(nums1, 5) == 2);
    assert(findInsertPosition(nums1, 2) == 1);
    assert(findInsertPosition(nums1, 7) == 4);
    assert(findInsertPosition(nums1, 0) == 0);
    
    std::vector<int> nums2 = {2};
    assert(findInsertPosition(nums2, 1) == 0);
    assert(findInsertPosition(nums2, 2) == 0);
    assert(findInsertPosition(nums2, 3) == 1);
    
    std::vector<int> nums3 = {-10, -5, 0, 7, 12};
    assert(findInsertPosition(nums3, -5) == 1);
    assert(findInsertPosition(nums3, -1) == 2);
    assert(findInsertPosition(nums3, 13) == 5);
    assert(findInsertPosition(nums3, -11) == 0);
    
    return 0;
}
