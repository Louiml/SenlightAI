// Write a C++ function `int maxAssignableTasks(vector<int>& tasks, vector<int>& workers, int pills, int strength)` that determines the maximum number of tasks that can be completed. Each task has a difficulty (integer), and each worker has a strength (integer). A worker can complete a task without using a pill only if their strength is at least the task difficulty. They may also use one pill to temporarily boost their strength by `strength` (the given parameter, not their own), allowing them to complete tasks up to `worker_strength + strength`. Each worker can complete at most one task, each task at most once, and there are exactly `pills` pills available globally. The function should return the largest number of tasks that can be assigned to distinct workers, using at most `pills` pills total. The input vectors may be unsorted and contain duplicates. Tasks and workers sizes are up to 10^5, pills up to 10^9, and difficulty/strength values up to 10^9.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> tasks1 = {3, 2, 1};
    std::vector<int> workers1 = {0, 3, 3};
    assert(maxAssignableTasks(tasks1, workers1, 1, 1) == 3);

    std::vector<int> tasks2 = {5, 4};
    std::vector<int> workers2 = {0, 0};
    assert(maxAssignableTasks(tasks2, workers2, 1, 5) == 1);

    // No pills, trivial matching
    std::vector<int> tasks3 = {1, 2, 3};
    std::vector<int> workers3 = {3, 3, 3};
    assert(maxAssignableTasks(tasks3, workers3, 0, 0) == 3);

    // Insufficient workers
    std::vector<int> tasks4 = {1, 2};
    std::vector<int> workers4 = {2};
    assert(maxAssignableTasks(tasks4, workers4, 0, 0) == 1);

    // Empty input
    std::vector<int> tasks5 = {};
    std::vector<int> workers5 = {1, 2};
    assert(maxAssignableTasks(tasks5, workers5, 5, 5) == 0);

    // Pills are critical
    std::vector<int> tasks6 = {10, 9, 8};
    std::vector<int> workers6 = {5, 5, 5};
    assert(maxAssignableTasks(tasks6, workers6, 2, 4) == 2); // with 2 pills and boost 4, can do tasks 9 and 8? Actually 5+4=9 so can do 9 and 8, but not 10, so 2 tasks. With 3 pills boost 4, could do all three? 5+4=9 <10, so no. So 2 is correct.

    // Duplicates and large boost
    std::vector<int> tasks7 = {4, 4, 4};
    std::vector<int> workers7 = {3, 3, 3};
    assert(maxAssignableTasks(tasks7, workers7, 3, 1) == 3); // each worker uses pill to reach 4

    // Worker stronger than tasks
    std::vector<int> tasks8 = {1, 2};
    std::vector<int> workers8 = {10, 20};
    assert(maxAssignableTasks(tasks8, workers8, 0, 0) == 2);

    return 0;
}
#include <vector>
#include <algorithm>
#include <deque>

// Returns the maximum number of tasks that can be completed given a set of tasks,
// workers, a limited number of pills, and a strength boost per pill.
int maxAssignableTasks(std::vector<int>& tasks, std::vector<int>& workers, int pills, int strength) {
    std::sort(tasks.begin(), tasks.end());
    std::sort(workers.begin(), workers.end());

    const int n = static_cast<int>(tasks.size());
    const int m = static_cast<int>(workers.size());
    int left = 0;
    int right = std::min(m, n);

    // Check if we can complete exactly 'count' tasks using the easiest tasks
    // and the strongest workers, with at most 'pills' pills.
    auto feasible = [&](int count) -> bool {
        int remainingPills = pills;
        std::deque<int> candidateTasks;
        int taskIndex = 0;

        // Use the 'count' strongest workers (from index m-count to m-1).
        for (int workerIndex = m - count; workerIndex < m; ++workerIndex) {
            int workerStrength = workers[workerIndex];

            // Add all tasks (among the easiest 'count') that can be done
            // with the help of a pill by this worker.
            while (taskIndex < count && tasks[taskIndex] <= workerStrength + strength) {
                candidateTasks.push_back(tasks[taskIndex]);
                ++taskIndex;
            }

            if (candidateTasks.empty()) {
                return false; // No task available for this worker.
            }

            if (candidateTasks.front() <= workerStrength) {
                // Use the easiest task without a pill.
                candidateTasks.pop_front();
            } else {
                // Must use a pill; assign the hardest available task to save pills.
                if (remainingPills == 0) {
                    return false;
                }
                --remainingPills;
                candidateTasks.pop_back();
            }
        }
        return true;
    };

    // Binary search for the maximum feasible count.
    while (left < right) {
        int mid = left + (right - left + 1) / 2;
        if (feasible(mid)) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    return left;
}
// The solution uses a greedy binary search over the number of tasks we attempt to complete. First, sort both tasks ascending and workers ascending. For a candidate `k` (number of tasks to complete), we need to check if we can assign the `k` easiest tasks to the `k` strongest workers (the last `k` in sorted order). This is optimal because we want to match the easiest tasks with the strongest workers to minimize pill usage. The feasibility check uses a two-pointer technique with a deque of candidate tasks: for each worker from the strongest of the chosen group down to the weakest (iterating from `m-k` to `m-1` in sorted order), we add all tasks (from the first `k` sorted tasks) that can be completed by this worker with a pill (i.e., `tasks[i] <= worker + strength`) into the deque in increasing order. Then, if the smallest task in the deque can be done without a pill (i.e., `front <= worker`), we assign that easiest task to this worker. Otherwise, we must use a pill; to conserve pills, we assign the hardest task in the deque (pop_back) to this worker, decrementing the pill count. If no task is available or pills run out, the candidate is infeasible. The binary search finds the maximum `k` for which the check returns true. Edge cases include `k=0` (always feasible), workers or tasks empty, and cases where pills are insufficient. Time complexity is O((N+M) log N) due to sorting and binary search with a linear check. Space complexity O(N) for the deque.
