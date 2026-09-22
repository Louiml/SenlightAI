Given a sequence of `n` cells numbered 1 through `n` in a line, and an array `a` of length `n-1` where `a[i]` represents the number of steps allowed from cell `i+1` (i.e., from cell `i+1` you can jump exactly `a[i]` steps forward to cell `i+1 + a[i]`), write a standalone C++ function named `canReachTarget` that takes three parameters: the integer `n`, the integer `m` (the target cell you want to reach), and a `const std::vector<int>&` containing exactly `n-1` allowed step sizes. Starting from cell 1, you may repeatedly jump from your current cell `i` to cell `i + a[i-1]` (for every `i < n`), but you cannot jump beyond `n`. The function should return `true` if you can exactly land on cell `m` at some point, or `false` otherwise. Note that if you jump past `n`, the path is invalid and you stop (you cannot reach `m`). Also, the input vector size will always be `n-1` and each step size is a positive integer, with `1 ≤ m ≤ n`. Edge case: if `m == 1`, the function should immediately return `true` because you start at cell 1.

#include <cassert>
#include <vector>

// Declaration of the function (assume it's provided above)
bool canReachTarget(int n, int m, const std::vector<int>& a);

int main() {
    // Test 1: n=5, a=[2,3,1,2], start at 1 -> 1+2=3 -> 3+1=4, target m=4? yes
    assert(canReachTarget(5, 4, {2,3,1,2}) == true);
    
    // Test 2: n=5, a=[2,3,1,2], target m=5 -> path 1->3->4->? 4+2=6 >5, cannot reach 5
    assert(canReachTarget(5, 5, {2,3,1,2}) == false);
    
    // Test 3: n=3, a=[1,1], target m=1 -> immediately at start
    assert(canReachTarget(3, 1, {1,1}) == true);
    
    // Test 4: n=4, a=[2,2,2], target m=4 -> 1->3->? 3+2=5>4, cannot reach 4
    assert(canReachTarget(4, 4, {2,2,2}) == false);
    
    // Test 5: n=4, a=[1,1,1], target m=3 -> 1->2->3, yes
    assert(canReachTarget(4, 3, {1,1,1}) == true);
    
    // Test 6: n=1, a=[], target m=1 -> start is target
    assert(canReachTarget(1, 1, {}) == true);
    
    // Test 7: n=6, a=[2,2,1,3,1], target m=6 -> 1->3->? 3+1=4 ->4+3=7>6, cannot reach 6
    assert(canReachTarget(6, 6, {2,2,1,3,1}) == false);
    
    // Test 8: n=6, a=[2,2,1,3,1], target m=5 -> 1->3->4->? 4+3=7>6, no
    assert(canReachTarget(6, 5, {2,2,1,3,1}) == false);
    
    // Test 9: n=6, a=[2,1,1,1,1], target m=4 -> 1->3->4, yes
    assert(canReachTarget(6, 4, {2,1,1,1,1}) == true);
    
    // Test 10: n=5, a=[3,1,1,1], target m=5 -> 1->4->5, yes
    assert(canReachTarget(5, 5, {3,1,1,1}) == true);
    
    return 0;
}

#include <vector>

// Returns true if starting from cell 1 and following the jump rules
// (from cell i, jump to i + a[i-1]) we can exactly land on cell m.
bool canReachTarget(int n, int m, const std::vector<int>& a) {
    // Start at cell 1
    int current = 1;
    
    // If target is start, already there
    if (current == m) return true;
    
    // Track visited cells to be safe (though jumps strictly increase, so no cycles)
    std::vector<bool> visited(n + 1, false);
    visited[current] = true;
    
    // Follow the path
    while (current <= n) {
        // Compute next cell (1-indexed)
        int next = current + a[current - 1]; // a[current-1] because input is 0-indexed
        
        // If next exceeds n, we cannot reach m (unless m == current, already checked)
        if (next > n) break;
        
        current = next;
        
        // If we've visited this cell before, break to avoid infinite loop (shouldn't happen)
        if (visited[current]) break;
        visited[current] = true;
        
        // Check if we've reached the target
        if (current == m) return true;
    }
    
    // If we exited loop without reaching m, return false
    return false;
}

// The problem simulates a deterministic jump path. Starting from cell 1, we compute the next cell by adding the step size stored at index `current-1` of the array. We continue updating the current cell as long as the current cell is within bounds (1 to n) and we haven't already visited that cell (to avoid infinite loops, though in practice the jumps always increase, so no cycles exist). We track visited cells to be safe. If at any point the current cell equals `m`, we return `true`. If the current cell becomes either `0` or greater than `n` (i.e., we jump beyond the board), we stop and return `false` (unless we already reached `m`). The key edge cases: `m == 1` returns true immediately; if the first jump overshoots `n` and `m` is not 1, it's false; if the path never lands exactly on `m`, it's false. Since jumps strictly increase the index (because `a[i]` is positive), the loop runs at most `n` times, giving O(n) time. Space is O(n) for the visited array, but we can also avoid it by noting the strictly increasing nature (the index increases each time, so no revisited cells), making space O(1) if we just check bounds. However, we'll include a visited vector for clarity. Time complexity is O(n) in the worst case, and auxiliary space is O(n) if using visited, or O(1) if we rely on monotonicity.
