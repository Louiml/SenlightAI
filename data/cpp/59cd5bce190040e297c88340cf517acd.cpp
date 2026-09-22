You are given a line of `n` cells (numbered 1 to n), each with a score `a[i]`. You also have `m` movement cards, each labeled 1, 2, 3, or 4. When you use a card labeled `k`, you move exactly `k` cells forward from your current position. You start at cell 1 and must use every card exactly once, ending exactly at cell `n`. The total score is the sum of the scores of every cell you visit (including the starting cell and the ending cell). Write a C++ function named `maxPathScore` that takes `n`, a vector of scores (indexed from 0 to n-1), and a vector of card labels (each 1..4), and returns the maximum possible total score.
// We can model the problem using dynamic programming over the number of cards of each type used so far. Since card order matters only through the counts of 1,2,3,4 cards used, the position after using `c1` cards of type 1, `c2` of type 2, `c3` of type 3, and `c4` of type 4 is `1 + c1 + 2*c2 + 3*c3 + 4*c4`. We define `dp[c2][c3][c4]` as the maximum score achievable to reach the cell corresponding to having used `c1` cards of each type, but we note that `c1` is determined by the current iteration order if we loop `c1` outer and then `c2,c3,c4` inner. However, to avoid confusion, we fix a 3D DP indexed by `(c2,c3,c4)` and iterate `c1` from 0 to total count of type1. For each state, we consider the last card used: it could be type1,2,3,or4. The transition adds the score of the new cell. We must ensure the target position does not exceed `n`. Edge cases: if a type count is zero, that transition is invalid. The answer is the maximum DP value when the total steps exactly equal `n-1`. Complexity is `O(b1*b2*b3*b4)` where `bi` are counts of each card type; in worst case `m` up to 120, but since each card type is separate, the product of counts is at most `(m/4)^4` if balanced, but more generally bounded by `m^4/256`. With `m ≤ 120`, this is about `5.2 million` states, which is fine. Time complexity is `O(m^4)` worst case, space `O(m^3)` for the DP array. If the input guarantees `m` is small (like in the original snippet where `n ≤ 40` and `m ≤ 120`), it is acceptable.
#include <vector>
#include <algorithm>
#include <cstring>

