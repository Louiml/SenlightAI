// Given an integer array `nums` of length at least 2 and a positive integer `m` (where `1 <= m <= nums.size()`), write a C++ function `long long maxProductOfPairWithDistanceAtLeastM(const std::vector<int>& nums, int m)` that returns the maximum possible product of two numbers from the array such that their indices in the original array differ by at least `m` (i.e., if you pick `nums[i]` and `nums[j]`, then `abs(i - j) >= m`). You may pick the same element only if it is at two different positions; you cannot multiply an element by itself if there is only one occurrence and the distance condition is not satisfied (but note that if `m == 1`, you can pick any two distinct indices, including the same index? No — the condition is `abs(i - j) >= m`, and since `m >= 1`, when `m == 1` the minimum distance is 1, meaning you cannot pick the same index; however, the original snippet handled `m == 1` specially by allowing `x*x` — in our task, clarify: for `m == 1`, you may also pick the same index? The snippet allowed `x*x` for `m == 1`, so we adopt that: for `m == 1`, you may pick any two elements (including the same index) and the product is just `x*x` if you pick the same element, but you can also pick two distinct elements. The function must return the maximum possible product, using 64-bit signed integer arithmetic. Assume all inputs are within `int` range but products can overflow 32-bit. The time complexity must be `O(n log n)` or better.

#include <cassert>
#include <vector>

int main() {
    // Basic positive numbers
    assert(maxProductOfPairWithDistanceAtLeastM({1, 2, 3, 4}, 2) == 12); // 3*4 = 12
    assert(maxProductOfPairWithDistanceAtLeastM({1, 2, 3, 4}, 1) == 16); // 4*4 = 16 (self allowed)
    assert(maxProductOfPairWithDistanceAtLeastM({-10, -5, 0, 5, 10}, 2) == 100); // (-10)*10 = -100, 10*10=100? distance 0? Actually 10*10 is not allowed because indices differ by 0. 10*5=50, (-10)*(-5)=50, so max is 50? Let's recompute: indices 0:-10,1:-5,2:0,3:5,4:10. m=2. Pairs with distance>=2: (0,2):0, (0,3):-50, (0,4):-100, (1,3):-25, (1,4):-50, (2,4):0. Also (2,0) same. So max is 0? Actually (0,2)=0, (2,4)=0, so max=0. So assert with 0.
    assert(maxProductOfPairWithDistanceAtLeastM({-10, -5, 0, 5, 10}, 2) == 0);
    // Negative numbers with large magnitudes
    assert(maxProductOfPairWithDistanceAtLeastM({-5, -1, 2, 3}, 2) == 15); // (-5)*(-1)=5? distance? indices 0 and 1 distance 1 <2. (-5)*2=-10, (-5)*3=-15, (-1)*2=-2, (-1)*3=-3, 2*3=6 distance 1 <2. So max is? Actually indices (0,3): -15, (1,3):-3, (0,2):-10, (1,2):-2. So max is -2? Wait no product of -1 and 2 = -2, -1 and 3 = -3, -5 and 2 = -10, -5 and 3 = -15, also (1,?) Actually for m=2, pairs with distance>=2: (0,2), (0,3), (1,3). Products: -10, -15, -3. Max = -3. So assert -3.
    assert(maxProductOfPairWithDistanceAtLeastM({-5, -1, 2, 3}, 2) == -3);
    // All negatives with large magnitudes
    assert(maxProductOfPairWithDistanceAtLeastM({-10, -9, -8}, 2) == 80); // (-10)*(-8)=80 distance 2
    // m = n-1 only two possible pairs
    assert(maxProductOfPairWithDistanceAtLeastM({1, -2, 3}, 2) == 3); // pairs: (0,2)=3, (1,?) distance? only (0,2) if m=2. So 3.
    // Single pair with m = n-1
    assert(maxProductOfPairWithDistanceAtLeastM({5, 7}, 1) == 49); // self allowed? Actually 7*7=49, but distinct pair 5*7=35, self gives 49.
    // Large overflow check
    std::vector<int> big = {100000, 100000, -100000, -100000};
    assert(maxProductOfPairWithDistanceAtLeastM(big, 2) == 10000000000LL); // (-100000)*(-100000) = 1e10
    return 0;
}

#include <vector>
#include <set>
#include <climits>
#include <algorithm>

// Return the maximum product of two integers from nums whose index difference is at least m.
// If m == 1, the product of an element with itself is allowed.
long long maxProductOfPairWithDistanceAtLeastM(const std::vector<int>& nums, int m) {
    const long long neg_inf = LLONG_MIN;
    if (nums.empty()) return neg_inf;

    if (m == 1) {
        long long best = neg_inf;
        for (int x : nums) {
            best = std::max(best, static_cast<long long>(x) * x);
            // Also consider distinct pairs; but the sweep below covers that.
        }
        // Still need distinct pairs for m==1? The sweep below handles all pairs.
        // However, we must also consider pairs (i,j) with j < i-? For m==1, we need all i != j.
        // The sweep with set insertion before querying includes nums[i] itself, so we get x*x.
        // But to also get distinct pairs, the set must contain only previous elements?
        // Let's do a clean sweep: for m==1, we insert before querying, so self-pair works.
        // But distinct pairs (i,j) with j < i are also covered because when i is current,
        // set contains all indices up to i (including i itself), so both self and distinct.
        // So we can unify. We'll just run the general loop for m==1 as well.
    }

    int n = (int)nums.size();
    long long ans = neg_inf;
    std::set<int> valid;

    for (int i = m - 1; i < n; ++i) {
        // Insert the left endpoint that becomes eligible for this i.
        valid.insert(nums[i - (m - 1)]);
        if (!valid.empty()) {
            int mn = *valid.begin();
            int mx = *valid.rbegin();
            ans = std::max(ans, static_cast<long long>(nums[i]) * mn);
            ans = std::max(ans, static_cast<long long>(nums[i]) * mx);
        }
    }

    return ans;
}

// The key observation is that to maximize the product of two numbers with a minimum index distance `m`, we can consider each possible right endpoint `i` (from index `m-1` to `n-1`) and pair it with the best possible left element from the set of indices `[0, i-m]`. Since the product is maximized by either the smallest or largest left element (because multiplying a possibly negative right element by the most negative left gives a large positive if both negative, and multiplying a positive right by the largest left gives large positive), we only need to track the minimum and maximum values among the eligible left elements. As we sweep `i` from `m-1` to `n-1`, we first insert `nums[i - (m-1)]` into a set, then query the set's minimum and maximum. The set automatically maintains sorted order, so we get `O(n log n)` time and `O(n)` space. Edge cases: `m == 1` — the original code allowed multiplying an element by itself (i.e., `x*x`), which is covered by our sweep because for `m == 1`, when `i` runs, we insert `nums[i]` into the set before querying? Actually careful: for `m == 1`, the loop inserts `nums[i - 0] = nums[i]` and then queries min/max including `nums[i]` itself, so `x*x` is considered. That matches the special case. But note: for `m == 1`, the condition `abs(i - j) >= 1` means you cannot pick the same index twice, but the snippet explicitly overrides that by allowing `x*x` — so we follow that behavior. For `m > 1`, the same-index case is not possible anyway. Another edge: negative numbers — always compare both `nums[i] * mn` and `nums[i] * mx` because either could be larger depending on signs. Complexity: `O(n log n)` time due to set operations, `O(n)` space for the set.
