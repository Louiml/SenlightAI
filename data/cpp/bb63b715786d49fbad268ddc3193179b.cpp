/*
Given a square matrix `mat` of size `n` (where `1 <= n <= 17`) containing integer values, and considering assignments of each row index (0 to n-1) to exactly one distinct column index (a perfect matching), write a C++ function that computes the maximum possible sum of selected matrix elements under such a perfect matching. The function should take a vector of vectors of long long (or similar) representing the matrix and return the maximum sum as a `long long`. This is a classic assignment problem where each row must be paired with a unique column, and we need the maximum total value.
*/

#include <vector>
#include <algorithm>
#include <climits>

// Compute the maximum sum of selecting exactly one element per row and column from a square matrix.
// matrix is an n x n grid of long long values.
// Returns the maximum possible sum of a perfect matching.
long long maximumAssignmentSum(const std::vector<std::vector<long long>>& matrix) {
    const int n = static_cast<int>(matrix.size());
    if (n == 0) {
        return 0;
    }

    const int totalMasks = 1 << n;
    const long long NEG_INF = LLONG_MIN / 4; // avoid overflow

    // dp[mask] = maximum sum after assigning rows 0..popcount(mask)-1 to columns in mask.
    std::vector<long long> dp(totalMasks, NEG_INF);
    dp[0] = 0;

    for (int mask = 0; mask < totalMasks; ++mask) {
        int row = __builtin_popcount(static_cast<unsigned>(mask));
        if (row == n) {
            continue; // all rows assigned
        }
        if (dp[mask] == NEG_INF) {
            continue;
        }
        // Try assigning each unused column to the current row.
        for (int col = 0; col < n; ++col) {
            if ((mask & (1 << col)) == 0) {
                int newMask = mask | (1 << col);
                long long candidate = dp[mask] + matrix[row][col];
                if (candidate > dp[newMask]) {
                    dp[newMask] = candidate;
                }
            }
        }
    }

    return dp[totalMasks - 1];
}

#include <cassert>
#include <vector>
#include <climits>

long long maximumAssignmentSum(const std::vector<std::vector<long long>>& matrix);

