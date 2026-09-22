// Write a C++ function named `missingNumber` that accepts a vector of integers containing `n` distinct numbers from the range `[0, n]` (inclusive), where exactly one number from that range is missing. The function should return the missing number. The vector may be unsorted, contain no duplicates, and every number appears at most once. For example, if the input is `{3, 0, 1}`, the function must return `2`. Assume the vector always has at least one element and the elements satisfy the described property.

The key observation is that if the vector is sorted, every element at index `i` should equal `i` for all positions up to the missing value. After sorting, we iterate through the array and compare `nums[i]` to `i`. The first index where they differ is the missing number. If no mismatch is found after checking all `n` elements, then the missing number is `n` itself (the largest possible value). Edge cases include when the missing number is `0` (the first element will not equal its index) and when the missing number is `n` (all indices match, so we return `n`). Sorting the vector takes `O(n log n)` time and `O(1)` auxiliary space (depending on the sort implementation). The linear scan adds `O(n)` time, so overall time complexity is `O(n log n)` and space is `O(1)`.

#include <vector>
#include <algorithm>

// Returns the missing number from the range [0, n] given a vector of n distinct numbers
int missingNumber(std::vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
    int n = static_cast<int>(nums.size());
    
    for (int i = 0; i < n; ++i) {
        if (nums[i] != i) {
            return i;
        }
    }
    
    // If all indices matched, the missing number is n
    return n;
}

#include <cassert>
#include <vector>

int missingNumber(std::vector<int>& nums); // declared from solution

int main() {
    std::vector<int> v1 = {3, 0, 1};
    assert(missingNumber(v1) == 2);

    std::vector<int> v2 = {0, 1};
    assert(missingNumber(v2) == 2);

    std::vector<int> v3 = {9, 6, 4, 2, 3, 5, 7, 0, 1};
    assert(missingNumber(v3) == 8);

    std::vector<int> v4 = {0};
    assert(missingNumber(v4) == 1);

    std::vector<int> v5 = {1, 2, 3};
    assert(missingNumber(v5) == 0);

    std::vector<int> v6 = {5, 4, 3, 2, 1, 0, 7, 8, 9, 10};
    assert(missingNumber(v6) == 6);

    std::vector<int> v7 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(missingNumber(v7) == 11);

    std::vector<int> v8 = {2, 0};
    assert(missingNumber(v8) == 1);

    return 0;
}
