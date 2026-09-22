Write a C++ function named `binarySearch` that takes a sorted vector of integers (non-decreasing order) and a target integer, and returns the index of the target if it exists, or -1 if it does not. The vector may contain duplicates, negative numbers, and can be of any size (including empty). Use the classic binary search algorithm to achieve logarithmic time complexity. The function must be `const`-correct (i.e., it should not modify the input vector), and should be implemented as a standalone free function (no class or main). Ensure your implementation correctly handles edge cases: empty input, target smaller than all elements, target larger than all elements, and duplicate values (any matching index is acceptable).
// The solution uses the standard iterative binary search on a sorted array. Initialize two pointers `low` and `high` to the first and last indices. In each iteration, compute the middle index as `low + (high - low) / 2` to avoid integer overflow. Compare the element at `mid` with the target: if equal, return `mid`; if the element is less than the target, discard the left half by setting `low = mid + 1`; otherwise discard the right half by setting `high = mid - 1`. Continue until `low` exceeds `high`, indicating the target is not present, and return -1. Edge cases: for an empty vector, the loop condition is immediately false and -1 is returned. When the target is smaller than all elements, `low` moves past `high` after checking the first element, returning -1. When duplicates exist, any index of the target is correct because we return the first found. Time complexity is O(log n) due to halving the search space each step, and space complexity is O(1) as only a few integer variables are used.
#include <vector>

// Perform binary search on a sorted vector of integers.
// Returns the index of the target if found, otherwise -1.
int binarySearch(const std::vector<int>& nums, int target) {
    int low = 0;
    int high = static_cast<int>(nums.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // avoid overflow
        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it)
// int binarySearch(const std::vector<int>&, int);

int main() {
    // Basic cases
    std::vector<int> v1 = {1, 3, 5, 7, 9};
    assert(binarySearch(v1, 5) == 2);
    assert(binarySearch(v1, 1) == 0);
    assert(binarySearch(v1, 9) == 4);
    assert(binarySearch(v1, 4) == -1);

    // Empty vector
    std::vector<int> v2;
    assert(binarySearch(v2, 5) == -1);

    // Target smaller than all
    std::vector<int> v3 = {2, 4, 6};
    assert(binarySearch(v3, 1) == -1);

    // Target larger than all
    assert(binarySearch(v3, 7) == -1);

    // Duplicates
    std::vector<int> v4 = {1, 2, 2, 2, 3};
    int idx = binarySearch(v4, 2);
    assert(idx >= 1 && idx <= 3); // any duplicate index is valid

    // Negative numbers
    std::vector<int> v5 = {-5, -3, 0, 2};
    assert(binarySearch(v5, -3) == 1);
    assert(binarySearch(v5, 0) == 2);
    assert(binarySearch(v5, -1) == -1);

    // Single element
    std::vector<int> v6 = {42};
    assert(binarySearch(v6, 42) == 0);
    assert(binarySearch(v6, 43) == -1);
}
