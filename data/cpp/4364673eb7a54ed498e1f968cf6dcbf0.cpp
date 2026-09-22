Write a C++ function named `aliceCanWin` that accepts a non-empty vector of positive integers representing piles of stones. In each turn, a player can take either the first or the last pile from the remaining sequence. Alice moves first, and both players play optimally to maximize their own total. The function must return `true` if Alice can achieve a total score at least as large as Bob's, and `false` otherwise. The input vector may contain duplicate values and can have any length from 1 to 500. The function must be `const`‑correct and use dynamic programming or memoization to avoid exponential time.
#include <cassert>
#include <vector>

int main() {
    // Single pile
    assert(aliceCanWin({5}) == true);               // Alice takes 5, Bob 0
    // Two piles
    assert(aliceCanWin({3, 7}) == true);            // Alice takes 7, Bob 3
    // Classic [5,3,4,5] → Alice can get 9 vs Bob 8
    assert(aliceCanWin({5, 3, 4, 5}) == true);
    // [3,9,1,2] → Alice maximum is 10, Bob 5 → true
    assert(aliceCanWin({3, 9, 1, 2}) == true);
    // All equal (odd length) → Alice wins by one pile
    assert(aliceCanWin({4, 4, 4}) == true);         // Alice 8, Bob 4
    // Even length with all equal → Alice wins (e.g., [6,6]) → true
    assert(aliceCanWin({6, 6}) == true);
    // Larger test: [1, 100, 1] → Alice can take 1 then 1? Actually optimal: take 1, then Bob forced to take 100? No, Bob will take 100, Alice gets 1+1=2, Bob 100 → false
    assert(aliceCanWin({1, 100, 1}) == false);
    // [1, 5, 2] → Alice takes 2? Let's compute: best Alice 6? Actually: take left 1 → Bob max from [5,2] =5 (takes 5), Alice total 1+2=3; take right 2 → Bob max from [1,5]=5, Alice total 2+1=3 → Alice 3, Bob 5 → false
    assert(aliceCanWin({1, 5, 2}) == false);
    // Large random-like: [10, 20, 30, 40] → Alice can take 40 then 20 → 60 vs Bob 30+10=40 → true
    assert(aliceCanWin({10, 20, 30, 40}) == true);
    return 0;
}
#include <vector>
#include <cstring>
#include <algorithm>

// Returns true if Alice (first player) can achieve a total score >= Bob's total.
bool aliceCanWin(const std::vector<int>& piles) {
    int n = piles.size();
    // dp[i][j] = max score the current player can get from piles[i..j]
    std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, -1));
    
    // Helper lambda for memoized recursion.
    std::function<int(int, int)> solve = [&](int i, int j) -> int {
        if (i > j) return 0;
        if (dp[i][j] != -1) return dp[i][j];
        
        // Option 1: take left pile
        int takeLeft = piles[i] + std::min(solve(i + 2, j), solve(i + 1, j - 1));
        // Option 2: take right pile
        int takeRight = piles[j] + std::min(solve(i + 1, j - 1), solve(i, j - 2));
        
        return dp[i][j] = std::max(takeLeft, takeRight);
    };
    
    int aliceScore = solve(0, n - 1);
    int total = 0;
    for (int value : piles) total += value;
    int bobScore = total - aliceScore;
    return aliceScore >= bobScore;
}
// The problem is a classic minimax game on an array. Let `dp[i][j]` represent the maximum score the current player (whose turn it is) can obtain from the subarray `piles[i..j]`. If `i > j`, no stones remain, so the score is 0. Otherwise, the current player has two choices:  
// 1. Take `piles[i]`, then the opponent plays optimally on `piles[i+1..j]`, and the current player will get `piles[i] + min( dp[i+2][j], dp[i+1][j-1] )` because the opponent will force the worst remaining option for us.  
// 2. Take `piles[j]`, then the opponent plays on `piles[i..j-1]`, yielding `piles[j] + min( dp[i+1][j-1], dp[i][j-2] )`.  
// We choose the maximum of these two options. Base cases: when `i == j`, the player takes that single pile; the formula still works because `dp[i+1][j-1]` etc. become 0. Edge cases include a single pile, two piles, and even/odd lengths; since both play optimally, the recursion correctly captures the minimax. After computing Alice’s maximum score from the full array, compute Bob’s score as total sum minus Alice’s score, and return `aliceScore >= bobScore`. Time complexity: O(n²) states, each computed in O(1), so O(n²) time and O(n²) space (for the memo table). The base case of `i > j` returns 0.
