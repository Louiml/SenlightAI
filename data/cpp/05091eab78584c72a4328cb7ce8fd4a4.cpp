/*
Given an array `a` of `N` distinct positive integers (where `1 <= a[i] <= N`), write a C++ function `int minimumWindowSize(const vector<int>& a)` that returns the smallest window size `k` (with `2 <= k <= N`) such that when scanning the array with a sliding window of length `k`, the minimum value in each window is unique (no two windows share the same minimum value). If no such `k` exists, return `N+1`. The function must handle the trivial case `k=1` separately: for `k=1`, the minima are just the original elements, which are distinct by given constraints, so `k=1` always works; the function should return `1` if no `k >= 2` works, but the primary output is the smallest `k >= 2` that satisfies the condition. Note that the original code also checks that each minimum is within `[1, N-k+1]`; however, since all elements are within `[1, N]`, this constraint is automatically satisfied for any window minimum. Implement the solution efficiently for `N` up to 200,000.
*/

#include <vector>
#include <deque>
#include <algorithm>

// Returns the smallest window length k (1 <= k <= N) such that all sliding-window
// minima of length k are distinct. If no k >= 2 works, returns N+1.
int minimumWindowSize(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    if (n == 0) return 0;
    if (n == 1) return 1;

    auto check = [&](int k) -> bool {
        std::deque<int> dq; // stores indices, not values
        std::vector<bool> seen(n + 1, false);
        for (int i = 0; i < n; ++i) {
            // remove from back while current value is <= back value (to keep minimum)
            while (!dq.empty() && a[i] <= a[dq.back()]) {
                dq.pop_back();
            }
            dq.push_back(i);
            // remove from front if index is outside window
            while (dq.front() <= i - k) {
                dq.pop_front();
            }
            if (i >= k - 1) {
                int currMin = a[dq.front()];
                if (seen[currMin]) return false;
                seen[currMin] = true;
            }
        }
        return true;
    };

    // Binary search on k in [2, n+1]
    int lo = 2, hi = n + 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (check(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    if (lo == n + 1) {
        // no k >= 2 works, so return 1
        return 1;
    }
    return lo;
}

#include <cassert>
#include <vector>

// The function under test is declared above (minimumWindowSize).

int main() {
    // Example from typical problem: N=5, a = [2,1,3,5,4]
    // k=2: windows (2,1)->1, (1,3)->1 duplicate -> fail
    // k=3: (2,1,3)->1, (1,3,5)->1 duplicate -> fail
    // k=4: (2,1,3,5)->1, (1,3,5,4)->1 duplicate -> fail
    // k=5: (2,1,3,5,4)->1 only one window -> works
    assert(minimumWindowSize({2,1,3,5,4}) == 5);

    // N=3, a=[1,2,3] distinct values
    // k=2: windows (1,2)->1, (2,3)->2 distinct -> works
    assert(minimumWindowSize({1,2,3}) == 2);

    // N=3, a=[3,1,2]
    // k=2: windows (3,1)->1, (1,2)->1 duplicate -> fail
    // k=3: one window -> works
    assert(minimumWindowSize({3,1,2}) == 3);

    // N=2, a=[2,1] -> k=2 works (only one window)
    assert(minimumWindowSize({2,1}) == 2);

    // N=1 -> return 1
    assert(minimumWindowSize({1}) == 1);

    // N=6, a=[1,2,3,4,5,6]
    // k=2: minima are 1,2,3,4,5 -> distinct -> works
    assert(minimumWindowSize({1,2,3,4,5,6}) == 2);

    // N=6, a=[6,5,4,3,2,1]
    // k=2: minima are 5,4,3,2,1 -> distinct -> works
    assert(minimumWindowSize({6,5,4,3,2,1}) == 2);

    // N=4, a=[2,1,2,1]? But problem says distinct elements, so skip.
    // Instead, N=4, a=[4,3,2,1] -> k=2 minima: 3,2,1 distinct -> works
    assert(minimumWindowSize({4,3,2,1}) == 2);

    // N=5, a=[1,3,2,5,4] 
    // k=2: windows: (1,3)->1, (3,2)->2, (2,5)->2, (5,4)->4 -> duplicate 2 -> fail
    // k=3: (1,3,2)->1, (3,2,5)->2, (2,5,4)->2 duplicate -> fail
    // k=4: (1,3,2,5)->1, (3,2,5,4)->2 distinct -> works
    assert(minimumWindowSize({1,3,2,5,4}) == 4);

    // N=5, a=[5,4,3,2,1] -> k=2 minima: 4,3,2,1 distinct -> works
    assert(minimumWindowSize({5,4,3,2,1}) == 2);

    return 0;
}

// The problem asks for the smallest `k >= 2` such that all sliding-window minima of length `k` are distinct. We can binary search on `k` because the property is monotonic: if a certain `k` works, then any larger `k` also works. Why? For a larger window, the minimum values are necessarily less than or equal to the minima of smaller windows, but the distinctness property tends to become easier because there are fewer windows (N-k+1 windows) and the minima are drawn from a smaller set of possible values. More formally, if `k` works, then for `k' > k`, consider any two windows of length `k'`. Their minima are the minima of two disjoint or overlapping subwindows of length `k`; if those subwindows had distinct minima, then the larger windows' minima are distinct as well. The binary search range is `[2, N+1]`, where `N+1` is a sentinel meaning "no valid `k >= 2`". For each candidate `k`, we scan the array using a monotonic queue (deque) that maintains the minimum of the current window in amortized O(1) per element. As we slide the window, we add the new element (removing from the back any element that is greater than or equal to the new value, to keep the queue non-decreasing), get the front as the current minimum, then remove the element that leaves the window if it matches the front. We track whether each minimum value has been seen before using a boolean array of size `N+1`. If we encounter a duplicate minimum, the candidate `k` fails. If all `N-k+1` windows have distinct minima, it passes. The binary search does O(log N) iterations, each O(N), so total time is O(N log N). Space complexity is O(N) for the boolean array and O(k) for the deque (but bounded by N). Edge cases: `N=1`? The problem implies `N >= 2` for the main loop, but the function should handle `N=1` gracefully by returning `1` (since `k=1` works). For `N=2`, `k=2` is the only candidate; if the two elements are distinct (guaranteed since array has distinct values), it works, so return `2`. Also note that the original code's extra check `curr > N - k + 1` is redundant because all `a[i] <= N`, and the minimum of a window of size `k` is at most `N-k+1` if the distinctness condition holds for the range, but it's not always true; however, the problem statement here drops that requirement, so we only check distinctness. The test cases should verify the function against brute force for small inputs and include edge cases.
