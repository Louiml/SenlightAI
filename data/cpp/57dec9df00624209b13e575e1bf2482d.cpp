/*
Given \(n\) recipe temperature intervals \([a_i, b_i]\) that are "admissible" for cooking, an integer \(k\) (minimum number of overlapping recipes required for a temperature to be considered good), and \(q\) queries each asking how many integer temperatures in \([l, r]\) are good, write a C++ function `countGoodTemperatures` that takes `n`, `k`, `q`, the recipe intervals as vectors of pairs, the query intervals as vectors of pairs, and returns a `std::vector<long long>` of answers (one per query). The temperature range is assumed to be from 1 to \(2 \cdot 10^5\) inclusive. The function must handle up to \(2 \cdot 10^5\) intervals and queries efficiently.
*/
#include <vector>
#include <algorithm>

// Counts good temperatures in each query range.
// good temperature = covered by at least k recipe intervals.
std::vector<long long> countGoodTemperatures(
    int n, int k, int q,
    const std::vector<std::pair<int,int>>& recipes,
    const std::vector<std::pair<int,int>>& queries
) {
    const int MAX_TEMP = 200000; // given domain size
    std::vector<int> diff(MAX_TEMP + 2, 0); // +2 for safe b+1 when b=MAX

    // Build difference array from recipe intervals
    for (const auto& [a, b] : recipes) {
        diff[a] += 1;
        if (b + 1 <= MAX_TEMP) {
            diff[b + 1] -= 1;
        }
        // if b == MAX, we don't need to subtract beyond range
    }

    // Prefix to get active coverage per temperature
    std::vector<int> active(MAX_TEMP + 2, 0);
    int running = 0;
    for (int i = 1; i <= MAX_TEMP; ++i) {
        running += diff[i];
        active[i] = running;
    }

    // Build prefix of "good" temperatures (active >= k)
    std::vector<int> goodPrefix(MAX_TEMP + 1, 0);
    for (int i = 1; i <= MAX_TEMP; ++i) {
        goodPrefix[i] = goodPrefix[i-1] + (active[i] >= k ? 1 : 0);
    }

    // Answer each query
    std::vector<long long> answers;
    answers.reserve(q);
    for (const auto& [l, r] : queries) {
        answers.push_back(goodPrefix[r] - goodPrefix[l-1]);
    }
    return answers;
}
#include <cassert>
#include <vector>
#include <utility>

// Declaration of the solution function (placed above main)
std::vector<long long> countGoodTemperatures(
    int n, int k, int q,
    const std::vector<std::pair<int,int>>& recipes,
    const std::vector<std::pair<int,int>>& queries
);

int main() {
    // Test 1: basic overlap
    {
        std::vector<std::pair<int,int>> recipes = {{1,3},{2,4},{3,5}};
        std::vector<std::pair<int,int>> queries = {{2,4},{1,5},{6,10}};
        std::vector<long long> result = countGoodTemperatures(3, 2, 3, recipes, queries);
        assert(result == std::vector<long long>({3,3,0})); // 2,3,4 good in [2,4]; 2,3,4 good in [1,5]; none in [6,10]
    }
    // Test 2: single recipe, k=1
    {
        std::vector<std::pair<int,int>> recipes = {{5,5}};
        std::vector<std::pair<int,int>> queries = {{1,10},{5,5},{4,6}};
        std::vector<long long> result = countGoodTemperatures(1, 1, 3, recipes, queries);
        assert(result == std::vector<long long>({1,1,1}));
    }
    // Test 3: no recipe qualifies for k large
    {
        std::vector<std::pair<int,int>> recipes = {{1,10},{2,9},{3,8}};
        std::vector<std::pair<int,int>> queries = {{1,10},{2,8}};
        std::vector<long long> result = countGoodTemperatures(3, 5, 2, recipes, queries);
        assert(result == std::vector<long long>({0,0}));
    }
    // Test 4: boundary at max temperature
    {
        std::vector<std::pair<int,int>> recipes = {{1,200000},{200000,200000}};
        std::vector<std::pair<int,int>> queries = {{199999,200000},{200000,200000}};
        std::vector<long long> result = countGoodTemperatures(2, 2, 2, recipes, queries);
        assert(result == std::vector<long long>({1,1})); // only 200000 has overlap 2
    }
    // Test 5: large range with many recipes
    {
        std::vector<std::pair<int,int>> recipes = {{1,100000},{50000,150000},{100000,200000}};
        std::vector<std::pair<int,int>> queries = {{1,200000},{50000,100000},{150001,200000}};
        std::vector<long long> result = countGoodTemperatures(3, 2, 3, recipes, queries);
        // Good where at least 2 overlap: 50000..100000 (from first two), 100000..150000 (from second third? actually 100000 is in both first third? Let's compute manually)
        // Actually: 1-3 overlap? no. Let's check: 50000-100000 covered by recipe1 and recipe2 -> good. 100000 covered by recipe1, recipe2, recipe3? recipe1 ends at 100000, recipe2 covers 50000-150000, recipe3 starts at 100000 -> yes, good. 100001-150000 covered by recipe2 and recipe3 -> good. So good range 50000..150000 inclusive. Count = 100001. Others not. In [1,200000] all? good only that range, so count 100001. In [50000,100000] count 50001. In [150001,200000] count 0.
        assert(result[0] == 100001);
        assert(result[1] == 50001);
        assert(result[2] == 0);
    }
    // Test 6: query l=1
    {
        std::vector<std::pair<int,int>> recipes = {{1,1}};
        std::vector<std::pair<int,int>> queries = {{1,1}};
        std::vector<long long> result = countGoodTemperatures(1, 1, 1, recipes, queries);
        assert(result == std::vector<long long>({1}));
    }
    // Test 7: empty recipes
    {
        std::vector<std::pair<int,int>> recipes;
        std::vector<std::pair<int,int>> queries = {{1,10},{10,20}};
        std::vector<long long> result = countGoodTemperatures(0, 1, 2, recipes, queries);
        assert(result == std::vector<long long>({0,0}));
    }
    return 0;
}
// The solution uses an **implicit difference array** technique. Since the temperature domain is fixed and small (max \(2 \cdot 10^5\)), we can allocate a difference array `diff[0..MAX+1]` initialized to zero. For each recipe interval \([a, b]\), we increment `diff[a]` by 1 and decrement `diff[b+1]` by 1. After processing all intervals, we compute a prefix sum over `diff` to obtain `active[i]`, the number of intervals covering temperature `i`. Then we mark temperatures where `active[i] >= k` as good (1) else bad (0), and compute a second prefix sum `goodPrefix[i]` where `goodPrefix[i] = goodPrefix[i-1] + (active[i] >= k ? 1 : 0)`. For each query \([l, r]\), the answer is `goodPrefix[r] - goodPrefix[l-1]`. Edge cases include intervals where `b = MAX` (decrement at `MAX+1` ignored because we don't iterate there), and queries where `l` could be 1 (then `l-1` is 0, avoid out-of-bounds by indexing from 1). Time complexity is \(O(\text{MAX} + n + q)\) where MAX = \(2\cdot10^5\), and space is \(O(\text{MAX})\).
