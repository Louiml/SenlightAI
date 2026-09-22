// Write a C++ function `int bestRideTime(int n, int s, const std::vector<std::pair<int,int>>& rides)` that, given the number of rides `n`, a current time `s` (in minutes after the hour, from 0 to 59), and a list of rides where each pair `(hour, minute)` represents the scheduled departure time of a ride (hour between 0 and 23, minute between 0 and 59), returns the maximum number of minutes you must wait until a ride departs, considering that you can catch a ride if you arrive strictly before its departure hour, or if you arrive at exactly the same hour and the ride’s minute is exactly 0 (i.e., the ride departs on the hour). If you cannot catch any ride, return -1. The waiting time in minutes is computed as the difference between the ride’s total minutes from midnight and your arrival total minutes from midnight (written as `s` minutes after some fixed hour, assume you arrive at hour 0?—actually assume you arrive at hour 0 with `s` minutes past midnight, i.e., total minutes = `s`, and each ride’s total minutes = `hour*60 + minute`). If you arrive exactly on the hour (minute 0) and the ride is at that same hour minute 0, the wait is 0. The function must handle up to 100,000 rides efficiently.

// The logic mirrors the snippet: for each ride, the condition to catch it is `(s > x)` where `x` is the hour, or `(s == x && y == 0)` where `y` is the minute. But note the snippet uses `s` as minutes after the hour (0–59) and `x` as the hour. In our adapted problem, we interpret `s` as the current minute within hour 0 (so total minutes = `s`), and each ride’s hour and minute are given. The waiting time in the snippet is `(100 - y) % 100`, which equals `(60 - y) % 60` for minutes. In our formulation, we compute wait as `(hour*60 + minute) - s`. However, to keep consistency with the snippet’s semantics, we must enforce that we only consider rides where the total minutes of the ride is greater than `s`, or equal if minute=0 and hour=0? Actually the snippet’s condition says catch if (s > x) OR (s==x && y==0). This means if `s` is greater than the hour, you catch regardless of minute; if `s` equals the hour, you only catch if minute is 0. This is a simplified model ignoring real times beyond an hour. To make it sensible, we reinterpret: `s` is the current hour? But the snippet reads `s` as a single integer, and compares with `x` (hour). So we must design a task that matches exactly: Given your current hour `s` (0–23) and a list of departures `(hour, minute)`, you can catch a ride if your current hour is strictly less than the ride’s hour, OR if your current hour equals the ride’s hour and the ride’s minute is 0. The waiting time is computed as `(hour - s)*60 + minute` if hour>s, and as `minute` if hour==s and minute>0? But the snippet only allows minute=0 in the equal-hour case, so wait is 0. For hour>s, wait is `(hour-s)*60 + minute`. However the snippet uses `(100 - y) % 100` which for minutes yields `(60 - y)%60` returning 0 when y=0 and 60-y otherwise—but that is not a realistic wait. To stay faithful, we design the task such that the waiting time is simply `(100 - minute) % 100` meaning if minute is 0 wait=0, else wait=100-minute? That is odd. Let’s redefine cleanly: The task will be: given current time in minutes after midnight (`s`), and a list of rides each with departure minutes after midnight (`t`), you can catch a ride if `s < t` OR (`s == t` and the ride’s minute is 0? No—better to make it: you can catch if `s <= t` but if equal you must have minute=0? The snippet’s condition `(s > x)` with `x` being hour suggests a coarse comparison. I’ll create a task that exactly mirrors the snippet’s logic: we have a current hour `s` (0–23) and a list of `(hour, minute)`. You can catch a ride if `s < hour` OR (`s == hour` and `minute == 0`). The waiting time in minutes is `(hour - s)*60 + minute`. If none catch, return -1. This makes sense and matches the condition. Edge cases: if multiple rides are catchable, return the maximum wait. Complexity: O(n) time, O(1) space.

#include <vector>
#include <algorithm>

// Returns the maximum waiting time in minutes for a catchable ride, or -1 if none.
// A ride (hour, minute) is catchable if current hour s is strictly less than hour,
// or if s equals hour and minute is 0. Wait time = (hour - s) * 60 + minute.
int bestRideTime(int n, int s, const std::vector<std::pair<int,int>>& rides) {
    int best_wait = -1;
    for (int i = 0; i < n; ++i) {
        int hour = rides[i].first;
        int minute = rides[i].second;
        if (s < hour || (s == hour && minute == 0)) {
            int wait = (hour - s) * 60 + minute;
            if (wait > best_wait) {
                best_wait = wait;
            }
        }
    }
    return best_wait;
}

#include <cassert>
#include <vector>

int bestRideTime(int n, int s, const std::vector<std::pair<int,int>>& rides);

int main() {
    // Basic case: rides later today
    assert(bestRideTime(3, 10, {{10, 30}, {12, 0}, {9, 45}}) == 120); // wait for 12:00 -> 120 min
    // Exact hour match with minute=0 is catchable
    assert(bestRideTime(2, 10, {{10, 0}, {10, 30}}) == 0); // 10:00 wait =0, 10:30 not catchable (minute!=0)
    // No catchable rides
    assert(bestRideTime(2, 10, {{10, 30}, {5, 0}}) == -1);
    // Multiple catchable, maximum wait
    assert(bestRideTime(4, 8, {{9, 0}, {10, 15}, {7, 0}, {8, 0}}) == 135); // 10:15 -> 135
    // Edge: s equals hour and minute 0, wait 0
    assert(bestRideTime(1, 5, {{5, 0}}) == 0);
    // Edge: s greater than hour but minutes large
    assert(bestRideTime(1, 20, {{21, 59}}) == 119);
    // Large n test
    std::vector<std::pair<int,int>> many;
    for (int i = 0; i < 100000; ++i) many.push_back({23, 59});
    assert(bestRideTime(100000, 0, many) == 1439);
    return 0;
}
