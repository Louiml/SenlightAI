// Write a C++ function that plays an interactive guessing game with a hidden "black/white" boundary. The game uses a coordinate plane where every point is either black or white, and the boundary is a single diagonal line of slope 1 (i.e., all points with `x <= y` are one color, all points with `x > y` are the other color, but the orientation may be reversed). Your function should take an integer `n` (the number of queries allowed, with the first query already made implicitly) and use an `ask(x, y)` callback that returns `true` if the point `(x, y)` is black, `false` if white. The goal is to find two points `(l, r)` and `(r, l)` such that the line `x = y` passes between them — specifically, you must output a pair of points `(a, b)` and `(c, d)` such that one is black and the other is white, and both lie on opposite sides of the boundary. The function should interactively query at most `n` points (including an initial query at `(1e9, 1e9)`) and return a pair of pairs: `{{l, r}, {r, l}}` where `l` and `r` are integers between 0 and 1e9, and the two points are on opposite sides of the boundary. The hidden boundary is guaranteed to have integer coordinates between 0 and 1e9, and the function may assume the `ask` callback is provided externally. The solution must use binary search on the diagonal `(x, x)` to find two adjacent diagonals where the color flips, then return the two points `(l, r)` and `(r, l)` accordingly.

// The core idea is to binary search along the line `y = x` because the boundary is exactly the line `x = y` (possibly with swapped colors). Consider points `(a, a)` for `a` from 0 to 1e9. The color of `(a, a)` is constant across all points on the diagonal, because the boundary condition `x <= y` is always true when `x == y` — but wait, the boundary is strictly `x > y` vs `x <= y`, so all diagonal points have the same color. That means binary searching on the diagonal is useless. Instead, we need to find a point just below the diagonal and a point just above it that have different colors. The trick is to query points `(x, x)` and also `(x+1, x)` or similar, but the snippet uses a different approach: it queries points `(mid, mid)` and compares with the color of `(1e9, 1e9)`. Actually, re-reading the snippet: it initializes `col = ask(r, r)` with `r=1e9`. Then for each iteration it queries `ask(mid, mid)` and compares to `col`. If different, it moves `l=mid`; else `r=mid`. After `n-1` iterations, it outputs `l r r l`. This works because the boundary is a diagonal line, and the color of `(x, x)` is not constant — wait, if the boundary is `x > y` then for `(x, x)`, `x == y`, so it's not `x > y`, so it should be one color always. The snippet seems to assume a different boundary: perhaps the boundary is a line with slope 1 but not passing through origin? The snippet's binary search on the diagonal finds the point where the diagonal crosses the boundary. So the correct interpretation: the hidden boundary is a line of the form `y = x + c` for some integer `c` between `-1e9` and `1e9`. Points on one side of this line are black, the other white. Then the color of `(x, x)` changes exactly when `x` crosses `c`. The binary search finds `l` and `r` such that `(l, l)` and `(r, r)` have different colors, and since the boundary is diagonal, `(l, r)` and `(r, l)` will also straddle the boundary. In fact, the correct output is `(l, r)` and `(r, l)` because these two points are symmetric across the diagonal and will be on opposite sides of the boundary if `l` and `r` are on opposite sides. The algorithm: query `(1e9, 1e9)` to get a reference color. Then binary search on the diagonal: maintain `[l, r]` where `l` is a point known to have a different color from the reference (or same?) – actually the initial `l=0` and `r=1e9`. The reference color `col` is the color of `(1e9, 1e9)` which is at the right end. Then for each `mid`, query `(mid, mid)`. If its color differs from `col`, that means `mid` is on the opposite side of the boundary relative to `1e9`, so the boundary is to the right of `mid`? Actually if `col` is the color at `1e9`, and the boundary is a diagonal line, then as `x` decreases, the color flips exactly once. So binary search: if `ask(mid, mid) != col`, then the boundary is between `mid` and `1e9`, so set `l = mid`; else set `r = mid`. After `n-1` queries, `l` and `r` are adjacent (difference 1) and have different colors. Then output `(l, r)` and `(r, l)`. This works because `(l, r)` and `(r, l)` are on opposite sides of the diagonal. Edge cases: when `n=1`? The input `n` is the total number of queries allowed, but the snippet does one initial query plus `n-1` more, so if `n=1` it would only do the initial query and then output `(0,1e9)` and `(1e9,0)` – but that might not be valid since we don't know the boundary. However, the task likely has `n>=2` or the guarantee is that the boundary is between 0 and 1e9, so after the initial query we still need at least one binary search step. The time complexity is O(log 1e9) ≈ 30 queries per binary search, but `n` is given, so we must use exactly `n-1` more queries. The space complexity is O(1). Important edge case: if the boundary is exactly at the diagonal? But the boundary is a line with slope 1, so it either passes through the diagonal or is parallel to it? Actually a line `y = x + c` is parallel to the diagonal `y = x`. So points on the diagonal `(x,x)` are either all on one side (if `c != 0`) or all on the boundary (if `c=0`, but then all diagonal points are on the line, which might be considered one color? The problem likely ensures the boundary does not pass through any queried point, or that `c` is odd/even to avoid ties? The snippet's binary search relies on exactly one flip on the diagonal. That requires the boundary line to intersect the diagonal at a non-integer point, so that the colors of `(x,x)` change exactly once as `x` goes from 0 to 1e9. The problem likely guarantees that the boundary is `x + y < c` or something else? Let’s reconsider: The snippet is from an interactive problem where you have to find the boundary between black and white cells on a grid. The boundary is a diagonal line from `(0,1e9)` to `(1e9,0)`? Actually the output `l r r l` suggests two points on the line `y = -x + (l+r)`? No, `(l,r)` and `(r,l)` are symmetric about `y=x`. The binary search on the diagonal finds the point where `(x,x)` changes color, which is the intersection of the boundary with the diagonal. The boundary could be any line that separates the plane into two half-planes, but the snippet specifically works when the boundary is a line with slope -1? Because `(l,r)` and `(r,l)` are on opposite sides of the line `y = x`? Actually if the boundary is the line `y = x + c`, then for a point `(a,b)`, the sign of `b - a - c` determines color. For `(l,r)`, the expression is `r - l - c`; for `(r,l)`, it's `l - r - c`. These have opposite signs because one is `d - c` and the other is `-d - c` where `d = r-l`. If `d > |c|`, they are opposite. In the binary search, we ensure `l` and `r` are adjacent and have different colors on the diagonal, meaning `c` is between `l` and `r` (non-integer). Then `r-l=1`, and `c` is strictly between `l` and `r`, so `c` is not an integer. Then `(l,r)` gives `r - l - c = 1 - c` and `(r,l)` gives `l - r - c = -1 - c`. Since `c` is between `l` and `r` (integers) and not integer, `c` is a real number with `l < c < r = l+1`. Then `1-c > 0` and `-1-c < 0` (since `c > -1`), so they are opposite signs. So correct. So the boundary is any line of slope 1 (i.e., `y = x + c`) with `c` a real number not an integer, between `0` and `1e9 - 1`? Actually the binary search range is `[0, 1e9]` for the diagonal. The initial query at `(1e9,1e9)` gives color at that point. The other end `(0,0)` might be same or different. The binary search finds the integer `l` such that `(l,l)` and `(l+1,l+1)` have different colors. Then `c` is between `l` and `l+1`. So the task is to implement a function that, given an integer `n` and a callback `ask`, returns a pair of pairs. The callback is provided as a function object. The function must use exactly `n-1` queries after an initial query. If `n` is too small, it may fail, but the problem guarantees `n` is large enough (e.g., 30). We'll design the function signature: `std::pair<std::pair<int,int>, std::pair<int,int>> findDiagonalBoundary(int n, const std::function<bool(int,int)>& ask)`. The function performs the initial query at `(1e9,1e9)`, then binary searches. Edge cases: if `n` is 1, we cannot complete. But we'll assume `n>=2`. The time complexity is O(n) queries, each O(1). Space O(1).

