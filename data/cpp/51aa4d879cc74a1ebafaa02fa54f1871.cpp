/*
You are given an integer `n` (with `2 <= n <= 10^9`). There is a hidden integer `k` (the number of rows in an unknown grid), but you do not know it. You may ask at most `22` queries of the form: given a positive integer `x`, the judge returns the number of columns `c` such that `x * c >= n` and `c` is positive; more precisely, the query returns `Ceil(n, x)`, but if `Ceil(n, x) == 1` the actual returned value is `1` (never `0`). You need to find the minimum possible value of `h * w` over all pairs `(h, w)` of positive integers such that `h * w >= n`, and you can use the query function to learn about the hidden `n` indirectly. Write a C++ function `long long minArea(long long n)` that, without direct access to `n` (except as passed), simulates the query process exactly as described and returns the answer. The query function `query(x)` is given for free; it prints `"? x"` and reads the answer. Your solution must call at most `O(log n)` queries plus at most `n` extra queries, but for this task assume `n` is small enough that up to `2 * n` queries are allowed. The answer is the minimum of `h * w` over all positive integer `h, w` with `h * w >= n`.
*/
#include <algorithm>
#include <cmath>

// Return the minimum possible area h*w such that h*w >= n, for positive integers h,w.
long long minArea(long long n) {
    long long best = n; // at worst, 1*n = n
    long long limit = static_cast<long long>(std::sqrt(static_cast<double>(n))) + 1;
    for (long long h = 1; h <= limit; ++h) {
        long long w = (n + h - 1) / h; // Ceil(n / h)
        best = std::min(best, h * w);
        // Also consider w as the height: but that is symmetric and will be found when h reaches the smaller of the two.
        // However, to be safe, also check products when w is small relative to sqrt(n).
        if (w >= 1) {
            long long area_swap = w * h; // same as h*w
            best = std::min(best, area_swap);
        }
    }
    return best;
}
#include <cassert>

int main() {
    assert(minArea(1) == 1); // 1*1
    assert(minArea(2) == 2); // 1*2 or 2*1
    assert(minArea(3) == 3); // 1*3 or 3*1
    assert(minArea(4) == 4); // 2*2
    assert(minArea(5) == 6); // 2*3
    assert(minArea(6) == 6); // 2*3 or 3*2
    assert(minArea(7) == 8); // 2*4 or 4*2
    assert(minArea(8) == 9); // 3*3
    assert(minArea(9) == 9); // 3*3
    assert(minArea(10) == 10); // 2*5 or 5*2 or 10*1
    assert(minArea(12) == 12); // 3*4
    assert(minArea(13) == 14); // 2*7 or 7*2
    assert(minArea(100) == 100); // 10*10
    assert(minArea(101) == 102); // 6*17 = 102, or 10*11=110, better is 6*17? Actually 10*11=110, 2*51=102, 3*34=102 etc. best is 102.
    assert(minArea(1000000) == 1000000); // 1000*1000
}
// The key insight is that we can first discover a lower bound `L` for `n` by performing a binary search on the range `[0, 2^21 - 1]` using the query function. The query `Query(x)` returns `Ceil(n, x)`; if that equals `1`, it means `n <= x`, so we adjust the search. After `22` queries we obtain `L` such that `n = L + t` for some `0 <= t <= 21` (but since we subtract `n-2` later, the exact reconstruction is: after the loop, `L` satisfies `n - 1 <= L <= n`; we set `L -= n - 2` so that `L` becomes `2` when `n=2` and otherwise `L = 2` or `3`; actually the original code sets `L` to be `n-1` or `n` and then subtracts `n-2`, giving `L` around 2). For this task we can simplify: we know `n` is passed directly, so we do not need the query simulation; but to match the spirit, we can use the query to discover `n` approximately and then compute the answer. However, since the function receives `n` directly, we can compute the answer directly without any queries. The minimum of `h * w` over `h*w >= n` is achieved by setting `h` near `sqrt(n)` and `w = Ceil(n / h)`. More precisely, iterate `h` from 1 up to `sqrt(n)` (or up to `n`) and for each `h` compute `w = Ceil(n, h)`, then take the minimum of `h*w`. Since `h` and `w` are symmetric, we can iterate only up to `sqrt(n)` and also check `w` as the height. Edge case: `n` may be as large as `10^9`, so iteration up to `sqrt(n)` (about 31623) is fine. For each `h`, compute `w = (n + h - 1) / h` and update the answer. Also consider `w` as height, but by symmetry it is covered because when we iterate `h` up to `sqrt(n)`, the corresponding `w` is at least `sqrt(n)`, and the product is symmetric. Time complexity: `O(sqrt(n))`; space: `O(1)`.
