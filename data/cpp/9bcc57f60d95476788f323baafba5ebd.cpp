/*
Write a C++ function `std::string canReach(int n, int t, const std::vector<int>& jumps)` that solves the following problem: There are `n` cells numbered from 1 to `n`. You start at cell 1. For each cell `i` from 1 to `n-1`, you are given a positive integer `jumps[i]` (1-indexed: the jump value for cell `i` is stored at index `i-1` in the vector). From cell `i`, you can move exactly `jumps[i]` steps forward to cell `i + jumps[i]`. You cannot move backward or remain in place. Determine if you can reach cell `t` exactly following these moves. If you reach `t`, return `"YES"`; otherwise, return `"NO"`. It is guaranteed that all jumps are within the board (i.e., from any cell, the next cell is at most `n`), and `t` is between 1 and `n`. The function should not modify the input vector.
*/
#include <string>
#include <vector>

// Determine if starting at cell 1 and following the jump schedule we can reach cell t.
// jumps[i] is the forward step from cell (i+1). Returns "YES" or "NO".
std::string canReach(int n, int t, const std::vector<int>& jumps) {
    if (t == 1) {
        return "YES";
    }
    int current = 1;
    while (current <= n) {
        if (current == t) {
            return "YES";
        }
        // current is between 1 and n-1 because if current == n we either already returned
        // or we are at the last cell and cannot move further (jumps[n-1] is not defined, but
        // the input guarantees current < n before moving). So safe to access jumps[current-1].
        current += jumps[current - 1];
    }
    return "NO";
}
#include <cassert>
#include <vector>
#include <string>

// The solution function declaration (included above or here)
std::string canReach(int n, int t, const std::vector<int>& jumps);

int main() {
    // Basic reachable path
    assert(canReach(5, 5, {2, 2, 2, 2}) == "YES"); // 1->3->5
    // Unreachable target
    assert(canReach(5, 4, {2, 2, 2, 2}) == "NO"); // 1->3->5 skip 4
    // Target is start
    assert(canReach(3, 1, {1, 1}) == "YES");
    // Single step
    assert(canReach(2, 2, {1}) == "YES");
    // Cannot move from last cell, target not reached
    assert(canReach(3, 3, {1, 2}) == "NO"); // 1->2->4 (exceeds n) loop stops
    // Large jump directly to target
    assert(canReach(10, 9, {8, 1, 1, 1, 1, 1, 1, 1, 1}) == "YES"); // 1->9
    // Jump over target from start
    assert(canReach(10, 2, {3, 1, 1, 1, 1, 1, 1, 1, 1}) == "NO"); // 1->4...
    // Many steps with exact landing
    assert(canReach(7, 7, {1, 1, 1, 1, 1, 1}) == "YES"); // 1->2->...->7
    // Complexity: n=1, only cell is the start and target
    assert(canReach(1, 1, {}) == "YES");
    // Target at end, but jumps skip over
    assert(canReach(6, 6, {2, 2, 2, 2, 2}) == "NO"); // 1->3->5->7 exceeds n
    return 0;
}
// The problem is a deterministic path traversal. Starting at `current = 1`, we repeatedly move to `current + jumps[current-1]` (since jumps are 1-indexed in the problem description, but vector is 0-indexed). If at any step `current == t`, we return `"YES"`. If `current` exceeds `n` or becomes stuck (though the guarantee prevents exceeding `n`, but we still handle it for safety), we stop and return `"NO"`. Because each move increases `current` by a positive amount, we never revisit a cell, so the loop runs at most `n` steps. Edge cases: if `t` is 1 (start cell), return `"YES"` immediately. If `t` is unreachable because jumps skip over it, we return `"NO"`. Time complexity is O(n) in the worst case (since we visit each cell at most once), and space complexity is O(1) auxiliary (we only use a few variables). No need for visited set because moves are strictly increasing.
