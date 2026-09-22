// Write a C++ function `long long frogJumpCount(const std::vector<int>& stones, int minJump, int maxJump)` that, given a sequence of stone positions sorted in strictly increasing order, returns the minimum number of jumps needed to reach the last stone starting from the first stone (index 0). A jump from stone `i` to stone `j` (where `j > i`) is allowed only if the distance `stones[j] - stones[i]` is between `minJump` and `maxJump` inclusive. If it is impossible to reach the last stone, return `-1`. The function should use dynamic programming with memoization (top-down) to avoid exponential runtime.

// We model the problem as a directed acyclic graph where each stone is a node and a directed edge exists from stone `i` to stone `j` if `minJump <= stones[j] - stones[i] <= maxJump`. We need the shortest path (in terms of number of edges) from node 0 to node `n-1`. Use recursive memoization: define `dp(i)` as the minimum jumps from stone `i` to the last stone. Base case: if `i == n-1`, return `0`. Otherwise, initialize the result to infinity (`1e9`). For every `j` from `i+1` to `n-1`, check the distance; if valid, compute `1 + dp(j)` and take the minimum. Store the computed result in a memo array (size `n`) initialized to `-1` to distinguish uncomputed values. After computing, if the result remains infinity, set it to a large sentinel (or store `-1` explicitly) and return -1 at the top level. Edge cases: input size 1 (already at last stone → 0), impossible jumps (return -1), very large jump distances that exceed `maxJump`. Time complexity: O(n^2) because for each of the n states we iterate over up to n possible next stones. Space complexity: O(n) for the memo array plus O(n) recursion stack depth.

#include <vector>
#include <algorithm>
#include <climits>

// Returns minimum number of jumps from first to last stone, or -1 if impossible.
// stones: sorted strictly increasing positions.
// minJump, maxJump: inclusive bounds for a valid jump distance.
long long frogJumpCount(const std::vector<int>& stones, int minJump, int maxJump) {
    int n = static_cast<int>(stones.size());
    if (n == 1) return 0;

    // memo[i] = -1 means not computed yet.
    // For reachable states, memo[i] will be the min jumps from i to last.
    std::vector<long long> memo(n, -1);

    // Recursive helper function (lambda or separate function).
    // Use a named lambda with self-reference via std::function for clarity.
    std::function<long long(int)> solve = [&](int i) -> long long {
        if (i == n - 1) return 0;
        if (memo[i] != -1) return memo[i];

        long long best = LLONG_MAX;
        for (int j = i + 1; j < n; ++j) {
            int dist = stones[j] - stones[i];
            if (dist < minJump || dist > maxJump) continue;
            long long sub = solve(j);
            if (sub != LLONG_MAX) {
                best = std::min(best, 1 + sub);
            }
        }
        // if no reachable next stone, best remains LLONG_MAX
        memo[i] = (best == LLONG_MAX) ? LLONG_MAX : best;
        return memo[i];
    };

    long long result = solve(0);
    return (result == LLONG_MAX) ? -1 : result;
}

#include <cassert>
#include <vector>
#include <functional>

// assume frogJumpCount is defined as above

int main() {
    // Basic case from original snippet: stones {0,3,4,6,10}, a=3, b=4
    {
        std::vector<int> stones = {0,3,4,6,10};
        int minJump = 3, maxJump = 4;
        assert(frogJumpCount(stones, minJump, maxJump) == 2);
        // Possible path: 0 -> 4 (dist 4), 4 -> 10 (dist 6? no, 6>4) so 0->6 (dist 6? no) 
        // Let's check valid: 0->3(dist3), 3->6(dist3), 6->10(dist4) => 3 jumps, but better 0->4(dist4), 4->6(dist2? no) 
        // Actually correct minimum is 2: 0->4 (dist4), 4->10(dist6>4 invalid) so not 2; 0->3,3->6,6->10 = 3; 0->4,4->6 invalid; 
        // after verification, the only 2-step is 0->3(dist3), 3->10(dist7>4) invalid. So answer is 3. Let's adjust assert to 3.
    }
    assert(frogJumpCount({0,3,4,6,10}, 3, 4) == 3);

    // Impossible case
    assert(frogJumpCount({0,1,100}, 10, 20) == -1);

    // Single stone
    assert(frogJumpCount({5}, 1, 10) == 0);

    // Two stones with valid jump
    assert(frogJumpCount({0,5}, 5, 5) == 1);
    // Two stones with invalid jump
    assert(frogJumpCount({0,5}, 4, 4) == -1);

    // All jumps are length 2, but maxJump 2 => chain
    assert(frogJumpCount({0,2,4,6,8}, 2, 2) == 4);

    // Multiple paths choose shortest: {0,2,4,6,8,10}, min=2 max=4
    // 0->2->4->6->8->10 (5 jumps) but 0->4 (dist4), 4->6? 2, 6->8?2,8->10?2 => 4 jumps, actually 0->4->8->10 (3 jumps) 0->4 (4),4->8(4),8->10(2) => 3 jumps
    assert(frogJumpCount({0,2,4,6,8,10}, 2, 4) == 3);

    // Larger gap impossible for some stones
    assert(frogJumpCount({0,2,100}, 2, 2) == -1);

    return 0;
}
