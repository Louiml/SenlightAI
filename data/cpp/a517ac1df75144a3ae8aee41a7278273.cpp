Write a C++ function `vector<int> scheduleGnomes(const vector<int>& careTimes, const vector<int>& sleepTimes)` that takes two arrays of equal length `n` (1 ≤ n ≤ 10^5, values positive integers) representing the time needed to care for each gnome and the maximum permissible time after that gnome's care is finished before the gnome must fall asleep. The function must return the indices (1‑based) in the order in which the gnomes should be cared for so that each gnome falls asleep within its allowed `sleepTime` after its care finishes. Specifically, if the gnomes are processed in order `p[0], p[1], ..., p[n-1]`, then for each gnome at position `i`, the total care time of all *previously* processed gnomes (i.e., before this gnome) plus this gnome's own care time must be strictly less than `sleepTime[p[i]]`. If no such ordering exists, return an empty vector. If multiple orderings exist, any valid one is acceptable, but the ordering must be deterministic, which the greedy algorithm below guarantees.

This is a greedy scheduling problem with a twist: the deadline for each job is not absolute but relative to the summation of all care times before and including that job. The key observation is that we should process gnomes in decreasing order of `(careTime + sleepTime)`. Intuitively, gnomes with larger `careTime + sleepTime` are more flexible because they can tolerate being started later, so we schedule them as early as possible to leave the tighter ones for later. After sorting by that key descending, we simulate the process: maintain `totalCareTime` as the sum of care times of all gnomes *not yet processed* (initialized to the sum of all care times). For each gnome in the sorted order, subtract its own `careTime` from `totalCareTime` (now `totalCareTime` equals the sum of care times of gnomes scheduled after this one). The condition `sleepTime > totalCareTime` must hold; otherwise, this gnome would have to fall asleep before the subsequent gnomes' care finishes, which is impossible. If any gnome violates this, no ordering exists. The greedy choice is provably optimal because if a valid order exists, sorting by `(careTime + sleepTime)` descending preserves feasibility (exchange argument). Time complexity is O(n log n) for sorting, space O(n) for the output. Edge case: `n=1` works if `sleepTime[0] > 0`, since `totalCareTime` after subtracting becomes 0 and `sleepTime > 0` holds.

#include <vector>
#include <algorithm>
#include <numeric>

// Returns the order (1-based indices) of gnomes satisfying the sleep constraints,
// or an empty vector if impossible.
std::vector<int> scheduleGnomes(const std::vector<int>& careTimes, const std::vector<int>& sleepTimes) {
    int n = static_cast<int>(careTimes.size());
    if (n == 0) return {};
    
    // Create a vector of indices and sort by careTime + sleepTime descending.
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return careTimes[a] + sleepTimes[a] > careTimes[b] + sleepTimes[b];
    });
    
    // totalCareTime is the sum of care times of gnomes not yet processed.
    long long totalCareTime = std::accumulate(careTimes.begin(), careTimes.end(), 0LL);
    
    std::vector<int> result;
    result.reserve(n);
    
    for (int idx : order) {
        totalCareTime -= careTimes[idx];
        if (sleepTimes[idx] <= totalCareTime) {
            return {};
        }
        result.push_back(idx + 1); // 1-based index
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume scheduleGnomes is defined above (not repeated here for brevity).

int main() {
    // Example 1: Simple valid case
    {
        std::vector<int> care = {1, 2, 3};
        std::vector<int> sleep = {4, 5, 6};
        auto res = scheduleGnomes(care, sleep);
        assert(res.size() == 3);
        // Check validity: compute cumulative care and verify each sleep constraint
        long long sum = 0;
        for (size_t i = 0; i < res.size(); ++i) {
            int idx = res[i] - 1;
            sum += care[idx];
            assert(sleep[idx] > sum - care[idx]); // condition: sleep > totalCare after this gnome
        }
    }

    // Example 2: Impossible case (first gnome's sleep too short)
    {
        std::vector<int> care = {5, 1};
        std::vector<int> sleep = {1, 100};
        auto res = scheduleGnomes(care, sleep);
        assert(res.empty());
    }

    // Example 3: Single gnome
    {
        std::vector<int> care = {10};
        std::vector<int> sleep = {11};
        auto res = scheduleGnomes(care, sleep);
        assert(res.size() == 1 && res[0] == 1);
    }

    // Example 4: Single impossible gnome
    {
        std::vector<int> care = {10};
        std::vector<int> sleep = {10};
        auto res = scheduleGnomes(care, sleep);
        assert(res.empty());
    }

    // Example 5: Larger valid case with many gnomes
    {
        std::vector<int> care = {1, 1, 1, 1, 1};
        std::vector<int> sleep = {5, 5, 5, 5, 5};
        auto res = scheduleGnomes(care, sleep);
        assert(res.size() == 5);
        long long sum = 0;
        for (int idx1 : res) {
            int idx = idx1 - 1;
            sum += care[idx];
            assert(sleep[idx] > sum - care[idx]);
        }
    }

    // Example 6: Edge with equal keys, still valid
    {
        std::vector<int> care = {2, 2, 2};
        std::vector<int> sleep = {4, 4, 4};
        auto res = scheduleGnomes(care, sleep);
        assert(res.size() == 3);
        long long sum = 0;
        for (int idx1 : res) {
            int idx = idx1 - 1;
            sum += care[idx];
            assert(sleep[idx] > sum - care[idx]);
        }
    }

    return 0;
}
