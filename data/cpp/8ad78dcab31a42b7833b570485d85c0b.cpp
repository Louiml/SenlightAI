// You are given `n` tasks, each described by a start time `x` and a finish time `y` (with `x < y`). You have `k` identical machines that can each handle at most one task at a time, and a machine can start a new task immediately after finishing a previous one (no gap required). Write a C++ function that returns the maximum number of tasks that can be scheduled across the `k` machines. Input is provided as vectors `starts` and `finishes` (both of length `n`, 1 ≤ n ≤ 2×10^5, 0 ≤ x < y ≤ 10^9, 1 ≤ k ≤ n). You may assume the input is valid. The function signature is: `int scheduleTasks(const std::vector<int>& starts, const std::vector<int>& finishes, int k)`.
This is a classic interval scheduling on `k` machines problem. The greedy approach is to sort tasks by start time. Maintain a multiset (or priority queue) representing the current finish time of each machine. Initially, all `k` machines are available at time 0, so insert `0` for each. Iterate through tasks in increasing start order. For each task, find the machine that finished earliest — but since we need to check if any machine is free before the task starts, we look for a finish time ≤ current start. Using `std::multiset<int, std::greater<int>>` with `lower_bound(start)` finds the largest finish time that is ≤ start. If such a machine exists, schedule the task: remove that machine’s old finish time, add the new finish time (the task’s finish time), and increment the answer. If no machine is free (the earliest finishing machine finishes after the task starts), skip the task. Sorting takes O(n log n), each multiset operation is O(log k), so overall O(n log n + n log k) = O(n log n) time, O(k) auxiliary space for the multiset. Edge cases: k ≥ n, then all tasks can be scheduled; tasks that start at the same time are handled correctly because sorting is stable and we process them in order; if a machine finishes exactly at a task’s start time, it can be reused (≤ comparison).
#include <vector>
#include <algorithm>
#include <set>

// Returns the maximum number of tasks that can be scheduled on k machines.
// Each task has start time starts[i] and finish time finishes[i].
int scheduleTasks(const std::vector<int>& starts, const std::vector<int>& finishes, int k) {
    int n = static_cast<int>(starts.size());
    // Combine tasks into pairs (start, finish) and sort by start time.
    std::vector<std::pair<int, int>> tasks;
    tasks.reserve(n);
    for (int i = 0; i < n; ++i) {
        tasks.emplace_back(starts[i], finishes[i]);
    }
    std::sort(tasks.begin(), tasks.end()); // sorts by start, then finish if equal

    // Multiset of machine finish times, ordered descending.
    // We maintain k machines, initially all finish at time 0.
    std::multiset<int, std::greater<int>> machineFinish;
    for (int i = 0; i < k; ++i) {
        machineFinish.insert(0);
    }

    int scheduled = 0;
    for (const auto& [start, finish] : tasks) {
        // Find the machine with the largest finish time <= start.
        auto it = machineFinish.lower_bound(start);
        if (it != machineFinish.end()) {
            // This machine is free, schedule the task on it.
            machineFinish.erase(it);
            machineFinish.insert(finish);
            ++scheduled;
        }
        // Otherwise, no machine is free, skip this task.
    }
    return scheduled;
}
#include <cassert>
#include <vector>

// Function declaration (included from the solution).
int scheduleTasks(const std::vector<int>& starts, const std::vector<int>& finishes, int k);

int main() {
    // Example 1: Simple 2 tasks, 1 machine, intervals [1,3] and [2,4] can't overlap.
    assert(scheduleTasks({1, 2}, {3, 4}, 1) == 1);
    // Example 2: Same with 2 machines, both can run.
    assert(scheduleTasks({1, 2}, {3, 4}, 2) == 2);
    // Example 3: Tasks that touch: [1,2] and [2,3] can use the same machine.
    assert(scheduleTasks({1, 2}, {2, 3}, 1) == 2);
    // Example 4: Multiple tasks, 2 machines.
    assert(scheduleTasks({1, 2, 3, 4}, {2, 3, 4, 5}, 2) == 4);
    // Example 5: All tasks conflict, only 1 of 3 fits on 1 machine.
    assert(scheduleTasks({1, 2, 3}, {10, 11, 12}, 1) == 1);
    // Example 6: k >= n, all scheduled.
    assert(scheduleTasks({1, 5, 10}, {2, 6, 11}, 5) == 3);
    // Example 7: Zero-length? Not allowed per spec, but check typical case.
    // Example with identical start times and 2 machines.
    assert(scheduleTasks({1, 1, 1}, {2, 3, 4}, 2) == 2);
    // Example 8: Large gap, one machine can do all.
    assert(scheduleTasks({0, 100, 200}, {1, 101, 201}, 1) == 3);
    // Example 9: Negative times? Spec says 0 ≤ x, but test robust.
    assert(scheduleTasks({-5, 0}, {-1, 1}, 1) == 2);
    // Example 10: Complex overlapping with 3 machines.
    assert(scheduleTasks({1, 2, 3, 4, 5}, {3, 4, 5, 6, 7}, 3) == 5);
    return 0;
}
