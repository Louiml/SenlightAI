// Write a C++ function `int countDistinctAfterSort(vector<int> nums)` that takes a vector of integers (possibly containing duplicates and negative values) and returns the number of distinct integers in the vector after sorting and removing consecutive duplicates. The function must not modify the input vector. The behavior should match the given snippet’s logic: sort the array, then use `unique` to remove adjacent duplicates, and return the size of the resulting unique elements. The input can be empty, in which case the function returns 0. Ensure the function is efficient and works for large inputs.
#include <cassert>
#include <vector>

int countDistinctAfterSort(const std::vector<int>& nums);

int main() {
    // Empty vector
    assert(countDistinctAfterSort({}) == 0);
    // Single element
    assert(countDistinctAfterSort({5}) == 1);
    // All duplicates
    assert(countDistinctAfterSort({7, 7, 7}) == 1);
    // Mixed with negatives and duplicates
    assert(countDistinctAfterSort({3, -1, 3, -1, 0}) == 3); // {-1, 0, 3}
    // Already sorted
    assert(countDistinctAfterSort({-2, 0, 1}) == 3);
    // Unsorted with many duplicates
    assert(countDistinctAfterSort({4, 2, 4, 2, 1, 4}) == 3); // {1, 2, 4}
    // Large range
    assert(countDistinctAfterSort({100, -100, 0, 100}) == 3);
    // Check original not modified
    std::vector<int> input = {3, 1, 3};
    int result = countDistinctAfterSort(input);
    assert(result == 2);
    assert(input == std::vector<int>({3, 1, 3}));
    // All same negative
    assert(countDistinctAfterSort({-1, -1}) == 1);
    return 0;
}
#include <vector>
#include <algorithm>

// Return the number of distinct integers after sorting and removing duplicates.
// Does not modify the input vector.
int countDistinctAfterSort(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    std::vector<int> sorted = nums; // copy to avoid modifying input
    std::sort(sorted.begin(), sorted.end());
    auto last = std::unique(sorted.begin(), sorted.end());
    return static_cast<int>(last - sorted.begin());
}
// The solution is straightforward: copy the input vector into a local vector (to avoid modifying the original), sort it in ascending order using `std::sort`, then apply `std::unique` which moves unique elements to the front and returns an iterator to the new logical end. The number of distinct elements is obtained by subtracting the beginning iterator from that returned iterator, or using `std::distance`. Edge cases: empty vector returns 0; a vector with all identical elements returns 1; negative numbers and large values are handled naturally by sorting. Time complexity is O(n log n) due to sorting, with O(n) auxiliary space for the copy (if we don't modify the input). Space can be reduced to O(1) if we are allowed to modify the input, but the task requires not modifying the input, so we use a copy.
