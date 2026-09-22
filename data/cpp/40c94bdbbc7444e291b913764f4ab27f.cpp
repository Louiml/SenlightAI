Write a C++ function `countGoodSegments` that takes a vector of pairs of integers `(l, r)` representing the start and end of `n` segments on a number line, and two integers `n` and `k`. The function must return the number of ways to choose exactly `k` segments that all share at least one common integer point, modulo 998244353. The segments are inclusive on both ends, and all coordinates are distinct for segment starts and ends (no two starts equal, no two ends equal, but a start can equal another segment's end). Each segment satisfies `l <= r`.

#include <cassert>
#include <vector>
#include <utility>

// Declaration of the function (import from solution).
long long countGoodSegments(const std::vector<std::pair<int,int>>& segments, int k);

int main() {
    // Case 1: 3 segments, each pair shares at least one point, choose 2.
    // [1,2], [2,3], [3,4] -> all pairs of segments share a point.
    {
        std::vector<std::pair<int,int>> segs = {{1,2}, {2,3}, {3,4}};
        assert(countGoodSegments(segs, 2) == 3);
    }

    // Case 2: 2 disjoint segments, choose 2 -> no common point.
    {
        std::vector<std::pair<int,int>> segs = {{1,1}, {5,5}};
        assert(countGoodSegments(segs, 2) == 0);
    }

    // Case 3: 1 segment, choose 1 -> 1 way.
    {
        std::vector<std::pair<int,int>> segs = {{10,20}};
        assert(countGoodSegments(segs, 1) == 1);
    }

    // Case 4: k > n -> 0.
    {
        std::vector<std::pair<int,int>> segs = {{1,2}, {2,3}};
        assert(countGoodSegments(segs, 3) == 0);
    }

    // Case 5: All segments start at 0, end at distinct positive values.
    // 4 segments, choose 3 -> all combinations of 3 from 4 = 4.
    {
        std::vector<std::pair<int,int>> segs = {{0,1}, {0,2}, {0,3}, {0,4}};
        assert(countGoodSegments(segs, 3) == 4);
    }

    // Case 6: Nested segments.
    // [1,10], [2,9], [3,8] -> choose 2 -> all pairs share points, choose 2 from 3 = 3.
    {
        std::vector<std::pair<int,int>> segs = {{1,10}, {2,9}, {3,8}};
        assert(countGoodSegments(segs, 2) == 3);
    }

    // Case 7: Negative coordinates.
    // [-5,-1], [-3,0], [-2,2] -> choose 2 -> all pairs share, 3 ways.
    {
        std::vector<std::pair<int,int>> segs = {{-5,-1}, {-3,0}, {-2,2}};
        assert(countGoodSegments(segs, 2) == 3);
    }

    // Case 8: Large n with all identical segments.
    // 5 segments [1,1] choose 2 -> C(5,2)=10.
    {
        std::vector<std::pair<int,int>> segs(5, {1,1});
        assert(countGoodSegments(segs, 2) == 10);
    }

    // Case 9: k=0 -> 1 (empty choice).
    {
        std::vector<std::pair<int,int>> segs = {{1,2}};
        assert(countGoodSegments(segs, 0) == 1);
    }

    // Case 10: Boundary events processed correctly.
    // [1,2] and [2,3] share point 2 exactly.
    {
        std::vector<std::pair<int,int>> segs = {{1,2}, {2,3}};
        assert(countGoodSegments(segs, 2) == 1);
    }

    return 0;
}

#include <bits/stdc++.h>

// Compute C(n, k) modulo MOD using precomputed factorials.
// Assumes factorials up to maxN have been precomputed.
long long chooseMod(int n, int k, const std::vector<long long>& fact, const std::vector<long long>& invFact) {
    if (k < 0 || k > n || n < 0) return 0;
    return fact[n] * invFact[k] % 998244353 * invFact[n - k] % 998244353;
}

// Count ways to choose k segments that all share at least one common integer point.
long long countGoodSegments(const std::vector<std::pair<int,int>>& segments, int k) {
    const long long MOD = 998244353;
    int n = (int)segments.size();
    if (k > n) return 0;
    if (k == 0) return 1;

    // Precompute factorials and inverse factorials up to n.
    std::vector<long long> fact(n + 1), invFact(n + 1);
    fact[0] = 1;
    for (int i = 1; i <= n; ++i) fact[i] = fact[i-1] * i % MOD;
    // Fermat's little theorem for modular inverse.
    auto modPow = [&](long long a, long long e) {
        long long res = 1;
        while (e > 0) {
            if (e & 1) res = res * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return res;
    };
    invFact[n] = modPow(fact[n], MOD - 2);
    for (int i = n; i > 0; --i) invFact[i-1] = invFact[i] * i % MOD;

    // Build events: (coordinate, type) where type=+1 for opening, -1 for closing.
    std::vector<std::pair<int,int>> events;
    events.reserve(2*n);
    for (const auto& seg : segments) {
        events.emplace_back(seg.first, 1);
        events.emplace_back(seg.second, -1);
    }
    // Sort by coordinate; if equal, opening (+1) goes first.
    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second > b.second;
    });

    long long ans = 0;
    int opened = 0;
    for (const auto& ev : events) {
        if (ev.second == 1) {
            ++opened;
            if (opened >= k) {
                ans = (ans + chooseMod(opened - 1, k - 1, fact, invFact)) % MOD;
            }
        } else {
            --opened;
        }
    }
    return ans;
}

// The key insight is to sweep along the number line and track how many segments are currently open (covering the current point). Process events in increasing coordinate order; when two events share the same coordinate, process "+1" (opening) events before "-1" (closing) events because the point at the boundary belongs to both segments. When a new segment opens and the number of open segments becomes `opened`, the newly opened segment can be combined with any `k-1` already-open segments to form a valid group. The number of ways to choose those `k-1` from `opened-1` is `C(opened-1, k-1)`. Summing this over every opening event gives the total count. Edge cases: if `k > n`, return 0; if `k == 0`, the answer is 1 (empty selection), though typically `k >= 1`; coordinates can be negative. Precompute factorials and inverse factorials up to maximum `n` to compute combinations in O(1). Time complexity: O(n log n) for sorting events, plus O(n) for sweep and O(n) for precomputation; space complexity O(n).
