/*
You are given `n` workstations. Workstation `i` can process at most `a[i]` units of work, and each unit at that workstation requires `t[i]` seconds. You have a total available time of `T` seconds. You must process work sequentially across the workstations: for each workstation, you may process any number of units (including a fractional number) from `0` to `a[i]`, but once you move on from a workstation, you may never return to it. You can choose the order in which you visit the workstations. Write a C++ function `bool canAchieveUnits(const std::vector<long long>& a, const std::vector<long long>& t, long double T, long double U)` that returns `true` if there exists an ordering and a choice of (possibly fractional) units for each workstation such that the total number of units processed is exactly `U` and the total processing time does not exceed `T` (the total time used will equal exactly `T` if you process exactly `U` units, provided the `U` is within the feasible range; you may use less than `T` if the last workstation’s allocation makes the total time less, but the definition here requires you to use exactly `T`? To match the underlying code, assume you must use exactly `T` seconds in total, i.e., you fill the available time exactly). In other words, determine if `U` lies between the minimum and maximum total units that can be processed given a total exact time budget `T`, where processing a workstation in order from smallest `t[i]` to largest maximizes units, and from largest to smallest minimizes units. The function should return `true` when `U` is within that achievable interval (including the endpoints), and `false` otherwise.
*/

#include <bits/stdc++.h>

// Given arrays a (max units) and t (seconds per unit) and an exact total time T,
// return true if exactly U units can be processed by visiting workstations in some order,
// processing a (possibly fractional) number of units per workstation without revisiting.
bool canAchieveUnits(const std::vector<long long>& a, const std::vector<long long>& t, long double T, long double U) {
    const int n = static_cast<int>(a.size());
    if (n == 0) return (U == 0.0L && T == 0.0L);

    // Build pairs (time_per_unit, max_units) and sort by time_per_unit.
    std::vector<std::pair<long long, long long>> items;
    items.reserve(n);
    for (int i = 0; i < n; ++i) {
        items.emplace_back(t[i], a[i]);
    }
    std::sort(items.begin(), items.end());

    // Compute maximum units: visit in increasing t[i].
    long double maxUnits = 0.0L;
    long double remMax = T;
    for (int i = 0; i < n; ++i) {
        long long ti = items[i].first;
        long long ai = items[i].second;
        if (remMax <= 0.0L) break;
        long double take = std::min(static_cast<long double>(ai), remMax / static_cast<long double>(ti));
        maxUnits += take;
        remMax -= take * static_cast<long double>(ti);
    }

    // Compute minimum units: visit in decreasing t[i].
    long double minUnits = 0.0L;
    long double remMin = T;
    for (int i = n - 1; i >= 0; --i) {
        long long ti = items[i].first;
        long long ai = items[i].second;
        if (remMin <= 0.0L) break;
        long double take = std::min(static_cast<long double>(ai), remMin / static_cast<long double>(ti));
        minUnits += take;
        remMin -= take * static_cast<long double>(ti);
    }

    const long double eps = 1e-12L;
    // Check whether U lies in [minUnits, maxUnits], allowing for floating rounding.
    return (U + eps >= minUnits) && (U - eps <= maxUnits);
}

#include <cassert>
#include <vector>
#include <cmath>

// Assume the solution function is declared above.

int main() {
    // Basic case: two workstations, T=3
    std::vector<long long> a1 = {10, 20};
    std::vector<long long> t1 = {1, 2};
    assert(canAchieveUnits(a1, t1, 3.0L, 3.0L));   // max = 3
    assert(canAchieveUnits(a1, t1, 3.0L, 1.5L));   // min = 1.5
    assert(canAchieveUnits(a1, t1, 3.0L, 2.25L));  // between
    assert(!canAchieveUnits(a1, t1, 3.0L, 1.0L));  // below min
    assert(!canAchieveUnits(a1, t1, 3.0L, 3.5L));  // above max

    // Single workstation
    std::vector<long long> a2 = {5};
    std::vector<long long> t2 = {2};
    assert(canAchieveUnits(a2, t2, 1.0L, 0.5L));
    assert(canAchieveUnits(a2, t2, 5.0L, 2.5L));
    assert(!canAchieveUnits(a2, t2, 1.0L, 0.75L));

    // Edge: T=0
    std::vector<long long> a3 = {3, 4};
    std::vector<long long> t3 = {1, 2};
    assert(canAchieveUnits(a3, t3, 0.0L, 0.0L));
    assert(!canAchieveUnits(a3, t3, 0.0L, 0.1L));

    // Identical t: min == max
    std::vector<long long> a4 = {7, 7};
    std::vector<long long> t4 = {3, 3};
    assert(canAchieveUnits(a4, t4, 6.0L, 2.0L));
    assert(!canAchieveUnits(a4, t4, 6.0L, 2.5L));

    // All workstations full capacity with enough time
    std::vector<long long> a5 = {2, 3};
    std::vector<long long> t5 = {1, 1};
    // Total full units = 5, time = 5, so with T=5 only 5 units possible
    assert(canAchieveUnits(a5, t5, 5.0L, 5.0L));
    assert(!canAchieveUnits(a5, t5, 5.0L, 4.9L)); // because T must be used exactly, 4.9 would leave time unused? Actually if you take 4.9 units at t=1, time used=4.9, not 5, so not allowed because T is exact? The problem says "must use exactly T". So it's false.

    // Larger T than total maximum time (sum of a[i]*t[i]) leaves leftover time that cannot be filled
    std::vector<long long> a6 = {1, 1};
    std::vector<long long> t6 = {1, 1};
    // Max time = 2, so with T=3 impossible (cannot use exactly 3)
    assert(!canAchieveUnits(a6, t6, 3.0L, 2.0L));
    assert(!canAchieveUnits(a6, t6, 3.0L, 0.0L));

    return 0;
}

// The key observation is that because you can process each workstation partially and you never revisit a workstation, the set of achievable total unit counts for a fixed total time `T` is a continuous interval. To maximize the total number of units processed, you should visit workstations in increasing order of `t[i]` (i.e., time per unit), because that lets you spend the available time on the cheapest units first. Starting with `remaining = T`, for each workstation in that order you take `take = min(a[i], remaining / t[i])` units, add `take` to the running total, and subtract `take * t[i]` from `remaining`. The final total is the maximum possible units. To minimize the total units, use the reverse order (decreasing `t[i]`) and apply the same greedy procedure. The achievable region is exactly the closed interval `[minUnits, maxUnits]`. Therefore, after computing these two extremes, the answer is `minUnits <= U <= maxUnits` (with a small epsilon for floating-point comparison). Edge cases: if `T` is 0, the only achievable `U` is 0 (unless some `t[i]` is 0, but we assume positive `t[i]`). If a workstation has `t[i]` extremely large, the algorithm still works with long double. Time complexity is `O(n log n)` for sorting, and `O(n)` for the two passes; space complexity is `O(n)` for the sorted copy.
