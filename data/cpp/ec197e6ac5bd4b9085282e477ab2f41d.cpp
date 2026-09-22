Given a vector of integers that may contain duplicates and is not necessarily sorted, write a C++ function named `wiggleSortInPlace` that rearranges the elements so that they follow a "wiggle" pattern: `nums[0] <= nums[1] >= nums[2] <= nums[3] >= nums[4] ...` (i.e., every odd index element is greater than or equal to its neighboring even-index elements). The function must modify the input vector in-place and may not use extra space proportional to the input size (only O(1) auxiliary space is allowed, aside from the input vector itself). The relative order of equal elements does not matter. The input vector may have any size, including 0 or 1, and for such cases the function should leave the vector unchanged. The function signature should be `void wiggleSortInPlace(std::vector<int>& nums)`. Do not use sorting (the standard library `sort` or any custom sorting) because that would be inefficient for this specific problem; instead, use a single pass that swaps adjacent elements only when needed.

The key observation is that the pattern only compares adjacent pairs, not all elements globally. We process the vector with a single forward pass from index 0 to `n-2`. At each index `i`, we check the required relationship based on its parity (whether `i` is even or odd). If `i` is even, we need `nums[i] <= nums[i+1]`; if `i` is odd, we need `nums[i] >= nums[i+1]`. When the condition fails, we swap `nums[i]` and `nums[i+1]`. This local swap is sufficient because the pattern is enforced locally at each adjacent pair; previous constraints remain satisfied because swapping only affects the current pair and the next pair (which is fixed in the next iteration). Edge cases: empty or single-element vectors require no work. Duplicate values are handled naturally since `<=` and `>=` allow equality. The algorithm runs in O(n) time and uses O(1) extra space, making it much more efficient than sorting for large inputs.

#include <vector>
#include <utility> // for std::swap

// Rearrange the vector in-place to satisfy the wiggle pattern:
// nums[0] <= nums[1] >= nums[2] <= nums[3] >= ...
// Uses a single pass with adjacent swaps only.
void wiggleSortInPlace(std::vector<int>& nums) {
    const std::size_t n = nums.size();
    for (std::size_t i = 0; i + 1 < n; ++i) {
        if (i % 2 == 0) {
            // Even index: need nums[i] <= nums[i+1]
            if (nums[i] > nums[i + 1]) {
                std::swap(nums[i], nums[i + 1]);
            }
        } else {
            // Odd index: need nums[i] >= nums[i+1]
            if (nums[i] < nums[i + 1]) {
                std::swap(nums[i], nums[i + 1]);
            }
        }
    }
}

#include <cassert>
#include <vector>

// The function is declared here (or included from the solution).
void wiggleSortInPlace(std::vector<int>& nums);

// Helper to check the wiggle property for a vector.
bool isWiggle(const std::vector<int>& nums) {
    for (std::size_t i = 0; i + 1 < nums.size(); ++i) {
        if (i % 2 == 0) {
            if (nums[i] > nums[i + 1]) return false;
        } else {
            if (nums[i] < nums[i + 1]) return false;
        }
    }
    return true;
}

int main() {
    // Basic test with distinct values.
    std::vector<int> v1 = {3, 5, 2, 1, 4};
    wiggleSortInPlace(v1);
    assert(isWiggle(v1));

    // Duplicate values.
    std::vector<int> v2 = {1, 1, 1, 2, 2};
    wiggleSortInPlace(v2);
    assert(isWiggle(v2));

    // Already wiggle sorted.
    std::vector<int> v3 = {1, 3, 2, 5, 4};
    wiggleSortInPlace(v3);
    assert(isWiggle(v3));

    // Single element.
    std::vector<int> v4 = {42};
    wiggleSortInPlace(v4);
    assert(isWiggle(v4));

    // Empty vector.
    std::vector<int> v5;
    wiggleSortInPlace(v5);
    assert(isWiggle(v5));

    // Two elements: should be non-decreasing.
    std::vector<int> v6 = {2, 1};
    wiggleSortInPlace(v6);
    assert(isWiggle(v6));

    // Large all-equal vector.
    std::vector<int> v7(1000, 7);
    wiggleSortInPlace(v7);
    assert(isWiggle(v7));

    // Negative numbers and zeros.
    std::vector<int> v8 = {-5, -3, -1, 0, 2, -4};
    wiggleSortInPlace(v8);
    assert(isWiggle(v8));

    return 0;
}
