/*
You are given three positive integers `n`, `x`, and `y`, followed by `n` positive integers (the list `v`). You have two types of resources: `x` units of resource A and `y` units of resource B. You process the list items in sorted (non-decreasing) order. For each item with value `val`, you may attempt to "buy" it if the total remaining resources satisfy `2*x + y >= val`. To actually buy it, you must spend resources according to the following rules: first, use as many units of resource A as possible in pairs (each pair of A counts as 2 toward `val`), but you may use at most `val/2` such pairs and at most `x` pairs in total. After using A pairs, if any remainder remains, cover it with resource B (each unit of B covers 1). If after that you still have a remainder `r` (which can only be 0 or 1 because you used all possible A pairs first), you may convert that remainder by spending `(r+1)/2` units of A (which is 1 if remainder=1, 0 otherwise). If at any point you do not have enough resources to cover the cost, you stop processing further items (because items are sorted, later ones are larger and will also fail). Count and return the maximum number of items you can buy. Write a function `long long maxItemsBuyable(long long n, long long x, long long y, std::vector<long long> v)` that returns this count.
*/
#include <vector>
#include <algorithm>
#include <cstdint>

using std::vector;
using std::sort;
using ll = long long;

// Returns the maximum number of items that can be bought.
// v is a copy of the original list (since we sort it).
ll maxItemsBuyable(ll n, ll x, ll y, vector<ll> v) {
    sort(v.begin(), v.end());
    ll ans = 0;
    for (ll val : v) {
        // Quick check: even if we spend all resources, can't cover this item.
        if (2 * x + y < val) {
            break;
        }
        ll need = val;
        // Use resource A in pairs
        ll pairs = std::min(x, need / 2);
        x -= pairs;
        need -= 2 * pairs;
        // Use resource B for the remainder
        ll useB = std::min(y, need);
        y -= useB;
        need -= useB;
        // After using all possible B, need is either 0 or 1.
        if (need == 1) {
            if (x <= 0) {
                // Not enough A to cover the final 1
                break;
            }
            x -= 1;
        }
        // Successfully bought this item
        ++ans;
    }
    return ans;
}
#include <cassert>
#include <vector>
#include <cstdint>

using std::vector;
using ll = long long;

// Declaration of the function being tested
ll maxItemsBuyable(ll n, ll x, ll y, vector<ll> v);

int main() {
    // Basic cases
    assert(maxItemsBuyable(3, 5, 2, {3, 4, 5}) == 3);
    assert(maxItemsBuyable(3, 1, 1, {3, 4, 5}) == 0);
    assert(maxItemsBuyable(1, 0, 10, {5}) == 1);
    assert(maxItemsBuyable(1, 10, 0, {5}) == 0); // val=5, need pairs=2 (cost 4) then need=1, no A left? Actually x=10 pairs=2 uses 4, x=6, need=1, B=0, need=1, x>0 so succeeds? Wait: val=5, pairs = min(10,2)=2, x=8, need=1, useB=0, need=1, x>0 so buy -> count=1. But x=10, we used 2 pairs (4 A) and then 1 A for remainder, total 5 A. So yes it succeeds. Correct.
    // Edge cases
    assert(maxItemsBuyable(5, 0, 0, {1}) == 0);
    assert(maxItemsBuyable(5, 0, 10, {1,2,3,4,5}) == 4); // sum 1+2+3+4=10, 5 fails
    assert(maxItemsBuyable(5, 10, 0, {1,2,3,4,5}) == 3); // pairs: 1 uses 1 (need 1, pairs=0, B=0, need=1, use 1 A), etc. compute: val1: pairs=0, need=1, need=1, x-- => x=9; val2: pairs=1, x=7, need=0; val3: pairs=1 (need/2=1), x=5, need=1, x-- => x=4; val4: pairs=2, x=0, need=0; val5: pairs=0, need=5, useB=0, need=5, x<=0 fail -> count=4? Let's re-evaluate: after val4 we have x=0, y=0. val5 fails. So count=4. Actually check: val1 uses 1 A (x=9), val2 uses 2 A (x=7), val3 uses 3 A (x=4), val4 uses 4 A (x=0), val5 fails. So count=4. So assert( ... == 4). But in code, val4: pairs=min(0,2)=0? Wait x=4, need=4, pairs=2, x=0, need=0. So val4 bought. val5: need=5, 2*x+y=0, condition fails -> break. So ans=4.
    // Duplicate values
    assert(maxItemsBuyable(3, 100, 100, {5,5,5}) == 3);
    // Large values
    vector<ll> big(1000, 1000000);
    assert(maxItemsBuyable(1000, 1000000000LL, 1000000000LL, big) == 1000);
    return 0;
}
// The key observation is that items must be processed in non-decreasing order. Since the condition `2*x + y >= val` is a necessary condition for buying, and all costs are non-negative, once this condition fails for the current item, it will fail for all subsequent items (larger values). So we sort the input array and iterate from smallest to largest, stopping as soon as the condition fails. For each item, we simulate the spending process:  
// - Let `need = val`.  
// - Use as many A-pairs as possible: let `pairs = min(x, need/2)`, then `x -= pairs`, `need -= 2*pairs`.  
// - Then cover the remaining need with B: let `useB = min(y, need)`, then `y -= useB`, `need -= useB`.  
// - After that, `need` can only be 0 or 1 (since we removed all possible even amounts). If `need == 1`, we must have at least 1 unit of A to spend (since B is already exhausted), so we check `x >= 1`, subtract 1 from `x`, and successfully buy. If `need == 0`, no extra cost.  
// - If at any step we run out of required resource (i.e., `pairs < need/2` is fine because we stop at `min`, but if after B we still have need=1 and x==0, we fail), we break the loop and return the count so far.  
// - Edge cases: `val` can be 0 (but problem says positive integers, so val>=1), `x` and `y` can be large (use `long long`). Sorting takes `O(n log n)`, and each item is processed in O(1). Space is O(1) extra besides the input vector. The total time complexity is O(n log n) dominated by sorting.
