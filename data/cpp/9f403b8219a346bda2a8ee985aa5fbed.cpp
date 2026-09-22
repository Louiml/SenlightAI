/*
You are given `n` stars on a 100×100 grid (coordinates 1..100). Each star has an initial brightness `s` (0 ≤ s ≤ c). At time `t` (integer), the brightness of a star that initially had brightness `s` becomes `(s + t) mod (c+1)`. You must answer `q` queries: for a query with time `t` and rectangle `(r1, c1)` to `(r2, c2)` inclusive, output the sum of brightnesses of all stars inside that rectangle at that exact time. All times and rectangle corners are integers. Write a C++ function `int totalBrightness(int n, int c, const std::vector<Star>& stars, int q, const std::vector<Query>& queries)` where `Star` holds `x, y, s` and `Query` holds `t, r1, c1, r2, c2`. Return a vector of answers (one per query) in order. The grid is fixed at size 100×100, and coordinates are 1-indexed. Assume all inputs are valid, and `c` is non‑negative.
*/
#include <vector>
#include <cstddef>

struct Star {
    int x, y, s;
};

struct Query {
    int t, r1, c1, r2, c2;
};

// Returns answers for all queries on a 100x100 grid.
std::vector<int> totalBrightness(int n, int c,
                                const std::vector<Star>& stars,
                                int q,
                                const std::vector<Query>& queries) {
    const int GRID = 100;
    int period = c + 1;

    // table[k][x][y] holds sum of brightness contributions for time residue k.
    // Dimensions: period x (GRID+1) x (GRID+1), 1-indexed.
    std::vector<std::vector<std::vector<int>>> table(
        period, std::vector<std::vector<int>>(GRID + 1, std::vector<int>(GRID + 1, 0)));

    // Insert each star's contribution for every possible time residue.
    for (const auto& star : stars) {
        for (int k = 0; k < period; ++k) {
            int brightness_at_time_k = (star.s + k) % period;
            table[k][star.x][star.y] += brightness_at_time_k;
        }
    }

    // Build 2D prefix sums for each residue table.
    for (int k = 0; k < period; ++k) {
        for (int i = 1; i <= GRID; ++i) {
            for (int j = 1; j <= GRID; ++j) {
                table[k][i][j] += table[k][i - 1][j] + table[k][i][j - 1] - table[k][i - 1][j - 1];
            }
        }
    }

    // Answer each query.
    std::vector<int> answers;
    answers.reserve(q);
    for (const auto& query : queries) {
        int tmod = query.t % period;
        const auto& pref = table[tmod];
        int sum = pref[query.r2][query.c2]
                - (query.r1 > 1 ? pref[query.r1 - 1][query.c2] : 0)
                - (query.c1 > 1 ? pref[query.r2][query.c1 - 1] : 0)
                + (query.r1 > 1 && query.c1 > 1 ? pref[query.r1 - 1][query.c1 - 1] : 0);
        answers.push_back(sum);
    }

    return answers;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Single star, simple query at time 0 and time 1.
    {
        int n = 1, c = 2;
        std::vector<Star> stars = {{1, 1, 1}}; // s=1, c=2 => period 3
        int q = 2;
        std::vector<Query> queries = {
            {0, 1, 1, 1, 1}, // t=0 => brightness = (1+0)%3 = 1
            {1, 1, 1, 1, 1}  // t=1 => brightness = (1+1)%3 = 2
        };
        std::vector<int> ans = totalBrightness(n, c, stars, q, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 1);
        assert(ans[1] == 2);
    }

    // Test 2: Two stars overlap at same cell, sum them.
    {
        int n = 2, c = 1; // period 2
        std::vector<Star> stars = {{2, 3, 0}, {2, 3, 1}};
        int q = 2;
        std::vector<Query> queries = {
            {0, 2, 3, 2, 3}, // t=0 => s values: 0 and 1 => sum = 1
            {1, 2, 3, 2, 3}  // t=1 => s values: (0+1)%2=1, (1+1)%2=0 => sum = 1
        };
        std::vector<int> ans = totalBrightness(n, c, stars, q, queries);
        assert(ans[0] == 1);
        assert(ans[1] == 1);
    }

    // Test 3: Multiple stars in rectangle, verify with direct computation.
    {
        int n = 3, c = 4; // period 5
        std::vector<Star> stars = {{1, 1, 2}, {2, 2, 3}, {1, 2, 0}};
        int q = 2;
        std::vector<Query> queries = {
            {3, 1, 1, 2, 2}, // t=3: brightnesses: (2+3)%5=0, (3+3)%5=1, (0+3)%5=3 => sum=4
            {7, 2, 2, 2, 2}  // t=7%5=2: only star at (2,2) with s=3 => (3+2)%5=0
        };
        std::vector<int> ans = totalBrightness(n, c, stars, q, queries);
        assert(ans[0] == 4);
        assert(ans[1] == 0);
    }

    // Test 4: Rectangle that excludes all stars.
    {
        int n = 1, c = 0; // period 1
        std::vector<Star> stars = {{5, 5, 3}};
        int q = 1;
        std::vector<Query> queries = {{0, 1, 1, 4, 4}};
        std::vector<int> ans = totalBrightness(n, c, stars, q, queries);
        assert(ans[0] == 0);
    }

    // Test 5: Large time value, modulo works.
    {
        int n = 1, c = 3; // period 4
        std::vector<Star> stars = {{10, 10, 2}};
        int q = 1;
        std::vector<Query> queries = {{1003, 10, 10, 10, 10}}; // 1003%4=3 => (2+3)%4=1
        std::vector<int> ans = totalBrightness(n, c, stars, q, queries);
        assert(ans[0] == 1);
    }

    return 0;
}
// The key is to pre‑compute prefix sums per possible brightness modulo. Since brightness cycles with period `c+1`, we can store for each residue `k` (0 ≤ k ≤ c) a 2D prefix‑sum table over the grid. For every star, add its initial `s` (as a value, not count) at position `(x,y)` to every residue table, but only the table for residue `k` where the star’s brightness at time `t` equals `k`? Actually, the star's brightness at time `t` is `(s + t) mod (c+1)`. So for a given query time `t`, we only need the star’s contribution to the sum if its current brightness is known. A simpler approach: build a 3D array `mp[tmod][x][y]` where `tmod` is the time modulo `c+1` from 0 to `c`. For each star, for each possible time residue `j` (0..c), set `mp[j][x][y] += (s + j) % (c+1)`. But careful: the original code overwrites with assignment `=`, but we must sum multiple stars at same cell. So we should add, not assign. Then build 2D prefix sums for each time residue table. For a query with time `t`, let `tmod = t % (c+1)`. Then answer = sum over rows r1..r2 of `mp[tmod][row][c2] - mp[tmod][row][c1-1]`. Complexity: pre‑processing O(c * 100^2) for prefix sums, plus O(n*c) for star insertion (n ≤ ? but 100×100 grid, n could be large; we use `int` for sums, but total could be up to n*(c+1) which fits in int if n ≤ 1e5, c ≤ 10? Assume safe). Queries O(q * (r2-r1+1)). We can optimize queries with 2D prefix per residue: compute `prefix[k][x][y]` = sum of `mp[k][1..x][1..y]`. Then answer = sum over rows is O(rows), but we can also store 2D prefix and answer in O(1) per query: `prefix[tmod][r2][c2] - prefix[tmod][r1-1][c2] - prefix[tmod][r2][c1-1] + prefix[tmod][r1-1][c1-1]`. That is better. We'll implement that. Edge cases: stars may share coordinates; we sum. Coordinates start at 1, so index `0` rows/cols are zero. Time can be large; take modulo at query time. `c` could be zero – mod 1, so only one residue. Use `std::vector<std::vector<std::vector<int>>>` or a flat array. We'll return vector of ints.
