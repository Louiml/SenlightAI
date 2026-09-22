Given `n` closed intervals `[l_i, r_i]` on the integer line, each with a weight `w_i`, and a function `f(x) = sum of weights of intervals covering point x`, write a C++ function `long long maximumWeightSumCovered(const std::vector<std::tuple<int,int,int>>& intervals)` that returns the maximum value of `f(x)` over all integer points `x` (including points inside and outside all intervals). Intervals are closed, meaning both endpoints are covered. Use 64-bit integers for sums, as weights and `n` can be large. The input may contain duplicate intervals and overlapping intervals arbitrarily.

The key idea is to avoid evaluating `f(x)` at every integer point (which could be huge). Instead, we observe that `f(x)` only changes at points where an interval starts or ends (or one past the end for closed intervals). We collect all relevant event points: each interval contributes `l_i` (start), `r_i + 1` (one past end), and also `l_i - 1` and `r_i + 1` with zero weight to ensure that boundaries are processed correctly. We sort these points. Sweep from left to right: maintain a running sum of weights of intervals that have started but not yet ended. At each distinct x-coordinate, we add all weights of intervals starting exactly at that x to the running sum. Then we process all end events up to and including that x: subtract their weights from the running sum, and crucially, after subtracting for a batch of endpoints, we update a variable `min_prefix_sum` that stores the minimum running sum seen so far after handling all endpoints up to that point. Then the current candidate `f(x)` is `current_sum - min_prefix_sum`? Actually, we need to be careful: The intended algorithm from the snippet is: sum of weights at point x = (total sum of all starting weights) minus (sum of weights of intervals that have ended before or at x). But we want maximum over x. The snippet does: sort all starts and all ends. It sweeps through sorted starts, adding their weights; when moving to a new distinct x, it processes all ends with `end_coord <= x`, accumulating their weights, and updates `mi` as the minimum value of `sub` after each batch of ends. Then `ans = max(ans, sum - mi)`. This works because `sum` is total weight of intervals that have started at any coordinate ≤ x, and `sub` is total weight of those that have also ended before or at x. So `sum - sub` = weight of intervals covering x (since intervals that started but not ended by x cover x, and intervals that started and ended before x don't cover x). To maximize `sum - sub`, we want `sum` high and `sub` low; the snippet maximizes `sum - min(sub_so_far)`. Since `sub` is non-decreasing as x increases, `mi` is the minimum prefix sum of `sub` seen up to current x, which corresponds to the earliest point in the sweep where `sub` was minimal relative to `sum`. This technique correctly finds the maximum difference. Edge cases: negative weights are allowed, so the maximum could be at a point where no intervals cover (i.e., 0). Also, handle duplicate coordinates carefully by batching. Time complexity: O(n log n) due to sorting, with O(n) memory. Space complexity O(n). Use `long long` for sums.

#include <bits/stdc++.h>

// Compute the maximum total weight covered by any integer point x
// given intervals [l, r] with weight w.
long long maximumWeightSumCovered(const std::vector<std::tuple<int, int, int>>& intervals) {
    if (intervals.empty()) return 0;

    std::vector<std::pair<int, long long>> starts; // (coordinate, weight)
    std::vector<std::pair<int, long long>> ends;   // (coordinate+1, weight)

    for (const auto& [l, r, w] : intervals) {
        starts.emplace_back(l, static_cast<long long>(w));
        ends.emplace_back(r + 1, static_cast<long long>(w));
        // Include zero-weight events to process boundaries completely
        starts.emplace_back(l - 1, 0);
        starts.emplace_back(r + 1, 0);
    }

    std::sort(starts.begin(), starts.end());
    std::sort(ends.begin(), ends.end());

    long long total_started = 0;
    long long total_ended = 0;
    long long min_ended_prefix = 0; // minimum of total_ended seen so far
    long long answer = 0;
    size_t end_index = 0;
    const size_t starts_size = starts.size();
    const size_t ends_size = ends.size();

    for (size_t i = 0; i < starts_size; ++i) {
        total_started += starts[i].second;

        // If next start has same coordinate, defer processing until later
        if (i + 1 < starts_size && starts[i + 1].first == starts[i].first) {
            continue;
        }

        int current_x = starts[i].first;

        // Process all ends with coordinate <= current_x
        while (end_index < ends_size && ends[end_index].first <= current_x) {
            total_ended += ends[end_index].second;

            // After adding this end, update min prefix sum, but only when
            // we finish a batch of identical end coordinates.
            if (end_index + 1 == ends_size || ends[end_index + 1].first != ends[end_index].first) {
                min_ended_prefix = std::min(min_ended_prefix, total_ended);
            }
            ++end_index;
        }

        answer = std::max(answer, total_started - min_ended_prefix);
    }

    return answer;
}

#include <cassert>
#include <vector>
#include <tuple>

// Assume maximumWeightSumCovered is declared above

int main() {
    // Single interval [1, 3] weight 5
    std::vector<std::tuple<int,int,int>> test1 = {{1,3,5}};
    assert(maximumWeightSumCovered(test1) == 5);

    // Two overlapping intervals [1,5] w=2 and [3,7] w=3, max at x=3..5 sum=5
    std::vector<std::tuple<int,int,int>> test2 = {{1,5,2}, {3,7,3}};
    assert(maximumWeightSumCovered(test2) == 5);

    // Non-overlapping: [1,2] w=10 and [10,12] w=7, max 10 at x=1 or 2
    std::vector<std::tuple<int,int,int>> test3 = {{1,2,10}, {10,12,7}};
    assert(maximumWeightSumCovered(test3) == 10);

    // Negative weights: [-5,0] w=-2 and [ -1,1] w=3, max 1 at x=0
    std::vector<std::tuple<int,int,int>> test4 = {{-5,0,-2}, {-1,1,3}};
    assert(maximumWeightSumCovered(test4) == 1);

    // All negative: [-2,-1] w=-1, [0,1] w=-2, max 0 (no coverage at other points)
    std::vector<std::tuple<int,int,int>> test5 = {{-2,-1,-1}, {0,1,-2}};
    assert(maximumWeightSumCovered(test5) == 0);

    // Duplicate intervals: [1,4] w=3 twice, max 6
    std::vector<std::tuple<int,int,int>> test6 = {{1,4,3}, {1,4,3}};
    assert(maximumWeightSumCovered(test6) == 6);

    // Edge case: same start and end, single point interval [2,2] w=7
    std::vector<std::tuple<int,int,int>> test7 = {{2,2,7}};
    assert(maximumWeightSumCovered(test7) == 7);

    // Large values to test 64-bit: weight 1e9 at [0, 0] twice, sum 2e9
    std::vector<std::tuple<int,int,int>> test8 = {{0,0,1000000000}, {0,0,1000000000}};
    assert(maximumWeightSumCovered(test8) == 2000000000LL);

    // Empty input returns 0
    std::vector<std::tuple<int,int,int>> test9 = {};
    assert(maximumWeightSumCovered(test9) == 0);

    // Intervals covering a wide range with gaps: [0,2] w=1, [5,7] w=2, [1,6] w=3
    // Max at x=1..2: 1+3=4; x=5..6: 2+3=5; so answer 5
    std::vector<std::tuple<int,int,int>> test10 = {{0,2,1}, {5,7,2}, {1,6,3}};
    assert(maximumWeightSumCovered(test10) == 5);

    return 0;
}
