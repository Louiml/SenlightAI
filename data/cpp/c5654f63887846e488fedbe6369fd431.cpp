// Write a C++ function `int findDuplicate(const std::vector<int>& nums)` that, given a non-empty vector `nums` containing `n + 1` integers where each integer is in the range `[1, n]` inclusive and exactly one integer appears more than once, returns that duplicate integer. You may assume that there is exactly one duplicate, but it may appear more than twice, and the vector is read-only (cannot be modified). The function must not use extra auxiliary space beyond a few scalar variables (i.e., constant extra space) and must have linear time complexity. For example, given `[3,1,3,4,2]`, the function should return `3`.
This problem is the classic "Find the Duplicate Number" from LeetCode. The key insight is to treat the array as a linked list where `nums[i]` points to the next index. Because the values are in `[1, n]` and there are `n+1` elements, there must be a cycle in this implicit linked list, and the entrance of that cycle corresponds to the duplicate value. The algorithm uses Floyd's Tortoise and Hare (cycle detection). Start both `slow` and `fast` pointers at `nums[0]`. In each step, `slow` moves one step (`slow = nums[slow]`) and `fast` moves two steps (`fast = nums[nums[fast]]`). They are guaranteed to meet inside the cycle. After they meet, reset `slow` to `nums[0]` and move both pointers one step at a time until they meet again; that meeting point is the cycle entrance, which equals the duplicate value. Edge cases: the duplicate can appear exactly twice or many times, and the array length is at least 2 (since `n+1` with `n>=1`). The algorithm works for all such inputs because the cycle always exists. Time complexity is O(n), and space complexity is O(1) aside from the read-only input vector.
#include <vector>

// Returns the duplicate number in a vector where each value is in [1, n]
// and there is exactly one duplicate (appearing at least twice).
// Uses Floyd's cycle detection (tortoise and hare) in O(n) time and O(1) space.
int findDuplicate(const std::vector<int>& nums) {
    // Both pointers start at the first element (index 0, which is a value that points to another index).
    int slow = nums[0];
    int fast = nums[0];

    // Phase 1: find the meeting point inside the cycle.
    do {
        slow = nums[slow];          // move one step
        fast = nums[nums[fast]];    // move two steps
    } while (slow != fast);

    // Phase 2: find the cycle entrance (the duplicate value).
    slow = nums[0];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[fast];
    }

    return fast;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case with duplicate 3
    assert(findDuplicate({3, 1, 3, 4, 2}) == 3);

    // Duplicate appears multiple times (e.g., 5 appears three times)
    assert(findDuplicate({1, 2, 5, 3, 5, 4, 5}) == 5);

    // Duplicate is the smallest value (1)
    assert(findDuplicate({1, 2, 1, 3}) == 1);

    // Duplicate is the largest value (n)
    assert(findDuplicate({4, 2, 1, 3, 4}) == 4);

    // Duplicate appears exactly twice at the end
    assert(findDuplicate({1, 2, 3, 4, 4}) == 4);

    // Duplicate appears exactly twice at the beginning
    assert(findDuplicate({2, 2, 1, 3}) == 2);

    // n = 1 (vector has 2 elements, both are 1)
    assert(findDuplicate({1, 1}) == 1);

    // Larger test with random-like arrangement
    assert(findDuplicate({7, 1, 3, 6, 2, 4, 8, 5, 8}) == 8);

    // Duplicate in the middle
    assert(findDuplicate({1, 5, 4, 3, 5, 2}) == 5);

    // Already sorted with duplicate at middle
    assert(findDuplicate({1, 2, 3, 3, 4, 5}) == 3);

    return 0;
}
