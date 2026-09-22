// Write a C++ function that takes a non-empty vector of integers and returns the minimum number of operations required to make all elements equal, where a single operation consists of decreasing any one element by 1 (i.e., each operation reduces exactly one element by 1). The function must compute the total number of decrements needed so that every element becomes equal to the current minimum value of the vector. The function should be efficient and handle vectors with large values (e.g., up to 1e9) and large sizes without overflow. The result should be returned as a `long long` to accommodate large totals. The input vector must not be modified.

The goal is to make all elements equal to the smallest element in the vector, because we can only decrease values. The minimal total operations is the sum of differences between each element and the minimum value. The algorithm is straightforward: first find the minimum element using a single pass or `std::min_element`. Then iterate through all elements, summing `element - min_value` into a `long long` accumulator to avoid integer overflow (since differences can be up to 1e9 and there could be many elements). Edge cases: a vector with one element requires 0 operations; duplicate minimums add zero for those positions; large values like 1e9 and many elements require a 64-bit type. Time complexity is O(n) because we do one pass to find min and another to sum, or we can combine them by computing min first and then summing. Space complexity is O(1) extra space. No special handling is needed for negative numbers because the sum of differences is non-negative.

#include <vector>
#include <algorithm>
#include <cstdint>

// Compute total decrements to make all elements equal to the minimum value.
long long minDecrementsToEqualize(const std::vector<int>& nums) {
    if (nums.empty()) return 0;

    const int min_val = *std::min_element(nums.begin(), nums.end());
    long long total = 0;
    for (const int& value : nums) {
        total += static_cast<long long>(value) - min_val;
    }
    return total;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    // Basic case
    std::vector<int> nums1 = {1, 1000000000};
    assert(minDecrementsToEqualize(nums1) == 999999999);

    // Single element
    assert(minDecrementsToEqualize({5}) == 0);

    // All equal
    assert(minDecrementsToEqualize({3, 3, 3}) == 0);

    // Negative values
    assert(minDecrementsToEqualize({-5, -1, -10}) == 9);

    // Large vector with many elements and large values
    std::vector<int> nums2(100000, 1000000000);
    nums2.push_back(1);
    assert(minDecrementsToEqualize(nums2) == 99999999LL * 100000);

    // Empty vector edge (though spec says non-empty, test for safety)
    assert(minDecrementsToEqualize({}) == 0);

    // Mixed values
    std::vector<int> nums3 = {7, 2, 9, 2, 4};
    assert(minDecrementsToEqualize(nums3) == (5 + 0 + 7 + 0 + 2)); // 14

    return 0;
}
