// Write a C++ function `long long maximumBeautySum(int n, int m, int k, const std::vector<int>& weights)` that, given a rectangular grid of size `n` by `m` and a square filter of size `k` (with `1 <= k <= min(n, m)`), computes the maximum possible sum of "beauty weights" assigned to `w = weights.size()` cells. Each cell can be assigned exactly one weight, and each weight can be used only once. The beauty of a cell `(i,j)` (0-indexed) is defined as the number of positions where the top-left corner of the filter can be placed such that the filter covers that cell. More precisely, the beauty is `(min(i, n-k) - max(i-k+1, 0) + 1) * (min(j, m-k) - max(j-k+1, 0) + 1)`. You may select any `w` distinct cells (not necessarily distinct positions? They must be distinct positions, but because `w` can be larger than the number of cells, ensure that `w <= n*m`). To maximize the sum, you may assign the weights to cells in any order. Return the maximum possible sum using `long long` for the result.

The key observation is that the sum is maximized by pairing the largest weights with the highest beauty cells. The beauty of each cell is independent of the weights and depends only on its coordinates. Compute the beauty for every cell in the grid via the given formula, which counts how many possible top-left positions of the `k x k` filter can cover that cell. Then sort both the beauty values and the weights in descending order. Since we have exactly `w` weights, we only need the `w` largest beauties (note that if there are more cells than weights, we ignore the smaller beauties). Pair the i-th largest weight with the i-th largest beauty and sum their products. This is optimal because for two sequences sorted descending, the maximum dot product is achieved by aligning them in the same order (rearrangement inequality). Edge cases: ensure `w <= n*m` (if `w` is larger, the function should still handle gracefully, perhaps by taking only the largest `n*m` weights, but the problem assumes `w <= n*m`). Also, if `k` is 1, every cell has beauty exactly 1, so the sum is simply the sum of all weights (if `w <= n*m`). The time complexity is `O(n*m log(n*m) + w log w)` due to sorting the beauty array and the weights array, with `O(n*m)` auxiliary space for the beauty values. Since `n*m` can be up to maybe 10^5 or 10^6, this is efficient.

#include <vector>
#include <algorithm>

long long maximumBeautySum(int n, int m, int k, const std::vector<int>& weights) {
    // Compute beauty for every cell in the n x m grid.
    std::vector<long long> beauty;
    beauty.reserve(n * m);
    for (int i = 0; i < n; ++i) {
        int rowWays = std::min(i, n - k) - std::max(i - k + 1, 0) + 1;
        for (int j = 0; j < m; ++j) {
            int colWays = std::min(j, m - k) - std::max(j - k + 1, 0) + 1;
            beauty.push_back(1LL * rowWays * colWays);
        }
    }
    
    // Sort beauties descending.
    std::sort(beauty.begin(), beauty.end(), std::greater<long long>());
    
    // Sort weights descending.
    std::vector<int> sortedWeights = weights;
    std::sort(sortedWeights.begin(), sortedWeights.end(), std::greater<int>());
    
    // Pair the largest weights with the largest beauties.
    long long total = 0;
    int count = static_cast<int>(sortedWeights.size());
    for (int i = 0; i < count; ++i) {
        total += 1LL * sortedWeights[i] * beauty[i];
    }
    return total;
}

#include <cassert>
#include <vector>

// Declaration (function is defined elsewhere)
long long maximumBeautySum(int n, int m, int k, const std::vector<int>& weights);

int main() {
    // Test 1: single cell, k=1, one weight.
    assert(maximumBeautySum(1, 1, 1, {5}) == 5);
    
    // Test 2: 2x2 grid, k=1, weights {1,2,3,4}, all beauties are 1.
    assert(maximumBeautySum(2, 2, 1, {1,2,3,4}) == 10);
    
    // Test 3: 2x2 grid, k=2, weights {10,20,30,40}.
    // With k=2, every cell's beauty: for n=m=2,k=2: rowWays = min(i,0)-max(i-1,0)+1 = 1, same for col, so all 1.
    assert(maximumBeautySum(2, 2, 2, {10,20,30,40}) == 100);
    
    // Test 4: 3x3 grid, k=2. Compute beauties manually:
    // Cells (i,j): rowWays = min(i,1)-max(i-1,0)+1 => for i=0:1, i=1:2, i=2:1. Similarly col. So beauties:
    // (0,0)=1, (0,1)=2, (0,2)=1
    // (1,0)=2, (1,1)=4, (1,2)=2
    // (2,0)=1, (2,1)=2, (2,2)=1
    // Sorted beauties descending: 4,2,2,2,2,1,1,1,1 (actually 2 appears 4 times).
    // Weights {5,4,3,2,1} (w=5). Largest 5 weights: 5,4,3,2,1.
    // Pair: 5*4 + 4*2 + 3*2 + 2*2 + 1*2 = 20+8+6+4+2 = 40.
    assert(maximumBeautySum(3, 3, 2, {5,4,3,2,1}) == 40);
    
    // Test 5: larger weights, check order independence via small case.
    // 3x3, k=2, weights {10,1,1,1,1}. Sorted weights: 10,1,1,1,1.
    // Pair: 10*4 + 1*2 + 1*2 + 1*2 + 1*2 = 40+2+2+2+2=48.
    assert(maximumBeautySum(3, 3, 2, {10,1,1,1,1}) == 48);
    
    // Test 6: w equals number of cells, all weights used.
    // 2x3, k=2. n=2,m=3: rowWays: i=0:1, i=1:1. colWays: j=0:1, j=1:2, j=2:1. So beauties: 1,2,1,1,2,1 => sorted: 2,2,1,1,1,1.
    // Weights {6,5,4,3,2,1} => sum = 6*2+5*2+4*1+3*1+2*1+1*1 = 12+10+4+3+2+1=32.
    assert(maximumBeautySum(2, 3, 2, {6,5,4,3,2,1}) == 32);
    
    // Test 7: k=1, all beauties 1, any order: sum of weights.
    assert(maximumBeautySum(4, 4, 1, {1,2,3}) == 6);
    
    // Test 8: large filter covering whole grid, all beauties 1.
    // 2x2, k=2, weights {7,8} => sum = 15 (since all beauties 1).
    assert(maximumBeautySum(2, 2, 2, {7,8}) == 15);
    
    // Test 9: n=1, m=5, k=1, weights {9,1,1,1,1} => sum=13.
    assert(maximumBeautySum(1, 5, 1, {9,1,1,1,1}) == 13);
    
    // Test 10: n=5, m=1, k=3, weights {3,2,1}. 
    // for n=5,m=1,k=3: rowWays: i=0:1, i=1:2, i=2:3, i=3:2, i=4:1 (since min(i,2)-max(i-2,0)+1). colWays always 1 (m=1,k=3? k<=m fails because k=3>m=1, but problem states k<=min(n,m), so this test invalid. Use valid case instead.
    // Replace with n=5,m=3,k=3: valid. Compute quickly: rowWays as above, colWays same pattern. Largest beauty is 3*3=9 at cell (2,1). Next are 3*2=6 etc. Sorted: 9,6,6,6,6,4,4,4,4,4,2,2,2,2,1? Actually better to trust algorithm.
    // Simpler: n=5,m=3,k=3, weights {10,5,2}. Sorted weights: 10,5,2. Largest beauties: 9,6,6. Sum = 10*9 + 5*6 + 2*6 = 90+30+12=132.
    assert(maximumBeautySum(5, 3, 3, {10,5,2}) == 132);
    
    return 0;
}
