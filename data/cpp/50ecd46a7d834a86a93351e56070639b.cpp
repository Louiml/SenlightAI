Write a C++ function `int maxScore(int numChapters, int totalTime, const vector<pair<int,int>>& chapters)` that solves the following problem: You have a limited amount of study time `totalTime` (1 ≤ totalTime ≤ 10000) and `numChapters` (1 ≤ numChapters ≤ 100) chapters. Each chapter has a required study time `K` and a score `S` (both between 1 and 1000). You may study any subset of chapters, but you can only study each chapter fully or not at all (i.e., you cannot partially study a chapter). You want to maximize the total score you can obtain given that the sum of study times of chosen chapters does not exceed `totalTime`. Return the maximum possible total score. Your function must handle arbitrary input values within the given ranges and must not modify the input vector. The function should be efficient for the given constraints.
// This is a classic 0/1 knapsack problem where each chapter is an item with weight `K` and value `S`, and the knapsack capacity is `totalTime`. The optimal substructure is: for the first `i` chapters and a given time limit `t`, the best score is either not taking chapter `i` (same as best for first `i-1` chapters) or taking chapter `i` if `t >= K_i`, in which case we add `S_i` to the best score for first `i-1` chapters with time `t - K_i`. We iterate chapters from 1 to `numChapters`, and for each chapter we iterate time from 1 to `totalTime`. A 2D DP table of size `(numChapters+1) x (totalTime+1)` is used, initialized to 0. The recurrence is: `dp[i][t] = max(dp[i-1][t], dp[i-1][t-K_i] + S_i)` if `t >= K_i`, else `dp[i][t] = dp[i-1][t]`. The answer is `dp[numChapters][totalTime]`. Edge cases: when a chapter's time exceeds the current time limit, we simply skip it; when `totalTime` is 0 (though constraints say ≥1), the answer would be 0; the input vector may have duplicates or be unsorted, which is fine. Time complexity is O(numChapters * totalTime) = up to 100*10000 = 1,000,000 operations, and space complexity is O(numChapters * totalTime) for the 2D table, which is about 1e6 integers (~4 MB), acceptable. The function uses `const vector<pair<int,int>>&` to avoid copying, and `const` correctness is maintained.
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum total score achievable with given totalTime using a 0/1 knapsack DP.
int maxScore(int numChapters, int totalTime, const std::vector<std::pair<int,int>>& chapters) {
    // dp[i][t] = max score using first i chapters with exactly t time (t can be less than capacity)
    std::vector<std::vector<int>> dp(numChapters + 1, std::vector<int>(totalTime + 1, 0));
    
    for (int i = 1; i <= numChapters; ++i) {
        int time_i = chapters[i-1].first;
        int score_i = chapters[i-1].second;
        for (int t = 1; t <= totalTime; ++t) {
            // Option 1: skip chapter i
            dp[i][t] = dp[i-1][t];
            // Option 2: take chapter i if time allows
            if (t >= time_i) {
                dp[i][t] = std::max(dp[i][t], dp[i-1][t - time_i] + score_i);
            }
        }
    }
    return dp[numChapters][totalTime];
}
#include <cassert>
#include <vector>
#include <utility>

// Assume maxScore is defined above.
int main() {
    // Sample from problem statement
    std::vector<std::pair<int,int>> chapters1 = {{50, 40}, {100, 70}, {200, 150}};
    assert(maxScore(3, 310, chapters1) == 220);
    
    // Single chapter fits exactly
    std::vector<std::pair<int,int>> chapters2 = {{5, 10}};
    assert(maxScore(1, 5, chapters2) == 10);
    
    // Single chapter does not fit
    assert(maxScore(1, 4, chapters2) == 0);
    
    // Multiple chapters, all fit
    std::vector<std::pair<int,int>> chapters3 = {{1, 1}, {2, 2}, {3, 3}};
    assert(maxScore(3, 6, chapters3) == 6); // take all three: 1+2+3=6 time, score 6
    
    // Capacity exactly enough for best combination but not all
    std::vector<std::pair<int,int>> chapters4 = {{2, 5}, {3, 6}, {4, 8}};
    assert(maxScore(3, 5, chapters4) == 8); // take second (3,6) and first (2,5)? Actually 3+2=5 time -> 6+5=11? Wait: 3 and 2 sum to 5, score 6+5=11; or just 4,8? Let's compute: take 2+3 = 5 -> 5+6=11; take 4 alone -> 8. So max is 11.
    
    // capacity 5, chapters (2,5) and (3,6) -> 11; check
    std::vector<std::pair<int,int>> chapters4b = {{2, 5}, {3, 6}};
    assert(maxScore(2, 5, chapters4b) == 11);
    
    // Zero total time (not per constraints but edge test)
    std::vector<std::pair<int,int>> chapters5 = {{1, 100}};
    assert(maxScore(1, 0, chapters5) == 0);
    
    // Large capacity, only one chapter fits many times? No, knapsack takes each once
    std::vector<std::pair<int,int>> chapters6 = {{10, 50}, {10, 50}};
    assert(maxScore(2, 10, chapters6) == 50); // can only take one of them, not both due to time limit 10
    
    // All chapters have time > totalTime, result 0
    std::vector<std::pair<int,int>> chapters7 = {{100, 1}, {200, 2}};
    assert(maxScore(2, 50, chapters7) == 0);
    
    return 0;
}
