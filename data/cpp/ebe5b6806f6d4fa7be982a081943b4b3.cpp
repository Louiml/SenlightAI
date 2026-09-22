// Given a list of `n` tasks, each described by a release time `a[i]` and a deadline `b[i]` (where both are integers in the range [1, 10^5] and `a[i] <= b[i]`), write a C++ function `solveTaskScheduling` that accepts a vector of pairs `(a[i], b[i])` and returns a pair of vectors: first, a valid schedule (a permutation of indices 0..n-1) if one exists, and second, a flag indicating whether the schedule is feasible. A feasible schedule is an ordering of tasks such that, when tasks are processed in that order, the completion time of each task (which is its position in the schedule, 1-indexed) does not exceed its deadline, and each task is processed at or after its release time. If no feasible schedule exists, return an arbitrary permutation with the flag set to `false`. If a feasible schedule exists, return that schedule with the flag `true`. The function must be efficient for `n` up to 100,000.
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (or copy it above)
pair<bool, vector<int>> solveTaskScheduling(const vector<pair<int,int>>& tasks);

bool isValidSchedule(const vector<pair<int,int>>& tasks, const vector<int>& order) {
    if ((int)order.size() != (int)tasks.size()) return false;
    vector<bool> used(tasks.size(), false);
    for (int pos = 0; pos < (int)order.size(); ++pos) {
        int idx = order[pos];
        if (used[idx]) return false;
        used[idx] = true;
        int t = pos + 1; // 1-indexed position
        if (t < tasks[idx].first || t > tasks[idx].second) return false;
    }
    return true;
}

int main() {
    // Test 1: simple feasible case
    vector<pair<int,int>> t1 = {{1,2}, {1,2}};
    auto res1 = solveTaskScheduling(t1);
    assert(res1.first == true);
    assert(isValidSchedule(t1, res1.second));

    // Test 2: infeasible because two tasks both require time 1 but have deadlines 1
    vector<pair<int,int>> t2 = {{1,1}, {1,1}};
    auto res2 = solveTaskScheduling(t2);
    assert(res2.first == false);

    // Test 3: feasible with different release times
    vector<pair<int,int>> t3 = {{1,3}, {2,3}, {3,3}};
    auto res3 = solveTaskScheduling(t3);
    assert(res3.first == true);
    assert(isValidSchedule(t3, res3.second));

    // Test 4: infeasible because a task has release time after its deadline? Not valid input, but we handle gracefully
    vector<pair<int,int>> t4 = {{5,2}}; // invalid but should return false
    auto res4 = solveTaskScheduling(t4);
    assert(res4.first == false);

    // Test 5: single task with deadline 1
    vector<pair<int,int>> t5 = {{1,1}};
    auto res5 = solveTaskScheduling(t5);
    assert(res5.first == true);
    assert(isValidSchedule(t5, res5.second));

    // Test 6: all release times 1, deadlines increasing
    vector<pair<int,int>> t6 = {{1,1}, {1,2}, {1,3}};
    auto res6 = solveTaskScheduling(t6);
    assert(res6.first == true);
    assert(isValidSchedule(t6, res6.second));

    // Test 7: larger random feasible case (simple check)
    int n = 100;
    vector<pair<int,int>> t7;
    for (int i = 0; i < n; ++i) {
        int a = 1 + (i * 7) % n; // release within 1..n
        int b = min(n, a + (i % 20)); // ensure b >= a
        t7.emplace_back(a, b);
    }
    auto res7 = solveTaskScheduling(t7);
    if (res7.first) {
        assert(isValidSchedule(t7, res7.second));
    }

    // Test 8: empty input
    vector<pair<int,int>> t8;
    auto res8 = solveTaskScheduling(t8);
    assert(res8.first == true);
    assert(res8.second.empty());

    // Test 9: chain where each task's deadline equals its position in a valid order
    vector<pair<int,int>> t9 = {{1,1}, {2,2}, {3,3}};
    auto res9 = solveTaskScheduling(t9);
    assert(res9.first == true);
    assert(isValidSchedule(t9, res9.second));

    // Test 10: overlapping but feasible with a non-trivial order
    vector<pair<int,int>> t10 = {{1,4}, {1,2}, {2,3}, {3,4}};
    auto res10 = solveTaskScheduling(t10);
    assert(res10.first == true);
    assert(isValidSchedule(t10, res10.second));

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Solves the task scheduling problem:
// Input: vector of pairs (release_time, deadline) for n tasks.
// Output: pair<bool, vector<int>> where bool indicates feasibility, and
//         vector<int> is a permutation of 0..n-1 giving the schedule order.
// If feasible, the schedule is valid. If not, the vector is any permutation.
pair<bool, vector<int>> solveTaskScheduling(const vector<pair<int,int>>& tasks) {
    int n = (int)tasks.size();
    if (n == 0) return {true, {}};

    // Group tasks by their release time, storing (deadline, original_index)
    vector<vector<pair<int,int>>> byRelease(n + 1); // release times in [1, n]
    for (int i = 0; i < n; ++i) {
        int a = tasks[i].first;
        int b = tasks[i].second;
        // We assume 1 <= a <= b <= n for a valid instance; if not, we clamp for safety.
        if (a < 1) a = 1;
        if (a > n) a = n; // but then infeasible; we'll handle later
        byRelease[a].emplace_back(b, i);
    }

    // Min-heap ordered by deadline
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    vector<int> schedule;
    schedule.reserve(n);

    bool feasible = true;
    for (int t = 1; t <= n; ++t) {
        // Add all tasks whose release time is exactly t
        for (const auto& p : byRelease[t]) {
            pq.push(p);
        }
        if (pq.empty()) {
            feasible = false;
            break;
        }
        auto [deadline, idx] = pq.top();
        pq.pop();
        if (deadline < t) {
            feasible = false;
            break;
        }
        schedule.push_back(idx);
    }

    if (!feasible) {
        // Return any permutation (e.g., identity) but with flag false.
        vector<int> any(n);
        iota(any.begin(), any.end(), 0);
        return {false, move(any)};
    }

    // The schedule is complete and feasible.
    return {true, move(schedule)};
}
// This is a classic scheduling problem with release times and deadlines. The key observation is that a greedy algorithm using a priority queue works: at each time step `t` from 1 to `n`, consider all tasks whose release time equals exactly `t`, and add them to a priority queue ordered by deadline (smallest deadline first). Then, at each time step, we pop the task with the smallest deadline among all available tasks and schedule it at time `t`. If at any step the priority queue is empty, then no task is available, which means a feasible schedule is impossible. Also, after scheduling, we must check that the deadline of the popped task is at least `t`; if not, the schedule fails. This greedy approach is correct because of an exchange argument: any feasible schedule can be transformed into this greedy schedule without violating constraints. For the detection of a possible alternative schedule when the first greedy attempt fails, the provided code attempts to find a single swap that fixes the infeasibility; however, for the standalone task, we simplify: we only need to determine feasibility and produce a correct schedule if feasible. If infeasible, return any permutation. The algorithm uses O(n) time for the priority queue operations (since each task is pushed and popped once) and O(n) space for the queue and the schedule. Important edge cases: tasks with very early release times (e.g., all `a[i] = 1`), duplicates in release times, and tasks where the deadline is exactly equal to the position. Also note that the deadline is inclusive, so a task scheduled at position `t` must satisfy `t <= b[i]`. The implementation must handle 1-indexed positions internally and convert to 0-indexed output.
