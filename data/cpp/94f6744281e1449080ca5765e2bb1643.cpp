// Given an array `arr` of size `n` where each element is a task type (an integer between 1 and n) and an array `time` of size `n` where `time[i]` represents the cooldown time needed before task type `i+1` can be executed again, write a C++ function `minTotalTime(int n, const vector<int>& arr, const vector<int>& time)` that returns the minimum total time (in unit steps) required to process all tasks sequentially. You start at time 0. For each task in `arr` in order: if the same task type was last executed at time `t_prev` and the current time is `t`, then you must wait until `max(t, t_prev + time[taskType-1])` before starting this task, and then executing the task itself takes one unit of time (increment the clock by 1 after starting). The total time is the time after the last task finishes. Note: the first occurrence of each task type has no cooldown constraint. The arrays are 1-indexed conceptually (task types from 1 to n). Return the total time elapsed after processing all `n` tasks.

#include <cassert>
#include <vector>

// The function to test is declared above (included via #include or copied here).
int minTotalTime(int n, const std::vector<int>& arr, const std::vector<int>& time);

int main() {
    // Test 1: All distinct tasks, no cooldown waits.
    std::vector<int> arr1 = {1, 2, 3};
    std::vector<int> time1 = {0, 0, 0};
    assert(minTotalTime(3, arr1, time1) == 3);

    // Test 2: Same task repeated, cooldown longer than 1.
    std::vector<int> arr2 = {1, 1, 1};
    std::vector<int> time2 = {5, 0, 0};
    // Times: start 0 at t=0, finish at 1; next wait until max(1,0+5)=5, start at 5, finish at 6; next wait until max(6,5+5)=10, start at 10, finish at 11.
    assert(minTotalTime(3, arr2, time2) == 11);

    // Test 3: Mix with cooldown zero.
    std::vector<int> arr3 = {1, 2, 1};
    std::vector<int> time3 = {2, 1, 0};
    // t0: task1 start at 0, finish 1; task2 start at 1, finish 2; task1: last at 0, need max(2,0+2)=2, start at 2, finish 3.
    assert(minTotalTime(3, arr3, time3) == 3);

    // Test 4: Cooldown forces waiting.
    std::vector<int> arr4 = {1, 1, 2, 1};
    std::vector<int> time4 = {3, 0, 0, 0};
    // t0: task1 start 0 finish 1; task1: wait until max(1,0+3)=3, start 3 finish 4; task2: first time start 4 finish 5; task1: last start 3, need max(5,3+3)=6, start 6 finish 7.
    assert(minTotalTime(4, arr4, time4) == 7);

    // Test 5: Single task.
    std::vector<int> arr5 = {5};
    std::vector<int> time5 = {10, 0, 0, 0, 0};
    assert(minTotalTime(1, arr5, time5) == 1);

    // Test 6: All same task with cooldown 1 (no wait needed).
    std::vector<int> arr6 = {2, 2, 2};
    std::vector<int> time6 = {0, 1, 0};
    // t0: task2 start 0 finish 1; task2: last start 0, need max(1,0+1)=1, start 1 finish 2; task2: max(2,1+1)=2, start 2 finish 3.
    assert(minTotalTime(3, arr6, time6) == 3);

    return 0;
}

#include <vector>
#include <algorithm>

// Compute minimum total time to process all tasks with cooldown constraints.
int minTotalTime(int n, const std::vector<int>& arr, const std::vector<int>& time) {
    // lastTime[i] = time (before execution) when task type i was last started, -1 if never.
    std::vector<int> lastTime(n + 1, -1);
    int currentTime = 0;

    for (int i = 0; i < n; ++i) {
        int task = arr[i];
        if (lastTime!= -1) {
            // Wait until the cooldown period for this task type has passed.
            currentTime = std::max(currentTime, lastTime+ time[task - 1]);
        }
        // Record this start time for the task.
        lastTime= currentTime;
        // Execute the task (takes one unit of time).
        ++currentTime;
    }
    // currentTime is now one past the finish time, so subtract 1.
    return currentTime - 1;
}

// The problem simulates a process where each task type has a cooldown period after it is executed. The key is to keep track of the last execution time for each task type. Since task types are between 1 and n, we can use an array `lastTime` of size `n+1` initialized to -1 (meaning not yet executed). Iterate through the tasks in order. For each task `x = arr[i]`:
// - If `lastTime[x] == -1`, this is the first occurrence, so no cooldown applies. We set `lastTime[x] = currentTime` (where `currentTime` is the time before starting this task).
// - If `lastTime[x] != -1`, we must ensure that `currentTime` is at least `lastTime[x] + time[x-1]`. If not, we must wait, so we set `currentTime = max(currentTime, lastTime[x] + time[x-1])`, then update `lastTime[x] = currentTime`.
// - After handling the cooldown, we increment `currentTime` by 1 (the execution time of the task).
//
// At the end, the total time is `currentTime - 1` because we started at 0 and the last increment brings us to the time after the last task. However, careful: if we initialize `currentTime = 0` and increment after each task, after the last task `currentTime` is one more than the finishing time. The reference solution above uses a variable `ans` that is incremented each loop, so we return `ans-1`. Important edge cases: `n = 0` (though constraints likely require `n>=1`), all tasks distinct (no waits), same task repeated consecutively (waits based on `time`), and `time` values could be zero (meaning no cooldown). The algorithm runs in O(n) time and O(n) auxiliary space for the `lastTime` array. The solution must handle large `n` efficiently.
