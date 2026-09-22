Write a C++ function `maxHappinessSchedule` that takes an integer `n` representing the number of days, and a 2D vector or array of `n` rows each containing three integers `a`, `b`, `c` representing the happiness gained on that day if you do activity A, B, or C respectively. You must choose exactly one activity per day, but you cannot choose the same activity on two consecutive days. Return the maximum total happiness achievable over all `n` days. The input is guaranteed to have at least one day, and all happiness values are positive integers.
// This is a classic dynamic programming problem where the state depends on the last activity chosen. Define `dp[i][0]`, `dp[i][1]`, `dp[i][2]` as the maximum total happiness after processing the first `i` days (1-indexed) and ending with activity A, B, C on day `i`, respectively. The transition is: for each activity on day `i`, you can come from either of the other two activities on day `i-1`. So `dp[i][0] = max(dp[i-1][1], dp[i-1][2]) + a_i`, and similarly for the other two. Initialize `dp[0][0] = dp[0][1] = dp[0][2] = 0` (no days processed). The answer is `max(dp[n][0], dp[n][1], dp[n][2])`. Edge case: `n=1` simply returns the maximum of the three single-day values. The algorithm runs in O(n) time and O(1) auxiliary space if we only keep the previous row, but using a full DP table is also fine (O(n) space). The code snippet provided uses a full table, but we can optimize to O(1) space. Care must be taken to not reuse updated values incorrectly within the same day; the original snippet correctly uses the previous day’s values because it reads all three `a,b,c` first before updating. For a standalone function, we can process each day sequentially and keep three variables for the previous day, computing the new three variables based on previous ones.
#include <vector>
#include <algorithm>

// Returns the maximum total happiness over n days given per-day happiness 
// for activities A, B, C, with no same activity on consecutive days.
int maxHappinessSchedule(int n, const std::vector<std::vector<int>>& activities) {
    // activities is a vector of n rows, each containing 3 ints [a, b, c]
    // dp_prev[0], dp_prev[1], dp_prev[2] = max happiness up to previous day
    // ending with activity A, B, C respectively.
    int prevA = 0, prevB = 0, prevC = 0;
    
    for (int i = 0; i < n; ++i) {
        int a = activities[i][0];
        int b = activities[i][1];
        int c = activities[i][2];
        
        int newA = std::max(prevB, prevC) + a;
        int newB = std::max(prevC, prevA) + b;
        int newC = std::max(prevA, prevB) + c;
        
        prevA = newA;
        prevB = newB;
        prevC = newC;
    }
    
    return std::max({prevA, prevB, prevC});
}
#include <cassert>
#include <vector>

int main() {
    // Case 1: n=1, pick the max single day
    std::vector<std::vector<int>> act1 = {{10, 20, 30}};
    assert(maxHappinessSchedule(1, act1) == 30);
    
    // Case 2: n=2, no consecutive same activity
    std::vector<std::vector<int>> act2 = {{1, 2, 3}, {3, 2, 1}};
    // Best: day1 C(3), day2 A(3) -> 6, or day1 B(2), day2 A(3) -> 5, etc.
    assert(maxHappinessSchedule(2, act2) == 6);
    
    // Case 3: alternating to get high total
    std::vector<std::vector<int>> act3 = {{10, 100, 1}, {100, 10, 1}, {1, 100, 10}};
    // Best: day1 B(100), day2 A(100), day3 C(10) -> 210
    assert(maxHappinessSchedule(3, act3) == 210);
    
    // Case 4: all equal, any sequence works
    std::vector<std::vector<int>> act4 = {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}};
    assert(maxHappinessSchedule(3, act4) == 15);
    
    // Case 5: large values, check no overflow (values within int range)
    std::vector<std::vector<int>> act5 = {{1000000, 1, 1}, {1, 1000000, 1}, {1, 1, 1000000}};
    // Best: day1 A(1e6), day2 B(1e6), day3 C(1e6) -> 3e6
    assert(maxHappinessSchedule(3, act5) == 3000000);
    
    // Case 6: n=1 with one dominant
    std::vector<std::vector<int>> act6 = {{7, 3, 5}};
    assert(maxHappinessSchedule(1, act6) == 7);
    
    // Case 7: n=2, same highest value in all positions forces selection
    std::vector<std::vector<int>> act7 = {{10, 9, 8}, {10, 9, 8}};
    // Can't take A both days: best is A(10) + B(9)=19 or A(10)+C(8)=18 or B(9)+A(10)=19
    assert(maxHappinessSchedule(2, act7) == 19);
    
    // Case 8: n=4 mixed pattern
    std::vector<std::vector<int>> act8 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
    // Best: day1 C(3), day2 B(5), day3 C(9), day4 B(11) -> 28? Check alternatives: 
    // day1 C(3), day2 A(4), day3 C(9), day4 B(11) total 27; 
    // day1 B(2), day2 C(6), day3 B(8), day4 C(12) total 28; 
    // day1 A(1), day2 B(5), day3 C(9), day4 B(11) total 26; 
    // day1 B(2), day2 C(6), day3 A(7), day4 C(12) total 27.
    // Actually best is 28.
    assert(maxHappinessSchedule(4, act8) == 28);
    
    return 0;
}
