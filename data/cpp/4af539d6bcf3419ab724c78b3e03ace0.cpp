Given a sorted array of `n` integers (1 ≤ n ≤ 1000, each value in range [-10000, 10000]), write a C++ function that finds the minimum possible maximum absolute error `L` and a linear function `a_i ≈ first + delta * i` (where `i` is the 1-based index in the sorted array) such that for every element `a[i]`, the absolute difference between `a[i]` and `first + delta * i` is at most `L`. The function must return a struct containing the minimal `L`, and the corresponding `first` and `delta` values, where `delta` is guaranteed to be in range [0, 20000] and `first` is an integer such that the linear fit error is minimized. If multiple valid triples exist for the minimal `L`, choose the one with the smallest `first`, and if still tied, smallest `delta`. The input array is already sorted in non-decreasing order.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Perfect linear progression
    int a1[] = {1, 3, 5, 7, 9};
    auto res1 = find_optimal_linear_fit(a1, 5);
    assert(res1.minimal_error == 0);
    assert(res1.first == 1);
    assert(res1.delta == 2);

    // Test 2: Constant array
    int a2[] = {5, 5, 5};
    auto res2 = find_optimal_linear_fit(a2, 3);
    assert(res2.minimal_error == 0);
    // For constant, any delta works; the algorithm picks delta=0 and first=5
    assert(res2.first == 5);
    assert(res2.delta == 0);

    // Test 3: Slight noise around a line
    int a3[] = {2, 4, 5, 8, 10};
    auto res3 = find_optimal_linear_fit(a3, 5);
    // Minimal error is at least 1 (point 5 vs line 6? Actually test with L=1)
    assert(res3.minimal_error == 1);
    // Known: line first=1 delta=2 gives values 3,5,7,9,11 -> errors 1,1,2,1,1 -> max 2
    // Line first=2 delta=2 gives 4,6,8,10,12 -> errors 2,2,3,2,2 -> max 3
    // Line first=1 delta=1 gives 2,3,4,5,6 -> errors 0,1,1,3,4 -> max 4
    // So best is actually first=2 delta=1? Let's compute: 3,4,5,6,7 -> errors 1,0,0,2,3 -> max 3
    // Better: first=1 delta=1? already done. Try first=0 delta=2: 2,4,6,8,10 -> errors 0,0,1,0,0 -> max 1! So minimal L=1.
    // The algorithm will find some first/delta with L=1, e.g., first=0 delta=2.
    assert(res3.first == 0);
    assert(res3.delta == 2);

    // Test 4: Single element
    int a4[] = {-5};
    auto res4 = find_optimal_linear_fit(a4, 1);
    assert(res4.minimal_error == 0);
    assert(res4.first == -5);
    assert(res4.delta == 0);

    // Test 5: Worst-case spread
    int a5[] = {-10000, 10000};
    auto res5 = find_optimal_linear_fit(a5, 2);
    // With delta=0, first can be between -10000-L and 10000-L for both, need L >= 10000
    // Try delta=1: requires |a1 - (first+1)| <= L and |a2 - (first+2)| <= L
    // Let first=-10000, then values: -9999, -9998 -> errors 1, 19998 -> L >=19998 >10000
    // So minimal L=10000 with delta=0, first could be 0? Check first=0 gives errors 10000 each -> L=10000.
    // Tie-breaking smallest first: first=-10000? For L=10000, feasible range for first is [-10000-10000, -10000+10000] = [-20000,0] from first element, and [10000-10000, 10000+10000] = [0,20000] from second, intersection is [0,0] so first=0.
    assert(res5.minimal_error == 10000);
    assert(res5.first == 0);
    assert(res5.delta == 0);
}

#include <algorithm>
#include <climits>

struct LinearFitResult {
    int minimal_error;
    int first;
    int delta;
};

// Given a sorted array `a` of size `n`, find the minimal L and the corresponding
// first and delta such that |a[i] - (first + delta*i)| <= L for all 1-based i.
LinearFitResult find_optimal_linear_fit(const int* a, int n) {
    // Binary search on the minimal error L
    int lo = 0;
    int hi = 10000; // worst case difference between min and max values
    int best_first = 0;
    int best_delta = 0;

    auto feasible = [&](int limit, int* out_first, int* out_delta) -> bool {
        for (int d = 0; d <= 20000; ++d) {
            int l = INT_MIN;
            int r = INT_MAX;
            bool ok = true;
            for (int i = 1; i <= n; ++i) {
                long long lower = (long long)a[i-1] - (long long)d * i - limit;
                long long upper = (long long)a[i-1] - (long long)d * i + limit;
                if (lower > l) l = (int)lower;
                if (upper < r) r = (int)upper;
                if (l > r) {
                    ok = false;
                    break;
                }
            }
            if (ok && l <= r) {
                *out_first = l;
                *out_delta = d;
                return true;
            }
        }
        return false;
    };

    while (lo < hi) {
        int mid = (lo + hi) / 2;
        int first_check, delta_check;
        if (feasible(mid, &first_check, &delta_check)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    bool found = feasible(lo, &best_first, &best_delta);
    // feasible must be true for the minimal L because binary search invariant
    // ensures lo is the smallest L that works.
    return {lo, best_first, best_delta};
}

// The problem asks for the smallest possible `L` such that there exists an integer arithmetic progression `first + delta*i` (with `i` starting at 1) that stays within a band of width `2L` centered at each `a[i]`. Since the array is sorted, we can binary search on `L` in range [0, 10000] (worst case difference between two elements). For a fixed `L`, we need to check whether there exists an integer `delta` in [0, 20000] and an integer `first` such that for all `i`, `|a[i] - (first + delta*i)| ≤ L`. For each candidate `delta`, we can derive the feasible range for `first` from the inequalities: `a[i] - delta*i - L ≤ first ≤ a[i] - delta*i + L`. So for each `delta`, we compute the lower bound `l` as the maximum of all lower bounds and upper bound `r` as the minimum of all upper bounds. If `l ≤ r`, then such a `first` exists (we pick the smallest integer `first = l` to satisfy tie-breaking). We iterate `delta` from 0 to 20000. The time complexity is O(log 10000 * 20000 * n) ≈ O(15 * 20000 * 1000) ≈ 3e8 operations, which is borderline but acceptable for typical limits. We can optimize by early breaking when `l > r`. Space complexity is O(1) aside from input array.
