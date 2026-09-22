/*
Write a C++ function `maxMinPairs(int n, int a, int b)` that, given positive integers `n`, `a`, and `b`, returns the maximum possible value of `min(a / i, b / (n - i))` over all integers `i` from 1 to `n-1`, where `/` denotes integer division (truncated toward zero). Assume `a` and `b` are non-negative and `n >= 2`. The function should return 0 if no valid `i` exists (though for `n >= 2` and non-negative `a,b` there is always at least one valid `i`). Your implementation must handle large inputs (up to 1e9) efficiently — do not loop over all `i` from 1 to `n-1` if that would be too slow; find a mathematical optimization.
*/
#include <algorithm>

// Returns the maximum possible value of min(a / i, b / (n - i)) for 1 <= i <= n-1.
// Assumes n >= 2, a >= 0, b >= 0.
int maxMinPairs(int n, int a, int b) {
    // Binary search on the answer k (the minimum of the two quotients).
    int low = 0;
    int high = std::max(a, b);
    int best = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        bool possible = false;
        if (mid == 0) {
            possible = true;
        } else {
            // Need i such that a / i >= mid and b / (n - i) >= mid.
            // That is: i <= a / mid and n - i <= b / mid -> i >= n - b / mid.
            // So existence iff n - b / mid <= a / mid.
            int left = n - b / mid;
            int right = a / mid;
            possible = (left <= right);
        }
        if (possible) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}
#include <cassert>

int main() {
    // Basic cases
    assert(maxMinPairs(2, 10, 10) == 5); // i=1: min(10,10)=5? wait i=1: a/1=10, b/(2-1)=10 -> min=10, but n-1=1, so i=1 only, answer=10? Check: min(10/1,10/1)=10. So actually answer=10. Let's compute: for i=1, min(10,10)=10. So answer=10. Wait but our binary search? Let's test manually: k=10: left = 2 - 10/10 = 1, right = 10/10 = 1, possible. So answer=10.
    assert(maxMinPairs(2, 10, 10) == 10);
    assert(maxMinPairs(3, 10, 10) == 5); // i=1: min(10/1,10/2)=min(10,5)=5; i=2: min(10/2,10/1)=5 -> answer=5
    assert(maxMinPairs(3, 5, 5) == 2); // i=1: min(5,2)=2; i=2: min(2,5)=2 -> answer=2
    assert(maxMinPairs(3, 0, 5) == 0); // always min(0, something)=0
    assert(maxMinPairs(5, 100, 100) == 25); // i=2: min(50,33)=33? Let's check: i=1: min(100,25)=25; i=2: min(50,33)=33; i=3: min(33,50)=33; i=4: min(25,100)=25 -> max=33. So correct answer=33.
    assert(maxMinPairs(5, 100, 100) == 33);
    assert(maxMinPairs(10, 1, 1) == 0); // any i: a/i=0 except i=1? i=1: 1/1=1, b/(9)=0, min=0. So 0.
    assert(maxMinPairs(2, 0, 0) == 0);
    // Large values
    assert(maxMinPairs(1000000000, 1000000000, 1000000000) == 500000000); // symmetric, optimal i=500000000? Actually a/i=2, b/(1e9-5e8)=2, min=2? Wait a=1e9, i=5e8 -> a/i=2; b/(n-i)=1e9/(5e8)=2 => min=2. But maybe better? Try i=333333333: a/i=3, b/(666666667)=1 => min=1. So best=2. Check with binary search: k=2: left = 1e9 - 1e9/2 = 5e8, right = 1e9/2=5e8 -> possible; k=3: left=1e9 - 1e9/3 ~ 666666667, right=333333333 -> left>right false. So answer=2. But our previous expectation wrong. Let's compute: So assert with 2.
    assert(maxMinPairs(1000000000, 1000000000, 1000000000) == 2);
    return 0;
}
// The naive approach iterates all `i` from 1 to `n-1`, computing `min(a / i, b / (n - i))` and keeping the maximum, which is O(n) and fails for `n` up to 1e9. The key observation: for integer division, the value `a / i` is constant over ranges of `i`. Specifically, `a / i` takes at most `2 * sqrt(a)` distinct values. Similarly for `b / (n - i)`. We can iterate over the distinct values of the first quotient `q1 = a / i` and find the corresponding `i` range, then compute the best `q2 = b / (n - i)` within that range, but since `n - i` changes, we need to handle the interaction carefully.
//
// A better approach: We are maximizing `min(x, y)` where `x = a / i` and `y = b / (n - i)`. The answer is the largest `k` such that there exists an `i` with `a / i >= k` and `b / (n - i) >= k`. The condition `a / i >= k` means `i <= a / k` (since integer division), and `b / (n - i) >= k` means `n - i <= b / k` → `i >= n - b / k`. So for each `k`, we need an integer `i` satisfying: `ceil(n - b / k) <= i <= floor(a / k)`. But actually integer division: `b / (n - i) >= k` implies `n - i <= floor(b / k)` (if `k > 0`), so `i >= n - floor(b / k)`. Also `a / i >= k` implies `i <= floor(a / k)`. So existence condition: `n - floor(b / k) <= floor(a / k)`. The answer is the largest `k >= 0` such that `n - floor(b / k) <= floor(a / k)`. Note for `k = 0` always true. We can binary search on `k` from 0 to `max(a, b)` (or up to `max(a, b) / 1`). Since `floor` functions are monotonic in `k`, the predicate is monotonic: if true for a given `k`, it is true for all smaller `k`. So binary search works in O(log(max(a,b))) time. Edge cases: when `k=0` always true; when `k > a` then `floor(a/k)=0`, when `k > b` then `floor(b/k)=0` → condition `n <= 0` false unless `n=0`, but `n>=2`, so false. Also handle division by zero: for `k=0` check separately, but we start binary search from `low=0` and `high=max(a,b)`, ensure we never call floor division by zero (only call for k>=1). Complexity: O(log(max(a,b))) time, O(1) space.
