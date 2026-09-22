/*
Write a C++ function `double maxAreaUnderLine(ll n, ll X, const std::vector<std::pair<ll, ll>>& points)` that, given a set of `n` vertical segments on the plane (each segment from `(x, 0)` to `(x, y)` with positive `y`), and a horizontal limit `X` (with `X` greater than or equal to the maximum `x` coordinate), computes the area of the region under the upper envelope of these segments and above the x-axis, from `x = 0` to `x = X`. The upper envelope is formed by taking, for each distinct `x`, the maximum `y` among all segments at that `x`. Between consecutive distinct x-coordinates, the envelope is linear (connecting the maximum points). For `x` values from 0 to the first segment's x, the envelope is 0. For `x` from the last segment's x to `X`, the envelope extends horizontally at the last maximum height. The input may contain multiple segments at the same `x` (take the tallest one), and segments are not necessarily sorted. The coordinates are up to `10^9`, so use `long long`. Return the area as a `double`, with precision at least 10 decimal places.
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Compute the area under the upper envelope of vertical segments from x=0 to x=X.
long double maxAreaUnderLine(ll n, ll X, const vector<pair<ll,ll>>& points) {
    // Compress: for each x, keep the maximum y.
    map<ll, ll> maxY;
    for (const auto& p : points) {
        ll x = p.first, y = p.second;
        maxY[x] = max(maxY[x], y);
    }

    long double area = 0.0L;
    ll prevX = 0, prevY = 0;
    bool first = true;

    for (const auto& kv : maxY) {
        ll x = kv.first, y = kv.second;
        if (first) {
            // Triangle from (0,0) to (x,y)
            area += 0.5L * static_cast<long double>(x) * y;
            first = false;
        } else {
            // Trapezoid from (prevX, prevY) to (x, y)
            area += 0.5L * static_cast<long double>(x - prevX) * (prevY + y);
        }
        prevX = x;
        prevY = y;
    }

    // Final rectangle from the last x to X
    if (!first) {
        area += static_cast<long double>(X - prevX) * prevY;
    }
    return area;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// assume the function is defined above

int main() {
    // Test 1: single point
    vector<pair<ll,ll>> p1 = {{3,4}};
    long double a1 = maxAreaUnderLine(1, 5, p1);
    assert(fabsl(a1 - 14.0L) < 1e-12L);

    // Test 2: two points increasing
    vector<pair<ll,ll>> p2 = {{1,2},{3,4}};
    long double a2 = maxAreaUnderLine(2, 5, p2);
    assert(fabsl(a2 - 15.0L) < 1e-12L);

    // Test 3: two points decreasing
    vector<pair<ll,ll>> p3 = {{1,4},{3,2}};
    long double a3 = maxAreaUnderLine(2, 4, p3);
    assert(fabsl(a3 - 10.0L) < 1e-12L);

    // Test 4: duplicate x take max
    vector<pair<ll,ll>> p4 = {{2,3},{2,5}};
    long double a4 = maxAreaUnderLine(2, 4, p4);
    assert(fabsl(a4 - 15.0L) < 1e-12L);

    // Test 5: unsorted input
    vector<pair<ll,ll>> p5 = {{5,2},{1,6}};
    long double a5 = maxAreaUnderLine(2, 6, p5);
    assert(fabsl(a5 - 21.0L) < 1e-12L);

    // Test 6: all same x
    vector<pair<ll,ll>> p6 = {{2,1},{2,1},{2,1}};
    long double a6 = maxAreaUnderLine(3, 3, p6);
    assert(fabsl(a6 - 2.0L) < 1e-12L);

    // Test 7: X equals last x
    vector<pair<ll,ll>> p7 = {{1,3},{2,5}};
    long double a7 = maxAreaUnderLine(2, 2, p7);
    assert(fabsl(a7 - 5.5L) < 1e-12L);

    // Test 8: point at x=0
    vector<pair<ll,ll>> p8 = {{0,2}};
    long double a8 = maxAreaUnderLine(1, 3, p8);
    assert(fabsl(a8 - 6.0L) < 1e-12L);

    // Test 9: large coordinate sanity
    vector<pair<ll,ll>> p9 = {{1000000000LL, 1000000000LL}};
    long double a9 = maxAreaUnderLine(1, 1000000000LL, p9);
    // area = 0.5*1e9*1e9 = 5e17
    assert(fabsl(a9 - 5e17L) < 1e-2L);

    return 0;
}

// The problem reduces to computing the area under a piecewise-linear function. First, compress the input by keeping, for each distinct x, the tallest segment height. This is naturally done with a `std::map<long long,long long>` which also sorts the x-coordinates. After compression, the envelope is a polyline starting at `(0,0)`, rising linearly to the first point `(x1, y1)`, then connecting consecutive points `(x_i, y_i)` and `(x_{i+1}, y_{i+1})` with straight lines, and finally extending horizontally from the last point `(x_k, y_k)` to `X`. The area is the sum of: a triangle from `0` to `x1` with area `0.5 * x1 * y1`; a trapezoid for each interval `[x_i, x_{i+1}]` with area `0.5 * (x_{i+1} - x_i) * (y_i + y_{i+1})`; and a rectangle from `x_k` to `X` with area `(X - x_k) * y_k`. If there is only one distinct x, the triangle and rectangle cover the whole area. Edge cases include duplicate x-coordinates (resolved by taking the max), unsorted input (handled by the map), and X possibly equal to the last x (the final rectangle area becomes zero). The time complexity is `O(n log n)` due to the map insertion, and the space complexity is `O(n)`. The use of `long double` ensures high precision for areas up to `1e18`.
