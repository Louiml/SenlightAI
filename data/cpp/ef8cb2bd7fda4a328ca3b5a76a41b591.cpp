Write a C++ function `int maxMeritPoints(int n, const std::vector<std::vector<int>>& points)` that computes the maximum total merit points a ninja can earn over `n` consecutive days. On each day, the ninja must perform exactly one of three activities (0, 1, or 2), and each activity yields a certain number of merit points given by `points[day][activity]`. The ninja cannot perform the same activity on two consecutive days. The function should return the maximum possible total points over all days, choosing the optimal sequence of activities. Assume `n >= 1`, each `points[i]` has exactly 3 non-negative integers, and the input is valid.
This is a classic dynamic programming problem with a state defined by the current day and the activity performed on the previous day. We can process days from 0 to n-1, maintaining for each possible "last activity" (including a sentinel value 3 meaning no previous day, only for day 0) the maximum points achievable up to that day. Initialize the base case for day 0: for each possible last activity (0,1,2,3), set the value to the maximum points obtainable on day 0 without picking that last activity (for last=3, any of the three activities is allowed). Then for each subsequent day, for each possible "last" activity (0,1,2,3), compute the best value by considering each activity different from last, adding the current day's points to the previous day's best for that activity, and taking the maximum. The answer is the value for last=3 after processing all days. This can be implemented with space optimization using two 1D arrays (previous and current) of size 4, reducing auxiliary space to O(1). Edge case: when n=1, the base case directly yields the maximum of the three values. Time complexity is O(n * 4 * 3) = O(n), and space complexity is O(1) with the optimized version.
#include <vector>
#include <algorithm>

// Return the maximum total merit points over n days, with no same activity on consecutive days.
int maxMeritPoints(int n, const std::vector<std::vector<int>>& points) {
    // prev[last] = best total points up to previous day, where 'last' is the activity done on the previous day.
    // last can be 0,1,2 for actual activities, and 3 as a sentinel for "no previous day" (only used at day 0).
    std::vector<int> prev(4, 0);

    // Base case: day 0
    prev[0] = std::max(points[0][1], points[0][2]); // if last=0, choose max of 1,2
    prev[1] = std::max(points[0][0], points[0][2]); // if last=1
    prev[2] = std::max(points[0][0], points[0][1]); // if last=2
    prev[3] = std::max(points[0][0], std::max(points[0][1], points[0][2])); // if last=3, any allowed

    // Process remaining days
    for (int day = 1; day < n; ++day) {
        std::vector<int> curr(4, 0);
        for (int last = 0; last < 4; ++last) {
            int best = 0;
            for (int task = 0; task < 3; ++task) {
                if (task != last) {
                    best = std::max(best, points[day]+ prev);
                }
            }
            curr[last] = best;
        }
        prev = curr;
    }

    return prev[3];
}
#include <cassert>
#include <vector>
int main() {
    // Single day: choose the max activity
    std::vector<std::vector<int>> p1 = {{10, 20, 30}};
    assert(maxMeritPoints(1, p1) == 30);

    // Two days: cannot repeat same activity
    std::vector<std::vector<int>> p2 = {{10, 20, 30}, {5, 15, 25}};
    // Best: day0 activity2 (30) + day1 activity0 (5) = 35 or day0 activity1 (20)+day1 activity2(25)=45? Actually day0 act2=30, day1 act1=15 total=45. Let's compute: 30+15=45, 20+25=45, 10+25=35, etc. max=45
    assert(maxMeritPoints(2, p2) == 45);

    // Three days: classic example
    std::vector<std::vector<int>> p3 = {{1, 2, 5}, {3, 1, 1}, {3, 3, 3}};
    // Best: day0 act2(5) + day1 act0(3) + day2 act1(3) = 11; or day0 act1(2)+day1 act0(3)+day2 act2(3)=8; max=11
    assert(maxMeritPoints(3, p3) == 11);

    // All same points: only choose max each day, but skip same as previous -> still max each day except possibly alternate same? e.g., all 5: day0=5, day1 cannot 2? Actually all activities same value, best is to choose any, total = 5*n
    std::vector<std::vector<int>> p4 = {{5,5,5}, {5,5,5}, {5,5,5}};
    assert(maxMeritPoints(3, p4) == 15);

    // Zero points on some days
    std::vector<std::vector<int>> p5 = {{10, 0, 0}, {0, 10, 0}, {0, 0, 10}};
    // Best: day0 act0(10) + day1 act1(10) + day2 act2(10) = 30
    assert(maxMeritPoints(3, p5) == 30);

    // Long sequence with increasing values, ensure no repeat
    std::vector<std::vector<int>> p6 = {{1,2,3}, {3,2,1}, {1,2,3}, {3,2,1}};
    // Compute manually: choose day0 act2(3), day1 act0(3), day2 act2(3), day3 act0(3) = 12
    assert(maxMeritPoints(4, p6) == 12);

    // Edge: n=1 with only one valid choice
    std::vector<std::vector<int>> p7 = {{7, 8, 9}};
    assert(maxMeritPoints(1, p7) == 9);

    // Large points values but n small
    std::vector<std::vector<int>> p8 = {{100, 200, 300}, {300, 200, 100}};
    // day0 act2(300) + day1 act0(300) = 600
    assert(maxMeritPoints(2, p8) == 600);

    // Two days, all same values: must choose different activities but all equal -> any different works same total
    std::vector<std::vector<int>> p9 = {{4,4,4}, {4,4,4}};
    assert(maxMeritPoints(2, p9) == 8);

    // Mixed: verify exact case
    std::vector<std::vector<int>> p10 = {{10, 40, 70}, {20, 50, 80}, {30, 60, 90}};
    // Best: day0 act2(70) + day1 act1(50) + day2 act0(30) = 150 or day0 act1(40)+day1 act2(80)+day2 act0(30)=150, day0 act0(10)+day1 act2(80)+day2 act1(60)=150, max=150
    assert(maxMeritPoints(3, p10) == 150);

    return 0;
}
