Write a C++ function `count_valid_assignments` that takes two integers `n` and `x`, followed by a vector of `n` positive integers representing employee skill levels, and returns the number of ways to partition all employees into non-empty groups such that the total "imbalance" of the partition is at most `x`. Here, the imbalance of a partition is defined as the sum over all groups of (maximum skill in group minus minimum skill in group). Each employee must belong to exactly one group, and groups are unlabeled (order of groups doesn't matter). The answer should be returned modulo 1,000,000,007. All skill levels are between 1 and 10,000, `n` ≤ 50, and `x` ≤ 5000.
The key observation is that if we sort the employees by skill level, the imbalance contribution of a group depends only on the difference between its maximum and minimum skill. This suggests a dynamic programming approach that processes employees in sorted order. We maintain a DP state `dp[balance][open_groups]`, where `balance` is the current accumulated imbalance (offset by a constant to handle negative indices), and `open_groups` is the number of groups currently "open" — that is, groups we have started but not yet finished. Initially, `dp[0][0] = 1`. When processing the `i`-th employee (in sorted order), we consider four transitions:
1. Start a new group: open_groups increases by 1, and the balance decreases by `a[i]` (because this will be the minimum of that group, so the eventual imbalance will include `+max - a[i]`).
2. Close an existing group: open_groups decreases by 1, and balance increases by `a[i]` (since this employee becomes the maximum of that group). There are `open_groups` ways to choose which group to close, so multiply by `open_groups`.
3. Add this employee as a "singleton" group that is simultaneously opened and closed: open_groups unchanged, balance unchanged (since max - min = 0 for a singleton). This contributes exactly `dp[balance][open_groups]`.
4. Add this employee to an existing open group (not as the maximum, since groups are processed in sorted order, but as a new member that doesn't change the min or max yet): open_groups unchanged, balance unchanged. There are `open_groups` ways to choose which group, so multiply by `open_groups`.

At the end, after processing all employees, the answer is the sum of `dp[balance][0]` for all `balance` from 0 to `x` (inclusive). Important edge cases: `n` can be up to 50, so the number of open groups never exceeds `n`, and the balance can range from -5000 to 5000, so we offset by 5000. Also note that when starting a new group, we ensure `k+1 ≤ n` and `balance - a[i] ≥ -5000`; when closing, ensure `k ≥ 1` and `balance + a[i] ≤ 5000`. The DP uses a rolling array to save memory, but since the state space is small (10001 × 51), we can just use two 2D arrays. Time complexity is O(n * balance_range * max_groups) = O(50 * 10001 * 51) ≈ 25.5 million operations, which is efficient. Space complexity is O(10001 * 51) ≈ 510,000 integers.
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int OFFSET = 5000;

int count_valid_assignments(int n, int x, const vector<int>& skills) {
    vector<int> a = skills;
    sort(a.begin(), a.end());
    
    // dp[balance][open_groups] where balance is stored with offset
    vector<vector<int>> dp(2 * OFFSET + 1, vector<int>(n + 1, 0));
    vector<vector<int>> nxt(2 * OFFSET + 1, vector<int>(n + 1, 0));
    
    dp[OFFSET][0] = 1; // zero balance, zero open groups
    
    for (int i = 0; i < n; ++i) {
        // Reset next table
        for (auto& row : nxt) fill(row.begin(), row.end(), 0);
        
        for (int bal = -OFFSET; bal <= OFFSET; ++bal) {
            for (int k = 0; k <= n; ++k) {
                int cur = dp[bal + OFFSET][k];
                if (cur == 0) continue;
                
                // 1. Start a new group (this employee is the minimum)
                if (k < n && bal - a[i] >= -OFFSET) {
                    int& target = nxt[bal - a[i] + OFFSET][k + 1];
                    target = (target + cur) % MOD;
                }
                // 2. Close an existing group (this employee is the maximum)
                if (k > 0 && bal + a[i] <= OFFSET) {
                    int& target = nxt[bal + a[i] + OFFSET][k - 1];
                    target = (target + (long long)cur * k) % MOD;
                }
                // 3. Singleton group (open and close immediately)
                {
                    int& target = nxt[bal + OFFSET][k];
                    target = (target + cur) % MOD;
                }
                // 4. Attach to an existing open group (not changing min/max)
                if (k > 0) {
                    int& target = nxt[bal + OFFSET][k];
                    target = (target + (long long)cur * k) % MOD;
                }
            }
        }
        swap(dp, nxt);
    }
    
    int ans = 0;
    for (int bal = 0; bal <= x; ++bal) {
        ans = (ans + dp[bal + OFFSET][0]) % MOD;
    }
    return ans;
}
#include <bits/stdc++.h>
using namespace std;

// Declare function (copied from solution for test)
const int MOD = 1000000007;
const int OFFSET = 5000;

int count_valid_assignments(int n, int x, const vector<int>& skills) {
    vector<int> a = skills;
    sort(a.begin(), a.end());
    vector<vector<int>> dp(2 * OFFSET + 1, vector<int>(n + 1, 0));
    vector<vector<int>> nxt(2 * OFFSET + 1, vector<int>(n + 1, 0));
    dp[OFFSET][0] = 1;
    for (int i = 0; i < n; ++i) {
        for (auto& row : nxt) fill(row.begin(), row.end(), 0);
        for (int bal = -OFFSET; bal <= OFFSET; ++bal) {
            for (int k = 0; k <= n; ++k) {
                int cur = dp[bal + OFFSET][k];
                if (cur == 0) continue;
                if (k < n && bal - a[i] >= -OFFSET) {
                    int& target = nxt[bal - a[i] + OFFSET][k + 1];
                    target = (target + cur) % MOD;
                }
                if (k > 0 && bal + a[i] <= OFFSET) {
                    int& target = nxt[bal + a[i] + OFFSET][k - 1];
                    target = (target + (long long)cur * k) % MOD;
                }
                {
                    int& target = nxt[bal + OFFSET][k];
                    target = (target + cur) % MOD;
                }
                if (k > 0) {
                    int& target = nxt[bal + OFFSET][k];
                    target = (target + (long long)cur * k) % MOD;
                }
            }
        }
        swap(dp, nxt);
    }
    int ans = 0;
    for (int bal = 0; bal <= x; ++bal) {
        ans = (ans + dp[bal + OFFSET][0]) % MOD;
    }
    return ans;
}

int main() {
    // Test 1: n=1, x=0, skill=5 → only singleton, imbalance 0 ≤ 0 → 1
    assert(count_valid_assignments(1, 0, {5}) == 1);
    
    // Test 2: n=2, skills={1,2}, x=0 → only two singletons (imbalance 0) → 1; grouping together gives imbalance 1 >0
    assert(count_valid_assignments(2, 0, {1,2}) == 1);
    
    // Test 3: n=2, skills={1,2}, x=1 → singletons (0) + pair (1) → 2
    assert(count_valid_assignments(2, 1, {1,2}) == 2);
    
    // Test 4: n=3, all equal, x=0 → all singletons = 1 way only
    assert(count_valid_assignments(3, 0, {7,7,7}) == 1);
    
    // Test 5: n=3, skills={1,2,3}, x=2 → brute force: all partitions:
    // {1},{2},{3} (imb=0), {1,2},{3} (imb=1), {1,3},{2} (imb=2), {1},{2,3} (imb=1), {1,2,3} (imb=2) → total 5
    assert(count_valid_assignments(3, 2, {1,2,3}) == 5);
    
    // Test 6: n=3, skills={1,2,3}, x=1 → only those with imb ≤1: 3 ways
    assert(count_valid_assignments(3, 1, {1,2,3}) == 3);
    
    // Test 7: n=4, all equal 10, x=0 → only all singletons → 1
    assert(count_valid_assignments(4, 0, {10,10,10,10}) == 1);
    
    // Test 8: n=4, skills={1,1,2,2}, x=0 → only singletons → 1
    assert(count_valid_assignments(4, 0, {1,1,2,2}) == 1);
    
    // Test 9: n=4, skills={1,2,3,4}, x=3 → brute force check gives known value: 13
    assert(count_valid_assignments(4, 3, {1,2,3,4}) == 13);
    
    // Test 10: n=2, skills={10000,1}, x=9999 → pair imbalance=9999 ≤9999, plus singletons 0 → total 2
    assert(count_valid_assignments(2, 9999, {10000,1}) == 2);
    
    cout << "All tests passed!\n";
    return 0;
}
