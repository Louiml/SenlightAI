// Write a standalone C++ function `countGoodPairs` that takes a square matrix of non-negative integers, represented as a `std::vector<std::vector<unsigned long long>>`, and returns the number of pairs `(i, j)` such that the sum of column `i` is strictly greater than the sum of row `j`. Both indices range from `0` to `n-1` where `n` is the matrix size (1 ≤ n ≤ 30). The matrix is guaranteed to be square, and all values fit within `unsigned long long`. Count each valid pair exactly once, regardless of duplicate row or column sums.
// The algorithm computes the sum of each column and each row separately. First, initialize two arrays (or vectors) of size `n` with zeros: `colSum` and `rowSum`. Iterate through the matrix, adding `a[j][i]` to `colSum[i]` (column sums) and `a[i][j]` to `rowSum[i]` (row sums). Then, for every pair of indices `i` and `j`, increment the answer if `colSum[i] > rowSum[j]`. This is a direct O(n²) double loop. Edge cases include a 1×1 matrix where only comparison is with the same value—since it uses `>`, if the single row sum equals the single column sum (which they always do), the result is 0. Duplicates in sums are handled naturally because each pair is counted independently. Time complexity is O(n²) for computing sums plus O(n²) for the comparison loop, so overall O(n²). Space complexity is O(n) for the two sum arrays. Since n ≤ 30, this is trivial, but the approach scales well.
#include <vector>

// Return the number of pairs (i,j) where column sum i > row sum j.
unsigned long long countGoodPairs(const std::vector<std::vector<unsigned long long>>& matrix) {
    int n = static_cast<int>(matrix.size());
    if (n == 0) return 0;

    std::vector<unsigned long long> colSum(n, 0);
    std::vector<unsigned long long> rowSum(n, 0);

    // Compute both column and row sums in one pass.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            colSum[j] += matrix[i][j];  // column index j
            rowSum[i] += matrix[i][j];  // row index i
        }
    }

    // Count pairs where column sum > row sum.
    unsigned long long ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (colSum[i] > rowSum[j]) {
                ++ans;
            }
        }
    }
    return ans;
}
#include <cassert>
#include <vector>

// Assume countGoodPairs is defined above.

int main() {
    // Test 1: 1x1 matrix
    std::vector<std::vector<unsigned long long>> m1 = {{5}};
    assert(countGoodPairs(m1) == 0);

    // Test 2: 2x2 with distinct sums
    std::vector<std::vector<unsigned long long>> m2 = {{1, 2}, {3, 4}};
    // column sums: [4,6], row sums: [3,7]. Pairs: col0>row0? 4>3 yes, col0>row1? 4>7 no, col1>row0? 6>3 yes, col1>row1? 6>7 no => 2
    assert(countGoodPairs(m2) == 2);

    // Test 3: 2x2 with equal sums
    std::vector<std::vector<unsigned long long>> m3 = {{1, 1}, {1, 1}};
    // all sums = 2, so no column > row => 0
    assert(countGoodPairs(m3) == 0);

    // Test 4: 3x3 with larger values
    std::vector<std::vector<unsigned long long>> m4 = {{10, 20, 30}, {5, 5, 5}, {1, 1, 1}};
    // row sums: [60,15,3]; column sums: [16,26,36]
    // col0=16 > rows? 16>60 no, 16>15 yes, 16>3 yes => 2
    // col1=26 > rows? 26>60 no, 26>15 yes, 26>3 yes => 2
    // col2=36 > rows? 36>60 no, 36>15 yes, 36>3 yes => 2
    // total 6
    assert(countGoodPairs(m4) == 6);

    // Test 5: Empty matrix
    std::vector<std::vector<unsigned long long>> m5;
    assert(countGoodPairs(m5) == 0);

    // Test 6: 2x2 with zeros
    std::vector<std::vector<unsigned long long>> m6 = {{0, 0}, {0, 0}};
    // all sums 0, 0>0 false => 0
    assert(countGoodPairs(m6) == 0);

    // Test 7: 1x2? Not square, but our function should handle? Actually we assume square, but let's test non-square? We'll skip, but function would treat n as rows, not safe. So we only test square.

    // Test 8: Large values
    std::vector<std::vector<unsigned long long>> m7 = {{18446744073709551615ULL, 1}, {1, 1}};
    // col sums: [18446744073709551616? No, 18446744073709551615+1 overflows! So avoid. Use smaller.
    // Use safe: {{18446744073709551615ULL, 0}, {0, 0}}
    // row sums: [max, 0], col sums: [max, 0] => no col > row (since max>max false, max>0 true? col0=max > row0=max? false, col0 > row1=0? true; col1=0 > row0=max? false, col1=0 > row1=0? false => 1
    std::vector<std::vector<unsigned long long>> m8 = {{18446744073709551615ULL, 0}, {0, 0}};
    assert(countGoodPairs(m8) == 1);

    return 0;
}
