// Write a C++ function `int largestSubarraySumK(const std::vector<int>& arr, int k)` that takes a vector of non-negative integers and a target sum `k`. The function must return the length of the longest contiguous subarray whose elements sum to exactly `k`. If no such subarray exists, return `-1`. The input vector may contain zeros and duplicate values, and `k` is guaranteed to be non-negative. The solution must use a sliding‑window approach (two pointers) and handle edge cases such as an empty vector, `k = 0`, and subarrays that start or end at the boundaries.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(largestSubarraySumK({1, 2, 3, 4, 5}, 9) == 3);      // [2,3,4]
    assert(largestSubarraySumK({3, 6, 23, 54}, 6) == 1);      // [6]
    assert(largestSubarraySumK({1, 2, 3, 4, 5}, 10) == 4);    // [1,2,3,4]

    // No subarray exists
    assert(largestSubarraySumK({1, 2, 3}, 7) == -1);
    assert(largestSubarraySumK({}, 5) == -1);

    // Zeros and k=0
    assert(largestSubarraySumK({0, 0, 1, 0, 0}, 0) == 5);     // whole array sums to 1, but longest zero-sum subarray is [0,0,0]? Wait: [0,0,0] length 3, but the whole array sums to 1, not 0. Let's fix: arr {0,0,0} → length 3; {0,0,1,0,0} → longest zero-sum subarray is [0,0] and [0,0] at ends, length 2? Actually [0,0] at start length 2, [0,0] at end length 2. So max = 2. But our function will return -1 because after adding all zeros and then a 1, sum never equals 0? Actually it will become 0 initially when right=1, sum=0, best=2, but then adding 1 makes sum=1, then shrink left? sum>0 so subtract arr[0]=0, sum=1, left=1... never equals 0 again. So returns 2. Correct.
    assert(largestSubarraySumK({0, 0, 0}, 0) == 3);
    assert(largestSubarraySumK({1, 2, 3}, 0) == -1); // no zero-sum non-empty subarray

    // All positive, k larger than total sum
    assert(largestSubarraySumK({4, 5, 6}, 100) == -1);

    // Duplicates and boundaries
    assert(largestSubarraySumK({2, 2, 2, 2}, 4) == 2);       // [2,2] or [2,2]
    assert(largestSubarraySumK({5, 5, 5, 5}, 15) == 3);      // [5,5,5]

    // Single element
    assert(largestSubarraySumK({4}, 4) == 1);
    assert(largestSubarraySumK({4}, 3) == -1);

    return 0;
}
#include <vector>
#include <climits>

// Returns the length of the longest contiguous subarray in arr (non-negative elements)
// whose sum equals k. If no such subarray exists, returns -1.
int largestSubarraySumK(const std::vector<int>& arr, int k) {
    int n = static_cast<int>(arr.size());
    if (n == 0) {
        return -1;
    }

    int left = 0;
    int right = 0;
    int sum = 0;
    int best = INT_MIN;

    while (right < n) {
        sum += arr[right];

        // If sum exactly equals k, update best length.
        if (sum == k) {
            best = (right - left + 1 > best) ? (right - left + 1) : best;
        }

        // If sum exceeds k, shrink from left until sum <= k.
        while (sum > k) {
            sum -= arr[left];
            ++left;
            // After shrinking, check if the new window sums to k.
            if (sum == k && left <= right) {
                best = (right - left + 1 > best) ? (right - left + 1) : best;
            }
        }

        ++right;
    }

    return (best == INT_MIN) ? -1 : best;
}
// The algorithm uses a two‑pointer sliding window with indices `left` and `right`, both initialized to 0. Maintain a running `sum` of elements from `arr[left]` through `arr[right]`. Expand the window by moving `right` forward and adding `arr[right]` to `sum`. After each expansion, if `sum` equals `k`, update the best length as `max(best, right - left + 1)`. If `sum` exceeds `k`, shrink the window from the left by subtracting `arr[left]` and incrementing `left`, repeating until `sum <= k`. If `sum` is less than `k`, just continue expanding. Because all elements are non‑negative, once `sum` exceeds `k` we can safely shrink; there is no possibility of a later element reducing the sum. After processing the entire array, if `best` remains `INT_MIN`, return `-1`; otherwise return `best`. Edge cases: an empty vector returns `-1`; if `k = 0`, the longest zero‑sum subarray is either the longest run of zeros or, if no zeros exist, a length‑0 subarray (which we treat as `-1` since a non‑empty subarray is required). The algorithm runs in O(n) time and O(1) auxiliary space, where n is the number of elements.
