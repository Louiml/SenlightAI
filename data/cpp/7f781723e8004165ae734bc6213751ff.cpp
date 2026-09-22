Write a C++ function `countGoodRecipes` that takes three integers `n`, `k`, `q`, followed by `n` pairs of integers `a, b` (each pair representing the start and end day a recipe is available), and then `q` queries each consisting of two integers `l, r`. For each query, return the number of days in the inclusive range `[l, r]` for which at least `k` recipes are available simultaneously. The day indices range from `1` to `200000`, inclusive. The function should return a `std::vector<int>` containing the answers to the queries in the order they were given.

#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Example 1: simple overlap.
    std::vector<std::pair<int, int>> recipes1 = {{1, 3}, {2, 4}};
    std::vector<std::pair<int, int>> queries1 = {{1, 4}, {2, 2}, {3, 3}};
    std::vector<int> result1 = countGoodRecipes(2, 2, 3, recipes1, queries1);
    assert(result1 == std::vector<int>({2, 1, 1})); // days 2 and 3 have 2 recipes.

    // Example 2: no day reaches k.
    std::vector<std::pair<int, int>> recipes2 = {{1, 2}, {3, 4}};
    std::vector<std::pair<int, int>> queries2 = {{1, 4}};
    std::vector<int> result2 = countGoodRecipes(2, 3, 1, recipes2, queries2);
    assert(result2 == std::vector<int>({0}));

    // Example 3: boundary queries.
    std::vector<std::pair<int, int>> recipes3 = {{1, 1}, {1, 1}};
    std::vector<std::pair<int, int>> queries3 = {{1, 1}, {2, 200000}};
    std::vector<int> result3 = countGoodRecipes(2, 2, 2, recipes3, queries3);
    assert(result3 == std::vector<int>({1, 0})); // only day 1 good.

    // Example 4: large range with single recipe.
    std::vector<std::pair<int, int>> recipes4 = {{5, 10}};
    std::vector<std::pair<int, int>> queries4 = {{1, 20}, {5, 10}, {6, 9}};
    std::vector<int> result4 = countGoodRecipes(1, 1, 3, recipes4, queries4);
    assert(result4 == std::vector<int>({6, 6, 4})); // days 5-10 inclusive = 6 days.

    // Example 5: all days good.
    std::vector<std::pair<int, int>> recipes5 = {{1, 200000}};
    std::vector<std::pair<int, int>> queries5 = {{1, 200000}, {100, 200}};
    std::vector<int> result5 = countGoodRecipes(1, 1, 2, recipes5, queries5);
    assert(result5 == std::vector<int>({200000, 101}));

    return 0;
}

#include <vector>

// Count days in each query range where at least k recipes overlap.
std::vector<int> countGoodRecipes(int n, int k, int q,
                                  const std::vector<std::pair<int, int>>& recipes,
                                  const std::vector<std::pair<int, int>>& queries) {
    const int MAX_DAY = 200000;
    std::vector<int> diff(MAX_DAY + 2, 0);

    // Apply difference array updates.
    for (const auto& recipe : recipes) {
        int a = recipe.first;
        int b = recipe.second;
        diff[a] += 1;
        diff[b + 1] -= 1;
    }

    // Build prefix sum to get number of available recipes per day.
    std::vector<int> available(MAX_DAY + 2, 0);
    for (int day = 1; day <= MAX_DAY; ++day) {
        available[day] = available[day - 1] + diff[day];
    }

    // Build prefix sum of "good" days (days where count >= k).
    std::vector<int> goodPrefix(MAX_DAY + 2, 0);
    for (int day = 1; day <= MAX_DAY; ++day) {
        goodPrefix[day] = goodPrefix[day - 1] + (available[day] >= k ? 1 : 0);
    }

    // Answer each query.
    std::vector<int> results;
    results.reserve(q);
    for (const auto& query : queries) {
        int l = query.first;
        int r = query.second;
        results.push_back(goodPrefix[r] - goodPrefix[l - 1]);
    }

    return results;
}

// The problem requires answering range queries about the number of days where the count of overlapping intervals is at least `k`. A naive approach of counting overlaps per day and then answering each query by scanning the range would be too slow if there are many queries and a large range bound.  
// The solution uses a difference array to compute, for each day `i`, the number of recipes available that day. First, create an array `diff` of size `200002` initialized to zero. For each recipe pair `(a, b)`, increment `diff[a]` by 1 and decrement `diff[b+1]` by 1 (since the recipe is available on days `a` through `b` inclusive). Then compute the prefix sum to get `available[i]`, the count of recipes available on day `i`.  
// Next, convert this into a binary indicator array where each day is `1` if `available[i] >= k`, else `0`. Then compute prefix sums of that indicator array so that `pref[i]` gives the number of "good" days from day 1 to day `i` inclusive. For each query `(l, r)`, the answer is `pref[r] - pref[l-1]`, which is `O(1)` per query.  
// Edge cases include: recipes that start and end on the same day (the diff update works correctly), queries where `l=1` (requires `pref[0] = 0`), and cases where `k` is larger than any overlap count (then no day is good, and all answers are 0). The day range is fixed at `200000`, so the arrays are sized accordingly.  
// Time complexity is `O(n + 200000 + q)` = `O(n + q + maxDay)` where `maxDay = 200000`. Space complexity is `O(maxDay)` for the arrays.
