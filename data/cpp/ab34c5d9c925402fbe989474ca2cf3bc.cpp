/*
Write a C++ function that takes a vector of integers (`nums`) and an integer `k`, and returns the maximum possible frequency of any single value after performing at most `k` increment operations, where each operation increases any chosen element by 1. The function should sort the array and use a sliding window to find the longest contiguous subarray (after sorting) such that the total number of increments needed to make all elements in that window equal to the maximum element of the window does not exceed `k`. Return the length of such a longest window.
*/
#include <vector>
#include <algorithm>

// Returns the maximum frequency achievable after at most k increments.
int maximumFrequency(std::vector<int>& nums, int k) {
    if (nums.empty()) return 0;
    std::sort(nums.begin(), nums.end());
    int left = 0;
    int best = 0;
    long long windowSum = 0;
    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        windowSum += nums[right];
        // Check if making all elements in [left, right] equal to nums[right] exceeds k.
        while (static_cast<long long>(nums[right]) * (right - left + 1) > windowSum + k) {
            windowSum -= nums[left];
            ++left;
        }
        best = std::max(best, right - left + 1);
    }
    return best;
}
#include <cassert>
#include <vector>

// Function declaration (as per solution above, but included here for completeness)
int maximumFrequency(std::vector<int>& nums, int k);

int main() {
    std::vector<int> v1 = {1, 2, 4};
    assert(maximumFrequency(v1, 5) == 3);

    std::vector<int> v2 = {1, 4, 8, 13};
    assert(maximumFrequency(v2, 5) == 2);

    std::vector<int> v3 = {3, 9, 6};
    assert(maximumFrequency(v3, 2) == 1);

    std::vector<int> v4 = {1, 1, 1};
    assert(maximumFrequency(v4, 0) == 3);

    std::vector<int> v5 = {5};
    assert(maximumFrequency(v5, 10) == 1);

    std::vector<int> v6 = {};
    assert(maximumFrequency(v6, 3) == 0);

    std::vector<int> v7 = {1, 2, 2, 3};
    assert(maximumFrequency(v7, 2) == 3); // make 1,2,2 -> 2,2,2

    std::vector<int> v8 = {2, 2, 2, 2};
    assert(maximumFrequency(v8, 100) == 4);

    std::vector<int> v9 = {1, 2, 3, 4, 5};
    assert(maximumFrequency(v9, 0) == 1);

    std::vector<int> v10 = {1, 10, 10, 10};
    assert(maximumFrequency(v10, 9) == 4); // increment 1 nine times to 10

    return 0;
}
// The solution sorts the array in ascending order, so that any contiguous window represents a set of numbers we can try to make equal to the window's largest element (the rightmost element). For a window from index `l` to `r` (inclusive), the total increments required is `nums[r] * (r - l + 1) - sum(nums[l..r])`. This must be ≤ `k`. We maintain a sliding window: expand the right pointer `r`, adding `nums[r]` to a running sum. If the required increments exceed `k`, we shrink from the left by subtracting `nums[l]` and incrementing `l`. The window's validity is ensured by the while loop. The answer is the maximum window length seen. Edge cases include an empty vector (return 0), a single element (return 1), and large values where `nums[r] * length` may overflow a 32-bit int, so we use `long long`. Time complexity is O(n log n) due to sorting, plus O(n) for the sliding window; space complexity is O(1) auxiliary (excluding sorting's internal storage).
