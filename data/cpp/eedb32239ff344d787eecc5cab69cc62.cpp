// Write a C++ function `int nonCollinearPoint(const std::vector<long long>& xs, const std::vector<long long>& ys)` that, given two equal-length vectors of integer coordinates (x, y) for `n ≥ 3` distinct points, returns the **index** (0-based) of a point that does **not** lie on the straight line determined by the first two points in the sorted-by-distance-from-first-point order. More precisely: sort all points by their squared Euclidean distance from point 0 (the first point). In that sorted order, let the smallest index be 0 (the first point itself). Among the remaining points, find the first point in this sorted order that is **not collinear** with point 0 and the second point in that sorted order (i.e., the point in position 1 of the sorted list). Return the original index (in the input vectors) of that third point. The input guarantees that not all points are collinear, so such a point always exists. Duplicate distances are possible; the sorting is stable in the sense that if two distances are equal, their relative order in the sorted list follows their original index order. The function must handle up to 100,000 points, with coordinates ranging from -10^9 to 10^9 (use `long long` for squared distances to avoid overflow).
The core idea is to first compute the squared distance of every point from the point at index 0, then sort the points by this distance. Since the first point itself has distance 0 and will always be the smallest, it will occupy position 0 in the sorted list. The point in position 1 is the closest point to the first point (ties broken by original index). We then need to find the first point in positions 2..n-1 that is not collinear with point[0] and point[1]. Collinearity is checked using the cross product: for points A, B, C, they are collinear iff `(B.x - A.x)*(C.y - A.y) == (B.y - A.y)*(C.x - A.x)`. Because the input guarantees not all points are collinear, such a point must exist. The algorithm performs one sort: `O(n log n)` time. Space is `O(n)` for storing the index array. Edge cases: n=3 already fine; duplicate distances are handled by stable sorting on index; overflow is avoided by using `long long` for the squared distance and for the cross product (since coordinates up to 1e9, differences up to 2e9, product up to 4e18, still within 64-bit signed range). The return value is the original index, not the sorted position.
#include <vector>
#include <algorithm>
#include <cstddef>

// Helper: squared Euclidean distance from point 0.
static long long squaredDistance(const std::vector<long long>& xs,
                                 const std::vector<long long>& ys,
                                 std::size_t idx) {
    long long dx = xs[idx] - xs[0];
    long long dy = ys[idx] - ys[0];
    return dx * dx + dy * dy;
}

// Return the original index of a point that is not collinear
// with point[0] and the point closest to point[0] (by distance, ties by index).
int nonCollinearPoint(const std::vector<long long>& xs,
                      const std::vector<long long>& ys) {
    std::size_t n = xs.size();
    std::vector<std::size_t> order(n);
    for (std::size_t i = 0; i < n; ++i) order[i] = i;

    // Stable sort by squared distance from point 0.
    std::stable_sort(order.begin(), order.end(),
        [&](std::size_t i, std::size_t j) {
            return squaredDistance(xs, ys, i) < squaredDistance(xs, ys, j);
        });

    // Positions 0 and 1 are the first point and the closest other point.
    std::size_t a = order[0];
    std::size_t b = order[1];

    // Find the first later point not collinear with a and b.
    for (std::size_t k = 2; k < n; ++k) {
        std::size_t c = order[k];
        long long dx_b = xs[b] - xs[a];
        long long dy_b = ys[b] - ys[a];
        long long dx_c = xs[c] - xs[a];
        long long dy_c = ys[c] - ys[a];
        if (dx_b * dy_c != dy_b * dx_c) {
            return static_cast<int>(c);
        }
    }
    // Should never reach here because input guarantees non-collinear points.
    return -1;
}
#include <cassert>
#include <vector>

int nonCollinearPoint(const std::vector<long long>& xs,
                      const std::vector<long long>& ys);

