// Write a C++ function `intersectionLength` that takes four integers `a, b, c, d` representing two closed intervals `[a, b]` and `[c, d]` (with `a <= b` and `c <= d` guaranteed by the caller), and returns the length of their intersection. The length of the intersection is defined as `end - start` where `start` is the larger of the two lower bounds and `end` is the smaller of the two upper bounds, but if the intervals do not overlap (i.e., `start >= end`), the length is `0`. Input integers can be negative, and the intervals may be single points (e.g., `[3, 3]`). The function must not read from standard input; it must compute the result purely from its arguments. Use `long long` internally to avoid overflow when computing differences (since inputs are `int` and the difference can exceed `INT_MAX` if bounds are extreme).
The core idea is to compute the overlap of two closed intervals. The intersection of `[a, b]` and `[c, d]` is `[max(a, c), min(b, d)]`. The length is `min(b, d) - max(a, c)` if this value is positive, otherwise the intervals do not overlap (they may touch at a point, giving length 0). Edge cases: (1) When one interval is completely left of the other, e.g., `[1, 2]` and `[3, 4]`, the start is 3 and end is 2, so `start > end`, result 0. (2) When they only touch at a point, e.g., `[1, 3]` and `[3, 5]`, start=3 and end=3, length 0 (since point overlap has zero length). (3) When one is fully inside the other, e.g., `[2, 4]` and `[1, 5]`, start=2 and end=4, length 2. The algorithm is straightforward: compute `start = max(a, c)` and `end = min(b, d)`, then return `(start < end) ? (end - start) : 0`. Since inputs are `int`, but the difference `end - start` could exceed `int` range (e.g., a = -2e9, c = -2e9, b = 2e9, d = 2e9), cast to `long long` before subtraction. Time complexity is O(1), space complexity O(1).
#include <algorithm>

// Returns the length of the intersection of closed intervals [a, b] and [c, d].
// Assumes a <= b and c <= d. Returns 0 if intervals do not overlap (including touching at a point).
long long intersectionLength(int a, int b, int c, int d) {
    int start = std::max(a, c);
    int end = std::min(b, d);

    if (start < end) {
        return static_cast<long long>(end) - static_cast<long long>(start);
    }
    return 0LL;
}
#include <cassert>

int main() {
    // Basic non-overlapping intervals
    assert(intersectionLength(1, 2, 3, 4) == 0);
    // Touching at a point
    assert(intersectionLength(1, 3, 3, 5) == 0);
    // Partial overlap
    assert(intersectionLength(1, 5, 3, 7) == 2);
    // One interval inside another
    assert(intersectionLength(2, 4, 1, 5) == 2);
    // Identical intervals
    assert(intersectionLength(1, 4, 1, 4) == 3);
    // Negative values
    assert(intersectionLength(-5, -1, -3, 0) == 2);
    // Single-point intervals
    assert(intersectionLength(3, 3, 2, 4) == 0); // point has zero length
    // Large extreme values (difference may exceed int range)
    assert(intersectionLength(-2000000000, 2000000000, -2000000000, 2000000000) == 4000000000LL);
    // Touching at exactly one point but lower bound equal upper bound of other
    assert(intersectionLength(-2, 0, 0, 3) == 0);
    // Completely disjoint with one interval to the right
    assert(intersectionLength(5, 10, -3, 4) == 0);
    // Overlap with negative and positive
    assert(intersectionLength(-10, 10, -5, 5) == 10);
    return 0;
}
