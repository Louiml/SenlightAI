// Write a C++ function named `countDistinctIntegers` that takes a `const std::vector<int>&` as input and returns the number of distinct integer values present in the vector. The function must handle vectors with duplicate values, negative numbers, zeros, and positive numbers, and must treat each unique numeric value as one distinct element regardless of how many times it appears. The implementation should not modify the input vector and must work correctly for empty vectors (returning 0) and for single-element vectors (returning 1). The function should be efficient for large inputs by using an appropriate data structure to avoid O(n²) time complexity.

// The most straightforward and efficient approach is to insert all elements from the input vector into a `std::set<int>` or `std::unordered_set<int>`. A set automatically eliminates duplicates, and the number of elements in the set after insertion equals the number of distinct integers. The main algorithm is: iterate through the vector once, insert each element into the set, then return `set.size()`. This handles all edge cases naturally: an empty vector results in an empty set and returns 0; a single-element vector returns 1; duplicate values are ignored by the set. The only edge case to be careful about is that the input vector must not be modified (ensure the parameter is `const`). Time complexity is O(n log n) for `std::set` due to balanced tree insertions, or O(n) average for `std::unordered_set`. Space complexity is O(n) in the worst case for storing distinct elements. The choice between set and unordered_set depends on whether worst-case performance or average-case speed is preferred; for this task, `std::set` is simple and deterministic.

#include <set>
#include <vector>

// Return the number of distinct integer values in the given vector.
int countDistinctIntegers(const std::vector<int>& nums) {
    std::set<int> unique_nums;
    for (const int& value : nums) {
        unique_nums.insert(value);
    }
    return static_cast<int>(unique_nums.size());
}

#include <cassert>
#include <vector>

// Function under test
int countDistinctIntegers(const std::vector<int>& nums);

int main() {
    assert(countDistinctIntegers({}) == 0);
    assert(countDistinctIntegers({5}) == 1);
    assert(countDistinctIntegers({1, 1, 1, 1}) == 1);
    assert(countDistinctIntegers({1, 2, 3, 4, 5}) == 5);
    assert(countDistinctIntegers({-3, -3, 0, 0, 2, 2, 7}) == 4);
    assert(countDistinctIntegers({10, -10, 10, -10}) == 2);
    assert(countDistinctIntegers({0, 0, 0}) == 1);
    assert(countDistinctIntegers({7, 7, 7, 8, 8, 9}) == 3);
    assert(countDistinctIntegers({100, -100, 5, -5, 0}) == 5);
    assert(countDistinctIntegers({-1, -2, -3, -2, -1}) == 3);
    return 0;
}
