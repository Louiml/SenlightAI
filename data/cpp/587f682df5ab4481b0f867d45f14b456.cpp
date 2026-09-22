// Write a C++ function named `stablePivotPartition` that accepts a `const std::vector<int>& nums` and an integer `pivot`, and returns a new `std::vector<int>` containing all elements of the input rearranged so that all elements less than `pivot` appear first (in their original relative order), followed by all elements equal to `pivot` (also in original relative order), and then all elements greater than `pivot` (again in original relative order). This is sometimes called a "pivot partition" or "three-way partition" but with stability preserved. The function must not modify the input vector. Ensure the implementation handles an empty input vector gracefully and works for any integer values, including negative numbers, duplicates, and cases where the pivot may not appear in the input.

The solution is straightforward and explicitly maintains stability: it performs three separate linear passes over the input array. The first pass collects all elements strictly less than the pivot in the order they appear. The second pass collects all elements exactly equal to the pivot in order. The third pass collects all elements strictly greater than the pivot in order. These three sublists are appended sequentially to a result vector. This approach does not require any sorting or in-place swapping, thus preserving the relative order of equal elements, which satisfies the "stable" requirement. Important edge cases include an empty input (returns an empty vector), a pivot value that occurs zero, one, or many times, and inputs where all elements are on one side of the pivot (e.g., all less, all equal, or all greater). The algorithm runs in O(n) time, where n is the number of elements, and uses O(n) auxiliary space for the result vector. The auxiliary space beyond the output is O(1), as only a loop counter and result vector are needed.

#include <vector>

// Returns a new vector with elements less than pivot, then equal to pivot, then greater than pivot,
// preserving the original relative order of elements within each group.
std::vector<int> stablePivotPartition(const std::vector<int>& nums, int pivot) {
    std::vector<int> result;
    result.reserve(nums.size()); // Optional, avoids reallocations

    // First pass: collect elements strictly less than pivot
    for (int value : nums) {
        if (value < pivot) {
            result.push_back(value);
        }
    }

    // Second pass: collect elements equal to pivot
    for (int value : nums) {
        if (value == pivot) {
            result.push_back(value);
        }
    }

    // Third pass: collect elements strictly greater than pivot
    for (int value : nums) {
        if (value > pivot) {
            result.push_back(value);
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// Declare the function to test (assume it's defined above in the same translation unit)
std::vector<int> stablePivotPartition(const std::vector<int>& nums, int pivot);

int main() {
    // Basic case with all three groups
    assert(stablePivotPartition({9, 3, 5, 3, 7, 1, 5, 8}, 5) == std::vector<int>({3, 3, 1, 5, 5, 9, 7, 8}));

    // Empty input
    assert(stablePivotPartition({}, 0) == std::vector<int>({}));

    // Pivot not present
    assert(stablePivotPartition({1, 2, 3, 4}, 10) == std::vector<int>({1, 2, 3, 4}));

    // All elements less than pivot
    assert(stablePivotPartition({2, 1, 3}, 10) == std::vector<int>({2, 1, 3}));

    // All elements equal to pivot
    assert(stablePivotPartition({7, 7, 7}, 7) == std::vector<int>({7, 7, 7}));

    // All elements greater than pivot
    assert(stablePivotPartition({8, 9, 10}, 5) == std::vector<int>({8, 9, 10}));

    // Negative numbers and duplicates
    assert(stablePivotPartition({-2, -1, -2, 3, 0, -1}, 0) == std::vector<int>({-2, -1, -2, -1, 0, 3}));

    // Pivot appears multiple times, mixed with duplicates
    assert(stablePivotPartition({5, 2, 5, 3, 1, 5, 4}, 5) == std::vector<int>({2, 3, 1, 4, 5, 5, 5}));

    // Single element less than pivot
    assert(stablePivotPartition({1}, 2) == std::vector<int>({1}));

    // Single element greater than pivot
    assert(stablePivotPartition({3}, 2) == std::vector<int>({3}));
}
