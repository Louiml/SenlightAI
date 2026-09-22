Write a C++ function that takes a vector of integers `nums` containing the numbers from 1 to n, where n is the size of the vector, except that exactly one number is duplicated and one number is missing (so the multiset has n elements but contains one duplicate and one missing value from the range 1..n). Return a vector of two integers: the duplicate number first, then the missing number. For example, given `[1,2,2,4]`, return `[2,3]`. The input will always satisfy the problem constraints (exactly one duplicate and one missing). The function must handle vectors of any size ≥ 2 and work with values in the range 1..n.
#include <cassert>
#include <vector>

int main() {
    // Test cases from various scenarios
    assert(findDuplicateAndMissing({1, 2, 2, 4}) == std::vector<int>({2, 3}));
    assert(findDuplicateAndMissing({1, 1}) == std::vector<int>({1, 2}));
    assert(findDuplicateAndMissing({3, 2, 2}) == std::vector<int>({2, 1}));
    assert(findDuplicateAndMissing({1, 3, 3, 4}) == std::vector<int>({3, 2}));
    assert(findDuplicateAndMissing({2, 2}) == std::vector<int>({2, 1}));
    assert(findDuplicateAndMissing({1, 2, 3, 4, 5, 5, 7}) == std::vector<int>({5, 6}));
    assert(findDuplicateAndMissing({2, 3, 4, 4, 5}) == std::vector<int>({4, 1}));
    assert(findDuplicateAndMissing({1, 2, 3, 3, 5}) == std::vector<int>({3, 4}));
    assert(findDuplicateAndMissing({1, 1, 3, 4}) == std::vector<int>({1, 2}));
    assert(findDuplicateAndMissing({5, 5, 2, 3, 4}) == std::vector<int>({5, 1}));
    return 0;
}
#include <vector>
#include <cstddef>

// Given a vector of size n containing numbers 1..n with exactly one duplicate
// and one missing, returns {duplicate, missing}.
std::vector<int> findDuplicateAndMissing(const std::vector<int>& nums) {
    const std::size_t n = nums.size();
    std::vector<bool> seen(n, false);
    int duplicate = -1;
    
    for (std::size_t i = 0; i < n; ++i) {
        int val = nums[i];
        if (seen[val - 1]) {
            duplicate = val;
        } else {
            seen[val - 1] = true;
        }
    }
    
    for (std::size_t i = 0; i < n; ++i) {
        if (!seen[i]) {
            return {duplicate, static_cast<int>(i + 1)};
        }
    }
    
    return {duplicate, -1}; // Should never reach here for valid input.
}
// The problem is a classic "find the duplicate and missing" puzzle. The supplied code uses a boolean vector to mark which numbers from 1 to n have been seen. It iterates through the input; if a number is already marked, that number is the duplicate. Then it scans the boolean vector from index 1 to n to find the first unmarked index, which is the missing number. This approach is straightforward and correct because the input is guaranteed to have exactly one duplicate and one missing, so the boolean vector will have exactly one `false` entry.
//
// Edge cases: The duplicate can be any number in 1..n, and the missing can be 1 or n. The function must correctly handle cases where the duplicate is smaller or larger than the missing. Since the vector size is at least 2, we are safe. Time complexity is O(n) for the two passes. Space complexity is O(n) for the boolean vector. An alternative solution could use sum/2 sum of squares for O(1) space, but the boolean method is simpler and directly matches the snippet.
