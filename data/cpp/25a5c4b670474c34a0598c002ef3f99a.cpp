// Write a C++ function named `canReachEnd` that takes a non-empty vector of non-negative integers (`std::vector<int>`) representing the maximum jump length from each position, and returns a boolean indicating whether it is possible to reach the last index starting from index 0. The function must implement a greedy or dynamic programming approach that processes the array in a single forward pass, tracking the farthest reachable index. Handle the edge case where the vector has only one element (always reachable). The function should use `const` reference for the input vector and be declared as a free function (not a class method).

The solution maintains a variable representing the farthest index that can be reached from any position processed so far. Initially, this is set to `nums[0]` (the max jump from index 0). If at any point the current index `i` is greater than the farthest reachable index, it means we cannot proceed further, so return false. Otherwise, update the farthest reachable index as the maximum of its current value and `i + nums[i]`. If at any point the farthest reachable index is at least the last index (`n-1`), we can return true early. After processing all indices up to `n-2`, if the farthest reachable index is still less than `n-1`, return false. The algorithm correctly handles the edge case `n == 1` by returning true immediately. The time complexity is O(n) and space complexity is O(1), since we only use a single integer variable.

#include <vector>
#include <algorithm>

// Returns true if the last index of nums can be reached from index 0.
bool canReachEnd(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 1) {
        return true;
    }

    int farthest = nums[0]; // farthest reachable index from the start
    for (int i = 1; i < n - 1; ++i) {
        if (farthest < i) {
            return false;
        }
        // Update farthest if the current position can jump further.
        farthest = std::max(farthest, i + nums[i]);
        if (farthest >= n - 1) {
            return true;
        }
    }

    return farthest >= n - 1;
}

#include <cassert>
#include <vector>

int main() {
    assert(canReachEnd({2,3,1,1,4}) == true);
    assert(canReachEnd({3,2,1,0,4}) == false);
    assert(canReachEnd({0}) == true);
    assert(canReachEnd({1}) == true);
    assert(canReachEnd({0,1}) == false);
    assert(canReachEnd({1,0}) == true);
    assert(canReachEnd({2,0,0}) == true);
    assert(canReachEnd({1,1,0,1}) == false);
    assert(canReachEnd({5,0,0,0,0,0}) == true);
    assert(canReachEnd({1,2,3,4,5}) == true);
}
