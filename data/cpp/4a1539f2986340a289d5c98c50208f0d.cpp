Write a C++ function `avoidFlood` that takes a vector of integers `rains` (length n) representing daily rainfall, where `rains[i] > 0` means a lake with that ID is full and will flood if it is already full, and `rains[i] == 0` means a dry day on which you can empty any one full lake. The function must return a vector `answer` of length n where for each dry day (0) you choose which lake to empty (put its ID in `answer[i]`), and for each rainy day you put `-1` in `answer[i]`. If no sequence of choices prevents any flood (i.e., no lake is ever filled when it is already full), return an empty vector. On sunny days when no lake is full, you may choose to empty a virtual lake by outputting `1`. Otherwise, output the ID of the lake you empty (which must be currently full) or `1` if you prefer to do nothing (though you must pick some lake; `1` is allowed only if lake 1 is full, but for simplicity you may always output `1` when there are no full lakes). Implement the function with the exact behavior: each lake can only be emptied once per dry day, and you cannot empty a lake that is not full at that moment. The goal is to avoid any lake flooding, meaning on a rainy day for a lake already full, you must have emptied it on a previous dry day after it was last filled.
The problem is a classic greedy scheduling problem. We process days sequentially. We maintain a map `lastRain` from lake ID to the last day index that lake was filled (or initially never filled). We also maintain a set `dryDays` of indices of dry days (0) that have not yet been used. For each day `i`:
- If `rains[i] == 0`, insert index `i` into `dryDays`.
- Else (`rains[i] > 0`), let `lake = rains[i]`.
  - If `lake` is not in `lastRain` (i.e., not currently full), we simply record that it was last filled on day `i` by setting `lastRain[lake] = i`, and set `answer[i] = -1`.
  - If `lake` is already full (i.e., in `lastRain`), we need to find a dry day after the day it was last filled (`lastRain[lake]`) to empty it. Use `dryDays.lower_bound(lastRain[lake])` to find the earliest unused dry day strictly after the last fill. If no such dry day exists, we cannot prevent a flood, so return an empty vector. Otherwise, assign `answer[that dryDay] = lake` (empty that lake), remove that dry day from `dryDays`, update `lastRain[lake] = i` (now filled again), and set `answer[i] = -1`.
After processing all days, any dry day that was not used remains `0` in `answer`; we replace all such zeros with `1` (since we can empty lake 1 if it were full; but if lake 1 is not full, it's still acceptable per problem constraints—we just pick any lake, and the problem allows using `1` as a placeholder). Edge cases: multiple lakes, consecutive rainy days of same lake, dry days before any lake filled (we can output `1`), and no valid sequence returns empty. Time complexity: O(n log n) due to set operations and map lookups; space O(n).
#include <vector>
#include <map>
#include <set>
#include <algorithm>

// Given a rainfall schedule, return a schedule of which lakes to empty on dry days
// to prevent any lake from flooding. If impossible, return an empty vector.
std::vector<int> avoidFlood(const std::vector<int>& rains) {
    int n = static_cast<int>(rains.size());
    std::map<int, int> lastFill;      // lake ID -> last day it was filled
    std::set<int> dryDays;            // indices of unused dry days (0)
    std::vector<int> answer(n, 0);

    for (int i = 0; i < n; ++i) {
        if (rains[i] == 0) {
            dryDays.insert(i);
        } else {
            int lake = rains[i];
            auto it = lastFill.find(lake);
            if (it == lastFill.end()) {
                // Lake not currently full, so no flood risk.
                lastFill[lake] = i;
                answer[i] = -1;
            } else {
                int lastDay = it->second;
                // Find earliest unused dry day after the last fill.
                auto dryIt = dryDays.lower_bound(lastDay + 1);
                if (dryIt == dryDays.end()) {
                    // No dry day available to empty the lake before it floods again.
                    return {};  // empty vector indicates failure
                }
                // Use that dry day to empty this lake.
                answer[*dryIt] = lake;
                dryDays.erase(dryIt);
                // Update the last fill day for this lake.
                it->second = i;
                answer[i] = -1;
            }
        }
    }

    // For any unused dry days, assign a placeholder value (1) per problem constraints.
    for (int& val : answer) {
        if (val == 0) {
            val = 1;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

int main() {
    // Example 1: basic case
    std::vector<int> rains1 = {1, 2, 0, 0, 2, 1};
    std::vector<int> result1 = avoidFlood(rains1);
    assert(result1.size() == 6);
    // On day 2 (0-indexed) we empty lake 2, on day 3 empty lake 1.
    assert(result1[0] == -1 && result1[1] == -1 && result1[2] == 2 && result1[3] == 1 && result1[4] == -1 && result1[5] == -1);

    // Example 2: impossible (lake 1 rains twice with no dry day in between)
    std::vector<int> rains2 = {1, 1};
    assert(avoidFlood(rains2).empty());

    // Example 3: dry day before any fill -> output 1
    std::vector<int> rains3 = {0, 1, 0};
    std::vector<int> result3 = avoidFlood(rains3);
    assert(result3.size() == 3);
    assert(result3[0] == 1 && result3[1] == -1 && result3[2] == 1);

    // Example 4: multiple dry days, we must choose the earliest after last fill
    std::vector<int> rains4 = {1, 0, 1, 0, 1};
    std::vector<int> result4 = avoidFlood(rains4);
    assert(result4.size() == 5);
    // Day 1 empty lake1, day 3 empty lake1 again.
    assert(result4[0] == -1 && result4[1] == 1 && result4[2] == -1 && result4[3] == 1 && result4[4] == -1);

    // Example 5: all dry days
    std::vector<int> rains5 = {0, 0, 0};
    std::vector<int> result5 = avoidFlood(rains5);
    assert(result5 == std::vector<int>({1, 1, 1}));

    // Example 6: no dry days but lakes are distinct
    std::vector<int> rains6 = {1, 2, 3};
    std::vector<int> result6 = avoidFlood(rains6);
    assert(result6 == std::vector<int>({-1, -1, -1}));

    // Example 7: lake repeated after a dry day that is too early
    std::vector<int> rains7 = {0, 1, 1};  // dry day is before first fill, cannot help
    assert(avoidFlood(rains7).empty());

    // Example 8: lake repeated after a dry day that is just right
    std::vector<int> rains8 = {1, 0, 1};
    std::vector<int> result8 = avoidFlood(rains8);
    assert(result8[0] == -1 && result8[1] == 1 && result8[2] == -1);
    return 0;
}
