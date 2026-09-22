/*
Write a C++ function that takes a non-empty vector of integers and returns a boolean indicating whether the array is sorted in non-decreasing order (i.e., each element is less than or equal to the next element). The array may contain duplicate values, negative numbers, and be of any size from 1 to 100,000. The function should not modify the input vector and must work efficiently for large inputs.
*/
#include <vector>

// Returns true if the input vector is sorted in non-decreasing order.
bool isSortedNonDecreasing(const std::vector<int>& nums) {
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] < nums[i - 1]) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Function declaration (or include the solution above)
bool isSortedNonDecreasing(const std::vector<int>& nums);

int main() {
    // Single element
    assert(isSortedNonDecreasing({5}) == true);
    // Already sorted with duplicates
    assert(isSortedNonDecreasing({1, 2, 2, 3, 4}) == true);
    // Unsorted with a drop at the end
    assert(isSortedNonDecreasing({1, 2, 3, 2}) == false);
    // Unsorted at the beginning
    assert(isSortedNonDecreasing({5, 4, 6}) == false);
    // All equal elements (sorted)
    assert(isSortedNonDecreasing({7, 7, 7, 7}) == true);
    // Negative numbers sorted
    assert(isSortedNonDecreasing({-5, -2, 0, 3}) == true);
    // Negative numbers unsorted
    assert(isSortedNonDecreasing({-2, -5, 0}) == false);
    // Descending order
    assert(isSortedNonDecreasing({10, 9, 8}) == false);
    // Large sorted array (trivially true)
    std::vector<int> largeSorted(100000);
    for (int i = 0; i < 100000; ++i) largeSorted[i] = i;
    assert(isSortedNonDecreasing(largeSorted) == true);
    // Large unsorted array (drop in the middle)
    std::vector<int> largeUnsorted(100000);
    for (int i = 0; i < 100000; ++i) largeUnsorted[i] = 100000 - i;
    assert(isSortedNonDecreasing(largeUnsorted) == false);
    return 0;
}
// The solution iterates through the array once, comparing each element with its predecessor. If any element is smaller than the previous one, the array is not sorted and the function immediately returns `false`. If the loop completes without finding such a violation, the array is sorted and the function returns `true`. Edge cases include a single-element array (trivially sorted), arrays with all equal elements (sorted), and arrays that are sorted in descending order (not sorted). The algorithm runs in O(n) time and uses O(1) auxiliary space, where n is the size of the input vector. Since the function receives the vector by const reference, no copying occurs and the original data remains unchanged.