int main() {
    // Test 1: Simple square
    std::vector<long long> xs1 = {0, 1, 0, 1};
    std::vector<long long> ys1 = {0, 0, 1, 1};
    int r1 = nonCollinearPoint(xs1, ys1);
    assert(r1 >= 0 && r1 < 4);
    // Verify returned point is not collinear with points 0 and the closest.
    // Distances: p0=0, p1=1, p2=1, p3=2. Sorted order: p0, p1, p2, p3.
    // p0 and p1 form horizontal line y=0. p2 (0,1) is not on it. So answer 2.
    assert(r1 == 2);

    // Test 2: Points on a line except one
    std::vector<long long> xs2 = {0, 1, 2, 5};
    std::vector<long long> ys2 = {0, 0, 0, 1};
    // Distances from p0: p0=0, p1=1, p2=4, p3=26. Sorted: p0,p1,p2,p3.
    // p1 and p2 are collinear with p0 (y=0). p3 is first non-collinear. Answer 3.
    assert(nonCollinearPoint(xs2, ys2) == 3);

    // Test 3: Duplicate distances, stable order required
    std::vector<long long> xs3 = {0, 1, -1, 0};
    std::vector<long long> ys3 = {0, 0, 0, 2};
    // Distances: p0=0, p1=1, p2=1, p3=4. Sorted stable: p0, p1, p2, p3.
    // p1, p2 are collinear with p0 (x-axis). p3 (0,2) not. Answer 3.
    assert(nonCollinearPoint(xs3, ys3) == 3);

    // Test 4: Already non-collinear third point
    std::vector<long long> xs4 = {0, 2, 1};
    std::vector<long long> ys4 = {0, 0, 1};
    // Distances: p0=0, p1=4, p2=2. Sorted: p0, p2, p1. Check p2 vs p0&p1? Actually sorted order: p0,p2,p1. p2 is non-collinear with p0 and p1? p0(0,0), p1(2,0), p2(1,1) -> non-collinear. Answer 2.
    assert(nonCollinearPoint(xs4, ys4) == 2);

    // Test 5: Larger collinear set, n=5, last point off line
    std::vector<long long> xs5 = {0, 10, 20, 30, 5};
    std::vector<long long> ys5 = {0, 0, 0, 0, 7};
    // Distances: p0=0, p4=25+49=74, p1=100, p2=400, p3=900. Sorted: p0,p4,p1,p2,p3.
    // p4 is non-collinear with p0 and p1? p0(0,0), p1(10,0), p4(5,7) -> non-collinear. Answer 4.
    assert(nonCollinearPoint(xs5, ys5) == 4);

    // Test 6: Negative coordinates
    std::vector<long long> xs6 = {-3, -2, -1, 0};
    std::vector<long long> ys6 = {1, 1, 2, 1};
    // Distances: p0=0, p1=1, p3=9+0=9? Actually p3(0,1): dx=3,dy=0->9. p2(-1,2): dx=2,dy=1->5. Sorted: p0,p1,p2,p3.
    // p1 is (-2,1) collinear with p0? dx=1,dy=0; p2(-1,2): dx=2,dy=1 cross=1*1-0*2=1 !=0 -> non-collinear. Answer 2.
    assert(nonCollinearPoint(xs6, ys6) == 2);

    // Test 7: All points with same distance from origin (circle) – stable order by original index
    std::vector<long long> xs7 = {0, 1, -1, 0};
    std::vector<long long> ys7 = {1, 0, 0, -1};
    // Distances: p0=1, p1=1, p2=1, p3=1. Stable sort keeps original order: p0,p1,p2,p3.
    // p0 and p1 collinear? p0(0,1), p1(1,0). p2(-1,0) is not collinear with them? cross = (1-0)*(-1-1) - (0-1)*(-1-0) = 1*(-2) - (-1)*(-1) = -2 - 1 = -3 !=0. Answer 2.
    assert(nonCollinearPoint(xs7, ys7) == 2);

    // Test 8: Large triangle
    std::vector<long long> xs8 = {0, 1000000000, -1000000000, 1};
    std::vector<long long> ys8 = {0, 0, 0, 1};
    // Distances: p0=0, p3=1+1=2, p1=1e18, p2=1e18. Sorted: p0,p3,p1,p2.
    // p3 (1,1) non-collinear with p0,p1? p0(0,0), p1(1e9,0), p3(1,1) cross = 1e9*1 - 0*1 = 1e9 !=0. Answer 3.
    assert(nonCollinearPoint(xs8, ys8) == 3);

    // Test 9: Three points, one exactly on the line between the first two but not collinear with the closest? Actually test collinearity correctly.
    std::vector<long long> xs9 = {0, 2, 4, 3};
    std::vector<long long> ys9 = {0, 0, 0, 1};
    // Distances: p0=0, p1=4, p2=16, p3=9+1=10. Sorted: p0,p1,p3,p2.
    // p1 and p3? p1(2,0), p3(3,1). Cross with p0: (2-0)*(1-0) - (0-0)*(3-0) = 2*1 - 0 = 2 !=0. So p3 is the answer. Index 3.
    assert(nonCollinearPoint(xs9, ys9) == 3);

    // Test 10: Large n, ensure no crash (simplified check for correctness)
    std::vector<long long> xs10(1000), ys10(1000);
    for (int i = 0; i < 1000; ++i) {
        xs10[i] = i;
        ys10[i] = 0;
    }
    xs10[500] = 500;
    ys10[500] = 1;
    int r10 = nonCollinearPoint(xs10, ys10);
    assert(r10 == 500);

    return 0;
}
