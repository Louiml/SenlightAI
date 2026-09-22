You are given a list of tasks, where each task is represented as `[enqueueTime, processingTime]`. The CPU processes tasks one at a time using the following rule: whenever the CPU is idle and there are tasks whose `enqueueTime` has already arrived, it picks the task with the smallest `processingTime` (ties broken by the smaller original index). If no task has arrived yet, the CPU waits until the earliest enqueue time among pending tasks. Write a C++ function `getOrder(const std::vector<std::vector<int>>& tasks)` that returns a vector of the original task indices in the order they are executed. The original index is defined by the order in which tasks appear in the input (0-based). The input is non-empty, and enqueue and processing times are positive integers fitting in a 32-bit signed integer. The total number of tasks is at most 10^5, and the sum of processing times may exceed 32-bit range.
// The solution simulates the CPU scheduler using a min-heap (priority queue) keyed by `(processingTime, originalIndex)` to always pick the optimal task among available ones. First, we augment each task with its original index (by pushing it into the task vector) and sort tasks by `enqueueTime` in ascending order. We maintain a pointer `i` to the next unscheduled task in the sorted list. The current time `t` starts at 0. While there are pending tasks or available tasks: if the heap is empty, jump `t` to the earliest enqueue time among remaining tasks. Then, push all tasks whose enqueue time is ≤ `t` into the heap (each as a pair of processing time and original index). Pop the top (smallest processing time, then smallest index), record its index, and add its processing time to `t`. This greedy choice is optimal because at any moment the CPU must execute one of the available tasks, and choosing the one with smallest processing time minimizes the completion time of that task without affecting future availability (all enqueued tasks are already available; tasks arriving later are unaffected by which available task is run first). Edge cases: when all enqueue times are the same, the heap processes by processing time and then original index; when processing times are large, use `long long` for cumulative time to avoid overflow. Time complexity is O(n log n) due to sorting and heap operations (each task is pushed and popped once). Space complexity is O(n) for the heap and the augmented task vector.
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>

// Returns the execution order of tasks as a vector of original indices.
// Each task is {enqueueTime, processingTime}. Original index = position in input.
std::vector<int> getOrder(const std::vector<std::vector<int>>& tasks) {
    int n = static_cast<int>(tasks.size());
    // Create a copy and attach original indices.
    std::vector<std::vector<int>> augmented = tasks;
    for (int i = 0; i < n; ++i) {
        augmented[i].push_back(i);
    }
    // Sort by enqueueTime (first element), then processingTime, then index (default).
    std::sort(augmented.begin(), augmented.end());

    // Min-heap: pairs of (processingTime, originalIndex).
    using TaskPair = std::pair<int, int>;
    std::priority_queue<TaskPair, std::vector<TaskPair>, std::greater<TaskPair>> available;

    int i = 0;                     // next unscheduled task in augmented
    long long currentTime = 0;     // use long long to avoid overflow
    std::vector<int> order;
    order.reserve(n);

    while (!available.empty() || i < n) {
        if (available.empty()) {
            // Jump to the earliest enqueue time among remaining tasks.
            currentTime = std::max(currentTime, static_cast<long long>(augmented[i][0]));
        }
        // Enqueue all tasks whose enqueue time has arrived.
        while (i < n && augmented[i][0] <= currentTime) {
            available.push({augmented[i][1], augmented[i][2]});
            ++i;
        }
        // Execute the best available task.
        auto [processingTime, idx] = available.top();
        available.pop();
        order.push_back(idx);
        currentTime += processingTime;
    }
    return order;
}
#include <cassert>
#include <vector>

int main() {
    // Example 1: Basic ordering.
    std::vector<std::vector<int>> t1 = {{1, 2}, {2, 4}, {3, 2}, {4, 1}};
    std::vector<int> r1 = getOrder(t1);
    assert((r1 == std::vector<int>{0, 2, 3, 1}));

    // Example 2: All enqueue at same time, tie broken by processing time.
    std::vector<std::vector<int>> t2 = {{0, 5}, {0, 3}, {0, 3}};
    std::vector<int> r2 = getOrder(t2);
    assert((r2 == std::vector<int>{1, 2, 0}));

    // Example 3: CPU waits for next enqueue time.
    std::vector<std::vector<int>> t3 = {{1, 2}, {2, 3}, {10, 1}};
    std::vector<int> r3 = getOrder(t3);
    assert((r3 == std::vector<int>{0, 1, 2}));

    // Example 4: Large times, check cumulative overflow handling.
    std::vector<std::vector<int>> t4 = {{1000000000, 1000000000}, {1000000000, 1000000000}};
    std::vector<int> r4 = getOrder(t4);
    assert((r4 == std::vector<int>{0, 1}));

    // Example 5: Single task.
    std::vector<std::vector<int>> t5 = {{1, 1}};
    std::vector<int> r5 = getOrder(t5);
    assert((r5 == std::vector<int>{0}));

    // Example 6: Reversed enqueue times but tie in processing.
    std::vector<std::vector<int>> t6 = {{5, 2}, {1, 2}, {3, 2}};
    std::vector<int> r6 = getOrder(t6);
    assert((r6 == std::vector<int>{1, 2, 0}));

    // Example 7: Multiple tasks with same processing time, distinct enqueue times.
    std::vector<std::vector<int>> t7 = {{10, 1}, {1, 1}, {5, 1}};
    std::vector<int> r7 = getOrder(t7);
    assert((r7 == std::vector<int>{1, 2, 0}));

    // Example 8: Mixed case with longer processing delays.
    std::vector<std::vector<int>> t8 = {{0, 10}, {1, 5}, {2, 2}, {4, 1}};
    std::vector<int> r8 = getOrder(t8);
    assert((r8 == std::vector<int>{0, 1, 2, 3}));

    // Example 9: Verify no dependence on input order beyond indices.
    std::vector<std::vector<int>> t9 = {{2, 3}, {1, 4}, {3, 2}};
    std::vector<int> r9 = getOrder(t9);
    assert((r9 == std::vector<int>{1, 0, 2}));

    // Example 10: All tasks available immediately, tie by index.
    std::vector<std::vector<int>> t10 = {{0, 2}, {0, 2}, {0, 2}};
    std::vector<int> r10 = getOrder(t10);
    assert((r10 == std::vector<int>{0, 1, 2}));

    return 0;
}
