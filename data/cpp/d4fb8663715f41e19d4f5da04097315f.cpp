// Given an array of positive integers representing the dimensions of a chain of matrices (where the i-th matrix has dimensions `arr[i-1] × arr[i]`), write a C++ function `int minimumMatrixMultiplications(const std::vector<int>& dimensions)` that returns the minimum number of scalar multiplications needed to multiply the entire chain of matrices together. Use a bottom-up dynamic programming approach. The input vector will have at least 2 elements (i.e., at least 1 matrix), and all dimensions are positive integers. The function should be const-correct and handle edge cases like a single matrix (return 0) or two matrices (return the product of the three dimensions). The algorithm must be efficient for up to `n = 100` matrices.

#include <cassert>
#include <vector>

int minimumMatrixMultiplications(const std::vector<int>& dimensions);

int main() {
    // Example from problem statement: arr = {1,2,3,4,3} -> 30
    assert(minimumMatrixMultiplications({1, 2, 3, 4, 3}) == 30);

    // Single matrix -> 0
    assert(minimumMatrixMultiplications({5, 10}) == 0);

    // Two matrices -> product of three dimensions
    assert(minimumMatrixMultiplications({2, 3, 4}) == 24);

    // Chain of three matrices: dimensions (10,30) (30,5) (5,60) -> min is 4500
    assert(minimumMatrixMultiplications({10, 30, 5, 60}) == 4500);

    // Another classic: (40,20) (20,30) (30,10) (10,30) -> 26000
    assert(minimumMatrixMultiplications({40, 20, 30, 10, 30}) == 26000);

    // All same dimensions: (2,2) x4 matrices -> split equally gives 24
    assert(minimumMatrixMultiplications({2, 2, 2, 2, 2}) == 24);

    // Two dimensions only (1 matrix) -> 0
    assert(minimumMatrixMultiplications({7, 7}) == 0);

    // Larger chain: (1,2) (2,3) (3,4) (4,5) -> optimal split gives 38
    assert(minimumMatrixMultiplications({1, 2, 3, 4, 5}) == 38);

    // Two matrices with large numbers
    assert(minimumMatrixMultiplications({100, 1, 100}) == 10000);

    // Chain of 5 with increasing sizes: (1,2) (2,3) (3,4) (4,5) (5,6) -> 84
    assert(minimumMatrixMultiplications({1, 2, 3, 4, 5, 6}) == 84);

    return 0;
}

#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of scalar multiplications needed to multiply
// a chain of matrices whose dimensions are given in the vector dimensions.
// Matrix i (1-indexed) has dimensions dimensions[i-1] x dimensions[i].
int minimumMatrixMultiplications(const std::vector<int>& dimensions) {
    int n = static_cast<int>(dimensions.size()) - 1; // number of matrices
    if (n <= 1) {
        return 0;
    }

    // dp[i][j] = minimum cost to multiply matrices i..j (1-indexed)
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));

    // chain length L from 2 to n
    for (int L = 2; L <= n; ++L) {
        for (int i = 1; i <= n - L + 1; ++i) {
            int j = i + L - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {
                int cost = dp[i][k] + dp[k + 1][j]
                           + dimensions[i - 1] * dimensions[k] * dimensions[j];
                dp[i][j] = std::min(dp[i][j], cost);
            }
        }
    }

    return dp[1][n];
}

// This is the classic Matrix Chain Multiplication problem. The optimal substructure is: to multiply matrices from index `i` to `j` (using 1-based indexing where matrix `k` has dimensions `arr[k-1] × arr[k]`), we split at some `k` between `i` and `j-1`, compute cost of left chain, right chain, and the cost of multiplying the two resulting matrices (`arr[i-1]*arr[k]*arr[j]`). The DP table `dp[i][j]` stores the minimum cost for chain `i..j`. We iterate over chain length from 2 to `n` (where `n = number of matrices`), and for each length, we iterate over all possible starting indices and all split points. The base case `dp[i][i] = 0` for a single matrix. For `n=1`, return 0 directly. The answer is `dp[1][n]`. Time complexity is O(n³) and space complexity is O(n²). Edge cases: single matrix → 0; two matrices → product of the three involved dimensions. The input vector size `N = number_of_matrices + 1`, so number of matrices = `N-1`. We use 1-based indexing for the DP table, so we allocate `(numMatrices+1) × (numMatrices+1)`.
