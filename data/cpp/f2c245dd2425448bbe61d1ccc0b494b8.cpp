// Given a sequence of positive integers, write a C++ function `int findMissingGap(const std::vector<int>& nums)` that returns the smallest integer `x` such that `x` is not present in the sequence, and `x+1` is also not present, and the difference between `x` and the next larger present integer is exactly 2 when considering a sorted distinct version of the input. In other words, after sorting the unique numbers, find the first position where a gap of exactly 2 occurs between consecutive numbers (i.e., `b - a == 2`), then return `a + 1`. If no such pair exists, return 0. The input may contain duplicates, and the sequence may be empty. For example, for `[3, 5, 7]`, the sorted unique numbers are `[3,5,7]`, gaps are 2 and 2, so return `3+1 = 4`. For `[1,2,3]` gaps are 1 and 1, no gap of 2, return 0. For `[5, 10, 12]`, gaps are 5 and 2, the first gap of 2 is between 10 and 12, so return `10+1 = 11`. For empty input, return 0. For `[1]`, no gap possible, return 0.

The solution begins by copying the input vector, sorting it, and then removing duplicate elements using `std::unique` on the sorted range. This gives a sequence of distinct, sorted numbers. Then we iterate through consecutive pairs of this unique list. Whenever the difference between the current value and the previous value is exactly 2, we immediately return `previous + 1` because that is the missing integer that lies exactly in the middle of that gap. If we finish the loop without finding such a difference, we return 0. Important edge cases: empty input (return 0), single element (no pairs to check, return 0), duplicates (handled by unique), and cases where there are multiple gaps of 2 — we only care about the first one in sorted order. Time complexity is O(n log n) due to sorting, where n is the number of elements. Space complexity is O(n) for the copy used in sorting, but we can sort the parameter by value to avoid extra copies. The function should be `const` correct by taking the vector by value (since we need to modify it) or by const reference and making a local copy.

#include <vector>
#include <algorithm>

// Given a list of positive integers, return the missing integer that lies
// in the first gap of exactly 2 between consecutive distinct sorted values.
// Return 0 if no such gap exists.
int findMissingGap(std::vector<int> nums) {
    if (nums.size() < 2) return 0;
    
    std::sort(nums.begin(), nums.end());
    nums.erase(std::unique(nums.begin(), nums.end()), nums.end());
    
    if (nums.size() < 2) return 0;
    
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] - nums[i-1] == 2) {
            return nums[i-1] + 1;
        }
    }
    return 0;
}

#include <cassert>
#include <vector>

int findMissingGap(std::vector<int> nums);

int main() {
    // Empty input
    assert(findMissingGap({}) == 0);
    // Single element
    assert(findMissingGap({5}) == 0);
    // Consecutive numbers, no gap of 2
    assert(findMissingGap({1,2,3}) == 0);
    // Simple gap of 2 in middle
    assert(findMissingGap({1,2,4}) == 3);
    // Gap of 2 at the beginning of pair
    assert(findMissingGap({3,5,7}) == 4);
    // Larger gap before a gap of 2
    assert(findMissingGap({5,10,12}) == 11);
    // Duplicates and unsorted
    assert(findMissingGap({4,1,1,3}) == 2); // sorted unique: 1,3,4 → gap of 2 between 1 and 3
    // Duplicates with no gap of 2
    assert(findMissingGap({2,2,2,3,3}) == 0); // sorted unique: 2,3
    // Negative numbers (though task says positive, we test robustness)
    assert(findMissingGap({-1,1}) == 0);
    assert(findMissingGap({-3,-1,0}) == -2); // gap of 2 between -3 and -1
    // Large values
    assert(findMissingGap({1000000, 1000002}) == 1000001);
    // Multiple gaps of 2, first one wins
    assert(findMissingGap({1,3,5,7}) == 2);
    return 0;
}
