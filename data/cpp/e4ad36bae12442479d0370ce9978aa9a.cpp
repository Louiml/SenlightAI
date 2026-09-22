Write a C++ function `findValidMultiples` that takes three integers `l`, `r`, and `s` as input, where `l` and `r` define a closed interval `[l, r]` (with `1 ≤ l ≤ r`), and `s` is a positive integer. The function must return a pair of integers `(p, q)` such that: `p` is the smallest integer in the interval `[l, r]` that is divisible by `s`, and `q` is the largest integer in the interval `[l, r]` that is divisible by `s`. If no integer in the interval is divisible by `s`, the function must return `(-1, -1)`. Ensure the solution handles large values (up to `10^18`) and is efficient. The function signature is `std::pair<long long, long long> findValidMultiples(long long l, long long r, long long s)`.
#include <cassert>
#include <utility>

// The solution function is declared above; include it here or link appropriately.
std::pair<long long, long long> findValidMultiples(long long, long long, long long);

int main() {
    // Basic cases
    assert(findValidMultiples(1, 10, 3) == std::make_pair(3LL, 9LL));
    assert(findValidMultiples(5, 5, 5) == std::make_pair(5LL, 5LL));
    assert(findValidMultiples(5, 5, 2) == std::make_pair(-1LL, -1LL));
    assert(findValidMultiples(7, 10, 2) == std::make_pair(8LL, 10LL));

    // Edge: l and r are the same
    assert(findValidMultiples(100, 100, 100) == std::make_pair(100LL, 100LL));
    assert(findValidMultiples(100, 100, 101) == std::make_pair(-1LL, -1LL));

    // Large values
    assert(findValidMultiples(1000000000000000000LL, 1000000000000000000LL, 1LL) == std::make_pair(1000000000000000000LL, 1000000000000000000LL));
    assert(findValidMultiples(999999999999999999LL, 1000000000000000000LL, 1000000000000000000LL) == std::make_pair(-1LL, -1LL));
    assert(findValidMultiples(999999999999999999LL, 1000000000000000000LL, 999999999999999999LL) == std::make_pair(999999999999999999LL, 999999999999999999LL));

    // l is exactly a multiple and r is exactly a multiple
    assert(findValidMultiples(12, 24, 6) == std::make_pair(12LL, 24LL));

    // Interval with only one multiple
    assert(findValidMultiples(10, 14, 7) == std::make_pair(14LL, 14LL));
    assert(findValidMultiples(10, 13, 7) == std::make_pair(-1LL, -1LL));

    return 0;
}
#include <utility>

// Find the smallest and largest multiples of s in the closed interval [l, r].
// Returns (-1, -1) if no multiple exists.
std::pair<long long, long long> findValidMultiples(long long l, long long r, long long s) {
    // Compute smallest multiple >= l safely without overflow.
    long long p = (l / s) * s;
    if (l % s != 0) {
        p += s; // safe because p < l + s, and l + s <= 2e18 < 9e18
    }

    // Compute largest multiple <= r.
    long long q = (r / s) * s;

    if (p > r) {
        return {-1, -1};
    }
    return {p, q};
}
// The task is to find the smallest and largest multiples of `s` that lie within the closed interval `[l, r]`. The smallest multiple of `s` that is at least `l` can be computed as `ceil(l / s) * s`. In integer division, `ceil(l/s)` equals `(l + s - 1) / s` when using integer arithmetic. Let `p = ((l + s - 1) / s) * s`. The largest multiple of `s` that is at most `r` is simply `q = (r / s) * s`. If `p > r`, then there is no multiple of `s` in the interval, so return `(-1, -1)`. Otherwise, return `(p, q)`. Edge cases: if `l` itself is divisible by `s`, then `p = l`; if `r` itself is divisible by `s`, then `q = r`; if the interval is very large and `s` is huge, the calculations still use only a few arithmetic operations. Time complexity is O(1) and space complexity is O(1). The main pitfall is avoiding overflow when computing `(l + s - 1)` — but with `l` and `s` up to `10^18`, their sum could exceed `long long` range (max ~9.22e18), so we should compute `ceil(l/s)` safely using `l / s` and `l % s` rather than adding `s-1` to `l`. Specifically, `p = (l / s) * s; if (l % s != 0) p += s;` This avoids addition overflow.
