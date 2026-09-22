/*
You are given an array `a` of length `n` (1-indexed internally) that is initially all zeros. There are `q` range update operations, each specified by two integers `x` and `y` (1-indexed), meaning that for every index `i` in `[x, y]`, the value `a[i]` is incremented by 1. After all updates, you need to partition the array into contiguous groups such that within each group, all elements have the same value, and consecutive groups have distinct values. Let `t` be the number of such groups. Compute the value `2^t mod 998244353`. Write a C++ function `int countWays(int n, vector<pair<int,int>>& updates)` that takes the size `n` and a list of update pairs, and returns the result modulo `998244353`. The function must handle `n` up to `2e5` and `q` up to `2e5`, with each `x, y` satisfying `1 ≤ x ≤ y ≤ n`. Note that groups of zero-valued elements are ignored — only consecutive runs where the value is non-zero are counted. However, if the entire array is zeros after updates, then `t = 0` and the answer is `1`.
*/

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;

// Fast modular exponentiation: compute (base^exp) % mod
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Count groups and return 2^t mod MOD
int countWays(int n, const vector<pair<int, int>>& updates) {
    vector<int> diff(n + 1, 0);
    for (const auto& upd : updates) {
        int x = upd.first - 1; // convert to 0-indexed
        int y = upd.second - 1;
        diff[x]++;
        diff[y + 1]--;
    }

    // Build final array via prefix sum
    vector<int> a(n, 0);
    int cur = 0;
    for (int i = 0; i < n; ++i) {
        cur += diff[i];
        a[i] = cur;
    }

    // Count contiguous groups of identical non-zero values
    int t = 0;
    int i = 0;
    while (i < n) {
        if (a[i] == 0) {
            ++i;
            continue;
        }
        ++t;
        int value = a[i];
        while (i < n && a[i] == value) {
            ++i;
        }
    }

    return static_cast<int>(modPow(2, t, MOD));
}

#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (or copy the code above)
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int countWays(int n, const vector<pair<int, int>>& updates) {
    vector<int> diff(n + 1, 0);
    for (const auto& upd : updates) {
        int x = upd.first - 1;
        int y = upd.second - 1;
        diff[x]++;
        diff[y + 1]--;
    }
    vector<int> a(n, 0);
    int cur = 0;
    for (int i = 0; i < n; ++i) {
        cur += diff[i];
        a[i] = cur;
    }
    int t = 0;
    int i = 0;
    while (i < n) {
        if (a[i] == 0) {
            ++i;
            continue;
        }
        ++t;
        int value = a[i];
        while (i < n && a[i] == value) {
            ++i;
        }
    }
    return static_cast<int>(modPow(2, t, MOD));
}

int main() {
    // Test 1: single update covering whole array -> one group
    assert(countWays(5, {{1,5}}) == 2); // 2^1 = 2

    // Test 2: two disjoint updates -> two groups
    assert(countWays(6, {{1,2}, {5,6}}) == 4); // 2^2 = 4

    // Test 3: overlapping updates result in different values -> two groups
    // n=4, updates: [1,4] and [2,3] -> values: [1,2,2,1] -> groups: [1], [2,2], [1] -> 3 groups
    assert(countWays(4, {{1,4}, {2,3}}) == 8); // 2^3 = 8

    // Test 4: no updates -> t=0 -> answer 1
    assert(countWays(3, {}) == 1);

    // Test 5: updates that produce zeros? Actually can't if x<=y and all positive increments, but test with n=1
    assert(countWays(1, {{1,1}}) == 2); // one group

    // Test 6: adjacent different groups from intervals
    // n=5, updates: [1,2] and [3,5] -> values: [1,1,1,1,1]?? Wait: [1,2] => a[0]=1,a[1]=1; [3,5] => a[2]=1,a[3]=1,a[4]=1 -> all 1s -> one group
    assert(countWays(5, {{1,2}, {3,5}}) == 2);

    // Test 7: three non-overlapping groups
    // n=7, updates: [1,1], [3,4], [7,7] -> values: [1,0,1,1,0,0,1] -> groups: [1], [1,1], [1] -> t=3 -> 8
    assert(countWays(7, {{1,1}, {3,4}, {7,7}}) == 8);

    // Test 8: overlapping creating constant increase but still one group
    // n=3, updates: [1,3], [1,3] -> all values 2 -> one group
    assert(countWays(3, {{1,3}, {1,3}}) == 2);

    // Test 9: checking large n but small updates
    assert(countWays(100000, {{1,100000}}) == 2);

    // Test 10: mixed with zeros in middle
    // n=5, updates: [1,2] and [4,5] -> values: [1,1,0,1,1] -> groups: [1,1], [1,1] -> t=2 -> 4
    assert(countWays(5, {{1,2}, {4,5}}) == 4);

    cout << "All tests passed!" << endl;
    return 0;
}

// The problem is solved using a difference array to apply all range updates in `O(q)` time. For each update `(x, y)` in 1-indexed coordinates, we subtract 1 from `x` and `y` to make them 0-indexed, then increment `diff[x]` and decrement `diff[y+1]`. After processing all updates, we compute the prefix sum to obtain the final array `a[0...n-1]`. The number of groups `t` is counted by scanning `a`. We iterate through the array, and whenever we encounter a non-zero element that either starts a new run or is different from the previous run's value, we increment `t`. Specifically, we skip over all zeros. When we find a non-zero value, we increment `t` and then advance through the contiguous segment where the array value is equal to that starting value. This ensures that each contiguous block of identical non-zero values counts as one group. After computing `t`, we return `powmod(2, t, mod)` using fast exponentiation. Edge cases: if `n=0` (though constraints say `n>=1`), or if all updates result in zeros, `t=0` and answer is 1. The time complexity is `O(n + q)` and space is `O(n)` for the difference array and final array.
