You are given an array of `n` positive integers. For each of `m` independent queries, each consisting of two integers `(x, y)`, you must find the minimum possible penalty after optimally choosing one element from the array and modifying it (the modification is allowed to be zero, i.e., keeping the element unchanged). The penalty is defined as: if you choose an element `a[i]`, you must pay `max(0, x - a[i])` as the cost to increase `a[i]` up to at least `x` (if it is already ≥ `x`, cost is 0), and then you pay an additional penalty of `max(0, y - (sum_of_all_elements_after_increase))`. In other words, you first increase the chosen element to at least `x` (only upward, no decreasing allowed), and then the total sum `S` of the modified array is compared to `y`; if `S < y`, you pay the deficit `y - S`, otherwise pay 0. The total penalty is `max(0, x - a[i]) + max(0, y - new_sum)`. For each query independently, output the minimal possible total penalty over all choices of `i` (1 ≤ i ≤ n). The array is the same for all queries; only the query parameters `(x, y)` change. All values are 64-bit integers.
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// (The solution function is included above; below is the test main.)

int main() {
    // Test case 1: simple case
    vector<long long> a1 = {1, 5, 9};
    long long sum1 = 15;
    assert(minPenalty(a1, sum1, 3, 20) == 5); // choose 1 -> inc=2, sum=17, deficit=3, total=5; choose 5 -> inc=0, sum=15, deficit=5, total=5; choose 9 -> inc=0, total=5
    assert(minPenalty(a1, sum1, 3, 10) == 2); // choose 1 -> inc=2, sum=17, deficit=0, total=2; others -> inc=0, deficit=0
    assert(minPenalty(a1, sum1, 10, 20) == 5); // all <10, best choose 9 -> inc=1, sum=16, deficit=4, total=5

    // Test case 2: all elements >= x
    vector<long long> a2 = {10, 20, 30};
    long long sum2 = 60;
    assert(minPenalty(a2, sum2, 5, 50) == 0); // no inc, sum=60>=50
    assert(minPenalty(a2, sum2, 5, 100) == 40); // deficit=40

    // Test case 3: all elements < x
    vector<long long> a3 = {1, 2, 3};
    long long sum3 = 6;
    assert(minPenalty(a3, sum3, 10, 20) == 7); // choose 3 -> inc=7, sum=13, deficit=7, total=7; others worse

    // Test case 4: duplicates and negative D
    vector<long long> a4 = {2, 2, 8};
    long long sum4 = 12;
    assert(minPenalty(a4, sum4, 5, 10) == 3); // choose 2 -> inc=3, sum=15, deficit=0, total=3; choose 8 -> inc=0, sum=12, deficit=0
    assert(minPenalty(a4, sum4, 5, 20) == 8); // choose 2 -> inc=3, sum=15, deficit=5, total=8; choose 8 -> inc=0, sum=12, deficit=8

    // Test case 5: large x, y less than sum
    vector<long long> a5 = {4, 6};
    long long sum5 = 10;
    assert(minPenalty(a5, sum5, 3, 5) == 0); // choose 4 or 6, inc=0, sum=10>=5

    // Test case 6: single element
    vector<long long> a6 = {7};
    long long sum6 = 7;
    assert(minPenalty(a6, sum6, 10, 20) == 13); // inc=3, sum=10, deficit=10, total=13

    // Test case 7: boundary where predecessor and successor both exist
    vector<long long> a7 = {1, 4, 7};
    long long sum7 = 12;
    assert(minPenalty(a7, sum7, 4, 15) == 0); // successor 4 inc=0, sum=12<15 deficit=3 -> wait actually compute: sum=12, D=3, successor gives max(0,3)=3; predecessor 1 gives inc=3, sum=15, deficit=0, total=3; so min=3? Let's recalc: choose 4 -> inc=0, sum=12, deficit=3; choose 1 -> inc=3, sum=15, deficit=0, total=3; choose 7 -> inc=0, sum=12, deficit=3. So answer 3, not 0. Fix assert.
    assert(minPenalty(a7, sum7, 4, 15) == 3);

    // Test case 8: y exactly equal to sum + inc
    vector<long long> a8 = {3, 5};
    long long sum8 = 8;
    assert(minPenalty(a8, sum8, 4, 9) == 1); // choose 3 -> inc=1, sum=9, deficit=0, total=1; choose 5 -> inc=0, sum=8, deficit=1

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Given a sorted array `a` of size `n`, original sum `sum`, and a query (x, y),
// return the minimal penalty as described.
long long minPenalty(const vector<long long>& a, long long sum, long long x, long long y) {
    int n = (int)a.size();
    long long D = y - sum; // base deficit if no increment needed

    // Find first index with a[idx] >= x using binary search (sorted array)
    int t = lower_bound(a.begin(), a.end(), x) - a.begin(); // 0-based index, t in [0, n]

    long long ans = LLONG_MAX;

    // Candidate 1: use an element already >= x (if exists)
    if (t < n) {
        // No increment cost, penalty is only deficit (if any)
        ans = max(0LL, D);
    }

    // Candidate 2: use the largest element strictly less than x (if exists)
    if (t > 0) {
        long long inc = x - a[t-1]; // cost to raise it to x
        // Total penalty = max(inc, D) because new sum = sum + inc,
        // so deficit = max(0, y - (sum + inc)) = max(0, D - inc)
        // and total = inc + max(0, D - inc) = max(inc, D)
        long long cand = max(inc, D);
        if (cand < ans) ans = cand;
    }

    return ans;
}
// The key observation is that the penalty depends on two parts: the increment cost to raise `a[i]` to `x` (if needed), and a deficit cost based on the new total sum. If we choose an element `a[i]` and increase it to at least `x`, the increase amount is `max(0, x - a[i])`. The new sum becomes `S + max(0, x - a[i])`, where `S` is the original sum. The deficit term is then `max(0, y - (S + max(0, x - a[i])))`. However, increasing the chosen element beyond `x` is never beneficial because it only increases the sum (reducing deficit) but costs extra in the first term; the increase is exactly `max(0, x - a[i])` regardless of the second term, and increasing more would only add cost without reducing the first term. So for each element, the optimal is to increase it exactly to `x` (if it is below) or not change it (if it already ≥ `x`). The total penalty for choosing `i` is: `cost_i = max(0, x - a[i]) + max(0, y - (S + max(0, x - a[i])))`. We need the minimum over all `i`.
//
// Define `S` as the original sum. For an element with `a[i] >= x`, the increase is 0, so penalty = `max(0, y - S)`. For an element with `a[i] < x`, the increase is `x - a[i]`, so new sum = `S + x - a[i]`, penalty = `(x - a[i]) + max(0, y - (S + x - a[i]))`. Observe that for `a[i] < x`, the expression simplifies: `(x - a[i]) + max(0, y - S - x + a[i]) = max(x - a[i], y - S)`. Because if `y - (S + x - a[i]) >= 0`, then the sum is `x - a[i] + y - S - x + a[i] = y - S`. If negative, the max term is 0, leaving `x - a[i]`. Thus for `a[i] < x`, cost = `max(x - a[i], y - S)`. For `a[i] >= x`, cost = `max(0, y - S)`. So the problem reduces to: among all elements, find the minimal of:
// - If any element ≥ x, candidate = `max(0, y - S)`.
// - For elements < x, we need to minimize `max(x - a[i], D)` where `D = y - S`. That is, we want to choose an element with the smallest `x - a[i]`, i.e., the largest `a[i]` below `x`. Let `a[k]` be the largest element strictly less than `x` (or the predecessor of `x` in sorted order). Then the best candidate from below is `max(x - a[k], D)`. Also consider the smallest element ≥ `x` (the successor) as the candidate with cost `max(0, D)`.
//
// Thus, sort the array. For each query, find the lower bound of `x` (first index ≥ x). Let `t` be that index (1-based). If `t <= n`, candidate1 = `max(0, y - S)` (using that element). If `t > 1`, the predecessor at `t-1` gives candidate2 = `max(0, y - S) + (x - a[t-1])`? Wait careful: For `a[i] < x`, cost = `max(x - a[i], D)` where `D = y - S`. If `D >= 0`, then `max(x - a[i], D)` is either `D` if `D >= x - a[i]` else `x - a[i]`. But note that `D` is constant for all queries; the predecessor has the largest `a[i]` below `x`, so smallest `x - a[i]`, hence the max is minimized. So candidate2 = `max(D, x - a[t-1])` = `max(y - S, x - a[t-1])`. But if `D` is negative (i.e., `y < S`), then `max` with a negative number yields `x - a[t-1]` (since that is positive), and also candidate1 becomes 0. So we take the minimum of the two candidates: if `t <= n`, ans = `max(0, D)`; if `t > 1`, ans = min(ans, `max(D, x - a[t-1])`). Note that `D` can be negative, so `max(0, D)` = 0 if D < 0. The predecessor candidate uses `max(D, x - a[t-1])` which is at least 0 because `x - a[t-1] > 0`. So the answer is the minimum over valid candidates.
//
// Edge cases: If `t = n+1` (all elements < x), only predecessor exists. If `t = 1` (all elements ≥ x), only successor exists. If the array has duplicates, the predecessor is the largest value strictly less than x, which is fine. All calculations fit in 64-bit; use `long long`. Sorting takes O(n log n), each query O(log n) via binary search, total O((n+m) log n) time, O(1) extra space beyond the array.
