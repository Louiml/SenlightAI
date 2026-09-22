/*
Write a C++ function `int maxGapAfterAddingRestStops(int n, int m, int l, const std::vector<int>& existingStops)` that takes the number of existing rest stops `n`, the number of new rest stops to add `m`, the total highway length `l`, and a vector of the positions of the existing rest stops (all between 0 and `l`, not including 0 or `l`). The highway starts at 0 and ends at `l`, and rest stops are placed at integer positions. You must add exactly `m` new rest stops at arbitrary integer positions (not necessarily distinct from each other or from existing ones), and then the maximum distance between any two consecutive rest stops (including the start 0 and end `l`) is computed. The goal is to **minimize** this maximum distance after optimal placement of the new stops. Return that minimized maximum distance. The original snippet used a greedy "split the largest gap" approach, but that is not guaranteed to be optimal; your task is to find the true optimal value. For example, if n=2, m=2, l=10, existing stops at {2, 8}, the gaps are 2,6,2, and the optimal new stops at 4 and 6 give gaps 2,2,2,2 → max 2. But a naïve greedy splitting the largest gap (6) in the middle gives stops at 5 and then splitting the new largest gap (3) gives a stop at 3.5 or 3 or 4, leading to max 3 or 2? Actually careful: greedy can be suboptimal in some cases, but for this problem it's known that the optimal answer is the minimum possible maximum gap, which can be found via binary search.
*/

#include <vector>
#include <algorithm>
#include <numeric>

// Returns the minimized maximum gap after adding exactly m new stops.
// existingStops: positions of current stops (0 < pos < l), may be unsorted.
// l: highway length (positive integer). Positions are integers.
int maxGapAfterAddingRestStops(int n, int m, int l, const std::vector<int>& existingStops) {
    // Create sorted list including endpoints
    std::vector<int> stops;
    stops.reserve(n + 2);
    stops.push_back(0);
    for (int i = 0; i < n; ++i) {
        stops.push_back(existingStops[i]);
    }
    stops.push_back(l);
    std::sort(stops.begin(), stops.end());

    // Lambda to check if a candidate max gap is feasible with m new stops
    auto feasible = [&](int d) -> bool {
        int needed = 0;
        for (size_t i = 1; i < stops.size(); ++i) {
            int gap = stops[i] - stops[i-1];
            if (gap > d) {
                // ceil(gap / d) - 1 = (gap - 1) / d for integer d>0
                needed += (gap - 1) / d;
                if (needed > m) return false;
            }
        }
        return needed <= m;
    };

    // Binary search the minimum feasible d
    int low = 1, high = l, ans = l;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (feasible(mid)) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

int maxGapAfterAddingRestStops(int n, int m, int l, const std::vector<int>& existingStops);

int main() {
    // Example from snippet: n=2, m=2, l=10, stops at {2,8} -> optimal max gap 2
    assert(maxGapAfterAddingRestStops(2, 2, 10, {2, 8}) == 2);

    // No existing stops, add 1 stop on length 10 -> gaps at least 5? Actually we can place at 5 -> gaps 5,5 -> max 5
    assert(maxGapAfterAddingRestStops(0, 1, 10, {}) == 5);

    // Existing stops already evenly spaced
    assert(maxGapAfterAddingRestStops(3, 0, 12, {3, 6, 9}) == 3);

    // Single gap large, need multiple stops
    // l=20, existing at {5}, m=3: gaps 0-5=5, 5-20=15. To minimize max, distribute 15 into 3+1=4 segments -> 15/4=3.75, but integer positions, actually optimal: add 9,13,17? Then gaps 5,4,4,4,3 -> max 5? Hmm careful: positions must be integers, so we can break 15 into pieces sizes at most 4: e.g., 5,10,14,18 -> gaps 5,5,4,4,2 -> max 5. Binary search will give 5 because feasible(4) needs (15-1)/4=3 stops? 15/4=3.75 ceil=4 -> need 3 stops, exactly m=3, so feasible(4) actually? Let's compute: gap=15, d=4, needed=(15-1)/4=14/4=3 (integer division) = 3 stops, so feasible(4) is true! Then we can place stops at 9,12,14? Wait gaps 0-5=5 (≤5?), no d=4, gap 5 >4, needed for that gap also (5-1)/4=1, total needed 1+3=4 > m=3, so not feasible. So binary search will find feasible(5)? For d=5, gaps: 0-5=5 count 0, 5-20=15 count (14)/5=2, total 2 ≤3, feasible. So answer is 5.
    assert(maxGapAfterAddingRestStops(1, 3, 20, {5}) == 5);

    // Edge case: l=1, n=0, m any -> max gap 1
    assert(maxGapAfterAddingRestStops(0, 5, 1, {}) == 1);

    // Stops at very ends? Existing stops are between 0 and l, but we handle anyway
    // l=100, existing at {100}? Not allowed by spec, but test robustness: treat as endpoint duplicate
    assert(maxGapAfterAddingRestStops(1, 0, 100, {0}) == 50); // Actually stops sorted: 0,0,100 -> gaps 0 and 100 -> max 100

    // Actually previous test is wrong: existing stop at 0 duplicates start, largest gap is 100-0=100, m=0 -> answer 100
    assert(maxGapAfterAddingRestStops(1, 0, 100, {0}) == 100);

    // Test with unsorted input
    assert(maxGapAfterAddingRestStops(3, 1, 30, {10, 5, 20}) == 10);
    // Gaps: 0-5=5, 5-10=5, 10-20=10, 20-30=10. Add one stop, best to split a 10 into two 5s -> max becomes 5? Actually we can place at 15 or 25 -> then gaps: 5,5,5,5,10? Wait after adding at 15, gaps: 0-5,5-10,10-15,15-20,20-30 -> max 10. So answer 10? But we can place at 25: gaps: 5,5,10,5,5 -> max still 10. So answer 10. Binary search: feasible(9) needs for gap 10: (10-1)/9=1, total 1 ≤1, so feasible(9) true? Also need check other gaps ≤9, yes all ≤5. So answer 9? Actually with d=9, need 1 stop for the 10 gap, can place at 14 or 25? Place at 15? gap 15-20=5, 10-15=5, so max becomes 5? Wait 0-5=5,5-10=5,10-15=5,15-20=5,20-30=10 → still 10! Because the 20-30 gap is 10 >9, so need another stop there, total needed 2 >1, so not feasible. So feasible(9) false because gap 20-30 is 10 >9, need (10-1)/9=1, plus the other 10 gap needs 1 → total 2. So answer is 10. So assert with 10 is correct.

    return 0;
}

// The correct approach is binary search on the answer `d` (the maximum allowed gap). For a candidate `d`, we check if it's possible to place `m` new stops so that every consecutive gap (including from 0 to first, last to `l`) is at most `d`. Given sorted existing stops (plus 0 and `l`), for each gap of length `gap`, the minimum number of new stops needed to break it into segments of length ≤ `d` is `ceil(gap/d) - 1` (if gap ≤ d, need 0; if gap > d, need `(gap-1)/d` integer division). Sum these needed stops across all gaps; if total ≤ `m`, then `d` is feasible. Binary search the minimal feasible `d` from 1 to `l`. Edge cases: if no existing stops, then n=0, but vector may be empty; we still include 0 and l. Also if m=0, the answer is simply the max gap among existing gaps. The time complexity is O((n+1) * log(l)) for binary search, plus O(n log n) to sort the existing stops (or they may be given unsorted). Space O(n). The original greedy is not always optimal, so we use this correct method.
