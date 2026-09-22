// Write a C++ function `int minimumMatrixMultiplicationCost(int n, const int dimensions[], int* splitPoints)` that computes the minimum number of scalar multiplications needed to multiply a chain of `n` matrices, where matrix `i` has dimensions `dimensions[i-1] x dimensions[i]` (so the chain is `A1 * A2 * ... * An` with `A_i` having `dimensions[i-1]` rows and `dimensions[i]` columns). The function must return the minimum cost. Additionally, it must fill the `splitPoints` array (of size `n*n`) with the optimal split points, using 1‑based indexing: `splitPoints[(i-1)*n + (j-1)]` should store the optimal split index `k` (where `i ≤ k < j`) for the subproblem of multiplying matrices `i` through `j`. For `i == j` (base case), the split point can be left as 0. If `n` is 0 or negative, or `dimensions` is null, return 0 and do not modify `splitPoints`. If `n == 1`, return 0 and leave `splitPoints` untouched. The input values in `dimensions` are non-negative, and the total cost will not exceed `INT_MAX` (use a large sentinel like `0x7fffffff` for infinity, but if the computed cost equals that sentinel, treat it as an invalid chain and return 0). You must implement the dynamic programming (bottom‑up) approach, not recursion.

The classic matrix‑chain multiplication problem is solved via dynamic programming. Let `dp[i][j]` (stored in a 1‑D array `M` of size `n*n`) represent the minimum number of scalar multiplications to multiply matrices `i..j` (1‑based). The base case `dp[i][i] = 0`. For chain length `len` from 2 to `n`, for every starting index `i` from 1 to `n-len+1`, set `j = i+len-1`. Initialize `dp[i][j]` to a large sentinel (e.g., `0x7fffffff`). Then for every split `k` from `i` to `j-1`, compute `cost = dp[i][k] + dp[k+1][j] + dimensions[i-1]*dimensions[k]*dimensions[j]`. If `cost < dp[i][j]`, update `dp[i][j]` and store `k` in `splitPoints[(i-1)*n+(j-1)]`. After filling all lengths, return `dp[1][n]`. Edge cases: if `n <= 1` or `dimensions == nullptr` or `splitPoints == nullptr`, return 0 without modifying `splitPoints` (for `n == 1`, also no modification). If during computation a cost equals or exceeds the sentinel, that indicates an invalid chain (e.g., a dimension is 0 causing multiplication cost to overflow) — but here inputs are non‑negative and bounded so we can just let the sentinel be a high value; to be safe, if the final `dp[1][n] >= 0x7fffffff` return 0. Time complexity is \(O(n^3)\) because of the three nested loops (chain length, start, split). Space complexity is \(O(n^2)\) for the `dp` table plus `O(n^2)` for `splitPoints` (the caller provides it). We must allocate a temporary `dp` array inside the function and delete it before returning.

#include <climits>
#include <cstddef>

/**
 * Computes the minimum number of scalar multiplications for a matrix chain.
 * @param n Number of matrices (>=0).
 * @param dimensions Array of size n+1 (dimensions[i-1] and dimensions[i] are
 *        rows and cols of matrix i). Assumed non-negative.
 * @param splitPoints Output array of size n*n (1-based indices) that receives
 *        the optimal split k for each subproblem (i,j). Must be non-null if n>1.
 * @return Minimum cost, or 0 if invalid input (n<=0, null dimensions, or
 *         computed cost equals inf).
 */
int minimumMatrixMultiplicationCost(int n, const int dimensions[], int* splitPoints) {
    if (n <= 0 || dimensions == nullptr) {
        return 0;
    }
    if (n == 1) {
        // No multiplication needed; splitPoints not touched.
        return 0;
    }
    if (splitPoints == nullptr) {
        return 0;
    }

    const int INF = 0x7fffffff;
    int* M = new int[n * n]();  // zero-initialized

    // Define convenient 1-based macros for M and splitPoints
    #define M_AT(i, j) M[(i-1)*n + (j-1)]
    #define S_AT(i, j) splitPoints[(i-1)*n + (j-1)]

    // Base case: cost for single matrices is 0 (already zero-initialized).

    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            M_AT(i, j) = INF;
            for (int k = i; k < j; ++k) {
                long long cost = static_cast<long long>(M_AT(i, k)) +
                                 M_AT(k+1, j) +
                                 static_cast<long long>(dimensions[i-1]) *
                                 dimensions[k] * dimensions[j];
                if (cost < M_AT(i, j)) {
                    M_AT(i, j) = static_cast<int>(cost);
                    S_AT(i, j) = k;
                }
            }
        }
    }

    int result = M_AT(1, n);
    delete[] M;
    if (result == INF) {
        return 0;
    }
    return result;

    #undef M_AT
    #undef S_AT
}

#include <cassert>

int main() {
    // Test 1: Standard 4 matrices: dimensions [5,4,6,2,7]
    // Expected minimum cost: 158 (split at 1: (A1)(A2A3A4)? Actually compute)
    // Known optimal cost for 5x4,4x6,6x2,2x7 is 158.
    int dims1[] = {5,4,6,2,7};
    int splits1[4*4] = {0};
    int cost1 = minimumMatrixMultiplicationCost(4, dims1, splits1);
    assert(cost1 == 158);
    // Verify a split point for full chain (1,4): should be 2
    assert(splits1[(1-1)*4 + (4-1)] == 2);

    // Test 2: Single matrix: cost 0, splitPoints untouched
    int dims2[] = {3,4};
    int splits2[1] = {12345};
    int cost2 = minimumMatrixMultiplicationCost(1, dims2, splits2);
    assert(cost2 == 0);
    assert(splits2[0] == 12345); // unchanged

    // Test 3: Two matrices: cost = dims[0]*dims[1]*dims[2]
    int dims3[] = {10,20,30};
    int splits3[4] = {0};
    int cost3 = minimumMatrixMultiplicationCost(2, dims3, splits3);
    assert(cost3 == 6000);
    assert(splits3[(1-1)*2 + (2-1)] == 1);

    // Test 4: n=0 returns 0
    int dims4[] = {1};
    int splits4[1] = {0};
    assert(minimumMatrixMultiplicationCost(0, dims4, splits4) == 0);

    // Test 5: null dimensions
    assert(minimumMatrixMultiplicationCost(2, nullptr, splits4) == 0);

    // Test 6: All ones: cost for n matrices = (n-1)*1
    int dims6[] = {1,1,1,1};
    int splits6[3*3] = {0};
    int cost6 = minimumMatrixMultiplicationCost(3, dims6, splits6);
    assert(cost6 == 2);

    // Test 7: Non-trivial n=3: dims [2,3,4,5] → min cost 64 (split at 1 or 2)
    int dims7[] = {2,3,4,5};
    int splits7[9] = {0};
    int cost7 = minimumMatrixMultiplicationCost(3, dims7, splits7);
    assert(cost7 == 64);

    // Test 8: Zero dimension causes invalid? Dimensions are all non-negative,
    // but zero leads to cost 0 anyway; just test that it doesn't crash.
    int dims8[] = {0,0,0};
    int splits8[4] = {0};
    int cost8 = minimumMatrixMultiplicationCost(2, dims8, splits8);
    assert(cost8 == 0);

    return 0;
}
