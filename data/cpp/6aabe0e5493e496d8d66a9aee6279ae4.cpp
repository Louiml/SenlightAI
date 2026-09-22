// Write a C++ function named `minimumJumps` that takes a non-empty vector of non-negative integers where each element represents the maximum forward jump length from that index, and returns the minimum number of jumps required to reach the last index (index `n-1`). It is guaranteed that the last index is always reachable. If the vector has only one element, the function must return 0 because we are already at the destination. The function should handle large inputs efficiently and must not use recursion or dynamic programming tables (the intended solution is a greedy BFS-like sweep). The function must be `const`-correct, meaning the input vector is passed as `const std::vector<int>&` and never modified. The function should be placed in a standalone header-free implementation with necessary includes (`<vector>`, `<algorithm>`, `<cstddef>`).
#include <cassert>
#include <vector>

int main() {
    // Already at the end
    assert(minimumJumps({0}) == 0);

    // First jump reaches the end
    assert(minimumJumps({5, 1, 1, 1, 1}) == 1);

    // Simple case
    assert(minimumJumps({2, 3, 1, 1, 4}) == 2);

    // Greedy required: cannot stay in current range too long
    assert(minimumJumps({1, 3, 2, 4, 1, 1, 1}) == 3);

    // All zeros except the last? Actually last is reachable via guarantee, but test typical
    assert(minimumJumps({2, 1, 1, 1, 1}) == 3);

    // Large leaps
    assert(minimumJumps({1, 1, 1, 1, 1}) == 4);

    // Jump of zero in middle, still must go around
    assert(minimumJumps({3, 0, 0, 1, 1}) == 2);

    // Two elements
    assert(minimumJumps({3, 0}) == 1);

    // Random case from LeetCode
    assert(minimumJumps({4, 1, 1, 1, 1, 3, 1, 1, 1}) == 2);

    return 0;
}
#include <vector>
#include <algorithm>

// Returns the minimum number of jumps to reach the last index.
// The input is a non-empty vector of non-negative jump lengths.
// It is guaranteed that the last index is reachable.
int minimumJumps(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 1) {
        return 0;
    }

    int currentEnd = nums[0];      // Farthest index reachable with current jump count
    int farthest = nums[0];        // Farthest index reachable from any index in current range
    int jumps = 1;                 // We already take the first jump from index 0

    // Traverse indices within the current range; expand until we reach the end
    for (int i = 1; i < n && currentEnd < n - 1; ++i) {
        farthest = std::max(farthest, i + nums[i]);

        // When we finish scanning the current range, take another jump
        if (i == currentEnd) {
            ++jumps;
            currentEnd = farthest;
        }
    }

    return jumps;
}
// The problem is the classic “Jump Game II”. The greedy approach works by simulating a breadth-first search over “jump ranges”. We maintain three variables: `currentEnd` (the farthest index reachable with the current number of jumps), `farthest` (the farthest index reachable from any index within the current range), and `jumps` (the number of jumps used so far). Initially, if the array size is 1, return 0. Otherwise, set `currentEnd = farthest = nums[0]` (from index 0) and `jumps = 1`. Then iterate `i` from 1 to `n-1` (or as long as `currentEnd < n-1`). For each index `i` that is within the current range (`i <= currentEnd`), update `farthest = max(farthest, i + nums[i])`. When `i` reaches `currentEnd` (the end of the current jump’s reachable range), that means we must take another jump: increment `jumps`, and set `currentEnd = farthest`. The loop ends when `currentEnd >= n-1`. This works because we always expand the reachable frontier maximally. Edge cases: single-element array returns 0; a jump length of zero is allowed but reachability is guaranteed; if the first element already reaches the end, then `currentEnd >= n-1` initially so the loop never runs and jumps remains 1 (correct). Time complexity is O(n) because each index is visited at most once. Space complexity is O(1) auxiliary, excluding the input.
