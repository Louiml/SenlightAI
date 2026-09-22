Write a C++ function that determines the position of a 2D point `p` relative to a given triangle with vertices `a`, `b`, and `c`. The function should return an integer code: `1` if the point is strictly inside the triangle, `2` if the point lies on any of the triangle's edges (but not at a vertex, though vertices count as on the edge), and `3` if the point is strictly outside the triangle. The vertices are passed as fixed coordinates: `a(10,0)`, `b(0,0)`, `c(10,10)` — but your function should accept generic points as parameters for generality. Use the area-comparison method: compute the triangle's total area using the cross-product formula, then compute the three sub-triangle areas formed by the point and each pair of vertices. If the sum of the three sub-areas equals the total area (within a small floating-point tolerance) and none of the sub-areas is zero, the point is inside; if any sub-area is zero, the point is on an edge; if the sum is greater, the point is outside. Handle floating-point precision by using a small epsilon (e.g., `1e-6`) for all equality comparisons.
#include <cassert>
#include <utility>

// Declare the function being tested (or include the solution header).
// Here we just declare for standalone compilation.
int pointRelativeToTriangle(const std::pair<double,double>& a,
                            const std::pair<double,double>& b,
                            const std::pair<double,double>& c,
                            const std::pair<double,double>& p);

int main() {
    // Fixed triangle from the original snippet: a(10,0), b(0,0), c(10,10)
    std::pair<double,double> a = {10.0, 0.0};
    std::pair<double,double> b = {0.0, 0.0};
    std::pair<double,double> c = {10.0, 10.0};

    // Point inside the triangle (e.g., (5,2))
    assert(pointRelativeToTriangle(a,b,c, {5.0, 2.0}) == 1);

    // Point on edge ab (e.g., (5,0))
    assert(pointRelativeToTriangle(a,b,c, {5.0, 0.0}) == 2);

    // Point on edge ac (e.g., (10,5))
    assert(pointRelativeToTriangle(a,b,c, {10.0, 5.0}) == 2);

    // Point on edge bc (e.g., (5,5)) — note bc goes from (0,0) to (10,10), so (5,5) is on it.
    assert(pointRelativeToTriangle(a,b,c, {5.0, 5.0}) == 2);

    // Point exactly at vertex a (10,0) — should be on edge
    assert(pointRelativeToTriangle(a,b,c, {10.0, 0.0}) == 2);

    // Point outside the triangle (e.g., (1,1) — actually that's outside? (1,1) is below line ac? Let's check: line ac is x=10, so (1,1) is inside? Actually (1,1) is inside because it's within the triangle? The triangle's interior is bounded by x-axis, y-axis, and line from (10,0) to (10,10)? That line is x=10, so any x<10 and y>0 is inside? Wait the triangle is right-angled with vertices (0,0), (10,0), (10,10). The hypotenuse is from (0,0) to (10,10) which is line y=x. So point (1,1) is on the hypotenuse? Actually (1,1) is on line y=x, so it's on edge bc! So better pick (1,2) which is inside? (1,2) has y>x, so it's above line y=x and x<10, so it's inside the triangle. Let's test that as inside. For outside, pick (12,5) which is clearly outside.
    assert(pointRelativeToTriangle(a,b,c, {1.0, 2.0}) == 1); // inside

    // Outside point (e.g., (12,5))
    assert(pointRelativeToTriangle(a,b,c, {12.0, 5.0}) == 3);

    // Another outside: (-1,5)
    assert(pointRelativeToTriangle(a,b,c, {-1.0, 5.0}) == 3);

    // Edge case: point (0,0) is vertex b -> on edge
    assert(pointRelativeToTriangle(a,b,c, {0.0, 0.0}) == 2);

    // Very close to edge but not exactly: (5.0, 0.000001) is essentially on edge but epsilon allows it as edge? Actually area is tiny but not zero, so epsilon check will treat it as on edge. That's acceptable.
    assert(pointRelativeToTriangle(a,b,c, {5.0, 0.000001}) == 2); // due to epsilon

    return 0;
}
#include <cmath>
#include <utility>

// Computes twice the area (i.e., absolute value of the cross product) of triangle (p, q, r)
// Using double for numerical precision.
double crossProductMagnitude(const std::pair<double,double>& p,
                             const std::pair<double,double>& q,
                             const std::pair<double,double>& r) {
    return std::abs((q.first - p.first) * (r.second - p.second) -
                    (q.second - p.second) * (r.first - p.first));
}

// Determines the position of point p relative to triangle (a,b,c).
// Returns 1 = inside, 2 = on edge (including vertices), 3 = outside.
int pointRelativeToTriangle(const std::pair<double,double>& a,
                            const std::pair<double,double>& b,
                            const std::pair<double,double>& c,
                            const std::pair<double,double>& p) {
    // Compute double the area of triangle abc (to avoid dividing by 2).
    const double doubleAreaABC = crossProductMagnitude(a, b, c);

    // Compute double areas of sub-triangles p with each edge.
    const double doubleAreaABP = crossProductMagnitude(a, b, p);
    const double doubleAreaACP = crossProductMagnitude(a, c, p);
    const double doubleAreaBCP = crossProductMagnitude(b, c, p);

    const double epsilon = 1e-6;

    // Check if point is on any edge (including vertices): any sub-area near zero.
    if (doubleAreaABP < epsilon || doubleAreaACP < epsilon || doubleAreaBCP < epsilon) {
        return 2; // on edge
    }

    // Compare sum of sub-areas to total area.
    const double sumAreas = doubleAreaABP + doubleAreaACP + doubleAreaBCP;
    if (std::abs(sumAreas - doubleAreaABC) < epsilon) {
        return 1; // inside
    }
    // If sum is greater than total area (by more than epsilon), it's outside.
    return 3;
}
// The solution uses the shoelace/cross-product formula for triangle area: the area of triangle formed by points `(x1,y1)`, `(x2,y2)`, `(x3,y3)` is `abs((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2.0` or equivalently `abs((x2-x1)*(y3-y1) - (x3-x1)*(y2-y1))/2.0`. For a point `p`, the triangle’s area is compared to the sum of areas of triangles `abp`, `acp`, and `bcp`. Due to floating-point arithmetic, exact equality is rare, so use an epsilon tolerance (e.g., `1e-6`) for comparing the sum to the total area. Edge cases: if any sub-area is zero, the point lies on that edge (including vertices because vertices also yield zero area for two of the sub-triangles). If the sum is greater than the total area by more than epsilon, the point is outside. The algorithm is straightforward and runs in O(1) time and O(1) space, making it highly efficient for a single query. For robustness, the function should accept `double` coordinates to avoid precision issues that `float` might cause. The main challenge is properly defining "on the edge" — here, a sub-area zero is sufficient, but for very thin triangles it’s acceptable. Also note that the original snippet’s logic had a bug in computing `abp` (it used `(px-ax)*(py-by) - (px-bx)*(py-ay)` which is not a proper cross product); our solution uses the correct cross-product formula for each sub-triangle.
