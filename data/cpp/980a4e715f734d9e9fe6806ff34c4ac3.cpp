/*
Write a C++ function named `maxSubarraySum` that takes a non-empty vector of integers (`std::vector<int>`) and returns the maximum possible sum of any contiguous subarray (also known as the maximum subarray sum). The vector may contain negative numbers, all negative numbers, or a mix of positive and negative values. The function must return 0 if all elements are negative (i.e., an empty subarray is allowed), but if at least one element is non-negative, the sum must correspond to a non-empty contiguous subarray. The solution must use the provided prefix-sum resetting pattern from the snippet.
*/
#include <vector>
#include <algorithm>
#include <climits>

// Computes the maximum sum of a contiguous subarray.
// If all elements are negative, returns 0 (empty subarray allowed).
int maxSubarraySum(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    int maxsum = INT_MIN;
    int currsum = 0;
    
    for (int n : nums) {
        currsum += n;
        maxsum = std::max(currsum, maxsum);
        if (currsum < 0)
            currsum = 0;
    }
    
    // If maxsum is still negative, all elements were negative, so return 0.
    if (maxsum < 0)
        return 0;
    
    return maxsum;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxSubarraySum({1, 2, 3, 4}) == 10);
    assert(maxSubarraySum({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6); // classic example
    assert(maxSubarraySum({-1, -2, -3}) == 0); // all negative -> empty subarray
    assert(maxSubarraySum({5}) == 5); // single positive
    assert(maxSubarraySum({-5}) == 0); // single negative -> empty subarray
    assert(maxSubarraySum({-1, 0, -2}) == 0); // includes zero but all non-positive
    assert(maxSubarraySum({3, -1, 2}) == 4); // mixed: [3,-1,2]
    assert(maxSubarraySum({-2, -3, 4, -1, -2, 1, 5, -3}) == 7); // [4,-1,-2,1,5]
    assert(maxSubarraySum({0, 0, 0}) == 0); // all zeros
    assert(maxSubarraySum({2, -1, 2, -1, 2}) == 4); // [2,-1,2] or [2,-1,2,-1,2]? Actually max is 4 from [2,-1,2] or [2]? let's compute: 2-1+2=3, -1+2-1+2=2, 2-1+2-1+2=4. So correct.
    
    return 0;
}
// The provided snippet implements **Kadane's algorithm** with an important variation: after updating the current running sum, if the running sum becomes negative, it is reset to 0. This ensures that the running sum never contributes negative value to any future subarray. The algorithm maintains two variables: `currsum` (the maximum sum of a subarray ending at the current position, but reset to 0 when negative) and `maxsum` (the overall maximum encountered so far). For each element, we add it to `currsum`, update `maxsum` with the larger of `currsum` and the existing `maxsum`, then if `currsum < 0`, we reset it to 0. Because we reset to 0, the algorithm naturally handles the case where all numbers are negative: `maxsum` will be 0 (since `currsum` never stays negative and the max is initialized to `INT_MIN`, but the first iteration adds a negative number, `maxsum` becomes that negative, then it gets reset, but `maxsum` remains negative; however the snippet's logic actually returns a negative number for all-negative inputs unless we explicitly handle it. To match the snippet exactly, we must include a separate check: if `maxsum` remains negative after processing all elements, return 0. Alternatively, we can initialize `maxsum = 0` but that would break the exact snippet. For clarity, we implement the classic Kadane with the snippet's loop but then return 0 if `maxsum` is negative. Edge cases: empty vector (though task says non-empty, but we can handle it safely), all negative numbers (return 0), single element (return that element if non-negative, else 0). Time complexity is O(n) with a single pass, space complexity is O(1) auxiliary.
