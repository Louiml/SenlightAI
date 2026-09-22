// Write a C++ function named `leastInterval` that takes a vector of characters representing task types and an integer `n` representing the cooldown period between two identical tasks. The function should return the minimum number of time units required to schedule all tasks, where each time unit can execute at most one task, and identical tasks must be separated by at least `n` time units where idle time can be inserted. For example, given tasks `['A','A','A','B','B','B']` and `n = 2`, the optimal schedule is `A -> B -> idle -> A -> B -> idle -> A -> B` returning `8`.
The solution uses a max-heap (priority queue) to always schedule the task with the highest remaining frequency first, minimizing idle time. We first count the frequency of each task type. Then push all frequencies into a max-heap. Maintain a FIFO queue of pairs `{remaining_frequency, available_time}` to represent tasks that are in cooldown. Simulate time step by step: at each time unit, increment `time`, check if the front of the cooldown queue becomes available (its `available_time` equals current time), and if so, push its remaining frequency back into the heap. If the heap is non-empty, pop the highest frequency task, decrement its frequency, and if it still has remaining occurrences, push it into the cooldown queue with availability at `time + n + 1`. The loop continues until both heap and queue are empty. The key edge case is when `n = 0` (no cooldown) – then the answer is simply the number of tasks, and the algorithm naturally handles it because tasks are pushed back immediately. Another edge case: a task with frequency 1 never enters the cooldown queue. Time complexity is \(O(T \cdot \log K)\) where `T` is the total number of tasks and `K` is the number of distinct task types, since each task is processed once and each heap operation is logarithmic. Space complexity is \(O(K)\) for the heap and queue.
#include <queue>
#include <vector>
#include <unordered_map>
#include <utility>

// Returns the minimum number of time units to schedule all tasks with a cooldown of n between identical tasks.
int leastInterval(const std::vector<char>& tasks, int n) {
    std::unordered_map<char, int> freqMap;
    for (char task : tasks) {
        ++freqMap;
    }

    std::priority_queue<int> maxHeap;
    for (const auto& entry : freqMap) {
        maxHeap.push(entry.second);
    }

    std::queue<std::pair<int, int>> cooldownQueue; // {remaining_frequency, available_time}
    int time = 0;

    while (!maxHeap.empty() || !cooldownQueue.empty()) {
        ++time;

        // Move tasks that have completed cooldown back to the heap
        if (!cooldownQueue.empty() && cooldownQueue.front().second == time) {
            maxHeap.push(cooldownQueue.front().first);
            cooldownQueue.pop();
        }

        // Schedule the most frequent available task
        if (!maxHeap.empty()) {
            int remaining = maxHeap.top();
            maxHeap.pop();
            if (remaining > 1) {
                cooldownQueue.push({remaining - 1, time + n + 1});
            }
        }
    }

    return time;
}
#include <cassert>
#include <vector>

int main() {
    // Basic example from problem statement
    std::vector<char> tasks1 = {'A','A','A','B','B','B'};
    assert(leastInterval(tasks1, 2) == 8);

    // No cooldown required
    std::vector<char> tasks2 = {'A','A','A','B','B','B'};
    assert(leastInterval(tasks2, 0) == 6);

    // Single task type with large cooldown
    std::vector<char> tasks3 = {'A','A','A'};
    assert(leastInterval(tasks3, 3) == 9); // A idle idle idle A idle idle idle A

    // All distinct tasks, no cooldown needed
    std::vector<char> tasks4 = {'A','B','C','D'};
    assert(leastInterval(tasks4, 2) == 4);

    // Mixed frequencies with cooldown
    std::vector<char> tasks5 = {'A','A','A','B','C','D'};
    assert(leastInterval(tasks5, 2) == 7); // A B C A D idle A

    // Only one task total
    std::vector<char> tasks6 = {'A'};
    assert(leastInterval(tasks6, 5) == 1);

    // Empty input edge case (though problem may not require, but good to handle)
    std::vector<char> tasks7;
    assert(leastInterval(tasks7, 2) == 0);
}
