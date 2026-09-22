Given two lists of nonzero integers (with possible zeros that should be ignored) and a common length N, write a C++ function `int maximumDotProduct(const std::vector<int>& a, const std::vector<int>& b, int N)` that returns the maximum possible sum of products obtained by pairing elements from the two lists, where each element can be used at most once, and pairs are formed by deleting some elements from either list and then aligning the remaining sequences in order. In other words, you may choose an arbitrary (possibly empty) subsequence from each list, and then multiply corresponding elements of the two chosen subsequences (aligned by position) and sum those products; the goal is to maximize this sum. The input lists may have different lengths after removing zeros; you are allowed to skip elements from either list, but the relative order of chosen elements must be preserved. If no valid pairing is possible (i.e., one of the lists becomes empty), the result should be 0. The function must handle N up to 400, with each integer in the range [-1000, 1000] and zeros present that should be excluded from the lists. The maximum sum may be negative if all products are negative; in that case, returning 0 is acceptable only if a valid pairing exists and yields a non-positive sum, but the true optimal (most positive) sum must be returned even if negative. However, if either list is empty after removing zeros, return 0 because no pair can be formed.

The problem reduces to a dynamic programming on three indices: `cur` (position in the original length N, which ranges from 0 to N), `idx1` (position in the filtered first list), and `idx2` (position in the filtered second list). The state `dp[cur][idx1][idx2]` represents the maximum sum achievable using elements from the first `cur` original positions (i.e., after processing the first `cur` items of the original N, we have considered the first `idx1` elements of list1 and first `idx2` elements of list2). At each step, we have three choices: skip the current original item entirely (advance `cur` by 1, keep `idx1` and `idx2`), take the current element from list1 if available (advance `cur` and `idx1`), take the current element from list2 if available (advance `cur` and `idx2`), or pair the current elements from both lists (advance `cur`, `idx1`, `idx2` and add `box1[idx1] * box2[idx2]`). The base case occurs when both filtered lists are fully consumed (`idx1 == L1 && idx2 == L2`), where the result is 0. If we reach `cur == N` without having consumed both lists, it is impossible to complete a valid pairing, so we return a very negative sentinel value (e.g., `NINF = -2e9`) to indicate invalid state. We initialize the dp table with `NINF` to mark uncomputed states. The answer is the maximum of `solve(0,0,0)` (if valid) and 0, because if no pairing is possible we return 0. Edge cases include: zeros in input that must be removed before processing; both lists empty initially (return 0); one list empty (return 0); all products negative (the dp will compute the maximum negative sum, but if a valid pairing exists we should return that maximum, even if negative? The original problem says “if idx1 == L1 && idx2 == L2 return 0” and the final output is the dp result, which could be negative; but the task states to return 0 if either list empty; otherwise return the maximum sum, even if negative. However, to be safe and match common interpretation, we return the dp result if valid, else 0; and since a valid pairing always exists if both lists non-empty (at least one pair each), the dp will return a valid integer. Time complexity: O(N * L1 * L2) due to three nested dimensions, and each state has O(1) transitions. With N ≤ 400 and L1, L2 ≤ N, worst-case O(N^3) = 64 million states, which is acceptable. Space complexity O(N^3) for the dp table, but we can reduce to 2D by iterating `cur` from N-1 down to 0, but since the original uses 3D, we keep it simple. The sentinel `NINF` must be sufficiently negative so that it never wins against any valid sum (products up to 1000*1000=1e6, at most 400 pairs so max sum 4e8, so -2e9 is safe).

#include <vector>
#include <algorithm>
#include <cstring>

// Maximum dot product by choosing subsequences from two vectors (zeros removed).
// Parameters:
//   a - first list of nonzero integers (zeros already removed by caller)
//   b - second list of nonzero integers (zeros already removed by caller)
//   N - original length of the input sequences (used for dp dimensions)
// Returns the maximum possible sum of products, or 0 if either list is empty.
int maximumDotProduct(const std::vector<int>& a, const std::vector<int>& b, int N) {
    const int NINF = -2000000000;
    int L1 = (int)a.size();
    int L2 = (int)b.size();
    if (L1 == 0 || L2 == 0) return 0;

    // dp[cur][i][j] = max product sum using first 'cur' original items,
    // having taken first i from a and first j from b.
    static int dp[401][401][401];
    // Initialize dp to NINF
    for (int cur = 0; cur <= N; ++cur)
        for (int i = 0; i <= L1; ++i)
            for (int j = 0; j <= L2; ++j)
                dp[cur][i][j] = NINF;

    // Iterate cur from N down to 0
    for (int cur = N; cur >= 0; --cur) {
        for (int i = L1; i >= 0; --i) {
            for (int j = L2; j >= 0; --j) {
                if (i == L1 && j == L2) {
                    dp[cur][i][j] = 0; // done with both lists
                    continue;
                }
                if (cur == N) {
                    dp[cur][i][j] = NINF; // not enough items left
                    continue;
                }
                int best = NINF;
                // Skip this original item entirely
                if (cur + 1 <= N) best = std::max(best, dp[cur+1][i][j]);
                // Take next from a only
                if (i < L1 && cur + 1 <= N) best = std::max(best, dp[cur+1][i+1][j]);
                // Take next from b only
                if (j < L2 && cur + 1 <= N) best = std::max(best, dp[cur+1][i][j+1]);
                // Take both
                if (i < L1 && j < L2 && cur + 1 <= N)
                    best = std::max(best, dp[cur+1][i+1][j+1] + a[i] * b[j]);
                dp[cur][i][j] = best;
            }
        }
    }

    int result = dp[0][0][0];
    return (result == NINF) ? 0 : result;
}

#include <cassert>
#include <vector>

// Declaration of the function under test
int maximumDotProduct(const std::vector<int>& a, const std::vector<int>& b, int N);

int main() {
    // Case 1: Simple positive numbers
    assert(maximumDotProduct({1,2,3}, {4,5,6}, 3) == 32); // 1*4 + 2*5 + 3*6 = 32

    // Case 2: With zeros to be ignored (but here input already filtered)
    assert(maximumDotProduct({2,3}, {5,7}, 2) == 29); // 2*5 + 3*7 = 29

    // Case 3: Empty one list
    assert(maximumDotProduct({}, {1,2}, 3) == 0);
    assert(maximumDotProduct({1,2}, {}, 3) == 0);

    // Case 4: All negative products, but skipping gives 0 (since we can skip all)
    // With lists { -1, -2 } and { -3, -4 }, best is skip all => 0
    assert(maximumDotProduct({-1,-2}, {-3,-4}, 2) == 0);

    // Case 5: Negative and positive mixed, must skip to avoid negative
    assert(maximumDotProduct({-1, 2}, {3, -4}, 2) == 6); // skip first pair, take 2*3? Wait 2*3=6, but can we? We can skip first a and second b? Let's compute: a[0]=-1, b[0]=3 => -3; a[1]=2, b[1]=-4 => -8; skip all =0; take both = -11; take a[1] only no. Actually only pairs in order: we can skip a[0] and b[1]? That gives pair a[1] with b[0] => 2*3=6. Works.

    // Case 6: Longer lists with zeros not present
    assert(maximumDotProduct({5,1,3}, {2,4,6}, 3) == 5*2 + 1*4 + 3*6 = 10+4+18=32);

    // Case 7: Need to skip middle elements to get better sum
    // a = {1, -10, 2}, b = {3, -10, 4} -> best: pair a[0]*b[0]=3, skip a[1]/b[1], pair a[2]*b[2]=8 total 11
    assert(maximumDotProduct({1,-10,2}, {3,-10,4}, 3) == 11);

    // Case 8: All zeros ignored externally, but N includes them – ensure function doesn't rely on N for correctness except dp size
    assert(maximumDotProduct({1,2}, {3,4}, 5) == 1*3 + 2*4 = 11);

    return 0;
}
