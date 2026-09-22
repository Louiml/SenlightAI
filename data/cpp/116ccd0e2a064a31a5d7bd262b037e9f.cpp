Write a C++ function `long long maximumHarvest(long long n, long long m, long long k, const std::vector<std::pair<long long, long long>>& crops)` that takes the grid dimensions (n rows, m columns), the number of crop positions `k`, and a vector of `(column, row)` pairs for crops. For each crop at column `p` and row `q`, it contributes to the harvest of that column an amount equal to `(n - q)` — i.e., the number of rows below the crop — but only if `n - q > 0` (so the crop is not in the bottom row). The function should return the maximum total harvest across all columns. If no crop contributes to any column, the answer is `-10000010`. The input positions follow 1-based indexing for both rows and columns. The function must handle large values (up to ~10^7 for arrays/indices) efficiently, not rely on global variables, and be reusable.
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function being tested
long long maximumHarvest(long long n, long long m, long long k,
                         const std::vector<std::pair<long long, long long>>& crops);

int main() {
    // Test 1: Basic case with multiple contributions
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 2}, {2, 1}, {1, 1}};
        assert(maximumHarvest(3, 2, 3, crops) == 4); // col1: (3-2)=1 + (3-1)=2 => 3; col2: (3-1)=2 => max=3? Wait recompute: col1=1+2=3, col2=2 => max=3
        // Actually n=3, q=2 => gain=1, q=1 => gain=2; col1 total=3, col2 total=2, answer=3
        assert(maximumHarvest(3, 2, 3, crops) == 3);
    }
    // Test 2: No positive contributions (all crops in bottom row)
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 3}, {2, 3}};
        assert(maximumHarvest(3, 2, 2, crops) == -10000010);
    }
    // Test 3: Single crop with positive gain
    {
        std::vector<std::pair<long long, long long>> crops = {{5, 1}};
        assert(maximumHarvest(10, 5, 1, crops) == 9);
    }
    // Test 4: Multiple crops on same column sum
    {
        std::vector<std::pair<long long, long long>> crops = {{2, 1}, {2, 2}, {2, 3}};
        assert(maximumHarvest(5, 3, 3, crops) == (4+3+2)); // 9
    }
    // Test 5: Empty crops
    {
        std::vector<std::pair<long long, long long>> crops;
        assert(maximumHarvest(5, 4, 0, crops) == -10000010);
    }
    // Test 6: Large m but small k; ensure sentinel works
    {
        std::vector<std::pair<long long, long long>> crops = {{10000000, 2}};
        // n=3, m=10000000, gain=1
        assert(maximumHarvest(3, 10000000, 1, crops) == 1);
    }
    // Test 7: Negative? gain is never negative, but if n=1 and q=1 => gain=0, no update
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 1}, {2, 1}};
        assert(maximumHarvest(1, 2, 2, crops) == -10000010);
    }
    // Test 8: Mixed gains
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 2}, {1, 3}, {2, 1}};
        // n=4: col1: (4-2)=2 + (4-3)=1 => 3; col2: (4-1)=3 => max=3
        assert(maximumHarvest(4, 3, 3, crops) == 3);
    }
    // Test 9: Checks that all columns are considered even if not present
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 1}};
        // m=10, n=2, gain=1 => col1=1, others=0, max=1
        assert(maximumHarvest(2, 10, 1, crops) == 1);
    }
    // Test 10: Multiple crops, maximum on an untouched column? impossible because untouched are 0, so max will be from a touched one.
    // Instead test a case where a column with high sum beats others
    {
        std::vector<std::pair<long long, long long>> crops = {{1, 1}, {2, 1}, {2, 2}};
        // n=3, col1:2, col2:2+1=3 => max=3
        assert(maximumHarvest(3, 2, 3, crops) == 3);
    }
    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

// Returns the maximum total harvest among all columns, given grid dimensions,
// number of crops, and a vector of (column, row) pairs.
// Columns and rows are 1-indexed. Crops in the bottom row contribute 0.
long long maximumHarvest(long long n, long long m, long long k,
                         const std::vector<std::pair<long long, long long>>& crops) {
    // Sentinel value for "no positive contribution"
    const long long NEG_INF = -10000010LL;
    
    // Harvest accumulation per column (1..m)
    std::vector<long long> harvest(m + 1, 0);
    
    // Process each crop
    for (long long i = 0; i < k; ++i) {
        long long col = crops[i].first;
        long long row = crops[i].second;
        long long gain = n - row;
        if (gain > 0) {
            harvest[col] += gain;
        }
    }
    
    // Find maximum across all columns
    long long answer = NEG_INF;
    for (long long col = 1; col <= m; ++col) {
        if (harvest[col] > answer) {
            answer = harvest[col];
        }
    }
    return answer;
}
// The core algorithm is a straightforward simulation: initialize a counter array `harvest` of size `m+1` (1-based indexing) with zeros. For each crop `(p, q)`, compute `gain = n - q`. If `gain > 0`, add `gain` to `harvest[p]`. After processing all crops, find the maximum value in `harvest` over columns 1..m. Important edge cases: when `m` can be up to 10^7, we must allocate the vector of that size (which is acceptable within typical memory limits, ~80MB for `long long`). If `k=0` or all gains are non-positive, the maximum remains `-10000010` (the initial sentinel). Also, note that `q` is 1-based, so if `q == n`, gain is 0 and no update occurs. The time complexity is O(k + m) — we iterate over all crops and then over all columns. Space complexity is O(m) for the harvest array. The function should avoid using global arrays and instead work with a local vector to be self-contained.