int main() {
    // Test 1: 2x2 matrix
    std::vector<std::vector<long long>> m1 = {{1, 2}, {3, 4}};
    // Options: (0,0)+(1,1)=1+4=5; (0,1)+(1,0)=2+3=5
    assert(maximumAssignmentSum(m1) == 5);

    // Test 2: 3x3 matrix
    std::vector<std::vector<long long>> m2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // Best: (0,2)=3 + (1,1)=5 + (2,0)=7 = 15
    assert(maximumAssignmentSum(m2) == 15);

    // Test 3: Single element
    std::vector<std::vector<long long>> m3 = {{42}};
    assert(maximumAssignmentSum(m3) == 42);

    // Test 4: All zeros
    std::vector<std::vector<long long>> m4 = {{0, 0}, {0, 0}};
    assert(maximumAssignmentSum(m4) == 0);

    // Test 5: Mix with negatives
    std::vector<std::vector<long long>> m5 = {{-5, 10}, {20, -1}};
    // Options: (-5)+(-1)=-6; (10)+(20)=30
    assert(maximumAssignmentSum(m5) == 30);

    // Test 6: Larger n=4, known pattern
    std::vector<std::vector<long long>> m6 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    // Greedy pick max each row: 4+8+12+16=40 but better: (0,3)=4, (1,2)=7, (2,1)=10, (3,0)=13 => sum=34? Actually 4+7+10+13=34, but 4+8+11+13=36, 4+8+12+13=37, 3+8+12+14=37, 4+7+12+14=37, 4+8+11+14=37, 4+7+11+15=37, 3+7+12+15=37, 2+8+12+15=37, 2+7+11+16=36, etc. Known max is 34? Let's compute: we need one per row/col. Max sum is 4+8+12+16=40 but rows 0,1,2,3 all column 3,2,1,0? Actually use permutation: row0->col3=4, row1->col2=7, row2->col1=10, row3->col0=13 sum=34. But 4+6+11+13=34, 4+6+12+13=35? row1 col2=7, row2 col1=10 sum 34. Better: row0 col2=3, row1 col3=8, row2 col1=10, row3 col0=13 => 34. Actually maximum is 34? Let's test with brute: permutations of 0..3: sum = mat[0][p0]+mat[1][p1]+mat[2][p2]+mat[3][p3]. Maximum appears to be 34? Check (0,3)=4, (1,2)=7, (2,0)=9, (3,1)=14 => 34. (0,3)=4, (1,1)=6, (2,2)=11, (3,0)=13 => 34. (0,2)=3, (1,3)=8, (2,1)=10, (3,0)=13 => 34. (0,1)=2, (1,3)=8, (2,2)=11, (3,0)=13 => 34. So max is 34. But wait (0,3)=4, (1,2)=7, (2,1)=10, (3,0)=13 =34. (0,2)=3, (1,3)=8, (2,0)=9, (3,1)=14 =34. (0,1)=2, (1,3)=8, (2,0)=9, (3,2)=15? But col2 used? no. Actually let's trust DP: we can run the function and assert it equals 34. For safety, we can just assert it's >=34 and <=40, but better to compute exactly. Let's compute manually: row0: pick col3=4; row1: remaining cols 0,1,2 -> max is 7 (col2); row2: remaining 0,1 -> max is 10 (col1); row3: remaining col0 -> 13. Sum=4+7+10+13=34. Is there any better? Try row0 col2=3, row1 col3=8, row2 col1=10, row3 col0=13 =>34. row0 col1=2, row1 col3=8, row2 col2=11, row3 col0=13 =>34. row0 col0=1, row1 col3=8, row2 col2=11, row3 col1=14 =>34. Seems max is 34. So assert ==34.
    assert(maximumAssignmentSum(m6) == 34);

    // Test 7: Large numbers and negative
    std::vector<std::vector<long long>> m7 = {{1000000000, -1000000000}, {-1000000000, 1000000000}};
    // Options: 1e9+1e9=2e9; (-1e9)+(-1e9)=-2e9
    assert(maximumAssignmentSum(m7) == 2000000000LL);

    // Test 8: n=17 worst size (just ensure it runs, result arbitrary but expected from brute? Not needed)
    // But we can do a simple 17x17 identity-like check
    std::vector<std::vector<long long>> m8(17, std::vector<long long>(17, 0));
    for (int i = 0; i < 17; ++i) m8[i][i] = i + 1;
    // Maximum sum = sum of 1..17 = 153
    assert(maximumAssignmentSum(m8) == 153);

    // Test 9: Empty matrix
    std::vector<std::vector<long long>> m9;
    assert(maximumAssignmentSum(m9) == 0);

    // Test 10: 3x3 with all equal
    std::vector<std::vector<long long>> m10 = {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}};
    assert(maximumAssignmentSum(m10) == 15);

    return 0;
}

// The problem is to find a permutation `p` of columns such that the sum `Σ mat[i][p[i]]` is maximized. Since `n` is small (≤ 17), we can use bitmask dynamic programming. Let `dp[mask]` represent the maximum sum achievable when we have assigned the first `popcount(mask)` rows to the columns represented by the set bits in `mask`. We process rows in order (row index = number of bits set in the mask). Transition: for a given mask, the next row index is `r = __builtin_popcount(mask)`. For each column `c` not yet used (i.e., bit `c` is 0 in mask), we compute `dp[mask | (1<<c)] = max(dp[mask | (1<<c)], dp[mask] + mat[r][c])`. Initialize `dp[0] = 0`. The answer is `dp[(1<<n)-1]` (all columns used). Edge case: `n=1`, only one assignment. Since `n` can be zero? Typically no, but we can handle empty matrix returning 0. Time complexity: There are `2^n` masks, each iterating over at most `n` unused bits, but total transitions sum to `O(n * 2^n)`. Space complexity: `O(2^n)` for the DP table. We can also use a 2D DP but the 1D mask DP is more efficient and standard.
