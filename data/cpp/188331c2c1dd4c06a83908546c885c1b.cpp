/*
Given a non-empty vector of non-negative integers where each element represents the maximum jump length from that position, write a C++ function `bool canReachEnd(const std::vector<int>& nums)` that determines whether you can reach the last index starting from index 0. You may only jump forward (i.e., from index i you can jump to any index j with i < j <= i + nums[i]). The function must return `true` if the last index is reachable, and `false` otherwise. The vector length is at least 1, and the last index is always index `size-1`. Handle cases where the first element is 0 (only possible if the vector size is 1) and where large jumps allow skipping over zero positions.
*/
#include <vector>
#include <algorithm> // for std::max

// Determine whether the last index can be reached from index 0,
// where nums[i] is the maximum jump length from index i.
bool canReachEnd(const std::vector<int>& nums) {
    int farthest = 0;
    int size = static_cast<int>(nums.size());
    // Iterate while current index is within reach and we haven't already reached the end.
    for (int i = 0; i < size && i <= farthest && farthest < size - 1; ++i) {
        farthest = std::max(farthest, i + nums[i]);
    }
    return farthest >= size - 1;
}
#include <cassert>
#include <vector>

// The function under test is declared above (or included here).

int main() {
    // Basic cases
    assert(canReachEnd(std::vector<int>{2, 3, 1, 1, 4}) == true);
    assert(canReachEnd(std::vector<int>{3, 2, 1, 0, 4}) == false);
    
    // Single-element vectors
    assert(canReachEnd(std::vector<int>{0}) == true);
    assert(canReachEnd(std::vector<int>{5}) == true);
    
    // Zeros in the middle but reachable
    assert(canReachEnd(std::vector<int>{2, 0, 0}) == true);
    assert(canReachEnd(std::vector<int>{1, 0, 1}) == false);
    
    // Large jumps skipping positions
    assert(canReachEnd(std::vector<int>{4, 0, 0, 0, 0}) == true);
    assert(canReachEnd(std::vector<int>{0, 1}) == false); // cannot move from start
    
    // All zeros except first
    assert(canReachEnd(std::vector<int>{1, 0, 0, 0}) == true);
    assert(canReachEnd(std::vector<int>{0, 0, 0}) == false);
    
    // Non-trivial with multiple paths
    assert(canReachEnd(std::vector<int>{2, 0, 2, 0, 1}) == true);
    assert(canReachEnd(std::vector<int>{1, 1, 1, 0}) == true);
    
    return 0;
}
// The solution uses a greedy approach: maintain a variable `farthest` that tracks the maximum index reachable so far. Iterate through the array from left to right, but only while the current index `i` is within the reachable range (i.e., `i <= farthest`) and we haven't already reached or passed the last index (`farthest < size - 1`). For each visited position, update `farthest` to be the maximum of its current value and `i + nums[i]`. If we ever find that we cannot move past an index (i.e., `i` exceeds `farthest`), the loop terminates early and `farthest` remains less than `size-1`, indicating failure. The key edge case is when the vector contains only one element: then `farthest` is already `size-1` (since size-1 is 0), and the loop condition `farthest < size - 1` is false from the start, so it correctly returns `true`. Another edge case is when there are zeros in the middle; the greedy approach still works because we only need to know if there exists a path, and the farthest reachable index is non-decreasing. The time complexity is O(n) because we scan each index at most once, and the space complexity is O(1) beyond the input.
