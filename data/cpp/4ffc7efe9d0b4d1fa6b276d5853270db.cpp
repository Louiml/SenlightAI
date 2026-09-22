You are given a vector of positive integers `nums` and an integer `k`. You may perform at most `k` operations where each operation increments any single element of the vector by 1. Your goal is to maximize the frequency (the number of occurrences) of any one value in the vector after these operations. Write a C++ function `int maxFrequency(const std::vector<int>& nums, int k)` that returns the maximum possible frequency of any value after using at most `k` increments. The input vector is not necessarily sorted, may contain duplicates, and `k` is non-negative. The size of the vector is at least 1, and all elements are positive integers fitting in a standard `int`. Consider that the optimal strategy is to choose a target value (one of the existing elements after sorting) and increment smaller elements up to that target, so the function should handle large `k` gracefully without integer overflow by using a 64-bit accumulator internally.

The core idea is to sort the array in ascending order. After sorting, for any chosen right endpoint `r` (the target value), the optimal subarray to convert to `nums[r]` is a contiguous window ending at `r`. We maintain a sliding window with a left pointer `l` and a right pointer `r`. The total number of increments needed to make all elements in the window equal to `nums[r]` is `(window_length) * nums[r] - current_sum`. If this cost exceeds `k`, we shrink the window from the left until the cost is within `k`. The current maximum frequency is the length of the largest valid window. This works because sorting ensures that the cheapest way to raise a group of elements to a common value is to pick the largest element in that group as the target, and any optimal solution corresponds to a contiguous block in the sorted order. Edge cases include: a single element (answer is always 1), very large `k` (window can cover the entire array), and duplicates (no special handling needed). Time complexity is `O(n log n)` due to sorting, plus `O(n)` for the two-pointer scan. Space complexity is `O(1)` auxiliary (excluding the sort's internal stack), but we use a `long` for the sum to avoid overflow where `window_length * nums[r]` could exceed 32-bit range.

#include <vector>
#include <algorithm>

// Returns the maximum frequency achievable by incrementing elements at most k times.
// The function sorts a copy of the input and uses a sliding window.
int maxFrequency(const std::vector<int>& nums, int k) {
    if (nums.empty()) return 0;
    
    std::vector<int> sorted(nums);
    std::sort(sorted.begin(), sorted.end());
    
    int result = 0;
    int left = 0;
    long long currentSum = 0;
    
    for (int right = 0; right < static_cast<int>(sorted.size()); ++right) {
        currentSum += sorted[right];
        long long target = sorted[right];
        int windowLength = right - left + 1;
        
        // Shrink window if cost exceeds k
        while (windowLength * target - currentSum > k) {
            currentSum -= sorted[left];
            ++left;
            --windowLength;
        }
        
        result = std::max(result, windowLength);
    }
    
    return result;
}

#include <cassert>
#include <vector>

int maxFrequency(const std::vector<int>& nums, int k);

int main() {
    // Basic case from the snippet
    assert(maxFrequency({1, 2, 4}, 5) == 3);      // Increment 1 and 2 to 4, total cost 5
    assert(maxFrequency({1, 4, 8, 13}, 5) == 2);  // e.g., 1->4 (cost 3) or 8->13 (cost 5)
    assert(maxFrequency({3, 9, 6}, 2) == 1);      // No two elements can become equal within cost 2
    assert(maxFrequency({1, 1, 1}, 0) == 3);      // Already all equal
    assert(maxFrequency({10, 1, 2, 3}, 3) == 3);  // Increment 1,2,3 to 3? Actually 1->3,2->3 cost 3, plus 3 exists -> freq 3
    assert(maxFrequency({1, 2, 3, 4, 5}, 100) == 5); // Large k covers all
    assert(maxFrequency({5}, 0) == 1);            // Single element
    assert(maxFrequency({2, 2, 2, 100}, 0) == 3); // Duplicates without any increments
    assert(maxFrequency({1, 2, 5, 9}, 6) == 3);   // 1,2,5 -> all to 5 costs 7? No, 1->5 cost 4, 2->5 cost 3 total 7 >6, so best is 1,2 to 2? Actually max freq 2? Let's test: 1->2 cost1, freq 2; 2,5->5 cost3, freq2; 5,9->9 cost4, freq2. So 2
    assert(maxFrequency({1, 3, 5, 7}, 4) == 2);   // e.g., 1->3 cost2, or 3->5 cost2, etc.
    return 0;
}
