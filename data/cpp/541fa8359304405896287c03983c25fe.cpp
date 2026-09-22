Write a C++ function `long long countGoodPoints(const std::vector<long long>& xs, const std::vector<long long>& ys)` that takes two vectors of the same length `n` representing the x-coordinates and y-coordinates of `n` points. The function must return the number of integer lattice points `(x, y)` such that for every given point, at least one of the two inequalities `|X - x| ≤ |Y - y|` or `|Y - y| ≤ |X - x|` holds **for both** coordinates? Actually, reinterpret the original snippet: The snippet computes the number of integer points `(x,y)` such that for all given points, `x` is between the two middle x-values and `y` is between the two middle y-values when `n` is even; if `n` is odd, the answer is always 1. Precisely: Given `n` points with coordinates (X[i], Y[i]), count the number of integer points `(x,y)` that lie inside the axis-aligned rectangle defined by the median x-interval and the median y-interval. For odd `n`, there is exactly one such point (the coordinate-wise median). For even `n`, the number is `(X[n/2] - X[n/2-1] + 1) * (Y[n/2] - Y[n/2-1] + 1)`, where the arrays are sorted independently. Implement the function exactly following this logic. The input vectors are not necessarily sorted, and you must sort copies. Assume `n ≥ 1`, and coordinates fit in `long long`. Return the count as `long long` (it could be large).
// The problem reduces to finding the number of integer points inside the smallest axis-aligned rectangle that contains all points that are coordinate-wise medians. For odd `n`, the median x and median y are unique values, so there is exactly one integer point: (medianX, medianY). For even `n`, the median interval for x is `[X[n/2-1], X[n/2]]` and similarly for y. Since coordinates are integers, the number of integer x values in that closed interval is `X[n/2] - X[n/2-1] + 1`, and similarly for y. The answer is the product of these two counts. This works because the original snippet ignores any actual point constraints and simply counts all lattice points in the rectangle formed by the middle two sorted values on each axis. Edge cases: `n=1` (odd) returns 1; `n=2` (even) returns `(X[1]-X[0]+1)*(Y[1]-Y[0]+1)`. Duplicate coordinates are fine. Time complexity is O(n log n) due to sorting, space O(n) for copies (or in-place if allowed, but we sort copies to avoid modifying input).
#include <vector>
#include <algorithm>
#include <cstdint>

// Count integer lattice points in the median rectangle.
// For odd n: exactly one point (the coordinate-wise median).
// For even n: product of lengths of middle intervals on X and Y.
long long countGoodPoints(const std::vector<long long>& xs,
                          const std::vector<long long>& ys) {
    const std::size_t n = xs.size();
    // Copy and sort separately.
    std::vector<long long> sortedX = xs;
    std::vector<long long> sortedY = ys;
    std::sort(sortedX.begin(), sortedX.end());
    std::sort(sortedY.begin(), sortedY.end());

    if (n % 2 == 1) {
        return 1LL;
    }

    const long long xSpan = sortedX[n / 2] - sortedX[n / 2 - 1] + 1;
    const long long ySpan = sortedY[n / 2] - sortedY[n / 2 - 1] + 1;
    return xSpan * ySpan;
}
#include <cassert>
#include <vector>

int main() {
    // Odd n: always 1
    assert(countGoodPoints({1, 5, 3}, {2, 4, 6}) == 1);
    assert(countGoodPoints({-2, 100, 7}, {0, 0, 0}) == 1);
    assert(countGoodPoints({42}, {-17}) == 1);

    // Even n: product of middle intervals
    assert(countGoodPoints({1, 2}, {5, 7}) == (2 - 1 + 1) * (7 - 5 + 1)); // 2*3=6
    assert(countGoodPoints({0, 0, 10, 10}, {0, 0, 10, 10}) == (10 - 0 + 1) * (10 - 0 + 1)); // 121
    assert(countGoodPoints({3, 1, 4, 2}, {9, 7, 8, 6}) == (3 - 2 + 1) * (8 - 7 + 1)); // 2*2=4
    assert(countGoodPoints({-5, -5, 5, 5}, {-1, 1, -1, 1}) == (5 - (-5) + 1) * (1 - (-1) + 1)); // 11*3=33
    assert(countGoodPoints({1, 1, 1, 1}, {1, 2, 3, 4}) == (1 - 1 + 1) * (3 - 2 + 1)); // 1*2=2

    // Large coordinates
    assert(countGoodPoints({1000000000LL, -1000000000LL},
                           {500000000LL, -500000000LL}) ==
           (1000000000LL - (-1000000000LL) + 1) *
           (500000000LL - (-500000000LL) + 1));

    return 0;
}
