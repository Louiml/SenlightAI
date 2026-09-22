/*
You are managing a set of projects, each with an initial cost `a_i` and a bonus reward `b_i`. You have the ability to upgrade up to `K` projects. Upgrading a project of type `i` changes its total profit from `min(a_i, b_i)` to `min(2*a_i, b_i)`. Given `N` projects and a maximum number of upgrades `K`, write a C++ function `long long maxTotalProfit(const vector<pair<long long, long long>>& projects, long long K)` that returns the maximum total profit achievable by selecting at most `K` projects to upgrade. You may choose fewer than `K` upgrades if it is not beneficial to upgrade more. All values are positive integers (a_i > 0, b_i > 0). The input may have `K` larger than `N`, in which case you can upgrade at most `N` projects.
*/

#include <bits/stdc++.h>
using namespace std;

// Function to compute maximum total profit after upgrading at most K projects.
// projects: vector of pairs (a_i, b_i) where a_i is initial cost, b_i is bonus reward.
// K: maximum number of projects that can be upgraded.
// Returns the maximum total profit.
long long maxTotalProfit(const vector<pair<long long, long long>>& projects, long long K) {
    int N = (int)projects.size();
    long long base = 0;
    vector<long long> gains;
    gains.reserve(N);
    
    for (const auto& p : projects) {
        long long a = p.first;
        long long b = p.second;
        long long original = min(a, b);
        long long upgraded = min(2 * a, b);
        base += original;
        gains.push_back(upgraded - original); // always non-negative
    }
    
    // Sort gains in descending order
    sort(gains.rbegin(), gains.rend());
    
    long long upgrades = min(K, (long long)N);
    for (long long i = 0; i < upgrades; ++i) {
        base += gains[i];
    }
    return base;
}

#include <cassert>
#include <vector>
#include <utility>
using namespace std;

// The solution function is assumed to be defined above.

int main() {
    // Test 1: Basic case, upgrade one out of two
    vector<pair<long long, long long>> p1 = {{5, 10}, {3, 4}};
    // base: min(5,10)=5 + min(3,4)=3 => 8
    // gains: min(10,10)-5=5, min(6,4)-3=1
    // K=1: add 5 => total 13
    assert(maxTotalProfit(p1, 1) == 13);

    // Test 2: No upgrades allowed
    assert(maxTotalProfit(p1, 0) == 8);

    // Test 3: K larger than N, upgrade all
    // gains sorted: 5, 1 => total base 8 + 6 = 14
    assert(maxTotalProfit(p1, 100) == 14);

    // Test 4: Single project, upgrade
    vector<pair<long long, long long>> p2 = {{7, 7}};
    // base = 7, gain = min(14,7)-7 = 0, so total remains 7
    assert(maxTotalProfit(p2, 1) == 7);

    // Test 5: Projects where gain is zero for all
    vector<pair<long long, long long>> p3 = {{1, 1}, {2, 2}};
    // base = 1+2=3, gains = 0+0=0
    assert(maxTotalProfit(p3, 2) == 3);

    // Test 6: Mixed large values, ensure long long overflow is not an issue
    vector<pair<long long, long long>> p4 = {{1000000000, 1000000001}, {999999999, 2000000000}};
    // base = min(1e9,1e9+1)=1e9 + min(999999999,2e9)=999999999 => 1999999999
    // gains: min(2e9,1e9+1)-1e9 = (1e9+1)-1e9=1 ; min(1999999998,2e9)-999999999 = 1999999998-999999999=999999999
    // K=1: add 999999999 => total 2999999998
    assert(maxTotalProfit(p4, 1) == 2999999998LL);

    // Test 7: Upgrade with K=1 but best gain should be chosen
    vector<pair<long long, long long>> p5 = {{10, 15}, {10, 12}, {10, 9}};
    // base: 10+10+9=29 (since min(10,15)=10, min(10,12)=10, min(10,9)=9)
    // gains: min(20,15)-10=5, min(20,12)-10=2, min(20,9)-9=0
    // K=2: add 5+2=7 => total 36
    assert(maxTotalProfit(p5, 2) == 36);

    return 0;
}

// The key insight is that upgrading a project always yields a non-negative increase in profit (since doubling the initial cost can only increase or keep the same the minimum with the reward). Therefore, to maximize total profit, we should always upgrade up to `K` projects, choosing the ones with the largest individual profit gains. For each project `i`, compute its base profit as `min(a_i, b_i)` and its upgraded profit as `min(2*a_i, b_i)`. The gain for upgrading that project is the difference `gain_i = min(2*a_i, b_i) - min(a_i, b_i)`, which is always non-negative. We compute the base total by summing all base profits, then sort the gains in descending order, and add the top `K` gains (or all gains if `K > N`) to the base total. Edge cases: if `K` is 0, return the base sum; if `K >= N`, sum all gains. Time complexity is O(N log N) due to sorting, and space complexity is O(N) for the gains array.
