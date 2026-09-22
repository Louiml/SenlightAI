// Write a C++ function named `canMakeSequence` that takes a vector of integers and returns `true` if the integers can be arranged into a sequence where every adjacent pair differs by at most 1, and `false` otherwise. The function must work for vectors of any size including 0 and 1. The input vector may contain duplicate values, negative numbers, and large numbers. The function should not modify the original vector unless necessary (you may copy it internally). Provide a standalone implementation with a descriptively named function.
The solution sorts the vector in ascending order. After sorting, if the original values can be arranged into a sequence where each step changes by at most 1, then the sorted order itself must have consecutive differences of at most 1 (since any valid sequence can be rearranged into sorted order without breaking the condition—if sorted adjacent values differ by more than 1, then no ordering can fix that gap). Therefore, after sorting, we check every pair `v[i]` and `v[i+1]`; if any difference exceeds 1, return `false`. If all differences are ≤ 1, return `true`. Edge cases: empty vector (trivially true), single element (true), duplicates (difference 0 is fine). Time complexity is O(n log n) due to sorting, space complexity O(n) for the copy if we keep the original const (or we can sort in place if we take by value). In the reference solution, we take the vector by value to allow sorting in place, avoiding extra copying.
#include <vector>
#include <algorithm>

// Returns true if the integers in nums can be arranged so that every
// adjacent pair differs by at most 1.
bool canMakeSequence(std::vector<int> nums) {
    if (nums.size() < 2) return true;
    std::sort(nums.begin(), nums.end());
    for (size_t i = 0; i < nums.size() - 1; ++i) {
        if (nums[i + 1] - nums[i] > 1) {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

bool canMakeSequence(std::vector<int> nums);

int main() {
    // Empty and single element
    assert(canMakeSequence({}) == true);
    assert(canMakeSequence({5}) == true);
    
    // Basic cases
    assert(canMakeSequence({1, 2, 3}) == true);
    assert(canMakeSequence({3, 1, 2}) == true);
    assert(canMakeSequence({1, 3}) == false);
    assert(canMakeSequence({1, 1, 2}) == true);
    
    // Negative and duplicates
    assert(canMakeSequence({-2, -1, 0, 1}) == true);
    assert(canMakeSequence({-5, -3}) == false);
    assert(canMakeSequence({2, 2, 2}) == true);
    
    // Large gaps
    assert(canMakeSequence({1, 100}) == false);
    assert(canMakeSequence({10, 11, 12, 15}) == false);
    
    // Duplicates with gap
    assert(canMakeSequence({1, 1, 3}) == false);
    
    return 0;
}
