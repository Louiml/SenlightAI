// Write a C++ function `bool canReachEnd(const std::vector<int>& jumps)` that determines whether it is possible to reach the last index of a non-empty array of non-negative integers, starting from index 0. At each index `i`, the value `jumps[i]` represents the maximum number of positions you can jump forward from that index (you may jump any number of steps from 1 up to that maximum). You win if you can reach or pass the final index. Return `true` if the last index is reachable, otherwise `false`. The input array may contain zeros, large numbers, and repeated values; handle the case where the first element alone is enough.
#include <cassert>
#include <vector>

int main() {
    // Single element: already at the end
    assert(canReachEnd({0}) == true);
    // Basic reachable case
    assert(canReachEnd({2, 3, 1, 1, 4}) == true);
    // Basic unreachable case with a zero trap
    assert(canReachEnd({3, 2, 1, 0, 4}) == false);
    // All zeros except start
    assert(canReachEnd({1, 0, 0, 0}) == false);
    // Large jumps overshoot the end
    assert(canReachEnd({5, 0, 0, 0}) == true);
    // Exact jumps to the last index
    assert(canReachEnd({1, 2, 0, 3, 1}) == true);
    // Zero at first index with more elements
    assert(canReachEnd({0, 1}) == false);
    // Many zeros but still reachable via early jump
    assert(canReachEnd({2, 0, 0}) == true);
    // Large array with sequence eventually stuck
    assert(canReachEnd({4, 0, 0, 0, 0, 0, 1}) == false);
}
#include <vector>
#include <algorithm>

// Determine if the last index of the vector is reachable from index 0.
// Each element gives the maximum forward jump length from that position.
bool canReachEnd(const std::vector<int>& jumps) {
    if (jumps.empty()) return false; // not specified, but safe
    int maxReach = 0;
    const int n = static_cast<int>(jumps.size());
    for (int i = 0; i < n; ++i) {
        if (i > maxReach) return false;
        maxReach = std::max(maxReach, i + jumps[i]);
        if (maxReach >= n - 1) return true;
    }
    return true;
}
// The solution uses a greedy approach with a single forward scan. Maintain a variable `maxReach` that stores the farthest index reachable from any position visited so far. Iterate through the array from left to right. If at any position `i` we find that `i > maxReach`, it means we cannot even reach this current index, so return `false`. Otherwise, update `maxReach = max(maxReach, i + jumps[i])`. If `maxReach` becomes greater than or equal to the last index, we can return `true` early. Key edge cases: an array of size 1 always returns `true` (we are already at the end). A value of 0 at index 0 with more than one element returns `false`. Values that overshoot the end are fine because we only need to reach or exceed the last index. Time complexity is O(n) because we traverse each element once. Space complexity is O(1) aside from input storage.
