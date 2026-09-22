Write a C++ function `int maxSatisfaction(int n, int m, const std::vector<std::vector<int>>& satisfaction)` that takes the number of users `n`, the number of available chicken dishes `m` (where `m >= 3`), and an `n x m` matrix `satisfaction` where `satisfaction[i][j]` is the satisfaction score (an integer, may be negative, zero, or positive) that user `i` would give to dish `j`. The function must select exactly 3 distinct dishes out of the `m` available. For each user, their total satisfaction is the maximum satisfaction among the 3 selected dishes (i.e., they only care about their favorite selected dish). The function should return the maximum possible sum of these per-user maximums over all choices of 3 dishes. The input will always have at least 3 dishes and at least 1 user.

#include <cassert>
#include <vector>

int main() {
    // Basic case with positive values
    {
        std::vector<std::vector<int>> sat = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
        // Best choose dishes 2,3,? Let's compute: for user0 max from (2,3,? ) etc.
        // Actually test simple combinations manually: choose dishes 1,2,3 -> user0 max=4, user1 max=8, user2 max=12 => sum=24
        // choose dishes 0,2,3 -> user0:4, user1:8, user2:12 => 24 too. So answer 24.
        assert(maxSatisfaction(3, 4, sat) == 24);
    }
    
    // Negative values: must pick the largest among negatives
    {
        std::vector<std::vector<int>> sat = {{-5, -1, -3, -2}, {-10, -20, -30, -40}};
        // Choose dishes 1,3,? Let's brute: dish indices 1,2,3 -> user0 max=-1, user1 max=-20 => sum=-21
        // indices 0,1,2 -> user0 max=-1, user1 max=-10 => -11
        // indices 0,1,3 -> user0 max=-1, user1 max=-10 => -11
        // indices 0,2,3 -> user0 max=-2, user1 max=-10 => -12
        // So best is -11.
        assert(maxSatisfaction(2, 4, sat) == -11);
    }
    
    // Single user, many dishes
    {
        std::vector<std::vector<int>> sat = {{10, 20, 30, 40, 50}};
        // Pick any three, user max is the largest of the three; best is pick 40,50 and any other -> max=50.
        // Actually pick 40,50,30 -> max=50; sum=50.
        assert(maxSatisfaction(1, 5, sat) == 50);
    }
    
    // All zeros
    {
        std::vector<std::vector<int>> sat = {{0, 0, 0, 0}, {0, 0, 0, 0}};
        assert(maxSatisfaction(2, 4, sat) == 0);
    }
    
    // Mixed positive and negative, multiple users
    {
        std::vector<std::vector<int>> sat = {{5, -1, 3}, {-2, 4, 6}, {7, 8, -3}};
        // n=3, m=3, only one combination: dishes 0,1,2.
        // user0 max=5, user1 max=6, user2 max=8 => sum=19.
        assert(maxSatisfaction(3, 3, sat) == 19);
    }
    
    // Larger m, check brute force correctness for known best
    {
        std::vector<std::vector<int>> sat = {
            {1, 2, 3, 4, 5},
            {5, 4, 3, 2, 1},
            {10, 10, 10, 10, 10}
        };
        // Choose dishes 0,4,? Let's compute: for user0 max among 0,4 and any third is 5 (if third not >5)
        // For user1 max could be 5 if we pick dish0 (value5) and any others, user2 always 10.
        // So sum = 5+5+10=20 if we pick dishes 0,4,1 (user0 max=5, user1 max=5, user2=10). 
        // Could we get more? Try dishes 0,1,4 -> user0:5, user1:5, user2:10 =20. So answer 20.
        assert(maxSatisfaction(3, 5, sat) == 20);
    }
    
    // Verify with m=3 and n=4, all negative
    {
        std::vector<std::vector<int>> sat = {
            {-1, -2, -3},
            {-4, -5, -6},
            {-7, -8, -9},
            {-10, -11, -12}
        };
        // Only combination 0,1,2: user0 max=-1, user1=-4, user2=-7, user3=-10 => sum=-22
        assert(maxSatisfaction(4, 3, sat) == -22);
    }
    
    return 0;
}

#include <vector>
#include <algorithm>

// Given n users and m dishes (m >= 3), select exactly 3 distinct dishes to maximize
// the sum over all users of their maximum satisfaction among the selected dishes.
// Returns the maximum possible total satisfaction.
int maxSatisfaction(int n, int m, const std::vector<std::vector<int>>& satisfaction) {
    int best = 0;
    bool first = true;
    
    // Iterate over all combinations of 3 distinct dish indices (i < j < k)
    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            for (int k = j + 1; k < m; ++k) {
                int total = 0;
                for (int user = 0; user < n; ++user) {
                    // Maximum satisfaction for this user among the three selected dishes
                    int mx = std::max({satisfaction[user][i], satisfaction[user][j], satisfaction[user][k]});
                    total += mx;
                }
                if (first || total > best) {
                    best = total;
                    first = false;
                }
            }
        }
    }
    
    return best;
}

// The problem reduces to choosing 3 dish indices from `0` to `m-1` (order doesn’t matter) and for each user, taking the maximum value among those three entries, then summing those maxima across all users. Since `m` is typically small (the original snippet uses `m` up to 34, but general constraints are not given), a brute‑force triple nested loop over all combinations of 3 distinct indices is sufficient and straightforward. For each combination, we iterate over all `n` users, compute the max of the three values for that user, and accumulate. We keep track of the global maximum sum. Edge cases include negative values (max still correctly picks the largest among three, possibly negative), duplicate dishes are not allowed (but duplicate satisfaction values are fine), and the matrix dimensions are consistent. Time complexity: there are `O(m^3)` combinations, and for each we process `n` users, so `O(n * m^3)` time. Space complexity: `O(1)` extra besides the input matrix. If `m` were large, a more optimized approach could be considered, but given the problem’s nature (selection of 3), brute force is acceptable and matches the reference snippet.
