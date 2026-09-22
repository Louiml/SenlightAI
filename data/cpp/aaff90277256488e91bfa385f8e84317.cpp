Write a C++ function that takes a vector of integers and an integer target value, removes all occurrences of the target value in-place, and then fills the remaining positions at the end of the vector with zeros. The function should preserve the relative order of the non-target elements and return a new vector containing only the non-target elements in their original order (without the appended zeros). For example, given `{0, 1, 4, 0, 2}` and target `0`, the input vector becomes `{1, 4, 2, 0, 0}` and the function returns `{1, 4, 2}`. If the input vector is empty or contains only target values, return an empty vector. The function must not use any additional container allocations beyond the returned vector.
// The task is a two‑pass in‑place algorithm. First, iterate through the vector with two indices: a "reader" that scans every element, and a "writer" that marks where the next non‑target element should be placed. Whenever the reader finds an element different from the target, copy it to the writer position and advance the writer. After this pass, the writer’s index equals the number of non‑target elements. The elements from that index to the end of the vector are no longer needed, so overwrite them with zeros. Finally, build the result vector from the first `writer` elements of the modified vector (these are exactly the non‑target elements in their original order). Edge cases: empty input (return empty, no changes), all elements are target (writer stays 0, fill all with zeros, return empty), no target present (writer reaches the end, no zeros appended, return a copy of the original). Time complexity is O(n) because each element is processed a constant number of times (once in the read/write pass, once in the zero‑fill). Space complexity is O(n) for the returned vector, but the modification of the input is done in O(1) extra space besides the result. Const‑correctness: the input vector is taken by reference and modified, so we must not mark it const.
#include <vector>

// Remove all occurrences of target from nums, fill the vacated tail with zeros,
// and return a vector containing the remaining non-target elements in original order.
std::vector<int> removeTargetAndZeroFill(std::vector<int>& nums, int target) {
    int writer = 0; // position to place the next non-target element
        
    // First pass: move all non-target elements to the front.
    for (int reader = 0; reader < static_cast<int>(nums.size()); ++reader) {
        if (nums[reader] != target) {
            nums[writer] = nums[reader];
            ++writer;
        }
    }
    
    // Second pass: overwrite the remaining positions with zeros.
    for (int i = writer; i < static_cast<int>(nums.size()); ++i) {
        nums[i] = 0;
    }
    
    // Build the result from the first 'writer' elements (the non-target ones).
    std::vector<int> result(nums.begin(), nums.begin() + writer);
    return result;
}
#include <cassert>
#include <vector>

// The function declaration is copied here for clarity (the real solution is above).
std::vector<int> removeTargetAndZeroFill(std::vector<int>& nums, int target);

int main() {
    // Example from the prompt
    std::vector<int> v1 = {0, 1, 4, 0, 2};
    auto res1 = removeTargetAndZeroFill(v1, 0);
    assert(res1 == std::vector<int>({1, 4, 2}));
    assert(v1 == std::vector<int>({1, 4, 2, 0, 0}));
    
    // Empty input
    std::vector<int> v2 = {};
    auto res2 = removeTargetAndZeroFill(v2, 5);
    assert(res2.empty());
    assert(v2.empty());
    
    // All elements are the target
    std::vector<int> v3 = {7, 7, 7};
    auto res3 = removeTargetAndZeroFill(v3, 7);
    assert(res3.empty());
    assert(v3 == std::vector<int>({0, 0, 0}));
    
    // No target present
    std::vector<int> v4 = {1, 2, 3};
    auto res4 = removeTargetAndZeroFill(v4, 0);
    assert(res4 == std::vector<int>({1, 2, 3}));
    assert(v4 == std::vector<int>({1, 2, 3}));
    
    // Negative numbers and duplicates
    std::vector<int> v5 = {-1, 0, -2, 0, 3, -1};
    auto res5 = removeTargetAndZeroFill(v5, 0);
    assert(res5 == std::vector<int>({-1, -2, 3, -1}));
    assert(v5 == std::vector<int>({-1, -2, 3, -1, 0, 0}));
    
    // Single element not equal to target
    std::vector<int> v6 = {9};
    auto res6 = removeTargetAndZeroFill(v6, 0);
    assert(res6 == std::vector<int>({9}));
    assert(v6 == std::vector<int>({9}));
    
    // Single element equal to target
    std::vector<int> v7 = {4};
    auto res7 = removeTargetAndZeroFill(v7, 4);
    assert(res7.empty());
    assert(v7 == std::vector<int>({0}));
    
    return 0;
}
