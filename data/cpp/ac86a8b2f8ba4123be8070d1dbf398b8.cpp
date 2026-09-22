/*
Given positive integers `k`, `n`, and a rectangular grid of size `k × n` that must be tiled entirely with domino-like tiles of dimensions either `1 × k` (horizontal) or `k × 1` (vertical), write a C++ function `int countTilings(int k, int n)` that returns the number of distinct tilings of the grid using exactly `n` tiles of type `1 × k` and exactly `k` tiles of type `k × 1`. The function must compute the count using a recursive memoized approach. Note that `k` and `n` are both positive and at most 100, and only grid sizes where at least one dimension is divisible by `k` can possibly be tiled; any invalid dimension combination should yield zero. The function must be self-contained, deterministic, and not rely on global mutable state other than a memoization table internal to the function.
*/
#include <vector>
#include <functional>

// Count distinct tilings of a k x n grid using 1xk and kx1 tiles.
// Returns the number of tilings (fits in int for reasonable k,n).
int countTilings(int k, int n) {
    if (k == 1) return 1; // Both tile orientations are 1x1, so only one tiling
    std::vector<int> memo(n + 1, -1);
    std::function<int(int)> solve = [&](int width) -> int {
        if (width < 0) return 0;
        if (width == 0) return 1;
        if (memo[width] != -1) return memo[width];
        memo[width] = solve(width - 1) + solve(width - k);
        return memo[width];
    };
    return solve(n);
}
#include <cassert>

int main() {
    assert(countTilings(2, 0) == 1);   // empty grid
    assert(countTilings(2, 1) == 1);   // one vertical tile
    assert(countTilings(2, 2) == 2);   // two verticals or one horizontal block
    assert(countTilings(2, 3) == 3);   // VVV, HHV, VHH
    assert(countTilings(2, 4) == 5);   // Fibonacci
    assert(countTilings(3, 4) == 4);   // compositions using 1 and 3
    assert(countTilings(3, 3) == 2);   // all vertical or one horizontal block
    assert(countTilings(1, 10) == 1);  // all 1x1 tiles, one tiling
    assert(countTilings(4, 1) == 1);   // only a vertical tile fits
    assert(countTilings(4, 5) == 2);   // either 5 verticals or 1 horizontal block + 1 vertical
    return 0;
}
// The grid has height `k` and width `n`. A vertical tile of size `k × 1` covers exactly one column of height `k`, so it occupies width 1. A horizontal tile of size `1 × k` covers exactly one row segment of length `k`, so to fill the full height `k`, we must place `k` such tiles stacked vertically, occupying a block of width `k`. Therefore, any tiling corresponds to a sequence of width segments: each segment is either a single column (using one vertical tile) or a block of `k` columns (using `k` horizontal tiles). Thus the number of tilings for width `n` follows the recurrence `ways[n] = ways[n-1] + ways[n-k]`, with base cases `ways[0] = 1` and `ways[n] = 0` for `n < 0`. If `k == 1`, both tile types are `1 × 1` and geometrically identical, so there is exactly one tiling regardless of `n`. This recurrence is a straightforward memoized recursion, giving O(n) time and O(n) space. Edge cases include `n = 0` (empty grid, one tiling), `k > n` (only vertical tiles possible, exactly one tiling), and `k = 1` (one tiling).
