// Write a standalone C++ function `winningHoldTimes` that takes a vector of race durations (total time allowed) and a vector of distance records, and returns a vector containing, for each race, the number of distinct integer hold times that would result in a distance strictly greater than the record. Each race is independent: for a given total time `T` and record `D`, a hold time `h` (with `0 <= h <= T`) yields a distance of `(T - h) * h`. The function must compute the count of integers `h` satisfying `(T - h) * h > D`. The input vectors are guaranteed to be non-empty and of equal length. All values are positive integers that fit in `int`. The function should be efficient even for large `T` values (up to `10^9`), using mathematics and binary search rather than brute‑force iteration. Return the result as a `std::vector<int>`.
The distance function `f(h) = (T - h) * h` is a downward‑opening parabola with its maximum at `h = T/2`. The set of `h` where `f(h) > D` forms a contiguous interval `(L, R)` of integers (if any exists) because the function increases up to the vertex and then decreases symmetrically. The key observations:  
- For even `T`, the maximum is at `h = T/2` with value `(T/2)^2`. If this isn’t greater than `D`, no solution exists (count 0).  
- For odd `T`, the two central points `h = floor(T/2)` and `h = ceil(T/2)` both give the same maximum `(T//2) * (T//2+1)`. If neither exceeds `D`, count 0.  
Otherwise, we search on the left side for the smallest `h` where `f(h) > D` (binary search in `[0, mid]`), and on the right side for the largest `h` where `f(h) > D` (binary search in `[mid, T]`). The count is `right - left + 1`. The binary searches are monotonic because `f(h)` is strictly increasing on `[0, floor(T/2)]` and strictly decreasing on `[ceil(T/2), T]`. Edge case: `D` might be zero, but that’s fine since any positive hold time (except h=0 or h=T) gives positive distance. Time complexity: O(n log T) for n races. Space complexity: O(n) for the output vector, O(1) extra per race.
#include <vector>

// Returns for each race the number of hold times h (0 ≤ h ≤ T) such that (T-h)*h > D.
std::vector<int> winningHoldTimes(const std::vector<int>& times, const std::vector<int>& records) {
    std::vector<int> results;
    results.reserve(times.size());

    for (size_t i = 0; i < times.size(); ++i) {
        int T = times[i];
        int D = records[i];
        int count = 0;

        // Maximum distance at the vertex (or two central points for odd T).
        if (T % 2 == 0) {
            long long mid = T / 2;
            long long maxDist = mid * mid;
            if (maxDist <= D) {
                results.push_back(0);
                continue;
            }
        } else {
            long long mid = T / 2;
            long long maxDist = (T - mid) * mid;  // same for mid and mid+1
            if (maxDist <= D) {
                results.push_back(0);
                continue;
            }
        }

        // Binary search for leftmost h where (T-h)*h > D.
        int lo = 0, hi = T / 2;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            long long dist = (long long)(T - mid) * mid;
            if (dist > D) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        int left = lo;

        // Binary search for rightmost h where (T-h)*h > D.
        lo = T / 2 + (T % 2);  // ceil(T/2)
        hi = T;
        while (lo < hi) {
            int mid = (lo + hi + 1) / 2;
            long long dist = (long long)(T - mid) * mid;
            if (dist > D) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        int right = lo;

        count = right - left + 1;
        results.push_back(count);
    }

    return results;
}
#include <cassert>
#include <vector>

// The solution function is declared above (include it verbatim).
int main() {
    // Example from Advent of Code 2023 Day 6 (first sample).
    std::vector<int> times1 = {7, 15, 30};
    std::vector<int> records1 = {9, 40, 200};
    std::vector<int> expected1 = {4, 8, 9};
    assert(winningHoldTimes(times1, records1) == expected1);

    // Edge case: impossible to beat.
    std::vector<int> times2 = {1};
    std::vector<int> records2 = {0};  // max distance = 0, can't beat 0? Actually h=0 or 1 gives 0, not >0, so 0.
    assert(winningHoldTimes(times2, records2) == std::vector<int>{0});

    // Edge case: exactly at maximum is not enough.
    std::vector<int> times3 = {4};
    std::vector<int> records3 = {4};  // max is 4 at h=2, but need >4, so none.
    assert(winningHoldTimes(times3, records3) == std::vector<int>{0});

    // Simple case where every interior hold wins.
    std::vector<int> times4 = {3};
    std::vector<int> records4 = {0};  // h=1 gives 2, h=2 gives 2, both >0.
    assert(winningHoldTimes(times4, records4) == std::vector<int>{2});

    // Odd total time.
    std::vector<int> times5 = {5};
    std::vector<int> records5 = {5};  // h=1:4, h=2:6, h=3:6, h=4:4 → only 2 and 3 win.
    assert(winningHoldTimes(times5, records5) == std::vector<int>{2});

    // Large T to ensure binary search works.
    std::vector<int> times6 = {1000000000};
    std::vector<int> records6 = {1000000000LL - 1};  // Most h win except near edges.
    // Compute expected by brute force for small sanity, but here just check it's positive.
    int cnt = winningHoldTimes(times6, records6)[0];
    assert(cnt > 0);
    assert(cnt == 1000000000 - 2);  // h from 1 to 999999999 all win? Actually need (T-h)*h > D. For T=1e9, D=999999999, at h=1: 999999999*1 = 999999999 not >, so h=1 fails. h=999999999 also fails. All others win. So count = 1e9 - 2.
    // But verify that with a simple check for a few values: For h=2: (1e9-2)*2 = 1999999996 > 999999999, yes.

    // Mixed multiple races.
    std::vector<int> times7 = {6, 10};
    std::vector<int> records7 = {5, 20};
    // For T=6, D=5: h=1→5, h=2→8, h=3→9, h=4→8, h=5→5 → wins at 2,3,4 = 3
    // For T=10, D=20: h=2→16, h=3→21, h=4→24, h=5→25, h=6→24, h=7→21, h=8→16 → wins at 3..7 = 5
    std::vector<int> expected7 = {3, 5};
    assert(winningHoldTimes(times7, records7) == expected7);
}
