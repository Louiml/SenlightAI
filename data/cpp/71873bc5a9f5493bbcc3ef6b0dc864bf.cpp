Write a C++ function `countUncoveredDays` that takes two integers `n` (number of time intervals) and `m` (maximum day index), along with a vector of pairs `intervals` where each pair `(a, b)` represents a closed interval of covered days from day `a` to day `b` (both inclusive, with `1 ≤ a ≤ b ≤ m`). The function should return a vector of all day indices from 1 to `m` that are NOT covered by any interval. Days are considered covered if they lie inside at least one interval. The output vector must be sorted in ascending order. Handle the case where `n` can be 0, and intervals may overlap arbitrarily. The function should not modify the input vector.
// The problem is a classic interval coverage problem on a discrete line of days from 1 to `m`. The straightforward approach is to use a frequency array of size `m+1` (or `m` if using 0-based indexing) initialized to zero. For each interval `(a, b)`, we iterate from `a` to `b` and increment the count for each day. After processing all intervals, any day with a count of zero is uncovered. This works because `m` is typically small enough that `O(m)` per interval is acceptable, but if `n` and `m` are large (say up to 10^5 each), the total time is `O(n*m)` which could be too slow; however, since the task is inspired by the given snippet (which uses this exact method), we stick with this simple approach. Important edge cases: (1) empty intervals vector (n=0) means all days from 1 to `m` are uncovered; (2) intervals that fully overlap, so a day may get counted multiple times but still be covered; (3) `a` and `b` are inclusive, and `b` never exceeds `m`. Time complexity is `O(n*m)` in the worst case, and space complexity is `O(m)` for the frequency array plus `O(m)` for the result vector in the worst case.
#include <vector>
#include <utility>

// Count uncovered days: given n intervals [a,b] inclusive for days 1..m,
// return a sorted vector of all days not covered by any interval.
std::vector<int> countUncoveredDays(int n, int m, const std::vector<std::pair<int,int>>& intervals) {
    // Frequency array for days 1..m (index 0 unused for simplicity)
    std::vector<int> coverage(m + 1, 0);
    
    // Mark covered days by incrementing counts for each interval
    for (int i = 0; i < n; ++i) {
        int a = intervals[i].first;
        int b = intervals[i].second;
        for (int day = a; day <= b; ++day) {
            coverage[day]++;
        }
    }
    
    // Collect all days with zero coverage
    std::vector<int> result;
    for (int day = 1; day <= m; ++day) {
        if (coverage[day] == 0) {
            result.push_back(day);
        }
    }
    
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is defined above; here we test it.
int main() {
    // Test 1: Sample scenario, intervals [1,2] and [4,5], m=5 -> uncovered {3}
    std::vector<std::pair<int,int>> intervals1 = {{1,2}, {4,5}};
    assert(countUncoveredDays(2, 5, intervals1) == std::vector<int>{3});
    
    // Test 2: No intervals, m=4 -> all days uncovered {1,2,3,4}
    std::vector<std::pair<int,int>> intervals2 = {};
    assert(countUncoveredDays(0, 4, intervals2) == std::vector<int>({1,2,3,4}));
    
    // Test 3: Full coverage, m=3, intervals [1,3] -> empty result
    std::vector<std::pair<int,int>> intervals3 = {{1,3}};
    assert(countUncoveredDays(1, 3, intervals3) == std::vector<int>{});
    
    // Test 4: Overlapping intervals cover all, m=6, intervals [1,3] and [2,6] -> empty
    std::vector<std::pair<int,int>> intervals4 = {{1,3}, {2,6}};
    assert(countUncoveredDays(2, 6, intervals4) == std::vector<int>{});
    
    // Test 5: Overlapping intervals leaving gaps, m=8, intervals [2,4] and [3,5] -> uncovered {1,6,7,8}
    std::vector<std::pair<int,int>> intervals5 = {{2,4}, {3,5}};
    assert(countUncoveredDays(2, 8, intervals5) == std::vector<int>({1,6,7,8}));
    
    // Test 6: Single day interval, m=1 -> no uncovered
    std::vector<std::pair<int,int>> intervals6 = {{1,1}};
    assert(countUncoveredDays(1, 1, intervals6) == std::vector<int>{});
    
    // Test 7: Single day interval but m=2 -> uncovered {2}
    std::vector<std::pair<int,int>> intervals7 = {{1,1}};
    assert(countUncoveredDays(1, 2, intervals7) == std::vector<int>{2});
    
    // Test 8: Multiple overlapping with gaps, m=10, intervals [1,3],[2,4],[6,6],[8,9] -> uncovered {5,7,10}
    std::vector<std::pair<int,int>> intervals8 = {{1,3},{2,4},{6,6},{8,9}};
    assert(countUncoveredDays(4, 10, intervals8) == std::vector<int>({5,7,10}));
    
    return 0;
}