#include <functional>
#include <utility>

// Given a hidden diagonal boundary (line y = x + c, with c non-integer),
// use at most n queries to find two points (l,r) and (r,l) that lie on
// opposite sides of the boundary. The callback ask(x,y) returns true if
// the point is black, false if white. We binary search on the diagonal.
std::pair<std::pair<int,int>, std::pair<int,int>> findBoundary(
    int n,
    const std::function<bool(int,int)>& ask)
{
    const int MAX_COORD = 1000000000;

    // Initial query at the far right end of the diagonal.
    bool colRight = ask(MAX_COORD, MAX_COORD);

    int l = 0;
    int r = MAX_COORD;

    // We already used one query (above). Use the remaining n-1 queries.
    for (int i = 0; i < n - 1; ++i) {
        int mid = l + (r - l) / 2;
        bool colMid = ask(mid, mid);
        if (colMid != colRight) {
            // The color changed, so the boundary is to the right of mid.
            l = mid;
        } else {
            // Same as the right end, so the boundary is to the left of mid.
            r = mid;
        }
    }

    // Now l and r are adjacent integers with different colors on the diagonal.
    // Return the two points symmetric across the diagonal; they are on opposite sides.
    return {{l, r}, {r, l}};
}

#include <cassert>
#include <functional>
#include <utility>

// Solution function (paste here for testing)
// ...

int main() {
    // Simulate a vertical boundary at c = 500000000.5
    double c = 500000000.5;
    auto ask = [c](int x, int y) -> bool {
        // Black if x > c (strictly greater)
        return static_cast<double>(x) > c;
    };

    // Test with n = 31 (enough to binary search 1e9)
    auto result = findVerticalBoundary(31, ask);
    int l = result.first.first;  // l
    int r = result.first.second; // r

    // The two points should be on opposite sides: (l,r) has x=l, (r,l) has x=r
    assert(static_cast<double>(l) <= c && c < static_cast<double>(r));
    assert(r - l == 1);
    assert(ask(l, r) != ask(r, l)); // opposite colors

    // Test with boundary near left edge
    double c2 = 0.5;
    auto ask2 = [c2](int x, int y) -> bool { return static_cast<double>(x) > c2; };
    auto result2 = findVerticalBoundary(31, ask2);
    int l2 = result2.first.first;
    int r2 = result2.first.second;
    assert(l2 == 0 && r2 == 1);
    assert(ask2(l2, r2) != ask2(r2, l2));

    // Test with boundary near right edge
    double c3 = 999999999.5;
    auto ask3 = [c3](int x, int y) -> bool { return static_cast<double>(x) > c3; };
    auto result3 = findVerticalBoundary(31, ask3);
    int l3 = result3.first.first;
    int r3 = result3.first.second;
    assert(l3 == 999999999 && r3 == 1000000000);
    assert(ask3(l3, r3) != ask3(r3, l3));
}
