Write a C++ function `int maximumBeauty(std::vector<int>& nums, int k)` that, given a vector of integers and a non-negative integer `k`, returns the size of the longest subarray (contiguous segment) such that all elements in that subarray can be made equal by incrementing or decrementing each element by at most `k`. In other words, find the maximum length of a subarray where the difference between its maximum and minimum values is at most `2*k`. The input vector may be empty, may contain negative numbers, duplicates, and is not necessarily sorted. Your solution must not modify the input vector's order permanently; you may sort a copy if needed. The function must be `const`-correct with respect to the input (take `const std::vector<int>&`).

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maximumBeauty({1, 2, 3, 4, 5}, 1) == 3);  // e.g., [2,3,4] or [1,2,3]
    assert(maximumBeauty({1, 2, 3, 4, 5}, 0) == 1);  // all distinct, no change allowed
    assert(maximumBeauty({1, 1, 1, 1}, 5) == 4);     // all equal, any k works
    assert(maximumBeauty({}, 10) == 0);              // empty input
    
    // Negative numbers and duplicates
    assert(maximumBeauty({-5, -3, -1, 1, 3}, 2) == 4); // [-5,-3,-1,1] diff=6<=4? no; check: max-min=6, 2k=4 => fails; but [-3,-1,1] diff=4<=4 => 3; actually long: [-5,-3,-1] diff=4 => 3. Hmm correct is 3. Let's compute: sorted [-5,-3,-1,1,3], k=2 -> 2k=4. Longest window: [-5,-3,-1] diff4 => 3; [-3,-1,1] diff4 => 3; [-1,1,3] diff4 => 3. So answer 3.
    assert(maximumBeauty({-5, -3, -1, 1, 3}, 2) == 3);
    
    // Mixed with large range
    assert(maximumBeauty({100, 1, 50, 60, 70}, 20) == 3); // sorted [1,50,60,70,100], 2k=40. Longest: [50,60,70] diff20<=40 => 3; [1,50] diff49>40 no. So 3.
    
    // k=0 with duplicates
    assert(maximumBeauty({5, 5, 5, 2, 5}, 0) == 4); // three 5's? Actually four 5's? list has four 5's? It has 5,5,5,2,5 => four 5's. Longest run of same value: 4.
    
    // Single element
    assert(maximumBeauty({7}, 3) == 1);
    assert(maximumBeauty({7}, 0) == 1);
    
    // All elements within 2k of each other
    assert(maximumBeauty({1, 2, 3}, 1) == 3); // diff 2 <= 2
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Returns the size of the longest subarray where all elements can be made equal
// by changing each element by at most k (i.e., max-min <= 2*k).
int maximumBeauty(const std::vector<int>& nums, int k) {
    if (nums.empty()) return 0;
    
    // Work on a sorted copy to preserve the const input.
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());
    
    std::size_t left = 0;
    std::size_t right = 0;
    int result = 0;
    const std::size_t n = sorted.size();
    
    while (right < n) {
        // Expand the window to include sorted[right].
        // Adjust left until the window condition holds.
        while (sorted[right] - sorted[left] > 2 * k) {
            ++left;
        }
        int window_size = static_cast<int>(right - left + 1);
        result = std::max(result, window_size);
        ++right;
    }
    
    return result;
}

// The core insight is that if we sort the array, the problem becomes finding the longest contiguous subarray in the sorted order where `nums[j] - nums[i] <= 2*k`. This is because in a sorted array, the maximum difference within any subarray is simply the difference between its last and first elements. If that difference is at most `2*k`, then we can shift all elements to the median value (or any common value) with each element moving at most `k` steps. The algorithm uses a sliding window with two pointers: `left` and `right`. We iterate `right` from 0 to n-1. For each `right`, we maintain the window `[left, right]` such that `nums[right] - nums[left] <= 2*k`. If the condition is violated, we increment `left` until it holds again. The window length `right - left + 1` is a candidate answer, and we track the maximum. Edge cases: empty input returns 0; `k` can be 0 (then only identical elements are allowed); duplicates are naturally handled because the condition uses `<=`. Time complexity is O(n log n) due to sorting, and O(n) for the sliding window; space complexity is O(1) auxiliary (if we modify the input in-place, but we take const, so we copy the vector, making O(n) space for the copy). The sliding window itself uses O(1) extra space.
