/*
Write a C++ function that takes a vector of integers and returns `true` if the integers can be arranged into pairs where each pair consists of two equal numbers, and `false` otherwise. The input vector may contain duplicates and have any length, including zero. For example, `[1,1,2,2]` should return `true` because it can be paired as `(1,1)` and `(2,2)`, while `[1,2,2,2]` should return `false` because one `1` remains unpaired. The function must not modify the original vector and must handle vectors with an odd number of elements correctly.
*/
#include <vector>
#include <algorithm>

// Returns true if every element in nums can be paired with an equal value.
bool canPartitionIntoEqualPairs(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n % 2 != 0) {
        return false;
    }
    if (n == 0) {
        return true;
    }
    // Create a mutable copy to avoid modifying the input vector.
    std::vector<int> sorted_nums = nums;
    std::sort(sorted_nums.begin(), sorted_nums.end());
    for (int i = 0; i < n; i += 2) {
        if (sorted_nums[i] != sorted_nums[i + 1]) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Assume canPartitionIntoEqualPairs is defined above.

int main() {
    std::vector<int> v1 = {1, 1, 2, 2};
    assert(canPartitionIntoEqualPairs(v1) == true);

    std::vector<int> v2 = {1, 2, 2, 2};
    assert(canPartitionIntoEqualPairs(v2) == false);

    std::vector<int> v3 = {};
    assert(canPartitionIntoEqualPairs(v3) == true);

    std::vector<int> v4 = {5};
    assert(canPartitionIntoEqualPairs(v4) == false);

    std::vector<int> v5 = {3, 3, 3, 3};
    assert(canPartitionIntoEqualPairs(v5) == true);

    std::vector<int> v6 = {1, 2, 3, 4};
    assert(canPartitionIntoEqualPairs(v6) == false);

    std::vector<int> v7 = {1, 1};
    assert(canPartitionIntoEqualPairs(v7) == true);

    std::vector<int> v8 = {1, 2, 1, 2, 1, 2};
    assert(canPartitionIntoEqualPairs(v8) == true);

    // Ensure the input vector is not modified.
    std::vector<int> v9 = {2, 1, 2, 1};
    std::vector<int> original = v9;
    canPartitionIntoEqualPairs(v9);
    assert(v9 == original);
}
// The main algorithm relies on the observation that for all elements to be paired with an equal partner, every value must appear an even number of times in the vector. A simple approach is to sort the vector, then iterate through it in steps of two. If any adjacent pair `(nums[i], nums[i+1])` contains different values, then the array cannot be fully paired, and the function returns `false`. If the vector has an odd length, it is immediately impossible to form pairs, so return `false` without further processing. Sorting ensures that equal elements are adjacent, so this pair-wise check covers all possible pairings. Edge cases include an empty vector (which trivially returns `true` because zero elements can be paired vacuously), a single element (odd length, returns `false`), and vectors where duplicates are not adjacent until sorted (e.g., `[1,2,1,2]` becomes `[1,1,2,2]` after sorting). The time complexity is \(O(n \log n)\) due to sorting, and the space complexity is \(O(n)\) if a copy of the input is made to avoid modifying the original, or \(O(1)\) extra space if sorting in place is allowed (but since we must not modify input, we make a copy, giving \(O(n)\) extra space).
