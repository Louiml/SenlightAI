You are given `n` monsters indexed from `0` to `n-1`, where each monster `i` has a strength `r[i]` (an integer). There are `k` known directed dominance relationships: for each pair `(x, y)`, if the strength of monster `x` is strictly less than the strength of monster `y`, then monster `y` is said to be "dominated by" monster `x` (i.e., there is an edge from x to y). If the strength of `y` is strictly less than that of `x`, then the edge is reversed. If strengths are equal, no edge is added. For each monster `i`, define a "score" as: the number of monsters with strictly smaller strength than `i`, minus the number of monsters that are known to be stronger than `i` (i.e., those monsters that directly dominate `i` through one of the given relationships). Write a C++ function `vector<int> computeScores(const vector<int>& r, const vector<pair<int,int>>& edges)` that returns a vector of length `n` where each element is the score for the corresponding monster index. Note that the relationships are undirected in input, but only the direction determined by strength comparison is used. Also, the same pair of monsters may appear multiple times; you should count duplicate relationships once per occurrence (i.e., if two monsters are paired twice, both edges are counted). The function must handle up to `n = 2e5` and up to `k = 2e5` edges efficiently.

We need to compute for each monster `i`: `cnt_smaller[i] - cnt_dominated_by[i]`, where `cnt_smaller[i]` is the number of monsters with strength strictly less than `r[i]`, and `cnt_dominated_by[i]` is the number of monsters `j` such that there is a given relationship `(i, j)` (or `(j, i)`) and `r[j] < r[i]` (so j dominates i). Note that if a relationship connects two equal strengths, it contributes nothing because neither dominates the other.

Approach:
1. First, sort the indices by strength. Then, for each index `i` in sorted order, the number of monsters with strictly smaller strength is exactly the index position `p` after skipping all indices with equal strength. We can precompute an array `smaller_count[i]` for each monster `i` by scanning the sorted order and for each group of equal strength, set the count to the starting index of the group.
2. Then, process all edges. For each edge `(x, y)`, if `r[x] < r[y]`, then x dominates y, so increment `dominated[y]` by 1. If `r[y] < r[x]`, increment `dominated[x]`. If equal, do nothing. This gives `dominated[i]` = number of known monsters that are strictly stronger than i (via direct edges).
3. The answer for each `i` is `smaller_count[i] - dominated[i]`.

Edge cases: duplicate edges are counted separately, so we just add 1 per occurrence. Monsters with equal strength get the same smaller_count (the count of monsters with strictly smaller strength, not including equal ones). The time complexity is O(n log n + k) due to sorting and edge processing. Space complexity O(n) for arrays.

#include <vector>
#include <algorithm>
#include <numeric>

// Compute scores for each monster as: number of monsters with strictly smaller strength
// minus number of known monsters that are directly stronger (via given relationships).
// edges: list of pairs (x, y) representing a known relationship; direction is determined
// by comparing strengths: if r[x] < r[y], x dominates y; if r[y] < r[x], y dominates x.
// Duplicate pairs count multiple times.
std::vector<int> computeScores(const std::vector<int>& r, const std::vector<std::pair<int,int>>& edges) {
    int n = static_cast<int>(r.size());
    std::vector<int> smallerCount(n, 0);
    std::vector<int> dominated(n, 0);

    // Sort indices by strength
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(),
              [&](int a, int b) { return r[a] < r[b]; });

    // For each index, count how many monsters have strictly smaller strength.
    // We process groups of equal strength together.
    int p = 0;
    while (p < n) {
        int q = p;
        while (q < n && r[order[q]] == r[order[p]]) {
            // For all monsters in this equal-strength group, smallerCount is p
            // (number of monsters before this group).
            smallerCount[order[q]] = p;
            ++q;
        }
        p = q;
    }

    // Process edges: add to dominated count of the weaker monster.
    for (const auto& e : edges) {
        int x = e.first;
        int y = e.second;
        if (r[x] < r[y]) {
            // x dominates y
            ++dominated[y];
        } else if (r[y] < r[x]) {
            // y dominates x
            ++dominated[x];
        }
        // if equal, no effect
    }

    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = smallerCount[i] - dominated[i];
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>

// assume computeScores is available from the solution above
int main() {
    // Basic case
    {
        std::vector<int> r = {1, 2, 3};
        std::vector<std::pair<int,int>> edges;
        std::vector<int> res = computeScores(r, edges);
        assert(res == std::vector<int>({0, 1, 2}));
    }
    // With edges: r = [1,2,3], edge between 0 and 1 (1<2 so 0 dominates 1)
    {
        std::vector<int> r = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        std::vector<int> res = computeScores(r, edges);
        // smallerCount: [0,1,2], dominated: [0,1,0] => [0,0,2]
        assert(res == std::vector<int>({0, 0, 2}));
    }
    // Duplicate edges count twice
    {
        std::vector<int> r = {1, 2, 3};
        std::vector<std::pair<int,int>> edges = {{0,1}, {0,1}};
        std::vector<int> res = computeScores(r, edges);
        // dominated[1] becomes 2 => [0, -1, 2]
        assert(res == std::vector<int>({0, -1, 2}));
    }
    // Equal strengths ignore edges
    {
        std::vector<int> r = {2, 2, 3};
        std::vector<std::pair<int,int>> edges = {{0,1}};
        std::vector<int> res = computeScores(r, edges);
        // smallerCount: [0,0,2], dominated all 0 => [0,0,2]
        assert(res == std::vector<int>({0, 0, 2}));
    }
    // Larger example with reverse edge
    {
        std::vector<int> r = {3, 1, 2};
        std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}}; // r0>r1 => 1 dominates 0; r1<r2 => 1 dominates 2
        std::vector<int> res = computeScores(r, edges);
        // smallerCount: index1(strength1)=0, index2(strength2)=1, index0(strength3)=2
        // dominated: index1 gets 0, index0 gets +1 from edge(0,1) because 1 dominates 0, index2 gets +1 from edge(1,2) because 1 dominates 2
        // => ans: [2-1=1, 0-0=0, 1-1=0]
        assert(res == std::vector<int>({1, 0, 0}));
    }
    // Single monster
    {
        std::vector<int> r = {5};
        std::vector<std::pair<int,int>> edges;
        std::vector<int> res = computeScores(r, edges);
        assert(res == std::vector<int>({0}));
    }
    // All equal strengths, many edges ignored
    {
        std::vector<int> r = {4, 4, 4};
        std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {0,2}};
        std::vector<int> res = computeScores(r, edges);
        assert(res == std::vector<int>({0, 0, 0}));
    }
    return 0;
}
