/*
Write a C++ function `maximumScore` that takes a vector of non-negative integers `nums` and an integer index `k` (where `0 <= k < nums.size()`). You must start with a subarray consisting only of `nums[k]`, then repeatedly expand the subarray by adding either the element immediately to the left or the element immediately to the right (whichever is larger, with ties going to the right). After each expansion, compute the product of the current minimum element in the subarray and the subarray length. Return the maximum such product encountered during the entire process. The function must be `const`-correct (i.e., take the vector by const reference) and handle edge cases where the subarray expands to the full array.
*/
#include <vector>
#include <algorithm>

// Given a vector of non-negative integers and a starting index k,
// repeatedly expand the subarray [i,j] (initially i=j=k) by adding
// the larger of the two adjacent elements (ties go right), update
// the minimum, and return the maximum product of min * length.
int maximumScore(const std::vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());
    int i = k;
    int j = k;
    int currMin = nums[k];
    int result = nums[k]; // product for length 1

    while (i > 0 || j < n - 1) {
        int leftValue = 0;  // If i==0, treat as 0 (but i>0 check ensures valid)
        int rightValue = 0; // If j==n-1, treat as 0 (but j<n-1 check ensures valid)

        if (i > 0) leftValue = nums[i - 1];
        if (j < n - 1) rightValue = nums[j + 1];

        // Expand toward the larger value; ties go right
        if (leftValue > rightValue) {
            --i;
            currMin = std::min(currMin, nums[i]);
        } else {
            ++j;
            currMin = std::min(currMin, nums[j]);
        }

        int length = j - i + 1;
        result = std::max(result, currMin * length);
    }

    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Example from typical problem: nums = [1,4,3,7,4,5], k=3 → max is 15
    assert(maximumScore({1,4,3,7,4,5}, 3) == 15);
    // Single element
    assert(maximumScore({5}, 0) == 5);
    // k at left end
    assert(maximumScore({2,5,1}, 0) == 5);
    // k at right end
    assert(maximumScore({3,1,2}, 2) == 4);
    // All equal values
    assert(maximumScore({4,4,4,4}, 1) == 16);
    // Decreasing values from center
    assert(maximumScore({9,8,7,6}, 0) == 9);
    // Increasing values from center
    assert(maximumScore({1,2,3,4}, 2) == 8);
    // Larger test: full array minimum times length
    assert(maximumScore({1,1,1,1}, 2) == 4);
    // Zero values
    assert(maximumScore({0,5,0}, 1) == 5);
    // Ties go right: [2,2,2], k=1 → max = 6 (full array)
    assert(maximumScore({2,2,2}, 1) == 6);
}
// The core idea is a greedy expansion from the starting index `k`. At each step, the subarray is `[i, j]` initially with `i = j = k`. We consider the two possible next elements: `nums[i-1]` (if `i > 0`) and `nums[j+1]` (if `j < n-1`). To maximize the minimum value in the subarray for a given length, we should always add the larger of the two candidates; if both exist, pick the larger one, and if equal, expand right (as in the original snippet). After expanding, the new minimum is the minimum of the previous minimum and the newly added value. The score is `currmin * (j - i + 1)`, and we track the maximum over all steps. Edge cases: if `nums` has size 1, the result is `nums[k]` itself (since `i=j=k` and no expansion possible). If `k` is at an end (0 or `n-1`), only one direction is available. The loop runs exactly `n-1` times, one expansion per step, because the subarray grows by one element each iteration until it covers the entire array. Time complexity is `O(n)` since each element is added once. Space complexity is `O(1)` auxiliary, not counting the input vector.
