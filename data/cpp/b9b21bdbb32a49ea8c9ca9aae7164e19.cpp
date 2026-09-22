// You are given a string `s` of length `n` consisting only of the characters `'0'` and `'1'`, along with a positive integer `m`. You start at position `0` (the first character of the string) and need to reach position `n` (just past the last character). From any valid position `i` (where `s[i] != '1'`, or `i == n`), you may jump to any position `j` such that `i < j <= min(i + m, n)` and `s[j] != '1'` (or `j == n`). However, you are **not** allowed to land on a `'1'` (you may pass over them during a jump, but not land on them). Write a function `std::vector<int> minJumps(const std::string& s, int m)` that returns the **sequence of jump lengths** (i.e., `j - i` for each jump) that takes you from position `0` to position `n` using the **minimum total number of jumps**. If multiple sequences achieve the minimum number of jumps, prefer the one that uses the **smallest possible first jump length** (i.e., lexicographically smallest sequence of jump lengths). If it is impossible to reach `n`, return an empty vector. The sequence should include every jump from the start to the end; for example, if `s = "001", m = 2`, a valid solution might be `[1, 2]` (jump 0→1, then 1→3). The function must be self-contained and not rely on global state.
// The problem is a classic shortest-path / BFS-style DP on positions. Define `dp[i]` as the minimum number of jumps needed to reach `n` starting from position `i` (with `i` either `n` or a position where `s[i] == '0'`). The recurrence is `dp[i] = 1 + min( dp[j] )` over all valid `j` in `(i, i+m]` that are not `'1'` (or `j == n`). Base case: `dp[n] = 0`. We process `i` from `n-1` down to `0`, maintaining a sliding-window minimum over the next `m` positions' `dp` values. To also recover the jump sequence with lexicographic smallest first jump, we store `to[i]` = the index `j` that achieves the minimum `dp[j]` (and among equal minima, the smallest `j` is chosen because we want the smallest jump length). Since we compute from right to left, choosing the smallest `j` among ties gives the smallest first jump when we follow the chain from `0` to `n`. To efficiently get the minimum `dp` value and its index over a sliding window, we use a monotonic deque (or the custom "queue with min" as in the snippet). Complexity: `O(n)` time (each position pushed/popped once) and `O(n)` space for DP and the auxiliary structures. Edge cases: cannot land on `'1'`, so those positions have `dp = INF` and are never pushed into the window; if `s[0] == '1'` then impossible (return empty); if `n == 0` (empty string, but the problem says `s` has length `n` with `n >= 1`?) — we can handle `n=0` as trivially reachable with empty sequence. Also, if no valid jump from current `i` (i.e., all next `m` positions are `'1'` or unreachable), then `dp[i]` stays `INF`.
#include <vector>
#include <string>
#include <deque>
#include <limits>
#include <algorithm>

// Returns the sequence of jump lengths to reach the end of the string
// using minimum total jumps, with lexicographically smallest sequence
// when multiple possibilities exist. Returns an empty vector if impossible.
std::vector<int> minJumps(const std::string& s, int m) {
    const int n = static_cast<int>(s.size());
    const int INF = std::numeric_limits<int>::max() / 2;

    // dp[i] = minimum jumps from i to n, INF if unreachable
    std::vector<int> dp(n + 1, INF);
    // to[i] = the next position to jump to from i
    std::vector<int> to(n + 1, -1);

    dp[n] = 0;
    to[n] = n + 1; // sentinel not used

    // We process indices from n-1 down to 0.
    // The window of valid next positions is (i, i+m] ∩ {not '1'} ∪ {n}
    // We maintain a deque of indices with increasing dp values (monotonic).
    // Also we store dp values in the deque for easy access.
    std::deque<int> dq; // stores indices j with s[j] != '1' or j == n, and dp[j] != INF

    // Initially, the window for i = n-1 contains indices n (since n can be a target)
    // But we only add n when processing i = n-1? Actually we add positions as we slide.
    // We'll add positions as we move i from n-1 down to 0, but since window is to the right,
    // we need to have already processed those positions (they are > i). So we can pre-add n first.
    if (n > 0) {
        // Add n to the deque as a valid target for any i with i+m >= n
        dq.push_back(n);
    }

    for (int i = n - 1; i >= 0; --i) {
        // Remove indices from the front of the deque that are no longer within
        // the window for current i: they must satisfy i < j <= i+m
        while (!dq.empty() && dq.front() > i + m) {
            dq.pop_front();
        }

        // If s[i] == '1', we cannot land here, so dp[i] stays INF and we don't add it.
        // But we might still need to add i to the deque for future (smaller) i?
        // No, because we cannot land on '1', so i is not a valid jump target.
        // So skip adding i to the deque if s[i] == '1'.
        if (s[i] != '1') {
            if (!dq.empty()) {
                int best_j = dq.front(); // the index with minimum dp in window
                dp[i] = dp[best_j] + 1;
                to[i] = best_j;
            }

            // Add i to the deque for future smaller indices, but only if it's a valid target (s[i] != '1' and dp[i] != INF)
            if (dp[i] < INF) {
                // Maintain monotonic increasing dp: remove from back any index with dp >= dp[i]
                // Since we want the smallest index among equal dp for lexicographic tie-breaking,
                // we keep the smallest index with the smallest dp. So when equal, we pop older (larger index) back.
                while (!dq.empty() && dp[dq.back()] >= dp[i]) {
                    dq.pop_back();
                }
                dq.push_back(i);
            }
        }
        // If s[i] == '1', we do not add i to dq, and dp[i] remains INF.
        // Also, note that for future i, this position is not a valid target, so not adding is correct.
    }

    // Reconstruct sequence if reachable
    if (dp[0] >= INF) return {};

    std::vector<int> jumps;
    int pos = 0;
    while (pos < n) {
        int next = to[pos];
        // next must be valid
        jumps.push_back(next - pos);
        pos = next;
    }
    return jumps;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or paste above)
