// Write a C++ function `int insertSorted(std::vector<int>& nums, int target)` that takes a sorted vector of integers (non-decreasing order) and returns the index where `target` would be inserted to maintain sorted order. However, the function must also modify the vector in place: if `target` already exists in the vector, remove that single occurrence (so the vector length decreases by 1) and return the index where that removed element was located. If `target` is not present, insert it into the correct position (so the vector length increases by 1) and return the index of the newly inserted element. The input vector may be empty, and it is guaranteed to be sorted. The function should handle duplicates correctly: if there are multiple occurrences of `target`, remove exactly one (the first found) and return its original index; if inserting, insert after all existing equal elements? For simplicity, choose to insert before any equal elements (i.e., at the first position where `target` is less than or equal to the next larger element). Edge cases include an empty vector, target smaller than all elements, target larger than all elements, and target equal to existing elements.
// The core idea is to simulate insertion or deletion while tracking the correct index. A straightforward approach: first perform a linear scan from the end of the vector backward (or forward, but backward matches the original snippet). We can reuse the logic: extend the vector by one slot (if inserting) or find the matching element. But simpler and more robust: use `std::lower_bound` to find the insertion position for `target` (the first index where `target` <= element). If that position is not at the end and the element at that position equals `target`, then we remove that element and return its index. Otherwise, we insert `target` at that position and return the same index. This handles all cases cleanly. For an empty vector, `lower_bound` returns 0, we insert and return 0. For duplicates, `lower_bound` gives the first equal element; we remove that one. Time complexity is O(n) due to vector insertion/deletion shifting elements; `lower_bound` itself is O(log n), but the shift dominates. Space complexity is O(1) auxiliary. Edge cases: empty vector, target smaller than all, larger than all, equal to first/last element, duplicates.
#include <vector>
#include <algorithm>

// Inserts target into sorted nums if not present, returning the insertion index.
// If target already exists, removes one occurrence and returns its original index.
int insertSorted(std::vector<int>& nums, int target) {
    // Find the first position where target could be inserted to maintain sorted order.
    auto pos = std::lower_bound(nums.begin(), nums.end(), target);
    size_t index = pos - nums.begin();

    // Check if target already exists at the found position.
    if (pos != nums.end() && *pos == target) {
        // Remove the first occurrence of target.
        nums.erase(pos);
        // The index remains the same because we removed the element at that position.
        return static_cast<int>(index);
    } else {
        // Insert target at the found position.
        nums.insert(pos, target);
        return static_cast<int>(index);
    }
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Empty vector -> insert and return 0.
    {
        std::vector<int> nums = {};
        assert(insertSorted(nums, 5) == 0);
        assert(nums == std::vector<int>({5}));
    }

    // Test 2: Insert at beginning.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 5) == 0);
        assert(nums == std::vector<int>({5, 10, 20, 30}));
    }

    // Test 3: Insert at end.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 40) == 3);
        assert(nums == std::vector<int>({10, 20, 30, 40}));
    }

    // Test 4: Insert in middle.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 25) == 2);
        assert(nums == std::vector<int>({10, 20, 25, 30}));
    }

    // Test 5: Remove existing element.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 20) == 1);
        assert(nums == std::vector<int>({10, 30}));
    }

    // Test 6: Remove first element.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 10) == 0);
        assert(nums == std::vector<int>({20, 30}));
    }

    // Test 7: Remove last element.
    {
        std::vector<int> nums = {10, 20, 30};
        assert(insertSorted(nums, 30) == 2);
        assert(nums == std::vector<int>({10, 20}));
    }

    // Test 8: Duplicates - remove first occurrence.
    {
        std::vector<int> nums = {10, 20, 20, 30};
        assert(insertSorted(nums, 20) == 1);
        assert(nums == std::vector<int>({10, 20, 30}));
    }

    // Test 9: Duplicates - insert before all equal elements.
    {
        std::vector<int> nums = {10, 20, 20, 30};
        assert(insertSorted(nums, 15) == 1);
        assert(nums == std::vector<int>({10, 15, 20, 20, 30}));
    }

    // Test 10: Single element vector, insert smaller.
    {
        std::vector<int> nums = {7};
        assert(insertSorted(nums, 3) == 0);
        assert(nums == std::vector<int>({3, 7}));
    }

    return 0;
}
