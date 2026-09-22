// Given three integers `x`, `y`, and `k`, write a C++ function `minimumSteps` that returns the minimum number of steps required to reach exactly `y` from `x` using two allowed operations: (1) increase the current value by 1, or (2) jump directly to any value `z` such that `|z - current| <= k` (i.e., you may instantly move to any integer within distance `k` from your current position, and this jump counts as one step). You may use these operations in any order, any number of times. The function should compute the minimal number of steps, assuming `x`, `y`, and `k` are non-negative and `k >= 0`. For example, if `x=1`, `y=5`, `k=2`, the minimum steps is 3 (jump 1→3, then 3→5) — note that a jump of exactly `k` is allowed. If `y < x`, you may still only move upward (you cannot decrease), so it is impossible; return `-1` in that case.
#include <cassert>

int main() {
    assert(minimumSteps(1, 5, 2) == 2); // 1->3 (jump 2), 3->5 (jump 2)
    assert(minimumSteps(0, 10, 3) == 4); // ceil(10/3)=4
    assert(minimumSteps(5, 5, 10) == 0);
    assert(minimumSteps(5, 4, 1) == -1);
    assert(minimumSteps(0, 5, 0) == 5); // only +1 steps
    assert(minimumSteps(10, 20, 1) == 10); // 10 jumps of size 1
    assert(minimumSteps(3, 3, 0) == 0);
    assert(minimumSteps(2, 4, 10) == 1); // can jump directly
    assert(minimumSteps(0, 1000000000, 1000000000) == 1);
    assert(minimumSteps(0, 1, 1) == 1);
}
#include <algorithm>

// Returns the minimum number of steps to go from x to y, where each step either
// increases by 1 or jumps to any integer within distance k (inclusive). Returns -1 if impossible.
int minimumSteps(int x, int y, int k) {
    if (y < x) return -1;
    if (x == y) return 0;
    if (k == 0) return y - x; // only +1 steps allowed
    int diff = y - x;
    // Each jump covers at most k distance. Need ceil(diff / k) steps.
    return (diff + k - 1) / k;
}
// The problem reduces to a simple greedy strategy. Notice that if `y <= x + k`, you can reach `y` in exactly one jump (since `y - x <= k`), so the answer is 1 (or 0 if `x == y`). More generally, the optimal approach is to perform as many maximal jumps of size exactly `k` as possible, then finish with either a smaller jump or a +1 step. Because each jump covers at most `k` distance, the minimum number of steps is `ceil((y - x) / k)`, but with a special case: if `x == y`, the answer is 0 (no steps needed). For `y > x`, the number of jumps needed is `ceil(diff / k)` where `diff = y - x`, which can be computed as `(diff + k - 1) / k`. However, the original code snippet (which handled similar logic) also added an extra `diff - k` when `diff > k`, effectively resulting in `ceil(diff/k)` for positive `k`. For `k=0`, jumps are impossible, so only +1 steps work; the answer becomes `diff` (if `y > x`). Also, if `y < x`, return `-1` since movement is strictly upward. Edge cases: `x == y` → 0, `diff <= k` and `diff > 0` → 1, `k == 0` → `diff`. Time complexity: O(1). Space complexity: O(1).
