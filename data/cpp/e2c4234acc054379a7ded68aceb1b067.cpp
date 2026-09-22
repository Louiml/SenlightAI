// You are given \( n \) axis-aligned rectangles, each described by its 4 corner points (the corners can be given in any order). For each rectangle, compute its bounding box in two transformed coordinate systems: \( u = x + y \) and \( v = x - y \). Define the **overlap region** as the set of integer grid points \( (X, Y) \) that lie inside **every** rectangle simultaneously. A point is considered inside a rectangle if it is strictly inside the rectangle (not on the boundary). Write a C++ function `int countCommonInteriorPoints(const vector<vector<pair<int,int>>>& rects)` that, given the list of rectangles (each as a vector of 4 distinct corner points with integer coordinates), returns the number of integer grid points that are strictly inside all rectangles. All coordinates are within \([-10^9, 10^9]\), and the number of rectangles \( n \) satisfies \( 1 \le n \le 10^5 \). The bounding boxes of the rectangles in the \(u,v\) space must be used to determine the common interior efficiently (do not iterate over all possible points). The answer fits in a 64‑bit signed integer.

The key observation is that the transformation \( u = x + y, v = x - y \) is invertible: \( x = (u+v)/2, y = (u-v)/2 \). For an axis-aligned rectangle with corners \((x_1,y_1),(x_2,y_2),(x_3,y_3),(x_4,y_4)\), its image under this transformation is an axis-aligned rectangle in the \(uv\)-plane: \( u \) ranges from \( \min(u_i) \) to \( \max(u_i) \), and \( v \) ranges from \( \min(v_i) \) to \( \max(v_i) \). A point \((X,Y)\) is strictly inside the original rectangle iff both \( X+Y \) and \( X-Y \) are strictly inside the corresponding \(u\)- and \(v\)-intervals of that rectangle. Therefore, the common interior of all rectangles in the original plane corresponds exactly to the intersection of all \(u\)-intervals and all \(v\)-intervals from all rectangles: let \( U_{\min} \) be the maximum of all lower bounds of \(u\), \( U_{\max} \) the minimum of all upper bounds of \(u\), similarly \( V_{\min} \) and \( V_{\max} \) for \(v\). The valid integer points are those where \( U_{\min} < u < U_{\max} \) and \( V_{\min} < v < V_{\max} \) (strict because interior). Additionally, a point \((X,Y)\) is an integer point iff \( u+v \) is even and \( u-v \) is even; since \(u\) and \(v\) have the same parity (both even or both odd) for integer \(X,Y\). So the count is the number of pairs \((u,v)\) with \( U_{\min}+1 \le u \le U_{\max}-1 \), \( V_{\min}+1 \le v \le V_{\max}-1 \), and \(u \equiv v \pmod{2}\). Count valid \(u\) values: let \(L_u = U_{\min}+1, R_u = U_{\max}-1\); similarly \(L_v, R_v\). The number of pairs is: (count of even \(u\))×(count of even \(v\)) + (count of odd \(u\))×(count of odd \(v\)). If any interval is empty (i.e., \(L_u > R_u\) or \(L_v > R_v\)), the result is 0. Time complexity is \(O(n)\) (each rectangle has exactly 4 points, so constant factor), and space is \(O(1)\) auxiliary besides the input. Edge cases: rectangles may have zero width or height in the \(uv\)-plane (degenerate), but the strict inequality handles it; coordinates up to \(10^9\) require 64-bit arithmetic.

#include <bits/stdc++.h>

