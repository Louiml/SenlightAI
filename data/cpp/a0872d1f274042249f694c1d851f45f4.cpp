// Write a C++ function that, given an integer `X` representing available time in minutes and a fixed vector of activity durations `{1, 2, 4}` (each activity can be selected at most once), returns the maximum number of distinct activities that can be completed whose total duration does not exceed `X`. If no activity fits, return `0`. The function should handle any positive integer `X`, including values smaller than the smallest duration, and must not modify the input vector.

The problem is a small subset‑sum/count maximization problem over a fixed set of three items with durations 1, 2, and 4 minutes. Since there are only three items, the total number of subsets is \(2^3 = 8\). The simplest robust approach is to enumerate all subsets using a bitmask over the indices of the activity durations. For each mask from 0 to 7, compute the total duration and the count of selected items; if the total duration is ≤ `X`, update the maximum count. This brute‑force method handles all edge cases naturally: if `X` is 0, the empty subset (count 0) is valid; if `X` is very large (e.g., ≥7), the maximum count is 3 since all activities can be selected. If `X` is small (e.g., 1), only the activity of duration 1 fits, giving count 1. Time complexity is \(O(2^3 \cdot 3) = O(1)\) because the size of the activity list is fixed at 3; space complexity is \(O(1)\) extra storage. The function should accept the available time `X` as an `int` and return an `int`; it may optionally take a vector of durations by `const` reference for generality.

#include <vector>
#include <algorithm>

// Returns the maximum number of activities from the given durations
// whose total time does not exceed available_time.
int maxActivities(const std::vector<int>& durations, int available_time) {
    int n = static_cast<int>(durations.size());
    int best = 0;
    // Enumerate all subsets via bitmask
    for (int mask = 0; mask < (1 << n); ++mask) {
        int total = 0;
        int count = 0;
        for (int i = 0; i < n; ++i) {
            if (mask & (1 << i)) {
                total += durations[i];
                ++count;
            }
        }
        if (total <= available_time) {
            best = std::max(best, count);
        }
    }
    return best;
}

#include <cassert>
#include <vector>

int maxActivities(const std::vector<int>& durations, int available_time);

int main() {
    std::vector<int> act = {1, 2, 4};

    // Empty selection allowed
    assert(maxActivities(act, 0) == 0);
    // Only smallest activity fits
    assert(maxActivities(act, 1) == 1);
    // 1+2 fits
    assert(maxActivities(act, 3) == 2);
    // 2+4 fits but not all three
    assert(maxActivities(act, 6) == 2);
    // All three fit exactly
    assert(maxActivities(act, 7) == 3);
    // Large time, all fit
    assert(maxActivities(act, 100) == 3);
    // Negative time (not allowed but safe)
    assert(maxActivities(act, -5) == 0);
    // Custom durations
    std::vector<int> custom = {3, 5, 8};
    assert(maxActivities(custom, 10) == 2); // 3+5=8
    assert(maxActivities(custom, 15) == 2); // 5+8=13, 3+5+8=16 too big
    return 0;
}