// Returns the maximum total score possible starting at cell 0 (1-based index 1)
// using all movement cards (labels 1..4) exactly once, ending at cell n-1.
// scores: vector of length n, scores[i] is score of cell i (0-indexed).
// cards: vector of lengths m, each entry is 1,2,3,or4.
int maxPathScore(int n, const std::vector<int>& scores, const std::vector<int>& cards) {
    int cnt[5] = {0,0,0,0,0};
    for (int c : cards) {
        if (c >= 1 && c <= 4) cnt[c]++;
    }
    // dp dimensions: [cnt[2]+1][cnt[3]+1][cnt[4]+1]
    // To avoid dynamic allocation complexity, we use a fixed 3D array of size 41^3 = 68921
    // assuming card counts each ≤ 40 (which holds if m ≤ 120 and each type is possible).
    // But for generality, we allocate dynamically.
    int b2 = cnt[2];
    int b3 = cnt[3];
    int b4 = cnt[4];
    // Use a flattened 3D array: index = (c2*(b3+1) + c3)*(b4+1) + c4
    int dim2 = b3 + 1;
    int dim3 = b4 + 1;
    int size = (b2 + 1) * dim2 * dim3;
    std::vector<int> dp(size, -1);  // -1 means unreachable

    auto idx = [&](int c2, int c3, int c4) {
        return (c2 * dim2 + c3) * dim3 + c4;
    };

    // Initial state: 0 cards used -> position 1 (0-indexed 0)
    dp[idx(0,0,0)] = scores[0];

    for (int c1 = 0; c1 <= cnt[1]; ++c1) {
        for (int c2 = 0; c2 <= b2; ++c2) {
            for (int c3 = 0; c3 <= b3; ++c3) {
                for (int c4 = 0; c4 <= b4; ++c4) {
                    // position 1-based
                    int pos = 1 + c1 + 2*c2 + 3*c3 + 4*c4;
                    if (pos > n) continue;
                    int cur = dp[idx(c2,c3,c4)];
                    if (cur == -1) continue;
                    int all = scores[pos - 1];
                    // Try using one more card of type1 if c1 < cnt[1]
                    if (c1 < cnt[1]) {
                        int npos = pos + 1;
                        if (npos <= n) {
                            int ni = idx(c2,c3,c4); // c1 not stored, but we process c1 outer loop, so this would be considered when c1 increments
                            // Actually we need to store state with c1 implicit. But we update in a way: we can update dp for (c2,c3,c4) when c1 is fixed.
                            // Simpler: we'll compute forward from each state to next states.
                            // Better to do forward DP inside the loops.
                        }
                    }
                    // For simplicity, we'll use forward transitions: from current state, try adding one card of each type as long as counts allow.
                    // But we need c1 count. Since c1 is outer loop, we can update dp for c1+1 using same c2,c3,c4.
                    if (c1 < cnt[1]) {
                        int npos = pos + 1;
                        if (npos <= n) {
                            int ni = idx(c2,c3,c4); // c1 not stored
                            // But dp for c1+1 is the same index? No, to avoid confusion, we'll restructure:
                        }
                    }
                }
            }
        }
    }

    // The above loop is incorrect due to implicit c1. Let's use a proper 4D DP but with flattened 4D? 
    // Instead, we can loop c1 as part of the DP state but we can store dp with dimension for c1 as well.
    // For clarity, we'll allocate a 4D DP: dp[c1][c2][c3][c4].
    // Since max counts each ≤ 120, but we can cap at 41 or 121. Use dynamic size.
    // Actually we can make it 4D with vector of vectors, but for simplicity and given constraints,
    // we can assume counts ≤ 40 each, but to be safe, we'll allocate based on input.
    // Let's re-implement properly with 4D dynamic.

    // However, the constraints from the original problem (NOIP) have n ≤ 350, m ≤ 120, each card count ≤ 40.
    // So we can use static array [41][41][41][41]?
    // But the task says standalone, so we can use dynamic 4D.

    // For efficiency, we'll use a 3D DP where we iterate c1 in outer loop and store states for current c1.
    // That is possible: we can store dp[c2][c3][c4] for the current c1, and update to next c1.
    // But we need to know which c1 we're on. So we'll loop c1 from 0 to cnt[1].
    // Initialize a 2D DP for current c1? Actually we can have two 3D arrays: cur for current c1, nxt for c1+1.
    // But simpler: we can loop c1 outer and then for each state (c2,c3,c4) compute transitions to (c2,c3,c4) for type1 using the same dp array but careful that we are using values from the same c1? 
    // Actually we can do: for c1 from 0 to cnt[1], for each c2,c3,c4, we compute from dp[c2][c3][c4] which is the best score up to this position using exactly c1 ones, c2 twos, etc. Then we can transition to using one more type1, type2, type3, type4, updating the corresponding dp cells for the next c1? But type2 increases c2, which would be in the same 3D but with larger c2. However, that would require iterating in increasing order of c2, c3, c4 to avoid overriding. Since we loop c2,c3,c4 in increasing order, and when we add a type2 card we go to (c2+1,c3,c4) which is larger index, it's safe if we process increasing order. For type1, we go to the same (c2,c3,c4) but with c1+1; since c1 is outer, we can update a separate dp for next c1. To avoid complexity, we'll just use a 4D DP but with dynamic allocation using vector<int> flattened with size (cnt[1]+1)*(cnt[2]+1)*(cnt[3]+1)*(cnt[4]+1).

    // Let's allocate 4D flattened.
    int dim1 = cnt[1] + 1;
    int dim2v = cnt[2] + 1;
    int dim3v = cnt[3] + 1;
    int dim4v = cnt[4] + 1;
    int totalSize = dim1 * dim2v * dim3v * dim4v;
    std::vector<int> dp4(totalSize, -1);

    auto idx4 = [&](int c1, int c2, int c3, int c4) {
        return ((c1 * dim2v + c2) * dim3v + c3) * dim4v + c4;
    };

    dp4[idx4(0,0,0,0)] = scores[0];

    for (int c1 = 0; c1 <= cnt[1]; ++c1) {
        for (int c2 = 0; c2 <= cnt[2]; ++c2) {
            for (int c3 = 0; c3 <= cnt[3]; ++c3) {
                for (int c4 = 0; c4 <= cnt[4]; ++c4) {
                    int cur = dp4[idx4(c1,c2,c3,c4)];
                    if (cur == -1) continue;
                    int pos = 1 + c1 + 2*c2 + 3*c3 + 4*c4;
                    if (pos > n) continue;
                    // Try adding each type of card if available
                    if (c1 < cnt[1]) {
                        int npos = pos + 1;
                        if (npos <= n) {
                            int &nxt = dp4[idx4(c1+1,c2,c3,c4)];
                            nxt = std::max(nxt, cur + scores[npos-1]);
                        }
                    }
                    if (c2 < cnt[2]) {
                        int npos = pos + 2;
                        if (npos <= n) {
                            int &nxt = dp4[idx4(c1,c2+1,c3,c4)];
                            nxt = std::max(nxt, cur + scores[npos-1]);
                        }
                    }
                    if (c3 < cnt[3]) {
                        int npos = pos + 3;
                        if (npos <= n) {
                            int &nxt = dp4[idx4(c1,c2,c3+1,c4)];
                            nxt = std::max(nxt, cur + scores[npos-1]);
                        }
                    }
                    if (c4 < cnt[4]) {
                        int npos = pos + 4;
                        if (npos <= n) {
                            int &nxt = dp4[idx4(c1,c2,c3,c4+1)];
                            nxt = std::max(nxt, cur + scores[npos-1]);
                        }
                    }
                }
            }
        }
    }

    int ans = 0;
    // The final state must have used all cards and reached exactly position n.
    for (int c1 = cnt[1]; c1 <= cnt[1]; ++c1) {
        for (int c2 = cnt[2]; c2 <= cnt[2]; ++c2) {
            for (int c3 = cnt[3]; c3 <= cnt[3]; ++c3) {
                for (int c4 = cnt[4]; c4 <= cnt[4]; ++c4) {
                    int pos = 1 + c1 + 2*c2 + 3*c3 + 4*c4;
                    if (pos == n) {
                        ans = std::max(ans, dp4[idx4(c1,c2,c3,c4)]);
                    }
                }
            }
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Function declaration from solution
int maxPathScore(int n, const std::vector<int>& scores, const std::vector<int>& cards);

int main() {
    // Basic test: 4 cells, one card of each type 1,2,3,4? But total moves 10 > 3, so invalid.
    // Example 1: n=5, scores 1,2,3,4,5, cards: [1,1,1,1] -> 4 moves of 1, total 4 steps, end at 5.
    {
        int n = 5;
        std::vector<int> scores = {1,2,3,4,5};
        std::vector<int> cards = {1,1,1,1};
        int result = maxPathScore(n, scores, cards);
        // Only path: 1->2->3->4->5, total = 1+2+3+4+5=15
        assert(result == 15);
    }
    // Example 2: n=5, cards: [2,2] total 4 moves, path: 1->3->5, score 1+3+5=9
    {
        int n = 5;
        std::vector<int> scores = {10, 1, 20, 1, 30};
        std::vector<int> cards = {2,2};
        int result = maxPathScore(n, scores, cards);
        // Only path: 1->3->5, sum = 10+20+30=60
        assert(result == 60);
    }
    // Example 3: n=7, cards: [1,2,3] total 6 moves, possible paths:
    // Order 1,2,3: 1->2->4->7 sum = scores[0]+scores[1]+scores[3]+scores[6]
    // Order 1,3,2: 1->2->5->7 sum = scores[0]+scores[1]+scores[4]+scores[6]
    // Order 2,1,3: 1->3->4->7 sum = scores[0]+scores[2]+scores[3]+scores[6]
    // Order 2,3,1: 1->3->6->7 sum = scores[0]+scores[2]+scores[5]+scores[6]
    // Order 3,1,2: 1->4->5->7 sum = scores[0]+scores[3]+scores[4]+scores[6]
    // Order 3,2,1: 1->4->6->7 sum = scores[0]+scores[3]+scores[5]+scores[6]
    // Use scores: 1,100,1,1,1,1,100 -> best is 1+100+1+100 = 202? Actually check each:
    // P1: 1+100+1+100=202; P2:1+100+1+100=202; P3:1+1+1+100=103; P4:1+1+1+100=103; P5:1+1+1+100=103; P6:1+1+1+100=103. Best 202.
    {
        int n = 7;
        std::vector<int> scores = {1,100,1,1,1,1,100};
        std::vector<int> cards = {1,2,3};
        int result = maxPathScore(n, scores, cards);
        assert(result == 202);
    }
    // Example 4: single cell, no cards
    {
        int n = 1;
        std::vector<int> scores = {42};
        std::vector<int> cards = {};
        assert(maxPathScore(n, scores, cards) == 42);
    }
    // Example 5: all cards of type 4, n=9, scores 1..9, path 1->5->9 sum 1+5+9=15
    {
        int n = 9;
        std::vector<int> scores = {1,2,3,4,5,6,7,8,9};
        std::vector<int> cards = {4,4};
        assert(maxPathScore(n, scores, cards) == 15);
    }
    // Example 6: mixed, n=10, cards [1,1,2,2,3,3] total 12 moves > 9 invalid, so skip.
    // Example 7: n=4, cards [3,1] -> total 4 moves, possible orders: 3 then 1: 1->4->5 out of range, invalid; 1 then 3: 1->2->5 out of range. So no valid path, but the function should return something? The problem guarantees a valid path exists? In the original, input is guaranteed valid. So we test only valid cases.
    // Example 8: n=6, cards [2,3] total 5 moves, path: 1->3->6 sum = scores[0]+scores[2]+scores[5]
    {
        int n = 6;
        std::vector<int> scores = {1,10,2,10,3,10};
        std::vector<int> cards = {2,3};
        assert(maxPathScore(n, scores, cards) == 13); // 1+2+10=13
    }
    return 0;
}
