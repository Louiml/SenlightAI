/*
You are given `n` time intervals `[a_i, b_i]` (with `a_i < b_i`) and three cost values `x`, `y`, `z` (all positive integers). At any moment in time, the number of intervals that are "active" (i.e., started but not yet finished) changes. Initially, at time `-∞`, all `n` intervals are "not yet started". Over time, each interval transitions from "not yet started" to "active" at its start time `a_i`, then from "active" to "finished" at its end time `b_i`. The total "value" at any moment is `x * (number of not‑started intervals) + y * (number of active intervals) + z * (number of finished intervals)`. Write a C++ function `long long maximumValue(int n, int x, int y, int z, const std::vector<pair<int,int>>& intervals)` that returns the maximum possible total value over all times. The intervals are not necessarily sorted, and the end time of one interval may coincide with the start time of another. At a tie (i.e., an interval ends and another starts at the same instant), treat the ending event as happening exactly at that time and the starting event as happening an infinitesimally later instant — so in the sorted sweep, process all end events before start events at the same coordinate. (This is the opposite of the original snippet's ordering, which processed starts first — your solution must correctly handle ties by processing ends first to match the intended continuous-time semantics.) Note that the maximum may occur at `t = -∞` (all not started) or at `t = +∞` (all finished), so include those initial and final states in your sweep.
*/
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

// Returns the maximum possible total value over all times.
// Each interval is {start, end} with start < end.
long long maximumValue(int n, int x, int y, int z, const vector<pair<int,int>>& intervals) {
    // Events: (time, type) where type = 0 for end, 1 for start.
    // We process ends before starts at the same time.
    vector<pair<int,int>> events;
    events.reserve(2 * n);
    for (const auto& iv : intervals) {
        events.push_back({iv.second, 0}); // end event
        events.push_back({iv.first, 1});  // start event
    }
    // Sort by time first, then by type (ends first, since 0 < 1).
    sort(events.begin(), events.end());

    long long notStarted = n;
    long long active = 0;
    long long finished = 0;

    long long ans = notStarted * x;

    for (size_t i = 0; i < events.size(); ) {
        // Process all events at the same time.
        // Ends first (type 0) then starts (type 1) — guaranteed by sort.
        while (i < events.size() && events[i].second == 0) {
            active--;   // interval that was active finishes
            finished++;
            i++;
        }
        while (i < events.size() && events[i].second == 1) {
            notStarted--; // interval that was not started begins
            active++;
            i++;
        }
        long long current = notStarted * x + active * y + finished * z;
        if (current > ans) ans = current;
    }
    // After all events, all intervals are finished: value = z * n, which is already captured
    // by the last update (finished == n, active == 0, notStarted == 0).
    return ans;
}
#include <cassert>
#include <vector>
#include <utility>

// Function under test is declared above; include it here.
// (In a real test file, include the solution header.)

int main() {
    // Basic case: one interval [1, 5], x=10, y=1, z=0
    // At t<1: value=10; at t in [1,5]: value=1; at t>5: value=0 => max=10
    assert(maximumValue(1, 10, 1, 0, {{1,5}}) == 10);

    // Two non-overlapping intervals: [1,2] and [3,4], costs 0,5,0
    // t<1: 0; [1,2]:5; [2,3]:0; [3,4]:5; >4:0 => max=5
    assert(maximumValue(2, 0, 5, 0, {{1,2},{3,4}}) == 5);

    // Overlapping intervals: [1,3] and [2,4], costs 0,5,0
    // t<1:0; [1,2):5; [2,3]:10; (3,4]:5; >4:0 => max=10
    assert(maximumValue(2, 0, 5, 0, {{1,3},{2,4}}) == 10);

    // Tie: one ends and another starts at same time: [1,2] and [2,3], costs 0,5,10
    // t<1:0; [1,2]:5; t=2 after end (first finished) before start (second not yet): active=0, finished=1 => value=10; (2,3]:5; >3:20 => max=20 (at +inf)
    assert(maximumValue(2, 0, 5, 10, {{1,2},{2,3}}) == 20);

    // All intervals already finished at start? Not possible since starts < ends.
    // A large test with negative start? Times can be negative, costs positive.
    // Intervals: [-5,-1] and [-2,0], costs 1,2,3.
    // Sweep: initial value = 2*1=2; after first start: notStarted=1, active=1 => 1+2=3; after second start: active=2 => 4; after first end: active=1, finished=1 => 2+3=5; after second end: finished=2 => 6. Max=6.
    assert(maximumValue(2, 1, 2, 3, {{-5,-1},{-2,0}}) == 6);

    // Test multiple simultaneous starts/ends: three intervals [1,2], [1,2], [1,2], costs 0,10,0
    // At t=1 all become active, value=30; max=30.
    assert(maximumValue(3, 0, 10, 0, {{1,2},{1,2},{1,2}}) == 30);

    // Test where max occurs at end (all finished): two intervals [1,2],[3,4], costs 0,0,7 => final value=14, max=14
    assert(maximumValue(2, 0, 0, 7, {{1,2},{3,4}}) == 14);

    // Test where max at beginning: two intervals [10,11],[20,21], costs 5,0,0 => initial value=10, max=10
    assert(maximumValue(2, 5, 0, 0, {{10,11},{20,21}}) == 10);

    // Test with large numbers and long long: n=1000, all intervals [0,1], x=1e9,y=1e9,z=1e9
    // At any time in (0,1): active=1000 => value=1e12; initial and final=1e12 too? initial: 1000*1e9=1e12; active=1000*1e9=1e12; final=1000*1e9=1e12. Max=1e12.
    vector<pair<int,int>> many;
    for (int i=0;i<1000;i++) many.push_back({0,1});
    assert(maximumValue(1000, 1000000000LL, 1000000000LL, 1000000000LL, many) == 1000000000000LL);

    // Edge: n=0 (no intervals) -> initial value = 0, no events, max=0
    assert(maximumValue(0, 5, 6, 7, {}) == 0);

    return 0;
}
// The problem reduces to sweeping over time while maintaining counts of intervals in each of the three states. First, collect all events: for each interval, an "end" event at `b_i` and a "start" event at `a_i`. Sort all events by time. Because continuous time treats an end and a start at the same moment as the end occurring first (the interval being active up to its end), when two events share the same time coordinate, process all end events before start events. At the very beginning (before the smallest event), all intervals are not‑started, so the value is `x * n`. Then we sweep through events in order: for each end event, move one interval from active to finished; for each start event, move one interval from not‑started to active. After processing all events at a given time coordinate, update the answer with the current value. After processing all events (i.e., at `+∞`), all intervals are finished, giving value `z * n`, which is also considered. The time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the event list. Edge cases: when multiple intervals share the same start or end times, the sweep must process all simultaneous end events before simultaneous start events, and after processing all events at one timestamp, the state correctly reflects the instant just after that timestamp.