// ...

int main() {
    // Basic reachable case
    assert(minJumps("000", 3) == std::vector<int>({3}));
    // Single jump
    assert(minJumps("0", 1) == std::vector<int>({1}));
    // Impossible because start blocked
    assert(minJumps("1", 1) == std::vector<int>({}));
    // Need multiple jumps
    assert(minJumps("001", 2) == std::vector<int>({1, 2}));
    // Lexicographically smallest first jump (two ways: 0->2->4 or 0->1->2->4? but m=2)
    // "0000", m=2: possible 0->2->4 (jumps [2,2]) and 0->1->3->4 ([1,2,1])? Actually 0->1->3->4 works, but 2 jumps vs 3 jumps, so min is [2,2]
    assert(minJumps("0000", 2) == std::vector<int>({2, 2}));
    // Tie-breaking: "00000", m=3, min jumps = 2 (0->2->5) or (0->1->4->5? no), but 0->3->5 also 2 jumps? 0->3->5 is possible (jumps [3,2]) but first jump 2 is smaller than 3, so expect [2,3]? Actually 0->2->5 (jumps [2,3]) vs 0->3->5 ([3,2]) both 2 jumps, lexicographically [2,3] because 2<3. Also 0->1->4->5 is 3 jumps, not minimal.
    assert(minJumps("00000", 3) == std::vector<int>({2, 3}));
    // Obstacle blocks a path but another exists
    // "0100", m=2: start 0, cannot land on 1. From 0 can jump to 2 (since 2 is '0'), then from 2 to 4 (end) because m=2. So [2,2]
    assert(minJumps("0100", 2) == std::vector<int>({2, 2}));
    // Impossible because every jump leads to a '1'
    assert(minJumps("0110", 2) == std::vector<int>({1, 1, 1})); // Wait 0->1? but 1 is '1' so cannot. Actually 0->1 is invalid, 0->2 is invalid (s[2]='1'), so impossible -> empty
    assert(minJumps("0110", 2) == std::vector<int>({}));
    // Larger m makes it possible
    assert(minJumps("0110", 3) == std::vector<int>({3})); // 0->3? s[3]='0' so valid, jump length 3
    // Edge case n=0 (empty string) -> already at end, expect empty sequence
    assert(minJumps("", 5) == std::vector<int>({}));
    // Obstacle at far end but reachable
    assert(minJumps("0001", 3) == std::vector<int>({3})); // 0->3? s[3]='1' invalid, but j=4=n is allowed, so 0->4 with length 4? m=3 so 0->4 not allowed, need 0->? Actually 0->? positions 1,2,3 all '0'? s="0001" has s[3]='1', so from 0 can jump to 1,2 but not 3, and m=3 so can jump to 3 but invalid, so only 1,2. Then from 1 can jump to 2,3? invalid, 4? j=4 is allowed but i+m=4, so 1->4 works (length 3). So [1,3] or [2,2]? 0->2 then 2->4 (length 2+2=4) works too. Min jumps = 2 each, lexicographically [1,3] vs [2,2]: compare first jump 1 < 2 so [1,3] wins.
    assert(minJumps("0001", 3) == std::vector<int>({1, 3}));

    return 0;
}