// Count integer points strictly inside the common intersection of all rectangles.
// Each rectangle is given by 4 corner points (order arbitrary).
long long countCommonInteriorPoints(const std::vector<std::vector<std::pair<int,int>>>& rects) {
    const long long INF = 4e18; // larger than any possible coordinate range
    long long minU = -INF, maxU = INF;
    long long minV = -INF, maxV = INF;

    for (const auto& corners : rects) {
        long long rMinU = INF, rMaxU = -INF;
        long long rMinV = INF, rMaxV = -INF;
        for (const auto& p : corners) {
            long long x = p.first, y = p.second;
            long long u = x + y;
            long long v = x - y;
            rMinU = std::min(rMinU, u);
            rMaxU = std::max(rMaxU, u);
            rMinV = std::min(rMinV, v);
            rMaxV = std::max(rMaxV, v);
        }
        // Intersect with current rectangle's u and v intervals
        minU = std::max(minU, rMinU);
        maxU = std::min(maxU, rMaxU);
        minV = std::max(minV, rMinV);
        maxV = std::min(maxV, rMaxV);
    }

    // Strict interior: u must be in (minU, maxU), v in (minV, maxV)
    long long lu = minU + 1, ru = maxU - 1;
    long long lv = minV + 1, rv = maxV - 1;

    if (lu > ru || lv > rv) return 0;

    // Count even and odd numbers in range [l, r]
    auto countEven = [](long long l, long long r) {
        if (l > r) return 0LL;
        long long total = r - l + 1;
        long long odd = 0;
        if (l % 2 != 0) odd = (r - l) / 2 + 1;
        else odd = (r - l + 1) / 2;
        return total - odd;
    };
    auto countOdd = [](long long l, long long r) {
        if (l > r) return 0LL;
        long long odd = 0;
        if (l % 2 != 0) odd = (r - l) / 2 + 1;
        else odd = (r - l + 1) / 2;
        return odd;
    };

    long long evenU = countEven(lu, ru);
    long long oddU = countOdd(lu, ru);
    long long evenV = countEven(lv, rv);
    long long oddV = countOdd(lv, rv);

    return evenU * evenV + oddU * oddV;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is assumed to be defined above in the same translation unit.
// Test cases.
int main() {
    // Single unit square: corners (0,0),(1,0),(1,1),(0,1)
    // Interior points are those with 0<X<1 and 0<Y<1: none.
    std::vector<std::vector<std::pair<int,int>>> r1 = {{{0,0},{1,0},{1,1},{0,1}}};
    assert(countCommonInteriorPoints(r1) == 0);

    // Large rectangle from (0,0) to (2,2) (corners in order)
    // Interior integer points: (1,1) only.
    std::vector<std::vector<std::pair<int,int>>> r2 = {{{0,0},{2,0},{2,2},{0,2}}};
    assert(countCommonInteriorPoints(r2) == 1);

    // Two overlapping rectangles both from (0,0) to (3,3) and (1,1) to (4,4)
    // Common interior: (2,2) only.
    std::vector<std::vector<std::pair<int,int>>> r3 = {
        {{0,0},{3,0},{3,3},{0,3}},
        {{1,1},{4,1},{4,4},{1,4}}
    };
    assert(countCommonInteriorPoints(r3) == 1);

    // Two rectangles that touch at boundary only: (0,0)-(2,2) and (2,0)-(4,2)
    // No common interior.
    std::vector<std::vector<std::pair<int,int>>> r4 = {
        {{0,0},{2,0},{2,2},{0,2}},
        {{2,0},{4,0},{4,2},{2,2}}
    };
    assert(countCommonInteriorPoints(r4) == 0);

    // Wide rectangle with odd width and height: (0,0) to (4,4) -> interior X=1,2,3 and Y=1,2,3 => 9 points
    std::vector<std::vector<std::pair<int,int>>> r5 = {{{0,0},{4,0},{4,4},{0,4}}};
    assert(countCommonInteriorPoints(r5) == 9);

    // Rectangle with negative coordinates: (-3,-3) to (-1,-1)
    // Interior point: (-2,-2) only.
    std::vector<std::vector<std::pair<int,int>>> r6 = {{{-3,-3},{-1,-3},{-1,-1},{-3,-1}}};
    assert(countCommonInteriorPoints(r6) == 1);

    // Single point rectangle (degenerate) - no interior.
    std::vector<std::vector<std::pair<int,int>>> r7 = {{{1,1},{1,1},{1,1},{1,1}}};
    assert(countCommonInteriorPoints(r7) == 0);

    // Multiple rectangles same area: two squares (0,0)-(2,2) and (0,0)-(2,2) -> interior (1,1) only.
    std::vector<std::vector<std::pair<int,int>>> r8 = {
        {{0,0},{2,0},{2,2},{0,2}},
        {{0,0},{2,0},{2,2},{0,2}}
    };
    assert(countCommonInteriorPoints(r8) == 1);

    // Large coordinates to verify 64-bit handling: rectangle from (-1e9,-1e9) to (1e9,1e9)
    // Interior integer points: X from -999999999 to 999999999 (1999999999 values), similarly Y,
    // total = (1999999999)^2.
    long long size = 1999999999LL;
    std::vector<std::vector<std::pair<int,int>>> r9 = {
        {{-1000000000,-1000000000},{1000000000,-1000000000},{1000000000,1000000000},{-1000000000,1000000000}}
    };
    assert(countCommonInteriorPoints(r9) == size * size);

    return 0;
}
