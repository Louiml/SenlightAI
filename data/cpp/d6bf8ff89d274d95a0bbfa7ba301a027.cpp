Write a C++ function named `minimumJumpsToReachEnd` that takes a non-empty vector of non-negative integers, where each element `nums[i]` represents the maximum jump length from that index, and returns the minimum number of jumps required to reach the last index. You are guaranteed that it is always possible to reach the last index. The function should use an efficient greedy approach with O(1) extra space beyond the input vector. For example, for `[2,3,1,1,4]`, the minimum jumps is 2 (from index 0 to 1, then to 4). The function must be `const`-correct and operate on a read-only reference to the vector.
#include <cassert>
#include <vector>

int main() {
    // Single element: no jumps needed.
    assert(minimumJumpsToReachEnd({0}) == 0);

    // Already at the last index with one move.
    assert(minimumJumpsToReachEnd({1, 0}) == 1);

    // Classic example: [2,3,1,1,4] -> 2 jumps (0->1, then 1->4).
    assert(minimumJumpsToReachEnd({2, 3, 1, 1, 4}) == 2);

    // [2,1,2,1,4] -> 2 jumps (0->2, then 2->4).
    assert(minimumJumpsToReachEnd({2, 1, 2, 1, 4}) == 2);

    // [1,1,1,1] -> each index jumps by 1, so 3 jumps.
    assert(minimumJumpsToReachEnd({1, 1, 1, 1}) == 3);

    // [3,0,0,0] -> 1 jump from index 0 to last.
    assert(minimumJumpsToReachEnd({3, 0, 0, 0}) == 1);

    // Large jump at start: [4,1,1,1,0] -> 1 jump.
    assert(minimumJumpsToReachEnd({4, 1, 1, 1, 0}) == 1);

    // Zero elements in middle: [1,0,2,1] -> 2 jumps (0->1, 1->? actually 0 jump at index1; but guaranteed reachable, so use [2,0,1,0] -> 0->2->3 = 2 jumps).
    assert(minimumJumpsToReachEnd({2, 0, 1, 0}) == 2);

    // [1,2,3] -> 0->1->2 = 2 jumps, but better 0->1 (jump 2) covers to index 3? Actually length 3, so last index 2; from 0 jump 1 to index1, then jump2 to index3 out of bounds, but actually max jump from 1 is 2 so it reaches index3 (last is 2), so 2 jumps? Wait: index1 +2 =3, but last index is 2, so we stop at 2, so 1 jump? Let's check: from 0 jump 1 to index1, from index1 jump 2 covers index2 and 3, so we reach last index (2) in 2 jumps. But can we do 1 jump? From 0 max jump 1, so no. So answer 2. Test: {1,2,3} -> 2.
    assert(minimumJumpsToReachEnd({1, 2, 3}) == 2);

    // Long vector where greedy is optimal: [2,3,1,1,4] already tested; add [1,3,1,2,0,1] -> 0->1->3->5=3 jumps? Let's compute: 0 jump1 to1; from1 jump3 to3 or4; to reach5 (last index), from3 jump2 to5; so 3 jumps. Check.
    assert(minimumJumpsToReachEnd({1, 3, 1, 2, 0, 1}) == 3);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the minimum number of jumps to reach the last index.
// nums[i] is the maximum jump length from index i.
int minimumJumpsToReachEnd(const std::vector<int>& nums) {
    const int length = static_cast<int>(nums.size());
    if (length <= 1) {
        return 0;
    }

    int left = 0;
    int right = 0;
    int minJumps = 0;
    int maxReachable = 0;

    while (right < length - 1) {
        for (int i = left; i <= right; ++i) {
            maxReachable = std::max(maxReachable, i + nums[i]);
        }
        left = right + 1;
        right = maxReachable;
        ++minJumps;
    }
    return minJumps;
}
// The solution uses a greedy strategy based on a sliding window of indices reachable with the current number of jumps. We maintain three variables: `left` and `right` define the current window of indices that are reachable with exactly `minJumps` jumps, and `maxReachable` tracks the furthest index reachable from any index in this window. Initially, the window is just index 0 (`left=0, right=0`) with `minJumps=0`. In each iteration, we scan all indices from `left` to `right`, updating `maxReachable` as the maximum of `i + nums[i]`. After the scan, we set the new window to `[right+1, maxReachable]` and increment `minJumps`. This continues until `right` reaches or exceeds the last index. Since each index is visited at most once, the time complexity is O(n). Space usage is O(1) besides the input vector. Edge cases include a single element (where the answer is 0) and cases where the first element already reaches the end. The greedy approach is optimal because each window represents the set of indices reachable with the smallest possible number of jumps, and we always extend the boundary as far as possible in one jump.
