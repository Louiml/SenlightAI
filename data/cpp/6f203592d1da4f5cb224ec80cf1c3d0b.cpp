/*
Given a non-empty vector of integers, write a C++ function `prefixConversionScores` that returns a vector of `long long` where the i-th element is the cumulative sum of the i-th "conversion value". The conversion value for a prefix ending at index i is defined as `max(nums[0..i]) + nums[i]`. In other words, for each prefix of the input array, compute: (a) the maximum element in that prefix, (b) add that maximum to the last element of the prefix, and (c) accumulate these per-prefix values into a running sum that becomes the output element for that index. The function must handle both positive and negative integers, including all-negative or all-positive inputs, and must work efficiently for arrays up to 10^5 elements, where each value fits in a 32-bit signed int. Note that the maximum prefix value may change as you scan, but it is monotonic non-decreasing; use this property to avoid recomputing it each time. Return the resulting vector of cumulative sums.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Compute cumulative prefix scores where each score is a running sum of
// (max of prefix so far) + (current element).
std::vector<long long> prefixConversionScores(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    std::vector<long long> result;
    result.reserve(n);
    
    int currentMax = INT_MIN;
    long long runningSum = 0;
    
    for (int i = 0; i < n; ++i) {
        currentMax = std::max(currentMax, nums[i]);
        long long conversionValue = static_cast<long long>(currentMax) + nums[i];
        runningSum += conversionValue;
        result.push_back(runningSum);
    }
    
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Example from prompt
    std::vector<int> nums1 = {2, 3, 7, 5, 10};
    std::vector<long long> expected1 = {4, 10, 24, 36, 56};
    assert(prefixConversionScores(nums1) == expected1);
    
    // Single element
    std::vector<int> nums2 = {5};
    std::vector<long long> expected2 = {10};
    assert(prefixConversionScores(nums2) == expected2);
    
    // All negative
    std::vector<int> nums3 = {-3, -5, -2, -10};
    // prefix1: max=-3, conv=-3+(-3)=-6, sum=-6
    // prefix2: max=-3, conv=-3+(-5)=-8, sum=-14
    // prefix3: max=-2, conv=-2+(-2)=-4, sum=-18
    // prefix4: max=-2, conv=-2+(-10)=-12, sum=-30
    std::vector<long long> expected3 = {-6, -14, -18, -30};
    assert(prefixConversionScores(nums3) == expected3);
    
    // Duplicate maximums and mixed signs
    std::vector<int> nums4 = {4, 4, -1, 4};
    // p1: max=4, conv=4+4=8, sum=8
    // p2: max=4, conv=4+4=8, sum=16
    // p3: max=4, conv=4+(-1)=3, sum=19
    // p4: max=4, conv=4+4=8, sum=27
    std::vector<long long> expected4 = {8, 16, 19, 27};
    assert(prefixConversionScores(nums4) == expected4);
    
    // Large values to ensure long long correctness
    std::vector<int> nums5 = {1000000000, 1000000000};
    // Both conversions: 2,000,000,000 each, sums: 2e9 then 4e9 (fits in long long)
    std::vector<long long> expected5 = {2000000000LL, 4000000000LL};
    assert(prefixConversionScores(nums5) == expected5);
    
    // Strictly increasing
    std::vector<int> nums6 = {1, 2, 3, 4};
    // p1: max=1, conv=2, sum=2
    // p2: max=2, conv=4, sum=6
    // p3: max=3, conv=6, sum=12
    // p4: max=4, conv=8, sum=20
    std::vector<long long> expected6 = {2, 6, 12, 20};
    assert(prefixConversionScores(nums6) == expected6);
    
    return 0;
}

// The key idea is to process the array from left to right while maintaining the current maximum value seen so far (`currentMax`) and the cumulative sum of all conversion values (`runningSum`). For each element `nums[i]`, we first update `currentMax = max(currentMax, nums[i])`. The conversion value for this prefix is `currentMax + nums[i]`. We add this conversion value to `runningSum` and store `runningSum` as the result for index i. This works because the score at each prefix is defined recursively: `score[i] = score[i-1] + conversion[i]`. Edge cases include a single-element array (where the conversion value is `nums[0] + nums[0]`), negative numbers (where the maximum may initially be negative or change to a larger negative), and duplicate maximums (which do not affect correctness). Time complexity is O(n) since we scan the array once and each step is O(1). Space complexity is O(n) for the output vector, but O(1) auxiliary space if we ignore the output storage.
