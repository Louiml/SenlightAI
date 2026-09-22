Write a C++ function named `maxMeritPoints` that takes an integer `n` (number of days) and a vector of vectors `points` of size `n x 3`, where `points[day]` gives the merit points earned for performing task `task` (0 = swimming, 1 = running, 2 = cycling) on that day. The constraint is that on any given day, you cannot perform the same task you performed on the previous day. Starting with no previous task on day 0, determine the maximum total merit points you can accumulate over all `n` days. The function should return the maximum total as an integer. Assume `n >= 1` and each merit point is a non-negative integer. Do not modify the input vector (use `const` reference).
The problem is a classic dynamic programming (DP) problem often called "Ninja's Training" or "Merit Points". The key observation is that the optimal choice on any given day depends only on which task was performed the previous day. Therefore, we can define a DP table `dp[day][last]` where `day` ranges from 0 to n-1 and `last` is an integer from 0 to 3, where `last = 3` is a sentinel meaning "no previous task" (only used for day 0 initializations). For `last` values 0,1,2, it represents that the previous day's task was that task. The recurrence: for each day `day` and each possible `last` (0..3), we consider all three tasks `task` (0..2) such that `task != last` (if `last == 3`, all tasks allowed), and we take `points[day]+ dp[day-1]` for `day > 0`, and for `day == 0`, `dp[0][last]` is simply the maximum of the points on day 0 for tasks not equal to `last`. To avoid special-casing day 0 separately, we can initialize `dp[0][0] = max(points[0][1], points[0][2])`, `dp[0][1] = max(points[0][0], points[0][2])`, `dp[0][2] = max(points[0][0], points[0][1])`, and `dp[0][3] = max of all three`. Then for days 1 to n-1, we fill `dp[day][last]` by iterating `task` from 0 to 2, skipping if `task == last`, and taking the maximum of `points[day]+ dp[day-1]`. The final answer is `dp[n-1][3]` because on the last day, there is no restriction about the next day. Edge cases: n=1 → answer is simply the maximum of the three points on day 0 (works because `dp[0][3]` gives that). All points being zero is fine. The time complexity is O(n * 4 * 3) = O(n) with constant factors, and space complexity is O(n*4) = O(n), which can be reduced to O(1) if needed but we'll keep the full table for clarity.
#include <vector>
#include <algorithm>

// Returns the maximum total merit points given n days and a points matrix (n x 3).
// The same task cannot be performed on two consecutive days.
int maxMeritPoints(int n, const std::vector<std::vector<int>>& points) {
    // dp[day][last] where last in {0,1,2} means previous task, 3 means no restriction
    std::vector<std::vector<int>> dp(n, std::vector<int>(4, 0));

    // Base case for day 0
    dp[0][0] = std::max(points[0][1], points[0][2]);
    dp[0][1] = std::max(points[0][0], points[0][2]);
    dp[0][2] = std::max(points[0][0], points[0][1]);
    dp[0][3] = std::max({points[0][0], points[0][1], points[0][2]});

    // Fill the DP table for days 1 to n-1
    for (int day = 1; day < n; ++day) {
        for (int last = 0; last < 4; ++last) {
            dp[day][last] = 0; // reset before computing max
            for (int task = 0; task < 3; ++task) {
                if (task != last) { // cannot repeat previous task
                    int candidate = points[day]+ dp[day - 1];
                    dp[day][last] = std::max(dp[day][last], candidate);
                }
            }
        }
    }

    // last = 3 means no next-day restriction, so the optimal score is stored there
    return dp[n - 1][3];
}
#include <cassert>
#include <vector>

int main() {
    // Test case 1: Single day, choose the max of the three
    std::vector<std::vector<int>> points1 = {{10, 40, 70}};
    assert(maxMeritPoints(1, points1) == 70);

    // Test case 2: Two days, alternate to get max
    std::vector<std::vector<int>> points2 = {{10, 40, 70}, {20, 50, 80}};
    // Day0: task2=70, day1: task1=50 -> 120; day0: task1=40, day1: task2=80 -> 120; max is 120
    assert(maxMeritPoints(2, points2) == 120);

    // Test case 3: Three days classical ninja training
    std::vector<std::vector<int>> points3 = {{1, 2, 5}, {3, 1, 1}, {3, 3, 3}};
    // Best: day0 task2=5, day1 task0=3, day2 task1 or 2=3 -> total 11
    assert(maxMeritPoints(3, points3) == 11);

    // Test case 4: All zeros
    std::vector<std::vector<int>> points4 = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    assert(maxMeritPoints(3, points4) == 0);

    // Test case 5: Large values, check no overflow and proper alternation
    std::vector<std::vector<int>> points5 = {{100, 100, 100}, {1, 1, 1}, {100, 100, 100}};
    // Best: day0 task0=100, day1 task1=1, day2 task0=100 -> 201
    assert(maxMeritPoints(3, points5) == 201);

    // Test case 6: Force pick not max each day to test recurrence
    std::vector<std::vector<int>> points6 = {{5, 4, 3}, {1, 10, 6}, {9, 2, 7}};
    // Best: day0 task0=5, day1 task1=10, day2 task0=9 -> 24; or day0 task2=3, day1 task1=10, day2 task0=9 -> 22; max=24
    assert(maxMeritPoints(3, points6) == 24);

    // Test case 7: n=1 with equal points
    std::vector<std::vector<int>> points7 = {{7, 7, 7}};
    assert(maxMeritPoints(1, points7) == 7);
}
