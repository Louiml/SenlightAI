Given an array of non-negative integers representing the maximum forward jump length from each position (where a value at index `i` means you can jump from `i` to any index in `[i+1, i+a[i]]`), write a C++ function `std::vector<int> minJumpLabels(const std::vector<int>& jumps)` that returns a vector of length `n`. The returned vector should contain, for each index from `0` to `n-1`, the minimum number of jumps required to reach that index starting from index `0` (with index `0` labeled as `0`). If an index is unreachable, label it as `-1`. The function must implement a greedy approach that processes indices in order, extending the current "reachable frontier" by exactly one jump at a time, and assigns labels to all newly reachable indices in each jump layer. If the last index (`n-1`) remains unreachable after processing, the function should still return the full label vector with `-1` for unreachable indices (the original snippet printed an error message, but our function must simply leave those as `-1`). The input array length is at least 1. The function must not use memoization or dynamic programming (i.e., not a DP solution); it must strictly simulate the layered BFS-like greedy expansion from the given code. Also, ensure that the return vector has exactly `n` elements.

#include <cassert>
#include <vector>
#include <iostream>

// forward declaration of the solution function
std::vector<int> minJumpLabels(const std::vector<int>& a);

int main() {
    // Basic example from typical greedy jump problem
    std::vector<int> a1 = {2,3,1,1,4};
    std::vector<int> r1 = minJumpLabels(a1);
    // Expected: index 0:0, 1:1, 2:1, 3:2, 4:2 (0->1->3->4 or 0->2->4)
    assert(r1 == std::vector<int>({0,1,1,2,2}));

    // All zeros except first: only index 0 reachable
    std::vector<int> a2 = {0,0,0,0};
    std::vector<int> r2 = minJumpLabels(a2);
    assert(r2 == std::vector<int>({0,-1,-1,-1}));

    // Single element
    std::vector<int> a3 = {5};
    std::vector<int> r3 = minJumpLabels(a3);
    assert(r3 == std::vector<int>({0}));

    // Large jumps skip many indices
    std::vector<int> a4 = {3,0,0,0,0};
    std::vector<int> r4 = minJumpLabels(a4);
    // 0->1->? but a[1]=0, so index 1 reachable with 1 jump, others from 0 with 1 jump too (0,1,2,3 reachable)
    // Actually indices 1,2,3 reachable with 1 jump, index 4 not reachable (a[3]=0)
    assert(r4 == std::vector<int>({0,1,1,1,-1}));

    // Another case: a[0]=1, a[1]=2, a[2]=0, a[3]=1
    std::vector<int> a5 = {1,2,0,1};
    std::vector<int> r5 = minJumpLabels(a5);
    // 0:0, 1:1, 2:2 (0->1->2? but a[1]=2 so 1->2 yes), 3:2 (0->1->3)
    assert(r5 == std::vector<int>({0,1,2,2}));

    // Unreachable in the middle but later reachable? Not possible because if can't reach middle, can't reach later.
    std::vector<int> a6 = {2,0,0,1,1};
    // Index 0:0, 1:1, 2:1, 3 unreachable? a[2]=0 so cannot go further, index 3,4 unreachable
    std::vector<int> r6 = minJumpLabels(a6);
    assert(r6 == std::vector<int>({0,1,1,-1,-1}));

    // Edge: n=2 and a[0]=1
    std::vector<int> a7 = {1,0};
    std::vector<int> r7 = minJumpLabels(a7);
    assert(r7 == std::vector<int>({0,1}));

    // Edge: n=2 and a[0]=0
    std::vector<int> a8 = {0,5};
    std::vector<int> r8 = minJumpLabels(a8);
    assert(r8 == std::vector<int>({0,-1}));

    std::cout << "All tests passed.\n";
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the minimum number of jumps to reach each index from index 0.
// Unreachable indices are labeled -1. Uses a greedy layered approach.
std::vector<int> minJumpLabels(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> dp(n, -1);
    if (n == 0) return dp;
    dp[0] = 0;
    if (n == 1) return dp;

    int maxi = 0;
    int ed = a[0];           // farthest index reachable with current jump count
    int jmp = 1;             // current jump layer
    int i = 1;               // start of the next unprocessed index
    int j;
    bool done = false;

    while (i < n) {
        maxi = 0;
        int window_end = ed + i - 1;
        // Process the entire window of indices reachable with exactly 'jmp' jumps
        for (j = i; j <= window_end; ++j) {
            if (j >= n) {
                done = true;
                break;
            }
            dp[j] = jmp;
            // How far this index can extend beyond the current window end
            int extension = a[j] - (window_end - j);
            maxi = std::max(maxi, extension);
        }
        // If no extension possible and last not reached, stop (unreachable beyond)
        if (maxi == 0 && dp[n-1] == -1) {
            break;
        }
        if (done) break;

        i = j;               // Next start index is one past the window
        jmp++;
        ed = maxi;           // New maximum reach for the next layer
    }

    // Ensure the last index is correctly set (if reachable it was assigned in loop)
    return dp;
}

// The core algorithm mimics a BFS in terms of "jump layers". We start at index `0` with label `0`. We maintain a current reachable end (`ed`) which initially equals `a[0]` (the farthest index reachable with 1 jump), and a current jump count `jmp=1`. We also keep a pointer `i` that indicates the start of the next unprocessed index for the next layer. The loop runs while `i < n`. In each iteration, we consider all indices from `i` to `ed + i - 1` (this is the range that becomes reachable with exactly `jmp` jumps, given that `i` was the first index that required `jmp-1` jumps, and `ed` was the maximum reach from that layer). For each such index `j` (if `j < n`), we assign `dp[j] = jmp`. We then compute `ted = a[j] - (ed + i - 1 - j)`. The term `(ed + i - 1 - j)` is the number of steps remaining in the current layer's window after index `j`; subtracting this from `a[j]` gives how much farther index `j` can reach beyond the current window's end. `maxi` tracks the maximum of this value across all indices in the window. Then we set `i` to the next index after the window (`i = j` after the loop, so `i` becomes `ed + i`), increment `jmp`, and set `ed = maxi`. If at any point `maxi == 0` and the last index hasn't been reached (i.e., `dp[n-1] == 0`), we conclude unreachable and break (the original printed an error; our function will leave the remaining labels as `-1`). The loop also stops early if the window reaches beyond `n` (flag `f`), meaning the last index has been covered. Note: the original code uses `dp[n-1]` check with `==0` because it never assigns `dp[0]` (which is 0 by memset) and unreachable labels remain 0. In our function, we must start with `dp[0]=0` and leave unreachable as `-1` after the process. Edge cases: `n=1` returns `{0}`. If `a[0]==0` and `n>1`, then no index is reachable except index 0, so all others are `-1`. Large `a[i]` values may make the window exceed `n`, so we clamp `j < n`. Time complexity: each index is visited exactly once (when it gets its label), so O(n). Space: O(n) for the output vector (no extra large structures).
